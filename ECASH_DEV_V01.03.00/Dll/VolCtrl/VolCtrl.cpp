// VolCtrl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE VolCtrlDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("VOLCTRL.DLL Initializing!\n");
		
		if (!AfxInitExtensionModule(VolCtrlDLL, hInstance))
			return 0;

		new CDynLinkLibrary(VolCtrlDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("VOLCTRL.DLL Terminating!\n");
		AfxTermExtensionModule(VolCtrlDLL);
	}
	return 1;
}
