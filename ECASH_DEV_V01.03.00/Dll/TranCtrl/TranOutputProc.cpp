/////////////////////////////////////////////////////////////////////////////
// TranOutput.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	수신후처리함수
/////////////////////////////////////////////////////////////////////////////
// 명세프린트
int CTranCmn::fnAPP_PSPProc(int PrintDevice, int WaitFlg)
{
	int		HiddenFlg = TRUE;
	int		i = 0, js = 0, jj = 0;
	int		ii = 0;

	int		nJnlIndex = 0;
	int		nSlipIndex = 0;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_PSPProc() Start");
/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)
	{
	}
/////////////////////////////////////////////////////////////////////////////

	if (PrintDevice & (DEV_SPR | DEV_JPR))
	{
		if (!m_pDevCmn->TranResult);							// HOST NG
		else
		if (!Accept.SlipChoiceFlag)								// 명세표생략
			PrintDevice &= ~DEV_SPR;

		if (PrintDevice == DEV_NONE)
			return T_OK;
	}

	memset(ShuData, 0, sizeof(ShuData));						// Shu출력영역

	if (PrintDevice & DEV_JPR) 
	{
		HiddenFlg = FALSE;										// 저널만인자시 기능삭제
	}

	for (ii = 0; ii < 35; ii++)
	{
		fnAPP_EditPrintLine(PrintDevice, ShuData[nSlipIndex], ii, HiddenFlg);		// 라인 Edit

		if(TranProc == TRAN_TICKET)
		{
			if (ShuData[nSlipIndex][0] == 0)
				sprintf(ShuData[nSlipIndex], SPACE16);

			nSlipIndex++;
		}
		else
		{
			if (ShuData[nSlipIndex][0] != 0)
				nSlipIndex++;
		}
	}

	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)
	{
		sprintf(ShuData[nSlipIndex++], "[로컬 거래 모드]");
	}

	if (PSPRecvData.ParaLength)							// 명세표자료유
	{
		for (i = PSPRecvData.By; i <= PSPRecvData.Ey; i++)
		{
			memcpy(&ShuData[nSlipIndex++], PSPRecvData.Data[i], itemof(PSPRecvData.Data[i]));
		}
	}

	for (ii = 51; ii < 100; ii++)
	{
		fnAPP_EditPrintLine(PrintDevice, ShuData[nSlipIndex], ii, HiddenFlg);		// 라인 Edit
		if (ShuData[nSlipIndex][0] != 0)
			nSlipIndex++;
	}

	if ((!m_pDevCmn->TranResult)	&&
		(TranProc == TRAN_DEP)		&&
		(CashData.mTotalAmount)		)
	{
		sprintf(ShuData[nSlipIndex++], "[입금부 반환처리]");
	}



	for (i = 0; i < nSlipIndex; i++)
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_PSPProc[%s]", &ShuData[i]);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_PSPProc() end");
	fnAPD_ShuPrint(PrintDevice, JNL_TRAN, WaitFlg);						// 명세표인자

	return T_OK;
}

int CTranCmn::fnAPP_PPPProc(int WaitFlg)
{
	if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
		fnAPP_IBK_PPPProc();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
		fnAPP_NONGHYUP_PPPProc();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KEB) //#0085
		fnAPP_KEB_PPPProc();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KWANGJU) //#N0181
		fnAPP_KJB_PPPProc();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == HANA) //#N0214
		fnAPP_HANA_PPPProc();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KB)	//#N0226
		fnAPP_KB_PPPProc();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == CITI) //#N0214#N0252
		fnAPP_CTB_PPPProc();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == SC)	//#N0245
		fnAPP_SC_PPPProc();
	else
	if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM)	//#N0277
		fnAPP_KNB_PPPProc();
		
	
	return T_OK;
}

#define	PB_MAX_FRAME	1024
// 통장부프린트
int CTranCmn::fnAPP_NONGHYUP_PPPProc(int WaitFlg)
{
	int		i,j,k;
	int		startline, endline, szPbLen;
	int		nEndPage = FALSE;

// ================= Local Test ==================================	// V06-00-04-#01, 로컬의 경우 통장 1Line 찍도록 변경
	static   int    LocalLine = 0;				                    // V06-00-05-#01, 로컬 통장정리의 경우 2Page 찍고 Turn Page 하도록 변경.
	int             LocalPage = 0;
	int             LocalI = 0;
	int				TurnCheck = FALSE;

	if ((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&
	(PbTran))
	{
		PPPRecvDataNon.p = 0;
		memset(PPPRecvDataNon.Data, 0x00, sizeof(PPPRecvData.Data));	

		if (TranCode == TC_PBOOK)								// 통장정리
		{
			LocalLine = 0;
			LocalI = 47;
		}
		else 
		{
			LocalI = 0;
			if (LocalLine == 0)
				LocalLine = 1;
		}

		for(int   cnt = 0; cnt <= LocalI; cnt++) 
		{
			if ((LocalLine == 0) && (TranCode == TC_PBOOK))
			{
				Strcpy(PPPRecvDataNon.Data[LocalPage][LocalLine], 
				"         농협인    자유입출금     1234-56-78910  년05.000%");
			}
			else
			{
				if ((TranCode != TC_PBOOK) && (LocalLine >= 24))
				{
					LocalLine = 0;
					TurnCheck = TRUE;
					Strcpy(PPPRecvDataNon.Data[LocalPage][LocalLine], 
					"20020202 ATM IN/OUT   *123,456       *123,456 *123,456,789 농협");
				}
				else
				{
					Strcpy(PPPRecvDataNon.Data[LocalPage][LocalLine], 
					"20020202 ATM IN/OUT   *123,456       *123,456 *123,456,789 농협");
				}
			}

			LocalLine++;

			if (TranCode == TC_PBOOK)
			{
				if (LocalLine >= 24)
				{
					LocalLine = 0;

					if (cnt != LocalI)
					{
						if ((m_pDevCmn->AtmDefine.PBHandler == PBM_SET)		||
							(m_pDevCmn->AtmDefine.PBHandler == I_PBM_SET)	)
						{
							LocalPage++;
							PPPRecvDataNon.p = LocalPage;
						}
						else break;
					}
				}
			}
			else
			{
				if (LocalLine > 24)
				{
					memset(PPPRecvDataNon.Data, 0x00, sizeof(PPPRecvDataNon.Data));

					LocalLine = 0;
					TurnCheck = TRUE;
					Strcpy(PPPRecvDataNon.Data[LocalPage][LocalLine], 
					"20020202 ATM IN/OUT   *123,456       *123,456 *123,456,789 농협");
				}
			}
		}
	}

// ================= Local Test ==================================

	if ((!IsSpace(PPPRecvDataNon.Data, sizeof(PPPRecvDataNon.Data))) &&
		(!IsNull(PPPRecvDataNon.Data, sizeof(PPPRecvDataNon.Data))))
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_PPPProc start");
	}
	else
		return T_OK;

	memset(PbData, 0, sizeof(PbData));							// Pb출력영역
	PbDataCnt = 0;
	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);

	if((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&	(PbTran))		//V06-00-05-#01
	{
		if(TurnCheck == TRUE)
		{
			startline = LocalLine+1;
			TurnCheck = FALSE;
		}
		else
			startline = LocalLine;
	}
	else
		startline = Asc2Int(&PPPRecvDataNon.SkipLineCnt[0], 2);
	// 선두행 찾기 - 데이터가 NULL이면 라인피드를 한다.
	for(i=0; i < startline; i++) 
	{
		if (!strlen((char *)PPPRecvDataNon.Data[0][i]))
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (i == 11)	// 재봉선 skip
		{
			PbData[PbDataCnt++] = PbNewLine;
			PbData[PbDataCnt++] = PbNewLine;
		}
	}
	
	startline = i;


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "인자할페이지[%d] 인자시작라인[%d]",PPPRecvData.p, startline);
//MsgDumpAnal(TRACE_CODE_MODE, "Log", "인자할페이지[%d]", PPPRecvData.p);

	// 마지막 검지 조건 
	// PPPRecvData.p가 마지막 데이터가 있는 페이지의 번호이다.
	for (i=0; i <= PPPRecvDataNon.p; i++)
	{
		endline = 0;
		
		for(k=startline; k < 24; k++)
		{
			if ((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&	(PbTran) && (TranCode == TC_PBOOK))		// V06-00-05-#01
			{
				endline = k;
			}
			else
			{
				if (PPPRecvDataNon.Data[i][k][0] == 0x00)
				{
					endline = k;
					break;
				}
				else endline = k;
			}
		}
		
		for (j=startline; j <= endline + 1; j++)
		{
			szPbLen = strlen((char *)PPPRecvDataNon.Data[i][j]);

			// 전문이 있는 경우 길이가 리턴된다.
			if (szPbLen)
			{
				memcpy(&PbData[PbDataCnt], PPPRecvDataNon.Data[i][j], szPbLen);

				PbDataCnt += szPbLen;
				PbData[PbDataCnt++] = 0x0a;
				// 1줄 인쇄후 13행으로 건너갈 경우 두라인을 피드한다.
				if (j == 11) 
				{
					PbData[PbDataCnt++] = 0x0a;
					PbData[PbDataCnt++] = 0x0a;
				}
				// 페이지 마지막을 인쇄한 경우 다음 페이지로 넘어간다.
				else 
				if (j == 23)
				{
					// 마지막 페이지가 아닌경우 : 2002.02.25(임영석검증바람)
					if (i != PPPRecvDataNon.p)
					{
						memcpy(&PbData[PbDataCnt], PbFrameNextSet, sizeof(PbFrameNextSet));
						PbDataCnt += 5;
					}
					break;
				}

				// 인쇄하고자 하는 데이터가 1k block을 넘지않도록 한다.
//				2008.04.30 통장인자속도 개선 : 10K내에서 한방에 던지도록 한다.
//				if (PbDataCnt > (PB_MAX_FRAME / 2))		// 512/1024초과시(50%)
				if (PbDataCnt > (sizeof(PbData) -100))			// 10K(100%) 2004.07.29
				{
					// pbm으로 데이터를 Passing한다.
					fnAPD_PbPrint(TRUE);
					fnAPD_CheckDeviceAction(DEV_PBM);					// 동작완료검지&장애검지
					memset(PbData, 0, sizeof(PbData));
					PbDataCnt = 0;
				}
			}
			else break;
		}
		startline = 0;

		if (PbDataCnt)										// 미인자자료존재시
		{
			fnAPD_CheckDeviceAction(DEV_PBM);				// 동작완료검지&장애검지
			fnAPD_PbPrint(WaitFlg);							// 통장인자
			memset(PbData, 0, sizeof(PbData));
			PbDataCnt = 0;
		}	
		
		memcpy(PbData, PbFrameSet, sizeof(PbFrameSet));
		PbDataCnt = sizeof(PbFrameSet);
	}

	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_PBM);					// 동작완료검지&장애검지

	return T_OK;
}


int CTranCmn::fnAPP_KJB_PPPProc(int WaitFlg)		// #N0181
{
	int		j;
	int		startline, endline, szPbLen;
	static	int		LocalLine = 0;
	int				LocalPage = 0;
	int				LocalI = 0;

	if (m_pDevCmn->AtmDefine.PBHandler == PBM_NAT_SET)			// NO AUTO TURN
		PPPRecvData.Ep = 0;										// 2002.08.08

	if (!PPPRecvData.ParaLength)								// DATA부/통장부프린트(거래)
		return T_OK;											// 해당자료무

	PbDataCnt = 0;												// Pb출력길이
	memset(PbData, 0, sizeof(PbData));							// Pb출력영역
	PbPrintFlag = TRUE;

	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);

	startline = PPPRecvData.y;

	for(int ii = 0; ii <= 23; ii++) 
	{
		if (!PPPRecvData.DataLength[0][ii])
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (ii == 11)	// 재봉선 skip
		{
			PbData[PbDataCnt++] = PbNewLine;
			PbData[PbDataCnt++] = PbNewLine;
		}
	}

	startline = ii;
	endline = startline + PPPRecvData.ParaLength;
 	for (j = startline; j <= endline && j <= 24; j++) 
	{
		szPbLen = PPPRecvData.DataLength[0][j];
		// 전문이 있는 경우 길이가 리턴된다.
		if (szPbLen)
		{
			memcpy(&PbData[PbDataCnt], PPPRecvData.Data[0][j], szPbLen);
			PbDataCnt += szPbLen;
			PbData[PbDataCnt++] = PbNewLine;
			if(j == 11)
			{
				PbData[PbDataCnt++] = PbNewLine;
				PbData[PbDataCnt++] = PbNewLine;
			}
		}
		else
			break;
	}

	if (PbDataCnt)											// 미인자자료존재시
	{
		fnAPD_CheckDeviceAction(DEV_PBM);					// 동작완료검지&장애검지
		fnAPD_PbPrint(WaitFlg);								// 통장인자
		memset(PbData, 0, sizeof(PbData));
		PbDataCnt = 0;
	}
	fnAPD_CheckDeviceAction(DEV_PBM);						// 동작완료검지&장애검지

	return T_OK;
}

//cc
int CTranCmn::fnAPP_HANA_PPPProc(int WaitFlg)
{
	int		j;
	int		startline = 0, endline = 0, szPbLen = 0, nextstartline = 0, nextendline = 0;
	static	int		LocalLine = 0;
	int				LocalPage = 0;
	int				LocalI = 0;

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&
		(PbTran))												
	{
		PPPRecvData.ParaLength = 1;
		PPPRecvData.Ep = 0;
		if (TranCode == TC_PBOOK)								// 통장정리 : 2003.12.23
			LocalI = 4;
		else
			LocalI = 4;
		LocalLine = 0;
//		for(int i = 0; i <= LocalI; i++) 
		{
			Strcpy(PPPRecvData.Data[LocalPage][LocalLine], 
//			 12345678901234567890123456789012345678901234567890123456789012345678901234567890
			"         123-01-123456      #LOCAL TEST");
			PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);
			LocalLine++;
			Strcpy(PPPRecvData.Data[LocalPage][LocalLine], 
//			 12345678901234567890123456789012345678901234567890123456789012345678901234567890
			"20020202 ATM IN/OUT               *123,456        *123,456   *123,456,789 REMARK");
			PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);
			LocalLine++;
		}
	}
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[PPPRecvData]", &PPPRecvData.Data[0][0], PPPRecvData.DataLength[0][0]);

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->AtmDefine.PBHandler == PBM_NAT_SET)			// NO AUTO TURN
		PPPRecvData.Ep = 0;										// 2002.08.08

	if (!PPPRecvData.ParaLength)								// DATA부/통장부프린트(거래)
		return T_OK;											// 해당자료무

	PbDataCnt = 0;												// Pb출력길이
	memset(PbData, 0, sizeof(PbData));							// Pb출력영역
	PbPrintFlag = TRUE;

	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);

	startline = PPPRecvData.y;
	endline = startline + PPPRecvData.ParaLength;


	if(startline > 0)
	{
		for(int ii=0; ii<startline; ii++)
		{
			if (ii == 11)
			{
				PbData[PbDataCnt++] = PbNewLine;
				PbData[PbDataCnt++] = PbNewLine;
			}
			PbData[PbDataCnt++] = PbNewLine;
		}
	}
	
	for (j = startline; j <= endline ; j++) 
	{
		szPbLen = PPPRecvData.DataLength[0][j];
		if (szPbLen)
		{

			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_HANA_PPPProc() &PPPRecvData.Data[0][j][0] = %3.3s", &PPPRecvData.Data[0][j][0]);
			if (memcmp(&PPPRecvData.Data[0][j][0], "M01", 3) == 0)
			{
				nextstartline = 1;
				nextendline = endline - j - 1;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_HANA_PPPProc() startline[%d], endline[%d], nextstartline[%d], nextendline[%d] j[%d]", startline, endline, nextstartline, nextendline, j);
				break;
			}

			memcpy(&PbData[PbDataCnt], PPPRecvData.Data[0][j], szPbLen);
			PbDataCnt += szPbLen;
			PbData[PbDataCnt++] = PbNewLine;

			if	(j == 11)
			{
				PbData[PbDataCnt++] = PbNewLine;
				PbData[PbDataCnt++] = PbNewLine;
			}
			
		}
		else
			break;
	}

	if (PbDataCnt)											// 미인자자료존재시
	{
		fnAPD_CheckDeviceAction(DEV_PBM);					// 동작완료검지&장애검지
		fnAPD_PbPrint(WaitFlg);								// 통장인자
		memset(PbData, 0, sizeof(PbData));
		PbDataCnt = 0;
	}

//	if (nextstartline && nextendline)
	if (nextstartline)
	{
		memcpy(PbData, PbFrameNextSet, sizeof(PbFrameNextSet));
		PbDataCnt = sizeof(PbFrameNextSet);
		
		int nIdx =  j;
		for (j = nextstartline; j <= nextendline && j <= 23; j++) 
		{
			nIdx++;
			szPbLen = PPPRecvData.DataLength[0][nIdx];
			if (szPbLen)
			{
				memcpy(&PbData[PbDataCnt], PPPRecvData.Data[0][nIdx], szPbLen);
				PbDataCnt += szPbLen;
				PbData[PbDataCnt++] = PbNewLine;
				if (j == 11)
				{
					PbData[PbDataCnt++] = PbNewLine;
					PbData[PbDataCnt++] = PbNewLine;
				}
			}
			else
				break;
		}

		if (PbDataCnt)											// 미인자자료존재시
		{
			fnAPD_CheckDeviceAction(DEV_PBM);					// 동작완료검지&장애검지
			fnAPD_PbPrint(WaitFlg);								// 통장인자
			memset(PbData, 0, sizeof(PbData));
			PbDataCnt = 0;
		}

	}

	//#0099
	if (Accept.Index == 99)
	{
		CString strTmp("");
		strTmp = strCT(Accept.CrmDummy, sizeof(Accept.CrmDummy));
		strTmp.TrimLeft();
		strTmp.TrimRight();
		m_pDevCmn->fnSCR_DisplayString(1, "[통장내역]");
		m_pDevCmn->fnSCR_DisplayString(2, strTmp);
		m_pDevCmn->fnSCR_DisplayScreen(701);
		Delay_Msg(3000);
	}

	return T_OK;
}


////////////////////////////////////////////////////////////////////////////////
// 함 수 명 : fnAPP_KB_PPPProc
// 설    명 : 통장부프린트
// 파라미터 : WaitFlg   - 대기플래그 [IN]
// 주의사항 : 없음
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPP_KB_PPPProc(int WaitFlg) //#N0226
{
	int		i, j, k;
	int		startline, endline, szPbLen;
	static	int		LocalLine = 0;
	int				LocalPage = 0;
	int				LocalI = 0;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KB_PPPProc");

	if (!PPPRecvDataKb.ParaLength)									// 동작지시무
		return T_OK;
	

	PbDataCnt = 0;												// Pb출력길이
	memset(PbData, 0, sizeof(PbData));							// Pb출력영역
	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);	

	startline = PPPRecvDataKb.Bp;
	for(i=0; i < startline; i++) 
	{
		if (!strlen((char *)PPPRecvDataKb.Data[0][i]))
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (i == 11)	// 재봉선 skip
		{
			PbData[PbDataCnt++] = PbNewLine;
			PbData[PbDataCnt++] = PbNewLine;
		}
	}
	
	startline = i;


MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "인자할페이지[%d] 인자시작라인[%d]",PPPRecvDataKb.p, startline);

	for (i=0; i <= PPPRecvDataKb.p; i++)
	{
		endline = 0;
		
		for(k=startline; k < 24; k++)
		{
			if (PPPRecvDataKb.Data[i][k][0] == 0x00)
			{
				endline = k;
				break;
			}
			else endline = k;
		}
		
		for (j=startline; j <= endline + 1; j++)
		{
			szPbLen = strlen((char *)PPPRecvDataKb.Data[i][j]);

			if (szPbLen)
			{
				memcpy(&PbData[PbDataCnt], PPPRecvDataKb.Data[i][j], szPbLen);

				PbDataCnt += szPbLen;
				PbData[PbDataCnt++] = 0x0a;
				// 1줄 인쇄후 13행으로 건너갈 경우 두라인을 피드한다.
				if (j == 11) 
				{
					PbData[PbDataCnt++] = 0x0a;
					PbData[PbDataCnt++] = 0x0a;
				}
				// 페이지 마지막을 인쇄한 경우 다음 페이지로 넘어간다.
				else 
				if (j == 23)
				{
					if (i != PPPRecvDataKb.p)
					{
						memcpy(&PbData[PbDataCnt], PbFrameNextSet, sizeof(PbFrameNextSet));
						PbDataCnt += 5;
					}
					break;
				}

				if (PbDataCnt > (sizeof(PbData) - 100))			// 10K(100%) 2004.07.29
				{
					fnAPD_PbPrint(TRUE);
					fnAPD_CheckDeviceAction(DEV_PBM);					
					memset(PbData, 0, sizeof(PbData));
					PbDataCnt = 0;
				}
			}
			else break;
		}
		startline = 0;

		if (PbDataCnt)										// 미인자자료존재시
		{
			fnAPD_CheckDeviceAction(DEV_PBM);				// 동작완료검지&장애검지
			fnAPD_PbPrint(WaitFlg);							// 통장인자
			memset(PbData, 0, sizeof(PbData));
			PbDataCnt = 0;
		}	
		
		memcpy(PbData, PbFrameSet, sizeof(PbFrameSet));
		PbDataCnt = sizeof(PbFrameSet);
	}

	if (Accept.Index == 99)
	{
		m_pDevCmn->fnSCR_DisplayString(1, "통장에 정리할 내용이 남았습니다");
		m_pDevCmn->fnSCR_DisplayString(3, "통장정리를 다시하여 주십시오");
		m_pDevCmn->fnSCR_DisplayScreen(710);
		Delay_Msg(3000);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KB_PPPProc:return()");

	return T_OK;
}



// 통장부프린트
int CTranCmn::fnAPP_IBK_PPPProc(int WaitFlg)
{
	int		j;
	int		startline, endline, szPbLen;
	static	int		LocalLine = 0;
	int				LocalPage = 0;
	int				LocalI = 0;

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&
		(PbTran))												
	{
		PPPRecvData.ParaLength = 1;
		PPPRecvData.Ep = 0;
		if (TranCode == TC_PBOOK)								// 통장정리 : 2003.12.23
			LocalI = 4;
		else
			LocalI = 4;
		LocalLine = 0;
//		for(int i = 0; i <= LocalI; i++) 
		{
			Strcpy(PPPRecvData.Data[LocalPage][LocalLine], 
//			 12345678901234567890123456789012345678901234567890123456789012345678901234567890
			"         123-01-123456      #LOCAL TEST");
			PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);
			LocalLine++;
			Strcpy(PPPRecvData.Data[LocalPage][LocalLine], 
//			 12345678901234567890123456789012345678901234567890123456789012345678901234567890
			"20020202 ATM IN/OUT               *123,456        *123,456   *123,456,789 REMARK");
			PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);
			LocalLine++;
		}
	}
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[PPPRecvData]", &PPPRecvData.Data[0][0], PPPRecvData.DataLength[0][0]);

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->AtmDefine.PBHandler == PBM_NAT_SET)			// NO AUTO TURN
		PPPRecvData.Ep = 0;										// 2002.08.08

	if (!PPPRecvData.ParaLength)								// DATA부/통장부프린트(거래)
		return T_OK;											// 해당자료무

	PbDataCnt = 0;												// Pb출력길이
	memset(PbData, 0, sizeof(PbData));							// Pb출력영역
	PbPrintFlag = TRUE;

	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);

	startline = PPPRecvData.y;
	for(int ii = 0; ii <= 23; ii++) 
	{
		if (!PPPRecvData.DataLength[0][ii])
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (ii == 11)	// 재봉선 skip
		{
			PbData[PbDataCnt++] = PbNewLine;
			PbData[PbDataCnt++] = PbNewLine;
			PbData[PbDataCnt++] = PbNewLine;
		}
	}

	startline = ii;
	endline = startline + PPPRecvData.ParaLength;
 	for (j = startline; j <= endline && j <= 24; j++) 
	{
		szPbLen = PPPRecvData.DataLength[0][j];
		// 전문이 있는 경우 길이가 리턴된다.
		if (szPbLen)
		{
			memcpy(&PbData[PbDataCnt], PPPRecvData.Data[0][j], szPbLen);
			PbDataCnt += szPbLen;
			PbData[PbDataCnt++] = PbNewLine;
			if(j == 11)
			{
				PbData[PbDataCnt++] = PbNewLine;
				PbData[PbDataCnt++] = PbNewLine;
				PbData[PbDataCnt++] = PbNewLine;
			}
		}
		else
			break;
	}

	if (PbDataCnt)											// 미인자자료존재시
	{
		fnAPD_CheckDeviceAction(DEV_PBM);					// 동작완료검지&장애검지
		fnAPD_PbPrint(WaitFlg);								// 통장인자
		memset(PbData, 0, sizeof(PbData));
		PbDataCnt = 0;
	}
	//#0025
	fnAPD_CheckDeviceAction(DEV_PBM);						// 동작완료검지&장애검지 
	if(PbPrintAfterMsgFlag == TRUE)
	{
		m_pDevCmn->fnSCR_DisplayString(2, "[통장이월]");
		m_pDevCmn->fnSCR_DisplayString(3, "창구에서 통장을 재발급 받으십시오 ");
		m_pDevCmn->fnSCR_DisplayScreen(701);
		Delay_Msg(5000);
	}


	return T_OK;
}

// 통장부프린트
int CTranCmn::fnAPP_KEB_PPPProc(int WaitFlg)
{
	int		j;
	int		startline = 0, endline = 0, szPbLen = 0, nextstartline = 0, nextendline = 0;
	static	int		LocalLine = 0;
	int				LocalPage = 0;
	int				LocalI = 0;

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&
		(PbTran))												
	{
		PPPRecvData.ParaLength = 1;
		PPPRecvData.Ep = 0;
		if (TranCode == TC_PBOOK)								// 통장정리 : 2003.12.23
			LocalI = 4;
		else
			LocalI = 4;
		LocalLine = 0;
//		for(int i = 0; i <= LocalI; i++) 
		{
			Strcpy(PPPRecvData.Data[LocalPage][LocalLine], 
//			 12345678901234567890123456789012345678901234567890123456789012345678901234567890
			"         123-01-123456      #LOCAL TEST");
			PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);
			LocalLine++;
			Strcpy(PPPRecvData.Data[LocalPage][LocalLine], 
//			 12345678901234567890123456789012345678901234567890123456789012345678901234567890
			"20020202 ATM IN/OUT               *123,456        *123,456   *123,456,789 REMARK");
			PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);
			LocalLine++;
		}
	}
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[PPPRecvData]", &PPPRecvData.Data[0][0], PPPRecvData.DataLength[0][0]);

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->AtmDefine.PBHandler == PBM_NAT_SET)			// NO AUTO TURN
		PPPRecvData.Ep = 0;										// 2002.08.08

	if (!PPPRecvData.ParaLength)								// DATA부/통장부프린트(거래)
		return T_OK;											// 해당자료무

	PbDataCnt = 0;												// Pb출력길이
	memset(PbData, 0, sizeof(PbData));							// Pb출력영역
	PbPrintFlag = TRUE;

	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);

	startline = PPPRecvData.y;
	endline = startline + PPPRecvData.ParaLength;

	if(startline > 0)
	{
		for(int ii=0; ii<startline; ii++)
		{
			if (ii == 11)
			{
				PbData[PbDataCnt++] = PbNewLine;
				PbData[PbDataCnt++] = PbNewLine;
			}
			PbData[PbDataCnt++] = PbNewLine;
		}
	}
	
	for (j = startline; j <= endline && j <= startline + 23; j++) 
	{
		szPbLen = PPPRecvData.DataLength[0][j];
		if (szPbLen)
		{

			MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_KEB_PPPProc() &PPPRecvData.Data[0][j][0] = %3.3s", &PPPRecvData.Data[0][j][0]);
			if (memcmp(&PPPRecvData.Data[0][j][0], "M01", 3) == 0)
			{
				nextstartline = 1;
				nextendline = endline - j - 1;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KEB_PPPProc() startline[%d], endline[%d], nextstartline[%d], nextendline[%d] j[%d]", startline, endline, nextstartline, nextendline, j);
				break;
			}

			memcpy(&PbData[PbDataCnt], PPPRecvData.Data[0][j], szPbLen);
			PbDataCnt += szPbLen;
			PbData[PbDataCnt++] = PbNewLine;

			if	(j == 11)
			{
				PbData[PbDataCnt++] = PbNewLine;
				PbData[PbDataCnt++] = PbNewLine;
			}
			
		}
		else
			break;
	}

	if (PbDataCnt)											// 미인자자료존재시
	{
		fnAPD_CheckDeviceAction(DEV_PBM);					// 동작완료검지&장애검지
		fnAPD_PbPrint(WaitFlg);								// 통장인자
		memset(PbData, 0, sizeof(PbData));
		PbDataCnt = 0;
	}

	if (nextstartline && nextendline)
	{
		memcpy(PbData, PbFrameNextSet, sizeof(PbFrameNextSet));
		PbDataCnt = sizeof(PbFrameNextSet);
		
		int nIdx =  j;
		for (j = nextstartline; j <= nextendline && j <= 23; j++) 
		{
			nIdx++;
			szPbLen = PPPRecvData.DataLength[0][nIdx];
			if (szPbLen)
			{
				memcpy(&PbData[PbDataCnt], PPPRecvData.Data[0][nIdx], szPbLen);
				PbDataCnt += szPbLen;
				PbData[PbDataCnt++] = PbNewLine;
				if (j == 11)
				{
					PbData[PbDataCnt++] = PbNewLine;
					PbData[PbDataCnt++] = PbNewLine;
				}
			}
			else
				break;
		}

		if (PbDataCnt)											// 미인자자료존재시
		{
			fnAPD_CheckDeviceAction(DEV_PBM);					// 동작완료검지&장애검지
			fnAPD_PbPrint(WaitFlg);								// 통장인자
			memset(PbData, 0, sizeof(PbData));
			PbDataCnt = 0;
		}

	}

	//#0099
	if (Accept.Index == 99)
	{
		CString strTmp("");
		strTmp = strCT(Accept.CrmDummy, sizeof(Accept.CrmDummy));
		strTmp.TrimLeft();
		strTmp.TrimRight();
		m_pDevCmn->fnSCR_DisplayString(1, "[통장내역]");
		m_pDevCmn->fnSCR_DisplayString(2, strTmp);
		m_pDevCmn->fnSCR_DisplayScreen(701);
		Delay_Msg(3000);
	}
	
	return T_OK;
}


// 통장부프린트
int CTranCmn::fnAPP_CTB_PPPProc(int WaitFlg)				// #N0252
{
	int		i, j, k;
	int		startline, endline, szPbLen;
	static	int		LocalLine = 0;
	int				LocalPage = 0;
	int				LocalI = 0;

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&
		(PbTran))												
	{
		PPPRecvData.ParaLength = 1;
		PPPRecvData.Ep = 0;
		if (TranCode == TC_PBOOK)								// 통장정리 : 2003.12.23
			LocalI = 4;
		else
			LocalI = 4;
		LocalLine = 0;
//		for(int i = 0; i <= LocalI; i++) 
		{
			Strcpy(PPPRecvData.Data[LocalPage][LocalLine], 
//			 12345678901234567890123456789012345678901234567890123456789012345678901234567890
			"         123-01-123456      #LOCAL TEST");
			PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);
			LocalLine++;
			Strcpy(PPPRecvData.Data[LocalPage][LocalLine], 
//			 12345678901234567890123456789012345678901234567890123456789012345678901234567890
			"20020202 ATM IN/OUT               *123,456        *123,456   *123,456,789 REMARK");
			PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);
			LocalLine++;
		}
	}
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[PPPRecvData]", &PPPRecvData.Data[0][0], PPPRecvData.DataLength[0][0]);

/////////////////////////////////////////////////////////////////////////////

	if ((!IsSpace(PPPRecvData.Data, sizeof(PPPRecvData.Data))) &&
		(!IsNull(PPPRecvData.Data, sizeof(PPPRecvData.Data))))
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_PPPProc start");
	}
	else
		return T_OK;

	memset(PbData, 0, sizeof(PbData));							// Pb출력영역
	PbDataCnt = 0;
	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);

	if((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&	(PbTran))		//V06-00-05-#01
		startline = LocalLine;
	else
		startline = 25;
	// 선두행 찾기 - 데이터가 NULL이면 라인피드를 한다.
	for(i=0; i < startline; i++) 
	{
		if (!strlen((char *)PPPRecvData.Data[0][i]))
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (i == 11)	// 재봉선 skip
		{
			PbData[PbDataCnt++] = PbNewLine;
			PbData[PbDataCnt++] = PbNewLine;
		}
	}
	
	startline = i;


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "인자할페이지[%d] 인자시작라인[%d]",PPPRecvData.p, startline);
//MsgDumpAnal(TRACE_CODE_MODE, "Log", "인자할페이지[%d]", PPPRecvData.p);

	// 마지막 검지 조건 
	// PPPRecvData.p가 마지막 데이터가 있는 페이지의 번호이다.
	for (i=0; i <= PPPRecvData.p; i++)
	{
		endline = 0;
		
		for(k=startline; k < 25; k++)
		{
			if ((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&	(PbTran) && (TranCode == TC_PBOOK))		// V06-00-05-#01
			{
				endline = k;
			}
			else
			{
				if (PPPRecvData.Data[i][k][0] == 0x00)
				{
					endline = k;
					break;
				}
				else endline = k;
			}
		}
		
		for (j=startline; j <= endline + 1; j++)
		{
			szPbLen = strlen((char *)PPPRecvData.Data[i][j]);

			// 전문이 있는 경우 길이가 리턴된다.
			if (szPbLen)
			{
				memcpy(&PbData[PbDataCnt], PPPRecvData.Data[i][j], szPbLen);

				PbDataCnt += szPbLen;
				PbData[PbDataCnt++] = 0x0a;
				// 1줄 인쇄후 13행으로 건너갈 경우 두라인을 피드한다.
				if (j == 11) 
				{
					PbData[PbDataCnt++] = 0x0a;
					PbData[PbDataCnt++] = 0x0a;
				}
				// 페이지 마지막을 인쇄한 경우 다음 페이지로 넘어간다.
				else 
				if (j == 24)
				{
					// 마지막 페이지가 아닌경우 : 2002.02.25(임영석검증바람)
					if (i != PPPRecvData.p)
					{
						memcpy(&PbData[PbDataCnt], PbFrameNextSet, sizeof(PbFrameNextSet));
						PbDataCnt += 5;
					}
					break;
				}

				// 인쇄하고자 하는 데이터가 1k block을 넘지않도록 한다.
//				2008.04.30 통장인자속도 개선 : 10K내에서 한방에 던지도록 한다.
//				if (PbDataCnt > (PB_MAX_FRAME / 2))		// 512/1024초과시(50%)
				if (PbDataCnt > (sizeof(PbData) -100))			// 10K(100%) 2004.07.29
				{
					// pbm으로 데이터를 Passing한다.
					fnAPD_PbPrint(TRUE);
					fnAPD_CheckDeviceAction(DEV_PBM);					// 동작완료검지&장애검지
					memset(PbData, 0, sizeof(PbData));
					PbDataCnt = 0;
				}
			}
			else break;
		}
		startline = 0;

		if (PbDataCnt)										// 미인자자료존재시
		{
			fnAPD_CheckDeviceAction(DEV_PBM);				// 동작완료검지&장애검지
			fnAPD_PbPrint(WaitFlg);							// 통장인자
			memset(PbData, 0, sizeof(PbData));
			PbDataCnt = 0;
		}	
		
		memcpy(PbData, PbFrameSet, sizeof(PbFrameSet));
		PbDataCnt = sizeof(PbFrameSet);
	}

	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_PBM);					// 동작완료검지&장애검지

	return T_OK;
}

