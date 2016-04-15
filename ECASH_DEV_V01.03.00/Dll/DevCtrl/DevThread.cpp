/////////////////////////////////////////////////////////////////////////////
//	DevThread.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "direct.h"
#include "shlwapi.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Dll\Profile.h"
#include "..\..\H\Dll\DevCmn.h"

#include <Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
// VFD Display Thread
//////////////////////////////////////////////////////////////////////////
UINT thrVfdThread(LPVOID pParam)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "thrVfdThread() Start!");

	int		nCount = 0;
	int		bDisplayFlag = 0;
	CDevCmn*	m_pDevCmn = (CDevCmn*)pParam;
	
	Delay_Msg(500);
	m_pDevCmn->m_nVfdDoing = TRUE;
	m_pDevCmn->fnVFD_ClearDisplay();
	Delay_Msg(500);

	while (m_pDevCmn->m_nVfdDoing)
	{
		if (m_pDevCmn->fnAPL_GetDefineDevice(DEV_VFD))
		{
			nCount++;

			switch (nCount / (m_pDevCmn->VFDDisTerm * 2))
			{
			case VFD_DISPLAY:
				if (!bDisplayFlag)
				{
					bDisplayFlag++;
					m_pDevCmn->fnAPL_VFDDislpay(VFD_DISPLAY);
				}
				break;
			case VFD_BLINK:
				if (1 == bDisplayFlag)
				{
					bDisplayFlag++;
					m_pDevCmn->fnAPL_VFDDislpay(VFD_BLINK);
				}
				break;
			case VFD_SCROLL:
				if (2 == bDisplayFlag)
				{												
					bDisplayFlag++;
					m_pDevCmn->fnAPL_VFDDislpay(VFD_SCROLL);
				}												
				break;
			case 3:
				bDisplayFlag = 0;
				nCount = 0;
				break;
			default:
				break;
			}
		}
		
		Delay_Msg(500);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "thrVfdThread() End!");
	return 0;
}



//////////////////////////////////////////////////////////////////////////
// thrPollThead Nice Polling Thread (30 Sec) 
//////////////////////////////////////////////////////////////////////////
UINT thrPollThead(LPVOID pParam)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "thrPollThead() Start!");

	int		nCount = 0;
	int		bDisplayFlag = 0;
	CDevCmn*	m_pDevCmn = (CDevCmn*)pParam;
	
	if (m_pDevCmn->fnNET_GetDeviceStatus() != 'O')
	{
		m_pDevCmn->m_nTrPollDoing = FALSE;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "thrPollThead End - 1 ");
		AfxEndThread(0);
		return 0;
	}
	MsgDump(TRACE_DUMP_MODE, "POLL", __FILE__, __LINE__, "m_pDevCmn->m_SendRecvStaus Start (%d)", m_pDevCmn->m_SendRecvStaus);

	while (m_pDevCmn->m_nTrPollDoing)
	{
		Delay_Msg(1000);
		if (m_pDevCmn->m_SendRecvStaus == SENDING)
		{
			MsgDump(TRACE_DUMP_MODE, "POLL", __FILE__, __LINE__, "m_pDevCmn->m_SendRecvStaus SENDING (%d)", m_pDevCmn->m_SendRecvStaus);
			nCount++;	
			continue;
		}
		else
		if (m_pDevCmn->m_SendRecvStaus == RECVING)
		{
			MsgDump(TRACE_DUMP_MODE, "POLL", __FILE__, __LINE__, "m_pDevCmn->m_SendRecvStaus RECVING (%d)", m_pDevCmn->m_SendRecvStaus);
			nCount++;	
			continue;
		}
			
		if (!(nCount % K_60_WAIT))
		{
			m_pDevCmn->fn_fnNET_Polling();
		}

		if (nCount > K_30_WAIT * 20)
			break;

		nCount++;	
	}
	
	MsgDump(TRACE_DUMP_MODE, "POLL", __FILE__, __LINE__, "m_pDevCmn->m_SendRecvStaus End (%d)", m_pDevCmn->m_SendRecvStaus);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "thrPollThead end - 2 ");
	AfxEndThread(0);
	return 0;
}

