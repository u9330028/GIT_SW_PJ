#define REG_KEY_RDCTRL_DEVINFO		("SOFTWARE\\ATM\\DevInfo\\RDCTRL")
#define REG_KEY_RDCTRL_DEVSTATUS	("SOFTWARE\\ATM\\DevStatus\\RDCTRL")
#define REG_KEY_RDCTRL_EP_VER		("SOFTWARE\\ATM\\EP_Version")
#define REG_KEY_RDCTRL_SP_VER		("SOFTWARE\\ATM\\SP_Version")
#define REG_KEY_RDCTRL_ERRORCODE	("SOFTWARE\\ATM\\ErrorCode")

#define LOCK_STAT_CHECK_NONE                    (0)
#define LOCK_STAT_CHECK_OK		                (1)
#define LOCK_STAT_CHECK_NG                      (2)

#define WFS_STAT_DEVONLINE                      (0)
#define WFS_STAT_DEVOFFLINE                     (1)
#define WFS_STAT_DEVPOWEROFF                    (2)
#define WFS_STAT_DEVNODEVICE                    (3)
#define WFS_STAT_DEVHWERROR                     (4)
#define WFS_STAT_DEVUSERERROR                   (5)
#define WFS_STAT_DEVBUSY                        (6)

//Lock Control Access
AFX_API_EXPORT int	fnDll_ConnectRpcLock();
AFX_API_EXPORT BOOL fnDll_DisconnectRpcLock();
AFX_API_EXPORT int	fnDll_OpenRpcLock(BYTE* szKey, int nOption);
AFX_API_EXPORT BOOL fnDll_CloseRpcLock();
AFX_API_EXPORT BOOL fnDll_ResetRpcLock();
AFX_API_EXPORT BOOL fnDll_TMKeyDownload(int nKeyType, void* pData);
AFX_API_EXPORT int	fnDll_GetLockStatus();
AFX_API_EXPORT int	fnDll_GetDeviceStatus();
AFX_API_EXPORT int	fnDll_GetLoadKeyCheckFlag();
AFX_API_EXPORT int	fnDll_GetErrorCode(char* szReturnValue);
AFX_API_EXPORT BOOL fnDll_ClearLogNKey(BOOL bClearKeyTable, BOOL bClearLog);
AFX_API_EXPORT int	fnDll_GetKeyCount();

//Rpc Control Access
AFX_API_EXPORT int fnDll_PowerOnRpc(int nPort = 3);
AFX_API_EXPORT int fnDll_PowerOffRpc(int nPort = 3);
AFX_API_EXPORT int fnDll_PowerStatusRpc(int nPort = 0);
AFX_API_EXPORT int fnDll_PowerResetRpc(int nPort = 3);
AFX_API_EXPORT int fnDll_EnableWatchdogRpc(int nPort = 0);
AFX_API_EXPORT int fnDll_DisableWatchdogRpc(int nPort = 0);
AFX_API_EXPORT int fnDll_SetTimeWatchdogRpc(int nInterval = 0);
AFX_API_EXPORT int fnDll_Ch1TimeResetRpc(int nInterval= 0);
AFX_API_EXPORT int fnDll_Ch2TimeResetRpc(int nInterval= 0);
