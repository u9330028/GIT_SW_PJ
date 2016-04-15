/////////////////////////////////////////////////////////////////////////////
//	DevNet.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevRpcLockCtrlInc.h"


#include "..\..\H\Dll\DevEtc.h"
#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/************************************************************************************************/
/* Desc: Rpc + Lock Control 
/* Date: 2010.03.29
/* Name: Kim Gi Jin 
/* Load: NHRDCTRL.dll		  
/************************************************************************************************/
/*==============================================================================
* Function Name : fnLDCtrl_Initialize()
* Description   : Lock connected
* Parameters    : None
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/


int	CDevCmn::fnLDCtrl_Initialize()
{
	int		nReturn = FALSE;
	int		nRegFlag = FALSE;

	CString strPath("");
	CString strYYMMDD("");
	char szTemp[1024];

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_Initialize()");

	if (fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_ConnectRpcLock();

		if(nReturn == TRUE)
		{
			Delay_Msg(100);
			if(fnLDCtrl_GetDeviceStatus() != NORMAL)
				nReturn = FALSE;	
			else
			{
				Delay_Msg(200);
				fnLDCtrl_SetTimeWatchdogRpc(0);	// Regstry Setting
				Delay_Msg(200);
				fnLDCtrl_Ch1TimeResetRpc(0);	// Regstry Setting
				Delay_Msg(200);
				fnLDCtrl_Ch2TimeResetRpc(0);	// Regstry Setting
				Delay_Msg(200);
				fnLDCtrl_EnableWatchdogRpc(0);	

				if(!m_pDevCmn->fnLDCtrl_GetKeyCount())
				{
					fnLDCtrl_SetKeytableFile(0, "51515150");
					fnLDCtrl_GetErrorCode(szTemp);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_Initialize():[%s]", szTemp);
				}
			}
		}
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_Initialize():return(%d)", nReturn);
	return nReturn;
}
/*==============================================================================
* Function Name : fnLDCtrl_Deinitialize()
* Description   : Lock Disconnected
* Parameters    : None
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_Deinitialize()
{
	int		nReturn = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_Deinitialize()");

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_DisconnectRpcLock();
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnLDCtrl_OpenRpcLock()
* Description   : Lock Open
* Parameters    : - lpSetOpenKey : Null (강제 Open), KeyValue
*                 - nOption      : 0(키점검), 1(키 매치 후 Lock Open), 2(강제 OPEN)
* Return        : 
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_OpenRpcLock(LPCTSTR lpSetOpenKey, int nOption)
{
	int		nReturn = FALSE;
	int		nStatus = FALSE;
	CString strOpenKey(lpSetOpenKey);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_OpenRpcLock(%s, %d)", strOpenKey.GetBuffer(0), nOption);

	nStatus = fnLDCtrl_GetDeviceStatus();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_OpenRpcLock():Device Status[%d]", nStatus);
	if(nStatus != WFS_STAT_DEVONLINE)
		return T_ERROR;

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		if(strOpenKey != "")
			nReturn = fnDll_OpenRpcLock((BYTE *)strOpenKey.GetBuffer(0), nOption);
		else
			nReturn = fnDll_OpenRpcLock(NULL, 2);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_OpenLock():return(%d)", nReturn);
	}

	return nReturn;
}

/*==============================================================================
* Function Name : fnLDCtrl_CloseLock()
* Description   : Lock closed
* Parameters    : None
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_CloseLock()
{
	int		nReturn = FALSE;
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_CloseLock()");

	
	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_CloseRpcLock();
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_CloseLock():return(%d)", nReturn);

	return nReturn;
}

/*==============================================================================
* Function Name : fnLDCtrl_GetDoorStatus()
* Description   : Lock is monitoring 
* Parameters    : None
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_GetDoorStatus(int nFlag)
{
	static	int		nSaveReturn = TRUE;
	int				nReturn = TRUE;
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_GetDoorStatus()");

	if (fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_GetLockStatus();
	}

	if (nReturn != nSaveReturn)
	{
		nSaveReturn = nReturn;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_GetDoorStatus():return(%d)", nReturn);
	}


	return nReturn;
}


/*==============================================================================
* Function Name : fnLDCtrl_GetDeviceStatus()
* Description   : Device Connected Check
* Parameters    : None
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_GetDeviceStatus()
{
	static	int		nSaveReturn = -1;
	int				nReturn = -1;
	char	szTemp[10];
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_GetDeviceStatus()");

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_GetDeviceStatus();
	}

	if (nReturn != nSaveReturn)
	{
		nSaveReturn = nReturn;

		if(nReturn)
		{
			fnLDCtrl_GetErrorCode(szTemp);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_GetDeviceStatus - fnLDCtrl_GetErrorCode(%s)", szTemp);
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_GetDeviceStatus():return(%d)", nReturn);
	}

	return nReturn;
}
/*==============================================================================
* Function Name : fnLDCtrl_GetKeyCount()
* Description   : Device Connected Check
* Parameters    : None
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_GetKeyCount()
{
	static	int		nSaveReturn = 0;
	int				nReturn = 0;
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_GetKeyCount()");

	if (fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_GetKeyCount();
	}

	if (nReturn != nSaveReturn)
	{
		nSaveReturn = nReturn;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_GetKeyCount():return(%d)", nReturn);
	}


	return nReturn;
}

/*==============================================================================
* Function Name : fnLDCtrl_GetLoadKeyCheckFlag()
* Description   : Key Check counted
* Parameters    : None
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_GetLoadKeyCheckFlag(LPCTSTR lpSetOpenKey)
{
	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;
	ULONG			nWaitTime = 0;
	int				nRet = FALSE;
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_GetLoadKeyCheckFlag(%s)", lpSetOpenKey);

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		
		fnLDCtrl_OpenRpcLock(lpSetOpenKey, 0);
		nWaitTime = TimerSet(5);
		while (!CheckTimer(nWaitTime))
		{
			nRet = fnDll_GetLoadKeyCheckFlag();
			if(nRet == LOCK_STAT_CHECK_OK)
			{
				nReturn = LOCK_STAT_CHECK_OK;
				break;
			}
			else
			if(nRet == LOCK_STAT_CHECK_NG)
			{
				nReturn = LOCK_STAT_CHECK_NG;
				break;
			}
			Sleep(200);
		}

		
	}

	if (nReturn != nSaveReturn)
	{
		nSaveReturn = nReturn;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_GetLoadKeyCheckFlag():return(%d)", nReturn);
	}

	return nReturn;
}
/*==============================================================================
* Function Name : fnLDCtrl_SetClearLogandKey()
* Description   : Keytalbe cleard
* Parameters    : bClearKeyTable, bClearLog (Ignore)
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_SetClearLogandKey(BOOL bClearKeyTable, BOOL bClearLog)
{
	int		nReturn = FALSE;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_SetClearLogandKey( %d, %d)", bClearKeyTable, bClearLog);

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_ClearLogNKey(bClearKeyTable, bClearLog);
	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_SetClearLogandKey():return(%d)", nReturn);

	return nReturn;
}

/*==============================================================================
* Function Name : fnLDCtrl_SetKeytableFile()
* Description   : Keytalbe is setting
* Parameters    : nKeyType(0,1), pData(Key Number 8 byte)
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_SetKeytableFile(int nKeyType, void* pData)
{
	int		nReturn = FALSE;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_SetKeytableFile( %s, %d)", pData, nKeyType);

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_TMKeyDownload(nKeyType, pData);
	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_SetKeytableFile():return(%d)", nReturn);

	return nReturn;
}

/*==============================================================================
* Function Name : fnLDCtrl_GetErrorCode()
* Description   : GetErrorCode
* Parameters    : None
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_GetErrorCode(char* szReturnValue)
{
	int		nReturn = FALSE;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_GetErrorCode()");

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_GetErrorCode(szReturnValue);
	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_GetErrorCode():return(%s)", szReturnValue);

	return nReturn;
}

/*==============================================================================
* Function Name : fnLDCtrl_PowerOnRpc()
* Description   : RPC Poweron
* Parameters    : nPort : 1(Main), 2(Modem)
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_PowerOnRpc(int nPort)
{
	int		nReturn = FALSE;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_PowerOnRpc():Port(%d)", nPort);

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_PowerOnRpc(nPort);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_PowerOnRpc():Return(%d)", nReturn);
	}

	return nReturn;
}
/*==============================================================================
* Function Name : fnLDCtrl_PowerOffRpc()
* Description   : RPC PowerOff
* Parameters    : nPort : 1(Main), 2(Modem)
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_PowerOffRpc(int nPort)
{
	int		nReturn = FALSE;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_PowerOffRpc():Port(%d)", nPort);

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_PowerOffRpc(nPort);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_PowerOffRpc():Return(%d)", nReturn);
	}

	return nReturn;
}

/*==============================================================================
* Function Name : fnLDCtrl_PowerStatusRpc() - Not use
* Description   : RPC PowerStatus
* Parameters    : None
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_PowerStatusRpc(int nPort)
{
	int		nReturn = FALSE;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_PowerStatusRpc():Port(%d)", nPort);

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_PowerStatusRpc(nPort);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_PowerStatusRpc():Return(%d)", nReturn);
	}

	return nReturn;
}

/*==============================================================================
* Function Name : fnLDCtrl_PowerResetRpc() - Not use
* Description   : RPC PowerReset
* Parameters    : nPort : 1(Main), 2(Modem)
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_PowerResetRpc(int nPort)
{
	int		nReturn = FALSE;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_PowerResetRpc():Port(%d)", nPort);

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_PowerResetRpc(nPort);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_PowerResetRpc():Return(%d)", nReturn);
	}

	return nReturn;
}

/*==============================================================================
* Function Name : fnLDCtrl_EnableWatchdogRpc() 
* Description   : RPC Watchdog use
* Parameters    : nPort : 1(Main), 2(Modem)
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_EnableWatchdogRpc(int nPort)
{
	int		nReturn = FALSE;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_EnableWatchdogRpc():Port(%d)", nPort);

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_EnableWatchdogRpc(nPort);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_EnableWatchdogRpc():Return(%d)", nReturn);
	}

	return nReturn;
}

/*==============================================================================
* Function Name : fnLDCtrl_DisableWatchdogRpc() 
* Description   : RPC Watchdog don't use
* Parameters    : nPort : 1(Main), 2(Modem)
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_DisableWatchdogRpc(int nPort)
{
	int		nReturn = FALSE;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_DisableWatchdogRpc():Port(%d)", nPort);

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_DisableWatchdogRpc(nPort);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_DisableWatchdogRpc():Return(%d)", nReturn);
	}

	return nReturn;
}

/*==============================================================================
* Function Name : fnLDCtrl_SetTimeWatchdogRpc() 
* Description   : RPC Watchdog set Time
* Parameters    : nInterval (10 ~ 2500 sec)
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_SetTimeWatchdogRpc(int nInterval)
{
	int		nReturn = FALSE;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_SetTimeWatchdogRpc():nInterval(%d)", nInterval);

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_SetTimeWatchdogRpc(nInterval);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_SetTimeWatchdogRpc():Return(%d)", nReturn);
	}

	return nReturn;
}

/*==============================================================================
* Function Name : fnLDCtrl_Ch1TimeResetRpc() 
* Description   : RPC Reset Time
* Parameters    : nInterval (10 ~ 2500 sec)
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_Ch1TimeResetRpc(int nInterval)
{
	int		nReturn = FALSE;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_Ch1TimeResetRpc():nInterval(%d)", nInterval);

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_Ch1TimeResetRpc(nInterval);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_Ch1TimeResetRpc():Return(%d)", nReturn);
	}

	return nReturn;
}

/*==============================================================================
* Function Name : fnLDCtrl_Ch2TimeResetRpc() 
* Description   : RPC Reset Time
* Parameters    : nInterval (10 ~ 2500 sec)
* Return        : None
* Release       : Ver 1.0
* Notes			: 2010.04.01 Kim Gi Jin
* ============================================================================*/
int	CDevCmn::fnLDCtrl_Ch2TimeResetRpc(int nInterval)
{
	int		nReturn = FALSE;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_Ch2TimeResetRpc():nInterval(%d)", nInterval);

	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		nReturn = fnDll_Ch2TimeResetRpc(nInterval);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnLDCtrl_Ch2TimeResetRpc():Return(%d)", nReturn);
	}

	return nReturn;
}