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
//	������ó���Լ�
/////////////////////////////////////////////////////////////////////////////
// ������Ʈ
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
		if (!Accept.SlipChoiceFlag)								// ��ǥ����
			PrintDevice &= ~DEV_SPR;

		if (PrintDevice == DEV_NONE)
			return T_OK;
	}

	memset(ShuData, 0, sizeof(ShuData));						// Shu��¿���

	if (PrintDevice & DEV_JPR) 
	{
		HiddenFlg = FALSE;										// ���θ����ڽ� ��ɻ���
	}

	for (ii = 0; ii < 35; ii++)
	{
		fnAPP_EditPrintLine(PrintDevice, ShuData[nSlipIndex], ii, HiddenFlg);		// ���� Edit

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
		sprintf(ShuData[nSlipIndex++], "[���� �ŷ� ���]");
	}

	if (PSPRecvData.ParaLength)							// ��ǥ�ڷ���
	{
		for (i = PSPRecvData.By; i <= PSPRecvData.Ey; i++)
		{
			memcpy(&ShuData[nSlipIndex++], PSPRecvData.Data[i], itemof(PSPRecvData.Data[i]));
		}
	}

	for (ii = 51; ii < 100; ii++)
	{
		fnAPP_EditPrintLine(PrintDevice, ShuData[nSlipIndex], ii, HiddenFlg);		// ���� Edit
		if (ShuData[nSlipIndex][0] != 0)
			nSlipIndex++;
	}

	if ((!m_pDevCmn->TranResult)	&&
		(TranProc == TRAN_DEP)		&&
		(CashData.mTotalAmount)		)
	{
		sprintf(ShuData[nSlipIndex++], "[�Աݺ� ��ȯó��]");
	}



	for (i = 0; i < nSlipIndex; i++)
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_PSPProc[%s]", &ShuData[i]);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_PSPProc() end");
	fnAPD_ShuPrint(PrintDevice, JNL_TRAN, WaitFlg);						// ��ǥ����

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
// ���������Ʈ
int CTranCmn::fnAPP_NONGHYUP_PPPProc(int WaitFlg)
{
	int		i,j,k;
	int		startline, endline, szPbLen;
	int		nEndPage = FALSE;

// ================= Local Test ==================================	// V06-00-04-#01, ������ ��� ���� 1Line �ﵵ�� ����
	static   int    LocalLine = 0;				                    // V06-00-05-#01, ���� ���������� ��� 2Page ��� Turn Page �ϵ��� ����.
	int             LocalPage = 0;
	int             LocalI = 0;
	int				TurnCheck = FALSE;

	if ((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&
	(PbTran))
	{
		PPPRecvDataNon.p = 0;
		memset(PPPRecvDataNon.Data, 0x00, sizeof(PPPRecvData.Data));	

		if (TranCode == TC_PBOOK)								// ��������
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
				"         ������    ���������     1234-56-78910  ��05.000%");
			}
			else
			{
				if ((TranCode != TC_PBOOK) && (LocalLine >= 24))
				{
					LocalLine = 0;
					TurnCheck = TRUE;
					Strcpy(PPPRecvDataNon.Data[LocalPage][LocalLine], 
					"20020202 ATM IN/OUT   *123,456       *123,456 *123,456,789 ����");
				}
				else
				{
					Strcpy(PPPRecvDataNon.Data[LocalPage][LocalLine], 
					"20020202 ATM IN/OUT   *123,456       *123,456 *123,456,789 ����");
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
					"20020202 ATM IN/OUT   *123,456       *123,456 *123,456,789 ����");
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

	memset(PbData, 0, sizeof(PbData));							// Pb��¿���
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
	// ������ ã�� - �����Ͱ� NULL�̸� �����ǵ带 �Ѵ�.
	for(i=0; i < startline; i++) 
	{
		if (!strlen((char *)PPPRecvDataNon.Data[0][i]))
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (i == 11)	// ����� skip
		{
			PbData[PbDataCnt++] = PbNewLine;
			PbData[PbDataCnt++] = PbNewLine;
		}
	}
	
	startline = i;


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "������������[%d] ���ڽ��۶���[%d]",PPPRecvData.p, startline);
//MsgDumpAnal(TRACE_CODE_MODE, "Log", "������������[%d]", PPPRecvData.p);

	// ������ ���� ���� 
	// PPPRecvData.p�� ������ �����Ͱ� �ִ� �������� ��ȣ�̴�.
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

			// ������ �ִ� ��� ���̰� ���ϵȴ�.
			if (szPbLen)
			{
				memcpy(&PbData[PbDataCnt], PPPRecvDataNon.Data[i][j], szPbLen);

				PbDataCnt += szPbLen;
				PbData[PbDataCnt++] = 0x0a;
				// 1�� �μ��� 13������ �ǳʰ� ��� �ζ����� �ǵ��Ѵ�.
				if (j == 11) 
				{
					PbData[PbDataCnt++] = 0x0a;
					PbData[PbDataCnt++] = 0x0a;
				}
				// ������ �������� �μ��� ��� ���� �������� �Ѿ��.
				else 
				if (j == 23)
				{
					// ������ �������� �ƴѰ�� : 2002.02.25(�ӿ��������ٶ�)
					if (i != PPPRecvDataNon.p)
					{
						memcpy(&PbData[PbDataCnt], PbFrameNextSet, sizeof(PbFrameNextSet));
						PbDataCnt += 5;
					}
					break;
				}

				// �μ��ϰ��� �ϴ� �����Ͱ� 1k block�� �����ʵ��� �Ѵ�.
//				2008.04.30 �������ڼӵ� ���� : 10K������ �ѹ濡 �������� �Ѵ�.
//				if (PbDataCnt > (PB_MAX_FRAME / 2))		// 512/1024�ʰ���(50%)
				if (PbDataCnt > (sizeof(PbData) -100))			// 10K(100%) 2004.07.29
				{
					// pbm���� �����͸� Passing�Ѵ�.
					fnAPD_PbPrint(TRUE);
					fnAPD_CheckDeviceAction(DEV_PBM);					// ���ۿϷ����&��ְ���
					memset(PbData, 0, sizeof(PbData));
					PbDataCnt = 0;
				}
			}
			else break;
		}
		startline = 0;

		if (PbDataCnt)										// �������ڷ������
		{
			fnAPD_CheckDeviceAction(DEV_PBM);				// ���ۿϷ����&��ְ���
			fnAPD_PbPrint(WaitFlg);							// ��������
			memset(PbData, 0, sizeof(PbData));
			PbDataCnt = 0;
		}	
		
		memcpy(PbData, PbFrameSet, sizeof(PbFrameSet));
		PbDataCnt = sizeof(PbFrameSet);
	}

	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_PBM);					// ���ۿϷ����&��ְ���

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

	if (!PPPRecvData.ParaLength)								// DATA��/���������Ʈ(�ŷ�)
		return T_OK;											// �ش��ڷṫ

	PbDataCnt = 0;												// Pb��±���
	memset(PbData, 0, sizeof(PbData));							// Pb��¿���
	PbPrintFlag = TRUE;

	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);

	startline = PPPRecvData.y;

	for(int ii = 0; ii <= 23; ii++) 
	{
		if (!PPPRecvData.DataLength[0][ii])
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (ii == 11)	// ����� skip
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
		// ������ �ִ� ��� ���̰� ���ϵȴ�.
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

	if (PbDataCnt)											// �������ڷ������
	{
		fnAPD_CheckDeviceAction(DEV_PBM);					// ���ۿϷ����&��ְ���
		fnAPD_PbPrint(WaitFlg);								// ��������
		memset(PbData, 0, sizeof(PbData));
		PbDataCnt = 0;
	}
	fnAPD_CheckDeviceAction(DEV_PBM);						// ���ۿϷ����&��ְ���

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
		if (TranCode == TC_PBOOK)								// �������� : 2003.12.23
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

	if (!PPPRecvData.ParaLength)								// DATA��/���������Ʈ(�ŷ�)
		return T_OK;											// �ش��ڷṫ

	PbDataCnt = 0;												// Pb��±���
	memset(PbData, 0, sizeof(PbData));							// Pb��¿���
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

	if (PbDataCnt)											// �������ڷ������
	{
		fnAPD_CheckDeviceAction(DEV_PBM);					// ���ۿϷ����&��ְ���
		fnAPD_PbPrint(WaitFlg);								// ��������
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

		if (PbDataCnt)											// �������ڷ������
		{
			fnAPD_CheckDeviceAction(DEV_PBM);					// ���ۿϷ����&��ְ���
			fnAPD_PbPrint(WaitFlg);								// ��������
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
		m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
		m_pDevCmn->fnSCR_DisplayString(2, strTmp);
		m_pDevCmn->fnSCR_DisplayScreen(701);
		Delay_Msg(3000);
	}

	return T_OK;
}


////////////////////////////////////////////////////////////////////////////////
// �� �� �� : fnAPP_KB_PPPProc
// ��    �� : ���������Ʈ
// �Ķ���� : WaitFlg   - ����÷��� [IN]
// ���ǻ��� : ����
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPP_KB_PPPProc(int WaitFlg) //#N0226
{
	int		i, j, k;
	int		startline, endline, szPbLen;
	static	int		LocalLine = 0;
	int				LocalPage = 0;
	int				LocalI = 0;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KB_PPPProc");

	if (!PPPRecvDataKb.ParaLength)									// �������ù�
		return T_OK;
	

	PbDataCnt = 0;												// Pb��±���
	memset(PbData, 0, sizeof(PbData));							// Pb��¿���
	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);	

	startline = PPPRecvDataKb.Bp;
	for(i=0; i < startline; i++) 
	{
		if (!strlen((char *)PPPRecvDataKb.Data[0][i]))
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (i == 11)	// ����� skip
		{
			PbData[PbDataCnt++] = PbNewLine;
			PbData[PbDataCnt++] = PbNewLine;
		}
	}
	
	startline = i;


MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "������������[%d] ���ڽ��۶���[%d]",PPPRecvDataKb.p, startline);

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
				// 1�� �μ��� 13������ �ǳʰ� ��� �ζ����� �ǵ��Ѵ�.
				if (j == 11) 
				{
					PbData[PbDataCnt++] = 0x0a;
					PbData[PbDataCnt++] = 0x0a;
				}
				// ������ �������� �μ��� ��� ���� �������� �Ѿ��.
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

		if (PbDataCnt)										// �������ڷ������
		{
			fnAPD_CheckDeviceAction(DEV_PBM);				// ���ۿϷ����&��ְ���
			fnAPD_PbPrint(WaitFlg);							// ��������
			memset(PbData, 0, sizeof(PbData));
			PbDataCnt = 0;
		}	
		
		memcpy(PbData, PbFrameSet, sizeof(PbFrameSet));
		PbDataCnt = sizeof(PbFrameSet);
	}

	if (Accept.Index == 99)
	{
		m_pDevCmn->fnSCR_DisplayString(1, "���忡 ������ ������ ���ҽ��ϴ�");
		m_pDevCmn->fnSCR_DisplayString(3, "���������� �ٽ��Ͽ� �ֽʽÿ�");
		m_pDevCmn->fnSCR_DisplayScreen(710);
		Delay_Msg(3000);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_KB_PPPProc:return()");

	return T_OK;
}



// ���������Ʈ
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
		if (TranCode == TC_PBOOK)								// �������� : 2003.12.23
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

	if (!PPPRecvData.ParaLength)								// DATA��/���������Ʈ(�ŷ�)
		return T_OK;											// �ش��ڷṫ

	PbDataCnt = 0;												// Pb��±���
	memset(PbData, 0, sizeof(PbData));							// Pb��¿���
	PbPrintFlag = TRUE;

	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);

	startline = PPPRecvData.y;
	for(int ii = 0; ii <= 23; ii++) 
	{
		if (!PPPRecvData.DataLength[0][ii])
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (ii == 11)	// ����� skip
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
		// ������ �ִ� ��� ���̰� ���ϵȴ�.
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

	if (PbDataCnt)											// �������ڷ������
	{
		fnAPD_CheckDeviceAction(DEV_PBM);					// ���ۿϷ����&��ְ���
		fnAPD_PbPrint(WaitFlg);								// ��������
		memset(PbData, 0, sizeof(PbData));
		PbDataCnt = 0;
	}
	//#0025
	fnAPD_CheckDeviceAction(DEV_PBM);						// ���ۿϷ����&��ְ��� 
	if(PbPrintAfterMsgFlag == TRUE)
	{
		m_pDevCmn->fnSCR_DisplayString(2, "[�����̿�]");
		m_pDevCmn->fnSCR_DisplayString(3, "â������ ������ ��߱� �����ʽÿ� ");
		m_pDevCmn->fnSCR_DisplayScreen(701);
		Delay_Msg(5000);
	}


	return T_OK;
}

// ���������Ʈ
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
		if (TranCode == TC_PBOOK)								// �������� : 2003.12.23
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

	if (!PPPRecvData.ParaLength)								// DATA��/���������Ʈ(�ŷ�)
		return T_OK;											// �ش��ڷṫ

	PbDataCnt = 0;												// Pb��±���
	memset(PbData, 0, sizeof(PbData));							// Pb��¿���
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

	if (PbDataCnt)											// �������ڷ������
	{
		fnAPD_CheckDeviceAction(DEV_PBM);					// ���ۿϷ����&��ְ���
		fnAPD_PbPrint(WaitFlg);								// ��������
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

		if (PbDataCnt)											// �������ڷ������
		{
			fnAPD_CheckDeviceAction(DEV_PBM);					// ���ۿϷ����&��ְ���
			fnAPD_PbPrint(WaitFlg);								// ��������
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
		m_pDevCmn->fnSCR_DisplayString(1, "[���峻��]");
		m_pDevCmn->fnSCR_DisplayString(2, strTmp);
		m_pDevCmn->fnSCR_DisplayScreen(701);
		Delay_Msg(3000);
	}
	
	return T_OK;
}


// ���������Ʈ
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
		if (TranCode == TC_PBOOK)								// �������� : 2003.12.23
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

	memset(PbData, 0, sizeof(PbData));							// Pb��¿���
	PbDataCnt = 0;
	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);

	if((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&	(PbTran))		//V06-00-05-#01
		startline = LocalLine;
	else
		startline = 25;
	// ������ ã�� - �����Ͱ� NULL�̸� �����ǵ带 �Ѵ�.
	for(i=0; i < startline; i++) 
	{
		if (!strlen((char *)PPPRecvData.Data[0][i]))
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (i == 11)	// ����� skip
		{
			PbData[PbDataCnt++] = PbNewLine;
			PbData[PbDataCnt++] = PbNewLine;
		}
	}
	
	startline = i;


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "������������[%d] ���ڽ��۶���[%d]",PPPRecvData.p, startline);
//MsgDumpAnal(TRACE_CODE_MODE, "Log", "������������[%d]", PPPRecvData.p);

	// ������ ���� ���� 
	// PPPRecvData.p�� ������ �����Ͱ� �ִ� �������� ��ȣ�̴�.
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

			// ������ �ִ� ��� ���̰� ���ϵȴ�.
			if (szPbLen)
			{
				memcpy(&PbData[PbDataCnt], PPPRecvData.Data[i][j], szPbLen);

				PbDataCnt += szPbLen;
				PbData[PbDataCnt++] = 0x0a;
				// 1�� �μ��� 13������ �ǳʰ� ��� �ζ����� �ǵ��Ѵ�.
				if (j == 11) 
				{
					PbData[PbDataCnt++] = 0x0a;
					PbData[PbDataCnt++] = 0x0a;
				}
				// ������ �������� �μ��� ��� ���� �������� �Ѿ��.
				else 
				if (j == 24)
				{
					// ������ �������� �ƴѰ�� : 2002.02.25(�ӿ��������ٶ�)
					if (i != PPPRecvData.p)
					{
						memcpy(&PbData[PbDataCnt], PbFrameNextSet, sizeof(PbFrameNextSet));
						PbDataCnt += 5;
					}
					break;
				}

				// �μ��ϰ��� �ϴ� �����Ͱ� 1k block�� �����ʵ��� �Ѵ�.
//				2008.04.30 �������ڼӵ� ���� : 10K������ �ѹ濡 �������� �Ѵ�.
//				if (PbDataCnt > (PB_MAX_FRAME / 2))		// 512/1024�ʰ���(50%)
				if (PbDataCnt > (sizeof(PbData) -100))			// 10K(100%) 2004.07.29
				{
					// pbm���� �����͸� Passing�Ѵ�.
					fnAPD_PbPrint(TRUE);
					fnAPD_CheckDeviceAction(DEV_PBM);					// ���ۿϷ����&��ְ���
					memset(PbData, 0, sizeof(PbData));
					PbDataCnt = 0;
				}
			}
			else break;
		}
		startline = 0;

		if (PbDataCnt)										// �������ڷ������
		{
			fnAPD_CheckDeviceAction(DEV_PBM);				// ���ۿϷ����&��ְ���
			fnAPD_PbPrint(WaitFlg);							// ��������
			memset(PbData, 0, sizeof(PbData));
			PbDataCnt = 0;
		}	
		
		memcpy(PbData, PbFrameSet, sizeof(PbFrameSet));
		PbDataCnt = sizeof(PbFrameSet);
	}

	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_PBM);					// ���ۿϷ����&��ְ���

	return T_OK;
}

// ���������Ʈ  #N0245
int CTranCmn::fnAPP_SC_PPPProc(int WaitFlg)
{
	int		i, j, k;
	int		startline, endline, szPbLen;
	static	int		LocalLine = 0;
	int				LocalPage = 0;
	int				LocalI = 0;

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->HostLocalSvrMode == TRUE) && (PbTran))			// ���ü���������� (����������/���) #0424
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
				"2015     123-01-123456      SC���� ����TEST");
