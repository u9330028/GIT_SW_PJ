// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "nhalert4spackx.h"

/////////////////////////////////////////////////////////////////////////////
// CNHAlert4SPackX

IMPLEMENT_DYNCREATE(CNHAlert4SPackX, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CNHAlert4SPackX properties

long CNHAlert4SPackX::GetGSensorSensibility()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void CNHAlert4SPackX::SetGSensorSensibility(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long CNHAlert4SPackX::GetAlarmStatus()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void CNHAlert4SPackX::SetAlarmStatus(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long CNHAlert4SPackX::GetButtonPushStatus()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void CNHAlert4SPackX::SetButtonPushStatus(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long CNHAlert4SPackX::GetDeviceStatus()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void CNHAlert4SPackX::SetDeviceStatus(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

long CNHAlert4SPackX::GetGSensor()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void CNHAlert4SPackX::SetGSensor(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

long CNHAlert4SPackX::GetEnableAlarmStatus()
{
	long result;
	GetProperty(0xc, VT_I4, (void*)&result);
	return result;
}

void CNHAlert4SPackX::SetEnableAlarmStatus(long propVal)
{
	SetProperty(0xc, VT_I4, propVal);
}

long CNHAlert4SPackX::GetExternalSensorStatus_1()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}

void CNHAlert4SPackX::SetExternalSensorStatus_1(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}

long CNHAlert4SPackX::GetExternalSensorStatus_2()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void CNHAlert4SPackX::SetExternalSensorStatus_2(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

long CNHAlert4SPackX::GetExternalSensorEnableStatus_1()
{
	long result;
	GetProperty(0xf, VT_I4, (void*)&result);
	return result;
}

void CNHAlert4SPackX::SetExternalSensorEnableStatus_1(long propVal)
{
	SetProperty(0xf, VT_I4, propVal);
}

long CNHAlert4SPackX::GetExternalSensorEnableStatus_2()
{
	long result;
	GetProperty(0x10, VT_I4, (void*)&result);
	return result;
}

void CNHAlert4SPackX::SetExternalSensorEnableStatus_2(long propVal)
{
	SetProperty(0x10, VT_I4, propVal);
}

long CNHAlert4SPackX::GetBatteryLowStatus()
{
	long result;
	GetProperty(0x11, VT_I4, (void*)&result);
	return result;
}

void CNHAlert4SPackX::SetBatteryLowStatus(long propVal)
{
	SetProperty(0x11, VT_I4, propVal);
}

long CNHAlert4SPackX::GetDipSwitchStatus_1()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}

void CNHAlert4SPackX::SetDipSwitchStatus_1(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}

long CNHAlert4SPackX::GetDipSwitchStatus_2()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}

void CNHAlert4SPackX::SetDipSwitchStatus_2(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}

long CNHAlert4SPackX::GetACStopStatus()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}

void CNHAlert4SPackX::SetACStopStatus(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CNHAlert4SPackX operations

long CNHAlert4SPackX::OpenConnection()
{
	long result;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CNHAlert4SPackX::CloseConnection()
{
	long result;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CNHAlert4SPackX::Reset()
{
	long result;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CNHAlert4SPackX::AlarmOn()
{
	long result;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CNHAlert4SPackX::AlarmOff()
{
	long result;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CNHAlert4SPackX::SetEnableControl(long byAlramEnable, long byExternal1Enable, long byExternal2Enable)
{
	long result;
	static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms, byAlramEnable, byExternal1Enable, byExternal2Enable);
	return result;
}