// 통장부프린트  #N0245
int CTranCmn::fnAPP_SC_PPPProc(int WaitFlg)
{
	int		i, j, k;
	int		startline, endline, szPbLen;
	static	int		LocalLine = 0;
	int				LocalPage = 0;
	int				LocalI = 0;

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->HostLocalSvrMode == TRUE) && (PbTran))			// 로컬서버모드지원 (공장인증용/기업) #0424
	{
		PPPRecvData.p = 0;
		memset(PPPRecvData.Data, 0x00, sizeof(PPPRecvData.Data));	

		LocalLine = 0;
		LocalI = 24;

		for(int   cnt = 0; cnt <= LocalI; cnt++) 
		{
			if (LocalLine == 0)
			{
				Strcpy(PPPRecvData.Data[LocalPage][LocalLine], 
				"2015     123-01-123456      SC은행 통장TEST");
//				 12345678901234567890123456789012345678901234567890123456789012345678901234567890
				PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);
			}
			else
			{
				Strcpy(PPPRecvData.Data[LocalPage][LocalLine], 
				"20150211 BCD  테스트00         *123,456      *1,234,567 수서123");
//				 12345678901234567890123456789012345678901234567890123456789012345678901234567890
				PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);
			}

			LocalLine++;

			/* 통장 AutoTurn 테스트용
			if (LocalLine >= 24)
			{
				LocalLine = 0;

				if (m_pDevCmn->AtmDefine.PBHandler == I_PBM_SET)
				{
					LocalPage++;
					PPPRecvData.p = LocalPage;
					PbNextTranFlag = TRUE;
				}
				else break;
			}
			*/
		}

		PPPRecvData.ParaLength = cnt;
	}
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[PPPRecvData]", &PPPRecvData.Data[0][0], PPPRecvData.DataLength[0][0]);

/////////////////////////////////////////////////////////////////////////////

	if ((!IsSpace(PPPRecvData.Data, sizeof(PPPRecvData.Data))) &&
		(!IsNull(PPPRecvData.Data, sizeof(PPPRecvData.Data))))
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_PPPProc start");
	}
	else
		return T_OK;

	memset(PbData, 0, sizeof(PbData));							// Pb출력영역
	PbDataCnt = 0;
	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);

	if((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&	(PbTran))		//V06-00-05-#01
		startline = LocalLine;
	else
		startline = 23;

	// 선두행 찾기 - 데이터가 NULL이면 라인피드를 한다.
	for(i=0; i < startline; i++) 
	{
		if (!strlen((char *)PPPRecvData.Data[0][i]))
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (i == 11)	// 재봉선 skip
		{
			PbData[PbDataCnt++] = PbNewLine;
			PbData[PbDataCnt++] = PbNewLine;
		}
	}
	
	startline = i;


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "인자할페이지[%d] 인자시작라인[%d]",PPPRecvData.p, startline);
//MsgDumpAnal(TRACE_CODE_MODE, "Log", "인자할페이지[%d]", PPPRecvData.p);

	// 마지막 검지 조건 
	// PPPRecvData.p가 마지막 데이터가 있는 페이지의 번호이다.
	for (i=0; i <= PPPRecvData.p; i++)
	{
		endline = 0;
		
		for(k=startline; k < 25; k++)
		{
			if ((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&	(PbTran) && (TranCode == TC_PBOOK))		// V06-00-05-#01
			{
				endline = k;
			}
			else
			{
				if (PPPRecvData.Data[i][k][0] == 0x00)
				{
					endline = k;
					break;
				}
				else endline = k;
			}
		}
		
		for (j=startline; j <= endline + 1; j++)
		{
			szPbLen = strlen((char *)PPPRecvData.Data[i][j]);

			// 전문이 있는 경우 길이가 리턴된다.
			if (szPbLen)
			{
				memcpy(&PbData[PbDataCnt], PPPRecvData.Data[i][j], szPbLen);

				PbDataCnt += szPbLen;
				PbData[PbDataCnt++] = 0x0a;
				// 1줄 인쇄후 13행으로 건너갈 경우 두라인을 피드한다.
				if (j == 11) 
				{
					PbData[PbDataCnt++] = 0x0a;
					PbData[PbDataCnt++] = 0x0a;
				}
				// 페이지 마지막을 인쇄한 경우 다음 페이지로 넘어간다.
				else 
				if (j == 23)
				{
					// 마지막 페이지가 아닌경우 : 2002.02.25(임영석검증바람)
					if (i != PPPRecvData.p)
					{
						memcpy(&PbData[PbDataCnt], PbFrameNextSet, sizeof(PbFrameNextSet));
						PbDataCnt += 5;
					}
					break;
				}

				// 인쇄하고자 하는 데이터가 1k block을 넘지않도록 한다.
//				2008.04.30 통장인자속도 개선 : 10K내에서 한방에 던지도록 한다.
//				if (PbDataCnt > (PB_MAX_FRAME / 2))		// 512/1024초과시(50%)
				if (PbDataCnt > (sizeof(PbData) -100))			// 10K(100%) 2004.07.29
				{
					// pbm으로 데이터를 Passing한다.
					fnAPD_PbPrint(TRUE);
					fnAPD_CheckDeviceAction(DEV_PBM);					// 동작완료검지&장애검지
					memset(PbData, 0, sizeof(PbData));
					PbDataCnt = 0;
				}
			}
			else break;
		}
		startline = 0;

		// 마지막 라인까지 인자한 후 PbNextTranFlag가 TRUE인 경우 페이지를 넘긴다. SC은행은 한페이지 단위로 데이터가 수신됨
		if (j >= 23)
		{
			// 다음페이지 인자정보 Flag가 TRUE일 경우
			if (PbNextTranFlag && (m_pProfile->DEVICE.PBHandler == I_PBM_SET))		// #0424 IPBM만 적용
			{
				memcpy(&PbData[PbDataCnt], PbFrameNextSet, sizeof(PbFrameNextSet));
				PbDataCnt += 5;
			}
		}
		
		if (PbDataCnt)										// 미인자자료존재시
		{
			fnAPD_CheckDeviceAction(DEV_PBM);				// 동작완료검지&장애검지
			fnAPD_PbPrint(WaitFlg);							// 통장인자
			memset(PbData, 0, sizeof(PbData));
			PbDataCnt = 0;
		}	
		
		memcpy(PbData, PbFrameSet, sizeof(PbFrameSet));
		PbDataCnt = sizeof(PbFrameSet);
	}

	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_PBM);					// 동작완료검지&장애검지

	return T_OK;
}

// 통장부프린트  #N0277
int CTranCmn::fnAPP_KNB_PPPProc(int WaitFlg)
{
	int		j;
	int		startline, endline, szPbLen;
	static	int		LocalLine = 0;
	int				LocalPage = 0;
	int				LocalI = 0;

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&
		(PbTran))												
	{
		PPPRecvData.ParaLength = 1;
		PPPRecvData.Ep = 0;
		if (TranCode == TC_PBOOK)								// 통장정리 : 2003.12.23
			LocalI = 4;
		else
			LocalI = 4;
		LocalLine = 0;
//		for(int i = 0; i <= LocalI; i++) 
		{
			Strcpy(PPPRecvData.Data[LocalPage][LocalLine], 
//			 12345678901234567890123456789012345678901234567890123456789012345678901234567890
			"         123-01-123456      #LOCAL TEST");
			PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);
			LocalLine++;
			Strcpy(PPPRecvData.Data[LocalPage][LocalLine], 
//			 12345678901234567890123456789012345678901234567890123456789012345678901234567890
			"20020202 ATM IN/OUT               *123,456        *123,456   *123,456,789 REMARK");
			PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);
			LocalLine++;
		}
	}
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[PPPRecvData]", &PPPRecvData.Data[0][0], PPPRecvData.DataLength[0][0]);

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->AtmDefine.PBHandler == PBM_NAT_SET)			// NO AUTO TURN
		PPPRecvData.Ep = 0;										// 2002.08.08

	if (!PPPRecvData.ParaLength)								// DATA부/통장부프린트(거래)
		return T_OK;											// 해당자료무

	PbDataCnt = 0;												// Pb출력길이
	memset(PbData, 0, sizeof(PbData));							// Pb출력영역
	PbPrintFlag = TRUE;

	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);

	startline = PPPRecvData.y;

	for(int ii = 0; ii <= 23; ii++) 
	{
		if (!PPPRecvData.DataLength[0][ii])
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (ii == 11)	// 재봉선 skip
		{
			PbData[PbDataCnt++] = PbNewLine;
			PbData[PbDataCnt++] = PbNewLine;
		}
	}

	startline = ii;
	endline = startline + PPPRecvData.ParaLength;
 	for (j = startline; j <= endline && j <= 24; j++) 
	{
		szPbLen = PPPRecvData.DataLength[0][j];
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KNB_PPPProc() - Line:%d, Len:%d", j, szPbLen);
		// 전문이 있는 경우 길이가 리턴된다.
		if (szPbLen)
		{
			memcpy(&PbData[PbDataCnt], PPPRecvData.Data[0][j], szPbLen);
			PbDataCnt += szPbLen;
			PbData[PbDataCnt++] = PbNewLine;
			if(j == 11)
			{
				PbData[PbDataCnt++] = PbNewLine;
				PbData[PbDataCnt++] = PbNewLine;
			}
		}
		else
			break;
	}

	if (PbDataCnt)											// 미인자자료존재시
	{
		fnAPD_CheckDeviceAction(DEV_PBM);					// 동작완료검지&장애검지
		fnAPD_PbPrint(WaitFlg);								// 통장인자
		memset(PbData, 0, sizeof(PbData));
		PbDataCnt = 0;
	}
	fnAPD_CheckDeviceAction(DEV_PBM);						// 동작완료검지&장애검지

	return T_OK;
}

// 카드M/S기록
int CTranCmn::fnAPP_PCWProc(int WaitFlg)
{
	if (!PCWRecvData.ParaLength)								// DATA부/카드M/S WRITE(거래)
		return T_OK;											// 해당자료무

	if (MenuIrdaMode)											// 적외선거래
		return T_OK;

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		return T_OK;

	if (!CardTran)												// 카드유무체크
		return T_OK;

	return T_OK;
}

// 메세지표시
int CTranCmn::fnAPP_PMDProc()
{
	int		ScrNum = 523;
	CString	StrBuff("");
	CString	GetKeyStr("");
	CString cstrTemp("");
	CString strAccountNum("");
	int		FindFlag = FALSE;
	int		nWaitSec = K_15_WAIT;


	if (m_pDevCmn->TranResult)
	{

/////////////////////////////////////////////////////////////////////////////
//		Accept.SlipChoiceFlag = TRUE;								// Slip Select Flag
/////////////////////////////////////////////////////////////////////////////
		if ((!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))		||		
			(m_pDevCmn->fnAPL_GetAvailErrorDevice(DEV_SPR))	||
			(m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)	||
			(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER))
		{
			Accept.SlipChoiceFlag = FALSE;
			nWaitSec = K_10_WAIT;
			ScrNum = 524;
		}
		else
		if (!Accept.SlipChoiceFlag)
		{
			nWaitSec = K_5_WAIT;
			ScrNum = 524;			
		}

		if(TranCode2 == TRANID_4310  || TranCode2 == TRANID_4210 || TranCode2 == TRANID_4116 || TranCode2 == TRANID_4117)  //#N0182
		{
			if (m_pDevCmn->EarPhoneInserted == FALSE)   //전맹인인경우 계좌번호 필드 입력하면 안됨.
				m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_017x);

			m_pDevCmn->fnSCR_DisplayAmount(2,  m_RD.byDataField_023x);
			m_pDevCmn->fnSCR_DisplayAmount(3, (LPCTSTR)m_RD.byDataField_024x);
//   		m_pDevCmn->fnSCR_DisplayAmount(4, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			m_pDevCmn->fnSCR_DisplayAmount(4, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
		}
		else
		if(TranCode2 == TRANID_4330 )
		{

			if (m_pDevCmn->EarPhoneInserted == FALSE)   //#N0193 전맹인인경우 계좌번호 필드 입력하면 안됨.
				m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_017x);

			m_pDevCmn->fnSCR_DisplayAmount(2,  m_RD.byDataField_023x);

			if(memcmp(m_RD.byDataField_024x, "******", 6) == 0)   //-1  수수료 * 화면 표시
				m_pDevCmn->fnSCR_DisplayString(3, (LPCTSTR)m_RD.byDataField_024x);
			else
				m_pDevCmn->fnSCR_DisplayAmount(3, (LPCTSTR)m_RD.byDataField_024x);
			
//#0130		m_pDevCmn->fnSCR_DisplayAmount(4, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			m_pDevCmn->fnSCR_DisplayAmount(4, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			//
			if((memcmp(m_RD.byDataField_458x, "????", 4) != 0) && (memcmp(m_RD.byDataField_458x, "****", 4) != 0))
			{
				cstrTemp.Empty();
				if (m_pDevCmn->EarPhoneInserted == FALSE)		
					cstrTemp.Format("%d.%2.2s", Asc2Int(&m_RD.byDataField_458x[0], 2), &m_RD.byDataField_458x[2]);
				else
					cstrTemp.Format("이자율 연 %d.%2.2s  퍼센트", Asc2Int(&m_RD.byDataField_458x[0], 2), &m_RD.byDataField_458x[2]);
				m_pDevCmn->fnSCR_DisplayString(5, cstrTemp); //이자율  
			}
			else
			if(memcmp(m_RD.byDataField_458x, "****", 4) == 0)
				m_pDevCmn->fnSCR_DisplayString(5, (LPCTSTR)m_RD.byDataField_458x);
		}
		else
		if(TranCode2 == TRANID_4332)  //#N0171
		{
			m_pDevCmn->fnSCR_DisplayString(1,  m_RD.byDataField_017x);
			m_pDevCmn->fnSCR_DisplayAmount(2,  m_RD.byDataField_023x);
			m_pDevCmn->fnSCR_DisplayAmount(3, (LPCTSTR)m_RD.byDataField_024x);
			if(memcmp(Accept.MuConfirmNum, "CC", 2) != 0)
				m_pDevCmn->fnSCR_DisplayAmount(4, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			
			if(memcmp(Accept.MuConfirmNum, "CC", 2) == 0)
				m_pDevCmn->fnSCR_DisplayString(10, "CUP");
		}
		else
		if(TranCode2 == TRANID_4410 || TranCode2 == TRANID_4720)
		{
			if (m_pDevCmn->EarPhoneInserted == FALSE)   //#N0193 전맹인인경우 계좌번호 필드 입력하면 안됨.
				m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_017x);
			m_pDevCmn->fnSCR_DisplayAmount(2,  m_RD.byDataField_023x);
			m_pDevCmn->fnSCR_DisplayAmount(3, m_RD.byDataField_024x);
//  		m_pDevCmn->fnSCR_DisplayAmount(4, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			if((m_pProfile->DEVICE.ETCDevice20 == KB) && (MenuAvail & MENU_PB))	//#N0226 국민 통장 입금 시 잔액 표시 안함
				;
			else
				m_pDevCmn->fnSCR_DisplayAmount(4, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
		}
		else
		if (TranCode2 == TRANID_4390 || TranCode2 == TRANID_4370 || TranCode2 == TRANID_4215 || TranCode2 == TRANID_4396)
		{
			ScrNum = 546;	
			m_pDevCmn->fnSCR_DisplayString(1, fnAPP_Get_BankName(Accept.BankNum)); // 수취은행명																	
			m_pDevCmn->fnSCR_DisplayString(2, Accept.InAccountNum);     // 수취계좌번호																	
			m_pDevCmn->fnSCR_DisplayString(3, m_RD.byDataField_021x);   // 수취인성명																	
			m_pDevCmn->fnSCR_DisplayAmount(4, m_RD.byDataField_023x);   // 거래금액																
			m_pDevCmn->fnSCR_DisplayAmount(5, (LPCTSTR)m_RD.byDataField_024x); //수수료
			if (m_pDevCmn->EarPhoneInserted == TRUE)		
			{
				strAccountNum.Format("<digit>%s</digit>", m_RD.byDataField_017x);
				m_pDevCmn->fnSCR_DisplayString(6, strAccountNum); //계좌번호
			}
			else
				m_pDevCmn->fnSCR_DisplayString(6, m_RD.byDataField_017x); //계좌번호

		}
		else
		if (TranCode2 == TRANID_4350)
		{
			ScrNum = 547;	
			m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_020x);
			m_pDevCmn->fnSCR_DisplayAmount(2,  m_RD.byDataField_023x);

			if(memcmp(m_RD.byDataField_024x, "******", 6) == 0)   //-1  수수료 * 화면 표시
				m_pDevCmn->fnSCR_DisplayString(3, (LPCTSTR)m_RD.byDataField_024x);
			else
				m_pDevCmn->fnSCR_DisplayAmount(3, (LPCTSTR)m_RD.byDataField_024x);
			
//#0130		m_pDevCmn->fnSCR_DisplayAmount(4, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			m_pDevCmn->fnSCR_DisplayAmount(4, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
/*
			//
			if(memcmp(m_RD.byDataField_458x, "????", 4) != 0 )
			{
				cstrTemp.Empty();
				cstrTemp.Format("%d.%2.2s", Asc2Int(&m_RD.byDataField_458x[0], 2), &m_RD.byDataField_458x[2]);
				m_pDevCmn->fnSCR_DisplayString(5, cstrTemp); //이자율  
			}
*/
		}
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, nWaitSec, PIN_MENU_MODE1);

		GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
		if (GetKeyStr == S_TIMEOVER)
			;
		else
		if (GetKeyStr == S_CONFIRM)
			;
		else
		if (GetKeyStr == S_EXIT)
			;
		else
		if (GetKeyStr == S_CANCEL)
			;
		else
		if (GetKeyStr == S_YES)
			;
		else
		if (GetKeyStr == S_NO)
			Accept.SlipChoiceFlag = FALSE;
		else
			fnAPP_CancelProc(T_PROGRAM);


		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("명세표   : %s", (Accept.SlipChoiceFlag == TRUE)	? "선택" : "미선택"));
	}

	return T_OK;
}


// A4 PRINT PROC
int CTranCmn::fnAPP_A4PrintProc(int nTranCode)
{
	switch (nTranCode)
	{
		case TC_INQUIRYA:										// 거래내역조회
			fnAPP_A4PrintTranList();							// 거래내역조회	A4Print
			break;
		case TC_INQDEP:
		case TC_INQSECUR:										// 예수금조회	A4Print
			fnAPP_A4PrintDepSequreInquiry();					// 유가증권조회	A4Print
			break;
		case TC_INQSTOCK:
			fnAPP_A4PrintInqStock();					// 유가증권조회	A4Print
			break;
		default:
			break;
	}
	return T_OK;
}

// 거래내역조회 A4 Print
int CTranCmn::fnAPP_A4PrintTranList()							// 거래내역조회 A4Print
{
	int			PageCnt = 0;
	int			LineCnt = 0;
	int			nWaitSec = -1;
	int			nEditLoopCnt = 0;
	int			nPrintEnd = 0;
	CString		strTempPrintData("");
	CString		strEditData("");
	CString		strTempEditData("");
	CString		strTempUnPackPrintData("");
	CString		strFormData("");
	CString		strCount("");
	CString		strFormInformation("");

	CString		strDate("");
	CString		strTime("");
	CString		strTranContents("");
	CString		strWithAmount("");
	CString		strDepositAmount("");
	CString		strDetailContents("");
	CString		strRemainCount("");
	CString		strRemain("");
	CString		strBranchName("");
	


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_A4PrintTranList() return");
	return T_OK;
}

// 예수금조회 & 유가증권조회 A4 Print
int CTranCmn::fnAPP_A4PrintDepSequreInquiry()					// 예수금조회 & 유가증권조회 A4Print
{
	int			PageCnt = 0;
	int			LineCnt = 0;
	int			nPrintEnd = 0;
	CString		strTempPrintData("");
	CString		strEditData("");
	CString		strTempEditData("");
	CString		strTempUnPackPrintData("");
	CString		strFormData("");
	CString		strCount("");
	CString		strFormInformation("");

	CString		strItemSection("");
	CString		strItemName("");
	CString		strDepSecureRemain("");
	CString		strReference("");
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_A4PrintDepSequreInquiry()");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_A4PrintDepSequreInquiry() return");	
	return T_OK;
}


// 청약내역 조회 A4 Print
int CTranCmn::fnAPP_A4PrintInqStock()					// 예수금조회 & 유가증권조회 A4Print
{
	int			i = 0;
	int			PageCnt = 0;
	int			LineCnt = 0;
	int			nPrintEnd = 0;
	CString		strTempPrintData("");
	CString		strEditData("");
	CString		strTempEditData("");
	CString		strTempUnPackPrintData("");
	CString		strFormData("");
	CString		strCount("");
	CString		strFormInformation("");

	CString		strItemSection("");
	CString		strItemName("");
	CString		strDepSecureRemain("");
	CString		strReference("");
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_A4PrintDepSequreInquiry()");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_A4PrintDepSequreInquiry() return");	
	return T_OK;
}
/////////////////////////////////////////////////////////////////////////////
// 조회후거래선택
int CTranCmn::fnAPP_PMEAcceptInquiryTranSel()
{

	return T_OK;
}
// 서비스내역조회 확인
int CTranCmn::fnAPP_PMEAcceptInquiryTranServiceConfirm()
{
	int		ScrNum = 0;
	CString GetKeyStr;

	CString Data;
	Data.Empty();

	ChoiceTran = TRAN_NULL;									//#N0223 변수 초기화 

	if(WithAvail == WITH_NOT_AVAILABLE)
		m_pDevCmn->fnSCR_DisplayImage(1, FALSE);			//#N0223 출금 불가능일때 출금 저튼 삭제

	if(m_pProfile->DEVICE.ETCDevice20 != HANA)
	{
		m_pDevCmn->fnSCR_DisplayImage(1, FALSE);			//#N0223 하나은행 브랜드 에서만 출금, 이체 버튼 출력 HU81K508에만 버튼 존재.
		m_pDevCmn->fnSCR_DisplayImage(2, FALSE);			
	}

	if (TranCode == TC_INQUIRY)								// 은행
	{
		ScrNum = 508;											// 서비스내역조회
		m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_017x);
//#0130	m_pDevCmn->fnSCR_DisplayAmount(2, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
		m_pDevCmn->fnSCR_DisplayAmount(2, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
		m_pDevCmn->fnSCR_DisplayAmount(3, m_RD.byDataField_025x, strlen(m_RD.byDataField_025x), ' ', (m_RD.byDataField_025x[0] == '-') ? TRUE : FALSE);
	}
	else
	if (TranCode == TC_SVCINQ || TranCode == TC_FOREGIN)		// 서비스내역조회 거래
	{
		ScrNum = 509;											// 서비스내역조회
		m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_017x);
		m_pDevCmn->fnSCR_DisplayAmount(2, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
		if (TranCode2 == TRANID_7230) 
			m_pDevCmn->fnSCR_DisplayAmount(3, (LPCTSTR)m_RD.byDataField_024x);
	}

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();

	if (GetKeyStr == S_EXIT)								// 1차 조회거래에 대한 확인
	{
		ChoiceTran = TRAN_NULL;
	}
	else 
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
	else 
	if (GetKeyStr == S_TIMEOVER)							// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
//	else
//		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 조회결과표시
int CTranCmn::fnAPP_PMEAcceptInquiryDisplay()
{
	int		ScrNum = 0, StrIndex = 1;
	CString	GetKeyStr;
	int		i = 0;

	return T_OK;
}

// 예금이체 확인
int CTranCmn::fnAPP_PMEAcceptTransConfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString GetKeyStr("");
	CString	cstrTemp("");
	CString strAccountNum("");

	ScrNum = 515;

	for (int i = 0; i < itemof(BankCodeTbl); i++)
	{
		if (memcmp(Accept.BankNum, BankCodeTbl[i].BankCode, 3) == 0)
		{
			cstrTemp = GetString(BankCodeTbl[i].BankName, strlen(BankCodeTbl[i].BankName), strlen(BankCodeTbl[i].BankName));
			break;
		}
	}

	if(TranCode2 == TRANID_7260  || TranCode2 == TRANID_7261 || TranCode2 == TRANID_7276)
	{
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(cstrTemp.GetBuffer(0), cstrTemp.GetLength(), cstrTemp.GetLength()));
																	// 수취은행명
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, Accept.InAccountNum);
																	// 수취계좌번호
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_021x);
																	// 수취인성명
		m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, m_RD.byDataField_023x);
																	// 거래금액
		m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, (LPCTSTR)m_RD.byDataField_024x); //수수료
		
		if (m_pDevCmn->EarPhoneInserted == TRUE)	
		{
			strAccountNum.Format("<digit>%s</digit>", m_RD.byDataField_017x);
			m_pDevCmn->fnSCR_DisplayString(6, strAccountNum); //계좌번호
		}
		else
			m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_017x); //계좌번호
		
		memcpy(Accept.InAccountName, m_RD.byDataField_021x, strlen(m_RD.byDataField_021x)); // 수취계좌번호

		if(m_pProfile->DEVICE.ETCDevice20 == KEB && memcmp(&m_RD.byDataField_345x[0], "PT", 2) == 0)  //#N0180
			m_pDevCmn->fnSCR_DisplayString(10, m_RD.byDataField_345x);
	}
	else
	if(TranCode2 == TRANID_7380 || TranCode2 == TRANID_7250)  // 광주 브랜드 신용카드 이체
	{
		if(TranResultNGCode == TRAN_MSG_EXCEPTION_707)
			ScrNum = 427;
		else
			ScrNum = 527;
		if (m_pDevCmn->EarPhoneInserted == FALSE)   //전맹인인경우 은행명 필드 입력하면 안됨.
		{
			m_pDevCmn->fnSCR_DisplayString(1, GetString(cstrTemp.GetBuffer(0), cstrTemp.GetLength(), cstrTemp.GetLength()));
																	// 수취은행명
			m_pDevCmn->fnSCR_DisplayString(2, Accept.InAccountNum);
																		// 수취계좌번호
			m_pDevCmn->fnSCR_DisplayString(3, m_RD.byDataField_021x);
																	// 수취인성명
		}
		m_pDevCmn->fnSCR_DisplayAmount(4, m_RD.byDataField_023x);
																	// 거래금액
		m_pDevCmn->fnSCR_DisplayAmount(5, (LPCTSTR)m_RD.byDataField_024x); //수수료	

		//
		if (memcmp(m_RD.byDataField_458x, "????", 4) != 0)
		{
			cstrTemp.Empty();
			if (m_pDevCmn->EarPhoneInserted == FALSE)	
				cstrTemp.Format("%d.%2.2s", Asc2Int(&m_RD.byDataField_458x[0], 2), &m_RD.byDataField_458x[2]);
			else
				cstrTemp.Format("이자율 연 %d.%2.2s  퍼센트", Asc2Int(&m_RD.byDataField_458x[0], 2), &m_RD.byDataField_458x[2]);
			m_pDevCmn->fnSCR_DisplayString(6, cstrTemp); //이자율  
		}
		memcpy(Accept.InAccountName, m_RD.byDataField_021x, strlen(m_RD.byDataField_021x)); // 수취계좌번호
		
		StrIndex = 7; //전맹인 거래시 화면 출력 음성 출력 싱크 보정, 명세표 정보 출력 X
	}
	else
	{
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(cstrTemp.GetBuffer(0), cstrTemp.GetLength(), cstrTemp.GetLength()));
																	// 수취은행명
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_020x);
																	// 수취계좌번호
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_021x);
																	// 수취인성명
		m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, m_RD.byDataField_023x);
																	// 거래금액
		m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, (LPCTSTR)m_RD.byDataField_024x);
		memcpy(Accept.InAccountNum,  m_RD.byDataField_020x, strlen(m_RD.byDataField_020x));	// 입고계좌번호
		memcpy(Accept.InAccountName, m_RD.byDataField_021x, strlen(m_RD.byDataField_021x));// 입고계좌명
	}

	if (m_pDevCmn->EarPhoneInserted == FALSE)		// 전맹인 거래시 명세표 정보 입력 X
	{
		if ((!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))		||		
			(m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))	||
			(m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)	||
			(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER))
		{
			m_pDevCmn->fnSCR_DisplayString(StrIndex++, "1");				
		}
		else
			m_pDevCmn->fnSCR_DisplayString(StrIndex++, "0");				
	}
																// 수수료
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE1);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();

	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (GetKeyStr == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (GetKeyStr == S_CONFIRM)
		return T_OK;
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

// 송금 확인
int CTranCmn::fnAPP_PMEAcceptRemitConfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString GetKeyStr("");

	ScrNum = 512;

	m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_017x);
																// 수취계좌번호
	m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_021x);
																// 수취인성명
	m_pDevCmn->fnSCR_DisplayAmount(StrIndex++,  m_RD.byDataField_023x);
																// 거래금액
	m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, (LPCTSTR)m_RD.byDataField_024x);
																// 수수료
//#0130
//	m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
	m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
																// 거래금액
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();

	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (GetKeyStr == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (GetKeyStr == S_CONFIRM)
		return T_OK;
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

// 자동이체설정 확인
int CTranCmn::fnAPP_PMEAcceptAutoConfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString GetKeyStr("");

	ScrNum = 514;

	m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, (LPCTSTR)Accept.Money, 15);
																// 설정금액
	m_pDevCmn->fnSCR_DisplayString(StrIndex++, (LPCTSTR)Accept.AutoCycle, 2);
																// 이체주기
	m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetSprintf("%4.4s년 %2.2s월 %2.2s일",
															&PMERecvData.Data[2][0],
															&PMERecvData.Data[2][4],
															&PMERecvData.Data[2][6]));
																// 이체시작일
	m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetSprintf("%4.4s년 %2.2s월 %2.2s일",
															&PMERecvData.Data[2][8],
															&PMERecvData.Data[2][12],
															&PMERecvData.Data[2][14]));
																// 이체종료일
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();

	if (GetKeyStr == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (GetKeyStr == S_CONFIRM)
		return T_OK;
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

// 거래내역조회 거래 확인
int CTranCmn::fnAPP_PMEAcceptTranListConfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString	GetKeyStr("");
	CString	DispMsg(""), tmpMsg("");
	char	szTemp[32] = {0,};

	if(TranCode == TC_INQUIRYA)									// 거래내역조회
		ScrNum = 507;
	else if(TranCode == TC_INQSECUR)							// 유가증권조회
		ScrNum = 506;
	else if(TranCode == TC_INQSTOCK)							// 청약내역조회
		ScrNum = 962;


	return T_OK;
}

// 증권종목 리스트선택
int CTranCmn::fnAPP_PMEAcceptSeqListConfirm()
{


	return T_OK;
}

// 자사대체확인
int CTranCmn::fnAPP_PMEAcceptTranStocConfirm()
{
	int		ScrNum = 0, nIndex = 1;
	CString GetKeyStr("");

	ScrNum = 904;												// 자사대체 확인

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);

																// 입고계좌명
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	if (GetKeyStr == S_CONFIRM)									// 조회거래에 대한 확인
		return T_OK;
	else 
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);								// 거래취소처리
	else 
	if (GetKeyStr == S_TIMEOVER)								// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);


	return T_OK;
}

// 증권종목 청약확인
int CTranCmn::fnAPP_PMEAcceptSeqTranConfirm()
{
	int		ScrNum = 0, nIndex = 1;
	CString GetKeyStr("");

	ScrNum = 925;											// 청약확인

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);

	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	if (GetKeyStr == S_CONFIRM)									// 조회거래에 대한 확인
		return T_OK;
	else 
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);								// 거래취소처리
	else 
	if (GetKeyStr == S_TIMEOVER)								// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);


	return T_OK;
}

// 취소종목확인
int CTranCmn::fnAPP_PMEAcceptCanSeqTranConfirm()
{
	int		ScrNum = 0, nIndex = 1;
	CString GetKeyStr("");

	ScrNum = 942;											// 출고수량입력

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayString(nIndex++, GetSprintf("%-40.40s", &PMERecvData.Data[Accept.SelStockIndex-1][20]));
															// 청약취소종목명
	m_pDevCmn->fnSCR_DisplayAmount(nIndex++, GetString(&PMERecvData.Data[Accept.SelStockIndex-1][60], 15, 15), 15);
															// 청약신청수량
	m_pDevCmn->fnSCR_DisplayAmount(nIndex++, GetString(&PMERecvData.Data[Accept.SelStockIndex-1][75], 15, 15), 15);
															// 청약취소증거금
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);

	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	if (GetKeyStr == S_CONFIRM)									// 조회거래에 대한 확인
	{
		memcpy(Accept.StocksCode, &PMERecvData.Data[Accept.SelStockIndex-1][0], 20);// 청약증거금
		return T_OK;
	}
	else 
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);								// 거래취소처리
	else 
	if (GetKeyStr == S_TIMEOVER)								// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);


	return T_OK;
}

// 차액계좌선택
int CTranCmn::fnAPP_PMEAcceptRemitTranSel()
{
	int		ScrNum = 0;
	CString GetKeyStr("");

/////////////////////////////////////////////////////////////////////////////
	Accept.RemitAccountNumSize = 0;								// 차액입금계좌번호길이
	memset(Accept.RemitAccountNum, ' ', sizeof(Accept.RemitAccountNum));	
																// 차액입금계좌번호
/////////////////////////////////////////////////////////////////////////////
	ScrNum = 524;

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
																// 계좌번호선택
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelDepRejectProc(T_CANCEL);
	else
	if (GetKeyStr == S_TIMEOVER)
		fnAPP_CancelDepRejectProc(T_TIMEOVER);
	else
	if (GetKeyStr == S_INPUTOVER)
		fnAPP_CancelDepRejectProc(T_INPUTOVER);
	else
	if ((Asc2Int(GetKeyStr) <= 0)	||							// 자료검증
		(Asc2Int(GetKeyStr) > 3)	)
		fnAPP_CancelDepRejectProc(T_INPUTERROR);
	else
	{
		Accept.RemitAccountNumSize = 12;
///		memcpy(Accept.RemitAccountNum, &RecvHead.CardData[((Asc2Int(GetKeyStr) - 1)*12) + 9], 12);
	}

	return T_OK;
}