//				 12345678901234567890123456789012345678901234567890123456789012345678901234567890
				PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);
			}
			else
			{
				Strcpy(PPPRecvData.Data[LocalPage][LocalLine], 
				"20150211 BCD  �׽�Ʈ00         *123,456      *1,234,567 ����123");
//				 12345678901234567890123456789012345678901234567890123456789012345678901234567890
				PPPRecvData.DataLength[LocalPage][LocalLine] = Strlen(PPPRecvData.Data[LocalPage][LocalLine]);
			}

			LocalLine++;

			/* ���� AutoTurn �׽�Ʈ��
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

	memset(PbData, 0, sizeof(PbData));							// Pb��¿���
	PbDataCnt = 0;
	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);

	if((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE) &&	(PbTran))		//V06-00-05-#01
		startline = LocalLine;
	else
		startline = 23;

	// ������ ã�� - �����Ͱ� NULL�̸� �����ǵ带 �Ѵ�.
	for(i=0; i < startline; i++) 
	{
		if (!strlen((char *)PPPRecvData.Data[0][i]))
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (i == 11)	// ����� skip
		{
			PbData[PbDataCnt++] = PbNewLine;
			PbData[PbDataCnt++] = PbNewLine;
		}
	}
	
	startline = i;


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "������������[%d] ���ڽ��۶���[%d]",PPPRecvData.p, startline);
//MsgDumpAnal(TRACE_CODE_MODE, "Log", "������������[%d]", PPPRecvData.p);

	// ������ ���� ���� 
	// PPPRecvData.p�� ������ �����Ͱ� �ִ� �������� ��ȣ�̴�.
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

			// ������ �ִ� ��� ���̰� ���ϵȴ�.
			if (szPbLen)
			{
				memcpy(&PbData[PbDataCnt], PPPRecvData.Data[i][j], szPbLen);

				PbDataCnt += szPbLen;
				PbData[PbDataCnt++] = 0x0a;
				// 1�� �μ��� 13������ �ǳʰ� ��� �ζ����� �ǵ��Ѵ�.
				if (j == 11) 
				{
					PbData[PbDataCnt++] = 0x0a;
					PbData[PbDataCnt++] = 0x0a;
				}
				// ������ �������� �μ��� ��� ���� �������� �Ѿ��.
				else 
				if (j == 23)
				{
					// ������ �������� �ƴѰ�� : 2002.02.25(�ӿ��������ٶ�)
					if (i != PPPRecvData.p)
					{
						memcpy(&PbData[PbDataCnt], PbFrameNextSet, sizeof(PbFrameNextSet));
						PbDataCnt += 5;
					}
					break;
				}

				// �μ��ϰ��� �ϴ� �����Ͱ� 1k block�� �����ʵ��� �Ѵ�.
//				2008.04.30 �������ڼӵ� ���� : 10K������ �ѹ濡 �������� �Ѵ�.
//				if (PbDataCnt > (PB_MAX_FRAME / 2))		// 512/1024�ʰ���(50%)
				if (PbDataCnt > (sizeof(PbData) -100))			// 10K(100%) 2004.07.29
				{
					// pbm���� �����͸� Passing�Ѵ�.
					fnAPD_PbPrint(TRUE);
					fnAPD_CheckDeviceAction(DEV_PBM);					// ���ۿϷ����&��ְ���
					memset(PbData, 0, sizeof(PbData));
					PbDataCnt = 0;
				}
			}
			else break;
		}
		startline = 0;

		// ������ ���α��� ������ �� PbNextTranFlag�� TRUE�� ��� �������� �ѱ��. SC������ �������� ������ �����Ͱ� ���ŵ�
		if (j >= 23)
		{
			// ���������� �������� Flag�� TRUE�� ���
			if (PbNextTranFlag && (m_pProfile->DEVICE.PBHandler == I_PBM_SET))		// #0424 IPBM�� ����
			{
				memcpy(&PbData[PbDataCnt], PbFrameNextSet, sizeof(PbFrameNextSet));
				PbDataCnt += 5;
			}
		}
		
		if (PbDataCnt)										// �������ڷ������
		{
			fnAPD_CheckDeviceAction(DEV_PBM);				// ���ۿϷ����&��ְ���
			fnAPD_PbPrint(WaitFlg);							// ��������
			memset(PbData, 0, sizeof(PbData));
			PbDataCnt = 0;
		}	
		
		memcpy(PbData, PbFrameSet, sizeof(PbFrameSet));
		PbDataCnt = sizeof(PbFrameSet);
	}

	if (WaitFlg)
		fnAPD_CheckDeviceAction(DEV_PBM);					// ���ۿϷ����&��ְ���

	return T_OK;
}

// ���������Ʈ  #N0277
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
		if (TranCode == TC_PBOOK)								// �������� : 2003.12.23
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

	if (!PPPRecvData.ParaLength)								// DATA��/���������Ʈ(�ŷ�)
		return T_OK;											// �ش��ڷṫ

	PbDataCnt = 0;												// Pb��±���
	memset(PbData, 0, sizeof(PbData));							// Pb��¿���
	PbPrintFlag = TRUE;

	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));
	PbDataCnt += sizeof(PbFrameSet);

	startline = PPPRecvData.y;

	for(int ii = 0; ii <= 23; ii++) 
	{
		if (!PPPRecvData.DataLength[0][ii])
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (ii == 11)	// ����� skip
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
		// ������ �ִ� ��� ���̰� ���ϵȴ�.
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

	if (PbDataCnt)											// �������ڷ������
	{
		fnAPD_CheckDeviceAction(DEV_PBM);					// ���ۿϷ����&��ְ���
		fnAPD_PbPrint(WaitFlg);								// ��������
		memset(PbData, 0, sizeof(PbData));
		PbDataCnt = 0;
	}
	fnAPD_CheckDeviceAction(DEV_PBM);						// ���ۿϷ����&��ְ���

	return T_OK;
}

// ī��M/S���
int CTranCmn::fnAPP_PCWProc(int WaitFlg)
{
	if (!PCWRecvData.ParaLength)								// DATA��/ī��M/S WRITE(�ŷ�)
		return T_OK;											// �ش��ڷṫ

	if (MenuIrdaMode)											// ���ܼ��ŷ�
		return T_OK;

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		return T_OK;

	if (!CardTran)												// ī������üũ
		return T_OK;

	return T_OK;
}

// �޼���ǥ��
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
			if (m_pDevCmn->EarPhoneInserted == FALSE)   //�������ΰ�� ���¹�ȣ �ʵ� �Է��ϸ� �ȵ�.
				m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_017x);

			m_pDevCmn->fnSCR_DisplayAmount(2,  m_RD.byDataField_023x);
			m_pDevCmn->fnSCR_DisplayAmount(3, (LPCTSTR)m_RD.byDataField_024x);
//   		m_pDevCmn->fnSCR_DisplayAmount(4, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			m_pDevCmn->fnSCR_DisplayAmount(4, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
		}
		else
		if(TranCode2 == TRANID_4330 )
		{

			if (m_pDevCmn->EarPhoneInserted == FALSE)   //#N0193 �������ΰ�� ���¹�ȣ �ʵ� �Է��ϸ� �ȵ�.
				m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_017x);

			m_pDevCmn->fnSCR_DisplayAmount(2,  m_RD.byDataField_023x);

			if(memcmp(m_RD.byDataField_024x, "******", 6) == 0)   //-1  ������ * ȭ�� ǥ��
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
					cstrTemp.Format("������ �� %d.%2.2s  �ۼ�Ʈ", Asc2Int(&m_RD.byDataField_458x[0], 2), &m_RD.byDataField_458x[2]);
				m_pDevCmn->fnSCR_DisplayString(5, cstrTemp); //������  
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
			if (m_pDevCmn->EarPhoneInserted == FALSE)   //#N0193 �������ΰ�� ���¹�ȣ �ʵ� �Է��ϸ� �ȵ�.
				m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_017x);
			m_pDevCmn->fnSCR_DisplayAmount(2,  m_RD.byDataField_023x);
			m_pDevCmn->fnSCR_DisplayAmount(3, m_RD.byDataField_024x);
//  		m_pDevCmn->fnSCR_DisplayAmount(4, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
			if((m_pProfile->DEVICE.ETCDevice20 == KB) && (MenuAvail & MENU_PB))	//#N0226 ���� ���� �Ա� �� �ܾ� ǥ�� ����
				;
			else
				m_pDevCmn->fnSCR_DisplayAmount(4, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
		}
		else
		if (TranCode2 == TRANID_4390 || TranCode2 == TRANID_4370 || TranCode2 == TRANID_4215 || TranCode2 == TRANID_4396)
		{
			ScrNum = 546;	
			m_pDevCmn->fnSCR_DisplayString(1, fnAPP_Get_BankName(Accept.BankNum)); // ���������																	
			m_pDevCmn->fnSCR_DisplayString(2, Accept.InAccountNum);     // ������¹�ȣ																	
			m_pDevCmn->fnSCR_DisplayString(3, m_RD.byDataField_021x);   // �����μ���																	
			m_pDevCmn->fnSCR_DisplayAmount(4, m_RD.byDataField_023x);   // �ŷ��ݾ�																
			m_pDevCmn->fnSCR_DisplayAmount(5, (LPCTSTR)m_RD.byDataField_024x); //������
			if (m_pDevCmn->EarPhoneInserted == TRUE)		
			{
				strAccountNum.Format("<digit>%s</digit>", m_RD.byDataField_017x);
				m_pDevCmn->fnSCR_DisplayString(6, strAccountNum); //���¹�ȣ
			}
			else
				m_pDevCmn->fnSCR_DisplayString(6, m_RD.byDataField_017x); //���¹�ȣ

		}
		else
		if (TranCode2 == TRANID_4350)
		{
			ScrNum = 547;	
			m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_020x);
			m_pDevCmn->fnSCR_DisplayAmount(2,  m_RD.byDataField_023x);

			if(memcmp(m_RD.byDataField_024x, "******", 6) == 0)   //-1  ������ * ȭ�� ǥ��
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
				m_pDevCmn->fnSCR_DisplayString(5, cstrTemp); //������  
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


		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("��ǥ   : %s", (Accept.SlipChoiceFlag == TRUE)	? "����" : "�̼���"));
	}

	return T_OK;
}


// A4 PRINT PROC
int CTranCmn::fnAPP_A4PrintProc(int nTranCode)
{
	switch (nTranCode)
	{
		case TC_INQUIRYA:										// �ŷ�������ȸ
			fnAPP_A4PrintTranList();							// �ŷ�������ȸ	A4Print
			break;
		case TC_INQDEP:
		case TC_INQSECUR:										// ��������ȸ	A4Print
			fnAPP_A4PrintDepSequreInquiry();					// ����������ȸ	A4Print
			break;
		case TC_INQSTOCK:
			fnAPP_A4PrintInqStock();					// ����������ȸ	A4Print
			break;
		default:
			break;
	}
	return T_OK;
}

// �ŷ�������ȸ A4 Print
int CTranCmn::fnAPP_A4PrintTranList()							// �ŷ�������ȸ A4Print
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

// ��������ȸ & ����������ȸ A4 Print
int CTranCmn::fnAPP_A4PrintDepSequreInquiry()					// ��������ȸ & ����������ȸ A4Print
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


// û�೻�� ��ȸ A4 Print
int CTranCmn::fnAPP_A4PrintInqStock()					// ��������ȸ & ����������ȸ A4Print
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
// ��ȸ�İŷ�����
int CTranCmn::fnAPP_PMEAcceptInquiryTranSel()
{

	return T_OK;
}
// ���񽺳�����ȸ Ȯ��
int CTranCmn::fnAPP_PMEAcceptInquiryTranServiceConfirm()
{
	int		ScrNum = 0;
	CString GetKeyStr;

	CString Data;
	Data.Empty();

	ChoiceTran = TRAN_NULL;									//#N0223 ���� �ʱ�ȭ 

	if(WithAvail == WITH_NOT_AVAILABLE)
		m_pDevCmn->fnSCR_DisplayImage(1, FALSE);			//#N0223 ��� �Ұ����϶� ��� ��ư ����

	if(m_pProfile->DEVICE.ETCDevice20 != HANA)
	{
		m_pDevCmn->fnSCR_DisplayImage(1, FALSE);			//#N0223 �ϳ����� �귣�� ������ ���, ��ü ��ư ��� HU81K508���� ��ư ����.
		m_pDevCmn->fnSCR_DisplayImage(2, FALSE);			
	}

	if (TranCode == TC_INQUIRY)								// ����
	{
		ScrNum = 508;											// ���񽺳�����ȸ
		m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_017x);
//#0130	m_pDevCmn->fnSCR_DisplayAmount(2, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
		m_pDevCmn->fnSCR_DisplayAmount(2, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
		m_pDevCmn->fnSCR_DisplayAmount(3, m_RD.byDataField_025x, strlen(m_RD.byDataField_025x), ' ', (m_RD.byDataField_025x[0] == '-') ? TRUE : FALSE);
	}
	else
	if (TranCode == TC_SVCINQ || TranCode == TC_FOREGIN)		// ���񽺳�����ȸ �ŷ�
	{
		ScrNum = 509;											// ���񽺳�����ȸ
		m_pDevCmn->fnSCR_DisplayString(1, m_RD.byDataField_017x);
		m_pDevCmn->fnSCR_DisplayAmount(2, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
		if (TranCode2 == TRANID_7230) 
			m_pDevCmn->fnSCR_DisplayAmount(3, (LPCTSTR)m_RD.byDataField_024x);
	}

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();

	if (GetKeyStr == S_EXIT)								// 1�� ��ȸ�ŷ��� ���� Ȯ��
	{
		ChoiceTran = TRAN_NULL;
	}
	else 
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
	else 
	if (GetKeyStr == S_TIMEOVER)							// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
//	else
//		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// ��ȸ���ǥ��
int CTranCmn::fnAPP_PMEAcceptInquiryDisplay()
{
	int		ScrNum = 0, StrIndex = 1;
	CString	GetKeyStr;
	int		i = 0;

	return T_OK;
}

// ������ü Ȯ��
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
																	// ���������
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, Accept.InAccountNum);
																	// ������¹�ȣ
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_021x);
																	// �����μ���
		m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, m_RD.byDataField_023x);
																	// �ŷ��ݾ�
		m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, (LPCTSTR)m_RD.byDataField_024x); //������
		
		if (m_pDevCmn->EarPhoneInserted == TRUE)	
		{
			strAccountNum.Format("<digit>%s</digit>", m_RD.byDataField_017x);
			m_pDevCmn->fnSCR_DisplayString(6, strAccountNum); //���¹�ȣ
		}
		else
			m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_017x); //���¹�ȣ
		
		memcpy(Accept.InAccountName, m_RD.byDataField_021x, strlen(m_RD.byDataField_021x)); // ������¹�ȣ

		if(m_pProfile->DEVICE.ETCDevice20 == KEB && memcmp(&m_RD.byDataField_345x[0], "PT", 2) == 0)  //#N0180
			m_pDevCmn->fnSCR_DisplayString(10, m_RD.byDataField_345x);
	}
	else
	if(TranCode2 == TRANID_7380 || TranCode2 == TRANID_7250)  // ���� �귣�� �ſ�ī�� ��ü
	{
		if(TranResultNGCode == TRAN_MSG_EXCEPTION_707)
			ScrNum = 427;
		else
			ScrNum = 527;
		if (m_pDevCmn->EarPhoneInserted == FALSE)   //�������ΰ�� ����� �ʵ� �Է��ϸ� �ȵ�.
		{
			m_pDevCmn->fnSCR_DisplayString(1, GetString(cstrTemp.GetBuffer(0), cstrTemp.GetLength(), cstrTemp.GetLength()));
																	// ���������
			m_pDevCmn->fnSCR_DisplayString(2, Accept.InAccountNum);
																		// ������¹�ȣ
			m_pDevCmn->fnSCR_DisplayString(3, m_RD.byDataField_021x);
																	// �����μ���
		}
		m_pDevCmn->fnSCR_DisplayAmount(4, m_RD.byDataField_023x);
																	// �ŷ��ݾ�
		m_pDevCmn->fnSCR_DisplayAmount(5, (LPCTSTR)m_RD.byDataField_024x); //������	

		//
		if (memcmp(m_RD.byDataField_458x, "????", 4) != 0)
		{
			cstrTemp.Empty();
			if (m_pDevCmn->EarPhoneInserted == FALSE)	
				cstrTemp.Format("%d.%2.2s", Asc2Int(&m_RD.byDataField_458x[0], 2), &m_RD.byDataField_458x[2]);
			else
				cstrTemp.Format("������ �� %d.%2.2s  �ۼ�Ʈ", Asc2Int(&m_RD.byDataField_458x[0], 2), &m_RD.byDataField_458x[2]);
			m_pDevCmn->fnSCR_DisplayString(6, cstrTemp); //������  
		}
		memcpy(Accept.InAccountName, m_RD.byDataField_021x, strlen(m_RD.byDataField_021x)); // ������¹�ȣ
		
		StrIndex = 7; //������ �ŷ��� ȭ�� ��� ���� ��� ��ũ ����, ��ǥ ���� ��� X
	}
	else
	{
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(cstrTemp.GetBuffer(0), cstrTemp.GetLength(), cstrTemp.GetLength()));
																	// ���������
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_020x);
																	// ������¹�ȣ
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_021x);
																	// �����μ���
		m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, m_RD.byDataField_023x);
																	// �ŷ��ݾ�
		m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, (LPCTSTR)m_RD.byDataField_024x);
		memcpy(Accept.InAccountNum,  m_RD.byDataField_020x, strlen(m_RD.byDataField_020x));	// �԰���¹�ȣ
		memcpy(Accept.InAccountName, m_RD.byDataField_021x, strlen(m_RD.byDataField_021x));// �԰���¸�
	}

	if (m_pDevCmn->EarPhoneInserted == FALSE)		// ������ �ŷ��� ��ǥ ���� �Է� X
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
																// ������
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

// �۱� Ȯ��
int CTranCmn::fnAPP_PMEAcceptRemitConfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString GetKeyStr("");

	ScrNum = 512;

	m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_017x);
																// ������¹�ȣ
	m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_021x);
																// �����μ���
	m_pDevCmn->fnSCR_DisplayAmount(StrIndex++,  m_RD.byDataField_023x);
																// �ŷ��ݾ�
	m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, (LPCTSTR)m_RD.byDataField_024x);
																// ������
//#0130
//	m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
	m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, &m_RD.byDataField_022x[1], strlen(m_RD.byDataField_022x) - 1, ' ', (m_RD.byDataField_022x[0] == '-') ? TRUE : FALSE);
																// �ŷ��ݾ�
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

// �ڵ���ü���� Ȯ��
int CTranCmn::fnAPP_PMEAcceptAutoConfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString GetKeyStr("");

	ScrNum = 514;

	m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, (LPCTSTR)Accept.Money, 15);
																// �����ݾ�
	m_pDevCmn->fnSCR_DisplayString(StrIndex++, (LPCTSTR)Accept.AutoCycle, 2);
																// ��ü�ֱ�
	m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetSprintf("%4.4s�� %2.2s�� %2.2s��",
															&PMERecvData.Data[2][0],
															&PMERecvData.Data[2][4],
															&PMERecvData.Data[2][6]));
																// ��ü������
	m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetSprintf("%4.4s�� %2.2s�� %2.2s��",
															&PMERecvData.Data[2][8],
															&PMERecvData.Data[2][12],
															&PMERecvData.Data[2][14]));
																// ��ü������
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

// �ŷ�������ȸ �ŷ� Ȯ��
int CTranCmn::fnAPP_PMEAcceptTranListConfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString	GetKeyStr("");
	CString	DispMsg(""), tmpMsg("");
	char	szTemp[32] = {0,};

	if(TranCode == TC_INQUIRYA)									// �ŷ�������ȸ
		ScrNum = 507;
	else if(TranCode == TC_INQSECUR)							// ����������ȸ
		ScrNum = 506;
	else if(TranCode == TC_INQSTOCK)							// û�೻����ȸ
		ScrNum = 962;


	return T_OK;
}

// �������� ����Ʈ����
int CTranCmn::fnAPP_PMEAcceptSeqListConfirm()
{


	return T_OK;
}

// �ڻ��üȮ��
int CTranCmn::fnAPP_PMEAcceptTranStocConfirm()
{
	int		ScrNum = 0, nIndex = 1;
	CString GetKeyStr("");

	ScrNum = 904;												// �ڻ��ü Ȯ��

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);

																// �԰���¸�
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	if (GetKeyStr == S_CONFIRM)									// ��ȸ�ŷ��� ���� Ȯ��
		return T_OK;
	else 
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);								// �ŷ����ó��
	else 
	if (GetKeyStr == S_TIMEOVER)								// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);


	return T_OK;
}

// �������� û��Ȯ��
int CTranCmn::fnAPP_PMEAcceptSeqTranConfirm()
{
	int		ScrNum = 0, nIndex = 1;
	CString GetKeyStr("");

	ScrNum = 925;											// û��Ȯ��

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);

	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	if (GetKeyStr == S_CONFIRM)									// ��ȸ�ŷ��� ���� Ȯ��
		return T_OK;
	else 
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);								// �ŷ����ó��
	else 
	if (GetKeyStr == S_TIMEOVER)								// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);


	return T_OK;
}

// �������Ȯ��
int CTranCmn::fnAPP_PMEAcceptCanSeqTranConfirm()
{
	int		ScrNum = 0, nIndex = 1;
	CString GetKeyStr("");

	ScrNum = 942;											// �������Է�

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayString(nIndex++, GetSprintf("%-40.40s", &PMERecvData.Data[Accept.SelStockIndex-1][20]));
															// û����������
	m_pDevCmn->fnSCR_DisplayAmount(nIndex++, GetString(&PMERecvData.Data[Accept.SelStockIndex-1][60], 15, 15), 15);
															// û���û����
	m_pDevCmn->fnSCR_DisplayAmount(nIndex++, GetString(&PMERecvData.Data[Accept.SelStockIndex-1][75], 15, 15), 15);
															// û��������ű�
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_NUMERIC_MODE);

	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	if (GetKeyStr == S_CONFIRM)									// ��ȸ�ŷ��� ���� Ȯ��
	{
		memcpy(Accept.StocksCode, &PMERecvData.Data[Accept.SelStockIndex-1][0], 20);// û�����ű�
		return T_OK;
	}
	else 
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);								// �ŷ����ó��
	else 
	if (GetKeyStr == S_TIMEOVER)								// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);


	return T_OK;
}

// ���װ��¼���
int CTranCmn::fnAPP_PMEAcceptRemitTranSel()
{
	int		ScrNum = 0;
	CString GetKeyStr("");

/////////////////////////////////////////////////////////////////////////////
	Accept.RemitAccountNumSize = 0;								// �����Աݰ��¹�ȣ����
	memset(Accept.RemitAccountNum, ' ', sizeof(Accept.RemitAccountNum));	
																// �����Աݰ��¹�ȣ
/////////////////////////////////////////////////////////////////////////////
	ScrNum = 524;

	if (!ScrNum)
		fnAPP_CancelDepRejectProc(T_PROGRAM);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
																// ���¹�ȣ����
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
	if ((Asc2Int(GetKeyStr) <= 0)	||							// �ڷ����
		(Asc2Int(GetKeyStr) > 3)	)
		fnAPP_CancelDepRejectProc(T_INPUTERROR);
	else
	{
		Accept.RemitAccountNumSize = 12;
///		memcpy(Accept.RemitAccountNum, &RecvHead.CardData[((Asc2Int(GetKeyStr) - 1)*12) + 9], 12);
	}

	return T_OK;
}

// �Ϲ����� ���γ���Ȯ�� 1��
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

///	m_pDevCmn->fnSCR_DisplayString(1, GetString(RecvHead.CustomName, 20, 20));	// ����
	m_pDevCmn->fnSCR_DisplayString(2, GetString(PMERecvData.Data[1], 19, 19));	// ���ڳ��ι�ȣ
	m_pDevCmn->fnSCR_DisplayString(3, GetString(PMERecvData.Data[2], 16, 16));	// ���α��
	
	if (Asc2Int(GetString(PMERecvData.Data[23], 1, 1).GetBuffer(0), 1))
		m_pDevCmn->fnSCR_DisplayString(4, GetSprintf("ü������: %2.2s�� %2.2s�� - %2.2s�� %2.2s��(%3.3s����)",
													&PMERecvData.Data[24][0],
													&PMERecvData.Data[24][2],
													&PMERecvData.Data[24][4],
													&PMERecvData.Data[24][6],
													&PMERecvData.Data[24][8]));
	else
		m_pDevCmn->fnSCR_DisplayString(4, "ü������ ����");
	
	MaxTranCnt = __min(4, Asc2Int(&PMERecvData.Data[5], 2));	// �ŷ��Ǽ�(4���ʰ��� 4�Ǹ�ó��)

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
		fnAPP_CancelProc(T_CANCEL);								// �ŷ����ó��
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

// ���ΰ����� ���γ���Ȯ�� 2��
int CTranCmn::fnAPP_PMEAcceptJiroTaxConfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString	GetKeyStr;
	char	szData[1024];
	int		MaxTranCnt = 0, nTranCnt = 0;

	memset(szData,		0x00, sizeof(szData));

	ScrNum = 531;

///	m_pDevCmn->fnSCR_DisplayString(1, GetString(RecvHead.CustomName, 20, 20));	// ����
	m_pDevCmn->fnSCR_DisplayString(2, GetString(PMERecvData.Data[1], 19, 19));	// ���ڳ��ι�ȣ
	m_pDevCmn->fnSCR_DisplayString(3, GetString(PMERecvData.Data[2], 16, 16));	// ���α��
	
	if (Asc2Int(GetString(PMERecvData.Data[23], 1, 1).GetBuffer(0), 1))
		m_pDevCmn->fnSCR_DisplayString(4, GetSprintf("ü������: %2.2s�� %2.2s�� - %2.2s�� %2.2s��(%3.3s����)",
													&PMERecvData.Data[24][0],
													&PMERecvData.Data[24][2],
													&PMERecvData.Data[24][4],
													&PMERecvData.Data[24][6],
													&PMERecvData.Data[24][8]));
	else
		m_pDevCmn->fnSCR_DisplayString(4, "ü������ ����");

	for (int i = 0; i < 4; i++)
	{
		if (Accept.JiroTranDetails[24 + i*21] == 'Y')
		{
			Sprintf(szData, 24, "   %4.4s  %15.15s", PMERecvData.Data[5 + i*5 + 1],
													 Asc2Amt(PMERecvData.Data[5 + i*5 + 3], 12, 15));
			m_pDevCmn->fnSCR_DisplayString(i + 5, szData);		// ���αݾ�
			nTranCnt++;
		}
	}

	m_pDevCmn->fnSCR_DisplayNumeric(9, nTranCnt);							// ���ΰǼ�
	m_pDevCmn->fnSCR_DisplayAmount(10, GetString(Accept.JiroMoney, 8, 8));	// �ݾ�

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	
	if (GetKeyStr == S_CONFIRM)									// 1�� ��ȸ�ŷ��� ���� Ȯ��
	{
		return T_OK;
	}
	else 
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);								// �ŷ����ó��
	else 
	if (GetKeyStr == S_TIMEOVER)								// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

// ���ΰ����� �������� ���γ���Ȯ��
int CTranCmn::fnAPP_PMEAcceptJiroFinConfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString	GetKeyStr;
	char	szData[1024];

	if (TranCode == TC_JRTRAN)	 								// ���ΰ����ݳ���
	{
		memset(Accept.Money, '0', sizeof(Accept.Money));		// �ݾ�(����), �ݾ�(��õ��)
		memset(szData, 0x00, sizeof(szData));

		ScrNum = 533;											// �������� Ȯ��

		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(PMERecvData.Data[2], 20, 20));	// ���α����
///		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(RecvHead.CustomName, 20, 20));	// �����ڼ���
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(PMERecvData.Data[1], 20, 20));	// ���ڳ��ι�ȣ
		if (Accept.JiroTranCode == TC_JRPHONE)
			sprintf(szData, "KT��ȭ���");
		else
		if (Accept.JiroTranCode == TC_JRELEC)
			sprintf(szData, "������");
		else
		if (Accept.JiroTranCode == TC_JRINSURE)
			sprintf(szData, "�ǰ������");
		else
		if (Accept.JiroTranCode == TC_JRPANS)
			sprintf(szData, "���ο���");
		else
		if (Accept.JiroTranCode == TC_JRJIRO)
			sprintf(szData, "���δ��");
		else
		if (Accept.JiroTranCode == TC_JRNTAX)
			sprintf(szData, "����/����");
		else
		if (Accept.JiroTranCode == TC_JRLTAX)
			sprintf(szData, "���漼/���ܼ���");
		else
		if (Accept.JiroTranCode == TC_JRWATER)
			sprintf(szData, "���ϼ������");
		else
		if (Accept.JiroTranCode == TC_JRTRAFF)
			sprintf(szData, "�����Ģ��");
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, szData);									// ��������
		Sprintf(szData, 8, "%2.2s-%2.2s-%2.2s", 
							GetString(&PMERecvData.Data[5][2], 2, 2),						// 2007-01-03-#4 index���� 0->2
							GetString(&PMERecvData.Data[5][4], 2, 2),						// 2007-01-03-#4 index���� 2->4
							GetString(&PMERecvData.Data[5][6], 2, 2));						// 2007-01-03-#4 index���� 4->6
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(szData, 8, 8));				// ���α���
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, (PMERecvData.Data[6][0] == '0') ? "������" :
																					 "������");
																							// �������ı���
//		m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, (LPCTSTR)RecvHead.TranFee, 5);			// ���μ�����
		m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, (LPCTSTR)&PMERecvData.Data[7], 12);		// ���αݾ�
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(PMERecvData.Data[8], 15, 15));	// �����ڹ�ȣ
		Sprintf(szData, 7,  "%2.2s-%4.4s",
							GetString(&PMERecvData.Data[10][0], 2, 2),
							GetString(&PMERecvData.Data[10][2], 4, 4));
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, GetString(szData, 7, 7));				// ¡�����¹�ȣ

		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE);
		GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
		
		if (GetKeyStr == S_CONFIRM)								// 1�� ��ȸ�ŷ��� ���� Ȯ��
		{
			memcpy(Accept.JiroMoney, &PMERecvData.Data[7][4], sizeof(Accept.JiroMoney));
																// �ŷ��ݾ�����
			return T_OK;
		}
		else 
		if (GetKeyStr == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
		else 
		if (GetKeyStr == S_TIMEOVER)							// TIMEOVER
			fnAPP_CancelProc(T_TIMEOVER);
		else
			fnAPP_CancelProc(T_PROGRAM);
	}
	return T_OK;
}

// ���γ��γ��� ��ȸȮ��
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
	m_pDevCmn->fnSCR_DisplayString(1, GetString(RecvHead.CustomName, 20, 20));	// ����
	m_pDevCmn->fnSCR_DisplayString(2, GetString(PMERecvData.Data[2], 16, 16));	// ���α��
	m_pDevCmn->fnSCR_DisplayString(3, GetString(PMERecvData.Data[1], 19, 19));	// ���ڳ��ι�ȣ
	
	MaxTranCnt = __min(4, Asc2Int(&PMERecvData.Data[4], 1));	// �ŷ��Ǽ�(4���ʰ��� 4�Ǹ�ó��)

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
	
	if (GetKeyStr == S_CONFIRM)									// 1�� ��ȸ�ŷ��� ���� Ȯ��
		return T_OK;
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);								// �ŷ����ó��
	else 
	if (GetKeyStr == S_TIMEOVER)								// TIMEOVER
		fnAPP_CancelProc(T_TIMEOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);

	return T_OK;
}

// ���� ���γ��γ��� ��ȸȮ��(����,���漼,��Ģ��)
int CTranCmn::fnAPP_PMEAcceptJiroFinInqComfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString	GetKeyStr;
	char	szData[1024];

	memset(szData, 0x00, sizeof(szData));
	Accept.SlipChoiceFlag = FALSE;								// �⺻ : ��ǥ������ó��

	return T_OK;
}

// ����ü�Ա� ��ȸȮ��
int CTranCmn::fnAPP_PMEAcceptTaDepositConfirm()
{
	int		ScrNum = 0, StrIndex = 1;
	CString GetKeyStr("");
	CString	cstrTemp("");

	ScrNum = 516;

//	m_pDevCmn->fnSCR_DisplayString(StrIndex++, (LPCSTR)&m_RXATM3061.depositreslist[0], 16);		// ������¹�ȣ
//	m_pDevCmn->fnSCR_DisplayString(StrIndex++, (LPCSTR)&m_RXATM3061.depositreslist[16+40], 20);	// �����μ���
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
// Ÿ���Ա���ȸ 
int CTranCmn::fnAPP_PMEAcceptDepConfirm()
{
	int		ScrNum = 518;
	CString	strReturn = _T("");
	int		StrIndex = 1;

																// ���¹�ȣ
	m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_017x);
																// ����
	if((m_pProfile->DEVICE.ETCDevice20) == KWANGJU && (CardBank == KAWBANK))   //�������� �Ա� �� 018x�ʵ� ��� #N0181
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_018x);
	else
		m_pDevCmn->fnSCR_DisplayString(StrIndex++, m_RD.byDataField_021x);
																// ������
	m_pDevCmn->fnSCR_DisplayAmount(StrIndex++, m_RD.byDataField_024x);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_30_WAIT, PIN_MENU_MODE1);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();

	if (strReturn == S_CONFIRM)
		return T_OK;
	else
	if (strReturn == S_CANCEL)  //#SJ 1004 ������ȸ ��ȯ
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
// Ÿ���Ա���ȸ 
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
// �غ��ó��
int CTranCmn::fnAPP_ReadyProc()
{
	if (!m_pDevCmn->TranResult)
	{
		fnAPP_PMDProc();
	}
	return T_OK;
}

// �غ�ϰ������
int CTranCmn::fnAPP_ReadyPrintResult(int PrintDevice, int WaitFlg)
{
	return T_OK;
}

// ����ó��
int CTranCmn::fnAPP_OpenProc()
{
	char	Pdate[9];
//////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
	{
		GetDate(Pdate);											// ����
		m_pDevCmn->fnAPL_SetDate(Pdate);						// ����� : 2002.05.21
		m_pDevCmn->fnAPL_SetCheckDate(Pdate);					// ��ǥ��������

		m_pProfile->GetTransProfile();							// �������ʱ�ȭ : 2002.08.09
		m_pProfile->TRANS.MagamFlg = OFF;
		m_pProfile->PutTransProfile();
		memcpy(OpenInfo.WithCashCount, "100", 3);
		memcpy(OpenInfo.WithCheckCount,	"010", 3);
		
		return T_OK;											// SEND OK
	}

/////////////////////////////////////////////////////////////////////////////

	return T_OK;
}

// �����������
int CTranCmn::fnAPP_OpenPrintResult(int PrintDevice, LPCTSTR szDate, LPCTSTR szTime, int WaitFlg)
{
	int		HiddenFlg = FALSE;									// ��ɻ���
	int		ll = 0;

	if (PrintDevice & DEV_SPR)									// �������ڴ� ���θ� ����
		PrintDevice &= ~DEV_SPR;
	
	if (PrintDevice == DEV_NONE)
		return T_OK;

	memset(ShuData, 0, sizeof(ShuData));						// Shu��¿���

	// 1 line
	fnAPP_EditPrintLine(DEV_JPR, ShuData[ll++], 0, HiddenFlg);
	// 2 line
	sprintf(ShuData[ll++], "%4.4s��%2.2s��%2.2s�� %2.2s:%2.2s:%2.2s",
							&szDate[0], &szDate[4], &szDate[6],
							&szTime[0], &szTime[2], &szTime[4]);

	sprintf(ShuData[ll++], GetSprintf("POWER OFF TIME: %s", RegGetStr(_REGKEY_DEVICE, "POWER_OFF_TIME", "00000000000000")));
	sprintf(ShuData[ll++], GetSprintf("�ڵ������ȣ: %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum));
	sprintf(ShuData[ll++], GetSprintf("�ڵ���������ȣ: %20.20s",    m_pProfile->NETWORK.MachineMakeNum));
	sprintf(ShuData[ll++], "��������ȸ����: %s", (m_pProfile->TRANS.TranInqFee) ? "���" : "������");

	sprintf(ShuData[ll++], GetSprintf("���ִ����ݾ�: %d", m_pProfile->TRANS.JaMaxDispAmt));
	sprintf(ShuData[ll++], GetSprintf("Ÿ�ִ����ݾ�: %d", m_pProfile->TRANS.TaMaxDispAmt));
	sprintf(ShuData[ll++], GetSprintf("��ǥ��뼳��: %s", (m_pProfile->DEVICE.SHUHandler) ? "���" : "������"));
	sprintf(ShuData[ll++], GetSprintf("���弳������: %d", m_pProfile->DEVICE.SoundVolume));
	sprintf(ShuData[ll++], "AP����        : %s", VERSION);
	sprintf(ShuData[ll++], "�������      : %s", (m_pDevCmn->TranResult) ? "OK" : "NG");

	//����������������������������������������������������������  EMV �ŷ��ʱ�ȭ #N9999
	fnAPP_EMVInitProc();
	fnAPP_EMVOpenSetProc();
	//��������������������������������������������������������������
	fnAPD_ShuPrint(PrintDevice, JNL_TRAN, WaitFlg);						// ��ǥ&��������

	return T_OK;
}

// Des�����������
int CTranCmn::fnAPP_OpenDesPrintResult(int PrintDevice, int WaitFlg)
{
	int		ll = 0;

	if (PrintDevice & DEV_SPR)									// �������ڴ� ���θ� ����(�츮����)
		PrintDevice &= ~DEV_SPR;
	
	if (PrintDevice == DEV_NONE)
		return T_OK;

	if (!AddSerialFlg)											// �Ϸù�ȣ������
	{
		AddSerialFlg = TRUE;									// �Ϸù�ȣ������
		m_pDevCmn->fnAPL_AddSerialNo();							// �ŷ��Ϸù�ȣ����
	}

	memset(ShuData, 0, sizeof(ShuData));						// Shu��¿���

	char	CurDate[9];											// ��� ����
	char	CurTime[7];											// ��� �ð�

	memset(CurDate, 0x00, sizeof(CurDate));
	memset(CurTime, 0x00, sizeof(CurTime));
	GetDate(CurDate);
	GetTime(CurTime);
	
	sprintf(ShuData[ll++], "���νð�: %2.2s�� %2.2s�� %2.2s��",
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

	fnAPD_ShuPrint(PrintDevice, JNL_TRAN, WaitFlg);						// ��ǥ&��������
	
	return T_OK;
}

// �հ�ó��
int CTranCmn::fnAPP_SumProc(int InputID)
{
	char	iCount[11] = { "0000000001" };
	char	iMoney[10];											// �ݾ�(����), �ݾ�(��õ��) : �ŷ�(��/��) �ѱݾ�
	char	iCashMoney[10];										// �ݾ�(����), �ݾ�(��õ��)	: �ŷ�(��/��) ���ݾ�
	char	iCheckMoney[10];									// �ݾ�(����), �ݾ�(��õ��)	: �ŷ�(��/��) ��ǥ��
	char	iCheckMotherMoneyM[10];								// �ݾ�(����)				: �ŷ�(��)    ��ǥ��
	char	iCheckMotherMoneyO[10];								// �ݾ�(����)				: �ŷ�(��)    ��ǥ��
	char	iCheckOtherMoney[10];								// �ݾ�(����)				: �ŷ�(��)    ��ǥ��

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
		case 0:													// ����
			m_pProfile->GetWithTotal();

			if (FinishFlag == 'A')
				nSubFlag = 0;									// ������
			else
				nSubFlag = 5;									// ������

			if (CancelWithProcFlg)
			{
				if (Asc2Int(iMoney, sizeof(iMoney)))			// 3 : ������
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
				{												// 0 : �������  
					if(TranCode2 == TRANID_4310 || TranCode2 == TRANID_4210)  
					{
						AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+0].Amount, iCashMoney, 10);
						AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+0].Count, iCount, 10);
					}
					else
					if(TranCode2 == TRANID_4330)               //  4 : ������� 
					{
						AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+4].Amount, iCashMoney, 10);
						AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+4].Count, iCount, 10);
					}

				}

				if (Asc2Int(iCheckMoney, sizeof(iCheckMoney)))
				{												// 1 : ��ǥ���
					AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+1].Amount, iCheckMoney, 10);
					AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+1].Count, iCount, 10);
				}
																// 2 : Total
				AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+2].Amount, iMoney, 10);
				AddString(m_pProfile->WITHTOTAL.DATA[nSubFlag+2].Count, iCount, 10);
			}
			
			m_pProfile->PutWithTotal();
			break;

		case 1:													// ��ü(��ü)
			m_pProfile->GetTransTotal();

			if (FinishFlag == 'A')
				nSubFlag = 0;									// ������
			else
				nSubFlag = 0;									// ������


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

		case 2:													// �Ա�
			m_pProfile->GetDepTotal();

			if (FinishFlag == 'A')
				nSubFlag = 0;									// ������
			else												// ������
				nSubFlag = 4;

			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		// 0 : �����Ա�
			{
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+0].Amount, iCashMoney, 10);
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+0].Count, iCount, 10);
			}

			if (Asc2Int(iCheckMotherMoneyO, sizeof(iCheckMotherMoneyO)))
			{													// 1 : ����������ǥ�հ�
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+1].Amount, iCheckMotherMoneyO, 10);
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+1].Count, iCount, 10);
			}

			if (Asc2Int(iCheckMotherMoneyM, sizeof(iCheckMotherMoneyO)))
			{													// 1 : ����Ÿ����ǥ�հ�
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+1].Amount, iCheckMotherMoneyM, 10);
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+1].Count, iCount, 10);
			}

			if (Asc2Int(iCheckOtherMoney, sizeof(iCheckOtherMoney)))
			{													// 2 : Ÿ���ǥ�հ�
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+2].Amount, iCheckOtherMoney, 10);
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+2].Count, iCount, 10);
			}


			m_pProfile->PutDepTotal();
			break;
		case 3:													// �Ա�
			m_pProfile->GetDepTotal();

			if (FinishFlag == 'A')
				nSubFlag = 0;									// ������
			else												// ������
				nSubFlag = 4;
			
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  : �����Ա�(����)
			{
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+3].Amount, iCashMoney, 10);
				AddString(m_pProfile->DEPTOTAL.DATA[nSubFlag+3].Count, iCount, 10);
			}
			m_pProfile->PutDepTotal();
			break;
		case 4:													// �Ա�
			m_pProfile->GetDepTotal();

			if (FinishFlag == 'A')
				nSubFlag = 0;									// ������
			else												// ������
				nSubFlag = 4;
			
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  : �����Ա�(����)
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


// �Ա����
int CTranCmn::fnAPP_DepErrSumProc(int InputID)
{

	if (TranProc == TRAN_DEP)
		;
	else
		return T_OK;

	char	iCount[11] = { "0000000001" };
	char	iMoney[10];											// �ݾ�(����), �ݾ�(��õ��) : �ŷ�(��/��) �ѱݾ�
	char	iCashMoney[10];										// �ݾ�(����), �ݾ�(��õ��)	: �ŷ�(��/��) ���ݾ�
	char	iCheckMoney[10];									// �ݾ�(����), �ݾ�(��õ��)	: �ŷ�(��/��) ��ǥ��
	char	iCheckMotherMoneyM[10];								// �ݾ�(����)				: �ŷ�(��)    ��ǥ��
	char	iCheckMotherMoneyO[10];								// �ݾ�(����)				: �ŷ�(��)    ��ǥ��
	char	iCheckOtherMoney[10];								// �ݾ�(����)				: �ŷ�(��)    ��ǥ��
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
		sprintf(PERR.TranType_Account   , "�Աݾ���  %16.16s", szTmp);
	else
		sprintf(PERR.TranType_Account   , "�����  %16.16s", szTmp);

	switch (InputID) 
	{
		case 0:													// �Ա�������
			m_pProfile->GetDepErrTotal();
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		// 
			{
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+0].Amount, iCashMoney, 10);
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+0].Count, iCount, 10);
			}
			m_pProfile->PutDepErrTotal();
			
			memcpy(&PERR.ErrType_Result[0], "�Ա�������", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);
			break;
		case 1:													// �Աݰ�����
			m_pProfile->GetDepErrTotal();
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  
			{
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+1].Amount, iCashMoney, 10);
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+1].Count, iCount, 10);
			}
			m_pProfile->PutDepErrTotal();
			memcpy(&PERR.ErrType_Result[0], "�Աݰ�����", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);
			break;
		case 2:													// �Աݼ������
			m_pProfile->GetDepErrTotal();			
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  
			{
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+2].Amount, iCashMoney, 10);
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+2].Count, iCount, 10);
			}
			m_pProfile->PutDepErrTotal();
			memcpy(&PERR.ErrType_Result[0], "�Աݼ������", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);
			break;
		case 3:													// �Աݹ̼������
			m_pProfile->GetDepErrTotal();			
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  
			{
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+3].Amount, iCashMoney, 10);
				AddString(m_pProfile->DEPERRTOTAL.DATA[nSubFlag+3].Count, iCount, 10);
			}
			m_pProfile->PutDepErrTotal();
			memcpy(&PERR.ErrType_Result[0], "�Աݹ̼�����", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);
			break;
		default:
			break;
	}

	return T_OK;
}

// ������
int CTranCmn::fnAPP_WithErrSumProc(int InputID)
{

	if (TranProc == TRAN_WITH)
		;
	else
		return T_OK;

	char	iCount[11] = { "0000000001" };
	char	iMoney[10];											// �ݾ�(����), �ݾ�(��õ��) : �ŷ�(��/��) �ѱݾ�
	char	iCashMoney[10];										// �ݾ�(����), �ݾ�(��õ��)	: �ŷ�(��/��) ���ݾ�
	char	iCheckMoney[10];									// �ݾ�(����), �ݾ�(��õ��)	: �ŷ�(��/��) ��ǥ��
	char	iCheckMotherMoneyM[10];								// �ݾ�(����)				: �ŷ�(��)    ��ǥ��
	char	iCheckMotherMoneyO[10];								// �ݾ�(����)				: �ŷ�(��)    ��ǥ��
	char	iCheckOtherMoney[10];								// �ݾ�(����)				: �ŷ�(��)    ��ǥ��
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
		sprintf(PERR.TranType_Account   , "��ݾ���  %16.16s", szTmp);
	else
		sprintf(PERR.TranType_Account   , "�����  %16.16s", szTmp);

	switch (InputID) 
	{
		case 0:													// ���������
			m_pProfile->GetWithErrTotal();
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		// 
			{
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+0].Amount, iCashMoney, 10);
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+0].Count, iCount, 10);
			}
			m_pProfile->PutWithErrTotal();
			memcpy(&PERR.ErrType_Result[0], "���������", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);
			break;
		case 1:													// ��ݰ�����
			m_pProfile->GetWithErrTotal();
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  
			{
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+1].Amount, iCashMoney, 10);
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+1].Count, iCount, 10);
			}
			m_pProfile->PutWithErrTotal();
			memcpy(&PERR.ErrType_Result[0], "��ݰ�����", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);			
			break;
		case 2:													// ��ݼ������
			m_pProfile->GetWithErrTotal();			
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  
			{
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+2].Amount, iCashMoney, 10);
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+2].Count, iCount, 10);
			}
			m_pProfile->PutWithErrTotal();
			memcpy(&PERR.ErrType_Result[0], "��ݼ������", 12);
			sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
			m_pProfile->DetailHErrStack(&PERR);			
			break;
		case 3:													// ���ݹ̼������
			m_pProfile->GetWithErrTotal();			
			if (Asc2Int(iCashMoney, sizeof(iCashMoney)))		//  
			{
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+3].Amount, iCashMoney, 10);
				AddString(m_pProfile->WITHERRTOTAL.DATA[nSubFlag+3].Count, iCount, 10);
			}
			m_pProfile->PutWithErrTotal();
			memcpy(&PERR.ErrType_Result[0], "���ݹ̼�����", 12);
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
// 1:          ** ���۾�ü�� (1) **
// 2:========================================
// 3:ATM ID   : VANCD9910
// 4:�������� : NNNNNNNN
// 5:�������� : NNNNNNNN
// 6:����Ѿ� : 
// 7:��ݰǼ� : 
// 8:����Ѿ� : 
// 9:��ҰǼ� :
//10:���۱ݾ� :
//11:��    �� :                 
//13:��ȸ�Ǽ� :
//14:��ü�Ǽ� :
//15:���۱ݾ� :
//16:��    �� :
//17:ȸ���ݾ� :
//18:���̿�Ǽ�:
//19:���̿�ݾ�:
//20:( ** )��ְǼ�:
//22:(���)��ְǼ�:
//23:(JAM) ��ְǼ�:
//24:(CST) ȸ���Ǽ�:
*****************************************************************************/
int CTranCmn::fnAPP_MiniATM_SumPrint1(int PrintDevice, int WaitFlag)
{

	return T_OK;
}


