#include "stdafx.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int	CDevCmn::fnSPACK_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_Initialize()");

	int		nReturn = FALSE;
	int		nGetExternalS1EnSt = FALSE;
	int		nGetExternalS2EnSt = FALSE;
	int		nGetExternalS1 = FALSE;
	int		nGetExternalS2 = FALSE;
	int		nGSensor = FALSE;
	
	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		m_NHAlertSpack->OpenConnection();
		ULONG nWaitTime = TimerSet(5);
		while (!CheckTimer(nWaitTime))
		{
			Delay_Msg();
			if(fnSPACK_GetDeviceStatus() == TRUE)
			{

				Delay_Msg(5000);
				nGetExternalS1EnSt = fnSPACK_GetExternalSensorEnableStatus_1();
				nGetExternalS2EnSt = fnSPACK_GetExternalSensorEnableStatus_2();

				if(nGetExternalS1EnSt)
					IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19", 1);

				if(nGetExternalS2EnSt)
					IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19", 2);

				if(nGetExternalS1EnSt && nGetExternalS2EnSt)
					IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19", 3);

				if(!nGetExternalS1EnSt && !nGetExternalS2EnSt)
					IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19", 0);


				nGetExternalS1 = fnSPACK_GetExternalSensorStatus_1();
				nGetExternalS2 = fnSPACK_GetExternalSensorStatus_2();
				nGSensor = fnSPACK_GetGSensor();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "nGetExternalS1EnSt(%d), nGetExternalS2EnSt(%d), nGetExternalS1(%d), nGetExternalS2(%d), nGSensor(%d)", nGetExternalS1EnSt, nGetExternalS2EnSt, nGetExternalS1, nGetExternalS2, nGSensor);

				nReturn = TRUE;
				break;
			}					
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_Initialize():return(%d)", nReturn);
	return nReturn;
}


int	CDevCmn::fnSPACK_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_Deinitialize()");

	int		nReturn = FALSE;
	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		if(m_NHAlertSpack !=  NULL)
		{	
			m_NHAlertSpack->CloseConnection();
//			delete m_NHAlertSpack;
//			m_NHAlertSpack = NULL;
		}
	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_Deinitialize():nReturn(%d)", nReturn);

	return nReturn;

}


// H/w Init
int	CDevCmn::fnSPACK_ReSet()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_ReSet()");

	int		nReturn = FALSE;
	static	int		nSaveReturn = FALSE;
	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		nReturn = m_NHAlertSpack->Reset();
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_ReSet():nReturn(%d)", nReturn);
}
	return nReturn;

}

int	CDevCmn::fnSPACK_AlarmOnOff(int nOnOff)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_AlarmOnOff(%d)", nOnOff);

	int		nReturn = FALSE;
	static	int		nSaveReturn = FALSE;
	
	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		if(nOnOff)
		{
			ULONG nWaitTime = TimerSet(4);
			while (!CheckTimer(nWaitTime))
			{
				Delay_Msg(300);
				nReturn = m_NHAlertSpack->AlarmOff();
				if(nReturn < 0)
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_AlarmOnOffnReturn (%d)", nReturn);


				if(nReturn > 0) break;
			}
		}
		else
			nReturn = m_NHAlertSpack->AlarmOn();
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_AlarmOnOff():nReturn(%d)", nReturn);
}
	return nReturn;

}

// SetEnableControl(int byAlramEnable:1 Use, 0 Notuse, int byExternal1Enable, int byExternal2Enable) 
int	CDevCmn::fnSPACK_SetEnableControl(int nAlramEnable, int nExternal1Enable, int nExternal2Enable)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_SetEnableControl(%d, %d, %d)", nAlramEnable, nExternal1Enable, nExternal2Enable);

	int		nReturn = FALSE;
	static	int		nSaveReturn = FALSE;
	
	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		if(fnSPACK_GetDeviceStatus() == TRUE)
		{
			nReturn = m_NHAlertSpack->SetEnableControl(nAlramEnable, nExternal1Enable, nExternal2Enable);
		}
		else 
			nReturn = NODEVICE;

	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_SetEnableControl():nReturn(%d)", nReturn);
}
	return nReturn;

}


// Get Connection Status() (0, 1:normal)
int	CDevCmn::fnSPACK_GetDeviceStatus()
{
	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;

	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		nReturn = m_NHAlertSpack->GetDeviceStatus();
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetDeviceStatus():nReturn(%d)", nReturn);
}
	return nReturn;

}


// Get G-Sensor status (1 ~ 63) - default : 6
int	CDevCmn::fnSPACK_GetGSensorSensibility()
{
	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;

	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		if(fnSPACK_GetDeviceStatus() == TRUE)
			nReturn = m_NHAlertSpack->GetGSensorSensibility();
		else
			nReturn = 256;

	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GetGSensorSensibility():nReturn(%d)", nReturn);
}
	return nReturn;

}