// 일반지로 납부내역확인 1차
int CTranCmn::fnAPP_PMEAcceptJiroTaxSel()
{
	int		ScrNum = 0, StrIndex = 1;
	CString	GetKeyStr;
	CStringArray	StrReturn;
	char	szData[1024];
	int		MaxTranCnt = 0, TranCnt = 0;
	int		i = 0, j = 0;

	memset(szData, 0x00, sizeof(szData));
	memset(&Accept.JiroSelItem, 0x30, sizeof(Accept.JiroSelItem));
	memset(Accept.JiroMoney, '0', sizeof(Accept.JiroMoney));

	ScrNum = 530;

///	m_pDevCmn->fnSCR_DisplayString(1, GetString(RecvHead.CustomName, 20, 20));	// 성명
	m_pDevCmn->fnSCR_DisplayString(2, GetString(PMERecvData.Data[1], 19, 19));	// 전자납부번호
	m_pDevCmn->fnSCR_DisplayString(3, GetString(PMERecvData.Data[2], 16, 16));	// 납부기관
	
	if (Asc2Int(GetString(PMERecvData.Data[23], 1, 1).GetBuffer(0), 1))
		m_pDevCmn->fnSCR_DisplayString(4, GetSprintf("체납내역: %2.2s년 %2.2s월 - %2.2s년 %2.2s월(%3.3s개월)",
													&PMERecvData.Data[24][0],
													&PMERecvData.Data[24][2],
													&PMERecvData.Data[24][4],
													&PMERecvData.Data[24][6],
													&PMERecvData.Data[24][8]));
	else
		m_pDevCmn->fnSCR_DisplayString(4, "체납내역 없음");
	
	MaxTranCnt = __min(4, Asc2Int(&PMERecvData.Data[5], 2));	// 거래건수(4건초과시 4건만처리)

	j = 5;
	for (i = 0; i < MaxTranCnt; i++)
	{
		memset(szData, 0x00, sizeof(szData));
		Sprintf(szData, 4, "%4.4s", PMERecvData.Data[5 + i*5 + 1]);
		m_pDevCmn->fnSCR_DisplayString(j++, szData);
		memset(szData, 0x00, sizeof(szData));
		Sprintf(szData, 15, "%15.15s", Asc2Amt(PMERecvData.Data[5 + i*5 + 3], 12, 15));
		m_pDevCmn->fnSCR_DisplayString(j++, szData);
	}

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_INPUT_MODE);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();

	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);								// 거래취소처리
	else 
	if (GetKeyStr == S_TIMEOVER)								// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
	{
		SplitString(GetKeyStr, ",", StrReturn);

		for (i = 0; i < 4; i++)
		{
			if (StrReturn[0].Mid(i, 1) == "1")
			{
				TranCnt++;
				memcpy(&Accept.JiroTranDetails[24 + i*21], "Y", 1);
			}
			else
			{
				memcpy(&Accept.JiroTranDetails[24 + i*21], " ", 1);
			}
		}
		memcpy(&Accept.JiroTranDetails[2], Int2Asc(TranCnt, 2).GetBuffer(0), 2);
		memcpy(&Accept.JiroMoney[8-StrReturn[1].GetLength()], StrReturn[1].GetBuffer(0), StrReturn[1].GetLength());
	}

	return T_OK;
}

// 지로공과금 납부내역확인 2차
int CTranCmn::fnAPP_PMEAcceptJiroTaxConfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString	GetKeyStr;
	char	szData[1024];
	int		MaxTranCnt = 0, nTranCnt = 0;

	memset(szData,		0x00, sizeof(szData));

	ScrNum = 531;

///	m_pDevCmn->fnSCR_DisplayString(1, GetString(RecvHead.CustomName, 20, 20));	// 성명
	m_pDevCmn->fnSCR_DisplayString(2, GetString(PMERecvData.Data[1], 19, 19));	// 전자납부번호
	m_pDevCmn->fnSCR_DisplayString(3, GetString(PMERecvData.Data[2], 16, 16));	// 납부기관
	
	if (Asc2Int(GetString(PMERecvData.Data[23], 1, 1).GetBuffer(0), 1))
		m_pDevCmn->fnSCR_DisplayString(4, GetSprintf("체납내역: %2.2s년 %2.2s월 - %2.2s년 %2.2s월(%3.3s개월)",
													&PMERecvData.Data[24][0],
													&PMERecvData.Data[24][2],
													&PMERecvData.Data[24][4],
													&PMERecvData.Data[24][6],
													&PMERecvData.Data[24][8]));
	else
		m_pDevCmn->fnSCR_DisplayString(4, "체납내역 없음");

	for (int i = 0; i < 4; i++)
	{
		if (Accept.JiroTranDetails[24 + i*21] == 'Y')
		{
			Sprintf(szData, 24, "   %4.4s  %15.15s", PMERecvData.Data[5 + i*5 + 1],
													 Asc2Amt(PMERecvData.Data[5 + i*5 + 3], 12, 15));
			m_pDevCmn->fnSCR_DisplayString(i + 5, szData);		// 납부금액
			nTranCnt++;
		}
	}

	m_pDevCmn->fnSCR_DisplayNumeric(9, nTranCnt);							// 납부건수
	m_pDevCmn->fnSCR_DisplayAmount(10, GetString(Accept.JiroMoney, 8, 8));	// 금액

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	
	if (GetKeyStr == S_CONFIRM)									// 1차 조회거래에 대한 확인
	{
		return T_OK;
	}
	else 
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);								// 거래취소처리
	else 
	if (GetKeyStr == S_TIMEOVER)								// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

// 지로공과금 재정납부 납부내역확인
int CTranCmn::fnAPP_PMEAcceptJiroFinConfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString	GetKeyStr;
	char	szData[1024];

	if (TranCode == TC_JRTRAN)	 								// 지로공과금납부
	{
		memset(Accept.Money, '0', sizeof(Accept.Money));		// 금액(만원), 금액(만천원)
		memset(szData, 0x00, sizeof(szData));

		ScrNum = 533;											// 재정납부 확인

		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(PMERecvData.Data[2], 20, 20));	// 납부기관명
///		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(RecvHead.CustomName, 20, 20));	// 납부자성명
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(PMERecvData.Data[1], 20, 20));	// 전자납부번호
		if (Accept.JiroTranCode == TC_JRPHONE)
			sprintf(szData, "KT전화요금");
		else
		if (Accept.JiroTranCode == TC_JRELEC)
			sprintf(szData, "전기요금");
		else
		if (Accept.JiroTranCode == TC_JRINSURE)
			sprintf(szData, "건강보험료");
		else
		if (Accept.JiroTranCode == TC_JRPANS)
			sprintf(szData, "국민연금");
		else
		if (Accept.JiroTranCode == TC_JRJIRO)
			sprintf(szData, "지로대금");
		else
		if (Accept.JiroTranCode == TC_JRNTAX)
			sprintf(szData, "국고/국세");
		else
		if (Accept.JiroTranCode == TC_JRLTAX)
			sprintf(szData, "지방세/세외수입");
		else
		if (Accept.JiroTranCode == TC_JRWATER)
			sprintf(szData, "상하수도요금");
		else
		if (Accept.JiroTranCode == TC_JRTRAFF)
			sprintf(szData, "교통범칙금");
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, szData);									// 납부종류
		Sprintf(szData, 8, "%2.2s-%2.2s-%2.2s", 
							GetString(&PMERecvData.Data[5][2], 2, 2),						// 2007-01-03-#4 index조정 0->2
							GetString(&PMERecvData.Data[5][4], 2, 2),						// 2007-01-03-#4 index조정 2->4
							GetString(&PMERecvData.Data[5][6], 2, 2));						// 2007-01-03-#4 index조정 4->6
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(szData, 8, 8));				// 납부기한
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, (PMERecvData.Data[6][0] == '0') ? "납기전" :
																					 "납기후");
																							// 납부전후구분
//		m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, (LPCTSTR)RecvHead.TranFee, 5);			// 납부수수료
		m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, (LPCTSTR)&PMERecvData.Data[7], 12);		// 납부금액
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(PMERecvData.Data[8], 15, 15));	// 납부자번호
		Sprintf(szData, 7,  "%2.2s-%4.4s",
							GetString(&PMERecvData.Data[10][0], 2, 2),
							GetString(&PMERecvData.Data[10][2], 4, 4));
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(szData, 7, 7));				// 징수계좌번호

		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
		GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
		
		if (GetKeyStr == S_CONFIRM)								// 1차 조회거래에 대한 확인
		{
			memcpy(Accept.JiroMoney, &PMERecvData.Data[7][4], sizeof(Accept.JiroMoney));
																// 거래금액저장
			return T_OK;
		}
		else 
		if (GetKeyStr == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
		else 
		if (GetKeyStr == S_TIMEOVER)							// TIMEOVER
			fnAPP_CancelProc(T_TIMEOVER);
		else
			fnAPP_CancelProc(T_PROGRAM);
	}
	return T_OK;
}

// 지로납부내역 조회확인
int CTranCmn::fnAPP_PMEAcceptJiroInqComfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString	GetKeyStr;
	char	szData[1024];
	int		MaxTranCnt = 0;

	memset(szData,		0x00, sizeof(szData));
	memset(Accept.JiroMoney, '0', sizeof(Accept.JiroMoney));
	
	ScrNum = 777;
/*
	m_pDevCmn->fnSCR_DisplayString(1, GetString(RecvHead.CustomName, 20, 20));	// 성명
	m_pDevCmn->fnSCR_DisplayString(2, GetString(PMERecvData.Data[2], 16, 16));	// 납부기관
	m_pDevCmn->fnSCR_DisplayString(3, GetString(PMERecvData.Data[1], 19, 19));	// 전자납부번호
	
	MaxTranCnt = __min(4, Asc2Int(&PMERecvData.Data[4], 1));	// 거래건수(4건초과시 4건만처리)

	for (int i = 0; i < MaxTranCnt; i++)
	{
		sprintf(szData, " %2.2s%2.2s      %15.15s %2.2s-%2.2s-%2.2s   %2.2s-%4.4s",
							&PMERecvData.Data[(i*4)+5][0],
							&PMERecvData.Data[(i*4)+5][2],
							Asc2Amt(PMERecvData.Data[(i*4)+6], 12, 15),
							&PMERecvData.Data[(i*4)+7][0],
							&PMERecvData.Data[(i*4)+7][2],
							&PMERecvData.Data[(i*4)+7][4],
							&PMERecvData.Data[(i*4)+8][0],
							&PMERecvData.Data[(i*4)+8][2]);
		m_pDevCmn->fnSCR_DisplayString(i + 4, szData);
	}
*/	
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	
	if (GetKeyStr == S_CONFIRM)									// 1차 조회거래에 대한 확인
		return T_OK;
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);								// 거래취소처리
	else 
	if (GetKeyStr == S_TIMEOVER)								// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

// 재정 지로납부내역 조회확인(국세,지방세,범칙금)
int CTranCmn::fnAPP_PMEAcceptJiroFinInqComfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString	GetKeyStr;
	char	szData[1024];

	memset(szData, 0x00, sizeof(szData));
	Accept.SlipChoiceFlag = FALSE;								// 기본 : 명세표미인자처리

	return T_OK;
}

// 무매체입금 조회확인
int CTranCmn::fnAPP_PMEAcceptTaDepositConfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString GetKeyStr("");
	CString	cstrTemp("");

	ScrNum = 516;

//	m_pDevCmn->fnSCR_DisplayString(StrIndex++, (LPCSTR)&m_RXATM3061.depositreslist[0], 16);		// 수취계좌번호
//	m_pDevCmn->fnSCR_DisplayString(StrIndex++, (LPCSTR)&m_RXATM3061.depositreslist[16+40], 20);	// 수취인성명
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();

	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (GetKeyStr == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (GetKeyStr == S_CONFIRM)
		return T_OK;
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}
// 타행입금조회 
int CTranCmn::fnAPP_PMEAcceptDepConfirm()
{
	int		ScrNum = 518;
	CString	strReturn = _T("");
	int		StrIndex = 1;

																// 계좌번호
	m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_017x);
																// 성명
	if((m_pProfile->DEVICE.ETCDevice20) == KWANGJU && (CardBank == KAWBANK))   //광주은행 입금 시 018x필드 사용 #N0181
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_018x);
	else
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_021x);
																// 수수료
	m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, m_RD.byDataField_024x);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == S_CONFIRM)
		return T_OK;
	else
	if (strReturn == S_CANCEL)  //#SJ 1004 수취조회 반환
		if (m_pProfile->DEVICE.MachineType == U8100)
			fnAPP_CancelDepRejectProc(T_CANCEL);
		else
			fnAPP_CancelProc(T_CANCEL);
	else 
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelDepRejectProc(T_TIMEOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);

		
	return T_OK;
}
// 타행입금조회 
int CTranCmn::fnAPP_PMEAcceptMuDepConfirm()
{
	CString	strReturn = "";
	int		StrIndex = 1;
	int		ScrNum = 526;


	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == S_CONFIRM)
		return T_OK;
	else
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else 
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);
		
	return T_OK;
}

//////////////////////////////////////////////////////////////////////
// 준비완처리
int CTranCmn::fnAPP_ReadyProc()
{
	if (!m_pDevCmn->TranResult)
	{
		fnAPP_PMDProc();
	}
	return T_OK;
}

// 준비완결과인자
int CTranCmn::fnAPP_ReadyPrintResult(int PrintDevice, int WaitFlg)
{
	return T_OK;
}

// 개국처리
int CTranCmn::fnAPP_OpenProc()
{
	char	Pdate[9];
//////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
	{
		GetDate(Pdate);											// 일자
		m_pDevCmn->fnAPL_SetDate(Pdate);						// 운영일자 : 2002.05.21
		m_pDevCmn->fnAPL_SetCheckDate(Pdate);					// 수표인자일자

		m_pProfile->GetTransProfile();							// 마감후초기화 : 2002.08.09
		m_pProfile->TRANS.MagamFlg = OFF;
		m_pProfile->PutTransProfile();
		memcpy(OpenInfo.WithCashCount, "100", 3);
		memcpy(OpenInfo.WithCheckCount,	"010", 3);
		
		return T_OK;											// SEND OK
	}

/////////////////////////////////////////////////////////////////////////////

	return T_OK;
}

// 개국결과인자
int CTranCmn::fnAPP_OpenPrintResult(int PrintDevice, LPCTSTR szDate, LPCTSTR szTime, int WaitFlg)
{
	int		HiddenFlg = FALSE;									// 기능삭제
	int		ll = 0;

	if (PrintDevice & DEV_SPR)									// 개국인자는 저널만 수행
		PrintDevice &= ~DEV_SPR;
	
	if (PrintDevice == DEV_NONE)
		return T_OK;

	memset(ShuData, 0, sizeof(ShuData));						// Shu출력영역

	// 1 line
	fnAPP_EditPrintLine(DEV_JPR, ShuData[ll++], 0, HiddenFlg);
	// 2 line
	sprintf(ShuData[ll++], "%4.4s년%2.2s월%2.2s일 %2.2s:%2.2s:%2.2s",
							&szDate[0], &szDate[4], &szDate[6],
							&szTime[0], &szTime[2], &szTime[4]);

	sprintf(ShuData[ll++], GetSprintf("POWER OFF TIME: %s", RegGetStr(_REGKEY_DEVICE, "POWER_OFF_TIME", "00000000000000")));
	sprintf(ShuData[ll++], GetSprintf("자동기기기번호: %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum));
	sprintf(ShuData[ll++], GetSprintf("자동기제조번호: %20.20s",    m_pProfile->NETWORK.MachineMakeNum));
	sprintf(ShuData[ll++], "수수료조회설정: %s", (m_pProfile->TRANS.TranInqFee) ? "사용" : "사용안함");

	sprintf(ShuData[ll++], GetSprintf("자최대방출금액: %d", m_pProfile->TRANS.JaMaxDispAmt));
	sprintf(ShuData[ll++], GetSprintf("타최대방출금액: %d", m_pProfile->TRANS.TaMaxDispAmt));
	sprintf(ShuData[ll++], GetSprintf("명세표사용설정: %s", (m_pProfile->DEVICE.SHUHandler) ? "사용" : "사용안함"));
	sprintf(ShuData[ll++], GetSprintf("사운드설정정보: %d", m_pProfile->DEVICE.SoundVolume));
	sprintf(ShuData[ll++], "AP버전        : %s", VERSION);
	sprintf(ShuData[ll++], "개국결과      : %s", (m_pDevCmn->TranResult) ? "OK" : "NG");

	//─────────────────────────────  EMV 거래초기화 #N9999
	fnAPP_EMVInitProc();
	fnAPP_EMVOpenSetProc();
	//───────────────────────────────
	fnAPD_ShuPrint(PrintDevice, JNL_TRAN, WaitFlg);						// 명세표&저널인자

	return T_OK;
}

// Des개국결과인자
int CTranCmn::fnAPP_OpenDesPrintResult(int PrintDevice, int WaitFlg)
{
	int		ll = 0;

	if (PrintDevice & DEV_SPR)									// 개국인자는 저널만 수행(우리은행)
		PrintDevice &= ~DEV_SPR;
	
	if (PrintDevice == DEV_NONE)
		return T_OK;

	if (!AddSerialFlg)											// 일련번호증가무
	{
		AddSerialFlg = TRUE;									// 일련번호증가유
		m_pDevCmn->fnAPL_AddSerialNo();							// 거래일련번호증가
	}

	memset(ShuData, 0, sizeof(ShuData));						// Shu출력영역

	char	CurDate[9];											// 기기 일자
	char	CurTime[7];											// 기기 시간

	memset(CurDate, 0x00, sizeof(CurDate));
	memset(CurTime, 0x00, sizeof(CurTime));
	GetDate(CurDate);
	GetTime(CurTime);
	
	sprintf(ShuData[ll++], "저널시간: %2.2s시 %2.2s분 %2.2s초",
							&CurTime[0], &CurTime[2], &CurTime[4]);
	if ((m_pDevCmn->TranResult)		&&
		(m_pDevCmn->DesTranStatus)	)
	{
		if (TranCode == TC_TMKLOAD)
			sprintf(ShuData[ll++], "TMK Loading Success");
		else
			sprintf(ShuData[ll++], "TPK Loading Success");
	}
	else
	{
		if (TranCode == TC_TMKLOAD)
			sprintf(ShuData[ll++], "TMK Loading Fail");
		else
			sprintf(ShuData[ll++], "TPK Loading Fail");
	}

	fnAPD_ShuPrint(PrintDevice, JNL_TRAN, WaitFlg);						// 명세표&저널인자
	
	return T_OK;
}

// 합계처리
int CTranCmn::fnAPP_SumProc(int InputID)
{
	char	iCount[11] = { "0000000001" };
	char	iMoney[10];											// 금액(만원), 금액(만천원) : 거래(입/츨) 총금액
	char	iCashMoney[10];										// 금액(만원), 금액(만천원)	: 거래(입/츨) 현금액
	char	iCheckMoney[10];									// 금액(만원), 금액(만천원)	: 거래(입/츨) 수표액
	char	iCheckMotherMoneyM[10];								// 금액(만원)				: 거래(입)    수표액
	char	iCheckMotherMoneyO[10];								// 금액(만원)				: 거래(입)    수표액
	char	iCheckOtherMoney[10];								// 금액(만원)				: 거래(입)    수표액

	FinishFlag = '0';

	int		i = 0, j = 0;
	int		nSubFlag = 0;
	
	memset(iMoney,			  '0', sizeof(iMoney)			 );
	memset(iCashMoney,		  '0', sizeof(iCashMoney)		 );
	memset(iCheckMoney,		  '0', sizeof(iCheckMoney)		 );
	memset(iCheckMotherMoneyM,'0', sizeof(iCheckMotherMoneyM));
	memset(iCheckMotherMoneyO,'0', sizeof(iCheckMotherMoneyO));
	memset(iCheckOtherMoney,  '0', sizeof(iCheckOtherMoney)	 );
	
	

// 	memcpy(&iMoney			  [10 - sizeof(Accept.Money)			], Accept.Money,				sizeof(Accept.Money)	 );
// 	memcpy(&iCashMoney		  [10 - sizeof(Accept.CashMoney)		], Accept.CashMoney,			sizeof(Accept.CashMoney) );
// 	memcpy(&iCheckMoney		  [10 - sizeof(Accept.CheckMoney)		], Accept.CheckMoney,			sizeof(Accept.CheckMoney));
// 	memcpy(&iCheckMotherMoneyM[10 - sizeof(Accept.MotherCheckMoneyM)], Accept.MotherCheckMoneyM,	sizeof(Accept.MotherCheckMoneyM));
// 	memcpy(&iCheckMotherMoneyO[10 - sizeof(Accept.MotherCheckMoneyO)], Accept.MotherCheckMoneyO,	sizeof(Accept.MotherCheckMoneyO));
// 	memcpy(&iCheckOtherMoney  [10 - sizeof(Accept.OtherCheckMoney)  ], Accept.OtherCheckMoney,	sizeof(Accept.OtherCheckMoney) );
	memcpy(&iMoney			  [0], &Accept.Money[5],				10);
	memcpy(&iCashMoney		  [0], &Accept.CashMoney[5],			10);
	memcpy(&iCheckMoney		  [0], &Accept.CheckMoney[5],			10);
	memcpy(&iCheckMotherMoneyM[0], &Accept.MotherCheckMoneyM[5],	10);
	memcpy(&iCheckMotherMoneyO[0], &Accept.MotherCheckMoneyO[5],	10);
	memcpy(&iCheckOtherMoney  [0], &Accept.OtherCheckMoney[5],		10);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SumProc = iMoney :%10.10s iCashMoney :%10.10s InputID :%d", iMoney, iCashMoney, InputID);


	switch (InputID) 
	{
		case 0:													// 지급
			m_pProfile->GetWithTotal();

			if (FinishFlag == 'A')
				nSubFlag = 0;									// 마감전
			else
				nSubFlag = 5;									// 마감후

			if (CancelWithProcFlg)
			{
				if (Asc2Int(iMoney, sizeof(iMoney)))			// 3 : 출금취소
				{
					if (TranCode2 == TRANID_4320)
					{
						AddString(m_pProfile->WITHTOTAL.DATA[1].Amount, iMoney, 10);
						AddString(m_pProfile->WITHTOTAL.DATA[1].Count, iCount, 10);					
					}
					else
					if (TranCode2 == TRANID_4340)
					{
						AddString(m_pProfile->WITHTOTAL.DATA[2].Amount, iMoney, 10);
						AddString(m_pProfile->WITHTOTAL.DATA[2].Count, iCount, 10);					
					}

					AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+3].Amount, iMoney, 10);
					AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+3].Count, iCount, 10);
				}
			}
			else
			{
				if (Asc2Int(iCashMoney, sizeof(iCashMoney)))
				{												// 0 : 현금출금  
					if(TranCode2 == TRANID_4310 || TranCode2 == TRANID_4210)  
					{
						AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+0].Amount, iCashMoney, 10);
						AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+0].Count, iCount, 10);
					}
					else
					if(TranCode2 == TRANID_4330)               //  4 : 서비스출금 
					{
						AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+4].Amount, iCashMoney, 10);
						AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+4].Count, iCount, 10);
					}

				}

				if (Asc2Int(iCheckMoney, sizeof(iCheckMoney)))
				{												// 1 : 수표출금
					AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+1].Amount, iCheckMoney, 10);
					AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+1].Count, iCount, 10);
				}
																// 2 : Total
				AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+2].Amount, iMoney, 10);
				AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+2].Count, iCount, 10);
			}
			
			m_pProfile->PutWithTotal();
			break;

		case 1:													// 이체(대체)
			m_pProfile->GetTransTotal();

			if (FinishFlag == 'A')
				nSubFlag = 0;									// 마감전
			else
				nSubFlag = 0;									// 마감후


			if (TranCode2 == TRANID_4370)
			{
				AddString(m_pProfile->TRANSTOTAL.DATA[0].Amount, iMoney, 10);
				AddString(m_pProfile->TRANSTOTAL.DATA[0].Count, iCount, 10);			
			}
			else
			if (TranCode2 == TRANID_4390)
			{
				AddString(m_pProfile->TRANSTOTAL.DATA[1].Amount, iMoney, 10);
				AddString(m_pProfile->TRANSTOTAL.DATA[1].Count, iCount, 10);					
			}
			else
			if (TranCode2 == TRANID_4350)
			{
				AddString(m_pProfile->TRANSTOTAL.DATA[2].Amount, iMoney, 10);
				AddString(m_pProfile->TRANSTOTAL.DATA[2].Count, iCount, 10);					
			}

			AddString(m_pProfile->TRANSTOTAL.DATA[3].Amount, iMoney, 10);
			AddString(m_pProfile->TRANSTOTAL.DATA[3].Count, iCount, 10);			


			m_pProfile->PutTransTotal();
			break;

		case 2:													// 입금
			m_pProfile->GetDepTotal();

			if (FinishFlag == 'A')
				nSubFlag = 0;									// 마감전
			else												// 마감후
				nSubFlag = 4;

			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		// 0 : 현금입금
			{
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+0].Amount, iCashMoney, 10);
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+0].Count, iCount, 10);
			}

			if (Asc2Int(iCheckMotherMoneyO, sizeof(iCheckMotherMoneyO)))
			{													// 1 : 당행자점수표합계
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+1].Amount, iCheckMotherMoneyO, 10);
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+1].Count, iCount, 10);
			}

			if (Asc2Int(iCheckMotherMoneyM, sizeof(iCheckMotherMoneyO)))
			{													// 1 : 당행타점수표합계
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+1].Amount, iCheckMotherMoneyM, 10);
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+1].Count, iCount, 10);
			}

			if (Asc2Int(iCheckOtherMoney, sizeof(iCheckOtherMoney)))
			{													// 2 : 타행수표합계
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+2].Amount, iCheckOtherMoney, 10);
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+2].Count, iCount, 10);
			}


			m_pProfile->PutDepTotal();
			break;
		case 3:													// 입금
			m_pProfile->GetDepTotal();

			if (FinishFlag == 'A')
				nSubFlag = 0;									// 마감전
			else												// 마감후
				nSubFlag = 4;
			
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  : 현금입금(평일)
			{
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+3].Amount, iCashMoney, 10);
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+3].Count, iCount, 10);
			}
			m_pProfile->PutDepTotal();
			break;
		case 4:													// 입금
			m_pProfile->GetDepTotal();

			if (FinishFlag == 'A')
				nSubFlag = 0;									// 마감전
			else												// 마감후
				nSubFlag = 4;
			
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  : 현금입금(휴일)
			{
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+4].Amount, iCashMoney, 10);
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+4].Count, iCount, 10);
			}
			m_pProfile->PutDepTotal();
			break;
		default:
			break;
	}

	return T_OK;
}


// 입금장애
int CTranCmn::fnAPP_DepErrSumProc(int InputID)
{

	if (TranProc == TRAN_DEP)
		;
	else
		return T_OK;

	char	iCount[11] = { "0000000001" };
	char	iMoney[10];											// 금액(만원), 금액(만천원) : 거래(입/츨) 총금액
	char	iCashMoney[10];										// 금액(만원), 금액(만천원)	: 거래(입/츨) 현금액
	char	iCheckMoney[10];									// 금액(만원), 금액(만천원)	: 거래(입/츨) 수표액
	char	iCheckMotherMoneyM[10];								// 금액(만원)				: 거래(입)    수표액
	char	iCheckMotherMoneyO[10];								// 금액(만원)				: 거래(입)    수표액
	char	iCheckOtherMoney[10];								// 금액(만원)				: 거래(입)    수표액
	char    szTmp[17];
	PrintERRSTACK PERR;

	int		i = 0, j = 0;
	int		nSubFlag = 0;
	
	memset(iMoney,			  '0', sizeof(iMoney)			 );
	memset(iCashMoney,		  '0', sizeof(iCashMoney)		 );
	memset(iCheckMoney,		  '0', sizeof(iCheckMoney)		 );
	memset(iCheckMotherMoneyM,'0', sizeof(iCheckMotherMoneyM));
	memset(iCheckMotherMoneyO,'0', sizeof(iCheckMotherMoneyO));
	memset(iCheckOtherMoney,  '0', sizeof(iCheckOtherMoney)	 );
	memset(szTmp,             ' ', sizeof(szTmp)	 );

// 	memcpy(&iMoney			  [10 - sizeof(Accept.Money)			], Accept.Money,				sizeof(Accept.Money)	 );
// 	memcpy(&iCashMoney		  [10 - sizeof(Accept.CashMoney)		], Accept.CashMoney,			sizeof(Accept.CashMoney) );
// 	memcpy(&iCheckMoney		  [10 - sizeof(Accept.CheckMoney)		], Accept.CheckMoney,			sizeof(Accept.CheckMoney));
// 	memcpy(&iCheckMotherMoneyM[10 - sizeof(Accept.MotherCheckMoneyM)], Accept.MotherCheckMoneyM,	sizeof(Accept.MotherCheckMoneyM));
// 	memcpy(&iCheckMotherMoneyO[10 - sizeof(Accept.MotherCheckMoneyO)], Accept.MotherCheckMoneyO,	sizeof(Accept.MotherCheckMoneyO));
// 	memcpy(&iCheckOtherMoney  [10 - sizeof(Accept.OtherCheckMoney)  ], Accept.OtherCheckMoney,	sizeof(Accept.OtherCheckMoney) );
	memcpy(&iMoney			  [0], &Accept.Money[5],				10);
	memcpy(&iCashMoney		  [0], &Accept.CashMoney[5],			10);
	memcpy(&iCheckMoney		  [0], &Accept.CheckMoney[5],			10);
	memcpy(&iCheckMotherMoneyM[0], &Accept.MotherCheckMoneyM[5],	10);
	memcpy(&iCheckMotherMoneyO[0], &Accept.MotherCheckMoneyO[5],	10);
	memcpy(&iCheckOtherMoney  [0], &Accept.OtherCheckMoney[5],		10);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_DepErrSumProc = iMoney :%10.10s iCashMoney :%10.10s InputID :%d", iMoney, iCashMoney, InputID);

	StrAllSpace(PERR);

	sprintf(PERR.DateTime_Serial_Amt   , "%2.2s/%2.2s-%2.2s:%2.2s-%6.6s %10.10s", &GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6], &GetTime().GetBuffer(0)[0], &GetTime().GetBuffer(0)[2], m_pProfile->TRANS.SerialNo, iMoney);
	EditString(&m_pTranCmn->Accept.AccountNum, 16, "$$#####$$$$$##", szTmp, '#', '$', '*', TRUE);


	if(TranProc == TRAN_DEP)
		sprintf(PERR.TranType_Account   , "입금업무  %16.16s", szTmp);
	else
		sprintf(PERR.TranType_Account   , "운영업무  %16.16s", szTmp);

	switch (InputID) 
	{
		case 0:													// 입금통신장애
			m_pProfile->GetDepErrTotal();
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		// 
			{
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+0].Amount, iCashMoney, 10);
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+0].Count, iCount, 10);
			}
			m_pProfile->PutDepErrTotal();
			
			memcpy(&PERR.ErrType_Result[0], "입금통신장애", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);
			break;
		case 1:													// 입금계수장애
			m_pProfile->GetDepErrTotal();
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  
			{
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+1].Amount, iCashMoney, 10);
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+1].Count, iCount, 10);
			}
			m_pProfile->PutDepErrTotal();
			memcpy(&PERR.ErrType_Result[0], "입금계수장애", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);
			break;
		case 2:													// 입금수납장애
			m_pProfile->GetDepErrTotal();			
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  
			{
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+2].Amount, iCashMoney, 10);
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+2].Count, iCount, 10);
			}
			m_pProfile->PutDepErrTotal();
			memcpy(&PERR.ErrType_Result[0], "입금수납장애", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);
			break;
		case 3:													// 입금미수취장애
			m_pProfile->GetDepErrTotal();			
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  
			{
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+3].Amount, iCashMoney, 10);
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+3].Count, iCount, 10);
			}
			m_pProfile->PutDepErrTotal();
			memcpy(&PERR.ErrType_Result[0], "입금미수취장", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);
			break;
		default:
			break;
	}

	return T_OK;
}