/****************************************************************************
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:          ** ���۾�ü�� (2) **
// 2:========================================
// 3:ATM ID   : VANCD9910
// 4:������������ : 
// 5:���縶������ : 
// 6:�������۱ݾ� :
// 7:          ***** �հ� (2) *****                       
// 8:��������Ѿ� : 
// 9:������ݰǼ� : 
//10:��������Ѿ� : 
//11:������ҰǼ� :
//13:
//14:
//15:
//16:
//17:
//18:
//19:
//20:
//21:
//22:�� �� �� �� �� �� �� �� �� �� ��.
//23:����ĳ���� MyCashzone ����ĳ����
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
	//�ʱ⼳���ݾ�  
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		if (nNoteKind1 == NOTE_KIND_50000_USE) //#0057
		{
			nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����
			nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
			nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
			nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);

			nDepRB1Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0); //5����
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

		//Retract �ż�
		nRBRetract = RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //�̼���
		//Reject �ż�
		nRBReject  = RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //����Ʈ - �ҷ� 
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����
		nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
		nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
		nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);

		nDepRB1Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0); //5����
		nDepRB2Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", 0);
	
		
		n50000 = 0;
		n5000  = 0;
		n1000  = 0;
		//Retract �ż�
		nRBRetract = RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //�̼���
		n5000      = RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //õ��,5õ�� - �Ա�
		//Reject �ż�
		nRBReject  = RegGetInt(_REGKEY_BRMSTATUS, "RJB2NoteCnt",0); //����Ʈ - �ҷ� 
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
		sprintf(ShuData[j++], "[ ��  �� ]                        ");		
		sprintf(ShuData[j++], "����ȣ : %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
		sprintf(ShuData[j++], "�����հ� : %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szTemp[0], &szTemp[4], &szTemp[6], &szTemp[8],&szTemp[10]);
		sprintf(ShuData[j++], "�����հ� : %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szGetDateTime[0], &szGetDateTime[4], &szGetDateTime[6], &szGetDateTime[8],&szGetDateTime[10]);
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[����ŷ�]                             ");	
		sprintf(ShuData[j++], GetSprintf("���ݹڽ�:    1     2     3     4      "));		
		sprintf(ShuData[j++], GetSprintf("����ż�: %04d  %04d  %04d  %04d      ",nRB1DispCnt,nRB2DispCnt,0,0));		
		sprintf(ShuData[j++], GetSprintf("ȸ���ż�: %04d  %04d  %04d  %04d      ",nRBRetract,0,0,0));			
		sprintf(ShuData[j++], "=======================================");	
		sprintf(ShuData[j++], "��  ������� : [%04d] %10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[7].Count,10),    &m_pProfile->WITHTOTAL.DATA[7].Amount);
		sprintf(ShuData[j++], "����    ���� : [%04d] %10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[5].Count,10),	&m_pProfile->WITHTOTAL.DATA[5].Amount);
		sprintf(ShuData[j++], "�ſ�  ���� : [%04d] %10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[9].Count,10),	&m_pProfile->WITHTOTAL.DATA[9].Amount);
		sprintf(ShuData[j++], "��ü    �ŷ� : [%04d] %10.10s", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[3].Count,10),	&m_pProfile->TRANSTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "�Ա�    �ŷ� : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPTOTAL.DATA[4].Count,10),	    &m_pProfile->DEPTOTAL.DATA[4].Amount);	
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[�Աݰŷ���]                         ");
		if (nNoteKind1 == 0x34)
		{
			sprintf(ShuData[j++], GetSprintf("   ����     ��  ��õ+õ"));		
			sprintf(ShuData[j++], GetSprintf(" %04d�� %04d��   %04d��", nDepRB1Cnt, nDepRB2Cnt, nRBReject));		
		}
		else
		{
			sprintf(ShuData[j++], GetSprintf("   ����     ��   ��õ     õ"));		
			sprintf(ShuData[j++], GetSprintf(" %04d�� %04d�� %04d�� %04d��", n50000,nDepRB1Cnt+nDepRB2Cnt+nDepRB3Cnt,n5000,n1000));		
		}
		sprintf(ShuData[j++], "=======================================");
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&��������
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{	
		sprintf(ShuData[j++], "[�հ�]                        ");		
		sprintf(ShuData[j++], "���: %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
		sprintf(ShuData[j++], "����: %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szTemp[0], &szTemp[4], &szTemp[6], &szTemp[8],&szTemp[10]);
		sprintf(ShuData[j++], "����: %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szGetDateTime[0], &szGetDateTime[4], &szGetDateTime[6], &szGetDateTime[8],&szGetDateTime[10]);

		
		nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����
		nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
		nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
		nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);

		nDepRB1Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0); //5����
		nDepRB2Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", 0);
	
		
		n50000 = 0;
		n5000  = 0;
		n1000  = 0;
		//Retract �ż�
		nRBRetract = RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //�̼���
		n5000      = RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //õ��,5õ�� - �Ա�
		//Reject �ż�
		nRBReject  = RegGetInt(_REGKEY_BRMSTATUS, "RJB2NoteCnt",0); //����Ʈ - �ҷ� 
		
		sprintf(ShuData[j++], GetSprintf("��    ��: ����(%04d)     ��(%04d)", nCurRB1Cnt , nCurRB2Cnt));
		sprintf(ShuData[j++], GetSprintf("        * ȸ��(%04d) ����Ʈ(%04d)", nRBRetract , nRBReject));

		CString cstrAtm("");
		cstrAtm = IniGetStr(_SP_BRM_CSTINFO_INI, "CURR_R_T", "TOTAL_T0GAP",  ZERO10);
		sprintf(ShuData[j++], GetSprintf("���Աݾ�:%s��", Asc2Amt(cstrAtm.GetBuffer(0) , cstrAtm.GetLength(), cstrAtm.GetLength()+3)));

		sprintf(ShuData[j++], "[����ŷ�]                             ");
		sprintf(ShuData[j++], GetSprintf("���ݹڽ�: ����    �ϸ�"));		
		sprintf(ShuData[j++], GetSprintf("����ż�: %04d    %04d      ",nRB1DispCnt,nRB2DispCnt));		
		sprintf(ShuData[j++], GetSprintf("ȸ���ż�: %04d  ",nRBRetract));
		sprintf(ShuData[j++], GetSprintf("�����ż�: %04d  ",nRBReject));
		sprintf(ShuData[j++], "=======================================");	
		sprintf(ShuData[j++], "�ѹ�:[%04d]%10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[7].Count,10),    &m_pProfile->WITHTOTAL.DATA[7].Amount);
		sprintf(ShuData[j++], "����:[%04d]%10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[5].Count,10),	&m_pProfile->WITHTOTAL.DATA[5].Amount);
		sprintf(ShuData[j++], "�ſ�:[%04d]%10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[9].Count,10),	&m_pProfile->WITHTOTAL.DATA[9].Amount);
		sprintf(ShuData[j++], "��ü:[%04d]%10.10s", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[3].Count,10),	&m_pProfile->TRANSTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "���:[%04d]%10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[8].Count,10),	&m_pProfile->WITHTOTAL.DATA[8].Amount);		
		sprintf(ShuData[j++], "�Ա�:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPTOTAL.DATA[4].Count,10),	   &m_pProfile->DEPTOTAL.DATA[4].Amount);	
		sprintf(ShuData[j++], "[�Աݰŷ���]                         ");	
		sprintf(ShuData[j++], GetSprintf("  ����     �� ��õ+õ"));
		sprintf(ShuData[j++], GetSprintf("%04d�� %04d�� %04d��", nDepRB1Cnt,nDepRB2Cnt,n5000+n1000));
		
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&�������� 

		if (PrintDevice & DEV_SPR)
			fnAPD_MaterialRetract(DEV_SPR, TRUE);
	}	
		
	//fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&��������

	for(i = 0; i < j; i++ ) 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);

	memset(ShuData, 0, sizeof(ShuData));
	j = 0;	

	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[��ְŷ����]                         ");		
		sprintf(ShuData[j++], "���    ��� : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[0].Count,10), &m_pProfile->WITHERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "��ݰ����� : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[1].Count,10), &m_pProfile->WITHERRTOTAL.DATA[1].Amount);
		sprintf(ShuData[j++], "��ݼ������ : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[2].Count,10), &m_pProfile->WITHERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "��ݹ̼���� : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[3].Count,10), &m_pProfile->WITHERRTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "��Ÿ    ��� : [%04d] %10.10s",  Asc2Int(&m_pProfile->WITHTOTAL.DATA[8].Count,10), &m_pProfile->WITHTOTAL.DATA[8].Amount);
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[��ְŷ��Ա�]                         ");		
		sprintf(ShuData[j++], "�Ա������� : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[0].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "�Աݰ����� : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[1].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[1].Amount);	
		sprintf(ShuData[j++], "�Աݼ������ : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[2].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "�Աݹ̼���� : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[3].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[3].Amount);	
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&��������
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[��ְŷ����]                         ");		
		sprintf(ShuData[j++], "���:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[0].Count,10), &m_pProfile->WITHERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "���:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[1].Count,10), &m_pProfile->WITHERRTOTAL.DATA[1].Amount);
		sprintf(ShuData[j++], "����:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[2].Count,10), &m_pProfile->WITHERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "�̼�:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[3].Count,10), &m_pProfile->WITHERRTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "���:[%04d]%10.10s",  Asc2Int(&m_pProfile->WITHTOTAL.DATA[8].Count,10), &m_pProfile->WITHTOTAL.DATA[8].Amount);
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[��ְŷ��Ա�]                         ");		
		sprintf(ShuData[j++], "���:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[0].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "���:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[1].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[1].Amount);	
		sprintf(ShuData[j++], "����:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[2].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "�̼�:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[3].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[3].Amount);	
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&��������
	}

	if (PrintDevice & DEV_SPR)
		fnAPD_MaterialRetract(DEV_SPR, TRUE);					// ��ǥȸ��



	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);

	//PrintDevice &= ~DEV_SPR;

	memset(ShuData, 0, sizeof(ShuData));
	j = 0;
	sprintf(ShuData[j++], GetSprintf("[��һ󼼳��� (�Ǽ�: %d��)]", ErrCancelNum));	
	if (ErrCancelNum)
	{															// ������� �ֽ�
		for (int i = 0; i < ErrCancelNum; i++)
		{			
			if ((i) && (i == ErrCancelNum))						
				break;											

			// �ŷ����� - ó����ȣ - �ݾ�
			sprintf(ShuData[j++], "%02d|%24.24s", i, &ErrCancel[i].Remark[0]);
			sprintf(ShuData[j++], "**|%29.29s",      &ErrCancel[i].Remark[51]);

			if (j > 20)
			{
				fnAPD_ShuPrint(PrintDevice, JNL_CLOSE, TRUE);				// ��ǥ&��������

				//#CS0002
				if (PrintDevice & DEV_SPR)
					fnAPD_MaterialRetract(DEV_SPR, TRUE);					// ��ǥȸ��

				memset(ShuData, 0, sizeof(ShuData));
				j = 0;
			}
		}
	}
	else
	{
		sprintf(ShuData[j++], "��ҳ���       : ����");
		sprintf(ShuData[j++], "��ҳ��� �Ǽ�  : 0000");
	}	
		

	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);
	
/////////////////////////////////////////////////////////////////////////////
	fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&��������
	if (PrintDevice & DEV_SPR)
	{
		fnAPD_MaterialRetract(DEV_SPR, TRUE);					// ��ǥȸ��
	}


	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);

	//PrintDevice &= ~DEV_SPR;

	memset(ShuData, 0, sizeof(ShuData));
	j = 0;
	sprintf(ShuData[j++], GetSprintf("[��ֻ󼼳��� (�Ǽ�: %d��)]", DetailHNum));	
	if (DetailHNum)
	{															// ������� �ֽ�
		for (int i = 0; i < DetailHNum; i++)
		{			
			if ((i) && (i == DetailHNum))						
				break;											

			// �ŷ����� - �ŷ����� - �������
			memcpy(ShuData[j++], DetailErrTran[i].DateTime_Serial_Amt, strlen(DetailErrTran[i].DateTime_Serial_Amt));
			memcpy(ShuData[j++], DetailErrTran[i].TranType_Account,    strlen(DetailErrTran[i].TranType_Account));
			memcpy(ShuData[j++], DetailErrTran[i].ErrType_Result,      strlen(DetailErrTran[i].ErrType_Result));

			if (j > 20)
			{
				fnAPD_ShuPrint(PrintDevice, JNL_CLOSE, TRUE);				// ��ǥ&��������

				//#CS0002
				if (PrintDevice & DEV_SPR)
					fnAPD_MaterialRetract(DEV_SPR, TRUE);					// ��ǥȸ��

				memset(ShuData, 0, sizeof(ShuData));
				j = 0;
			}
		}
	}
	else
	{
		sprintf(ShuData[j++], "��ֳ���       : ����");
		sprintf(ShuData[j++], "��ֳ��� �Ǽ�  : 0000");
	}	

		
	sprintf(ShuData[j++], "ȸ���ż�       : %4.4d��",   RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt"));
	sprintf(ShuData[j++], "REJECT         : %4.4d��",   RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt"));
//	sprintf(ShuData[j++], "( ** )��ְǼ� : %010d",	ErrCancelNum);

	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);
	
/////////////////////////////////////////////////////////////////////////////
	fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&��������
	if (PrintDevice & DEV_SPR)
	{
		fnAPD_MaterialRetract(DEV_SPR, TRUE);					// ��ǥȸ��
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
	sprintf(ShuData[j++], "%10.10s** ���� ���� �ݾ� **", "");		// 2005.03.16
	sprintf(ShuData[j++], "========================================");
 	sprintf(ShuData[j++], "ATM SST      : %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
	j++;
	sprintf(ShuData[j++], "���缳������ : %4.4s-%2.2s-%2.2s %2.2s:%2.2s", &szGetDateTime[0], &szGetDateTime[4], &szGetDateTime[6], &szGetDateTime[8], &szGetDateTime[10]);
	j++;

	sprintf(ShuData[j++], "1 ī��Ʈ ����:  ������ %d��",m_pDevCmn->FstCSTSetCash);
	sprintf(ShuData[j++], "2 ī��Ʈ ����:  ������ %d��",m_pDevCmn->SndCSTSetCash);
	sprintf(ShuData[j++], "3 ī��Ʈ ����:  ������ %d��",m_pDevCmn->TrdCSTSetCash);
	sprintf(ShuData[j++], "4 ī��Ʈ ����:  ������ %d��",m_pDevCmn->FthCSTSetCash);
	sprintf(ShuData[j++], "TOTAL    �ݾ�:  ������ %d��",m_pDevCmn->FstCSTSetCash + m_pDevCmn->SndCSTSetCash);

	j++;
	j++;
/////////////////////////////////////////////////////////////////////////////
	sprintf(ShuData[j++], "�� �� �� �� �� �� �� �� �� �� ��.");										
/////////////////////////////////////////////////////////////////////////////
	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_SumPrint3| %s ", ShuData[i]);
/////////////////////////////////////////////////////////////////////////////
	fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&��������
	if (PrintDevice & DEV_SPR)
	{
		fnAPD_MaterialRetract(DEV_SPR, TRUE);					// ��ǥȸ��
	}


	return T_OK;
}
/****************************************************************************
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:          ** ���۾�ü�� (1) **
// 2:========================================
// 3:ATM ID   : VANCD9910
// 4:������������ : 
// 5:�߰��������� : 
// 6:�������۱ݾ� :
// 7:          ***** �߰����� (1) *****                       
// 8:�߰������Ѿ� : 
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
//22:�� �� �� �� �� �� �� �� �� �� ��.
//23:����ĳ���� MyCashzone ����ĳ����
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
	sprintf(ShuData[j++], "%10.10s** CD����Ȯ������ **", "");		// 2005.03.16
	sprintf(ShuData[j++], "========================================");
 	sprintf(ShuData[j++], "ATM ID       : %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
	j++;
	sprintf(ShuData[j++], "���缳������ : %4.4s-%2.2s-%2.2s %2.2s:%2.2s", &szGetDateTime[0], &szGetDateTime[4], &szGetDateTime[6], &szGetDateTime[8], &szGetDateTime[10]);
	j++;
/*
	sprintf(ShuData[j++], "1 ī��Ʈ ����:  %14.14s",m_RXATM0330.cst1);
	sprintf(ShuData[j++], "2 ī��Ʈ ����:  %14.14s",m_RXATM0330.cst2);
	sprintf(ShuData[j++], "TOTAL    �ݾ�:  %14.14s",m_RXATM0330.csttot);
*/
	j++;
	j++;