// Get Alarm Status (Use : true, Not Use : false)
int	CDevCmn::fnSPACK_GetAlarmStatus()
{
	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;
	int				nGetExternalS1 = FALSE;
	int				nGetExternalS2 = FALSE;
	static int		nSaveGSensor = FALSE;

	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		if(fnSPACK_GetDeviceStatus() == TRUE)
		{

			nReturn = m_NHAlertSpack->GetAlarmStatus();
			if(nReturn)
			{
				GExternalSensorStatus1 = fnSPACK_GetExternalSensorStatus_1();
				GExternalSensorStatus2 = fnSPACK_GetExternalSensorStatus_2();				
				GSensorStatus = fnSPACK_GetGSensor();
				if(nSaveGSensor != GSensorStatus)
				{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetAlarmStatus()_01:nSaveGSensor(%d), GSensorStatus(%d)", nSaveGSensor, GSensorStatus);		
					if(nSaveGSensor == TRUE && GSensorStatus == FALSE)
					{

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetAlarmStatus()_02:nSaveGSensor(%d), GSensorStatus(%d)", nSaveGSensor, GSensorStatus);
						int nGetIni = 0;
						nGetIni = IniGetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19");
						if(nGetIni == 1)
							m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_ON, CLKEY_OFF);
						else
						if(nGetIni == 2)
							m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_OFF, CLKEY_ON);
						else
						if(nGetIni == 3)
							m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_ON, CLKEY_ON);
						else
							m_pDevCmn->fnSPACK_SetEnableControl(CLKEY_OFF, CLKEY_OFF, CLKEY_OFF);
						
						fnSPACK_AlarmOnOff(TRUE);
					}

					nSaveGSensor = GSensorStatus;
				}

			}

		}
		else
			nReturn = NODEVICE;

	}


if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	if(ABNORMAL_POS == nSaveReturn)
	{
		fnAPL_CaptureFaceAtmShocked(10);
		fnAPL_CaptureFaceAtmShocked(11);
		fnAPL_CaptureFaceAtmShocked(12);
		fnAPL_CaptureFaceAtmShocked(13);
		fnAPL_CaptureFaceAtmShocked(14);
		fnAPL_CaptureFaceAtmShocked(15);
	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GExternalSensorStatus1():nReturn(%d)", GExternalSensorStatus1);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GExternalSensorStatus2():nReturn(%d)", GExternalSensorStatus2);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "GSensorStatus():nReturn(%d)", GSensorStatus);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetAlarmStatus():nReturn(%d)", nReturn);
}
	return nReturn;

}


// Release Button (Not use)
int	CDevCmn::fnSPACK_GetButtonPushStatus()
{
	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetButtonPushStatus()");

	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		nReturn = m_NHAlertSpack->GetButtonPushStatus();
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetAlarmStatus():nReturn(%d)", nReturn);
}
	return nReturn;
}


// ExternalSensor1Status (0, 1 = active)
int	CDevCmn::fnSPACK_GetExternalSensorStatus_1()
{
	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetExternalSensorStatus_1()");

	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		nReturn = m_NHAlertSpack->GetExternalSensorStatus_1();
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetExternalSensorStatus_1():nReturn(%d)", nReturn);
}
	return nReturn;
}


// ExternalSensor2Status(0, 1 = active)
int	CDevCmn::fnSPACK_GetExternalSensorStatus_2()
{
	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetExternalSensorStatus_2()");

	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		nReturn = m_NHAlertSpack->GetExternalSensorStatus_2();
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetExternalSensorStatus_2():nReturn(%d)", nReturn);
}
	return nReturn;
}

// BatteryLowStatus (0 = normal, 1 = low)
int	CDevCmn::fnSPACK_GetBatteryLowStatus()
{
	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetBatteryLowStatus()");

	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		if(fnSPACK_GetDeviceStatus() == TRUE)
		{
			nReturn = m_NHAlertSpack->GetBatteryLowStatus();
		}
		else
			nReturn = NODEVICE;

	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetBatteryLowStatus():nReturn(%d)", nReturn);
}
	return nReturn;
}

// ExternalSensorEnableStatus_1(0, 1 = enable)
int	CDevCmn::fnSPACK_GetExternalSensorEnableStatus_1()
{
	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetExternalSensorEnableStatus_1()");

	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		nReturn = m_NHAlertSpack->GetExternalSensorEnableStatus_1();
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetExternalSensorEnableStatus_1():nReturn(%d)", nReturn);
}
	return nReturn;
}

// ExternalSensorEnableStatus_2(0, 1 = enable)
int	CDevCmn::fnSPACK_GetExternalSensorEnableStatus_2()
{
	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetExternalSensorEnableStatus_2()");

	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		nReturn = m_NHAlertSpack->GetExternalSensorEnableStatus_2();
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetExternalSensorEnableStatus_2():nReturn(%d)", nReturn);
}
	return nReturn;
}

// DipSwitchStatus_1 (Not use)
int	CDevCmn::fnSPACK_GetDipSwitchStatus_1()
{
	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetDipSwitchStatus_1()");

	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		nReturn = m_NHAlertSpack->GetDipSwitchStatus_1();
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetDipSwitchStatus_1():nReturn(%d)", nReturn);
}
	return nReturn;
}

// DipSwitchStatus_2 (Not use)
int	CDevCmn::fnSPACK_GetDipSwitchStatus_2()
{
	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetDipSwitchStatus_2()");

	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		nReturn = m_NHAlertSpack->GetDipSwitchStatus_2();
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetDipSwitchStatus_2():nReturn(%d)", nReturn);
}
	return nReturn;
}

// GSensr
int	CDevCmn::fnSPACK_GetGSensor()
{
	static	int		nSaveReturn = FALSE;
	int				nReturn = FALSE;
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetGSensor()");

	if(fnAPL_GetDefineDevice(DEV_SPG))
	{
		nReturn = m_NHAlertSpack->GetGSensor();
	}

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSPACK_GetGSensor():nReturn(%d)", nReturn);
}
	return nReturn;
}