// 출금장애
int CTranCmn::fnAPP_WithErrSumProc(int InputID)
{

	if (TranProc == TRAN_WITH)
		;
	else
		return T_OK;

	char	iCount[11] = { "0000000001" };
	char	iMoney[10];											// 금액(만원), 금액(만천원) : 거래(입/츨) 총금액
	char	iCashMoney[10];										// 금액(만원), 금액(만천원)	: 거래(입/츨) 현금액
	char	iCheckMoney[10];									// 금액(만원), 금액(만천원)	: 거래(입/츨) 수표액
	char	iCheckMotherMoneyM[10];								// 금액(만원)				: 거래(입)    수표액
	char	iCheckMotherMoneyO[10];								// 금액(만원)				: 거래(입)    수표액
	char	iCheckOtherMoney[10];								// 금액(만원)				: 거래(입)    수표액
	char    szTmp[17];
	PrintERRSTACK PERR;

	int		i = 0, j = 0;
	int		nSubFlag = 0;
	
	memset(iMoney,			  '0', sizeof(iMoney)			 );
	memset(iCashMoney,		  '0', sizeof(iCashMoney)		 );
	memset(iCheckMoney,		  '0', sizeof(iCheckMoney)		 );
	memset(iCheckMotherMoneyM,'0', sizeof(iCheckMotherMoneyM));
	memset(iCheckMotherMoneyO,'0', sizeof(iCheckMotherMoneyO));
	memset(iCheckOtherMoney,  '0', sizeof(iCheckOtherMoney)	 );
	memset(szTmp,             ' ', sizeof(szTmp)	 );
	
// 	memcpy(&iMoney			  [10 - sizeof(Accept.Money)			], Accept.Money,				sizeof(Accept.Money)	 );
// 	memcpy(&iCashMoney		  [10 - sizeof(Accept.CashMoney)		], Accept.CashMoney,			sizeof(Accept.CashMoney) );
// 	memcpy(&iCheckMoney		  [10 - sizeof(Accept.CheckMoney)		], Accept.CheckMoney,			sizeof(Accept.CheckMoney));
// 	memcpy(&iCheckMotherMoneyM[10 - sizeof(Accept.MotherCheckMoneyM)], Accept.MotherCheckMoneyM,	sizeof(Accept.MotherCheckMoneyM));
// 	memcpy(&iCheckMotherMoneyO[10 - sizeof(Accept.MotherCheckMoneyO)], Accept.MotherCheckMoneyO,	sizeof(Accept.MotherCheckMoneyO));
// 	memcpy(&iCheckOtherMoney  [10 - sizeof(Accept.OtherCheckMoney)  ], Accept.OtherCheckMoney,	sizeof(Accept.OtherCheckMoney) );
	memcpy(&iMoney			  [0], &Accept.Money[5],				10);
	memcpy(&iCashMoney		  [0], &Accept.CashMoney[5],			10);
	memcpy(&iCheckMoney		  [0], &Accept.CheckMoney[5],			10);
	memcpy(&iCheckMotherMoneyM[0], &Accept.MotherCheckMoneyM[5],	10);
	memcpy(&iCheckMotherMoneyO[0], &Accept.MotherCheckMoneyO[5],	10);
	memcpy(&iCheckOtherMoney  [0], &Accept.OtherCheckMoney[5],		10);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_WithErrSumProc = iMoney :%10.10s iCashMoney :%10.10s InputID :%d", iMoney, iCashMoney, InputID);
	StrAllSpace(PERR);
	
	sprintf(PERR.DateTime_Serial_Amt   , "%2.2s/%2.2s-%2.2s:%2.2s-%6.6s %10.10s", &GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6], &GetTime().GetBuffer(0)[0], &GetTime().GetBuffer(0)[2], m_pProfile->TRANS.SerialNo, iMoney);
	EditString(&m_pTranCmn->Accept.AccountNum, 16, "$$#####$$$$$##", szTmp, '#', '$', '*', TRUE);

	if(TranProc == TRAN_WITH)
		sprintf(PERR.TranType_Account   , "출금업무  %16.16s", szTmp);
	else
		sprintf(PERR.TranType_Account   , "운영업무  %16.16s", szTmp);

	switch (InputID) 
	{
		case 0:													// 출금통신장애
			m_pProfile->GetWithErrTotal();
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		// 
			{
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+0].Amount, iCashMoney, 10);
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+0].Count, iCount, 10);
			}
			m_pProfile->PutWithErrTotal();
			memcpy(&PERR.ErrType_Result[0], "출금통신장애", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);
			break;
		case 1:													// 출금계수장애
			m_pProfile->GetWithErrTotal();
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  
			{
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+1].Amount, iCashMoney, 10);
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+1].Count, iCount, 10);
			}
			m_pProfile->PutWithErrTotal();
			memcpy(&PERR.ErrType_Result[0], "출금계수장애", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);			
			break;
		case 2:													// 출금셔터장애
			m_pProfile->GetWithErrTotal();			
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  
			{
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+2].Amount, iCashMoney, 10);
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+2].Count, iCount, 10);
			}
			m_pProfile->PutWithErrTotal();
			memcpy(&PERR.ErrType_Result[0], "출금셔터장애", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);			
			break;
		case 3:													// 현금미수취장애
			m_pProfile->GetWithErrTotal();			
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  
			{
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+3].Amount, iCashMoney, 10);
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+3].Count, iCount, 10);
			}
			m_pProfile->PutWithErrTotal();
			memcpy(&PERR.ErrType_Result[0], "현금미수취장", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);			
			break;
		default:
			break;
	}

	return T_OK;
}




// Sum Clear (Kim.Gi.Jin 0710 : Mini-ATM plused next spec)
int CTranCmn::fnAPP_SumClear(int InputID)
{
	CString		strDispCntData("");								// 2004.11.05
	CString		strCashCntData("");
	CString		strTotalCashCntData("");

/////////////////////////////////////////////////////////////////////////////
	SumClearFlag = TRUE;										// 2004.08.23
/////////////////////////////////////////////////////////////////////////////
	

	m_pProfile->InitWithTotal(1);
	m_pProfile->InitDepTotal(1);
	m_pProfile->InitTransTotal();
	m_pProfile->InitTransTotal();
	m_pProfile->InitDepErrTotal();
	m_pProfile->InitWithErrTotal();
	m_pProfile->ClearAllErrCancel();
	m_pProfile->ClearAllErrTran();
	m_pProfile->ClearAllErrStack();
	m_pProfile->DetailHClearAllErrStack();

	return T_OK;
}


/****************************************************************************
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:          ** 현송업체용 (1) **
// 2:========================================
// 3:ATM ID   : VANCD9910
// 4:시작일자 : NNNNNNNN
// 5:종료일자 : NNNNNNNN
// 6:출금총액 : 
// 7:출금건수 : 
// 8:취소총액 : 
// 9:취소건수 :
//10:현송금액 :
//11:잔    액 :                 
//13:조회건수 :
//14:이체건수 :
//15:현송금액 :
//16:잔    액 :
//17:회수금액 :
//18:고객이용건수:
//19:고객이용금액:
//20:( ** )장애건수:
//22:(취소)장애건수:
//23:(JAM) 장애건수:
//24:(CST) 회수건수:
*****************************************************************************/
int CTranCmn::fnAPP_MiniATM_SumPrint1(int PrintDevice, int WaitFlag)
{

	return T_OK;
}


/****************************************************************************
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:          ** 현송업체용 (2) **
// 2:========================================
// 3:ATM ID   : VANCD9910
// 4:이전현송일자 : 
// 5:현재마감일자 : 
// 6:이전현송금액 :
// 7:          ***** 합계 (2) *****                       
// 8:서버출금총액 : 
// 9:서버출금건수 : 
//10:서버취소총액 : 
//11:서버취소건수 :
//13:
//14:
//15:
//16:
//17:
//18:
//19:
//20:
//21:
//22:이 용 해 주 셔 서 감 사 합 니 다.
//23:마이캐쉬존 MyCashzone 마이캐쉬존
//24:
*****************************************************************************/
int CTranCmn::fnAPP_MiniATM_SumPrint2(int PrintDevice, int WaitFlag)
{
	DERRINFO	ErrCancel[ErrHost_MaxNumber];
	DERRINFO	ErrTran[ErrHost_MaxNumber];
	PrintERRSTACK	DetailErrTran[ErrSDeTailHost_MaxNumber];
//	DERRINFO	ErrHost[ErrHost_MaxNumber];
	int			ErrCancelNum = 0, ErrTranNum = 0, DetailHNum = 0;/*, ErrHostNum = 0;*/
	char		szGetDateTime[256];
	char		szDate[256];
	char		szTime[256];
	int			i = 0, j = 0;
	char		szTemp[256];
	int nRB1Cnt = 0;
	int nRB2Cnt = 0;
	int nRB3Cnt = 0;
	int nRB4Cnt = 0;
	int nDepRB1Cnt = 0;
	int nDepRB2Cnt = 0;
	int nDepRB3Cnt = 0;
	int nDepRB4Cnt = 0;
	int nCurRB1Cnt = 0;
	int nCurRB2Cnt = 0;
	int nCurRB3Cnt = 0;
	int nCurRB4Cnt = 0;
	int nRB1DispCnt = 0;
	int nRB2DispCnt = 0;
	int nRBRetract = 0;
	int nRBReject = 0;
	int n50000 = 0;
	int n5000  = 0;
	int n1000  = 0;
	int nNoteKind1 = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKind", 0);	
	//초기설정금액  
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		if (nNoteKind1 == NOTE_KIND_50000_USE) //#0057
		{
			nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원
			nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
			nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
			nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);

			nDepRB1Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0); //5만원
			nDepRB2Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", 0);
			n50000		= RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0);

		}
		else
		{
			nCurRB1Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt", 0);
			nCurRB2Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt", 0);
			nDepRB1Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_10000", 0);
			nDepRB2Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", 0);
			nDepRB3Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_10000", 0);
			n50000 = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_50000", 0);
			n5000  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_5000", 0);
			n1000  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_1000", 0);

		}

		//Retract 매수
		nRBRetract = RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //미수취
		//Reject 매수
		nRBReject  = RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //리젝트 - 불량 
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원
		nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
		nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
		nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);

		nDepRB1Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0); //5만원
		nDepRB2Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", 0);
	
		
		n50000 = 0;
		n5000  = 0;
		n1000  = 0;
		//Retract 매수
		nRBRetract = RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //미수취
		n5000      = RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //천원,5천원 - 입금
		//Reject 매수
		nRBReject  = RegGetInt(_REGKEY_BRMSTATUS, "RJB2NoteCnt",0); //리젝트 - 불량 
	}

	nRB1DispCnt = RegGetInt(_REGKEY_BRMSTATUS, "NDisp50000", 0);
	if (nRB1DispCnt < 0)
		nRB1DispCnt = 0;

	nRB2DispCnt = RegGetInt(_REGKEY_BRMSTATUS, "NDisp10000", 0);
	if (nRB2DispCnt < 0)
		nRB2DispCnt = 0;

/////////////////////////////////////////////////////////////////////////////
	m_pProfile->GetWithTotal();
	m_pProfile->GetTransTotal();
	m_pProfile->GetDepTotal();
	m_pProfile->GetDepErrTotal();
	m_pProfile->GetWithErrTotal();
	ErrCancelNum = m_pProfile->GetAllErrCancel(ErrCancel);
	ErrTranNum = m_pProfile->GetAllErrTran(ErrTran);
	DetailHNum = m_pProfile->DetailHGetAllErrStack(DetailErrTran);
//	ErrHostNum = m_pProfile->GetAllErrHost(ErrHost);

/////////////////////////////////////////////////////////////////////////////
	memset(ShuData, 0, sizeof(ShuData));

	j = 0;
	GetDateTime(szDate, szTime);
	sprintf(szGetDateTime, "%8.8s%6.6s", szDate, szTime);
	sprintf(szTemp, "%s", IniGetStr(_SP_BRM_CSTINFO_INI, "REFILL", "HOST_DATETIME_LAST", szGetDateTime));

	
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[ 합  계 ]                        ");		
		sprintf(ShuData[j++], "기기번호 : %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
		sprintf(ShuData[j++], "이전합계 : %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szTemp[0], &szTemp[4], &szTemp[6], &szTemp[8],&szTemp[10]);
		sprintf(ShuData[j++], "현재합계 : %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szGetDateTime[0], &szGetDateTime[4], &szGetDateTime[6], &szGetDateTime[8],&szGetDateTime[10]);
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[정상거래]                             ");	
		sprintf(ShuData[j++], GetSprintf("현금박스:    1     2     3     4      "));		
		sprintf(ShuData[j++], GetSprintf("방출매수: %04d  %04d  %04d  %04d      ",nRB1DispCnt,nRB2DispCnt,0,0));		
		sprintf(ShuData[j++], GetSprintf("회수매수: %04d  %04d  %04d  %04d      ",nRBRetract,0,0,0));			
		sprintf(ShuData[j++], "=======================================");	
		sprintf(ShuData[j++], "총  지폐방출 : [%04d] %10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[7].Count,10),    &m_pProfile->WITHTOTAL.DATA[7].Amount);
		sprintf(ShuData[j++], "예금    지급 : [%04d] %10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[5].Count,10),	&m_pProfile->WITHTOTAL.DATA[5].Amount);
		sprintf(ShuData[j++], "신용  서비스 : [%04d] %10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[9].Count,10),	&m_pProfile->WITHTOTAL.DATA[9].Amount);
		sprintf(ShuData[j++], "이체    거래 : [%04d] %10.10s", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[3].Count,10),	&m_pProfile->TRANSTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "입금    거래 : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPTOTAL.DATA[4].Count,10),	    &m_pProfile->DEPTOTAL.DATA[4].Amount);	
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[입금거래상세]                         ");
		if (nNoteKind1 == 0x34)
		{
			sprintf(ShuData[j++], GetSprintf("   오만     만  오천+천"));		
			sprintf(ShuData[j++], GetSprintf(" %04d매 %04d매   %04d매", nDepRB1Cnt, nDepRB2Cnt, nRBReject));		
		}
		else
		{
			sprintf(ShuData[j++], GetSprintf("   오만     만   오천     천"));		
			sprintf(ShuData[j++], GetSprintf(" %04d매 %04d매 %04d매 %04d매", n50000,nDepRB1Cnt+nDepRB2Cnt+nDepRB3Cnt,n5000,n1000));		
		}
		sprintf(ShuData[j++], "=======================================");
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{	
		sprintf(ShuData[j++], "[합계]                        ");		
		sprintf(ShuData[j++], "기번: %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
		sprintf(ShuData[j++], "이전: %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szTemp[0], &szTemp[4], &szTemp[6], &szTemp[8],&szTemp[10]);
		sprintf(ShuData[j++], "현재: %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szGetDateTime[0], &szGetDateTime[4], &szGetDateTime[6], &szGetDateTime[8],&szGetDateTime[10]);

		
		nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원
		nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
		nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
		nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);

		nDepRB1Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0); //5만원
		nDepRB2Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", 0);
	
		
		n50000 = 0;
		n5000  = 0;
		n1000  = 0;
		//Retract 매수
		nRBRetract = RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //미수취
		n5000      = RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //천원,5천원 - 입금
		//Reject 매수
		nRBReject  = RegGetInt(_REGKEY_BRMSTATUS, "RJB2NoteCnt",0); //리젝트 - 불량 
		
		sprintf(ShuData[j++], GetSprintf("잔    액: 오만(%04d)     만(%04d)", nCurRB1Cnt , nCurRB2Cnt));
		sprintf(ShuData[j++], GetSprintf("        * 회수(%04d) 리젝트(%04d)", nRBRetract , nRBReject));

		CString cstrAtm("");
		cstrAtm = IniGetStr(_SP_BRM_CSTINFO_INI, "CURR_R_T", "TOTAL_T0GAP",  ZERO10);
		sprintf(ShuData[j++], GetSprintf("장입금액:%s원", Asc2Amt(cstrAtm.GetBuffer(0) , cstrAtm.GetLength(), cstrAtm.GetLength()+3)));

		sprintf(ShuData[j++], "[정상거래]                             ");
		sprintf(ShuData[j++], GetSprintf("현금박스: 오만    일만"));		
		sprintf(ShuData[j++], GetSprintf("방출매수: %04d    %04d      ",nRB1DispCnt,nRB2DispCnt));		
		sprintf(ShuData[j++], GetSprintf("회수매수: %04d  ",nRBRetract));
		sprintf(ShuData[j++], GetSprintf("리젝매수: %04d  ",nRBReject));
		sprintf(ShuData[j++], "=======================================");	
		sprintf(ShuData[j++], "총방:[%04d]%10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[7].Count,10),    &m_pProfile->WITHTOTAL.DATA[7].Amount);
		sprintf(ShuData[j++], "예금:[%04d]%10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[5].Count,10),	&m_pProfile->WITHTOTAL.DATA[5].Amount);
		sprintf(ShuData[j++], "신용:[%04d]%10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[9].Count,10),	&m_pProfile->WITHTOTAL.DATA[9].Amount);
		sprintf(ShuData[j++], "이체:[%04d]%10.10s", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[3].Count,10),	&m_pProfile->TRANSTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "취소:[%04d]%10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[8].Count,10),	&m_pProfile->WITHTOTAL.DATA[8].Amount);		
		sprintf(ShuData[j++], "입금:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPTOTAL.DATA[4].Count,10),	   &m_pProfile->DEPTOTAL.DATA[4].Amount);	
		sprintf(ShuData[j++], "[입금거래상세]                         ");	
		sprintf(ShuData[j++], GetSprintf("  오만     만 오천+천"));
		sprintf(ShuData[j++], GetSprintf("%04d매 %04d매 %04d매", nDepRB1Cnt,nDepRB2Cnt,n5000+n1000));
		
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자 

		if (PrintDevice & DEV_SPR)
			fnAPD_MaterialRetract(DEV_SPR, TRUE);
	}	
		
	//fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자

	for(i = 0; i < j; i++ ) 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);

	memset(ShuData, 0, sizeof(ShuData));
	j = 0;	

	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[장애거래출금]                         ");		
		sprintf(ShuData[j++], "통신    장애 : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[0].Count,10), &m_pProfile->WITHERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "출금계수장애 : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[1].Count,10), &m_pProfile->WITHERRTOTAL.DATA[1].Amount);
		sprintf(ShuData[j++], "출금셔터장애 : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[2].Count,10), &m_pProfile->WITHERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "출금미수취금 : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[3].Count,10), &m_pProfile->WITHERRTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "기타    취소 : [%04d] %10.10s",  Asc2Int(&m_pProfile->WITHTOTAL.DATA[8].Count,10), &m_pProfile->WITHTOTAL.DATA[8].Amount);
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[장애거래입금]                         ");		
		sprintf(ShuData[j++], "입금통신장애 : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[0].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "입금계수장애 : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[1].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[1].Amount);	
		sprintf(ShuData[j++], "입금수납장애 : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[2].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "입금미수취금 : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[3].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[3].Amount);	
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[장애거래출금]                         ");		
		sprintf(ShuData[j++], "통신:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[0].Count,10), &m_pProfile->WITHERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "계수:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[1].Count,10), &m_pProfile->WITHERRTOTAL.DATA[1].Amount);
		sprintf(ShuData[j++], "셔터:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[2].Count,10), &m_pProfile->WITHERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "미수:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[3].Count,10), &m_pProfile->WITHERRTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "취소:[%04d]%10.10s",  Asc2Int(&m_pProfile->WITHTOTAL.DATA[8].Count,10), &m_pProfile->WITHTOTAL.DATA[8].Amount);
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[장애거래입금]                         ");		
		sprintf(ShuData[j++], "통신:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[0].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "계수:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[1].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[1].Amount);	
		sprintf(ShuData[j++], "수납:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[2].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "미수:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[3].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[3].Amount);	
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자
	}

	if (PrintDevice & DEV_SPR)
		fnAPD_MaterialRetract(DEV_SPR, TRUE);					// 명세표회수



	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);

	//PrintDevice &= ~DEV_SPR;

	memset(ShuData, 0, sizeof(ShuData));
	j = 0;
	sprintf(ShuData[j++], GetSprintf("[취소상세내역 (건수: %d건)]", ErrCancelNum));	
	if (ErrCancelNum)
	{															// 장애정보 있슴
		for (int i = 0; i < ErrCancelNum; i++)
		{			
			if ((i) && (i == ErrCancelNum))						
				break;											

			// 거래일자 - 처리번호 - 금액
			sprintf(ShuData[j++], "%02d|%24.24s", i, &ErrCancel[i].Remark[0]);
			sprintf(ShuData[j++], "**|%29.29s",      &ErrCancel[i].Remark[51]);

			if (j > 20)
			{
				fnAPD_ShuPrint(PrintDevice, JNL_CLOSE, TRUE);				// 명세표&저널인자

				//#CS0002
				if (PrintDevice & DEV_SPR)
					fnAPD_MaterialRetract(DEV_SPR, TRUE);					// 명세표회수

				memset(ShuData, 0, sizeof(ShuData));
				j = 0;
			}
		}
	}
	else
	{
		sprintf(ShuData[j++], "취소내역       : 없음");
		sprintf(ShuData[j++], "취소내역 건수  : 0000");
	}	
		

	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);
	
/////////////////////////////////////////////////////////////////////////////
	fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자
	if (PrintDevice & DEV_SPR)
	{
		fnAPD_MaterialRetract(DEV_SPR, TRUE);					// 명세표회수
	}


	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);

	//PrintDevice &= ~DEV_SPR;

	memset(ShuData, 0, sizeof(ShuData));
	j = 0;
	sprintf(ShuData[j++], GetSprintf("[장애상세내역 (건수: %d건)]", DetailHNum));	
	if (DetailHNum)
	{															// 장애정보 있슴
		for (int i = 0; i < DetailHNum; i++)
		{			
			if ((i) && (i == DetailHNum))						
				break;											

			// 거래일자 - 거래종류 - 장애종류
			memcpy(ShuData[j++], DetailErrTran[i].DateTime_Serial_Amt, strlen(DetailErrTran[i].DateTime_Serial_Amt));
			memcpy(ShuData[j++], DetailErrTran[i].TranType_Account,    strlen(DetailErrTran[i].TranType_Account));
			memcpy(ShuData[j++], DetailErrTran[i].ErrType_Result,      strlen(DetailErrTran[i].ErrType_Result));

			if (j > 20)
			{
				fnAPD_ShuPrint(PrintDevice, JNL_CLOSE, TRUE);				// 명세표&저널인자

				//#CS0002
				if (PrintDevice & DEV_SPR)
					fnAPD_MaterialRetract(DEV_SPR, TRUE);					// 명세표회수

				memset(ShuData, 0, sizeof(ShuData));
				j = 0;
			}
		}
	}
	else
	{
		sprintf(ShuData[j++], "장애내역       : 없음");
		sprintf(ShuData[j++], "장애내역 건수  : 0000");
	}	

		
	sprintf(ShuData[j++], "회수매수       : %4.4d매",   RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt"));
	sprintf(ShuData[j++], "REJECT         : %4.4d매",   RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt"));
//	sprintf(ShuData[j++], "( ** )장애건수 : %010d",	ErrCancelNum);

	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);
	
/////////////////////////////////////////////////////////////////////////////
	fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자
	if (PrintDevice & DEV_SPR)
	{
		fnAPD_MaterialRetract(DEV_SPR, TRUE);					// 명세표회수
	}

	return T_OK;
}



int CTranCmn::fnAPP_MiniATM_SumPrint3(int PrintDevice, int WaitFlag)
{
	char		szGetDateTime[256];
	char		szDate[256];
	char		szTime[256];
	int			i = 0, j = 0;
/////////////////////////////////////////////////////////////////////////////

	memset(ShuData, 0, sizeof(ShuData));

	j = 0;

	GetDateTime(szDate, szTime);
	sprintf(szGetDateTime, "%8.8s%6.6s", szDate, szTime);


	sprintf(ShuData[j++], "========================================");
	sprintf(ShuData[j++], "%10.10s** 현금 설정 금액 **", "");		// 2005.03.16
	sprintf(ShuData[j++], "========================================");
 	sprintf(ShuData[j++], "ATM SST      : %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
	j++;
	sprintf(ShuData[j++], "현재설정일자 : %4.4s-%2.2s-%2.2s %2.2s:%2.2s", &szGetDateTime[0], &szGetDateTime[4], &szGetDateTime[6], &szGetDateTime[8], &szGetDateTime[10]);
	j++;

	sprintf(ShuData[j++], "1 카세트 설정:  만원권 %d매",m_pDevCmn->FstCSTSetCash);
	sprintf(ShuData[j++], "2 카세트 설정:  만원권 %d매",m_pDevCmn->SndCSTSetCash);
	sprintf(ShuData[j++], "3 카세트 설정:  만원권 %d매",m_pDevCmn->TrdCSTSetCash);
	sprintf(ShuData[j++], "4 카세트 설정:  만원권 %d매",m_pDevCmn->FthCSTSetCash);
	sprintf(ShuData[j++], "TOTAL    금액:  만원권 %d매",m_pDevCmn->FstCSTSetCash + m_pDevCmn->SndCSTSetCash);

	j++;
	j++;
/////////////////////////////////////////////////////////////////////////////
	sprintf(ShuData[j++], "이 용 해 주 셔 서 감 사 합 니 다.");										
/////////////////////////////////////////////////////////////////////////////
	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_SumPrint3| %s ", ShuData[i]);
/////////////////////////////////////////////////////////////////////////////
	fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자
	if (PrintDevice & DEV_SPR)
	{
		fnAPD_MaterialRetract(DEV_SPR, TRUE);					// 명세표회수
	}


	return T_OK;
}
/****************************************************************************
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:          ** 현송업체용 (1) **
// 2:========================================
// 3:ATM ID   : VANCD9910
// 4:이전현송일자 : 
// 5:추가현송일자 : 
// 6:이전현송금액 :
// 7:          ***** 추가현송 (1) *****                       
// 8:추가현송총액 : 
// 9:
//10:
//11:
//13:
//14:
//15:
//16:
//17:
//18:
//19:
//20:
//21:
//22:이 용 해 주 셔 서 감 사 합 니 다.
//23:마이캐쉬존 MyCashzone 마이캐쉬존
//24:
*****************************************************************************/
int CTranCmn::fnAPP_MiniATM_SumPrint(int PrintDevice, int WaitFlag)
{
	char		szGetDateTime[256];
	char		szDate[256];
	char		szTime[256];
	int			i = 0, j = 0;
/////////////////////////////////////////////////////////////////////////////

	memset(ShuData, 0, sizeof(ShuData));

	j = 0;

	GetDateTime(szDate, szTime);
	sprintf(szGetDateTime, "%8.8s%6.6s", szDate, szTime);


	sprintf(ShuData[j++], "========================================");
	sprintf(ShuData[j++], "%10.10s** CD서버확인응답 **", "");		// 2005.03.16
	sprintf(ShuData[j++], "========================================");
 	sprintf(ShuData[j++], "ATM ID       : %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
	j++;
	sprintf(ShuData[j++], "현재설정일자 : %4.4s-%2.2s-%2.2s %2.2s:%2.2s", &szGetDateTime[0], &szGetDateTime[4], &szGetDateTime[6], &szGetDateTime[8], &szGetDateTime[10]);
	j++;
/*
	sprintf(ShuData[j++], "1 카세트 설정:  %14.14s",m_RXATM0330.cst1);
	sprintf(ShuData[j++], "2 카세트 설정:  %14.14s",m_RXATM0330.cst2);
	sprintf(ShuData[j++], "TOTAL    금액:  %14.14s",m_RXATM0330.csttot);
*/
	j++;
	j++;
/////////////////////////////////////////////////////////////////////////////
	sprintf(ShuData[j++], "이 용 해 주 셔 서 감 사 합 니 다.");										
	sprintf(ShuData[j++], "마이캐쉬존 MyCashzone 마이캐쉬존");
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_SumPrint| %s ", ShuData[i]);

	fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자
	if (PrintDevice & DEV_SPR)
	{
		fnAPD_MaterialRetract(DEV_SPR, TRUE);					// 명세표회수
	}
	return T_OK;
}

/****************************************************************************
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:          ** 현송업체용 (1) **
// 2:========================================
// 3:ATM ID   : VANCD9910
// 4:시작일자 : NNNNNNNN
// 5:종료일자 : NNNNNNNN
// 6:출금총액 : 
// 7:출금건수 : 
// 8:취소총액 : 
// 9:취소건수 :
//10:현송금액 :
//11:잔    액 :                 
//12:조회건수 :
//13:이체건수 :
//14:현송금액 :
//15:잔    액 :
//16:회수금액 :
//17고객이용건수:
//18:고객이용금액:
//19:( ** )장애건수:
//20:(취소)장애건수:
//21:(JAM) 장애건수:
//22:(CST) 회수건수:
//23:(CST) 회수건수:
//24:========================================
*****************************************************************************/
int CTranCmn::fnAPP_MiniATM_CashPrint(int PrintDevice, int WaitFlag)
{
	DERRINFO	ErrCancel[ErrHost_MaxNumber];
	DERRINFO	ErrTran[ErrHost_MaxNumber];
	PrintERRSTACK	DetailErrTran[ErrSDeTailHost_MaxNumber];
//	DERRINFO	ErrHost[ErrHost_MaxNumber];
	int			ErrCancelNum = 0, ErrTranNum = 0, DetailHNum = 0;/*, ErrHostNum = 0;*/
	char		szGetDateTime[256];
	char		szDate[256];
	char		szTime[256];
	int			i = 0, j = 0;
	char		szTemp[256];
	int nRB1Cnt = 0;
	int nRB2Cnt = 0;
	int nRB3Cnt = 0;
	int nRB4Cnt = 0;
	int nDepRB1Cnt = 0;
	int nDepRB2Cnt = 0;
	int nDepRB3Cnt = 0;
	int nDepRB4Cnt = 0;
	int nCurRB1Cnt = 0;
	int nCurRB2Cnt = 0;
	int nCurRB3Cnt = 0;
	int nCurRB4Cnt = 0;
	int nRB1DispCnt = 0;
	int nRB2DispCnt = 0;
	int nRBRetract = 0;
	int nRBReject = 0;
	int n50000 = 0;
	int n5000  = 0;
	int n1000  = 0;

	int nNoteKind1 = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKind", 0);	
	//초기설정금액  
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		if (nNoteKind1 == NOTE_KIND_50000_USE)  //#0057
		{
			nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원
			nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
			nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
			nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);

			nDepRB1Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0); //5만원
			nDepRB2Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", 0);
			n50000		= RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0);

		}
		else
		{
			nCurRB1Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt", 0);
			nCurRB2Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt", 0);
			nDepRB1Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_10000", 0);
			nDepRB2Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", 0);
			nDepRB3Cnt = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_10000", 0);
			n50000 = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_50000", 0);
			n5000  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_5000", 0);
			n1000  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB3NoteCnt_1000", 0);

		}

		//Retract 매수
		nRBRetract = RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //미수취
		//Reject 매수
		nRBReject  = RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //리젝트 - 불량 
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원
		nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
		nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
		nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);

		nDepRB1Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0); //5만원
		nDepRB2Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", 0);
	
		
		n50000 = 0;
		n5000  = 0;
		n1000  = 0;
		//Retract 매수
		nRBRetract = RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //미수취
		n5000      = RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //천원,5천원 - 입금
		//Reject 매수
		nRBReject  = RegGetInt(_REGKEY_BRMSTATUS, "RJB2NoteCnt",0); //리젝트 - 불량 

	}
 
	nRB1DispCnt = RegGetInt(_REGKEY_BRMSTATUS, "NDisp50000", 0);
	if (nRB1DispCnt < 0)
		nRB1DispCnt = 0;

	nRB2DispCnt = RegGetInt(_REGKEY_BRMSTATUS, "NDisp10000", 0);
	if (nRB2DispCnt < 0)
		nRB2DispCnt = 0;


/////////////////////////////////////////////////////////////////////////////
	m_pProfile->GetWithTotal();
	m_pProfile->GetTransTotal();
	m_pProfile->GetDepTotal();
	m_pProfile->GetDepErrTotal();
	m_pProfile->GetWithErrTotal();
	ErrCancelNum = m_pProfile->GetAllErrCancel(ErrCancel);
	ErrTranNum = m_pProfile->GetAllErrTran(ErrTran);
	DetailHNum = m_pProfile->DetailHGetAllErrStack(DetailErrTran);
//	ErrHostNum = m_pProfile->GetAllErrHost(ErrHost);