/////////////////////////////////////////////////////////////////////////////
	sprintf(ShuData[j++], "�� �� �� �� �� �� �� �� �� �� ��.");										
	sprintf(ShuData[j++], "����ĳ���� MyCashzone ����ĳ����");
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_SumPrint| %s ", ShuData[i]);

	fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&��������
	if (PrintDevice & DEV_SPR)
	{
		fnAPD_MaterialRetract(DEV_SPR, TRUE);					// ��ǥȸ��
	}
	return T_OK;
}

/****************************************************************************
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:          ** ���۾�ü�� (1) **
// 2:========================================
// 3:ATM ID   : VANCD9910
// 4:�������� : NNNNNNNN
// 5:�������� : NNNNNNNN
// 6:����Ѿ� : 
// 7:��ݰǼ� : 
// 8:����Ѿ� : 
// 9:��ҰǼ� :
//10:���۱ݾ� :
//11:��    �� :                 
//12:��ȸ�Ǽ� :
//13:��ü�Ǽ� :
//14:���۱ݾ� :
//15:��    �� :
//16:ȸ���ݾ� :
//17���̿�Ǽ�:
//18:���̿�ݾ�:
//19:( ** )��ְǼ�:
//20:(���)��ְǼ�:
//21:(JAM) ��ְǼ�:
//22:(CST) ȸ���Ǽ�:
//23:(CST) ȸ���Ǽ�:
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
	//�ʱ⼳���ݾ�  
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		if (nNoteKind1 == NOTE_KIND_50000_USE)  //#0057
		{
			nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����
			nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
			nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
			nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);

			nDepRB1Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0); //5����
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

		//Retract �ż�
		nRBRetract = RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //�̼���
		//Reject �ż�
		nRBReject  = RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //����Ʈ - �ҷ� 
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����
		nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
		nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);
		nCurRB2Cnt += RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt", 0);

		nDepRB1Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB1NoteCnt_50000", 0); //5����
		nDepRB2Cnt  = RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "RB2NoteCnt_10000", 0);
	
		
		n50000 = 0;
		n5000  = 0;
		n1000  = 0;
		//Retract �ż�
		nRBRetract = RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt",0); //�̼���
		n5000      = RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt",0); //õ��,5õ�� - �Ա�
		//Reject �ż�
		nRBReject  = RegGetInt(_REGKEY_BRMSTATUS, "RJB2NoteCnt",0); //����Ʈ - �ҷ� 

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
		sprintf(ShuData[j++], "[ ��  ��  ]                        ");		
		sprintf(ShuData[j++], "����ȣ : %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
		sprintf(ShuData[j++], "�����հ� : %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szTemp[0], &szTemp[4], &szTemp[6], &szTemp[8],&szTemp[10]);
		sprintf(ShuData[j++], "�����հ� : %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szGetDateTime[0], &szGetDateTime[4], &szGetDateTime[6], &szGetDateTime[8],&szGetDateTime[10]);
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[����ŷ�]                             ");	
		sprintf(ShuData[j++], GetSprintf("���ݹڽ�:    1     2     3     4      "));		
		sprintf(ShuData[j++], GetSprintf("����ż�: %04d  %04d  %04d  %04d      ",nRB1DispCnt,nRB2DispCnt,0,0));		
		sprintf(ShuData[j++], GetSprintf("ȸ���ż�: %04d  %04d  %04d  %04d      ",nRBRetract,0,0,0));			
		sprintf(ShuData[j++], "=======================================");	
		sprintf(ShuData[j++], "��  ������� : [%04d] %10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[7].Count,10),    &m_pProfile->WITHTOTAL.DATA[7].Amount);
		sprintf(ShuData[j++], "����    ���� : [%04d] %10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[5].Count,10),	&m_pProfile->WITHTOTAL.DATA[5].Amount);
		sprintf(ShuData[j++], "�ſ�  ���� : [%04d] %10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[9].Count,10),	&m_pProfile->WITHTOTAL.DATA[9].Amount);
		sprintf(ShuData[j++], "��ü    �ŷ� : [%04d] %10.10s", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[3].Count,10),	&m_pProfile->TRANSTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "�Ա�    �ŷ� : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPTOTAL.DATA[4].Count,10),	    &m_pProfile->DEPTOTAL.DATA[4].Amount);	
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[�Աݰŷ���]                         ");
		if (nNoteKind1 == 0x34)
		{
			sprintf(ShuData[j++], GetSprintf("   ����     ��  ��õ+õ"));		
			sprintf(ShuData[j++], GetSprintf(" %04d�� %04d��   %04d��", nDepRB1Cnt, nDepRB2Cnt, nRBReject));		
		}
		else
		{
			sprintf(ShuData[j++], GetSprintf("   ����     ��   ��õ     õ"));		
			sprintf(ShuData[j++], GetSprintf(" %04d�� %04d�� %04d�� %04d��", n50000,nDepRB1Cnt+nDepRB2Cnt+nDepRB3Cnt,n5000,n1000));		
		}
		sprintf(ShuData[j++], "=======================================");
		//#N0258
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&��������
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{	
		sprintf(ShuData[j++], "[ ��  ��  ]                        ");		
		sprintf(ShuData[j++], "���: %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
		sprintf(ShuData[j++], "����: %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szTemp[0], &szTemp[4], &szTemp[6], &szTemp[8],&szTemp[10]);
		sprintf(ShuData[j++], "����: %4.4s/%2.2s/%2.2s-%2.2s:%2.2s",&szGetDateTime[0], &szGetDateTime[4], &szGetDateTime[6], &szGetDateTime[8],&szGetDateTime[10]);
		sprintf(ShuData[j++], "[����ŷ�]                             ");	
		sprintf(ShuData[j++], GetSprintf("���ݹڽ�: ����    �ϸ�"));		
		sprintf(ShuData[j++], GetSprintf("����ż�: %04d    %04d      ",nRB1DispCnt,nRB2DispCnt));		
		sprintf(ShuData[j++], GetSprintf("ȸ���ż�: %04d  ",nRBRetract));
		sprintf(ShuData[j++], GetSprintf("�����ż�: %04d  ",nRBReject));
		sprintf(ShuData[j++], "=======================================");	
		sprintf(ShuData[j++], "�ѹ�:[%04d]%10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[7].Count,10),    &m_pProfile->WITHTOTAL.DATA[7].Amount);
		sprintf(ShuData[j++], "����:[%04d]%10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[5].Count,10),	&m_pProfile->WITHTOTAL.DATA[5].Amount);
		sprintf(ShuData[j++], "�ſ�:[%04d]%10.10s", Asc2Int(&m_pProfile->WITHTOTAL.DATA[9].Count,10),	&m_pProfile->WITHTOTAL.DATA[9].Amount);
		sprintf(ShuData[j++], "��ü:[%04d]%10.10s", Asc2Int(&m_pProfile->TRANSTOTAL.DATA[3].Count,10),	&m_pProfile->TRANSTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "�Ա�:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPTOTAL.DATA[4].Count,10),	    &m_pProfile->DEPTOTAL.DATA[4].Amount);	
		sprintf(ShuData[j++], "[�Աݰŷ���]                         ");	
		sprintf(ShuData[j++], GetSprintf("  ����     �� ��õ+õ"));
		sprintf(ShuData[j++], GetSprintf("%04d�� %04d�� %04d��", nDepRB1Cnt,nDepRB2Cnt,n5000+n1000));
		//#N0258
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&�������� 
		if (PrintDevice & DEV_SPR)
			fnAPD_MaterialRetract(DEV_SPR, TRUE);
	}	
		
//	fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&��������

	for(i = 0; i < j; i++ ) 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);

	memset(ShuData, 0, sizeof(ShuData));
	j = 0;	

	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[��ְŷ����]                         ");		
		sprintf(ShuData[j++], "���    ��� : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[0].Count,10), &m_pProfile->WITHERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "��ݰ����� : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[1].Count,10), &m_pProfile->WITHERRTOTAL.DATA[1].Amount);
		sprintf(ShuData[j++], "��ݼ������ : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[2].Count,10), &m_pProfile->WITHERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "��ݹ̼���� : [%04d] %10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[3].Count,10), &m_pProfile->WITHERRTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "��Ÿ    ��� : [%04d] %10.10s",  Asc2Int(&m_pProfile->WITHTOTAL.DATA[8].Count,10), &m_pProfile->WITHTOTAL.DATA[8].Amount);
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[��ְŷ��Ա�]                         ");		
		sprintf(ShuData[j++], "�Ա������� : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[0].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "�Աݰ����� : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[1].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[1].Amount);	
		sprintf(ShuData[j++], "�Աݼ������ : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[2].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "�Աݹ̼���� : [%04d] %10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[3].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[3].Amount);	
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&��������
	}
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
	{
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[��ְŷ����]                         ");		
		sprintf(ShuData[j++], "���:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[0].Count,10), &m_pProfile->WITHERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "���:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[1].Count,10), &m_pProfile->WITHERRTOTAL.DATA[1].Amount);
		sprintf(ShuData[j++], "����:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[2].Count,10), &m_pProfile->WITHERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "�̼�:[%04d]%10.10s",	Asc2Int(&m_pProfile->WITHERRTOTAL.DATA[3].Count,10), &m_pProfile->WITHERRTOTAL.DATA[3].Amount);
		sprintf(ShuData[j++], "���:[%04d]%10.10s",  Asc2Int(&m_pProfile->WITHTOTAL.DATA[8].Count,10), &m_pProfile->WITHTOTAL.DATA[8].Amount);
		sprintf(ShuData[j++], "=======================================");
		sprintf(ShuData[j++], "[��ְŷ��Ա�]                         ");		
		sprintf(ShuData[j++], "���:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[0].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[0].Amount);
		sprintf(ShuData[j++], "���:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[1].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[1].Amount);	
		sprintf(ShuData[j++], "����:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[2].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[2].Amount);
		sprintf(ShuData[j++], "�̼�:[%04d]%10.10s", Asc2Int(&m_pProfile->DEPERRTOTAL.DATA[3].Count,10),	&m_pProfile->DEPERRTOTAL.DATA[3].Amount);	
		fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&��������
	}

	if (PrintDevice & DEV_SPR)
		fnAPD_MaterialRetract(DEV_SPR, TRUE);					// ��ǥȸ��


	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);

	PrintDevice &= ~DEV_SPR;

	memset(ShuData, 0, sizeof(ShuData));
	j = 0;
	sprintf(ShuData[j++], GetSprintf("[��ֻ󼼳��� (�Ǽ�: %d��)]", DetailHNum));	
	if (DetailHNum)
	{															// ������� �ֽ�
		for (int i = 0; i < DetailHNum; i++)
		{			
			if ((i) && (i == DetailHNum))						
				break;											

			// �ŷ����� - �ŷ����� - �������
			memcpy(ShuData[j++], DetailErrTran[i].DateTime_Serial_Amt, strlen(DetailErrTran[i].DateTime_Serial_Amt));
			memcpy(ShuData[j++], DetailErrTran[i].TranType_Account,    strlen(DetailErrTran[i].TranType_Account));
			memcpy(ShuData[j++], DetailErrTran[i].ErrType_Result,      strlen(DetailErrTran[i].ErrType_Result));

			if (j > 21)
			{
				fnAPD_ShuPrint(PrintDevice, JNL_CLOSE, TRUE);				// ��ǥ&��������
				memset(ShuData, 0, sizeof(ShuData));
				j = 0;
			}
		}
	}
	else
	{
		sprintf(ShuData[j++], "��ֳ���       : ����");
		sprintf(ShuData[j++], "��ֳ��� �Ǽ�  : 0000");
	}	

	
	
	sprintf(ShuData[j++], "ȸ���ż�       : %4.4d��",   RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt"));
	sprintf(ShuData[j++], "REJECT         : %4.4d��",   RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt"));
//	sprintf(ShuData[j++], "( ** )��ְǼ� : %010d",	DetailHNum);

	for(i = 0; i < j; i++ )
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MiniATM_CashPrint| %s ", ShuData[i]);
	
/////////////////////////////////////////////////////////////////////////////
	fnAPD_ShuPrint(PrintDevice, JNL_OPER, TRUE);				// ��ǥ&��������
	if (PrintDevice & DEV_SPR)
	{
		fnAPD_MaterialRetract(DEV_SPR, TRUE);					// ��ǥȸ��
	}

	return T_OK;
}



// �հ�������
int CTranCmn::fnAPP_SumPrintResult(int TranId, int PrintDevice, int WaitFlg)
{
	int		HiddenFlg = FALSE;									// ��ɻ���
	int		ll = 0;

	memset(ShuData, 0, sizeof(ShuData));						// Shu��¿���

	return T_OK;
}

// ����������� Coding�ʿ�
int CTranCmn::fnAPP_ClosePrintResult(int TranId, int PrintDevice, int WaitFlg)
{
	int		HiddenFlg = FALSE;									// ��ɻ���
	int		ll = 0;

	if (!AddSerialFlg)											// �Ϸù�ȣ������
	{
		AddSerialFlg = TRUE;									// �Ϸù�ȣ������
		m_pDevCmn->fnAPL_AddSerialNo();							// �ŷ��Ϸù�ȣ����
	}

	m_pProfile->GetTransProfile();
	m_pProfile->TRANS.MagamFlg = ON;
	m_pProfile->PutTransProfile();

	memset(ShuData, 0, sizeof(ShuData));						// Shu��¿���

	return T_OK;
}

// ��ǥ�������
int	CTranCmn::fnAPP_CheckResultPrint(int PrintDevice, int nFlag, int WaitFlg)
{
	int		HiddenFlg = FALSE;									// ��ɻ���
	int		ll = 0;

	memset(ShuData, 0, sizeof(ShuData));						// Shu��¿���

	if (m_pDevCmn->TranStatus == TRAN_CHECK_REG)
		sprintf(ShuData[ll++], "        ��ǥ���");					// 1����
	else
		sprintf(ShuData[ll++], "        ��ǥ����");					// 1���� 

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
		sprintf(ShuData[ll++], "��ǥ��� ������");
		sprintf(ShuData[ll++], "��ǥ���۹�ȣ,�ż� Ȯ����");
		sprintf(ShuData[ll++], "�ٽ� ����Ͽ� �ֽʽÿ�");
		if (PrintDevice & DEV_SPR)
			ll += 3;
	}
	else
	{
		sprintf(ShuData[ll++], "�����ڵ�1 : %2.2s", m_pDevCmn->fnAPL_GetOutCheckBankNo().GetBuffer(0));
		sprintf(ShuData[ll++], "��ǥ1 ��ȣ: %8.8s", m_pDevCmn->fnAPL_GetNumberOfOutCheckNo().GetBuffer(0));
		sprintf(ShuData[ll++], "��ǥ1 �ż�: %04d", m_pDevCmn->fnAPL_GetNumberOfOutCheck());
		sprintf(ShuData[ll++], "�����ڵ�2 : %2.2s", m_pDevCmn->fnAPL_GetOutCheckBankNo2().GetBuffer(0));
		sprintf(ShuData[ll++], "��ǥ2 ��ȣ: %8.8s", m_pDevCmn->fnAPL_GetNumberOfOutCheckNo2().GetBuffer(0));
		sprintf(ShuData[ll++], "��ǥ2 �ż�: %04d", m_pDevCmn->fnAPL_GetNumberOfOutCheck2());
	}
	if (PrintDevice & DEV_SPR)
	{
		ll += 4;
		fnAPP_EditPrintLine(PrintDevice, ShuData[ll++], 17, HiddenFlg);	// 18���� 
		fnAPP_EditPrintLine(PrintDevice, ShuData[ll++], 18, HiddenFlg);	// 19���� 
	}

	fnAPD_ShuPrint(PrintDevice, JNL_CASH, TRUE);							// ��ǥ&��������
	if (PrintDevice & DEV_SPR)
	{
		m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);				// ��ǥȸ��
	}

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// �ŷ������������
int CTranCmn::fnAPP_ErrorTranProc()
{
//	  0123456789012345678900123456789012345|0123456789012345678900123456789012345|012345678901234567890012345678901234567890
//    12051324  123456 01000000            |������� 1234567890123456            |���ݹ��� LC[AB] 400AB(06)     
	char szAmt[9];
	char szTmp[17];
	CString cstrGetErr("");
	DERRINFO	ERR;

	if ((TranProc == TRAN_WITH) ||								// ���
		(TranProc == TRAN_DEP)	)								// �Ա�
		;
	else 
		return T_OK;

	StrAllSpace(ERR.Remark);
	ERR.Remark[25] = '|';
    ERR.Remark[50] = '|';
    ERR.Remark[76] = ' ';

	Sprintf(szAmt, 8, "%08d", Asc2Int(m_pTranCmn->Accept.Money, sizeof(m_pTranCmn->Accept.Money)));		
	
	memcpy(ERR.Remark,     &m_pProfile->TRANS.YYYYMMDD[4], 4);
	memcpy(&ERR.Remark[4], GetTime().GetBuffer(0), 4);          // �ŷ��ð�
	memcpy(&ERR.Remark[4+4+1], m_pProfile->TRANS.SerialNo, 6);		// �Ϸù�ȣ
	memcpy(&ERR.Remark[4+4+1+6+1], szAmt, strlen(szAmt));  			// �ŷ��ݾ�

	if(TranProc == TRAN_DEP)
		memcpy(&ERR.Remark[25+1], "�Աݾ���", 8);  			// �ŷ�����
	else
	if(TranProc == TRAN_WITH)
		memcpy(&ERR.Remark[25+1], "��ݾ���", 8);  			// �ŷ�����
	else
		memcpy(&ERR.Remark[25+1], "��Ÿ����", 8);  			// 


	EditString(&m_pTranCmn->Accept.AccountNum, 14, "$$#####$$$$$##", szTmp, '#', '$', '*', TRUE);

	memcpy(&ERR.Remark[26+1+8+1], szTmp, 14);
	
	if (!m_pDevCmn->fnAPL_CheckError())							// ���
	{
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "011", 3) == 0)
		{
			if ((m_pDevCmn->CurErrBuff.ErrorCode[4] == '2')	||
				(m_pDevCmn->CurErrBuff.ErrorCode[4] == '4')	)
				memcpy(&ERR.Remark[50+1], "���ݹ���", 8);
			else
				memcpy(&ERR.Remark[50+1], "��ü����", 8);
		}
		else
		if ((memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "77", 2) == 0) ||
			(memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "88", 2) == 0))
			memcpy(&ERR.Remark[50+1], "ȸ�����", 8);
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "5", 1) == 0)
		{
			memcpy(&ERR.Remark[50+1], "����ݺ�", 8);
		}
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "95", 1) == 0)
		{
			memcpy(&ERR.Remark[50+1], "����ݺ�", 8);
		}
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "97", 2) == 0)
			memcpy(&ERR.Remark[50+1], "�������", 8);
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "30", 2) == 0)
			memcpy(&ERR.Remark[50+1], "ī���", 6);		
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "20", 2) == 0)
			memcpy(&ERR.Remark[50+1], "��ǥ��", 8);
		else
			memcpy(&ERR.Remark[50+1], "������", 8);

		cstrGetErr = GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]);

		memcpy(&ERR.Remark[51+8+1], cstrGetErr.GetBuffer(0), cstrGetErr.GetLength());
	}

	m_pProfile->ErrTran(&ERR);								// �����������

	return T_OK;
}

// ��������������
int CTranCmn::fnAPP_ErrorCancelProc()
{
//	  0123456789012345678900123456789012345|0123456789012345678900123456789012345|012345678901234567890012345678901234567890
//    12051324  123456 01000000            |������� 1234567890123456            |���ݹ��� LC[AB] 400AB(06)     
	
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
	memcpy(&ERR.Remark[4], GetTime().GetBuffer(0), 4);          // �ŷ��ð�
	memcpy(&ERR.Remark[4+4+1], m_pProfile->TRANS.SerialNo, 6);		// �Ϸù�ȣ
	memcpy(&ERR.Remark[4+4+1+6+1], szAmt, strlen(szAmt));  			// �ŷ��ݾ�

	if(TranProc == TRAN_DEP)
		memcpy(&ERR.Remark[25+1], "�Աݾ���", 8);  			// �ŷ�����
	else
	if(TranProc == TRAN_WITH)
		memcpy(&ERR.Remark[25+1], "��ݾ���", 8);  			// �ŷ�����
	else
		memcpy(&ERR.Remark[25+1], "��Ÿ����", 8);  			// 

	EditString(&m_pTranCmn->Accept.AccountNum, 14, "$$#####$$$$$##", szTmp, '#', '$', '*', TRUE);

	memcpy(&ERR.Remark[26+1+8+1], szTmp, 14);
	
	if (!m_pDevCmn->fnAPL_CheckError())							// ���
	{
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "011", 3) == 0)
		{
			if ((m_pDevCmn->CurErrBuff.ErrorCode[4] == '2')	||
				(m_pDevCmn->CurErrBuff.ErrorCode[4] == '4')	)
				memcpy(&ERR.Remark[50+1], "���ݹ���", 8);
			else
				memcpy(&ERR.Remark[50+1], "��ü����", 8);
		}
		else
		if ((memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "77", 2) == 0) ||
			(memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "88", 2) == 0))
			memcpy(&ERR.Remark[50+1], "ȸ�����", 8);
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "5", 1) == 0)
		{
			memcpy(&ERR.Remark[50+1], "����ݺ�", 8);
		}
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "95", 1) == 0)
		{
			memcpy(&ERR.Remark[50+1], "����ݺ�", 8);
		}
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "97", 2) == 0)
			memcpy(&ERR.Remark[50+1], "�������", 8);
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "30", 2) == 0)
			memcpy(&ERR.Remark[50+1], "ī���", 6);		
		else
		if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "20", 2) == 0)
			memcpy(&ERR.Remark[50+1], "��ǥ��", 8);
		else
			memcpy(&ERR.Remark[50+1], "������", 8);

		cstrGetErr = GetSprintf("LC[%s] %5.5s(%2.2s)", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]);

		memcpy(&ERR.Remark[51+8+1], cstrGetErr.GetBuffer(0), cstrGetErr.GetLength());
	}

	if (m_pDevCmn->TranResult)									// �������
		memcpy(&ERR.Remark[76+1], "OK", 2);
	else
		memcpy(&ERR.Remark[76+1], "NG", 2);

	m_pProfile->ErrCancel(&ERR);								// ��������������


	StrAllSpace(PERR);
	sprintf(PERR.DateTime_Serial_Amt   , "%2.2s/%2.2s-%2.2s:%2.2s-%6.6s  %8.8s", &GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6], &GetTime().GetBuffer(0)[0], &GetTime().GetBuffer(0)[2], m_pProfile->TRANS.SerialNo, szAmt);
	if(TranProc == TRAN_WITH)
		sprintf(PERR.TranType_Account   , "��ݾ��� %16.16s", szTmp);
	else
	if(TranProc == TRAN_DEP)
		sprintf(PERR.TranType_Account   , "�Աݾ��� %16.16s", szTmp);
	else
		sprintf(PERR.TranType_Account   , "��Ÿ���� %16.16s", szTmp);

	

	memcpy(&PERR.ErrType_Result, &ERR.Remark[50+1], 8);
	
	if (m_pDevCmn->TranResult)							// HOST OK
		sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s) - OK", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
	else
		sprintf(&PERR.ErrType_Result[14], GetSprintf("LC[%s] %5.5s(%2.2s) - NG", m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode), m_pDevCmn->CurErrBuff.ErrorCode, &m_pDevCmn->CurErrBuff.ErrorCode[5]));
	m_pProfile->DetailHErrStack(&PERR);

	return T_OK;
}

// ȣ��Ʈ�����������
int CTranCmn::fnAPP_ErrorHostProc()
{
	return T_OK;
}

// KCash������������
int CTranCmn::fnAPP_ErrorKCashProc()
{
//	  0123456789012345678901	: 22 Col
//    SSS���� \12,345,678 OK	: OK/NG
	
	DERRINFO	ERR;
	char	StrBuff[81];

	if (TranProc == TRAN_IC)									// KCASH�ŷ�
		;
	else 
		return T_OK;

	StrAllSpace(ERR.Remark);
	memcpy(ERR.Remark, &m_pProfile->TRANS.SerialNo[0], 6);		// �Ϸù�ȣ

	if (TranCode == TC_KCLOADRCV)
		memcpy(&ERR.Remark[6], "����", 4);
	else
	if (TranCode == TC_KCUNLOADRCV)
		memcpy(&ERR.Remark[6], "ȯ��", 4);
	else
		return T_OK;											// �ٸ� �ŷ��� ó������ �ʴ´�.

	memcpy(StrBuff, Asc2Amt(Accept.Money, 8, 11), 11);			// �ŷ��ݾ�(Space(1) + �ݾ�(11))
	MarkAmount(StrBuff, 11, '\\');
	memcpy(&ERR.Remark[10], " ", 1);
	memcpy(&ERR.Remark[11], StrBuff, 11);

	memcpy(&ERR.Remark[22], " ", 1);							// SPACE(1)
	if (m_pDevCmn->TranResult)									// �������
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

	m_pProfile->ErrKCash(&ERR);									// KCash������������
	return T_OK;
}
/////////////////////////////////////////////////////////////////////////////
// �������ó��
int	CTranCmn::fnAPP_PrintError(int PrintDevice, int WaitFlg)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_PrintError");
	int		HiddenFlg = TRUE;									// ��ɻ���
	int		nSlipIndex = 0, nJnlIndex = 0;						// ��ǥ/���� �����ε���
	CString	strTemp("");
	int		nTempMoney = 0;
	int		ii = 0;

	if (MaterialOutFlg & DEV_SPR)								// ����/��ǥ���ڹ׹���Ϸ�
	{
		if (PrintDevice & DEV_SPR)								// ��ǥ �������� ����
			PrintDevice &= ~DEV_SPR;
	}

	if (PrintDevice == DEV_NONE)								// ���ڵ���̽� Ȯ��
		return T_OK;

	if (PrintDevice & DEV_JPR) 
	{
		HiddenFlg = FALSE;										// ���θ����ڽ� ��ɻ���
	}

	//#N0203 ��� ���� �α� �� �Ϸù�ȣ ���� ����.
	/*
	if (!AddSerialFlg)											// �Ϸù�ȣ������  
	{
		AddSerialFlg = TRUE;									// �Ϸù�ȣ������ -> #0059
		m_pDevCmn->fnAPL_AddSerialNo();							// �ŷ��Ϸù�ȣ����
	}
	*/

	memset(ShuData, 0, sizeof(ShuData));						// Shu��¿���
	nSlipIndex = 0;

	for (ii = 0; ii < 50; ii++)
	{
		fnAPP_EditPrintLine(PrintDevice, ShuData[nSlipIndex], ii, HiddenFlg);		// ���� Edit
		if (ShuData[nSlipIndex][0] != 0)
			nSlipIndex++;
	}

	nSlipIndex = 15;											// ��ֽ� ���ڿ���
	if (!m_pDevCmn->fnAPL_CheckError())							// ��ְ�����
	{
		ShuData[nSlipIndex][0] = m_pDevCmn->CurErrBuff.ProcCount[0];
		ShuData[nSlipIndex][1] = '-';

		// ����ڵ�
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
		(!strTemp.CompareNoCase("0110295"))		)				// �۱�����ȸ��
	{
		sprintf(ShuData[nSlipIndex++], "�ֹι�ȣ ����ġ�� ���� ȸ��");
	}
	else
	if ((TranProc == TRAN_DEP)					&&				// 2007-01-03-#05 ��ġ�̵�
		((!strTemp.CompareNoCase("0110050"))	||				// ���ݹ̼���
		 (!strTemp.CompareNoCase("0110040")))	)				// ��ǥ�̼����϶��� ����
	{
		sprintf(ShuData[nSlipIndex++], "[�ܾ���ȸ ���] �Աݺ� �ܷ�");
	}
	else
	if (TranProc == TRAN_DEP)
	{
		if (CashData.mTotalAmount)
			sprintf(ShuData[nSlipIndex++], "[�ܾ���ȸ ���]  �Աݺ� ����ó��");
		else sprintf(ShuData[nSlipIndex++], "[�ܾ���ȸ ���]");
	}
	else
	if (EMVAuthError)
	{								  
		sprintf(ShuData[nSlipIndex++], "EMV �������� �߻� â������ ���");
	}

	if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "0110", 4) == 0)
		fnAPD_ShuPrint(PrintDevice, JNL_STATUS, WaitFlg);
//		fnAPD_ShuPrint(PrintDevice, JNL_OPER, WaitFlg);
	else
		fnAPD_ShuPrint(PrintDevice, JNL_ERROR, WaitFlg);

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// �����Ұŷ�����ó��
int	CTranCmn::fnAPP_PrintWithCancelTran(int PrintDevice, int WaitFlg)
{
	int		HiddenFlg = TRUE;									// ��ɻ���
	int		nSlipIndex = 0, nJnlIndex = 0;						// ��ǥ/���� �����ε���
	CString	StrTemp("");
	int		ii = 0;

	if (MaterialOutFlg & DEV_SPR)								// ����/��ǥ���ڹ׹���Ϸ�
	{
		if (PrintDevice & DEV_SPR)								// ��ǥ �������� ����
			PrintDevice &= ~DEV_SPR;
	}

	if (PrintDevice == DEV_NONE)								// ���ڵ���̽� Ȯ��
		return T_OK;

	if (PrintDevice & DEV_JPR) 
	{
		HiddenFlg = FALSE;										// ���θ����ڽ� ��ɻ���
	}

//	if (!AddSerialFlg)											// �Ϸù�ȣ������
//	{
//		AddSerialFlg = TRUE;									// �Ϸù�ȣ������
//		m_pDevCmn->fnAPL_AddSerialNo();							// �ŷ��Ϸù�ȣ����
//	}

	memset(ShuData, 0, sizeof(ShuData));						// Shu��¿���
	nSlipIndex = 0;

	for (ii = 0; ii < 13; ii++)
	{
		fnAPP_EditPrintLine(PrintDevice, ShuData[nSlipIndex], ii, HiddenFlg);		// ���� Edit
		if (ShuData[nSlipIndex][0] != 0)
			nSlipIndex++;
	}

	StrTemp = m_pDevCmn->fnAPL_GetLC(m_pDevCmn->CurErrBuff.ErrorCode);

	// ��ֳ���
	if (m_pDevCmn->TranResult)
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			sprintf(ShuData[nSlipIndex++], "��Ұŷ� OK");
		else
			sprintf(ShuData[nSlipIndex++], "Cancel OK");
	}
	else
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			sprintf(ShuData[nSlipIndex++], "��Ұŷ� NG");
		else
			sprintf(ShuData[nSlipIndex++], "Cancel NG");

	}

	// ����ڵ�
	ShuData[nSlipIndex][0] = m_pDevCmn->CurErrBuff.ProcCount[0];
	ShuData[nSlipIndex][1] = '-';
	memcpy(&ShuData[nSlipIndex][2], StrTemp.Left(2).GetBuffer(0), 2);
	Sprintf(&ShuData[nSlipIndex][4], 9, "(%s)", m_pDevCmn->CurErrBuff.ErrorCode);
	memcpy(&ShuData[nSlipIndex][13], "  ", 2);
	
	if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "011", 3) == 0)
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			sprintf(&ShuData[nSlipIndex++][15], "[��ü�ܷ�]");
		else
			sprintf(&ShuData[nSlipIndex++][15], "[Remain material]");
	}
	else
	{
		if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
			sprintf(&ShuData[nSlipIndex++][15], "[������]");
		else
			sprintf(&ShuData[nSlipIndex++][15], "[Device error]");
	}

	if (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)
		sprintf(ShuData[nSlipIndex++], "[�ܾ���ȸ ���]");
	else
		sprintf(ShuData[nSlipIndex++], "[Check your account balance]");

	nSlipIndex++;												// 2004.03.10

	fnAPP_EditPrintLine(PrintDevice, ShuData[nSlipIndex++], 17, HiddenFlg);
	fnAPP_EditPrintLine(PrintDevice, ShuData[nSlipIndex++], 18, HiddenFlg);

	fnAPD_ShuPrint(PrintDevice, JNL_CANCEL, WaitFlg);
	
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// �Ա���Ұŷ�����ó�� (��빫)
int	CTranCmn::fnAPP_PrintDepCancelTran(int PrintDevice, int WaitFlg)
{
	int		HiddenFlg = FALSE;									// ��ɻ���
	int		nSlipIndex = 0, nJnlIndex = 0;						// ��ǥ/���� �����ε���

	if (MaterialOutFlg & DEV_SPR)								// ����/��ǥ���ڹ׹���Ϸ�
	{
		if (PrintDevice & DEV_SPR)								// ��ǥ �������� ����
			PrintDevice &= ~DEV_SPR;
	}

	if (PrintDevice == DEV_NONE)								// ���ڵ���̽� Ȯ��
		return T_OK;

	if (!AddSerialFlg)											// �Ϸù�ȣ������
	{
		AddSerialFlg = TRUE;									// �Ϸù�ȣ������
		m_pDevCmn->fnAPL_AddSerialNo();							// �ŷ��Ϸù�ȣ����
	}

	return T_OK;
}