/////////////////////////////////////////////////////////////////////////////
	memset(ShuData, 0, sizeof(ShuData));

	j = 0;
	GetDateTime(szDate, szTime);
	sprintf(szGetDateTime, "%8.8s%6.6s", szDate, szTime);
	sprintf(szTemp, "%s", IniGetStr(_SP_BRM_CSTINFO_INI, "REFILL", "HOST_DATETIME_LAST", szGetDateTime));

	
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[ 소  계  ]                        ");		
		sprintf(ShuData[j++], "기기번호 : %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
		sprintf(ShuData[j++], "이전합계 : %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szTemp[0], &szTemp[4], &szTemp[6], &szTemp[8],&szTemp[10]);
		sprintf(ShuData[j++], "현재합계 : %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szGetDateTime[0], &szGetDateTime[4], &szGetDateTime[6], &szGetDateTime[8],&szGetDateTime[10]);
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[정상거래]                             ");	
		sprintf(ShuData[j++], GetSprintf("현금박스:    1     2     3     4      "));		
		sprintf(ShuData[j++], GetSprintf("방출매수: %04d  %04d  %04d  %04d      ",nRB1DispCnt,nRB2DispCnt,0,0));		
		sprintf(ShuData[j++], GetSprintf("회수매수: %04d  %04d  %04d  %04d      ",nRBRetract,0,0,0));			
		sprintf(ShuData[j++], "=======================================");	
		sprintf(ShuData[j++], "총  지폐방출 : [%04d] %10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[7].Count,10),    &m_pProfile->WITHTOTAL.DATA[7].Amount);
		sprintf(ShuData[j++], "예금    지급 : [%04d] %10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[5].Count,10),	&m_pProfile->WITHTOTAL.DATA[5].Amount);
		sprintf(ShuData[j++], "신용  서비스 : [%04d] %10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[9].Count,10),	&m_pProfile->WITHTOTAL.DATA[9].Amount);
		sprintf(ShuData[j++], "이체    거래 : [%04d] %10.10s", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[3].Count,10),	&m_pProfile->TRANSTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "입금    거래 : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPTOTAL.DATA[4].Count,10),	    &m_pProfile->DEPTOTAL.DATA[4].Amount);	
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[입금거래상세]                         ");
		if (nNoteKind1 == 0x34)
		{
			sprintf(ShuData[j++], GetSprintf("   오만     만  오천+천"));		
			sprintf(ShuData[j++], GetSprintf(" %04d매 %04d매   %04d매", nDepRB1Cnt, nDepRB2Cnt, nRBReject));		
		}
		else
		{
			sprintf(ShuData[j++], GetSprintf("   오만     만   오천     천"));		
			sprintf(ShuData[j++], GetSprintf(" %04d매 %04d매 %04d매 %04d매", n50000,nDepRB1Cnt+nDepRB2Cnt+nDepRB3Cnt,n5000,n1000));		
		}
		sprintf(ShuData[j++], "=======================================");
		//#N0258
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{	
		sprintf(ShuData[j++], "[ 소  계  ]                        ");		
		sprintf(ShuData[j++], "기번: %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
		sprintf(ShuData[j++], "이전: %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szTemp[0], &szTemp[4], &szTemp[6], &szTemp[8],&szTemp[10]);
		sprintf(ShuData[j++], "현재: %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szGetDateTime[0], &szGetDateTime[4], &szGetDateTime[6], &szGetDateTime[8],&szGetDateTime[10]);
		sprintf(ShuData[j++], "[정상거래]                             ");	
		sprintf(ShuData[j++], GetSprintf("현금박스: 오만    일만"));		
		sprintf(ShuData[j++], GetSprintf("방출매수: %04d    %04d      ",nRB1DispCnt,nRB2DispCnt));		
		sprintf(ShuData[j++], GetSprintf("회수매수: %04d  ",nRBRetract));
		sprintf(ShuData[j++], GetSprintf("리젝매수: %04d  ",nRBReject));
		sprintf(ShuData[j++], "=======================================");	
		sprintf(ShuData[j++], "총방:[%04d]%10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[7].Count,10),    &m_pProfile->WITHTOTAL.DATA[7].Amount);
		sprintf(ShuData[j++], "예금:[%04d]%10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[5].Count,10),	&m_pProfile->WITHTOTAL.DATA[5].Amount);
		sprintf(ShuData[j++], "신용:[%04d]%10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[9].Count,10),	&m_pProfile->WITHTOTAL.DATA[9].Amount);
		sprintf(ShuData[j++], "이체:[%04d]%10.10s", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[3].Count,10),	&m_pProfile->TRANSTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "입금:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPTOTAL.DATA[4].Count,10),	    &m_pProfile->DEPTOTAL.DATA[4].Amount);	
		sprintf(ShuData[j++], "[입금거래상세]                         ");	
		sprintf(ShuData[j++], GetSprintf("  오만     만 오천+천"));
		sprintf(ShuData[j++], GetSprintf("%04d매 %04d매 %04d매", nDepRB1Cnt,nDepRB2Cnt,n5000+n1000));
		//#N0258
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자 
		if (PrintDevice & DEV_SPR)
			fnAPD_MaterialRetract(DEV_SPR, TRUE);
	}	
		
//	fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자

	for(i = 0; i < j; i++ ) 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);

	memset(ShuData, 0, sizeof(ShuData));
	j = 0;	

	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[장애거래출금]                         ");		
		sprintf(ShuData[j++], "통신    장애 : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[0].Count,10), &m_pProfile->WITHERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "출금계수장애 : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[1].Count,10), &m_pProfile->WITHERRTOTAL.DATA[1].Amount);
		sprintf(ShuData[j++], "출금셔터장애 : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[2].Count,10), &m_pProfile->WITHERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "출금미수취금 : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[3].Count,10), &m_pProfile->WITHERRTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "기타    취소 : [%04d] %10.10s",  Asc2Int(&m_pProfile->WITHTOTAL.DATA[8].Count,10), &m_pProfile->WITHTOTAL.DATA[8].Amount);
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[장애거래입금]                         ");		
		sprintf(ShuData[j++], "입금통신장애 : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[0].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "입금계수장애 : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[1].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[1].Amount);	
		sprintf(ShuData[j++], "입금수납장애 : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[2].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "입금미수취금 : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[3].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[3].Amount);	
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[장애거래출금]                         ");		
		sprintf(ShuData[j++], "통신:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[0].Count,10), &m_pProfile->WITHERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "계수:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[1].Count,10), &m_pProfile->WITHERRTOTAL.DATA[1].Amount);
		sprintf(ShuData[j++], "셔터:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[2].Count,10), &m_pProfile->WITHERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "미수:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[3].Count,10), &m_pProfile->WITHERRTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "취소:[%04d]%10.10s",  Asc2Int(&m_pProfile->WITHTOTAL.DATA[8].Count,10), &m_pProfile->WITHTOTAL.DATA[8].Amount);
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[장애거래입금]                         ");		
		sprintf(ShuData[j++], "통신:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[0].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "계수:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[1].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[1].Amount);	
		sprintf(ShuData[j++], "수납:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[2].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "미수:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[3].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[3].Amount);	
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자
	}

	if (PrintDevice & DEV_SPR)
		fnAPD_MaterialRetract(DEV_SPR, TRUE);					// 명세표회수


	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);

	PrintDevice &= ~DEV_SPR;

	memset(ShuData, 0, sizeof(ShuData));
	j = 0;
	sprintf(ShuData[j++], GetSprintf("[장애상세내역 (건수: %d건)]", DetailHNum));	
	if (DetailHNum)
	{															// 장애정보 있슴
		for (int i = 0; i < DetailHNum; i++)
		{			
			if ((i) && (i == DetailHNum))						
				break;											

			// 거래일자 - 거래종류 - 장애종류
			memcpy(ShuData[j++], DetailErrTran[i].DateTime_Serial_Amt, strlen(DetailErrTran[i].DateTime_Serial_Amt));
			memcpy(ShuData[j++], DetailErrTran[i].TranType_Account,    strlen(DetailErrTran[i].TranType_Account));
			memcpy(ShuData[j++], DetailErrTran[i].ErrType_Result,      strlen(DetailErrTran[i].ErrType_Result));

			if (j > 21)
			{
				fnAPD_ShuPrint(PrintDevice, JNL_CLOSE, TRUE);				// 명세표&저널인자
				memset(ShuData, 0, sizeof(ShuData));
				j = 0;
			}
		}
	}
	else
	{
		sprintf(ShuData[j++], "장애내역       : 없음");
		sprintf(ShuData[j++], "장애내역 건수  : 0000");
	}	

	
	
	sprintf(ShuData[j++], "회수매수       : %4.4d매",   RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt"));
	sprintf(ShuData[j++], "REJECT         : %4.4d매",   RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt"));
//	sprintf(ShuData[j++], "( ** )장애건수 : %010d",	DetailHNum);

	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);
	
/////////////////////////////////////////////////////////////////////////////
	fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// 명세표&저널인자
	if (PrintDevice & DEV_SPR)
	{
		fnAPD_MaterialRetract(DEV_SPR, TRUE);					// 명세표회수
	}

	return T_OK;
}



// 합계결과인자
int CTranCmn::fnAPP_SumPrintResult(int TranId, int PrintDevice, int WaitFlg)
{
	int		HiddenFlg = FALSE;									// 기능삭제
	int		ll = 0;

	memset(ShuData, 0, sizeof(ShuData));						// Shu출력영역

	return T_OK;
}

// 마감결과인자 Coding필요
int CTranCmn::fnAPP_ClosePrintResult(int TranId, int PrintDevice, int WaitFlg)
{
	int		HiddenFlg = FALSE;									// 기능삭제
	int		ll = 0;

	if (!AddSerialFlg)											// 일련번호증가무
	{
		AddSerialFlg = TRUE;									// 일련번호증가유
		m_pDevCmn->fnAPL_AddSerialNo();							// 거래일련번호증가
	}

	m_pProfile->GetTransProfile();
	m_pProfile->TRANS.MagamFlg = ON;
	m_pProfile->PutTransProfile();

	memset(ShuData, 0, sizeof(ShuData));						// Shu출력영역

	return T_OK;
}

// 수표등록인자
int	CTranCmn::fnAPP_CheckResultPrint(int PrintDevice, int nFlag, int WaitFlg)
{
	int		HiddenFlg = FALSE;									// 기능삭제
	int		ll = 0;

	memset(ShuData, 0, sizeof(ShuData));						// Shu출력영역

	if (m_pDevCmn->TranStatus == TRAN_CHECK_REG)
		sprintf(ShuData[ll++], "        수표등록");					// 1라인
	else
		sprintf(ShuData[ll++], "        수표집계");					// 1라인 

	if(PrintDevice & DEV_SPR)
		ll++;

	sprintf(ShuData[ll++], "%8.8s  %5.5s  %4.4s  %6.6s",
							&m_pProfile->TRANS.YYYYMMDD[0],
							&m_pProfile->NETWORK.SubBranchNum[3],
							&m_pProfile->NETWORK.AtmSerialNum[4],
							m_pProfile->TRANS.SerialNo );

	if (PrintDevice & DEV_SPR)
		ll += 4;

	if (!m_pDevCmn->TranResult)
	{
		sprintf(ShuData[ll++], "수표등록 비정상");
		sprintf(ShuData[ll++], "수표시작번호,매수 확인후");
		sprintf(ShuData[ll++], "다시 등록하여 주십시오");
		if (PrintDevice & DEV_SPR)
			ll += 3;
	}
	else
	{
		sprintf(ShuData[ll++], "은행코드1 : %2.2s", m_pDevCmn->fnAPL_GetOutCheckBankNo().GetBuffer(0));
		sprintf(ShuData[ll++], "수표1 번호: %8.8s", m_pDevCmn->fnAPL_GetNumberOfOutCheckNo().GetBuffer(0));
		sprintf(ShuData[ll++], "수표1 매수: %04d", m_pDevCmn->fnAPL_GetNumberOfOutCheck());
		sprintf(ShuData[ll++], "은행코드2 : %2.2s", m_pDevCmn->fnAPL_GetOutCheckBankNo2().GetBuffer(0));
		sprintf(ShuData[ll++], "수표2 번호: %8.8s", m_pDevCmn->fnAPL_GetNumberOfOutCheckNo2().GetBuffer(0));
		sprintf(ShuData[ll++], "수표2 매수: %04d", m_pDevCmn->fnAPL_GetNumberOfOutCheck2());
	}
	if (PrintDevice & DEV_SPR)
	{
		ll += 4;
		fnAPP_EditPrintLine(PrintDevice, ShuData[ll++], 17, HiddenFlg);	// 18라인 
		fnAPP_EditPrintLine(PrintDevice, ShuData[ll++], 18, HiddenFlg);	// 19라인 
	}

	fnAPD_ShuPrint(PrintDevice, JNL_CASH, TRUE);							// 명세표&저널인자
	if (PrintDevice & DEV_SPR)
	{
		m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);				// 명세표회수
	}

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 거래장애정보설정
int CTranCmn::fnAPP_ErrorTranProc()
{
//	  0123456789012345678900123456789012345|0123456789012345678900123456789012345|012345678901234567890012345678901234567890
//    12051324  123456 01000000            |은행출금 1234567890123456            |현금미취 LC[AB] 400AB(06)     
	char szAmt[9];
	char szTmp[17];
	CString cstrGetErr("");
	DERRINFO	ERR;

	if ((TranProc == TRAN_WITH) ||								// 출금
		(TranProc == TRAN_DEP)	)								// 입금
		;
	else 
		return T_OK;

	StrAllSpace(ERR.Remark);
	ERR.Remark[25] = '|';
    ERR.Remark[50] = '|';
    ERR.Remark[76] = ' ';

	Sprintf(szAmt, 8, "%08d", Asc2Int(m_pTranCmn->Accept.Money, sizeof(m_pTranCmn->Accept.Money)));		
	
	memcpy(ERR.Remark,     &m_pProfile->TRANS.YYYYMMDD[4], 4);
	memcpy(&ERR.Remark[4], GetTime().GetBuffer(0), 4);          // 거래시간
	memcpy(&ERR.Remark[4+4+1], m_pProfile->TRANS.SerialNo, 6);		// 일련번호
	memcpy(&ERR.Remark[4+4+1+6+1], szAmt, strlen(szAmt));  			// 거래금액

	if(TranProc == TRAN_DEP)
		memcpy(&ERR.Remark[25+1], "입금업무", 8);  			// 거래종류
	else
	if(TranProc == TRAN_WITH)
		memcpy(&ERR.Remark[25+1], "출금업무", 8);  			// 거래종류
	else
		memcpy(&ERR.Remark[25+1], "기타업무", 8);  			// 


	EditString(&m_pTranCmn->Accept.AccountNum, 14, "$$#####$$$$$##", szTmp, '#', '$', '*', TRUE);

	memcpy(&ERR.Remark[26+1+8+1], szTmp, 14);
	
	if (!m_pDevCmn->fnAPL_CheckError())							// 장애
	{
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "011", 3) == 0)
		{
			if ((m_pDevCmn->CurErrBuff.ErrorCode[4] == '2')	||
				(m_pDevCmn->CurErrBuff.ErrorCode[4] == '4')	)
				memcpy(&ERR.Remark[50+1], "현금미취", 8);
			else
				memcpy(&ERR.Remark[50+1], "매체미취", 8);
		}
		else
		if ((memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "77", 2) == 0) ||
			(memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "88", 2) == 0))
			memcpy(&ERR.Remark[50+1], "회선장애", 8);
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "5", 1) == 0)
		{
			memcpy(&ERR.Remark[50+1], "입출금부", 8);
		}
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "95", 1) == 0)
		{
			memcpy(&ERR.Remark[50+1], "입출금부", 8);
		}
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "97", 2) == 0)
			memcpy(&ERR.Remark[50+1], "내부통신", 8);
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "30", 2) == 0)
			memcpy(&ERR.Remark[50+1], "카드부", 6);		
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "20", 2) == 0)
			memcpy(&ERR.Remark[50+1], "명세표부", 8);
		else
			memcpy(&ERR.Remark[50+1], "기기장애", 8);

		cstrGetErr = GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]);

		memcpy(&ERR.Remark[51+8+1], cstrGetErr.GetBuffer(0), cstrGetErr.GetLength());
	}

	m_pProfile->ErrTran(&ERR);								// 장애정보설정

	return T_OK;
}

// 취소장애정보설정
int CTranCmn::fnAPP_ErrorCancelProc()
{
//	  0123456789012345678900123456789012345|0123456789012345678900123456789012345|012345678901234567890012345678901234567890
//    12051324  123456 01000000            |은행출금 1234567890123456            |현금미취 LC[AB] 400AB(06)     
	
	char szAmt[9];
	char szTmp[17];
	CString cstrGetErr("");
	DERRINFO	ERR;
	PrintERRSTACK PERR;

	memset(szTmp, 0x00, sizeof(szTmp));
	if (TranProc != TRAN_WITH) return T_OK;


	StrAllSpace(ERR.Remark);
	StrAllSpace(PERR);

    ERR.Remark[25] = '|';
    ERR.Remark[50] = '|';
    ERR.Remark[76] = ' ';

	Sprintf(szAmt, 8, "%08d", Asc2Int(m_pTranCmn->Accept.Money, sizeof(m_pTranCmn->Accept.Money)));		
	
	memcpy(ERR.Remark,     &m_pProfile->TRANS.YYYYMMDD[4], 4);
	memcpy(&ERR.Remark[4], GetTime().GetBuffer(0), 4);          // 거래시간
	memcpy(&ERR.Remark[4+4+1], m_pProfile->TRANS.SerialNo, 6);		// 일련번호
	memcpy(&ERR.Remark[4+4+1+6+1], szAmt, strlen(szAmt));  			// 거래금액

	if(TranProc == TRAN_DEP)
		memcpy(&ERR.Remark[25+1], "입금업무", 8);  			// 거래종류
	else
	if(TranProc == TRAN_WITH)
		memcpy(&ERR.Remark[25+1], "출금업무", 8);  			// 거래종류
	else
		memcpy(&ERR.Remark[25+1], "기타업무", 8);  			// 

	EditString(&m_pTranCmn->Accept.AccountNum, 14, "$$#####$$$$$##", szTmp, '#', '$', '*', TRUE);

	memcpy(&ERR.Remark[26+1+8+1], szTmp, 14);
	
	if (!m_pDevCmn->fnAPL_CheckError())							// 장애
	{
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "011", 3) == 0)
		{
			if ((m_pDevCmn->CurErrBuff.ErrorCode[4] == '2')	||
				(m_pDevCmn->CurErrBuff.ErrorCode[4] == '4')	)
				memcpy(&ERR.Remark[50+1], "현금미취", 8);
			else
				memcpy(&ERR.Remark[50+1], "매체미취", 8);
		}
		else
		if ((memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "77", 2) == 0) ||
			(memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "88", 2) == 0))
			memcpy(&ERR.Remark[50+1], "회선장애", 8);
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "5", 1) == 0)
		{
			memcpy(&ERR.Remark[50+1], "입출금부", 8);
		}
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "95", 1) == 0)
		{
			memcpy(&ERR.Remark[50+1], "입출금부", 8);
		}
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "97", 2) == 0)
			memcpy(&ERR.Remark[50+1], "내부통신", 8);
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "30", 2) == 0)
			memcpy(&ERR.Remark[50+1], "카드부", 6);		
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "20", 2) == 0)
			memcpy(&ERR.Remark[50+1], "명세표부", 8);
		else
			memcpy(&ERR.Remark[50+1], "기기장애", 8);

		cstrGetErr = GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]);

		memcpy(&ERR.Remark[51+8+1], cstrGetErr.GetBuffer(0), cstrGetErr.GetLength());
	}

	if (m_pDevCmn->TranResult)									// 취소정상
		memcpy(&ERR.Remark[76+1], "OK", 2);
	else
		memcpy(&ERR.Remark[76+1], "NG", 2);

	m_pProfile->ErrCancel(&ERR);								// 취소장애정보설정


	StrAllSpace(PERR);
	sprintf(PERR.DateTime_Serial_Amt   , "%2.2s/%2.2s-%2.2s:%2.2s-%6.6s  %8.8s", &GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6], &GetTime().GetBuffer(0)[0], &GetTime().GetBuffer(0)[2], m_pProfile->TRANS.SerialNo, szAmt);
	if(TranProc == TRAN_WITH)
		sprintf(PERR.TranType_Account   , "출금업무 %16.16s", szTmp);
	else
	if(TranProc == TRAN_DEP)
		sprintf(PERR.TranType_Account   , "입금업무 %16.16s", szTmp);
	else
		sprintf(PERR.TranType_Account   , "기타업무 %16.16s", szTmp);

	

	memcpy(&PERR.ErrType_Result, &ERR.Remark[50+1], 8);
	
	if (m_pDevCmn->TranResult)							// HOST OK
		sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s) - OK", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
	else
		sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s) - NG", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
	m_pProfile->DetailHErrStack(&PERR);

	return T_OK;
}

// 호스트장애정보설정
int CTranCmn::fnAPP_ErrorHostProc()
{
	return T_OK;
}

// KCash복구정보설정
int CTranCmn::fnAPP_ErrorKCashProc()
{
//	  0123456789012345678901	: 22 Col
//    SSS지급 \12,345,678 OK	: OK/NG
	
	DERRINFO	ERR;
	char	StrBuff[81];

	if (TranProc == TRAN_IC)									// KCASH거래
		;
	else 
		return T_OK;

	StrAllSpace(ERR.Remark);
	memcpy(ERR.Remark, &m_pProfile->TRANS.SerialNo[0], 6);		// 일련번호

	if (TranCode == TC_KCLOADRCV)
		memcpy(&ERR.Remark[6], "충전", 4);
	else
	if (TranCode == TC_KCUNLOADRCV)
		memcpy(&ERR.Remark[6], "환불", 4);
	else
		return T_OK;											// 다른 거래는 처리하지 않는다.

	memcpy(StrBuff, Asc2Amt(Accept.Money, 8, 11), 11);			// 거래금액(Space(1) + 금액(11))
	MarkAmount(StrBuff, 11, '\\');
	memcpy(&ERR.Remark[10], " ", 1);
	memcpy(&ERR.Remark[11], StrBuff, 11);

	memcpy(&ERR.Remark[22], " ", 1);							// SPACE(1)
	if (m_pDevCmn->TranResult)									// 취소정상
	{
		if (KCashHostICFlag)
			memcpy(&ERR.Remark[23], "IC", 2);
		else
			memcpy(&ERR.Remark[23], "OK", 2);
	}
	else
	{
		memcpy(&ERR.Remark[23], "NG", 2);
	}

	m_pProfile->ErrKCash(&ERR);									// KCash복구정보설정
	return T_OK;
}
/////////////////////////////////////////////////////////////////////////////
// 장애인자처리
int	CTranCmn::fnAPP_PrintError(int PrintDevice, int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_PrintError");
	int		HiddenFlg = TRUE;									// 기능삭제
	int		nSlipIndex = 0, nJnlIndex = 0;						// 명세표/저널 인자인덱스
	CString	strTemp("");
	int		nTempMoney = 0;
	int		ii = 0;

	if (MaterialOutFlg & DEV_SPR)								// 저널/명세표인자및방출완료
	{
		if (PrintDevice & DEV_SPR)								// 명세표 인자하지 않음
			PrintDevice &= ~DEV_SPR;
	}

	if (PrintDevice == DEV_NONE)								// 인자디바이스 확인
		return T_OK;

	if (PrintDevice & DEV_JPR) 
	{
		HiddenFlg = FALSE;										// 저널만인자시 기능삭제
	}

	//#N0203 장애 저널 로깅 시 일련번호 증가 안함.
	/*
	if (!AddSerialFlg)											// 일련번호증가무  
	{
		AddSerialFlg = TRUE;									// 일련번호증가유 -> #0059
		m_pDevCmn->fnAPL_AddSerialNo();							// 거래일련번호증가
	}
	*/

	memset(ShuData, 0, sizeof(ShuData));						// Shu출력영역
	nSlipIndex = 0;

	for (ii = 0; ii < 50; ii++)
	{
		fnAPP_EditPrintLine(PrintDevice, ShuData[nSlipIndex], ii, HiddenFlg);		// 라인 Edit
		if (ShuData[nSlipIndex][0] != 0)
			nSlipIndex++;
	}

	nSlipIndex = 15;											// 장애시 인자영역
	if (!m_pDevCmn->fnAPL_CheckError())							// 장애검지유
	{
		ShuData[nSlipIndex][0] = m_pDevCmn->CurErrBuff.ProcCount[0];
		ShuData[nSlipIndex][1] = '-';

		// 장애코드
		strTemp.Format("%s", m_pDevCmn->CurErrBuff.ErrorCode);
		memcpy(&ShuData[nSlipIndex][2], m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), 
			__min(5, sizeof(m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode))));
		Sprintf(&ShuData[nSlipIndex][4], 9, "(%s)", strTemp);
		memcpy(&ShuData[nSlipIndex][13], "  ", 2);
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		{
			sprintf(&ShuData[nSlipIndex][15], "%s",
				GetString(m_pDevCmn->CurErrBuff.ErrorString, strlen(m_pDevCmn->CurErrBuff.ErrorString), 40));
		}
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_PrintError2[%d][%s]", nSlipIndex, strTemp);
		nSlipIndex++;
	}
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_PrintError3[%d][%s]", nSlipIndex, strTemp);
	if ((TranProc == TRAN_DEP)					&&				// 2007-01-03-#05
		(!strTemp.CompareNoCase("0110295"))		)				// 송금현금회수
	{
		sprintf(ShuData[nSlipIndex++], "주민번호 불일치로 인한 회수");
	}
	else
	if ((TranProc == TRAN_DEP)					&&				// 2007-01-03-#05 위치이동
		((!strTemp.CompareNoCase("0110050"))	||				// 현금미수취
		 (!strTemp.CompareNoCase("0110040")))	)				// 수표미수취일때만 인자
	{
		sprintf(ShuData[nSlipIndex++], "[잔액조회 요망] 입금부 잔류");
	}
	else
	if (TranProc == TRAN_DEP)
	{
		if (CashData.mTotalAmount)
			sprintf(ShuData[nSlipIndex++], "[잔액조회 요망]  입금부 수납처리");
		else sprintf(ShuData[nSlipIndex++], "[잔액조회 요망]");
	}
	else
	if (EMVAuthError)
	{								  
		sprintf(ShuData[nSlipIndex++], "EMV 인증에러 발생 창구문의 요망");
	}

	if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "0110", 4) == 0)
		fnAPD_ShuPrint(PrintDevice, JNL_STATUS, WaitFlg);
//		fnAPD_ShuPrint(PrintDevice, JNL_OPER, WaitFlg);
	else
		fnAPD_ShuPrint(PrintDevice, JNL_ERROR, WaitFlg);

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 출금취소거래인자처리
int	CTranCmn::fnAPP_PrintWithCancelTran(int PrintDevice, int WaitFlg)
{
	int		HiddenFlg = TRUE;									// 기능삭제
	int		nSlipIndex = 0, nJnlIndex = 0;						// 명세표/저널 인자인덱스
	CString	StrTemp("");
	int		ii = 0;

	if (MaterialOutFlg & DEV_SPR)								// 저널/명세표인자및방출완료
	{
		if (PrintDevice & DEV_SPR)								// 명세표 인자하지 않음
			PrintDevice &= ~DEV_SPR;
	}

	if (PrintDevice == DEV_NONE)								// 인자디바이스 확인
		return T_OK;

	if (PrintDevice & DEV_JPR) 
	{
		HiddenFlg = FALSE;										// 저널만인자시 기능삭제
	}

//	if (!AddSerialFlg)											// 일련번호증가무
//	{
//		AddSerialFlg = TRUE;									// 일련번호증가유
//		m_pDevCmn->fnAPL_AddSerialNo();							// 거래일련번호증가
//	}

	memset(ShuData, 0, sizeof(ShuData));						// Shu출력영역
	nSlipIndex = 0;

	for (ii = 0; ii < 13; ii++)
	{
		fnAPP_EditPrintLine(PrintDevice, ShuData[nSlipIndex], ii, HiddenFlg);		// 라인 Edit
		if (ShuData[nSlipIndex][0] != 0)
			nSlipIndex++;
	}

	StrTemp = m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode);

	// 장애내용
	if (m_pDevCmn->TranResult)
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			sprintf(ShuData[nSlipIndex++], "취소거래 OK");
		else
			sprintf(ShuData[nSlipIndex++], "Cancel OK");
	}
	else
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			sprintf(ShuData[nSlipIndex++], "취소거래 NG");
		else
			sprintf(ShuData[nSlipIndex++], "Cancel NG");

	}

	// 장애코드
	ShuData[nSlipIndex][0] = m_pDevCmn->CurErrBuff.ProcCount[0];
	ShuData[nSlipIndex][1] = '-';
	memcpy(&ShuData[nSlipIndex][2], StrTemp.Left(2).GetBuffer(0), 2);
	Sprintf(&ShuData[nSlipIndex][4], 9, "(%s)", m_pDevCmn->CurErrBuff.ErrorCode);
	memcpy(&ShuData[nSlipIndex][13], "  ", 2);
	
	if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "011", 3) == 0)
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			sprintf(&ShuData[nSlipIndex++][15], "[매체잔류]");
		else
			sprintf(&ShuData[nSlipIndex++][15], "[Remain material]");
	}
	else
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			sprintf(&ShuData[nSlipIndex++][15], "[기기장애]");
		else
			sprintf(&ShuData[nSlipIndex++][15], "[Device error]");
	}

	if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		sprintf(ShuData[nSlipIndex++], "[잔액조회 요망]");
	else
		sprintf(ShuData[nSlipIndex++], "[Check your account balance]");

	nSlipIndex++;												// 2004.03.10

	fnAPP_EditPrintLine(PrintDevice, ShuData[nSlipIndex++], 17, HiddenFlg);
	fnAPP_EditPrintLine(PrintDevice, ShuData[nSlipIndex++], 18, HiddenFlg);

	fnAPD_ShuPrint(PrintDevice, JNL_CANCEL, WaitFlg);
	
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// 입금취소거래인자처리 (사용무)
int	CTranCmn::fnAPP_PrintDepCancelTran(int PrintDevice, int WaitFlg)
{
	int		HiddenFlg = FALSE;									// 기능삭제
	int		nSlipIndex = 0, nJnlIndex = 0;						// 명세표/저널 인자인덱스

	if (MaterialOutFlg & DEV_SPR)								// 저널/명세표인자및방출완료
	{
		if (PrintDevice & DEV_SPR)								// 명세표 인자하지 않음
			PrintDevice &= ~DEV_SPR;
	}

	if (PrintDevice == DEV_NONE)								// 인자디바이스 확인
		return T_OK;

	if (!AddSerialFlg)											// 일련번호증가무
	{
		AddSerialFlg = TRUE;									// 일련번호증가유
		m_pDevCmn->fnAPL_AddSerialNo();							// 거래일련번호증가
	}

	return T_OK;
}

// 취소인자처리
int	CTranCmn::fnAPP_PrintCancel(int PrintDevice, int WaitFlg)
{
	int		HiddenFlg = TRUE;									// 기능삭제
	int		nSlipIndex = 0, nJnlIndex = 0, ii = 0;						// 명세표/저널 인자인덱스

	if (!AddSerialFlg)											// 일련번호증가무
	{
		AddSerialFlg = TRUE;									// 일련번호증가유
		m_pDevCmn->fnAPL_AddSerialNo();							// 거래일련번호증가
	}

	if (PrintDevice & DEV_JPR) 
	{
		HiddenFlg = FALSE;										// 저널만인자시 기능삭제
	}

	memset(ShuData, 0, sizeof(ShuData));						// Shu출력영역
	nSlipIndex = 0;

	for (ii = 0; ii < 50; ii++)
	{
		fnAPP_EditPrintLine(PrintDevice, ShuData[nSlipIndex], ii, HiddenFlg);		// 라인 Edit
		if (ShuData[nSlipIndex][0] != 0)
			nSlipIndex++;
	}

	if (TranProc == TRAN_WITH)
	{
		if (m_pProfile->DEVICE.MachineType == U8100)
			sprintf(ShuData[0], "    [출금취소]");
		else
			sprintf(ShuData[0], "        [출금취소]");
	}
	else
	if (TranProc == TRAN_DEP)
	{
		if (m_pProfile->DEVICE.MachineType == U8100)
			sprintf(ShuData[0], "    [입금거래]"); //#CS0006		[입금거래취소] -> [입금거래]
		else
			sprintf(ShuData[0], "        [입금거래]"); //#0055 -> #0093
	}
	else
	if ((TranCode == TC_JRTRAN)	||
		(TranCode == TC_JRINQ)	)
	{
		if (m_pProfile->DEVICE.MachineType == U8100)
			sprintf(ShuData[0], "    [공과금취소]");
		else
			sprintf(ShuData[0], "        [공과금취소]");
	}
		else
	if ((TranCode == TC_HIPASS_CASH) ||						//#N0266
		(TranCode == TC_HIPASS_CREDIT))
	{
		if (m_pProfile->DEVICE.MachineType == U8100)
			sprintf(ShuData[0], "    [하이패스취소]");
		else
			sprintf(ShuData[0], "        [하이패스취소]");
	}
	else
	{
		if (m_pProfile->DEVICE.MachineType == U8100)
			sprintf(ShuData[0], "    [고객취소]");
		else
			sprintf(ShuData[0], "        [고객취소]");
	}


	fnAPD_ShuPrint(PrintDevice, JNL_OPER, WaitFlg);
	
	return T_OK;
}