// �������ó��
int	CTranCmn::fnAPP_PrintCancel(int PrintDevice, int WaitFlg)
{
	int		HiddenFlg = TRUE;									// ��ɻ���
	int		nSlipIndex = 0, nJnlIndex = 0, ii = 0;						// ��ǥ/���� �����ε���

	if (!AddSerialFlg)											// �Ϸù�ȣ������
	{
		AddSerialFlg = TRUE;									// �Ϸù�ȣ������
		m_pDevCmn->fnAPL_AddSerialNo();							// �ŷ��Ϸù�ȣ����
	}

	if (PrintDevice & DEV_JPR) 
	{
		HiddenFlg = FALSE;										// ���θ����ڽ� ��ɻ���
	}

	memset(ShuData, 0, sizeof(ShuData));						// Shu��¿���
	nSlipIndex = 0;

	for (ii = 0; ii < 50; ii++)
	{
		fnAPP_EditPrintLine(PrintDevice, ShuData[nSlipIndex], ii, HiddenFlg);		// ���� Edit
		if (ShuData[nSlipIndex][0] != 0)
			nSlipIndex++;
	}

	if (TranProc == TRAN_WITH)
	{
		if (m_pProfile->DEVICE.MachineType == U8100)
			sprintf(ShuData[0], "    [������]");
		else
			sprintf(ShuData[0], "        [������]");
	}
	else
	if (TranProc == TRAN_DEP)
	{
		if (m_pProfile->DEVICE.MachineType == U8100)
			sprintf(ShuData[0], "    [�Աݰŷ�]"); //#CS0006		[�Աݰŷ����] -> [�Աݰŷ�]
		else
			sprintf(ShuData[0], "        [�Աݰŷ�]"); //#0055 -> #0093
	}
	else
	if ((TranCode == TC_JRTRAN)	||
		(TranCode == TC_JRINQ)	)
	{
		if (m_pProfile->DEVICE.MachineType == U8100)
			sprintf(ShuData[0], "    [���������]");
		else
			sprintf(ShuData[0], "        [���������]");
	}
		else
	if ((TranCode == TC_HIPASS_CASH) ||						//#N0266
		(TranCode == TC_HIPASS_CREDIT))
	{
		if (m_pProfile->DEVICE.MachineType == U8100)
			sprintf(ShuData[0], "    [�����н����]");
		else
			sprintf(ShuData[0], "        [�����н����]");
	}
	else
	{
		if (m_pProfile->DEVICE.MachineType == U8100)
			sprintf(ShuData[0], "    [�����]");
		else
			sprintf(ShuData[0], "        [�����]");
	}


	fnAPD_ShuPrint(PrintDevice, JNL_OPER, WaitFlg);
	
	return T_OK;
}

// ��ǥ�Ա���������
int CTranCmn::fnAPP_PrintCheckDeposit(int PrintDevice, int WaitFlg)
{
	int		HiddenFlg = FALSE;									// ��ɻ���
	int		nIndex = 0;											// ��ǥ/���� �����ε���
	CString	tempStr("");
	char	StrBuff[41];

	memset(ShuData, 0, sizeof(ShuData));						// Shu��¿���
	nIndex = 0;
	
	fnAPP_EditPrintLine(PrintDevice, ShuData[nIndex++], 0, HiddenFlg);	// 1 ����
	fnAPP_EditPrintLine(PrintDevice, ShuData[nIndex++], 2, HiddenFlg);	// 2 ����

	nIndex++;

	memset(StrBuff, 0x00, sizeof(StrBuff));
	tempStr.Format("%d", CheckInfoAmt);
	sprintf(StrBuff, "      ��ǥ�ݾ� : ");
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
//	�����Լ�
/////////////////////////////////////////////////////////////////////////////
// �����ڵ�����
char* CTranCmn::fchpAPP_EditBank()
{
	CString StrBuff("");


	if (MenuAvail & MENU_MCU2)									// ����2TR
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
			StrBuff = GetSprintf("%3.3s", BANKJJ2);					// �������ī��
		else
			StrBuff = GetSprintf("%3.3s", &pCardData3->BankNo[0]);
	}
	else
	if (MenuAvail & MENU_PB)									// ����ŷ�
		StrBuff = GetSprintf("%3.3s", BANKJJ2);
	else
	if (MenuAvail & MENU_MU)									// ����ü�ŷ�
		StrBuff = GetSprintf("%3.3s", BANKJJ2);
	else
	if (MenuAvail & MENU_IC)									// K-CASH�ŷ�
		StrBuff = GetSprintf("%3.3s", &pCardData3->BankNo[0]);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fchpAPP_EditBank() : [%s]", StrBuff.GetBuffer(0));	// TEST!!!!!

	return StrBuff.GetBuffer(StrBuff.GetLength());
}

///////////////////////////////////////////////////////////////////////////////////
// ���¹�ȣ����
char* CTranCmn::fchpAPP_EditAcnt(int HiddenFlg, int AcntOnlyFlg)
{
	static	char	StrBuff[81];

	memset(StrBuff, 0, sizeof(StrBuff));						// �ʱ�ȭ

	if (!AcntOnlyFlg)
	{
		if ((MenuAvail & MENU_J2)	||
			(MenuAvail & MENU_T2)	)							// �ſ�ī��
		{
			EditString(pCardData2->MembershipNo, 16, "$$#####$$$$$####", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else 
		if ((MenuAvail & MENU_J3)	||							// ��������ī�常ó��
			(MenuIrdaMode)			)
		{
			//EditString(&pCardData3->OtherData[9], 11, "###-##-##$$$$", StrBuff, '#', '$', '*', HiddenFlg);
			EditString(Accept.AccountNum, 11, "###-##-##$$$#", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else
		if (MenuAvail & MENU_T3)								// Ÿ������ī��
		{
			EditString(&pCardData3->AccountNo[0], 16, "$$#####$$$$$####", StrBuff, '#', '$', '*', HiddenFlg);
//			EditString(pCardData2->MembershipNo, 16, "$$#####$$$$$####", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else
		if (MenuAvail & MENU_PB)								// ����
		{
//			EditString(&pPbMsData->AccountNo[0], 11, "###-##-##$$$#", StrBuff, '#', '$', '*', HiddenFlg);
			EditString(Accept.AccountNum, 11, "###-##-##$$$#", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else
		if (MenuAvail & MENU_MU)								// ����ü
		{
			EditString(Accept.MuAccountNum, Accept.MuAccountNumSize, "###-##-##$$$$", StrBuff, '#', '$', '*', HiddenFlg);
		}
	}
	else
	{
		if ((MenuAvail & MENU_J2)	||
			(MenuAvail & MENU_T2)	)							// �ſ�ī��
		{
			EditString(pCardData2->MembershipNo, 16, "$$#####$$$$$####", StrBuff, '#', '$', '*', HiddenFlg);
//			EditString(pCardData2->MembershipNo, 16, "######$$$$$$$$$$", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else 
		if ((MenuAvail & MENU_J3)	||							// ����ī�常ó��
//			(MenuAvail & MENU_T3)	||							// ����ī��
			(MenuIrdaMode)			)
		{
			//EditString(&pCardData3->OtherData[9], 11, "#######$$$$", StrBuff, '#', '$', '*', HiddenFlg);
			EditString(Accept.AccountNum, 11, "#######$$$#", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else
		if (MenuAvail & MENU_T3)								// Ÿ������ī��
		{
			EditString(&pCardData3->AccountNo[0], 16, "$$#####$$$$$####", StrBuff, '#', '$', '*', HiddenFlg);
//			EditString(pCardData2->MembershipNo, 16, "$$#####$$$$$####", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else
		if (MenuAvail & MENU_PB)								// ����
		{
//			EditString(&pPbMsData->AccountNo[0], 11, "#######$$$$", StrBuff, '#', '$', '*', HiddenFlg);
			EditString(Accept.AccountNum, 11, "#######$$$#", StrBuff, '#', '$', '*', HiddenFlg);
		}
		else
		if (MenuAvail & MENU_MU)								// ����ü
		{
			EditString(Accept.MuAccountNum, Accept.MuAccountNumSize, "#######$$$#", StrBuff, '#', '$', '*', HiddenFlg);
		}
	}

	return StrBuff;
}

///////////////////////////////////////////////////////////////////////////////////
// ���� ���¹�ȣ ����
CString CTranCmn::fchpAPP_EditSrcAcnt(void *inbuff, int inLen, int outLen, int HiddenFlg /*= FALSE*/,  int AcntOnlyFlg /*= FALSE*/)
{
	char	*inBuff  = (char *)inbuff;

	CString StrBuff("");

	if (!AcntOnlyFlg)
	{
		if (MenuAvail & MENU_MCU)								// ī��ŷ�
		{
			if (MenuAvail & MENU_J2T2)							// ��Ÿ��2TR�ŷ�
			{
				StrBuff = EditString(inbuff, 16, "$$##-###$-$$$$-####", '#', '$', '*', HiddenFlg);
			}
			else
			if (MenuAvail & MENU_T3)								// Ÿ������ī��
			{
				StrBuff = EditString(inbuff, 16, "$$##-###$-$$$$-####", '#', '$', '*', HiddenFlg);
			}
			else
			if (MenuAvail & MENU_J3)							// ��Ÿ��3TR�ŷ�
			{
				StrBuff = EditString(inbuff, 11, "###-##-##$$$#", '#', '$', '*', HiddenFlg);
			}
		}
		else
		if (MenuAvail & MENU_MU)								// ����ü
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
		if (MenuAvail & MENU_MCU)								// ī��ŷ�
		{
			if (MenuAvail & MENU_J2T2)							// ��Ÿ��2TR�ŷ�
			{
				StrBuff = EditString(inbuff, 16, "$$$$########$$$$", '#', '$', '*', HiddenFlg);
			}
			else
			if (MenuAvail & MENU_T3)								// Ÿ������ī��
			{
				StrBuff = EditString(inbuff, 16, "$$#####$$$$$####", '#', '$', '*', HiddenFlg);
			}
			else
			if (MenuAvail & MENU_J3)							// ��Ÿ��3TR�ŷ�
			{
				StrBuff = EditString(inbuff, 11, "#######$$$#", '#', '$', '*', HiddenFlg);
			}
		}
		else
		if (MenuAvail & MENU_MU)								// ����ü
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
// ���� ���¹�ȣ ����
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
// ��ǥ����
/////////////////////////////////////////////////////////////////////////////
// ��ǥ/���� ����
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
		"��������           ", //0
		"������ü           ", //1
		"���ݼ�������     ", //2
		"����������ü       ", //3
		"������ü         ", //4
		"���������(���) ", //5
		"��������� ��ȸ  ", //6
		"���ΰ����ݳ���     ", //7 //#0073
		"���ΰ����� ���γ���", //8
		"����Ʈ������ ����  ", //9
		"�ſ�������ȸ       ", //10
		"����������ȸ       ", //11
		"ī���             ", //12
		"����ȭ�����ں��� ", //13
		"Deposit            ", //14
		"�Ա�               ", //15
		"����Ʈ������ ����  ", //16
		"���ΰ����ݳ���     ", //17  //#SJ 1115 ��ǥ ����
		"ĳ���� ��ȸ        ", //18
		"ĳ���� ����        ", //19
		"Cash Advance       ", //20
		"Cash Withdrawal    ", //21
		"���ݼ�����ȸ     ", //22
		"���������ȸ       ", //23
		"��ȸ               ", //24
		"T-Money ����       ", //25
		"CGV                ", //26
		"Wire Transfer      ", //27
		"DOTONG             ", //28
		"�ܱ⿬ü������ȸ   ", //29
		"����ü �Ա�        ", //30
		"�ſ�����������ȸ   ", //31
		"�������           ", //32
		"������ȸ           ", //33
		"Withdrawal         ", //34
		"Balance Inquiry    ", //35
		"Account Transfer   ", //36
		"��������           ", //37
		"���漼������     ", //38
		"???????????????????"
	};
	
	char*	TranNameTbl[40] = 
	{
//		 12345678901234567890
		"����,����          ", //0
		"�ſ�ī��           ", //1
		"�Ｚī���         ", //2
		"�ѱ��ſ�����       ", //3
		"???????????????????"
	};

	if(MenuFICMode & MENU_IC)
		strTranKind = "IC";
	else
	if(PbTran & MENU_PB)
		strTranKind = "����";
	else
	if(MenuAvail & MENU_MU)
		strTranKind = " ��";
	else
		strTranKind = "MS";

	GetDate(CurDate);
	GetTime(CurTime);


	Data.Empty();
	memset(szTmp, 0x00, sizeof(szTmp));

	if (PrintDevice == (DEV_SPR | DEV_JPR))						// ���ڵ���Ÿ ������ �ϳ��� ��ġID��..~!!
		return T_OK;

	if (PrintDevice & (DEV_SPR | DEV_JPR) == DEV_NONE)			// ���ڵ���Ÿ ������ �ϳ��� ��ġID��..~!!
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
			Data = GetSprintf("[����ĳ�� �ŷ���ǥ]");
		break;
	case 3:
		Data = GetSprintf(SPACE16);	
		break;
	case 4:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			Data = GetSprintf("Date/Time:%4.4s%2.2s%2.2s-%2.2s:%2.2s", &CurDate[0], &CurDate[4],	&CurDate[6], &CurTime[0], &CurTime[2]);
		else
			Data = GetSprintf("�ŷ�����:%4.4s%2.2s%2.2s-%2.2s:%2.2s", &CurDate[0], &CurDate[4],	&CurDate[6], &CurTime[0], &CurTime[2]);
		break;
	case 5:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			Data = GetSprintf("Seque No:%6.6s%8.8s-%6.6s",  m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum, &m_pProfile->TRANS.SerialNo[0]);
		else
			Data = GetSprintf("ó����ȣ:%6.6s%8.8s-%6.6s",  m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum, &m_pProfile->TRANS.SerialNo[0]);
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
				Data = GetSprintf("�ŷ�����:%s", TranListTbl[0]);
				Data.TrimRight();
				Data += strTranKind;
			}
			break;
		case TRANID_7220:
		case TRANID_7230:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Trans.  :%s", TranListTbl[35]);
			else
				Data = GetSprintf("�ŷ�����:%s", TranListTbl[24]);
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
					Data = GetSprintf("�ŷ�����:%s", TranListTbl[2]);
					Data.TrimRight();
					Data += strTranKind;
			break;
		case TRANID_4350:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Trans.  :%s", TranListTbl[36]);
			else
				Data = GetSprintf("�ŷ�����:%s", TranListTbl[4]);
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
				Data = GetSprintf("�ŷ�����:%s", TranListTbl[1]);
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
				Data = GetSprintf("�ŷ�����:%s", TranListTbl[15]) ;
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_4730: //#0024
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Trans.  :%s", "Passbook Arrangement");
			else
				Data = GetSprintf("�ŷ�����:%s",TranListTbl[37]) ;  //#0036
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_4810:
		case TRANID_4813:
			Data = GetSprintf("�ŷ�����:%s", TranListTbl[7]);
			Data.TrimRight();
			Data += strTranKind;
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			Data = GetSprintf("�ŷ�����:%s", TranListTbl[38]);
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
				Data = GetSprintf("�ŷ����:%s", fnAPP_Get_BankName(CardBank));				
			break;
		case TRANID_4350:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Service :%s", "Credit");	
			else
				Data = GetSprintf("�ŷ����:%s", fnAPP_Get_CreditName(pCardData2->MembershipNo));
			break;
		case TRANID_7230:
		case TRANID_4330:
		case TRANID_4340:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Service :%s", "Credit");	
			else
				Data = GetSprintf("�ŷ����:%s", fnAPP_Get_CreditName(pCardData2->MembershipNo));
			break;
		case TRANID_7260:
		case TRANID_4390:
		case TRANID_4370:
		case TRANID_4215:
		case TRANID_4396:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Service :%s", fnAPP_Get_BankName(CardBank));	
			else
				Data = GetSprintf("�ŷ����:%s", fnAPP_Get_BankName(CardBank));				
			break;
		case TRANID_4810:
		case TRANID_4813:
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			Data = GetSprintf("������ȣ:%12.12s", m_RD.byDataField_026x);
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
				//���¹�ȣ	
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
				Data = GetSprintf("���¹�ȣ:%s", m_RD.byDataField_017x);
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
				Data = GetSprintf("���¹�ȣ:%s", m_RD.byDataField_017x);
			Data = "@" + Data;
			break;
		default:
			break;
		}		
		break;
		
	case 10:														// �ŷ��ݾ�
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
				Data = GetSprintf("�ŷ��ݾ�:%s��",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
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
				Data = GetSprintf("�ŷ��ݾ�:%s��",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			break;
		case TRANID_4810:
		case TRANID_4813:
			Data = GetSprintf("������  :%s��", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			Data = GetSprintf("�ŷ��ݾ�:%s��",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			break;
		default:
			break;
		}		
		break;
	case 11:														// �ŷ� �� �ݾ�
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
				Data = GetSprintf("������ȣ:%12.12s", m_RD.byDataField_026x);
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
				Data = GetSprintf("������ȣ:%12.12s", m_RD.byDataField_026x);
			break;
		case TRANID_4810:
		case TRANID_4813:
			Data = GetSprintf("�ŷ��ݾ�:%s��",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			Data = GetSprintf("�����ڸ�:%s", strCT(m_RD.byDataField_021x , 16));
			break;
		default:
			break;
		}		
		break;
	case 12:	
//		Data = GetSprintf("�����ڵ�:%03d[%s]", TranResultNGCode, (TranResultNGCode == 0)	? "����" : "�Ҵ�"); 

		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)		
			Data = GetSprintf("  Response:%03d[%s]", TranResultNGCode, (TranResultNGCode == 0)	? "OK" : "NG"); 
		else
			Data = GetSprintf("  �����ڵ�:%03d[%s]", TranResultNGCode, (TranResultNGCode == 0)	? "�ŷ�����" : "�ŷ��Ҵ�");
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
				Data = GetSprintf("������  :%s��", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
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
//				Data = GetSprintf("�����ڵ�:%03d", TranResultNGCode);
			else
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Fee     :%s Won", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			else
				Data = GetSprintf("������  :%s��", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
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
				Data = GetSprintf("������  :%s��", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			break;
		case TRANID_4810:
		case TRANID_4813:
			Data = GetSprintf("���α��:%s", strCT(m_RXJIRODATA.JiroBankCode , 16));
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			Data = GetSprintf("����:%s", strGetBufferArray[0]);
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
				Data = GetSprintf("�����ܾ�:%s��", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
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
				Data = GetSprintf("�����ܾ�:%s��", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
			Data = "@" + Data;
			break;
		case TRANID_4810:
		case TRANID_4813:
			Data = GetSprintf("�����ڸ�:%s", strCT(m_RD.byDataField_021x , 16));
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			Data = GetSprintf("�ŷ��ݾ�:%s��", strGetBufferArray[1]);
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
			Data = GetSprintf("���ι�ȣ:%19.19s", m_RXJIRODATA.ElecNum);
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			if(Asc2Int(Accept.JiroBillCount,2) >= 2 )
				Data = GetSprintf("����:%s", strGetBufferArray[2]);
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
					Data = GetSprintf("[�Աݻ�]");
			}
			else
			{
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("[Deposit Detail]");
				else
					Data = GetSprintf("[�Աݻ�]");
			}
			break;
		case TRANID_4810:
		case TRANID_4813:
			if(Asc2Int(Accept.JiroBillCount,2) >= 1 ) //#N0162-1
				Data = GetSprintf("���γ���(��/��)     �ݾ�");  //#SJ 1115 ��ǥ ����
			break;
		case TRANID_4811:
		case TRANID_4812:
		case TRANID_4814:
			if(Asc2Int(Accept.JiroBillCount,2) >= 2 )
				Data = GetSprintf("�ŷ��ݾ�:%s��", strGetBufferArray[3]);
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
				Data = GetSprintf("������:%s", m_RD.byDataField_012x);
			break;
		case TRANID_4350:
			if(!m_pDevCmn->TranResult) 
				break;
			memset(szTempBuff2, 0x00, sizeof(szTempBuff2));
			EditString(m_RD.byDataField_020x, 16, "########$$$$$$$$", szTempBuff2, '#', '$', '*', TRUE);
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("Recv. B,S:%s", &szTempBuff2);
			else
				Data = GetSprintf("�������:%s", &szTempBuff2);
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
				Data = GetSprintf("5��:%d 1��:%d ��:%d õ:%d", CashData.mFiftyThousand, CashData.mTenThousand, CashData.mFiveThousand, CashData.mOneThousand); //#0093
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
				Data = GetSprintf("����:%s", strGetBufferArray[4]);
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
					Data = GetSprintf("�������:%s", szTempBuff2);
			}
			else
			{
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("Recv. B,S: %s", Accept.InAccountNum);
				else
					Data = GetSprintf("�������:%s", Accept.InAccountNum);				
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
					Data = GetSprintf("�ܾ� Ȯ�ο�");
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
				Data = GetSprintf("�ŷ��ݾ�:%s��", strGetBufferArray[5]);
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
				Data = GetSprintf("������  :%s", strCT(m_RD.byDataField_021x, strlen(m_RD.byDataField_021x)));
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
				Data = GetSprintf("����:%s", strGetBufferArray[6]);
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
				Data = GetSprintf("�ŷ��ݾ�:%s��", strGetBufferArray[7]);
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
				if (!m_pDevCmn->fnAPL_CheckError())							// ��ְ�����
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
/* 2.      [MyCashZone  �� �� �� �� ǥ]         |         [MyCashZone  Receipt]          
/* 3.                                           |
/* 4.  �ŷ����� : 20090709-18:26 ,              |  Date/Time: 20090709-18:26 
/* 5.  ó����ȣ : NH0001                        |  Seque No : NH0001  
/* 6.  �ŷ����� : ��������	                    |  Trans.   : ��������
/* 7.  �ŷ���� : ����,����                     |  Service  : ����,����
/* 8.  ���¹�ȣ : 1234567890123456              |  CARD No. : 1234567890123456	
/* 9.  �ŷ��ݾ� : 23,000��                      |  Req. ATM : 23,000��
/*10.  ������ȣ : 123456789012                  |  Trans.NO : 123456789012	
/*11.                                           |  
/*12.  ������   : 600��	                        |  Fee      : 600��
/*13.  �����ܾ� : 23,178,000��                  |  Balance  : 23,178,000��                    
/*14.  Ÿ����   : 0��                           |  Oth. AMT : 0��	
/*15.  ������� : ����                          |  Org.Name : ����
/*15.1 ���¸�   : ����� <- �Աݰ���            |  
/*16.  ������ : ����                          |  Recv.Org : ����   
/*17.  ������� : 1234567890123456              |  Recv. B,S: 1234567890123456 
/*18.  ������   : �����                        |  Benefic. : ����� 
/*19.  ErrorCode: 0000                          |  ErrorCode: 0000
/*20.  �����޽���(����)                         |  �����޽���(����)  
/*21.  Tel : 080-920-2220                       |  Tel : 080-920-2220
/*22.	
/**************************************************************************************************/
	
 

// ��ǥ/���� ����(Finance Service Print Form 2009.07.13)
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
		if (TranProc == TRAN_HIPASS)		//#N0266 ��ǥ ���� ����
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
		"��������           ", //0
		"������ü           ", //1
		"�ܱ�ī����� ���  ", //2 #N0247
		"����������ü       ", //3
		"�ܱ�ī����� ��ü  ", //4
		"���������(���) ", //5
		"��������� ��ȸ  ", //6
		"���ΰ����� ����    ", //7
		"���ΰ����� ���γ���", //8
		"����Ʈ������ ����  ", //9
		"�ſ�������ȸ       ", //10
		"����������ȸ       ", //11
		"ī���             ", //12
		"����ȭ�����ں��� ", //13
		"Deposit            ", //14
		"�Ա�               ", //15
		"����Ʈ������ ����  ", //16
		"���ΰ����� ����    ", //17
		"ĳ���� ��ȸ        ", //18
		"ĳ���� ����        ", //19
		"Cash Advance       ", //20
		"Cash Withdrawal    ", //21
		"�ܱ�ī����� ��ȸ  ", //22
		"���������ȸ       ", //23
		"��ȸ               ", //24
		"T-Money ����       ", //25
		"CGV                ", //26
		"Wire Transfer      ", //27
		"DOTONG             ", //28
		"�ܱ⿬ü������ȸ   ", //29
		"����ü �Ա�        ", //30
		"�ſ�����������ȸ   ", //31
		"�������           ", //32
		"������ȸ           ", //33
		"Withdrawal         ", //34
		"Balance Inquiry    ", //35
		"Account Transfer   ", //36
		"��������           ", //37
		"���ڻ�ǰ�Ǳ���     ", //38
		"���ڻ�ǰ�����     ", //39
		"ONE�������	    ", //40 #N0248 => #N0259
		"ONE����������	", //41	#N0248 => #N0259
		"�����н�����(�ſ�)	", //42	#N0266
		"�����н� �ſ����	", //43	#N0266
		"�����н�����(����)	", //44	#N0270
		"???????????????????"
	};
	
	char*	TranNameTbl[40] = 
	{
//		 12345678901234567890
		"����,����          ", //0
		"�ſ�ī��           ", //1
		"�Ｚī���         ", //2
		"�ѱ��ſ�����       ", //3
		"???????????????????"
	};

	if(EMVReadFlg == TRUE)
		strTranKind = " (IC)";
	else
	if(MenuFICMode & MENU_IC)
		strTranKind = " (IC)";
	else
	if(PbTran & MENU_PB)
		strTranKind = " (����)";
	else
	if(MenuAvail & MENU_MU)
		strTranKind = " (��)";
	else
		strTranKind = " (MS)";

	GetDate(CurDate);
	GetTime(CurTime);


	Data.Empty();
	memset(szTmp, 0x00, sizeof(szTmp));

	if (PrintDevice == (DEV_SPR | DEV_JPR))						// ���ڵ���Ÿ ������ �ϳ��� ��ġID��..~!!
		return T_OK;

	if (PrintDevice & (DEV_SPR | DEV_JPR) == DEV_NONE)			// ���ڵ���Ÿ ������ �ϳ��� ��ġID��..~!!
		return T_OK;

	memset(szTmp, 0, sizeof(szTmp));

	if(m_pDevCmn->fnSCR_GetCurrentLangMode() == KEB_MODE)  // ������ ��ǥ ���� �ѱ� #N0181
		m_pDevCmn->fnSCR_SetCurrentLangMode(KOR_MODE);

	switch (LineNum)
	{													
	case 0:
		break;
	case 1:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			Data = GetSprintf("         [EasyCash Receipt]      ");
		else
			Data = GetSprintf("   [����ĳ�� �������ޱ� �ŷ���ǥ]    ");

		break;
	case 2:
		Data = GetSprintf(SPACE16);	
		break;
	case 3:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			Data = GetSprintf("  Date/Time: %4.4s%2.2s%2.2s-%2.2s:%2.2s", &CurDate[0], &CurDate[4],	&CurDate[6], &CurTime[0], &CurTime[2]);
		else
			Data = GetSprintf("  �ŷ����� : %4.4s%2.2s%2.2s-%2.2s:%2.2s", &CurDate[0], &CurDate[4],	&CurDate[6], &CurTime[0], &CurTime[2]);

		break;
	case 4:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			Data = GetSprintf("  Atm  Num : %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
		else
			Data = GetSprintf("  ����ȣ : %6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
		break;
	case 5:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			Data = GetSprintf("  Seque No : %6.6s", m_pProfile->TRANS.SerialNo);
		else
			Data = GetSprintf("  ó����ȣ : %6.6s", m_pProfile->TRANS.SerialNo);
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
				Data = GetSprintf("  �ŷ����� : %s", TranListTbl[0]) ;
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_7220:
		case TRANID_7230:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Trans.   : %s", TranListTbl[35]);
			else
				Data = GetSprintf("  �ŷ����� : %s", TranListTbl[24]) ;
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
					Data = GetSprintf("  �ŷ����� : %s", TranListTbl[2]);
					Data.TrimRight();
					Data += strTranKind;
			break;
		case TRANID_4330:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Trans.   : %s", TranListTbl[34]);
			else
				Data = GetSprintf("  �ŷ����� : %s", TranListTbl[2]) ;
				Data.TrimRight();
				if(EMVReadFlg == TRUE)   //#N0247
					Data += strTranKind;
			break;
		case TRANID_4350:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Trans.   : %s", TranListTbl[36]);
			else
				Data = GetSprintf("  �ŷ����� : %s", TranListTbl[4]) ;
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
				Data = GetSprintf("  �ŷ����� : %s", TranListTbl[1]) ;
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
				Data = GetSprintf("  �ŷ����� : %s", TranListTbl[15]);
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_4730: //#0024
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Trans.   : %s", "Passbook Arrangement");
			else
				Data = GetSprintf("  �ŷ����� : %s",TranListTbl[37]);   //#0036
				Data.TrimRight();
				Data += strTranKind;
			break;
		case TRANID_4117:   //#N0182
			Data = GetSprintf("  �ŷ����� : %s", TranListTbl[40]);
			Data.TrimRight();
			Data += strTranKind;
			break;
		case TRANID_4327:   //#N0182
			Data = GetSprintf("  �ŷ����� : %s", TranListTbl[41]);
			Data.TrimRight();
			Data += strTranKind;
			break;
		case TRANID_4850:	//#N0266
			Data = GetSprintf("  �ŷ����� : %s", TranListTbl[43]);
			Data.TrimRight();
			break;
		case TRANID_4717:	//#N0270
			Data = GetSprintf("  �ŷ����� : %s", TranListTbl[44]);
			Data.TrimRight();
			break;
		case TRANID_4840:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			if(TranCode == TC_HIPASS_CREDIT)
			{
				Data = GetSprintf("  �ŷ����� : %s", TranListTbl[42]);
				Data.TrimRight();		
			}
			else
			if(TranCode == TC_HIPASS_CASH)
			{
				Data = GetSprintf("  �ŷ����� : %s", TranListTbl[44]);
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
				Data = GetSprintf("  �ŷ���� : %s", fnAPP_Get_BankName(CardBank));				
			break;
		case TRANID_4350:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Service  : %s", "Credit");	
			else
				Data = GetSprintf("  �ŷ���� : %s", fnAPP_Get_CreditName(pCardData2->MembershipNo)); //#0100
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
					Data = GetSprintf("  ���¹�ȣ : %s", m_RD.byDataField_017x);
				Data = "@" + Data;
			}
			break;
		case TRANID_7230:
		case TRANID_4330:
		case TRANID_4340:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Service  : %s", "Credit");	
			else
				Data = GetSprintf("  �ŷ���� : %s", fnAPP_Get_CreditName(pCardData2->MembershipNo)); //#0100
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
				Data = GetSprintf("  �ŷ���� : %s", fnAPP_Get_BankName(CardBank));				
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
				//���¹�ȣ	
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
				Data = GetSprintf("  ���¹�ȣ : %s", m_RD.byDataField_017x);
			Data = "@" + Data;
			break;
		case TRANID_4117:   //#N0182
		case TRANID_4327:   //#N0182
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  CARD No. : %6.6s**********", Accept.PbConfirmNum);
			else
				Data = GetSprintf("  ���¹�ȣ : %6.6s**********", Accept.PbConfirmNum);
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
				Data = GetSprintf("  ���¹�ȣ : %s", m_RD.byDataField_017x);
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
				Data = GetSprintf("  �ŷ��ݾ� : %s��",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			break;
		default:
			break;
		}		
		break;
		
	case 10:														// �ŷ��ݾ�
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
				Data = GetSprintf("  �ŷ��ݾ� : %s��",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
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
				Data = GetSprintf("  �ŷ��ݾ� : %s��",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
			break;
		case TRANID_4717:	//#N0270
		case TRANID_4840:	//#N0266
		case TRANID_4850:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			if (TranCode == TC_HIPASS_CREDIT)
				Data = GetSprintf("  ���ι�ȣ : %8.8s", Accept.ETicketConfirmNum);
			break;
		default:
			break;
		}		
		break;
	case 11:														// �ŷ� �� �ݾ�
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
				Data = GetSprintf("  ������ȣ : %12.12s", m_RD.byDataField_026x);
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
				Data = GetSprintf("  ������ȣ : %12.12s", m_RD.byDataField_026x);
			break;
		case TRANID_4717:	//#N0270
		case TRANID_4840:	//#N0266
		case TRANID_4850:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			Data = GetSprintf("  ������   : %s��", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
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
			Data = GetSprintf("  �����ڵ� : %03d", TranResultNGCode);
		break;

	//#CS0006 (�ŷ���� �߰� - ûȣ��û)
	case 13:
		if(m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE)		
		{
			cstrTemp.Format("%03d", TranResultNGCode);
			if( IniGetStr(_ECASH_SVR_RES_INI, "PRINT_MSG", cstrTemp, SPACE3).GetLength() < 29 )
				Data = GetSprintf("  �ŷ����: %s", IniGetStr(_ECASH_SVR_RES_INI, "PRINT_MSG", cstrTemp, SPACE3));
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
				Data = GetSprintf("  ������   : %s��", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			break;
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_7200:	
		case TRANID_7126:
		case TRANID_4730:
		case TRANID_7236:
			if(!m_pDevCmn->TranResult) ;
//				Data = GetSprintf("  �����ڵ�   : %03d [�ŷ��Ҵ�]", TranResultNGCode);
			else
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Fee      : %s Won", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			else
				Data = GetSprintf("  ������   : %s��", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
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

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)  //-1  ��ǥ ������ **** ǥ��
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
					Data = GetSprintf("  ������   : %s��", strCT(m_RD.byDataField_024x, 7));
				else
					Data = GetSprintf("  ������   : %s��", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			}
			
			//if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
			//	Data = GetSprintf("  Fee      : %s Won", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			//else
			//	Data = GetSprintf("  ������   : %s��", strCT(Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_024x, strlen(m_RD.byDataField_024x), strlen(m_RD.byDataField_024x)+3).GetLength()));
			
			break;
		case TRANID_4717:	//#N0270
		case TRANID_4840:	//#N0266
		case TRANID_4850:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			Data = GetSprintf("  [��������]");
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

			if(m_pProfile->DEVICE.ETCDevice20 == KB && TranCode2 == TRANID_4720)	//#N0226 �������� �Աݽ� ��ǥ �ܾ� ��� ����
				break;
			
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Balance  : %s Won", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
			else
				Data = GetSprintf("  �����ܾ� : %s��", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
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
					if(memcmp(Accept.MuConfirmNum, "CC", 2) == 0)  //#N0171 ������ �ܾ� ǥ�� X
						;
					else
						Data = GetSprintf("  Balance  : %s", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
				}
				else
					Data = GetSprintf("  Balance  : %s Won", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
			}
			else
				Data = GetSprintf("  �����ܾ� : %s��", strCT(Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_022x, strlen(m_RD.byDataField_022x), strlen(m_RD.byDataField_022x)+3).GetLength()));
			Data = "@" + Data;
			break;
		case TRANID_4850:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			Data = GetSprintf("  ������ : %s��",  strCT(Asc2Amt(m_RD.byDataField_065x, strlen(m_RD.byDataField_065x), strlen(m_RD.byDataField_065x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_065x, strlen(m_RD.byDataField_065x), strlen(m_RD.byDataField_065x)+3).GetLength()));
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
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE) //#KIMH0145 => #N0192 ��ǥ ���� ����
				Data = GetSprintf("  50K: %d, 10K: %d", Asc2Int(Accept.Cash50T_Money, 15)/50000 , Asc2Int(Accept.Cash10T_Money, 15)/10000);
			else
				Data = GetSprintf("  ������: %d��, ����: %d��", Asc2Int(Accept.Cash50T_Money, 15)/50000 , Asc2Int(Accept.Cash10T_Money, 15)/10000);
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
			Data = GetSprintf("  �����ݾ� : %s��",  strCT(Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetBuffer(0), Asc2Amt(m_RD.byDataField_023x, strlen(m_RD.byDataField_023x), strlen(m_RD.byDataField_023x)+3).GetLength()));
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
					Data = GetSprintf("  [�Աݻ�]");
			}
			else
			{
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("  [Deposit Detail]");
				else
					Data = GetSprintf("  [�Աݻ�]");
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
				Data = GetSprintf("  ������ : %s", m_RD.byDataField_012x);
			break;
		case TRANID_4350:
			if(!m_pDevCmn->TranResult) 
				break;
			memset(szTempBuff2, 0x00, sizeof(szTempBuff2));
			EditString(m_RD.byDataField_020x, 16, "########$$$$$$$$", szTempBuff2, '#', '$', '*', TRUE);
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Recv. B,S : %s", &szTempBuff2);
			else
				Data = GetSprintf("  ������� : %s", &szTempBuff2);
			Data = "@" + Data;
			break;
		case TRANID_4410:
		case TRANID_4713:
		case TRANID_4720:
		case TRANID_4716:
		case TRANID_7200:
		case TRANID_7126:
		case TRANID_7236:
			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE) //��ǥ ���� ���� �� => ��õ
				Data = GetSprintf("  50K:%d 10K:%d 5K:%d 1K:%d", CashData.mFiftyThousand, CashData.mTenThousand, CashData.mFiveThousand, CashData.mOneThousand);
			else
				Data = GetSprintf("  5��:%d ����:%d 5õ:%d õ:%d", CashData.mFiftyThousand, CashData.mTenThousand, CashData.mFiveThousand, CashData.mOneThousand);
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
					Data = GetSprintf("  ������� : %s", szTempBuff2);
			}
			else
			{
				if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
					Data = GetSprintf("  Recv. B,S: %s", Accept.InAccountNum);
				else
					Data = GetSprintf("  ������� : %s", Accept.InAccountNum);				
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
					Data = GetSprintf("  �ܾ� Ȯ�ο�");
			}
			break;
		case TRANID_4717:	//#N0270
		case TRANID_4840:	//#N0266
		case TRANID_4850:	//#N0266
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			Data = GetSprintf("  ����ڹ�ȣ : 129-86-11103");
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
				Data = GetSprintf("  ������   : %s", strCT(Accept.EasyOneName, 25));
				break;
			}

			if(m_pDevCmn->fnSCR_GetCurrentLangMode() != KOR_MODE)
				Data = GetSprintf("  Benefic. : %s", strCT(m_RD.byDataField_021x, strlen(m_RD.byDataField_021x)));
			else
				Data = GetSprintf("  ������   : %s", strCT(m_RD.byDataField_021x, strlen(m_RD.byDataField_021x)));
			break;
		case TRANID_4670:	//#N0266
		case TRANID_4671:	//#N0266
			Data = GetSprintf("  ��ǥ�ڸ� : �ڱ���");
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
				Data = GetSprintf("  �������� : �����÷���ī��(��)");
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
			if (!m_pDevCmn->fnAPL_CheckError())							// ��ְ�����
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

	// #N0174 - ������ ��ǥ ��½� - ���� ����� ���� ����ó�� (��������) - 06.04
	if((TranProcSave == TRAN_EASYONE) && (m_pDevCmn->fnSCR_GetCurrentLangMode() == KOR_MODE))
		m_pDevCmn->fnSCR_SetCurrentLangMode(KEB_MODE);
	
	return T_OK;
}
/**************************************************************************************************/
/****01234567890123456789012345678901234567890   01234567890123456789012345678901234567890
/* 0.                                           |
/* 1.                                           |
/* 2.      [MyCashZone  �� �� �� �� ǥ]         |         [MyCashZone  Receipt]          
/* 3.                                           |
/* 4.  �ŷ����� : 20090709-18:26 ,              |  Date/Time: 20090709-18:26 
/* 5.  ó����ȣ : NH0001                        |  Seque No : NH0001  
/* 6.  �ŷ����� : ��������	                    |  Trans.   : ��������
/* 7.  �ŷ���� : ����,����                     |  Service  : ����,����
/* 8.  ���¹�ȣ : 1234567890123456              |  CARD No. : 1234567890123456	
/* 9.  �ŷ��ݾ� : 23,000��                      |  Req. ATM : 23,000��
/*10.  ������ȣ : 123456789012                  |  Trans.NO : 123456789012	
/*11.                                           |  
/*12.  ������   : 600��	                        |  Fee      : 600��
/*13.  �����ܾ� : 23,178,000��                  |  Balance  : 23,178,000��                    
/*14.  Ÿ����   : 0��                           |  Oth. AMT : 0��	
/*15.  ������� : ����                          |  Org.Name : ����
/*15.1 ���¸�   : ����� <- �Աݰ���            |  
/*16.  ������� : 1234567890123456              |  Recv. B,S: 1234567890123456 
/*17.  ������ : ����                          |  Recv.Org : ����   
/*18.  ������   : �����                        |  Benefic. : ����� 
/*19.  ErrorCode: 0000                          |  ErrorCode: 0000
/*20.  �����޽���(����)                         |  �����޽���(����)  
/*21.                                           |
/*22.  ��ƿ����ȿ��[080-920-2220]               |  N[080-920-2220]
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
// ��ǥ/���� ����(Ticket Service Print Form 2010.06.14)
int	CTranCmn::fnAPP_EditPrintLine_Sub_T3_2(int PrintDevice, void* cData, int LineNum, int HiddenFlg)
{
	
	return T_OK;
}
// ��ǥ/���� ����(Etc Service Print Form 2009.07.13)
int	CTranCmn::fnAPP_EditPrintLine_Sub_T3_3(int PrintDevice, void* cData, int LineNum, int HiddenFlg)
{

	
	return T_OK;
}
/*********************************************************************************************************************************
 *
 * ���Ӱŷ� �غ� Proc Kim.Gi.Jin 2010.10.20
 *
 *********************************************************************************************************************************/
int	CTranCmn::fnAPP_TranGetContinueProc(int nTranTmpCode)
{

	if ((m_pDevCmn->EarPhoneInserted)	||						// �����ATM
		(m_pDevCmn->SightHandiCapped))
		return T_OK;

	if(m_pDevCmn->fnSCR_GetCurrentLangMode() == JPN_MODE || m_pDevCmn->fnSCR_GetCurrentLangMode() == ENG_MODE || m_pDevCmn->fnSCR_GetCurrentLangMode() == CHN_MODE)
		return T_OK;

	return T_OK;
}

//2011.10.25 Yun.H.J ���α���/���ͷ�Ƽ��/���� ���� ����
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
				strT0 = "����";
				break;
			case '1':
				strT0 = "��ü";
				break;
			case '2':
				strT0 = "����";
				break;
			case '3':
				strT0 = "����";
				break;
			case '4':
				strT0 = "ü��";
				break;				
			}

			strT1.Format("%s",Asc2Amt(&m_RXJIRODATA.JiroPayData[slen + 5], 12,16));
			strT1.TrimLeft();
			strT1.TrimRight();
			
			if(m_RXJIRODATA.JiroPayData[slen + 4] == '0')
				strTemp.Format("%2.2s/%2.2s %4.4s|%s|%s",  &m_RXJIRODATA.JiroPayData[slen],&m_RXJIRODATA.JiroPayData[slen+2], strT0, "���⳻" , strT1);		
			else
				strTemp.Format("%2.2s/%2.2s %4.4s|%s|%s",  &m_RXJIRODATA.JiroPayData[slen],&m_RXJIRODATA.JiroPayData[slen+2], strT0, "������" , strT1);		


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
		fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
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
		
		// ó������
		strTemp.Format("%4.4s - %2.2s - %2.2s", &m_RXJIRODATA.JiroPayData[49], &m_RXJIRODATA.JiroPayData[53], &m_RXJIRODATA.JiroPayData[55]);	
		m_pDevCmn->fnSCR_DisplayString(6, strTemp);	

		ScrNum = 776;
	}
	else
	if(m_RXJIRODATA.JiroPayData[0]  == '0')
	{

		//����
		strTemp.Format("%s", strCT(m_RD.byDataField_021x, 16));
		m_pDevCmn->fnSCR_DisplayString(1, strTemp);	
		// ���� �����
		strTemp.Format("%s", strCT(m_RXJIRODATA.JiroBankCode,16));
		m_pDevCmn->fnSCR_DisplayString(2, strTemp);
		// ���ڳ��ι�ȣ 
		strTemp.Format("%s", strCT(m_RXJIRODATA.ElecNum,19));
		m_pDevCmn->fnSCR_DisplayString(3, strTemp);

		slen = 3;
		for(int i = 0; i < Asc2Int(&m_RXJIRODATA.JiroPayData[1], 2)  ; i++)
		{          
			switch(m_RXJIRODATA.JiroPayData[slen + 4])
			{
			case '0':
				strT0 = "����";
				break;
			case '1':
				strT0 = "��ü";
				break;
			case '2':
				strT0 = "����";
				break;
			case '3':
				strT0 = "����";
				break;
			case '4':
				strT0 = "ü��";
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

	//#��ǥ ������ ����
	slen = 4;
	for(int i = 0; i < Asc2Int(&m_RXJIRODATA.JiroPayData[2], 2) ; i++)
	{
		if(m_RXJIRODATA.JiroPayData[slen + 20] == 'Y')
		{
			switch(m_RXJIRODATA.JiroPayData[slen + 17])
			{
			case '0':
				strT0 = "����";
				break;
			case '1':
				strT0 = "��ü";
				break;
			case '2':
				strT0 = "����";
				break;
			case '3':
				strT0 = "����";
				break;
			case '4':
				strT0 = "ü��";
				break;				
			}

			strT1.Format("%s",Asc2Amt(&m_RXJIRODATA.JiroPayData[slen + 5] , 12,16));
			strT1.TrimLeft();
			strT1.TrimRight();
			
			 //#SJ 1115 ��ǥ ����

			if(strT1.GetLength() < 11)
			{
				while(strT1.GetLength() < 11)
				{
					strT1.Insert(0, ' ');
				}
			}		

			strTemp.Format("%2.2s/%2.2s %4.4s %s��",  &m_RXJIRODATA.JiroPayData[slen], &m_RXJIRODATA.JiroPayData[slen + 2], strT0 , strT1);		
			strGetBufferArray[j] = strTemp;

			j++;
		}
		slen += 21;
	}

	Int2Asc(j, Accept.JiroBillCount, sizeof(Accept.JiroBillCount));
	
		
	//����
	strTemp.Format("%s", strCT(m_RD.byDataField_021x, 16));
	m_pDevCmn->fnSCR_DisplayString(1, strTemp);	
	//���α��
	strTemp.Format("%s", strCT(m_RXJIRODATA.JiroBankCode , 16));
	m_pDevCmn->fnSCR_DisplayString(2, strTemp);	
	//���ڳ��ι�ȣ
	strTemp.Format("%s", strCT(m_RXJIRODATA.ElecNum , 19));
	m_pDevCmn->fnSCR_DisplayString(3, strTemp);
	//���αݾ�
	strTemp.Format("%s", strCT(Accept.Money, sizeof(Accept.Money) ));
	m_pDevCmn->fnSCR_DisplayAmount(4, strTemp);
	//������
	m_pDevCmn->fnSCR_DisplayAmount(5, (LPCTSTR)m_RD.byDataField_024x); //������

	slen = Asc2Int(&m_RXJIRODATA.JiroPayData[2], 2) * 21 + 2;

	/*
	if(m_HNetRD.HNet_Dummy1[slen] == 'Y')
	{
		strTemp.Format("%2.2s/%2.2s - %2.2s/%2.2s (%d����)", &m_HNetRD.HNet_Dummy1[slen+1],&m_HNetRD.HNet_Dummy1[slen+3],&m_HNetRD.HNet_Dummy1[slen+5],&m_HNetRD.HNet_Dummy1[slen+7], Asc2Int(&m_HNetRD.HNet_Dummy1[slen+9], 3) );
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
	
	strTemp.Format("%s", strCT(m_RD.byDataField_017x,19));  //ī���ȣ
	m_pDevCmn->fnSCR_DisplayString(1, strTemp);

	strTemp.Format("%s", strCT(m_RD.byDataField_021x, 16));  //�����ڼ���
	m_pDevCmn->fnSCR_DisplayString(2, strTemp);

	strTemp.Format("%d", Asc2Int(&m_RXLOCALTAXDATA.JiroPayData[1], 4));  //�Ѱ����Ǽ�
	m_pDevCmn->fnSCR_DisplayString(3, strTemp);

	strTemp.Format("%s",  strCT(&m_RXLOCALTAXDATA.JiroPayData[5], 12));  // �Ѱ����ݾ�
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
	

	if (GetKeyStr == "1����ȸ")
	{
		fnAPP_LOCALTAX_AcceptElecNum();
		Accept.JiroTranCode = TC_ELECINQ;
	}
	else
	if (GetKeyStr == S_CONFIRM)
		return T_OK;
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
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
	
	strTemp.Format("%s", strCT(m_RD.byDataField_017x,19));  //ī���ȣ
	m_pDevCmn->fnSCR_DisplayString(1, strTemp);

	strTemp.Format("%s", strCT(m_RD.byDataField_021x, 16));  //�����ڼ���
	m_pDevCmn->fnSCR_DisplayString(2, strTemp);

	m_pDevCmn->fnSCR_DisplayAmount(3, (LPCTSTR)m_RD.byDataField_024x); //������

	if(memcmp(m_RXLOCALTAXDATA.Gubun, "03", 2) == 0)
	{
		slen =19;
		strTemp.Format("%s", strCT(&m_RXLOCALTAXDATA.JiroPayData[slen + 20], 12));  // �Ѱ����ݾ�
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
		strTemp.Format("%d", nTotalAmount);  // �Ѱ����ݾ�
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
		fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
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
	
	strTemp.Format("%s", strCT(m_RD.byDataField_017x,19));  //ī���ȣ
	m_pDevCmn->fnSCR_DisplayString(1, strTemp);

	strTemp.Format("%s", strCT(m_RD.byDataField_021x, 16));  //�����ڼ���
	m_pDevCmn->fnSCR_DisplayString(2, strTemp);

/*	if(Accept.JiroTranCode == TC_ELECINQ)
	{
		strTemp.Format("%s", strCT(m_RXLOCALTAXDATA.TotalAmount, 12));  // �Ѱ����ݾ�
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
		strTemp.Format("%d", nTotalAmount);  // �Ѱ����ݾ�
		m_pDevCmn->fnSCR_DisplayAmount(3, strTemp);
		m_pDevCmn->fnSCR_DisplayAmount(9, strTemp);
		m_pDevCmn->fnSCR_DisplayAmount(10, (LPCTSTR)m_RD.byDataField_024x); //������
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
				strGetBufferArray[j*2] = strTemp;    //#SJ 1115 ��ǥ ������ ����
				strGetBufferArray[j*2 + 1] = strT1;

				j += 1;  //#SJ 1115 ��ġ����
			}

			slen += 69;
		}
		strTemp.Format("%d", nTotalAmount);  // �Ѱ����ݾ�
		m_pDevCmn->fnSCR_DisplayAmount(3, strTemp);
		m_pDevCmn->fnSCR_DisplayAmount(9, strTemp);
		m_pDevCmn->fnSCR_DisplayAmount(10, (LPCTSTR)m_RD.byDataField_024x); //������
	}
	Int2Asc(j, Accept.JiroBillCount, sizeof(Accept.JiroBillCount));  

	ScrNum = 3016;

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_15_WAIT, PIN_MENU_MODE1);  //#N0173
	GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
	
	if (GetKeyStr == S_CONFIRM)
		return T_OK;
	else
	if (GetKeyStr == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
	else 
	if (GetKeyStr == S_TIMEOVER)							// TIMEOVER
		return T_OK;										//#SJ 1115 Ÿ�Ӿƿ��� ��ǥ �̹��� ����
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
		fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
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
	m_pDevCmn->fnSCR_DisplayString(4, "����ICī�� ����");

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum);

	Delay_Msg(3000);

	return T_OK;
}


//#N0174 ��ȯ ������
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

	
	strTemp.Format("%15.15s  %25.25s", &m_RXEASYONEDATA.EasyOne_Acc[0][0], &m_RXEASYONEDATA.EasyOne_Acc[0][15]);  //������ ���� 1
	m_pDevCmn->fnSCR_DisplayString(1, strTemp);

	strTemp.Format("%35.35s",&m_RXEASYONEDATA.EasyOne_Acc[0][40]);  //������ ���� 1
	m_pDevCmn->fnSCR_DisplayString(2, strTemp);

	strTemp.Format("%15.15s  %25.25s", &m_RXEASYONEDATA.EasyOne_Acc[1][0], &m_RXEASYONEDATA.EasyOne_Acc[1][15]);  //������ ���� 1
	m_pDevCmn->fnSCR_DisplayString(3, strTemp);

	strTemp.Format("%35.35s", &m_RXEASYONEDATA.EasyOne_Acc[1][40]);  //������ ���� 1
	m_pDevCmn->fnSCR_DisplayString(4, strTemp);

	strTemp.Format("%15.15s  %25.25s", &m_RXEASYONEDATA.EasyOne_Acc[2][0], &m_RXEASYONEDATA.EasyOne_Acc[2][15]);  //������ ���� 1
	m_pDevCmn->fnSCR_DisplayString(5, strTemp);

	strTemp.Format("%35.35s",  &m_RXEASYONEDATA.EasyOne_Acc[2][40]);  //������ ���� 1
	m_pDevCmn->fnSCR_DisplayString(6, strTemp);

	strTemp.Format("%15.15s  %25.25s", &m_RXEASYONEDATA.EasyOne_Acc[3][0], &m_RXEASYONEDATA.EasyOne_Acc[3][15]);  //������ ���� 1
	m_pDevCmn->fnSCR_DisplayString(7, strTemp);

	strTemp.Format("%35.35s", &m_RXEASYONEDATA.EasyOne_Acc[3][40]);  //������ ���� 1
	m_pDevCmn->fnSCR_DisplayString(8, strTemp);

	strTemp.Format("%15.15s  %25.25s", &m_RXEASYONEDATA.EasyOne_Acc[4][0], &m_RXEASYONEDATA.EasyOne_Acc[4][15]);  //������ ���� 1
	m_pDevCmn->fnSCR_DisplayString(9, strTemp);

	strTemp.Format("%35.35s",  &m_RXEASYONEDATA.EasyOne_Acc[4][40]);  //������ ���� 1
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
		fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
	else
	if (GetKeyStr == S_CANCEL2)
		fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
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
		fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
	else
	if (GetKeyStr == S_CANCEL2)
		fnAPP_CancelProc(T_CANCEL);							// �ŷ����ó��
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

	if (!PPPRecvData.ParaLength)								// DATA��/���������Ʈ(�ŷ�)
		return T_OK;											// �ش��ڷṫ

	PbDataCnt = 0;												// Pb��±���
	memset(PbData, 0, sizeof(PbData));							// Pb��¿���

	memcpy(&PbData[PbDataCnt], PbFrameSet, sizeof(PbFrameSet));

	PbDataCnt += sizeof(PbFrameSet);

	for(i = 0; i <= 23; i++) 
	{
		if (!PPPRecvData.DataLength[0][i])
			PbData[PbDataCnt++] = PbNewLine;
		else break;
		if (i == 11)											// ����� skip
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
			// ������ �ִ� ��� ���̰� ���ϵȴ�.
			if (szPbLen)
			{
				memcpy(&PbData[PbDataCnt], PPPRecvData.Data[i][j], szPbLen);
				PbDataCnt += szPbLen;
				PbData[PbDataCnt++] = PbNewLine;
				// 1�� �μ��� 13������ �ǳʰ� ��� �ζ����� �ǵ��Ѵ�.
				if (j == 11) 
				{
					PbData[PbDataCnt++] = PbNewLine;
					PbData[PbDataCnt++] = PbNewLine;
				}
				// ������ �������� �μ��� ��� ���� �������� �Ѿ��.
				else if (j == 23)
				{
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_PPPProc : LocalPage[%d], PPPRecvData.Ep[%d], i[%d]", LocalPage, PPPRecvData.Ep, i );
					if (i != PPPRecvData.Ep)					// 2002.02.25
					{
						memcpy(&PbData[PbDataCnt], PbFrameNextSet, sizeof(PbFrameNextSet));
						PbDataCnt += sizeof(PbFrameNextSet);
					}
////////
// j = 23 ��� �������� �� ���� 
// ������ 
// ����Ϸ��� �ٽ� ���� ��Ʈ�� (���� ���Դ�� �ð� Ÿ�� Ȯ��, �����ð�����ϰ�쿡 ���� ����Ÿ ó��)
// ���� ������ 
// ���ó��(���Ե� ������ ���� ����� ������ Ȯ��)
// ��� �÷ο� ����


				}
				// �μ��ϰ��� �ϴ� �����Ͱ� 1k block�� �����ʵ��� �Ѵ�.
				if (PbDataCnt > (sizeof(PbData) / 2))			// 512/1024�ʰ���(50%)
				{
					// pbm���� �����͸� Passing�Ѵ�.
					fnAPD_PbPrint(TRUE);
					memset(PbData, 0, sizeof(PbData));
					PbDataCnt = 0;
				}
			}
			else break;
		}

		startline = 0;

		if (PbDataCnt)											// �������ڷ������
		{
			if (!m_pDevCmn->fnAPL_CheckDeviceAction(DEV_PBM))	// ���ۿϷ����&��ְ���
				return T_ERROR;
			m_pDevCmn->fnPBM_Print(MakeUnPack(PbData, PbDataCnt));	// ����(�����ڷ�)
			if (!m_pDevCmn->fnAPL_CheckDeviceAction(DEV_PBM))	// ���ۿϷ����&��ְ���
				return T_ERROR;

			memset(PbData, 0, sizeof(PbData));
			PbDataCnt = 0;
		}	
		
		memcpy(PbData, PbFrameSet, sizeof(PbFrameSet));
		PbDataCnt = sizeof(PbFrameSet);
	}

	return T_OK;
}