// 수표입금정보인자
int CTranCmn::fnAPP_PrintCheckDeposit(int PrintDevice, int WaitFlg)
{
	int		HiddenFlg = FALSE;									// 기능삭제
	int		nIndex = 0;											// 명세표/저널 인자인덱스
	CString	tempStr("");
	char	StrBuff[41];

	memset(ShuData, 0, sizeof(ShuData));						// Shu출력영역
	nIndex = 0;
	
	fnAPP_EditPrintLine(PrintDevice, ShuData[nIndex++], 0, HiddenFlg);	// 1 라인
	fnAPP_EditPrintLine(PrintDevice, ShuData[nIndex++], 2, HiddenFlg);	// 2 라인

	nIndex++;

	memset(StrBuff, 0x00, sizeof(StrBuff));
	tempStr.Format("%d", CheckInfoAmt);
	sprintf(StrBuff, "      수표금액 : ");
	memcpy(&StrBuff[17], Asc2Amt(tempStr.GetBuffer(tempStr.GetLength()), tempStr.GetLength(), 10).GetBuffer(0), 10);
	MarkAmount(&StrBuff[17], 10, '\\');
	strcpy(ShuData[nIndex++], StrBuff);
	for (int i = 0; i < CheckInfoCnt; i++)
	{
		sprintf(ShuData[nIndex++],
				"    %8.8s  %2.2s  %4.4s  %6.6s %2.2s  %10.10s",
				CheckInfoData[i].ReadData,
				CheckInfoData[i].Bank,
				CheckInfoData[i].Branch,
				CheckInfoData[i].AccountNum,
				CheckInfoData[i].Kind,
				CheckInfoData[i].Amount);
	}
	
	fnAPD_JnlPrint(WaitFlg);

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	편집함수
/////////////////////////////////////////////////////////////////////////////
// 은행코드편집
char* CTranCmn::fchpAPP_EditBank()
{
	CString StrBuff("");


	if (MenuAvail & MENU_MCU2)									// 자행2TR
	{
		if (CardTran & MENU_J2)
			StrBuff = GetSprintf("%3.3s", BANKJJ2);
		else
			StrBuff = GetSprintf("%3.3s", "   ");
	}
	else
	if (MenuAvail & MENU_MCU3)									// 3TR
	{
		if (CardTran & MENU_J3)
			StrBuff = GetSprintf("%3.3s", BANKJJ2);					// 기업은행카드
		else
			StrBuff = GetSprintf("%3.3s", &pCardData3->BankNo[0]);
	}
	else
	if (MenuAvail & MENU_PB)									// 통장거래
		StrBuff = GetSprintf("%3.3s", BANKJJ2);
	else
	if (MenuAvail & MENU_MU)									// 무매체거래
		StrBuff = GetSprintf("%3.3s", BANKJJ2);
	else
	if (MenuAvail & MENU_IC)									// K-CASH거래
		StrBuff = GetSprintf("%3.3s", &pCardData3->BankNo[0]);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fchpAPP_EditBank() : [%s]", StrBuff.GetBuffer(0));	// TEST!!!!!

	return StrBuff.GetBuffer(StrBuff.GetLength());
}

///////////////////////////////////////////////////////////////////////////////////
// 계좌번호편집
char* CTranCmn::fchpAPP_EditAcnt(int HiddenFlg, int AcntOnlyFlg)
{
	static	char	StrBuff[81];

	memset(StrBuff, 0, sizeof(StrBuff));						// 초기화

	if (!AcntOnlyFlg)
	{
		if ((MenuAvail & MENU_J2)	||
			(MenuAvail & MENU_T2)	)							// 신용카드
		{
			EditString(pCardData2->MembershipNo, 16, "$$#####$$$$$####", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else 
		if ((MenuAvail & MENU_J3)	||							// 자행현금카드만처리
			(MenuIrdaMode)			)
		{
			//EditString(&pCardData3->OtherData[9], 11, "###-##-##$$$$", StrBuff, '#', '$', '*', HiddenFlg);
			EditString(Accept.AccountNum, 11, "###-##-##$$$#", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else
		if (MenuAvail & MENU_T3)								// 타행현금카드
		{
			EditString(&pCardData3->AccountNo[0], 16, "$$#####$$$$$####", StrBuff, '#', '$', '*', HiddenFlg);
//			EditString(pCardData2->MembershipNo, 16, "$$#####$$$$$####", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else
		if (MenuAvail & MENU_PB)								// 통장
		{
//			EditString(&pPbMsData->AccountNo[0], 11, "###-##-##$$$#", StrBuff, '#', '$', '*', HiddenFlg);
			EditString(Accept.AccountNum, 11, "###-##-##$$$#", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else
		if (MenuAvail & MENU_MU)								// 무매체
		{
			EditString(Accept.MuAccountNum, Accept.MuAccountNumSize, "###-##-##$$$$", StrBuff, '#', '$', '*', HiddenFlg);
		}
	}
	else
	{
		if ((MenuAvail & MENU_J2)	||
			(MenuAvail & MENU_T2)	)							// 신용카드
		{
			EditString(pCardData2->MembershipNo, 16, "$$#####$$$$$####", StrBuff, '#', '$', '*', HiddenFlg);
//			EditString(pCardData2->MembershipNo, 16, "######$$$$$$$$$$", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else 
		if ((MenuAvail & MENU_J3)	||							// 자행카드만처리
//			(MenuAvail & MENU_T3)	||							// 현금카드
			(MenuIrdaMode)			)
		{
			//EditString(&pCardData3->OtherData[9], 11, "#######$$$$", StrBuff, '#', '$', '*', HiddenFlg);
			EditString(Accept.AccountNum, 11, "#######$$$#", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else
		if (MenuAvail & MENU_T3)								// 타행현금카드
		{
			EditString(&pCardData3->AccountNo[0], 16, "$$#####$$$$$####", StrBuff, '#', '$', '*', HiddenFlg);
//			EditString(pCardData2->MembershipNo, 16, "$$#####$$$$$####", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else
		if (MenuAvail & MENU_PB)								// 통장
		{
//			EditString(&pPbMsData->AccountNo[0], 11, "#######$$$$", StrBuff, '#', '$', '*', HiddenFlg);
			EditString(Accept.AccountNum, 11, "#######$$$#", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else
		if (MenuAvail & MENU_MU)								// 무매체
		{
			EditString(Accept.MuAccountNum, Accept.MuAccountNumSize, "#######$$$#", StrBuff, '#', '$', '*', HiddenFlg);
		}
	}

	return StrBuff;
}

///////////////////////////////////////////////////////////////////////////////////
// 인출 계좌번호 편집
CString CTranCmn::fchpAPP_EditSrcAcnt(void *inbuff, int inLen, int outLen, int HiddenFlg /*= FALSE*/,  int AcntOnlyFlg /*= FALSE*/)
{
	char	*inBuff  = (char *)inbuff;

	CString StrBuff("");

	if (!AcntOnlyFlg)
	{
		if (MenuAvail & MENU_MCU)								// 카드거래
		{
			if (MenuAvail & MENU_J2T2)							// 자타서2TR거래
			{
				StrBuff = EditString(inbuff, 16, "$$##-###$-$$$$-####", '#', '$', '*', HiddenFlg);
			}
			else
			if (MenuAvail & MENU_T3)								// 타행현금카드
			{
				StrBuff = EditString(inbuff, 16, "$$##-###$-$$$$-####", '#', '$', '*', HiddenFlg);
			}
			else
			if (MenuAvail & MENU_J3)							// 자타행3TR거래
			{
				StrBuff = EditString(inbuff, 11, "###-##-##$$$#", '#', '$', '*', HiddenFlg);
			}
		}
		else
		if (MenuAvail & MENU_MU)								// 무매체
		{
			EditString(Accept.MuAccountNum , 11, "###-##-##$$$#", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else
		if (MenuAvail & MENU_PB)
		{
			BYTE	PbAccount[16];
			int		PbAccLen = 0;
			memset(PbAccount, 0x00, sizeof(PbAccount));
			PbAccLen = fnAPD_GetPbAccount(PbAccount);
			EditString(PbAccount , 11, "###-##-##$$$#", StrBuff, '#', '$', '*', HiddenFlg);
		}
	}
	else
	{
		if (MenuAvail & MENU_MCU)								// 카드거래
		{
			if (MenuAvail & MENU_J2T2)							// 자타서2TR거래
			{
				StrBuff = EditString(inbuff, 16, "$$$$########$$$$", '#', '$', '*', HiddenFlg);
			}
			else
			if (MenuAvail & MENU_T3)								// 타행현금카드
			{
				StrBuff = EditString(inbuff, 16, "$$#####$$$$$####", '#', '$', '*', HiddenFlg);
			}
			else
			if (MenuAvail & MENU_J3)							// 자타행3TR거래
			{
				StrBuff = EditString(inbuff, 11, "#######$$$#", '#', '$', '*', HiddenFlg);
			}
		}
		else
		if (MenuAvail & MENU_MU)								// 무매체
		{
			EditString(Accept.MuAccountNum , 11, "#######$$$#", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else
		if (MenuAvail & MENU_PB)
		{
			BYTE	PbAccount[16];
			int		PbAccLen = 0;
			memset(PbAccount, 0x00, sizeof(PbAccount));
			PbAccLen = fnAPD_GetPbAccount(PbAccount);
			EditString(PbAccount , 11, "#######$$$#", StrBuff, '#', '$', '*', HiddenFlg);
		}
	}

	StrBuff = GetString(StrBuff.GetBuffer(0), StrBuff.GetLength(), outLen);

	return StrBuff;
}

///////////////////////////////////////////////////////////////////////////////////
// 수취 계좌번호 편집
CString CTranCmn::fchpAPP_EditDestAcnt(void *inbuff, int inLen, int outLen, int HiddenFlg /*= FALSE*/,  int AcntOnlyFlg /*= FALSE*/)
{
	char	*inBuff  = (char *)inbuff;
	CString StrBuff("");

	for (int i = 0; i < inLen; i++)
	{
		if (inBuff[i] == ' ')
			break;
	}

	inLen = i;

	if (!AcntOnlyFlg)
		StrBuff = EditString(inBuff, inLen, "###-##-##$$$#", '#', '$', '*', HiddenFlg);
	else
		StrBuff = EditString(inBuff, inLen, "#######$$$#", '#', '$', '*', HiddenFlg);
	
	StrBuff = GetString(StrBuff.GetBuffer(0), StrBuff.GetLength(), outLen);

	return StrBuff.GetBuffer(0);
}

//////////////////////////////////////////////////////////////////////
// 명세표편집
/////////////////////////////////////////////////////////////////////////////
// 명세표/저널 편집
int	CTranCmn::fnAPP_EditPrintLine(int PrintDevice, void* cData, int LineNum, int HiddenFlg)
{
	CString Data;
	CString	cstrTemp("");
	int     i = 0, ii = 0;
	char	szTempBuff[256] = {0,};
	char	szTempBuff2[256] = {0,};

		
/*  #0128
	if (m_pProfile->DEVICE.MachineType == U8100)
	{
		fnAPP_EditPrintLine_Sub_U8100_1(PrintDevice, cData, LineNum, HiddenFlg);
		return T_OK;
	}
*/
	switch(TranProc) 
	{
	case TRAN_UTILYBILLS:
	case TRAN_LOCALTAX:
		fnAPP_EditPrintLine_Sub_U8100_1(PrintDevice, cData, LineNum, HiddenFlg);  //#0129
		break;		
	case TRAN_DEP:
	case TRAN_WITH:
	case TRAN_INQ:
	case TRAN_TRANS:
		fnAPP_EditPrintLine_Sub_T3_1(PrintDevice, cData, LineNum, HiddenFlg);		
		break;
	default:
		fnAPP_EditPrintLine_Sub_T3_1(PrintDevice, cData, LineNum, HiddenFlg);		
		break;
	}

	
	return T_OK;
}

int	CTranCmn::fnAPP_EditPrintLine_Sub_U8100_1(int PrintDevice, void* cData, int LineNum, int HiddenFlg)
{
	CString Data;
	CString	cstrTemp("");
	CString strTranKind("");
	char	szTmp[1024];
	char	szTempBuff[256] = {0,};
	char	szTempBuff2[256] = {0,};
	char	CurDate[256];
	char	CurTime[256];
	int     i = 0;
	//Bank Row set
	m_pDevCmn->SprRowOption = SPR_MAX_ROW_U8100;

	char*	TranListTbl[50] = 
	{
//		 12345678901234567890
		"현금지급           ", //0
		"계좌이체           ", //1
		"현금서비스지급     ", //2
		"결제계좌이체       ", //3
		"서비스이체         ", //4
		"보험계약대출(출금) ", //5
		"보험계약대출 조회  ", //6
		"지로공과금납부     ", //7 //#0073
		"지로공과금 납부내역", //8
		"아파트관리비 납부  ", //9
		"신용정보조회       ", //10
		"대출정보조회       ", //11
		"카드론             ", //12
		"동부화재계약자보험 ", //13
		"Deposit            ", //14
		"입금               ", //15
		"아파트관리비 납부  ", //16
		"지로공과금납부     ", //17  //#SJ 1115 명세표 수정
		"캐쉬백 조회        ", //18
		"캐쉬백 적립        ", //19
		"Cash Advance       ", //20
		"Cash Withdrawal    ", //21
		"현금서비스조회     ", //22
		"결제대금조회       ", //23
		"조회               ", //24
		"T-Money 충전       ", //25
		"CGV                ", //26
		"Wire Transfer      ", //27
		"DOTONG             ", //28
		"단기연체정보조회   ", //29
		"무매체 입금        ", //30
		"신용평점정보조회   ", //31
		"증권출금           ", //32
		"증권조회           ", //33
		"Withdrawal         ", //34
		"Balance Inquiry    ", //35
		"Account Transfer   ", //36
		"통장정리           ", //37
		"지방세간편납부     ", //38
		"???????????????????"
	};
	
	char*	TranNameTbl[40] = 
	{
//		 12345678901234567890
		"은행,증권          ", //0
		"신용카드           ", //1
		"삼성카드론         ", //2
		"한국신용정보       ", //3
		"???????????????????"
	};

	if(MenuFICMode & MENU_IC)
		strTranKind = "IC";
	else
	if(PbTran & MENU_PB)
		strTranKind = "통장";
	else
	if(MenuAvail & MENU_MU)
		strTranKind = " 무";
	else
		strTranKind = "MS";

	GetDate(CurDate);
	GetTime(CurTime);


	Data.Empty();
	memset(szTmp, 0x00, sizeof(szTmp));

	if (PrintDevice == (DEV_SPR | DEV_JPR))						// 인자데이타 편집은 하나의 장치ID만..~!!
		return T_OK;

	if (PrintDevice & (DEV_SPR | DEV_JPR) == DEV_NONE)			// 인자데이타 편집은 하나의 장치ID만..~!!
		return T_OK;

	memset(szTmp, 0, sizeof(szTmp));

	switch (LineNum)
	{													
	case 0:
		break;
	case 1:
		break;
	case 2:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			Data = GetSprintf("  [EasyCash Receipt]");
		else
			Data = GetSprintf("[이지캐쉬 거래명세표]");
		break;
	case 3:
		Data = GetSprintf(SPACE16);	
		break;
	case 4:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			Data = GetSprintf("Date/Time:%4.4s%2.2s%2.2s-%2.2s:%2.2s", &CurDate[0], &CurDate[4],	&CurDate[6], &CurTime[0], &CurTime[2]);
		else
			Data = GetSprintf("거래일자:%4.4s%2.2s%2.2s-%2.2s:%2.2s", &CurDate[0], &CurDate[4],	&CurDate[6], &CurTime[0], &CurTime[2]);
		break;
	case 5:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			Data = GetSprintf("Seque No:%6.6s%8.8s-%6.6s",  m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum, &m_pProfile->TRANS.SerialNo[0]);
		else
			Data = GetSprintf("처리번호:%6.6s%8.8s-%6.6s",  m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum, &m_pProfile->TRANS.SerialNo[0]);
		break;

	case 6:		
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4320:
		case TRANID_4220:
		case TRANID_4210:
		case TRANID_4116:   
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Trans.  :%s", TranListTbl[21]);
			else
			{
				Data = GetSprintf("거래종류:%s", TranListTbl[0]);
				Data.TrimRight();
				Data += strTranKind;
			}
			break;
		case TRANID_7220:
		case TRANID_7230:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Trans.  :%s", TranListTbl[35]);
			else
				Data = GetSprintf("거래종류:%s", TranListTbl[24]);
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_4330:
		case TRANID_4340:
				if(TranCode == TC_FOREGIN)
				{
					if(memcmp(Accept.MuConfirmNum, "10", 2) == 0)
						Data = GetSprintf("Trans.  :%s", TranListTbl[20]);
					else
						Data = GetSprintf("Trans.  :%s", TranListTbl[21]);
				}
				else
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("Trans.  :%s", TranListTbl[34]);
				else
					Data = GetSprintf("거래종류:%s", TranListTbl[2]);
					Data.TrimRight();
					Data += strTranKind;
			break;
		case TRANID_4350:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Trans.  :%s", TranListTbl[36]);
			else
				Data = GetSprintf("거래종류:%s", TranListTbl[4]);
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Trans.  :%s", TranListTbl[36]);
			else
				Data = GetSprintf("거래종류:%s", TranListTbl[1]);
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:
		case TRANID_7126:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Trans.  :%s", TranListTbl[14]);
			else
				Data = GetSprintf("거래종류:%s", TranListTbl[15]) ;
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_4730: //#0024
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Trans.  :%s", "Passbook Arrangement");
			else
				Data = GetSprintf("거래종류:%s",TranListTbl[37]) ;  //#0036
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_4810:
		case TRANID_4813:
			Data = GetSprintf("거래종류:%s", TranListTbl[7]);
			Data.TrimRight();
			Data += strTranKind;
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			Data = GetSprintf("거래종류:%s", TranListTbl[38]);
			Data.TrimRight();
			Data += strTranKind;
			break;
		default:
			break;
		}		
		break;
	case 7:														
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:   
		case TRANID_4320:
		case TRANID_4220:
		case TRANID_7220:
		case TRANID_7290:
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:
		case TRANID_7126:
		case TRANID_4730:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Service :%s", fnAPP_Get_BankName(CardBank));	
			else
				Data = GetSprintf("거래기관:%s", fnAPP_Get_BankName(CardBank));				
			break;
		case TRANID_4350:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Service :%s", "Credit");	
			else
				Data = GetSprintf("거래기관:%s", fnAPP_Get_CreditName(pCardData2->MembershipNo));
			break;
		case TRANID_7230:
		case TRANID_4330:
		case TRANID_4340:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Service :%s", "Credit");	
			else
				Data = GetSprintf("거래기관:%s", fnAPP_Get_CreditName(pCardData2->MembershipNo));
			break;
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Service :%s", fnAPP_Get_BankName(CardBank));	
			else
				Data = GetSprintf("거래기관:%s", fnAPP_Get_BankName(CardBank));				
			break;
		case TRANID_4810:
		case TRANID_4813:
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			Data = GetSprintf("고유번호:%12.12s", m_RD.byDataField_026x);
			break;
		default:
			break;
		}		
		break;
	case 8:
		Data = GetSprintf(SPACE16);
		break;
	case 9:														
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:
		case TRANID_7220:
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:	
		case TRANID_7126:
		case TRANID_4320:
		case TRANID_4220:	
		case TRANID_4730:
			if (PbTran & MENU_PB) //#0036
			{
				if(!IsZero(&Accept.AccountNum, 16)) //#0024
					EditString(&m_pTranCmn->pPbMsData->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
				if(!IsSpace(&Accept.AccountNum, 16))
					EditString(&Accept.AccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
					EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7100: DataField Not Found - x017X[%s]" , x017X);
			}
			else
			if (MenuAvail & MENU_MU) 
			{
				if(!IsZero(&Accept.MuAccountNum, 16)) //#0024
					EditString(&Accept.MuAccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
				if(!IsSpace(&Accept.MuAccountNum, 16))
					EditString(&Accept.MuAccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
					EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7100: DataField Not Found - x017X[%s]" , x017X);
			}
			else
			{
				//계좌번호	
		//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));

				
				if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
					EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
				if(!IsSpace(&m_pTranCmn->pCardData3->AccountNo, 16))
					EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
					EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

			}				

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("CARD No.:%s", m_RD.byDataField_017x);
			else
				Data = GetSprintf("계좌번호:%s", m_RD.byDataField_017x);
			Data = "@" + Data;
			break;
		case TRANID_4330:
		case TRANID_7230:
		case TRANID_4350:
		case TRANID_4340:
			if(!m_pDevCmn->TranResult)
			{
				if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
					EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
				if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
					EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
					EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
			}
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("CARD No.:%s", m_RD.byDataField_017x);
			else
				Data = GetSprintf("계좌번호:%s", m_RD.byDataField_017x);
			Data = "@" + Data;
			break;
		default:
			break;
		}		
		break;
		
	case 10:														// 거래금액
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:   
		case TRANID_7220:
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:	
		case TRANID_7126:
		case TRANID_4320:
		case TRANID_4220:
			if(!m_pDevCmn->TranResult)
			{
				sprintf(m_RD.byDataField_023x, "%15.15s", Accept.Money);	
			}

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("R.Amount:%s Won",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			else
				Data = GetSprintf("거래금액:%s원",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			break;
		case TRANID_4330:
		case TRANID_7230:
		case TRANID_4350:
		case TRANID_4340:
			if(!m_pDevCmn->TranResult)
			{
				sprintf(m_RD.byDataField_023x, "%15.15s", Accept.Money);	
			}

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("R.Amount:%s Won",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			else
				Data = GetSprintf("거래금액:%s원",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			break;
		case TRANID_4810:
		case TRANID_4813:
			Data = GetSprintf("수수료  :%s원", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			Data = GetSprintf("거래금액:%s원",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			break;
		default:
			break;
		}		
		break;
	case 11:														// 거래 후 금액
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:   
		case TRANID_7220:
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:	
		case TRANID_7126:
		case TRANID_4320:
		case TRANID_4220:
			if(!m_pDevCmn->TranResult) 
				break;

			if (m_RD.byDataField_026x[0] = '?')
				break;

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Trans.NO:%12.12s", m_RD.byDataField_026x);
			else
				Data = GetSprintf("고유번호:%12.12s", m_RD.byDataField_026x);
			break;
		case TRANID_4330:
		case TRANID_7230:
		case TRANID_4350:
		case TRANID_4340:
			if(!m_pDevCmn->TranResult) 
				break;

			if (m_RD.byDataField_026x[0] = '?')
				break;

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Trans.NO:%12.12s", m_RD.byDataField_026x);
			else
				Data = GetSprintf("고유번호:%12.12s", m_RD.byDataField_026x);
			break;
		case TRANID_4810:
		case TRANID_4813:
			Data = GetSprintf("거래금액:%s원",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			Data = GetSprintf("납부자명:%s", strCT(m_RD.byDataField_021x , 16));
			break;
		default:
			break;
		}		
		break;
	case 12:	
//		Data = GetSprintf("응답코드:%03d[%s]", TranResultNGCode, (TranResultNGCode == 0)	? "정상" : "불능"); 

		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)		
			Data = GetSprintf("  Response:%03d[%s]", TranResultNGCode, (TranResultNGCode == 0)	? "OK" : "NG"); 
		else
			Data = GetSprintf("  응답코드:%03d[%s]", TranResultNGCode, (TranResultNGCode == 0)	? "거래정상" : "거래불능");
		break;
	case 13:	
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:   
		case TRANID_7220:
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_4320:
		case TRANID_4220:
			if(!m_pDevCmn->TranResult)
				break;

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Fee     :%s Won", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			else
				Data = GetSprintf("수수료  :%s원", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			break;
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:	
		case TRANID_7126:
		case TRANID_4730:
		case TRANID_7236:
			if(!m_pDevCmn->TranResult);
//				Data = GetSprintf("응답코드:%03d", TranResultNGCode);
			else
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Fee     :%s Won", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			else
				Data = GetSprintf("수수료  :%s원", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			break;
		case TRANID_4330:
		case TRANID_7230:
		case TRANID_4350:
		case TRANID_4340:
			if(!m_pDevCmn->TranResult) 
				break;

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Fee     :%s Won", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			else
				Data = GetSprintf("수수료  :%s원", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			break;
		case TRANID_4810:
		case TRANID_4813:
			Data = GetSprintf("납부기관:%s", strCT(m_RXJIRODATA.JiroBankCode , 16));
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			Data = GetSprintf("세목:%s", strGetBufferArray[0]);
			break;
		default:
			break;
		}		
		break;
	case 14:	
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:   
		case TRANID_7220:
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
//		case TRANID_4716: //#0044
		case TRANID_7200:	
		case TRANID_4320:
		case TRANID_4220:
			if(!m_pDevCmn->TranResult) 
				break;
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Balance :%s Won", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
			else
				Data = GetSprintf("계좌잔액:%s원", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
			Data = "@" + Data;
			break;
		case TRANID_4330:
		case TRANID_7230:
		case TRANID_4350:
		case TRANID_4340:
			if(!m_pDevCmn->TranResult) 
				break;

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Balance :%s Won", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
			else
				Data = GetSprintf("계좌잔액:%s원", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
			Data = "@" + Data;
			break;
		case TRANID_4810:
		case TRANID_4813:
			Data = GetSprintf("납부자명:%s", strCT(m_RD.byDataField_021x , 16));
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			Data = GetSprintf("거래금액:%s원", strGetBufferArray[1]);
			break;
		default:
			break;
		}		
		break;
	case 15:	
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:   
		case TRANID_7220:
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_7126:
		case TRANID_4320:
		case TRANID_4220:
		case TRANID_4330:
		case TRANID_7230:
		case TRANID_4350:
		case TRANID_4340:
			break;
		case TRANID_4810:
		case TRANID_4813:
			Data = GetSprintf("납부번호:%19.19s", m_RXJIRODATA.ElecNum);
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			if(Asc2Int(Accept.JiroBillCount,2) >= 2 )
				Data = GetSprintf("세목:%s", strGetBufferArray[2]);
			break;
		default:
			Data = GetSprintf(SPACE16);	
			break;
		}		
		break;		


	case 16:	
		switch(TranCode2)
		{
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
			break;
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:
		case TRANID_7126:
			if(!m_pDevCmn->TranResult) 
			{
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("[Deposit Detail]");
				else
					Data = GetSprintf("[입금상세]");
			}
			else
			{
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("[Deposit Detail]");
				else
					Data = GetSprintf("[입금상세]");
			}
			break;
		case TRANID_4810:
		case TRANID_4813:
			if(Asc2Int(Accept.JiroBillCount,2) >= 1 ) //#N0162-1
				Data = GetSprintf("납부내역(년/월)     금액");  //#SJ 1115 명세표 수정
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			if(Asc2Int(Accept.JiroBillCount,2) >= 2 )
				Data = GetSprintf("거래금액:%s원", strGetBufferArray[3]);
			break;
		default:
			break;
		}		
		break;

	case 17:
		switch(TranCode2)
		{
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
			if(!m_pDevCmn->TranResult) 
				break;

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Recv.Org:%s", m_RD.byDataField_012x);
			else
				Data = GetSprintf("수취기관:%s", m_RD.byDataField_012x);
			break;
		case TRANID_4350:
			if(!m_pDevCmn->TranResult) 
				break;
			memset(szTempBuff2, 0x00, sizeof(szTempBuff2));
			EditString(m_RD.byDataField_020x, 16, "########$$$$$$$$", szTempBuff2, '#', '$', '*', TRUE);
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Recv. B,S:%s", &szTempBuff2);
			else
				Data = GetSprintf("수취계좌:%s", &szTempBuff2);
			Data = "@" + Data;
			break;
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:
		case TRANID_7126:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("50K:%d 10K:%d 5K:%d 1K:%d", CashData.mFiftyThousand, CashData.mTenThousand, CashData.mFiveThousand, CashData.mOneThousand);
			else
				Data = GetSprintf("5만:%d 1만:%d 오:%d 천:%d", CashData.mFiftyThousand, CashData.mTenThousand, CashData.mFiveThousand, CashData.mOneThousand); //#0093
			break;
		case TRANID_4810:
		case TRANID_4813:
			if(Asc2Int(Accept.JiroBillCount,2) >= 1 ) //#N0162-1
				Data = GetSprintf("%s", strGetBufferArray[0]);
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			if(Asc2Int(Accept.JiroBillCount,2) >= 3 )
				Data = GetSprintf("세목:%s", strGetBufferArray[4]);
			break;
		default:
			break;
		}		
		break;
	case 18:		
		switch(TranCode2)
		{
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
			if(!m_pDevCmn->TranResult) 
				break;	
			memset(szTempBuff, 0x00, sizeof(szTempBuff));
			memset(szTempBuff2, 0x00, sizeof(szTempBuff2));
			if(Accept.InAccountNumSize > 8)
			{
//				memset(szTempBuff, '#', Accept.InAccountNumSize);
//				memset(&szTempBuff[Accept.InAccountNumSize - 8], '$', Accept.InAccountNumSize - 8);
				EditString(Accept.InAccountNum, 16, "########$$$$$$$$", szTempBuff2, '#', '$', '*', TRUE);
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("Recv.B,S:%s", szTempBuff2);
				else
					Data = GetSprintf("수취계좌:%s", szTempBuff2);
			}
			else
			{
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("Recv. B,S: %s", Accept.InAccountNum);
				else
					Data = GetSprintf("수취계좌:%s", Accept.InAccountNum);				
			}
			Data = "@" + Data;
			break;
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:
		case TRANID_7126:
			if(!m_pDevCmn->TranResult) 
			{
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("Please check your balance.");
				else
					Data = GetSprintf("잔액 확인요");
			}
			break;
		case TRANID_4810:
		case TRANID_4813:
			if(Asc2Int(Accept.JiroBillCount, 2) >= 2)
			Data = GetSprintf("%s", strGetBufferArray[1]);
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			if(Asc2Int(Accept.JiroBillCount,2) >= 3 )
				Data = GetSprintf("거래금액:%s원", strGetBufferArray[5]);
			break;
		default:
			break;
		}		
		break;		
	case 19:	
		switch(TranCode2)
		{
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
			if(!m_pDevCmn->TranResult) 
				break;

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Benefic.:%s", strCT(m_RD.byDataField_021x, strlen(m_RD.byDataField_021x)));
			else
				Data = GetSprintf("수취인  :%s", strCT(m_RD.byDataField_021x, strlen(m_RD.byDataField_021x)));
			break;
		case TRANID_4810:
		case TRANID_4813:
			if(Asc2Int(Accept.JiroBillCount, 2) >= 3)
			Data = GetSprintf("%s", strGetBufferArray[2]);
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			if(Asc2Int(Accept.JiroBillCount,2) >= 4 )
				Data = GetSprintf("세목:%s", strGetBufferArray[6]);
			break;
		default:
			Data = GetSprintf(SPACE16);	
			break;
		}		
		break;	
	case 20:
		switch(TranCode2)
		{
		case TRANID_4810:
		case TRANID_4813:
			if(Asc2Int(Accept.JiroBillCount, 2) >= 4)
			Data = GetSprintf("%s", strGetBufferArray[3]);
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			if(Asc2Int(Accept.JiroBillCount,2) >= 4 )
				Data = GetSprintf("거래금액:%s원", strGetBufferArray[7]);
			break;
		default:
			Data = GetSprintf(SPACE16);	
			break;
		}		
		break;
	case 21:
		switch(TranCode2)
		{
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			break;
		default:
			if(m_pDevCmn->TranResult)
			{
				if(m_pDevCmn->HostLocalSvrMode)
				{
					memcpy(szTmp, GetTime().GetBuffer(0), 6);
					Data = GetSprintf("%2.2s:%2.2s   [LOCAL TEST MODE]", &szTmp[0], &szTmp[2]);
				}
// 				else
// 				{
// 					Data = GetSprintf("%s", strCT(m_RD.byDataField_104x, 20));
// 				}
			}
			else
			{
				if (!m_pDevCmn->fnAPL_CheckError())							// 장애검지유
					Data = GetSprintf("ErrorCode: %c-%5.5s", m_pDevCmn->CurErrBuff.ProcCount[0], m_pDevCmn->CurErrBuff.ErrorCode);
				else
					Data = GetSprintf(SPACE16);	

			}
			break;
		}
		break;
	case 22:	
		if(!m_pDevCmn->TranResult) 
			break;

//		Data = GetSprintf("%s", strCT(&m_RD.byDataField_104x[20], 20));
		break;
	case 23:	
		if(!m_pDevCmn->TranResult) 
			break;

//		Data = GetSprintf("%s", strCT(&m_RD.byDataField_104x[40], 20));
		break;
	case 24:
		if(!m_pDevCmn->TranResult) 
			break;	
//		Data = GetSprintf("%s", strCT(&m_RD.byDataField_104x[60], 20));
		break;
	default:
		break;   
	}    

	if (Data.GetLength())
	{
		memcpy((char*)cData, Data.GetBuffer(Data.GetLength()), Data.GetLength()); 
		if(m_pDevCmn->HostLocalSvrMode)
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EditPrintLine : (%s)", Data.GetBuffer(Data.GetLength()));	// TEST!!!!!
	}

	Data = "";
	
	return T_OK;
}

/**************************************************************************************************/
/****01234567890123456789012345678901234567890   01234567890123456789012345678901234567890
/* 0.                                           |
/* 1.                                           |
/* 2.      [MyCashZone  거 래 명 세 표]         |         [MyCashZone  Receipt]          
/* 3.                                           |
/* 4.  거래일자 : 20090709-18:26 ,              |  Date/Time: 20090709-18:26 
/* 5.  처리번호 : NH0001                        |  Seque No : NH0001  
/* 6.  거래종류 : 현금지급	                    |  Trans.   : 현금지급
/* 7.  거래기관 : 은행,증권                     |  Service  : 은행,증권
/* 8.  계좌번호 : 1234567890123456              |  CARD No. : 1234567890123456	
/* 9.  거래금액 : 23,000원                      |  Req. ATM : 23,000원
/*10.  고유번호 : 123456789012                  |  Trans.NO : 123456789012	
/*11.                                           |  
/*12.  수수료   : 600원	                        |  Fee      : 600원
/*13.  계좌잔액 : 23,178,000원                  |  Balance  : 23,178,000원                    
/*14.  타점권   : 0원                           |  Oth. AMT : 0원	
/*15.  금융기관 : 국민                          |  Org.Name : 국민
/*15.1 계좌명   : 김기진 <- 입금계좌            |  
/*16.  수취기관 : 제일                          |  Recv.Org : 제일   
/*17.  수취계좌 : 1234567890123456              |  Recv. B,S: 1234567890123456 
/*18.  수취인   : 김기진                        |  Benefic. : 김기진 
/*19.  ErrorCode: 0000                          |  ErrorCode: 0000
/*20.  에러메시지(서버)                         |  에러메시지(서버)  
/*21.  Tel : 080-920-2220                       |  Tel : 080-920-2220
/*22.	
/**************************************************************************************************/
	
 

// 명세표/저널 편집(Finance Service Print Form 2009.07.13)
int	CTranCmn::fnAPP_EditPrintLine_Sub_T3_1(int PrintDevice, void* cData, int LineNum, int HiddenFlg)
{
	CString Data;
	CString	cstrTemp("");
	CString strTranKind("");
	char	szTmp[1024];
	char	szTempBuff[256] = {0,};
	char	szTempBuff2[256] = {0,};
	char	CurDate[256];
	char	CurTime[256];
	int     i = 0;

	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EditPrintLine : (%d)", LineNum);	// TEST!!!!!
	//Bank Row set
//	m_pDevCmn->SprRowOption = SPR_MAX_ROW;
//#N0132
	if (m_pProfile->DEVICE.MachineType == U3100K)
		m_pDevCmn->SprRowOption = SPR_MAX_ROW;
	else
	if (m_pProfile->DEVICE.MachineType == U8100)
	{
		if (TranProc == TRAN_HIPASS)		//#N0266 명세표 인자 라인
		{
			m_pDevCmn->SprRowOption = SPR_MAX_ROW;
		}
		else
			m_pDevCmn->SprRowOption = SPR_MAX_ROW_U8100;
	}
	else
		m_pDevCmn->SprRowOption = SPR_MAX_ROW_U8100;
		
	char*	TranListTbl[50] = 
	{
//		 12345678901234567890
		"현금지급           ", //0
		"계좌이체           ", //1
		"단기카드대출 출금  ", //2 #N0247
		"결제계좌이체       ", //3
		"단기카드대출 이체  ", //4
		"보험계약대출(출금) ", //5
		"보험계약대출 조회  ", //6
		"지로공과금 납부    ", //7
		"지로공과금 납부내역", //8
		"아파트관리비 납부  ", //9
		"신용정보조회       ", //10
		"대출정보조회       ", //11
		"카드론             ", //12
		"동부화재계약자보험 ", //13
		"Deposit            ", //14
		"입금               ", //15
		"아파트관리비 납부  ", //16
		"지로공과금 납부    ", //17
		"캐쉬백 조회        ", //18
		"캐쉬백 적립        ", //19
		"Cash Advance       ", //20
		"Cash Withdrawal    ", //21
		"단기카드대출 조회  ", //22
		"결제대금조회       ", //23
		"조회               ", //24
		"T-Money 충전       ", //25
		"CGV                ", //26
		"Wire Transfer      ", //27
		"DOTONG             ", //28
		"단기연체정보조회   ", //29
		"무매체 입금        ", //30
		"신용평점정보조회   ", //31
		"증권출금           ", //32
		"증권조회           ", //33
		"Withdrawal         ", //34
		"Balance Inquiry    ", //35
		"Account Transfer   ", //36
		"통장정리           ", //37
		"전자상품권구매     ", //38
		"전자상품권취소     ", //39
		"ONE페이출금	    ", //40 #N0248 => #N0259
		"ONE페이출금취소	", //41	#N0248 => #N0259
		"하이패스충전(신용)	", //42	#N0266
		"하이패스 신용취소	", //43	#N0266
		"하이패스충전(현금)	", //44	#N0270
		"???????????????????"
	};
	
	char*	TranNameTbl[40] = 
	{
//		 12345678901234567890
		"은행,증권          ", //0
		"신용카드           ", //1
		"삼성카드론         ", //2
		"한국신용정보       ", //3
		"???????????????????"
	};

	if(EMVReadFlg == TRUE)
		strTranKind = " (IC)";
	else
	if(MenuFICMode & MENU_IC)
		strTranKind = " (IC)";
	else
	if(PbTran & MENU_PB)
		strTranKind = " (통장)";
	else
	if(MenuAvail & MENU_MU)
		strTranKind = " (무)";
	else
		strTranKind = " (MS)";

	GetDate(CurDate);
	GetTime(CurTime);


	Data.Empty();
	memset(szTmp, 0x00, sizeof(szTmp));

	if (PrintDevice == (DEV_SPR | DEV_JPR))						// 인자데이타 편집은 하나의 장치ID만..~!!
		return T_OK;

	if (PrintDevice & (DEV_SPR | DEV_JPR) == DEV_NONE)			// 인자데이타 편집은 하나의 장치ID만..~!!
		return T_OK;

	memset(szTmp, 0, sizeof(szTmp));

	if(m_pDevCmn->fnSCR_GetCurrentLangMode() == KEB_MODE)  // 이지원 명세표 인자 한글 #N0181
		m_pDevCmn->fnSCR_SetCurrentLangMode(KOR_MODE);

	switch (LineNum)
	{													
	case 0:
		break;
	case 1:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			Data = GetSprintf("         [EasyCash Receipt]      ");
		else
			Data = GetSprintf("   [이지캐쉬 현금지급기 거래명세표]    ");

		break;
	case 2:
		Data = GetSprintf(SPACE16);	
		break;
	case 3:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			Data = GetSprintf("  Date/Time: %4.4s%2.2s%2.2s-%2.2s:%2.2s", &CurDate[0], &CurDate[4],	&CurDate[6], &CurTime[0], &CurTime[2]);
		else
			Data = GetSprintf("  거래일자 : %4.4s%2.2s%2.2s-%2.2s:%2.2s", &CurDate[0], &CurDate[4],	&CurDate[6], &CurTime[0], &CurTime[2]);

		break;
	case 4:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			Data = GetSprintf("  Atm  Num : %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
		else
			Data = GetSprintf("  기기번호 : %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
		break;
	case 5:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			Data = GetSprintf("  Seque No : %6.6s", m_pProfile->TRANS.SerialNo);
		else
			Data = GetSprintf("  처리번호 : %6.6s", m_pProfile->TRANS.SerialNo);
		break;

	case 6:		
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4320:
		case TRANID_4220:
		case TRANID_4210:
		case TRANID_4116:
		case TRANID_4326:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Trans.   : %s", TranListTbl[21]);
			else
				Data = GetSprintf("  거래종류 : %s", TranListTbl[0]) ;
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_7220:
		case TRANID_7230:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Trans.   : %s", TranListTbl[35]);
			else
				Data = GetSprintf("  거래종류 : %s", TranListTbl[24]) ;
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_4340:
				if(TranCode == TC_FOREGIN)
				{
					if(memcmp(Accept.MuConfirmNum, "CC", 2) == 0)  ////#N0171
						Data = GetSprintf("  Trans.   : %s", "CUP Withdrawal");
					else
						Data = GetSprintf("  Trans.   : %s", TranListTbl[21]);
				}
				else
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("  Trans.   : %s", TranListTbl[34]);
				else
					Data = GetSprintf("  거래종류 : %s", TranListTbl[2]);
					Data.TrimRight();
					Data += strTranKind;
			break;
		case TRANID_4330:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Trans.   : %s", TranListTbl[34]);
			else
				Data = GetSprintf("  거래종류 : %s", TranListTbl[2]) ;
				Data.TrimRight();
				if(EMVReadFlg == TRUE)   //#N0247
					Data += strTranKind;
			break;
		case TRANID_4350:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Trans.   : %s", TranListTbl[36]);
			else
				Data = GetSprintf("  거래종류 : %s", TranListTbl[4]) ;
				Data.TrimRight();
				if(EMVReadFlg == TRUE)   //#N0247
					Data += strTranKind;
			break;
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_7276:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Trans.   : %s", TranListTbl[36]);
			else
				Data = GetSprintf("  거래종류 : %s", TranListTbl[1]) ;
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:
		case TRANID_7126:
		case TRANID_7236:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Trans.   : %s", TranListTbl[14]);
			else
				Data = GetSprintf("  거래종류 : %s", TranListTbl[15]);
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_4730: //#0024
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Trans.   : %s", "Passbook Arrangement");
			else
				Data = GetSprintf("  거래종류 : %s",TranListTbl[37]);   //#0036
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_4117:   //#N0182
			Data = GetSprintf("  거래종류 : %s", TranListTbl[40]);
			Data.TrimRight();
			Data += strTranKind;
			break;
		case TRANID_4327:   //#N0182
			Data = GetSprintf("  거래종류 : %s", TranListTbl[41]);
			Data.TrimRight();
			Data += strTranKind;
			break;
		case TRANID_4850:	//#N0266
			Data = GetSprintf("  거래종류 : %s", TranListTbl[43]);
			Data.TrimRight();
			break;
		case TRANID_4717:	//#N0270
			Data = GetSprintf("  거래종류 : %s", TranListTbl[44]);
			Data.TrimRight();
			break;
		case TRANID_4840:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			if(TranCode == TC_HIPASS_CREDIT)
			{
				Data = GetSprintf("  거래종류 : %s", TranListTbl[42]);
				Data.TrimRight();		
			}
			else
			if(TranCode == TC_HIPASS_CASH)
			{
				Data = GetSprintf("  거래종류 : %s", TranListTbl[44]);
				Data.TrimRight();
			}
			break;
		default:
			break;
		}		
		break;
	case 7:														
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:
		case TRANID_4117:   //#N0182
		case TRANID_4320:
		case TRANID_4220:
		case TRANID_7220:
		case TRANID_7290:
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:
		case TRANID_7126:
		case TRANID_4730:
		case TRANID_4326:
		case TRANID_4327:   //#N0182
		case TRANID_7276:
		case TRANID_4396:
		case TRANID_7236:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Service  : %s", fnAPP_Get_BankName(CardBank));	
			else
				Data = GetSprintf("  거래기관 : %s", fnAPP_Get_BankName(CardBank));				
			break;
		case TRANID_4350:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Service  : %s", "Credit");	
			else
				Data = GetSprintf("  거래기관 : %s", fnAPP_Get_CreditName(pCardData2->MembershipNo)); //#0100
			break;
		case TRANID_4717:	//#N0270
		case TRANID_4840:	//#N0266
		case TRANID_4850:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			if (TranCode == TC_HIPASS_CREDIT)
			{
				if(!m_pDevCmn->TranResult)
				{
					if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
						EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
					else
					if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
						EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
					else
						EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				}
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("  CARD No. : %s", m_RD.byDataField_017x);
				else
					Data = GetSprintf("  계좌번호 : %s", m_RD.byDataField_017x);
				Data = "@" + Data;
			}
			break;
		case TRANID_7230:
		case TRANID_4330:
		case TRANID_4340:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Service  : %s", "Credit");	
			else
				Data = GetSprintf("  거래기관 : %s", fnAPP_Get_CreditName(pCardData2->MembershipNo)); //#0100
			break;
		case TRANID_4311:   //#N0171
		case TRANID_4321:   //#N0171
			Data = GetSprintf("  Service  : %s", "SAVINGACCOUNT");	// #N0278
			break;
		case TRANID_4312:
		case TRANID_4322:
			Data = GetSprintf("  Service  : %s", "CHECKINGACCOUNT"); // #N0278
			break;
		case TRANID_4331:
		case TRANID_4341:
			Data = GetSprintf("  Service  : %s", "CREDITCARD"); // #N0278
			break;
		case TRANID_4332:   //#N0171
		case TRANID_4342:   //#N0171
			if(memcmp(Accept.MuConfirmNum, "CC", 2) == 0)
				Data = GetSprintf("  Service  : %s", "CUP Card");
			else
				Data = GetSprintf("  Service  : %s", "Foregin Card");
			break;
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Service  : %s", fnAPP_Get_BankName(CardBank));	
			else
				Data = GetSprintf("  거래기관 : %s", fnAPP_Get_BankName(CardBank));				
			break;
		default:
			break;
		}		
		break;
	case 8:
		if (m_pProfile->DEVICE.MachineType == U3100K)  //#N0132
			Data = GetSprintf(SPACE16);
		break;
	case 9:														
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:
		case TRANID_7220:
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:	
		case TRANID_7126:
		case TRANID_4320:
		case TRANID_4220:
		case TRANID_4730:
		case TRANID_4326:
		case TRANID_7276:
		case TRANID_7236:


			if (PbTran & MENU_PB) //#0036
			{
				if(m_pProfile->DEVICE.ETCDevice20 == KYONGNAM)
				{
					if(!IsZero(&Accept.AccountNum, 4))
						EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
					else
					if(!IsSpace(&Accept.AccountNum, 14))
						EditString(&Accept.AccountNum[0], 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
					else
						EditString(ZERO16, 14, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				}
				else
				{
				if(!IsZero(&m_pTranCmn->pPbMsData->AccountNo, 16)) //#0024
					EditString(&m_pTranCmn->pPbMsData->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
				if(!IsSpace(&m_pTranCmn->pPbMsData->AccountNo, 16))
					EditString(&m_pTranCmn->pPbMsData->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
					EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				}

				MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fn_FRX_TRANID_7100: DataField Not Found - x017X[%s]" , x017X);
			}
			else
			if (MenuAvail & MENU_MU) //#0038
			{
				if(!IsZero(&Accept.MuAccountNum, 16)) //#0024
					EditString(&Accept.MuAccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
				if(!IsSpace(&Accept.MuAccountNum, 16))
					EditString(&Accept.MuAccountNum[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
					EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				
			}
			else
			{
				//계좌번호	
		//		sprintf(m_RD.byDataField_017x, "%s", strGetRxDataField.GetBuffer(0));
				if(!IsZero(&m_pTranCmn->pCardData3->AccountNo, 16))
					EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
				if(!IsSpace(&m_pTranCmn->pCardData3->AccountNo, 16))
					EditString(&m_pTranCmn->pCardData3->AccountNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
					EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);

			}				

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  CARD No. : %s", m_RD.byDataField_017x);
			else
				Data = GetSprintf("  계좌번호 : %s", m_RD.byDataField_017x);
			Data = "@" + Data;
			break;
		case TRANID_4117:   //#N0182
		case TRANID_4327:   //#N0182
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  CARD No. : %6.6s**********", Accept.PbConfirmNum);
			else
				Data = GetSprintf("  계좌번호 : %6.6s**********", Accept.PbConfirmNum);
			Data = "@" + Data;
			break;
		case TRANID_4330:
		case TRANID_7230:
		case TRANID_4350:
		case TRANID_4340:
		case TRANID_4311:   //#N0171
		case TRANID_4312:
		case TRANID_4331:
		case TRANID_4332:   //#N0171
		case TRANID_4342:   //#N0171
		case TRANID_4321:   //#N0171
		case TRANID_4322:
		case TRANID_4341:
			if(!m_pDevCmn->TranResult)
			{
				if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
					EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
				if(!IsZero(&m_pTranCmn->pCardData2->MembershipNo, 16))
					EditString(&m_pTranCmn->pCardData2->MembershipNo[0], 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
				else
					EditString(ZERO16, 16, "########$$$$$$$$", m_RD.byDataField_017x, '#', '$', '*', TRUE);
			}
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  CARD No. : %s", m_RD.byDataField_017x);
			else
				Data = GetSprintf("  계좌번호 : %s", m_RD.byDataField_017x);
			Data = "@" + Data;
			break;
		case TRANID_4717:	//#N0270
		case TRANID_4840:	//#N0266
		case TRANID_4850:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			if(!m_pDevCmn->TranResult)
			{
				sprintf(m_RD.byDataField_023x, "%15.15s", Accept.Money);	
			}

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  R.Amount : %s Won",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			else
				Data = GetSprintf("  거래금액 : %s원",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			break;
		default:
			break;
		}		
		break;
		
	case 10:														// 거래금액
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:
		case TRANID_4117:   //#N0182
		case TRANID_7220:
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:	
		case TRANID_7126:
		case TRANID_4320:
		case TRANID_4220:
		case TRANID_4326:
		case TRANID_4327:   //#N0182
		case TRANID_7276:
		case TRANID_7236:
			if(!m_pDevCmn->TranResult)
			{
				sprintf(m_RD.byDataField_023x, "%15.15s", Accept.Money);	
			}

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  R.Amount : %s Won",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			else
				Data = GetSprintf("  거래금액 : %s원",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			break;
		case TRANID_4330:
		case TRANID_7230:
		case TRANID_4350:
		case TRANID_4340:
		case TRANID_4311:   //#N0171
		case TRANID_4312:
		case TRANID_4331:
		case TRANID_4332:   //#N0171
		case TRANID_4342:   //#N0171
		case TRANID_4321:   //#N0171
		case TRANID_4322:
		case TRANID_4341:
			if(!m_pDevCmn->TranResult)
			{
				sprintf(m_RD.byDataField_023x, "%15.15s", Accept.Money);	
			}

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  R.Amount : %s Won",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			else
				Data = GetSprintf("  거래금액 : %s원",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			break;
		case TRANID_4717:	//#N0270
		case TRANID_4840:	//#N0266
		case TRANID_4850:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			if (TranCode == TC_HIPASS_CREDIT)
				Data = GetSprintf("  승인번호 : %8.8s", Accept.ETicketConfirmNum);
			break;
		default:
			break;
		}		
		break;
	case 11:														// 거래 후 금액
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:
		case TRANID_4117:   //#N0182
		case TRANID_7220:
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:	
		case TRANID_7126:
		case TRANID_4320:
		case TRANID_4220:
		case TRANID_4326:
		case TRANID_4327:   //#N0182
		case TRANID_7276:
		case TRANID_7236:
			if(!m_pDevCmn->TranResult) 
				break;

			if (m_RD.byDataField_026x[0] = '?')
				break;

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Trans.NO : %12.12s", m_RD.byDataField_026x);
			else
				Data = GetSprintf("  고유번호 : %12.12s", m_RD.byDataField_026x);
			break;
		case TRANID_4330:
		case TRANID_7230:
		case TRANID_4350:
		case TRANID_4340:
		case TRANID_4311:   //#N0171
		case TRANID_4312:
		case TRANID_4331:
		case TRANID_4332:   //#N0171
		case TRANID_4342:   //#N0171
		case TRANID_4321:   //#N0171
		case TRANID_4322:
		case TRANID_4341:
			if(!m_pDevCmn->TranResult) 
				break;

			if (m_RD.byDataField_026x[0] = '?')
				break;

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Trans.NO : %12.12s", m_RD.byDataField_026x);
			else
				Data = GetSprintf("  고유번호 : %12.12s", m_RD.byDataField_026x);
			break;
		case TRANID_4717:	//#N0270
		case TRANID_4840:	//#N0266
		case TRANID_4850:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			Data = GetSprintf("  수수료   : %s원", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			break;
		default:
			break;
		}		
		break;
	case 12:
//		Data = GetSprintf(SPACE16);	
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)		
			Data = GetSprintf("  Response:%03d[%s]", TranResultNGCode, (TranResultNGCode == 0)	? "OK" : "NG"); 
		else
			Data = GetSprintf("  응답코드 : %03d", TranResultNGCode);
		break;

	//#CS0006 (거래결과 추가 - 청호요청)
	case 13:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)		
		{
			cstrTemp.Format("%03d", TranResultNGCode);
			if( IniGetStr(_ECASH_SVR_RES_INI, "PRINT_MSG", cstrTemp, SPACE3).GetLength() < 29 )
				Data = GetSprintf("  거래결과: %s", IniGetStr(_ECASH_SVR_RES_INI, "PRINT_MSG", cstrTemp, SPACE3));
		}
		break;
	case 14:	
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:
		case TRANID_4117:   //#N0182
		case TRANID_7220:
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_4320:
		case TRANID_4220:
		case TRANID_4326:
		case TRANID_4327:   //#N0182
		case TRANID_7276:
		case TRANID_4716:
			if(!m_pDevCmn->TranResult) 
				break;

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Fee      : %s Won", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			else
				Data = GetSprintf("  수수료   : %s원", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			break;
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_7200:	
		case TRANID_7126:
		case TRANID_4730:
		case TRANID_7236:
			if(!m_pDevCmn->TranResult) ;
//				Data = GetSprintf("  응답코드   : %03d [거래불능]", TranResultNGCode);
			else
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Fee      : %s Won", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			else
				Data = GetSprintf("  수수료   : %s원", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			break;
		case TRANID_4330:
		case TRANID_7230:
		case TRANID_4350:
		case TRANID_4340:
		case TRANID_4311:   //#N0171
		case TRANID_4312:
		case TRANID_4331:
		case TRANID_4332:   //#N0171
		case TRANID_4342:   //#N0171
		case TRANID_4321:   //#N0171
		case TRANID_4322:
		case TRANID_4341:
			if(!m_pDevCmn->TranResult) 
				break;

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)  //-1  명세표 수수료 **** 표시
			{
				if(memcmp(m_RD.byDataField_024x, "******", 6) == 0)
					Data = GetSprintf("  Fee      : %s Won", strCT(m_RD.byDataField_024x, 7));
				else
				{
					if(memcmp(Accept.MuConfirmNum, "CC", 2) == 0)   //#N0171
						Data = GetSprintf("  Fee      : %s", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
					else
						Data = GetSprintf("  Fee      : %s Won", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
				}
			}
			else
			{
				if(memcmp(m_RD.byDataField_024x, "******", 6) == 0)
					Data = GetSprintf("  수수료   : %s원", strCT(m_RD.byDataField_024x, 7));
				else
					Data = GetSprintf("  수수료   : %s원", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			}
			
			//if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			//	Data = GetSprintf("  Fee      : %s Won", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			//else
			//	Data = GetSprintf("  수수료   : %s원", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			
			break;
		case TRANID_4717:	//#N0270
		case TRANID_4840:	//#N0266
		case TRANID_4850:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			Data = GetSprintf("  [충전내역]");
			break;
		default:
			break;
		}		
		break;
	case 15:	
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:
		case TRANID_4117:   //#N0182
		case TRANID_7220:
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_7200:	
		case TRANID_4320:
		case TRANID_4220:
		case TRANID_4326:
		case TRANID_4327:   //#N0182
		case TRANID_7276:
		case TRANID_7236:
			if(!m_pDevCmn->TranResult) 
				break;

			if(m_pProfile->DEVICE.ETCDevice20 == KB && TranCode2 == TRANID_4720)	//#N0226 국민통장 입금시 명세표 잔액 출력 안함
				break;
			
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Balance  : %s Won", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
			else
				Data = GetSprintf("  계좌잔액 : %s원", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
			Data = "@" + Data;
			break;
		case TRANID_4330:
		case TRANID_7230:
		case TRANID_4350:
		case TRANID_4340:
		case TRANID_4332:   //#N0171
		case TRANID_4342:   //#N0171
			if(!m_pDevCmn->TranResult) 
				break;

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			{
				if(TranCode == TC_FOREGIN)
				{
					if(memcmp(Accept.MuConfirmNum, "CC", 2) == 0)  //#N0171 은련은 잔액 표시 X
						;
					else
						Data = GetSprintf("  Balance  : %s", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
				}
				else
					Data = GetSprintf("  Balance  : %s Won", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
			}
			else
				Data = GetSprintf("  계좌잔액 : %s원", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
			Data = "@" + Data;
			break;
		case TRANID_4850:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			Data = GetSprintf("  할증액 : %s원",  strCT(Asc2Amt(m_RD.byDataField_065x, strlen(m_RD.byDataField_065x), strlen(m_RD.byDataField_065x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_065x, strlen(m_RD.byDataField_065x), strlen(m_RD.byDataField_065x)+3).GetLength()));
			break;
		default:
			break;
		}		
		break;
	case 16:	
		switch(TranCode2)
		{
		case TRANID_4310:
		case TRANID_4210:
		case TRANID_4116:
		case TRANID_4117:   //#N0182
		case TRANID_4215:
		case TRANID_4320:
		case TRANID_4220:
		case TRANID_4340:
		case TRANID_4326:
		case TRANID_4327:   //#N0182
		case TRANID_4330:  
			if(!m_pDevCmn->TranResult) 
				break;
			//#N0188
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE) //#KIMH0145 => #N0192 명세표 내용 수정
				Data = GetSprintf("  50K: %d, 10K: %d", Asc2Int(Accept.Cash50T_Money, 15)/50000 , Asc2Int(Accept.Cash10T_Money, 15)/10000);
			else
				Data = GetSprintf("  오만원: %d매, 만원: %d매", Asc2Int(Accept.Cash50T_Money, 15)/50000 , Asc2Int(Accept.Cash10T_Money, 15)/10000);
			break;
		case TRANID_7220:
		case TRANID_4350:
		case TRANID_7260:
		case TRANID_7126:
		case TRANID_7276:
		case TRANID_7236:
		case TRANID_7230:
		case TRANID_4716:
		case TRANID_4396:
		case TRANID_4390:
		case TRANID_4370:
			break;
		case TRANID_4717:	//#N0270
		case TRANID_4840:	//#N0266
		case TRANID_4850:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			Data = GetSprintf("  충전금액 : %s원",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			break;
		default:
			Data = GetSprintf(SPACE16);	
			break;
		}		
		break;		


	case 17:	
		switch(TranCode2)
		{
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
			break;
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:
		case TRANID_7126:
		case TRANID_7236:
			if(!m_pDevCmn->TranResult) 
			{
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("  [Deposit Detail]");
				else
					Data = GetSprintf("  [입금상세]");
			}
			else
			{
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("  [Deposit Detail]");
				else
					Data = GetSprintf("  [입금상세]");
			}
			break;
		default:
			break;
		}		
		break;

	case 18:
		switch(TranCode2)
		{
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_7276:
			if(!m_pDevCmn->TranResult) 
				break;

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Recv.Org : %s", m_RD.byDataField_012x);
			else
				Data = GetSprintf("  수취기관 : %s", m_RD.byDataField_012x);
			break;
		case TRANID_4350:
			if(!m_pDevCmn->TranResult) 
				break;
			memset(szTempBuff2, 0x00, sizeof(szTempBuff2));
			EditString(m_RD.byDataField_020x, 16, "########$$$$$$$$", szTempBuff2, '#', '$', '*', TRUE);
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Recv. B,S : %s", &szTempBuff2);
			else
				Data = GetSprintf("  수취계좌 : %s", &szTempBuff2);
			Data = "@" + Data;
			break;
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:
		case TRANID_7126:
		case TRANID_7236:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE) //명세표 내용 수정 오 => 오천
				Data = GetSprintf("  50K:%d 10K:%d 5K:%d 1K:%d", CashData.mFiftyThousand, CashData.mTenThousand, CashData.mFiveThousand, CashData.mOneThousand);
			else
				Data = GetSprintf("  5만:%d 만원:%d 5천:%d 천:%d", CashData.mFiftyThousand, CashData.mTenThousand, CashData.mFiveThousand, CashData.mOneThousand);
			break;
		default:
			break;
		}		
		break;
	case 19:		
		switch(TranCode2)
		{
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_7276:
			if(!m_pDevCmn->TranResult) 
				break;	
			memset(szTempBuff, 0x00, sizeof(szTempBuff));
			memset(szTempBuff2, 0x00, sizeof(szTempBuff2));
			if(Accept.InAccountNumSize > 8)
			{
//				memset(szTempBuff, '#', Accept.InAccountNumSize);
//				memset(&szTempBuff[Accept.InAccountNumSize - 8], '$', Accept.InAccountNumSize - 8);
				EditString(Accept.InAccountNum, 16, "########$$$$$$$$", szTempBuff2, '#', '$', '*', TRUE);
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("  Recv. B,S: %s", szTempBuff2);
				else
					Data = GetSprintf("  수취계좌 : %s", szTempBuff2);
			}
			else
			{
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("  Recv. B,S: %s", Accept.InAccountNum);
				else
					Data = GetSprintf("  수취계좌 : %s", Accept.InAccountNum);				
			}
			Data = "@" + Data;
			break;
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:
		case TRANID_7126:
		case TRANID_7236:
			if(!m_pDevCmn->TranResult) 
			{
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("  Please check your balance.");
				else
					Data = GetSprintf("  잔액 확인요");
			}
			break;
		case TRANID_4717:	//#N0270
		case TRANID_4840:	//#N0266
		case TRANID_4850:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			Data = GetSprintf("  사업자번호 : 129-86-11103");
			break;
		default:
			break;
		}		
		break;		
	case 20:	
		switch(TranCode2)
		{
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
		case TRANID_7276:
			if(!m_pDevCmn->TranResult) 
				break;

			if(TranProcSave == TRAN_EASYONE)
			{
				Data = GetSprintf("  수취인   : %s", strCT(Accept.EasyOneName, 25));
				break;
			}

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Benefic. : %s", strCT(m_RD.byDataField_021x, strlen(m_RD.byDataField_021x)));
			else
				Data = GetSprintf("  수취인   : %s", strCT(m_RD.byDataField_021x, strlen(m_RD.byDataField_021x)));
			break;
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			Data = GetSprintf("  대표자명 : 박기재");
			break;
		default:
			Data = GetSprintf(SPACE16);	
			break;
		}		
		break;	
	case 21:
		switch(TranCode2)
		{
			case TRANID_4717:	//#N0270
			case TRANID_4840:	//#N0266
			case TRANID_4850:	//#N0266
			case TRANID_4670:	//#N0266
			case TRANID_4671:	//#N0266
				Data = GetSprintf("  가맹점명 : 하이플러스카드(주)");
				break;
			default:
				Data = GetSprintf(SPACE16);	
				break;
		}
		break;
	case 22:
		if(m_pDevCmn->TranResult)
		{
			if(m_pDevCmn->HostLocalSvrMode)
			{
				memcpy(szTmp, GetTime().GetBuffer(0), 6);
				Data = GetSprintf("  %2.2s:%2.2s   [LOCAL TEST MODE]", &szTmp[0], &szTmp[2]);
			}
// 			else
// 			{
// 				Data = GetSprintf("%s", strCT(m_RD.byDataField_104x, 20));
// 			}
		}
		else
		{
			if (!m_pDevCmn->fnAPL_CheckError())							// 장애검지유
				Data = GetSprintf("  ErrorCode: %c-%5.5s", m_pDevCmn->CurErrBuff.ProcCount[0], m_pDevCmn->CurErrBuff.ErrorCode);
			else
				Data = GetSprintf(SPACE16);	

		}		
		break;
	case 23:
		break;

		if(!m_pDevCmn->TranResult) 
			break;
	case 24:
		break;

		if(!m_pDevCmn->TranResult) 
			break;		
	case 25:
		break;
			if(!m_pDevCmn->TranResult) 
			break;
	case 26:

		if(!m_pDevCmn->TranResult) 
			break;
	default:
		break;  
	}    

	if (Data.GetLength())
	{
		memcpy((char*)cData, Data.GetBuffer(Data.GetLength()), Data.GetLength()); 
		if(m_pDevCmn->HostLocalSvrMode)
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EditPrintLine : (%s)", Data.GetBuffer(Data.GetLength()));	// TEST!!!!!
	}

	Data = "";

	// #N0174 - 이지원 명세표 출력시 - 국문 출격을 위해 예외처리 (국문설정) - 06.04
	if((TranProcSave == TRAN_EASYONE) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
		m_pDevCmn->fnSCR_SetCurrentLangMode(KEB_MODE);
	
	return T_OK;
}
/**************************************************************************************************/
/****01234567890123456789012345678901234567890   01234567890123456789012345678901234567890
/* 0.                                           |
/* 1.                                           |
/* 2.      [MyCashZone  거 래 명 세 표]         |         [MyCashZone  Receipt]          
/* 3.                                           |
/* 4.  거래일자 : 20090709-18:26 ,              |  Date/Time: 20090709-18:26 
/* 5.  처리번호 : NH0001                        |  Seque No : NH0001  
/* 6.  거래종류 : 현금지급	                    |  Trans.   : 현금지급
/* 7.  거래기관 : 은행,증권                     |  Service  : 은행,증권
/* 8.  계좌번호 : 1234567890123456              |  CARD No. : 1234567890123456	
/* 9.  거래금액 : 23,000원                      |  Req. ATM : 23,000원
/*10.  고유번호 : 123456789012                  |  Trans.NO : 123456789012	
/*11.                                           |  
/*12.  수수료   : 600원	                        |  Fee      : 600원
/*13.  계좌잔액 : 23,178,000원                  |  Balance  : 23,178,000원                    
/*14.  타점권   : 0원                           |  Oth. AMT : 0원	
/*15.  금융기관 : 국민                          |  Org.Name : 국민
/*15.1 계좌명   : 김기진 <- 입금계좌            |  
/*16.  수취계좌 : 1234567890123456              |  Recv. B,S: 1234567890123456 
/*17.  수취기관 : 제일                          |  Recv.Org : 제일   
/*18.  수취인   : 김기진                        |  Benefic. : 김기진 
/*19.  ErrorCode: 0000                          |  ErrorCode: 0000
/*20.  에러메시지(서버)                         |  에러메시지(서버)  
/*21.                                           |
/*22.  노틸러스효성[080-920-2220]               |  N[080-920-2220]
/*23.
/*24.	
/*25.
/*26.	
/*27.
/*28.	
/*29.
/*30.	
/*31.
/*32.	
/*33.
/*34.	
/*35.
/*37.	
/*38.
/*39.	
/*40.
/*41.	
/*42.
/*43.	
/*44.
/*45.	
/*46.
/*47.	
/*48.
/*49.	
/*50.
/**************************************************************************************************/
// 명세표/저널 편집(Ticket Service Print Form 2010.06.14)
int	CTranCmn::fnAPP_EditPrintLine_Sub_T3_2(int PrintDevice, void* cData, int LineNum, int HiddenFlg)
{
	
	return T_OK;
}
// 명세표/저널 편집(Etc Service Print Form 2009.07.13)
int	CTranCmn::fnAPP_EditPrintLine_Sub_T3_3(int PrintDevice, void* cData, int LineNum, int HiddenFlg)
{

	
	return T_OK;
}
/*********************************************************************************************************************************
 *
 * 연속거래 준비 Proc Kim.Gi.Jin 2010.10.20
 *
 *********************************************************************************************************************************/
int	CTranCmn::fnAPP_TranGetContinueProc(int nTranTmpCode)
{

	if ((m_pDevCmn->EarPhoneInserted)	||						// 장애인ATM
		(m_pDevCmn->SightHandiCapped))
		return T_OK;

	if(m_pDevCmn->fnSCR_GetCurrentLangMode() == JPN_MODE || m_pDevCmn->fnSCR_GetCurrentLangMode() == ENG_MODE || m_pDevCmn->fnSCR_GetCurrentLangMode() == CHN_MODE)
		return T_OK;

	return T_OK;
}

//2011.10.25 Yun.H.J 메인광고/인터렉티브/베너 광고 통합
int	CTranCmn::fnAPP_TranGetContinueProc_Sub(int nTranTmpCode)
{
	CString strReturn("");
	CString strCmd("");
	int		ScrNum = 0;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranGetContinueProc_Sub()");

	ScrNum = 440;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranGetContinueProc_Sub():return");
	return T_OK;
}

int	CTranCmn::fnAPP_POINTS_Sub01_OutputProc()
{
	int			ScrNum = 0;
	CString		strTemp("");
	CString		strReturn("");
	CString			strGetData("");
	CStringArray	strGetDataArray;
		
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_10_WAIT);

	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CONFIRM)
		return T_OK;
	else
	if (strReturn == S_EXIT)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);		

	return T_OK;
}


int	CTranCmn::fnAPP_UTILYBILLS_Sub01_A_OutputProc()
{
	int		ScrNum = 0;
	int     j = 0; 
	int		nAmount = 0;
	int		slen =0;
	CString GetKeyStr;
	CString strT0("");
	CString strT1("");
	CString strTemp("");	
	CString	strGetData("");
	CStringArray strGetBufferArrayTmp;

	strGetBufferArray.RemoveAll();
	strGetBufferArrayTmp.RemoveAll();
	SplitString("0$0$0$0$0$", "$", strGetBufferArrayTmp);
	SplitString("0$0$0$0$0$", "$", strGetBufferArray);


	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_RXJIRODATA.JiroPayData", &m_RXJIRODATA.JiroPayData, sizeof(m_RXJIRODATA.JiroPayData));
	if( m_RXJIRODATA.JiroPayData[0] != 'C')
		fnAPP_CancelProc(T_PROGRAM);
	
	if(m_RXJIRODATA.JiroPayData[1] == '1')
	{
		strTemp.Format("%s", strCT(m_RD.byDataField_021x, 16));
		m_pDevCmn->fnSCR_DisplayString(1, strTemp);	

		strTemp.Format("%4.4s - %2.2s - %2.2s",  &m_RXJIRODATA.JiroPayData[2], &m_RXJIRODATA.JiroPayData[6], &m_RXJIRODATA.JiroPayData[8]);
		m_pDevCmn->fnSCR_DisplayString(2, strTemp);	

		strTemp.Format("%s", strCT(m_RXJIRODATA.JiroBankCode,16));  ////#N0162
		m_pDevCmn->fnSCR_DisplayString(3, strTemp);
		
		strTemp.Format("%s", strCT(m_RXJIRODATA.ElecNum,19));
		m_pDevCmn->fnSCR_DisplayString(4, strTemp);

		strTemp.Format("%12.12s", &m_RXJIRODATA.JiroPayData[11]);
		m_pDevCmn->fnSCR_DisplayAmount(5, strTemp);

		m_pDevCmn->fnSCR_DisplayAmount(6, (LPCTSTR)m_RD.byDataField_024x);  //#N0162

		ScrNum = 773;

		//#N0165
		memset(Accept.Money, ' ', sizeof(Accept.Money));
		nAmount +=	Asc2Int(&m_RXJIRODATA.JiroPayData[11], 12);
		Int2Asc(nAmount, Accept.Money, sizeof(Accept.Money));

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept_money = %15.15s" ,Accept.Money);
	}
	else
	if(m_RXJIRODATA.JiroPayData[1] == '0')
	{
		strTemp.Format("%s", strCT(m_RXJIRODATA.JiroBankCode,16));
		m_pDevCmn->fnSCR_DisplayString(1, strTemp);

		strTemp.Format("%s", strCT(m_RD.byDataField_021x,16));
		m_pDevCmn->fnSCR_DisplayString(2, strTemp);

		slen = 4;
		for(int i = 0; i < Asc2Int(&m_RXJIRODATA.JiroPayData[2], 2) ; i++)
		{
			switch(m_RXJIRODATA.JiroPayData[slen + 17])
			{
			case '0':
				strT0 = "정상";
				break;
			case '1':
				strT0 = "연체";
				break;
			case '2':
				strT0 = "독촉";
				break;
			case '3':
				strT0 = "임의";
				break;
			case '4':
				strT0 = "체납";
				break;				
			}

			strT1.Format("%s",Asc2Amt(&m_RXJIRODATA.JiroPayData[slen + 5], 12,16));
			strT1.TrimLeft();
			strT1.TrimRight();
			
			if(m_RXJIRODATA.JiroPayData[slen + 4] == '0')
				strTemp.Format("%2.2s/%2.2s %4.4s|%s|%s",  &m_RXJIRODATA.JiroPayData[slen],&m_RXJIRODATA.JiroPayData[slen+2], strT0, "납기내" , strT1);		
			else
				strTemp.Format("%2.2s/%2.2s %4.4s|%s|%s",  &m_RXJIRODATA.JiroPayData[slen],&m_RXJIRODATA.JiroPayData[slen+2], strT0, "납기후" , strT1);		


			slen += 21;

			m_pDevCmn->fnSCR_DisplayString(3+i, strTemp);	
		}
		ScrNum = 774;	
	}
	else
		fnAPP_CancelProc(T_PROGRAM);			

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT, PIN_MENU_MODE1);  //#N0173
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	
	if (GetKeyStr == S_CONFIRM)
		return T_OK;
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
	else 
	if (GetKeyStr == S_TIMEOVER)							// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
	{
		memset(Accept.JiroSelItem, ' ', sizeof(Accept.JiroSelItem));
		memcpy(Accept.JiroSelItem, GetKeyStr.GetBuffer(0), GetKeyStr.GetLength());
		
		slen = 4;
		j=0;
		for(int i = 0; i < Asc2Int(&m_RXJIRODATA.JiroPayData[2], 2) ; i++)
		{
			if(Accept.JiroSelItem[i] == '1')
			{
				m_RXJIRODATA.JiroPayData[slen + 20] = 'Y';
				nAmount +=	Asc2Int(&m_RXJIRODATA.JiroPayData[slen + 5], 12);
				strGetBufferArray[j] = strGetBufferArrayTmp[i];
			    j++;
			}
			else
				m_RXJIRODATA.JiroPayData[slen + 20] = ' ';

			slen += 21;
		}

		Int2Asc(nAmount, Accept.Money, sizeof(Accept.Money));
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GIRO_nAmount = %d Accept_money = %15.15s",nAmount ,Accept.Money);

	}

	return T_OK;
}


int	CTranCmn::fnAPP_UTILYBILLS_Sub01_B_OutputProc()
{
	int		ScrNum = 0, StrIndex = 1, j = 0;
	int		slen =0;
	CString GetKeyStr;
	CString strT0("");
	CString strT1("");
	CString strTemp("");	
	CString	strGetData("");
	strGetBufferArray.RemoveAll();
	SplitString("0$0$0$0$0$", "$", strGetBufferArray);	

	if(m_RXJIRODATA.JiroPayData[0] == '1')
	{
		strTemp.Format("%s", strCT(m_RD.byDataField_021x, 16));
		m_pDevCmn->fnSCR_DisplayString(1, strTemp);	

		strTemp.Format("%4.4s - %2.2s - %2.2s",  &m_RXJIRODATA.JiroPayData[1], &m_RXJIRODATA.JiroPayData[5], &m_RXJIRODATA.JiroPayData[7]);
		m_pDevCmn->fnSCR_DisplayString(2, strTemp);	

		strTemp.Format("%s", strCT(m_RXJIRODATA.JiroBankCode,16));
		m_pDevCmn->fnSCR_DisplayString(3, strTemp);
		
		strTemp.Format("%s", strCT(m_RXJIRODATA.ElecNum,19));
		m_pDevCmn->fnSCR_DisplayString(4, strTemp);

		strTemp.Format("%12.12s", &m_RXJIRODATA.JiroPayData[31]);
		m_pDevCmn->fnSCR_DisplayAmount(5, strTemp);
		
		// 처리일자
		strTemp.Format("%4.4s - %2.2s - %2.2s", &m_RXJIRODATA.JiroPayData[49], &m_RXJIRODATA.JiroPayData[53], &m_RXJIRODATA.JiroPayData[55]);	
		m_pDevCmn->fnSCR_DisplayString(6, strTemp);	

		ScrNum = 776;
	}
	else
	if(m_RXJIRODATA.JiroPayData[0]  == '0')
	{

		//성명
		strTemp.Format("%s", strCT(m_RD.byDataField_021x, 16));
		m_pDevCmn->fnSCR_DisplayString(1, strTemp);	
		// 납부 기관명
		strTemp.Format("%s", strCT(m_RXJIRODATA.JiroBankCode,16));
		m_pDevCmn->fnSCR_DisplayString(2, strTemp);
		// 전자납부번호 
		strTemp.Format("%s", strCT(m_RXJIRODATA.ElecNum,19));
		m_pDevCmn->fnSCR_DisplayString(3, strTemp);

		slen = 3;
		for(int i = 0; i < Asc2Int(&m_RXJIRODATA.JiroPayData[1], 2)  ; i++)
		{          
			switch(m_RXJIRODATA.JiroPayData[slen + 4])
			{
			case '0':
				strT0 = "정상";
				break;
			case '1':
				strT0 = "연체";
				break;
			case '2':
				strT0 = "독촉";
				break;
			case '3':
				strT0 = "임의";
				break;
			case '4':
				strT0 = "체납";
				break;				
			}

			strT1.Format("%s",Asc2Amt(&m_RXJIRODATA.JiroPayData[slen + 7], 12,16));
			strT1.TrimLeft();
			strT1.TrimRight();
			
			strTemp.Format("%2.2s/%2.2s(%4.4s) |%s|%4.4s-%2.2s-%2.2s",  &m_RXJIRODATA.JiroPayData[slen],&m_RXJIRODATA.JiroPayData[slen + 2],strT0, strT1,&m_RXJIRODATA.JiroPayData[slen + 26],&m_RXJIRODATA.JiroPayData[slen + 30],&m_RXJIRODATA.JiroPayData[slen + 32]);
			
			strGetBufferArray[i] = strTemp;
			slen += 38;

			m_pDevCmn->fnSCR_DisplayString(4+i, strTemp);	

		}

		ScrNum = 777;
	}
	else
		fnAPP_CancelProc(T_PROGRAM);	

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_5_WAIT, PIN_MENU_MODE1);  //#N0173
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	
	if (GetKeyStr == S_CONFIRM)
		return T_OK;
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							
	else 
	if (GetKeyStr == S_TIMEOVER);						
		return T_OK;

	return T_OK;
}

int	CTranCmn::fnAPP_UTILYBILLS_SR1_Sub01_OutputProc()	
{
	int		ScrNum = 0, StrIndex = 1;
	int		slen =0;
	CString GetKeyStr;
	CString strT0("");
	CString strT1("");
	CString strT2("");
	CString strTemp("");
	int j=0;
//	ScrNum = 775;
	ScrNum = 779;

	//#명세표 데이터 편집
	slen = 4;
	for(int i = 0; i < Asc2Int(&m_RXJIRODATA.JiroPayData[2], 2) ; i++)
	{
		if(m_RXJIRODATA.JiroPayData[slen + 20] == 'Y')
		{
			switch(m_RXJIRODATA.JiroPayData[slen + 17])
			{
			case '0':
				strT0 = "정상";
				break;
			case '1':
				strT0 = "연체";
				break;
			case '2':
				strT0 = "독촉";
				break;
			case '3':
				strT0 = "임의";
				break;
			case '4':
				strT0 = "체납";
				break;				
			}

			strT1.Format("%s",Asc2Amt(&m_RXJIRODATA.JiroPayData[slen + 5] , 12,16));
			strT1.TrimLeft();
			strT1.TrimRight();
			
			 //#SJ 1115 명세표 수정

			if(strT1.GetLength() < 11)
			{
				while(strT1.GetLength() < 11)
				{
					strT1.Insert(0, ' ');
				}
			}		

			strTemp.Format("%2.2s/%2.2s %4.4s %s원",  &m_RXJIRODATA.JiroPayData[slen], &m_RXJIRODATA.JiroPayData[slen + 2], strT0 , strT1);		
			strGetBufferArray[j] = strTemp;

			j++;
		}
		slen += 21;
	}

	Int2Asc(j, Accept.JiroBillCount, sizeof(Accept.JiroBillCount));
	
		
	//성명
	strTemp.Format("%s", strCT(m_RD.byDataField_021x, 16));
	m_pDevCmn->fnSCR_DisplayString(1, strTemp);	
	//납부기관
	strTemp.Format("%s", strCT(m_RXJIRODATA.JiroBankCode , 16));
	m_pDevCmn->fnSCR_DisplayString(2, strTemp);	
	//전자납부번호
	strTemp.Format("%s", strCT(m_RXJIRODATA.ElecNum , 19));
	m_pDevCmn->fnSCR_DisplayString(3, strTemp);
	//납부금액
	strTemp.Format("%s", strCT(Accept.Money, sizeof(Accept.Money) ));
	m_pDevCmn->fnSCR_DisplayAmount(4, strTemp);
	//수수료
	m_pDevCmn->fnSCR_DisplayAmount(5, (LPCTSTR)m_RD.byDataField_024x); //수수료

	slen = Asc2Int(&m_RXJIRODATA.JiroPayData[2], 2) * 21 + 2;

	/*
	if(m_HNetRD.HNet_Dummy1[slen] == 'Y')
	{
		strTemp.Format("%2.2s/%2.2s - %2.2s/%2.2s (%d개월)", &m_HNetRD.HNet_Dummy1[slen+1],&m_HNetRD.HNet_Dummy1[slen+3],&m_HNetRD.HNet_Dummy1[slen+5],&m_HNetRD.HNet_Dummy1[slen+7], Asc2Int(&m_HNetRD.HNet_Dummy1[slen+9], 3) );
		m_pDevCmn->fnSCR_DisplayString(3, strTemp);
	}
	*/

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT, PIN_MENU_MODE1);   //#N0173
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	
	if (GetKeyStr == S_CONFIRM)
	{
		return T_OK;
	}
	else
	if (GetKeyStr == S_YES || GetKeyStr == S_TIMEOVER)
		return T_OK;
	else
	if (GetKeyStr == S_NO)
		fnAPP_CancelProc(T_CANCEL);	
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							

	return T_OK;
}



int	CTranCmn::fnAPP_LOCALTAX_Sub01_OutputProc()
{
	int		ScrNum = 0;
	int     j = 0; 
	int		nAmount = 0;
	int		slen =0;
	CString GetKeyStr;
	CString strT0("");
	CString strT1("");
	CString strTemp("");	
	CString	strGetData("");
	
	strTemp.Format("%s", strCT(m_RD.byDataField_017x,19));  //카드번호
	m_pDevCmn->fnSCR_DisplayString(1, strTemp);

	strTemp.Format("%s", strCT(m_RD.byDataField_021x, 16));  //납부자성명
	m_pDevCmn->fnSCR_DisplayString(2, strTemp);

	strTemp.Format("%d", Asc2Int(&m_RXLOCALTAXDATA.JiroPayData[1], 4));  //총고지건수
	m_pDevCmn->fnSCR_DisplayString(3, strTemp);

	strTemp.Format("%s",  strCT(&m_RXLOCALTAXDATA.JiroPayData[5], 12));  // 총고지금액
	m_pDevCmn->fnSCR_DisplayAmount(4, strTemp);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_RXLOCALTAXDATA.JiroPayData=%s", m_RXLOCALTAXDATA.JiroPayData);
	HexDump(TRACE_DUMP_MODE, "log", __FILE__, __LINE__, "m_RXLOCALTAXDATA.JiroPayData", m_RXLOCALTAXDATA.JiroPayData, sizeof(m_RXLOCALTAXDATA.JiroPayData));

	slen = 19;
	for(int i = 0; i < Asc2Int(&m_RXLOCALTAXDATA.JiroPayData[17], 2) ; i++)
	{
		strT1.Format("%s",Asc2Amt(&m_RXLOCALTAXDATA.JiroPayData[slen + 20] , 12,16));
		strT1.TrimLeft();
		strT1.TrimRight();
		
		strTemp.Format("%s|%s|%s|%2.2s-%2.2s-%2.2s",  strCT(&m_RXLOCALTAXDATA.JiroPayData[slen], 10), strCT(&m_RXLOCALTAXDATA.JiroPayData[slen + 10], 10), strT1, &m_RXLOCALTAXDATA.JiroPayData[slen + 32], &m_RXLOCALTAXDATA.JiroPayData[slen + 34], &m_RXLOCALTAXDATA.JiroPayData[slen + 36]);		

		slen += 69;

		m_pDevCmn->fnSCR_DisplayString(5+i, strTemp);	
	}

	ScrNum = 3012;


	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT, PIN_MENU_MODE1);  //#N0173
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	

	if (GetKeyStr == "1건조회")
	{
		fnAPP_LOCALTAX_AcceptElecNum();
		Accept.JiroTranCode = TC_ELECINQ;
	}
	else
	if (GetKeyStr == S_CONFIRM)
		return T_OK;
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
	else 
	if (GetKeyStr == S_TIMEOVER)							// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
	{
		memset(Accept.JiroSelItem, ' ', sizeof(Accept.JiroSelItem));
		memcpy(Accept.JiroSelItem, GetKeyStr.GetBuffer(0), GetKeyStr.GetLength());
		
		slen = 19;
		for(int i = 0; i < Asc2Int(&m_RXLOCALTAXDATA.JiroPayData[17], 2) ; i++)
		{
			if(Accept.JiroSelItem[i] == '1')
			{
				m_RXLOCALTAXDATA.JiroPayData[slen + 68] = 'Y';
				nAmount +=	Asc2Int(&m_RXLOCALTAXDATA.JiroPayData[slen + 20], 12);
				j++;
			}
			else
				m_RXLOCALTAXDATA.JiroPayData[slen + 68] = ' ';

			slen += 69;
		}
		Int2Asc(j, Accept.JiroBillCount, sizeof(Accept.JiroBillCount)); 
		Int2Asc(nAmount, Accept.Money, sizeof(Accept.Money));

	}

	return T_OK;
}


int	CTranCmn::fnAPP_LOCALTAX_Sub02_OutputProc()
{
	int		ScrNum = 0;
	int     j = 0; 

	int		slen =0;
	int		nTotalAmount = 0;
	CString GetKeyStr;
	CString strT0("");
	CString strT1("");
	CString strTemp("");	
	CString	strGetData("");
	
	strTemp.Format("%s", strCT(m_RD.byDataField_017x,19));  //카드번호
	m_pDevCmn->fnSCR_DisplayString(1, strTemp);

	strTemp.Format("%s", strCT(m_RD.byDataField_021x, 16));  //납부자성명
	m_pDevCmn->fnSCR_DisplayString(2, strTemp);

	m_pDevCmn->fnSCR_DisplayAmount(3, (LPCTSTR)m_RD.byDataField_024x); //수수료

	if(memcmp(m_RXLOCALTAXDATA.Gubun, "03", 2) == 0)
	{
		slen =19;
		strTemp.Format("%s", strCT(&m_RXLOCALTAXDATA.JiroPayData[slen + 20], 12));  // 총고지금액
		m_pDevCmn->fnSCR_DisplayAmount(4, strTemp);
		m_pDevCmn->fnSCR_DisplayAmount(9, strTemp);

		strT1.Format("%s",Asc2Amt(&m_RXLOCALTAXDATA.JiroPayData[slen + 20] , 12,16));
		strT1.TrimLeft();
		strT1.TrimRight();
		
		memset(Accept.Money, '0' , sizeof(Accept.Money));
		memcpy(&Accept.Money[3], &m_RXLOCALTAXDATA.JiroPayData[slen + 20], 12);
		strTemp.Format("%s|%s|%s|  %2.2s-%2.2s-%2.2s",  strCT(&m_RXLOCALTAXDATA.JiroPayData[slen], 10), strCT(&m_RXLOCALTAXDATA.JiroPayData[slen + 10], 10), strT1,&m_RXLOCALTAXDATA.JiroPayData[slen + 32], &m_RXLOCALTAXDATA.JiroPayData[slen + 34], &m_RXLOCALTAXDATA.JiroPayData[slen + 36]);		

		m_pDevCmn->fnSCR_DisplayString(5+j, strTemp);
		j += 1;

		m_RXLOCALTAXDATA.JiroPayData[slen + 68] = 'Y';
	}
	else
	{
		slen = 19;
		for(int i = 0; i < Asc2Int(&m_RXLOCALTAXDATA.JiroPayData[17], 2) ; i++)
		{
			if(m_RXLOCALTAXDATA.JiroPayData[slen + 68] =='Y')
			{
				
				nTotalAmount += Asc2Int(&m_RXLOCALTAXDATA.JiroPayData[slen + 20], 12); 
				strT1.Format("%s",Asc2Amt(&m_RXLOCALTAXDATA.JiroPayData[slen + 20] , 12,16));
				strT1.TrimLeft();
				strT1.TrimRight();
				
				strTemp.Format("%s|%s|%s|%2.2s-%2.2s-%2.2s",  strCT(&m_RXLOCALTAXDATA.JiroPayData[slen], 10), strCT(&m_RXLOCALTAXDATA.JiroPayData[slen + 10], 10), strT1, &m_RXLOCALTAXDATA.JiroPayData[slen + 32], &m_RXLOCALTAXDATA.JiroPayData[slen + 34], &m_RXLOCALTAXDATA.JiroPayData[slen + 36]);		

				m_pDevCmn->fnSCR_DisplayString(5+j, strTemp);
				j += 1;
			}

			slen += 69;
		}
		strTemp.Format("%d", nTotalAmount);  // 총고지금액
		m_pDevCmn->fnSCR_DisplayAmount(4, strTemp);
		m_pDevCmn->fnSCR_DisplayAmount(9, strTemp);
	}
	Int2Asc(j, Accept.JiroBillCount, sizeof(Accept.JiroBillCount));  

	ScrNum = 3018;


	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT, PIN_MENU_MODE1);   //#N0173
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	

	if (GetKeyStr == S_CONFIRM)
		return T_OK;
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
	else 
	if (GetKeyStr == S_TIMEOVER)							// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}



int	CTranCmn::fnAPP_LOCALTAX_Sub03_OutputProc()
{
	int		ScrNum = 0;
	int     j = 0; 

	int		slen =0;
	int		nTotalAmount = 0;
	CString GetKeyStr;
	CString strT0("");
	CString strT1("");
	CString strTemp("");	
	CString	strGetData("");

	strGetBufferArray.RemoveAll();
	SplitString("0$0$0$0$0$0$0$0$0$0$0$0$0$0$0$", "$", strGetBufferArray);
	
	strTemp.Format("%s", strCT(m_RD.byDataField_017x,19));  //카드번호
	m_pDevCmn->fnSCR_DisplayString(1, strTemp);

	strTemp.Format("%s", strCT(m_RD.byDataField_021x, 16));  //납부자성명
	m_pDevCmn->fnSCR_DisplayString(2, strTemp);

/*	if(Accept.JiroTranCode == TC_ELECINQ)
	{
		strTemp.Format("%s", strCT(m_RXLOCALTAXDATA.TotalAmount, 12));  // 총고지금액
		m_pDevCmn->fnSCR_DisplayAmount(3, strTemp);
		m_pDevCmn->fnSCR_DisplayAmount(9, strTemp);

		strT1.Format("%s",Asc2Amt(&m_HNetRD.HNet_Dummy1[slen + 20] , 12,16));
		strT1.TrimLeft();
		strT1.TrimRight();
		
		strTemp.Format("%s|%s|%s|%2.2s-%2.2s-%2.2s",  &m_HNetRD.HNet_Dummy1[slen], &m_HNetRD.HNet_Dummy1[slen + 10], &m_HNetRD.HNet_Dummy1[slen + 32], &m_HNetRD.HNet_Dummy1[slen + 34], &m_HNetRD.HNet_Dummy1[slen + 36]);		

		m_pDevCmn->fnSCR_DisplayString(4+j, strTemp);
		j += 1;

		m_HNetRD.HNet_Dummy1[slen + 68] = 'Y';
	}
	else*/
	if(TranCode2 == TRANID_7326 || TranCode2 == TRANID_7327 || TranCode2 == TRANID_7329)
	{
		slen = 19;
		for(int i = 0; i < Asc2Int(&m_RXLOCALTAXDATA.JiroPayData[17], 2) ; i++)
		{
				
			nTotalAmount += Asc2Int(&m_RXLOCALTAXDATA.JiroPayData[slen + 20], 12);

			strT1.Format("%s",Asc2Amt(&m_RXLOCALTAXDATA.JiroPayData[slen + 20] , 12,16));
			strT1.TrimLeft();
			strT1.TrimRight();
			
			strTemp.Format("%s|%s|%s|%2.2s-%2.2s-%2.2s",  strCT(&m_RXLOCALTAXDATA.JiroPayData[slen], 10), strCT(&m_RXLOCALTAXDATA.JiroPayData[slen + 10], 10), strT1, &m_RXLOCALTAXDATA.JiroPayData[slen + 32], &m_RXLOCALTAXDATA.JiroPayData[slen + 34], &m_RXLOCALTAXDATA.JiroPayData[slen + 36]);		

			m_pDevCmn->fnSCR_DisplayString(4+j, strTemp);
			j += 1;

			slen += 69;
		}
		strTemp.Format("%d", nTotalAmount);  // 총고지금액
		m_pDevCmn->fnSCR_DisplayAmount(3, strTemp);
		m_pDevCmn->fnSCR_DisplayAmount(9, strTemp);
		m_pDevCmn->fnSCR_DisplayAmount(10, (LPCTSTR)m_RD.byDataField_024x); //수수료
	}
	else
	{
		slen = 19;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_RXLOCALTAXDATA.JiroPayData = %s",&m_RXLOCALTAXDATA.JiroPayData[0]);
		for(int i = 0; i < Asc2Int(&m_RXLOCALTAXDATA.JiroPayData[17], 2) ; i++)
		{
			if(m_RXLOCALTAXDATA.JiroPayData[slen + 68] =='Y')
			{
				CString strTmp1("");
				CString strTmp2("");
				nTotalAmount += Asc2Int(&m_RXLOCALTAXDATA.JiroPayData[slen + 20], 12);

				strT1.Format("%s",Asc2Amt(&m_RXLOCALTAXDATA.JiroPayData[slen + 20] , 12,16));
				strT1.TrimLeft();
				strT1.TrimRight();
				
				strTemp.Format("%s|%s|%s|%2.2s-%2.2s-%2.2s",  strCT(&m_RXLOCALTAXDATA.JiroPayData[slen], 10), strCT(&m_RXLOCALTAXDATA.JiroPayData[slen + 10], 10), strT1, &m_RXLOCALTAXDATA.JiroPayData[slen + 32], &m_RXLOCALTAXDATA.JiroPayData[slen + 34], &m_RXLOCALTAXDATA.JiroPayData[slen + 36]);		
				m_pDevCmn->fnSCR_DisplayString(4+j, strTemp);
				
				strTmp1.Format("%s", strCT(&m_RXLOCALTAXDATA.JiroPayData[slen], 10));
				strTmp1.TrimRight();
				strTmp2.Format("%s", strCT(&m_RXLOCALTAXDATA.JiroPayData[slen + 10], 10));
				strTmp2.TrimRight();
				strTemp.Format("%s %s", strTmp1, strTmp2);
				strGetBufferArray[j*2] = strTemp;    //#SJ 1115 명세표 데이터 오류
				strGetBufferArray[j*2 + 1] = strT1;

				j += 1;  //#SJ 1115 위치변경
			}

			slen += 69;
		}
		strTemp.Format("%d", nTotalAmount);  // 총고지금액
		m_pDevCmn->fnSCR_DisplayAmount(3, strTemp);
		m_pDevCmn->fnSCR_DisplayAmount(9, strTemp);
		m_pDevCmn->fnSCR_DisplayAmount(10, (LPCTSTR)m_RD.byDataField_024x); //수수료
	}
	Int2Asc(j, Accept.JiroBillCount, sizeof(Accept.JiroBillCount));  

	ScrNum = 3016;

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT, PIN_MENU_MODE1);  //#N0173
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	
	if (GetKeyStr == S_CONFIRM)
		return T_OK;
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
	else 
	if (GetKeyStr == S_TIMEOVER)							// TIMEOVER
		return T_OK;										//#SJ 1115 타임아웃시 명세표 미방출 수정
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}


int	CTranCmn::fnAPP_E_TICKET_Sub01_OutputProc()
{
	int		ScrNum = 0;
	int     j = 0; 

	int		slen =0;
	int		nTotalAmount = 0;
	CString GetKeyStr;
	CString strT0("");
	CString strT1("");
	CString strTemp("");	
	CString	strGetData("");
	
	ScrNum = 3515;

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
	Delay_Msg(3000);

	return T_OK;
}

int	CTranCmn::fnAPP_E_TICKET_Sub02_OutputProc()
{
	int		ScrNum = 0;
	int     j = 0; 

	int		slen =0;
	int		nTotalAmount = 0;
	CString GetKeyStr;
	CString strT0("");
	CString strT1("");
	CString strTemp("");	
	CString	strGetData("");
	
	ScrNum = 3516;

	m_pDevCmn->fnSCR_DisplayAmount(1, Accept.ETicketChange);
	
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
	Delay_Msg(3000);

	return T_OK;
}

int	CTranCmn::fnAPP_E_TICKET_Sub03_OutputProc()
{
	int		ScrNum = 0;
	int     j = 0; 

	int		slen =0;
	int		nTotalAmount = 0;
	CString GetKeyStr;
	CString strT0("");
	CString strT1("");
	CString strTemp("");	
	CString	strGetData("");
	
	ScrNum = 3517;

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();

	if (GetKeyStr == S_CONFIRM)
		return T_OK;
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
	else 
	if (GetKeyStr == S_TIMEOVER)							// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

int	CTranCmn::fnAPP_E_TICKET_Sub04_OutputProc()
{
	int		ScrNum = 0;
	int     j = 0; 

	int		slen =0;
	int		nTotalAmount = 0;
	CString GetKeyStr;
	CString strT0("");
	CString strT1("");
	CString strTemp("");	
	CString	strGetData("");
	
	ScrNum = 3518;

	m_pDevCmn->fnSCR_DisplayString(1, Accept.ETicketSerialNum);
	m_pDevCmn->fnSCR_DisplayString(2, m_RD.byDataField_017x);
	m_pDevCmn->fnSCR_DisplayAmount(3, m_RD.byDataField_023x);
	m_pDevCmn->fnSCR_DisplayString(4, "현금IC카드 결제");

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum);

	Delay_Msg(3000);

	return T_OK;
}


//#N0174 외환 이지원
int	CTranCmn::fnAPP_EASYONE_Sub01_OutputProc()
{
	int		ScrNum = 0;
	int     j = 0; 
	int		nAccIndex = 0;
	int		slen =0;
	CString GetKeyStr;
	CString strT0("");
	CString strT1("");
	CString strTemp("");	
	CString	strGetData("");

	
	strTemp.Format("%15.15s  %25.25s", &m_RXEASYONEDATA.EasyOne_Acc[0][0], &m_RXEASYONEDATA.EasyOne_Acc[0][15]);  //이지원 계좌 1
	m_pDevCmn->fnSCR_DisplayString(1, strTemp);

	strTemp.Format("%35.35s",&m_RXEASYONEDATA.EasyOne_Acc[0][40]);  //이지원 계좌 1
	m_pDevCmn->fnSCR_DisplayString(2, strTemp);

	strTemp.Format("%15.15s  %25.25s", &m_RXEASYONEDATA.EasyOne_Acc[1][0], &m_RXEASYONEDATA.EasyOne_Acc[1][15]);  //이지원 계좌 1
	m_pDevCmn->fnSCR_DisplayString(3, strTemp);

	strTemp.Format("%35.35s", &m_RXEASYONEDATA.EasyOne_Acc[1][40]);  //이지원 계좌 1
	m_pDevCmn->fnSCR_DisplayString(4, strTemp);

	strTemp.Format("%15.15s  %25.25s", &m_RXEASYONEDATA.EasyOne_Acc[2][0], &m_RXEASYONEDATA.EasyOne_Acc[2][15]);  //이지원 계좌 1
	m_pDevCmn->fnSCR_DisplayString(5, strTemp);

	strTemp.Format("%35.35s",  &m_RXEASYONEDATA.EasyOne_Acc[2][40]);  //이지원 계좌 1
	m_pDevCmn->fnSCR_DisplayString(6, strTemp);

	strTemp.Format("%15.15s  %25.25s", &m_RXEASYONEDATA.EasyOne_Acc[3][0], &m_RXEASYONEDATA.EasyOne_Acc[3][15]);  //이지원 계좌 1
	m_pDevCmn->fnSCR_DisplayString(7, strTemp);

	strTemp.Format("%35.35s", &m_RXEASYONEDATA.EasyOne_Acc[3][40]);  //이지원 계좌 1
	m_pDevCmn->fnSCR_DisplayString(8, strTemp);

	strTemp.Format("%15.15s  %25.25s", &m_RXEASYONEDATA.EasyOne_Acc[4][0], &m_RXEASYONEDATA.EasyOne_Acc[4][15]);  //이지원 계좌 1
	m_pDevCmn->fnSCR_DisplayString(9, strTemp);

	strTemp.Format("%35.35s",  &m_RXEASYONEDATA.EasyOne_Acc[4][40]);  //이지원 계좌 1
	m_pDevCmn->fnSCR_DisplayString(10, strTemp);

	ScrNum = 3400;

	if(m_RXEASYONEDATA.NextFlag[0] != 'Y')
		m_pDevCmn->fnSCR_DisplayImage(1, FALSE);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	

	if (GetKeyStr == S_NEXT)
	{
		Accept.EasyOneNextInqFlg =TRUE;
		return T_OK;
	}
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
	else
	if (GetKeyStr == S_CANCEL2)
		fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
	else 
	if (GetKeyStr == S_TIMEOVER)							// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
	{
		nAccIndex = Asc2Int(GetKeyStr, GetKeyStr.GetLength());
		if(nAccIndex > 0)
		{
			strT0.Empty();
			strT0.Format("%15.15s", &m_RXEASYONEDATA.EasyOne_Acc[nAccIndex - 1][0]);
			strT0.Remove('-');
		}		
		memcpy(Accept.InAccountNum, strT0, strT0.GetLength()); 
		Accept.InAccountNumSize = strT0.GetLength();
		memcpy(Accept.EasyOneName, &m_RXEASYONEDATA.EasyOne_Acc[nAccIndex - 1][15], 25);
		memcpy(Accept.EasyOneOrgName, &m_RXEASYONEDATA.EasyOne_Acc[nAccIndex - 1][40], 35);
		Accept.EasyOneNextInqFlg = FALSE;
	}
	
	return T_OK;
}


int	CTranCmn::fnAPP_EASYONE_Sub02_OutputProc()
{
	int		ScrNum = 0;
	int     j = 0; 
	int		nAccIndex = 0;
	int		slen =0;
	CString GetKeyStr;
	CString strT0("");
	CString strT1("");
	CString strTemp("");	
	CString	strGetData("");

	strTemp.Format("%16.16s", m_RD.byDataField_017x);
	m_pDevCmn->fnSCR_DisplayString(1, strTemp);

	strTemp.Empty();
	strTemp.Format("%16.16s", Accept.InAccountNum);
	m_pDevCmn->fnSCR_DisplayString(2, strTemp);

	m_pDevCmn->fnSCR_DisplayAmount(3, (LPCTSTR)Accept.Money, 15);

	ScrNum = 3401;


	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	

	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
	else
	if (GetKeyStr == S_CANCEL2)
		fnAPP_CancelProc(T_CANCEL);							// 거래취소처리
	else 
	if (GetKeyStr == S_TIMEOVER)							// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else 
	if (GetKeyStr == S_CONFIRM)							// TIMEOVER
		return T_OK;
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}



int CTranCmn::fnAPP_TestPPPProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TestPPPProc()");

	int		i, j, k;
	int		startline, endline, szPbLen;
	static	int		LocalLine = 0;
	int				LocalPage = 0;
	int				LocalI = 0;

//////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_PPPProc : LocalPage[%d], PPPRecvData.Ep[%d]", LocalPage, PPPRecvData.Ep);

	PPPRecvData.ParaLength = 1;
	PPPRecvData.Ep = 0;
	LocalI = 0;
	LocalLine =	PPPRecvData.y - 1;
	for(i = 0; i <= LocalI; i++) 
	{
		CString strTmpData("");
		if(m_pDevCmn->AtmDefine.PBHandler == PBM_SET)
			strTmpData.Format("%8.8s %6.6s %2.2s%4.4s L-PBM PRINT TEST  *123,456   *123,456,789", GetDate().GetBuffer(0), GetTime().GetBuffer(0), &m_pProfile->NETWORK.BranchNum[6], &m_pProfile->NETWORK.AtmSerialNum[4] );
		else
		if(m_pDevCmn->AtmDefine.PBHandler == I_PBM_SET)
			strTmpData.Format("%8.8s %6.6s %2.2s%4.4s I-PBM PRINT TEST  *123,456   *123,456,789", GetDate().GetBuffer(0), GetTime().GetBuffer(0), &m_pProfile->NETWORK.BranchNum[6], &m_pProfile->NETWORK.AtmSerialNum[4] );
		else
			strTmpData.Format("%8.8s %6.6s %2.2s%4.4s   PBM PRINT TEST  *123,456   *123,456,789", GetDate().GetBuffer(0), GetTime().GetBuffer(0), &m_pProfile->NETWORK.BranchNum[6], &m_pProfile->NETWORK.AtmSerialNum[4] );
		Strcpy(PPPRecvData.Data[LocalPage][LocalLine], 
//		 12345678901234567890123456789012345678901234567890123456789012345678901234567890
		strTmpData);
		PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);

		//LocalLine++;
		if (LocalLine >= itemof(PPPRecvData.DataLength[0]))
		{
			LocalLine = 0;
			if (i != LocalI)
			{
				if ((m_pDevCmn->AtmDefine.PBHandler == PBM_SET)		||
					(m_pDevCmn->AtmDefine.PBHandler == I_PBM_SET)	)
				{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_PPPProc 2 : LocalPage[%d], PPPRecvData.Ep[%d]", LocalPage, PPPRecvData.Ep);
					LocalPage++;
					PPPRecvData.Ep = LocalPage;
				}
				else break;
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PPPRecvData.ParaLength = %d",PPPRecvData.ParaLength);

	if (!PPPRecvData.ParaLength)								// DATA부/통장부프린트(거래)
		return T_OK;											// 해당자료무

	PbDataCnt = 0;												// Pb출력길이
	memset(PbData, 0, sizeof(PbData));							// Pb출력영역

	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));

	PbDataCnt += sizeof(PbFrameSet);

	for(i = 0; i <= 23; i++) 
	{
		if (!PPPRecvData.DataLength[0][i])
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (i == 11)											// 재봉선 skip
		{
			PbData[PbDataCnt++] = PbNewLine;
			PbData[PbDataCnt++] = PbNewLine;
			PbData[PbDataCnt++] = PbNewLine;
		}
	}
	startline = i;

	for (i = 0; i <= PPPRecvData.Ep; i++) 
	{
		endline = 0;
		for (k = startline; k <= 23; k++)
		{
			if (!PPPRecvData.DataLength[i][k])
			{
				endline = k;
				break;
			}
			else endline = k;
		}

		for (j = startline; j <= endline; j++) 
		{
			szPbLen = PPPRecvData.DataLength[i][j];
			// 전문이 있는 경우 길이가 리턴된다.
			if (szPbLen)
			{
				memcpy(&PbData[PbDataCnt], PPPRecvData.Data[i][j], szPbLen);
				PbDataCnt += szPbLen;
				PbData[PbDataCnt++] = PbNewLine;
				// 1줄 인쇄후 13행으로 건너갈 경우 두라인을 피드한다.
				if (j == 11) 
				{
					PbData[PbDataCnt++] = PbNewLine;
					PbData[PbDataCnt++] = PbNewLine;
				}
				// 페이지 마지막을 인쇄한 경우 다음 페이지로 넘어간다.
				else if (j == 23)
				{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_PPPProc : LocalPage[%d], PPPRecvData.Ep[%d], i[%d]", LocalPage, PPPRecvData.Ep, i );
					if (i != PPPRecvData.Ep)					// 2002.02.25
					{
						memcpy(&PbData[PbDataCnt], PbFrameNextSet, sizeof(PbFrameNextSet));
						PbDataCnt += sizeof(PbFrameNextSet);
					}
////////
// j = 23 경우 통장인자 후 방출 
// 수취대기 
// 수취완료후 다시 통장 엔트리 (통장 삽입대기 시간 타사 확인, 일정시간대기일경우에 남은 데이타 처리)
// 통장 들어오면 
// 계속처리(투입된 통장이 직전 통장과 같은지 확인)
// 상기 플로우 루프


				}
				// 인쇄하고자 하는 데이터가 1k block을 넘지않도록 한다.
				if (PbDataCnt > (sizeof(PbData) / 2))			// 512/1024초과시(50%)
				{
					// pbm으로 데이터를 Passing한다.
					fnAPD_PbPrint(TRUE);
					memset(PbData, 0, sizeof(PbData));
					PbDataCnt = 0;
				}
			}
			else break;
		}

		startline = 0;

		if (PbDataCnt)											// 미인자자료존재시
		{
			if (!m_pDevCmn->fnAPL_CheckDeviceAction(DEV_PBM))	// 동작완료검지&장애검지
				return T_ERROR;
			m_pDevCmn->fnPBM_Print(MakeUnPack(PbData, PbDataCnt));	// 인자(인자자료)
			if (!m_pDevCmn->fnAPL_CheckDeviceAction(DEV_PBM))	// 동작완료검지&장애검지
				return T_ERROR;

			memset(PbData, 0, sizeof(PbData));
			PbDataCnt = 0;
		}	
		
		memcpy(PbData, PbFrameSet, sizeof(PbFrameSet));
		PbDataCnt = sizeof(PbFrameSet);
	}

	return T_OK;
}