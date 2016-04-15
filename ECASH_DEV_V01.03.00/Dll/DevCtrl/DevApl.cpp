/////////////////////////////////////////////////////////////////////////////
//	DevApl.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "direct.h"
#include "shlwapi.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Dll\DevEtc.h"
#include "..\..\H\Dll\U8100ErrorTbl.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"
#include "..\..\H\Dll\BIPConfig.h"


#include "..\..\H\Dll\PowerTTs.h"   
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// 변수초기화
int CDevCmn::fnAPL_InitializeDS()
{

	HKEY     hKeyResult = 0;
	DWORD    dwDisposition = 0;
	DWORD    dwAtmPos = 0;
	LONG     lResult; 
	int		 nTranModeReg;
	int		 nTranModeIni;	
	CString	 strCode("");
	CString	 strTmp("");
	int nNoteKind1 = BRM_NOTEKIND_10000_OLD;			
	int nNoteKind2 = BRM_NOTEKIND_10000_OLD;		
	int nNoteKind3 = BRM_NOTEKIND_10000_OLD;	
	int nNoteKind4 = BRM_NOTEKIND_10000_OLD;		
	char	 szTmp[1024];
	memset(szTmp, 0x20, sizeof(szTmp));

	if(IniGetStr(_SYSTEMCDINI, "SYSTEM", "NAME").Find("WIN7") >= 0)   // WIN7 지원
		OSType = OS_WIN7;
	else
		OSType = 0;
	
	//#0033
//	IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//자행 30
//	IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 100);					//자행 100
//	IniSetInt(_DEVICE_INI, DEV_SEC, "ETCDevice02", 1);						//충격장치	

	int nMachineType = IniGetInt(_DEVICE_INI,DEV_SEC, "MachineType", 0);

	if(nMachineType == U3100K)
	{

		IniSetInt(_DEVICE_INI,DEV_SEC, "ETCDevice10", 1); //무조건 V15 Ver 이상 5만원 거래 
		RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwConvPCU2LCU", 15);
		RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwCompoundOptions", 2);
		//Kim.Gi.Jin 03.30 NICE ********************************************************************************
//		IniSetInt(_DEVICE_INI, DEV_SEC, "ETCDevice20", KIUP);					//KIUP Brand  TEST	#N0215 신한은행 U3100K 지원
		RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwCuMove2RJIfCuEmpty", 3); //카세트 Empty 사용 안함
		RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwMaxDispenseCount", 100); //최대방출금액
		IniSetInt(_DEVICE_INI, DEV_SEC, "FingerHandler", NOT_SET);					 //
		RegSetInt("SOFTWARE\\ATM\\DevInfo\\SIU\\CONF", "DOOR_CABINET_OPEN",  141); 
		RegSetInt("SOFTWARE\\ATM\\DevInfo\\SIU\\CONF", "DOOR_CABINET_CLOSE", 142); 
		RegSetInt("SOFTWARE\\ATM\\DevInfo\\SIU\\CONF", "DOOR_SAFE_OPEN",  59); 
		RegSetInt("SOFTWARE\\ATM\\DevInfo\\SIU\\CONF", "DOOR_SAFE_CLOSE", 60); 
		RegSetInt("SOFTWARE\\ATM\\DEVINFO\\IDC", "ICEntryTimeout", 4); 

		
		RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwResetControl", 0); //Mwi - V10.10.21 (CSM 내 매체 잔류 검지) -> 자동취소 Flag //#0122

		int	nWithdraw50TFlag = IniGetInt(_DEVICE_INI,DEV_SEC, "ETCDevice10", 0); //#0057
		if (nWithdraw50TFlag)
		{
			nNoteKind1 = BRM_NOTEKIND_50000;				// 오만원권 
			nNoteKind2 = BRM_NOTEKIND_10000_NEW;			// 신만원권 
			nNoteKind3 = BRM_NOTEKIND_10000_NEW;			// 신만원권 
			nNoteKind4 = BRM_NOTEKIND_10000_OLD;			//  

			RegSetInt(_REGKEY_BRMSTATUS, "RB1NoteKind", nNoteKind1);
			RegSetInt(_REGKEY_BRMSTATUS, "RB2NoteKind", nNoteKind2);
			RegSetInt(_REGKEY_BRMSTATUS, "RB3NoteKind", nNoteKind3);
			RegSetInt(_REGKEY_BRMSTATUS, "RB4NoteKind", nNoteKind4);

			//#0057
			RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwAccStopIfTypeAllNotAvail", 0); //이권종 Reject 처리 
			RegSetInt("SOFTWARE\\ATM\\DevInfo\\BRM\\DEVICE_INFORMATION\\SYSCONFIG", "dwRejectControl", 0);

			RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwConvPCU2LCU", 15);
			RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwCompoundOptions", 2);
			
			IniSetStr(_TRANS_INI, TRANS_SEC, "ClerkTransOnOff",  "(99)");  
		}
		else
		{
			nNoteKind1 = BRM_NOTEKIND_10000_NEW;			// 신만원권 
			nNoteKind2 = BRM_NOTEKIND_10000_NEW;			// 신만원권 
			nNoteKind3 = BRM_NOTEKIND_INPUT_DEP_ONLY;		// 입금전용 
			nNoteKind4 = BRM_NOTEKIND_10000_OLD;			//  
			
			RegSetInt(_REGKEY_BRMSTATUS, "RB1NoteKind", nNoteKind1);
			RegSetInt(_REGKEY_BRMSTATUS, "RB2NoteKind", nNoteKind2);
			RegSetInt(_REGKEY_BRMSTATUS, "RB3NoteKind", nNoteKind3);
			RegSetInt(_REGKEY_BRMSTATUS, "RB4NoteKind", nNoteKind4);

			IniSetStr(_TRANS_INI, TRANS_SEC, "ClerkTransOnOff",  "(91)");   //#0028 
		}

		
		RegSetStr(_REGKEY_SPRINFO, "ImageDownLoadOn", "0");
        RegSetStr(_REGKEY_SPRINFO, "PrintOption", "0");
		RegSetStr(_REGKEY_SPRINFO, "SetConf", "1");
		RegSetStr(_REGKEY_SPRINFO, "ImageNumber", "1");
		RegSetStr(_REGKEY_SPRINFO, "SelectBackground", "1");
		RegSetInt(_REGKEY_SPRINFO, "MBSupport", 0);
		RegSetStr(_REGKEY_SPRINFO, "PrintImageName1", "C:\\T1ATM\\APP\\Forms\\1.bmp");


	}
	else
	if(nMachineType == U8100)
	{
//		IniSetInt(_DEVICE_INI, DEV_SEC, "ETCDevice20", NONGHYUP);				
//		RegSetStr(_REGKEY_SPRINFO, "Initialize", "0"); -> 
		RegSetInt(_REGKEY_SPRINFO, "TotalColumn", 40);
		RegSetInt(_REGKEY_SPRINFO, "MaxColumn", 40);   
		IniSetStr(_TRANS_INI, TRANS_SEC, "ClerkTransOnOff",  "(99)");			
		IniSetInt(_DEVICE_INI, DEV_SEC, "DesHandler", D_DES_SET); 

		//#CS0002
		RegSetStr(_REGKEY_SPRINFO, "FreeForm", "1");
		
		if (IniGetInt(_DEVICE_INI, DEV_SEC, "ETCDevice20", 0) == KB)		
		{
			IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "PBHandler", 0);
		}
		else
		{
			IniSetInt("D:\\INI\\DEVICE.INI", "DEVICE", "PBHandler", 3);        
		}
	}
	
    //**********************************************************************************************************
//	fnAPL_DeleteFiles(_NHNW30_TESTPAGE_DIR);        //#0053  - 공장 출하 (Installer) - 20130301
//	fnAPL_DeleteFiles(_NHNW20_TESTPAGE_DIR);        //#0053  - 공장 출하 (Installer) - 20130301
	fnAPL_DeleteFiles(EXE_NEXTWARE_LNK);			//#0053
	fnAPL_DeleteFiles(EXE_UBIATOR_LNK);				//#0053
	
	fnAPL_DeleteFiles(EXE_ETC_LNK1);				//#0053
	fnAPL_DeleteFiles(EXE_ETC_LNK2);				//#0053
	fnAPL_DeleteFiles(EXE_ETC_LNK3);				//#0053
	
	RegSetInt(_REGKEY_MCUINFO, "Emboss", 0);

	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_REBOOT, GetDate());
	RegSetStr(_REGKEY_REMOTECTRL, "BID_RECV_RESULT", "NORMAL");



	// Open Key 
	lResult = RegOpenKeyEx(HKEY_CURRENT_USER,"Control Panel\\Desktop",0,KEY_ALL_ACCESS,&hKeyResult);
	if (lResult == ERROR_SUCCESS) 
	{
		  // Set Value On Open Key 
		 RegSetValueEx(hKeyResult,"AutoEndTasks",0,REG_SZ,(CONST BYTE*)"1",2);
		if(OSType == OS_WIN7)					//#N0228 WIN7 지원
			RegSetValueEx(hKeyResult,"HungAppTimeout",0,REG_SZ,(CONST BYTE*)"10000",5);
		else
			RegSetValueEx(hKeyResult,"HungAppTimeout",0,REG_SZ,(CONST BYTE*)"1000",5);
	}
	else 
	{
		  // Create New Key 
		  lResult = RegCreateKeyEx(HKEY_CURRENT_USER,"Control Panel\\Desktop",0,"",
								   REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,
								   NULL, &hKeyResult, &dwDisposition);

		  if (lResult == ERROR_SUCCESS)
		  {
				RegSetValueEx(hKeyResult,"AutoEndTasks",0,REG_SZ,(CONST BYTE*)"1",2);
				if(OSType == OS_WIN7)									//#N0228 WIN7 지원
					RegSetValueEx(hKeyResult,"HungAppTimeout",0,REG_SZ,(CONST BYTE*)"10000",5);
				else
					RegSetValueEx(hKeyResult,"HungAppTimeout",0,REG_SZ,(CONST BYTE*)"1000",5);
		  }
	}
	// Close Key 
	RegCloseKey(hKeyResult);
	RegSetStr("SYSTEM\\CurrentControlSet\\Control", "WaitToKillServiceTimeout", "1000");
//////////////////////////////////////////////////////////////////////////////////////////////////////////

	fnAPL_ClerkInitModeSet();						// 계원초기화모드설정
	DeleteFiles(_LOG_DIR);							// File정리(1달기준)
	DeleteFiles(_TRACE_DIR);

	fnAPL_DeleteFiles(_NHTM_DIR);
	fnAPL_DeleteFiles(_HBRM_DIR);	

	m_pProfile->InitProfileCheck();					// Profile존재유무체크&생성
    m_pProfile->GetDeviceProfile();					// Profile읽기 
	m_pProfile->GetBRMProfile();
	m_pProfile->GetUCMProfile();
	m_pProfile->GetNetworkProfile();
	m_pProfile->GetTransProfile();

	if (m_pProfile->DEVICE.MachineType == U8100)
		m_pProfile->GetLCDInfoProfile();						
	
	Strcpy(m_pProfile->TRANS.ApVersion, VERSION, 9); // Version등록 : VNN-NN-NN(9자리Move처리)
	

	strCode.Format("%8.8s", &m_pProfile->NETWORK.AtmSerialNum[0]);
	IniSetStr("D:\\INI\\ATM_CFG.ini", ATM_CFG_SEC, "ATM_ID", strCode);

	strTmp = IniGetStr("D:\\INI\\NETWORK.ini", "NETWORK", "BranchNum", "(00000096)");
	strCode.Format("%2.2s%8.8s", strTmp.Mid(7, 2), &m_pProfile->NETWORK.AtmSerialNum[0]);
	IniSetStr("D:\\INI\\ATM_CFG.ini", ATM_CFG_SEC, "F_ATM_ID", strCode);
	
	m_pProfile->PutTransProfile();
	m_pProfile->GetWithTotal();						// 합계표읽기				
	m_pProfile->GetTransTotal();
	m_pProfile->GetDepTotal();
	m_pProfile->GetMoneyTotal();								// 출금현/수합계

	// Atm기기구성 내부변수(변경불가)
	// m_pProfile->DEVICE는 업무단에서는 사용 불가
	memcpy(&AtmDefine, &m_pProfile->DEVICE, sizeof(AtmDefine));
	
	HostSvrPortMode     = 0;
	HostSvrPortMode     = Asc2Int(m_pProfile->NETWORK.BpPortNo, 5);
//	HostLineMode		= HOST_LOCAL_MODE;			// Oline/Local
	HostLineMode		= HOST_ONLINE_MODE;			// Oline/Local
//	BrmCycleMode		= BRM_RECYCLE_MODE;			// Recycle/Uncycle
///	BrmCashMode			= BRM_TESTCASH_MODE;		// RealCash/TestCash
	BrmCashMode			= BRM_REALCASH_MODE;		// RealCash/TestCash


	nTranModeReg	= RegGetInt(_REGKEY_DEVICE, "TRANMODE", 0);
	nTranModeIni	= IniGetInt(_TRANS_C_INI, TRANS_SEC, "TranMode", 0); //#0092
	
	if(nTranModeReg == TRUE && nTranModeIni == TRUE)
		HostLocalSvrMode = TRUE;
	else
	{
		RegSetInt(_REGKEY_DEVICE, "TRANMODE", 0);
		IniSetInt(_TRANS_C_INI, TRANS_SEC, "TranMode", 0); //#0092
		HostLocalSvrMode = FALSE;
	}
		
	if (BrmCashMode == BRM_TESTCASH_MODE)
	{
		RunExecProgram(LOCALDIREXE ,FALSE, FALSE);

		IniSetStr(_NETWORK_INI, "NETWORK", "BpCurIpAddress", "(127.000.000.001)");
		IniSetStr(_NETWORK_INI, "NETWORK", "BpIpAddress", "(127.000.000.001)");
		m_pProfile->GetNetworkProfile();
		m_pProfile->PutNetworkProfile();

		CString strTemp("");
		strTemp.Empty();
		strTemp.Format("%8.8s%6.6s", GetDate(), GetTime());
	}
	else
	{
		if(HostLocalSvrMode)
		{
			RunExecProgram(LOCALDIREXE ,FALSE, FALSE);
			CString strTemp("");
			if(AtmDefine.MachineType == U3100K)
			{
				//#N0135
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K", 0);
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_D", "NEW10K", 0);

				strTemp.Empty();
				strTemp.Format("%8.8s%6.6s", GetDate(), GetTime());
				fnUTL_IniSetInt(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedCstInfo", TRUE);
				fnUTL_IniSetStr(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedDateTime", strTemp);
				fnBRM_BrmCashClear(TRUE);
			}
			BrmCashMode			= BRM_TESTCASH_MODE;		// RealCash/TestCash

		}
		else
			WinExecWaitEnd("C:\\T1ATM\\APP\\Main\\GOMA.EXE   ECash_Simulator.exe", SW_HIDE);
		
	}

	// (SP관련)
	if ((OKI_SET == AtmDefine.CashHandler) || (HBRM_SET == AtmDefine.CashHandler)) // U8100 AP 변경내역 #04 - HBRM관련 변경내역
	{

		CString			strOMDepositInfo("");
		CStringArray	strOMDepositInfoArray;
		CString			strTemp = "";
		DWORD			dwData = 0;
		LONG			lRet;
		HKEY			hKey;
		DWORD			dwDisp;
		DWORD			dwSize;

		if(AtmDefine.MachineType == U8100) //농협 Brand - 사양 
		{

			RegSetInt(_REGKEY_BRMINFO, "만원권잔량예비매수",	    0);
			RegSetInt(_REGKEY_BRMINFO, "분실회수함최대매수",	OKI_MAXFRJBCNT);
			RegSetInt(_REGKEY_BRMINFO, "운용회수함1최대매수",	HBRM_MAXRJB1CNT);
			RegSetInt(_REGKEY_BRMINFO, "운용회수함1한도매수",	HBRM_MINRJB1CNT);
			RegSetInt(_REGKEY_BRMINFO, "운용회수함2최대매수",	HBRM_MAXRJB2CNT);
			RegSetInt(_REGKEY_BRMINFO, "운용회수함2한도매수",	HBRM_MINRJB2CNT);

			if(m_pProfile->DEVICE.ETCDevice20 == KWANGJU || m_pProfile->DEVICE.ETCDevice20 == KYONGNAM)	//#N0181 => #N0192  광주&경남 브랜드 입금 최대매수 150
				RegSetInt(_REGKEY_BRMINFO, "입금최대매수",			BRM_MAXCASHINCNT_150);
			else
				RegSetInt(_REGKEY_BRMINFO, "입금최대매수",			OKI_MAXCASHINCNT);
			
			RegSetInt(_REGKEY_BRMINFO, "출금최대매수",			    100); ////#N0140
			RegSetInt(_REGKEY_BRMINFO, "현금박스최대매수",		OKI_MAXRBCNT);
			RegSetInt(_REGKEY_BRMINFO, "현금부족알림매수",		BRM_AREFILLCONDCNT);
			RegSetInt(_REGKEY_BRMINFO, "현금자동보충매수",		BRM_AREFILLCNT); //#0089
			RegSetInt(_REGKEY_BRMINFO, "현금자동회수매수",		BRM_ATAKEUPCNT);

			RegSetInt(_REGKEY_BRMINFO, "오만원권잔량예비매수",	    0);
			RegSetInt(_REGKEY_BRMINFO, "오만원권출금최대매수",	    20); //5*14 = 70만원 -> #N0140 (기업 10만원)
			RegSetInt(_REGKEY_BRMINFO, "오만원권현금부족알림매수",	60); //#N0140
			RegSetInt(_REGKEY_BRMINFO, "오만원권박스최대매수",	OKI_50T_MAXRBCNT);

			if((m_pProfile->DEVICE.ETCDevice20 != NONGHYUP) && (m_pProfile->DEVICE.ETCDevice20 != SC))   
			{
				m_pProfile->DEVICE.Proximity = NOT_SET;			
				m_pProfile->DEVICE.EarHandler = NOT_SET;
				m_pProfile->DEVICE.SightHandicapped = NOT_SET;

				m_pProfile->PutDeviceProfile();										
				m_pProfile->GetDeviceProfile();
			}

		}
		else
		if(AtmDefine.MachineType == U3100K)
		{
			RegSetInt(_REGKEY_BRMINFO"\\COMMON", "dwMaxAcceptCount", 100); 
			RegSetInt(_REGKEY_BRMINFO, "운용회수함1최대매수",	800); //Reject
			RegSetInt(_REGKEY_BRMINFO, "운용회수함1한도매수",	800);
			RegSetInt(_REGKEY_BRMINFO, "운용회수함2최대매수",	200);
			RegSetInt(_REGKEY_BRMINFO, "운용회수함2한도매수",	200);
			RegSetInt(_REGKEY_BRMINFO, "오만원권출금최대매수",	OKI_50T_MAXCASHOUTCNT);
			RegSetInt(_REGKEY_BRMINFO, "오만원권잔량예비매수",	    0);
			
			RegSetInt(_REGKEY_BRMINFO, "입금최대매수",			OKI_MAXCASHINCNT);
			RegSetInt(_REGKEY_BRMINFO, "출금최대매수",			OKI_MAXCASHINCNT);
			RegSetInt(_REGKEY_BRMINFO, "만원권잔량예비매수",	0);
			RegSetInt(_REGKEY_BRMINFO, "분실회수함최대매수",	OKI_MAXFRJBCNT);

		}
		else
		{
			RegSetInt(_REGKEY_BRMINFO, "만원권잔량예비매수",	OKI_REVISECASHCNT);
			RegSetInt(_REGKEY_BRMINFO, "분실회수함최대매수",	OKI_MAXFRJBCNT);
			RegSetInt(_REGKEY_BRMINFO, "운용회수함1최대매수",	HBRM_MAXRJB1CNT);
			RegSetInt(_REGKEY_BRMINFO, "운용회수함1한도매수",	HBRM_MINRJB1CNT);
			RegSetInt(_REGKEY_BRMINFO, "운용회수함2최대매수",	HBRM_MAXRJB2CNT);
			RegSetInt(_REGKEY_BRMINFO, "운용회수함2한도매수",	HBRM_MINRJB2CNT);
			RegSetInt(_REGKEY_BRMINFO, "입금최대매수",			OKI_MAXCASHINCNT);
			RegSetInt(_REGKEY_BRMINFO, "출금최대매수",			OKI_MAXCASHOUTCNT);
			RegSetInt(_REGKEY_BRMINFO, "현금박스최대매수",		OKI_MAXRBCNT);
			RegSetInt(_REGKEY_BRMINFO, "현금부족알림매수",		BRM_AREFILLCONDCNT);
			RegSetInt(_REGKEY_BRMINFO, "현금자동보충매수",		BRM_AREFILLCNT);
			RegSetInt(_REGKEY_BRMINFO, "현금자동회수매수",		BRM_ATAKEUPCNT);

			RegSetInt(_REGKEY_BRMINFO, "오만원권잔량예비매수",	OKI_50T_REVISECASHCNT);
			RegSetInt(_REGKEY_BRMINFO, "오만원권출금최대매수",	OKI_50T_MAXCASHOUTCNT);
			RegSetInt(_REGKEY_BRMINFO, "오만원권현금부족알림매수",	OKI_50T_AREFILLCONDCNT); 
			RegSetInt(_REGKEY_BRMINFO, "오만원권박스최대매수",	OKI_50T_MAXRBCNT);
		}


		strOMDepositInfo = GetShareData("OM_입금권종선택");		// 입금가능권종
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[OM_입금권종선택][%s]", strOMDepositInfo);
		if ("" == strOMDepositInfo)
		{
			lRet = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE,
									_REGKEY_BRMINFO,
									0, NULL, REG_OPTION_NON_VOLATILE,
									KEY_READ, NULL, &hKey, &dwDisp);
			if (ERROR_SUCCESS == lRet)
			{
				dwSize = sizeof(DWORD);
				lRet = ::RegQueryValueEx(hKey, "입금가능권종", 0, NULL, (LPBYTE)&dwData, &dwSize);
				::RegCloseKey(hKey);

				if (ERROR_SUCCESS == lRet)
				{
					if (dwData & DEP_AVAIL_10000_OLD)
						strTemp += "가능||";
					else
						strTemp += "불가||";

					if (dwData & DEP_AVAIL_5000_OLD)
						strTemp += "가능||";
					else
						strTemp += "불가||";

					if (dwData & DEP_AVAIL_1000_OLD)
						strTemp += "가능||";
					else
						strTemp += "불가||";

					if (dwData & DEP_AVAIL_10000_NEW)
						strTemp += "가능||";
					else
						strTemp += "불가||";

					if (dwData & DEP_AVAIL_5000_NEW)
						strTemp += "가능||";
					else
						strTemp += "불가||";

					if (dwData & DEP_AVAIL_1000_NEW)
						strTemp += "가능||";
					else
						strTemp += "불가||";

					if (dwData & DEP_AVAIL_50000)
						strTemp += "가능||";
					else
						strTemp += "불가||";

					strTemp += "가능";	
				}
				else
				{
					if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
					{
						dwData = DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;
						strTemp = "불가||불가||불가||불가||불가||불가||가능||가능";	
					}
					else
					if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
					{
						dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;	
						strTemp = "가능||가능||가능||가능||가능||가능||가능||가능";	
					}
					else
					{
						dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;
						strTemp = "가능||가능||가능||가능||가능||가능||가능||가능";	
					}

					lRet = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE,
											_REGKEY_BRMINFO,
											0, NULL, REG_OPTION_NON_VOLATILE,
											KEY_ALL_ACCESS, NULL, &hKey, &dwDisp);
					if (ERROR_SUCCESS == lRet)
						::RegSetValueEx(hKey, "입금가능권종", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(int));
					::RegCloseKey(hKey);

				}
			}
			else
			{
				if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
				{
					dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;
					strTemp = "가능||가능||가능||가능||가능||가능||가능||가능";	
				}
				else
				if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
				{
					dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;	
					strTemp = "가능||가능||가능||가능||가능||가능||가능||가능";	
				}
				else
				if(m_pProfile->DEVICE.ETCDevice20 == KEB)
				{
					dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;	
					strTemp = "가능||가능||가능||가능||가능||가능||가능||가능";	
				}
				else
				{
					dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;
					strTemp = "가능||가능||가능||가능||가능||가능||가능||가능";	
				}

				lRet = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE,
										_REGKEY_BRMINFO,
										0, NULL, REG_OPTION_NON_VOLATILE,
										KEY_ALL_ACCESS, NULL, &hKey, &dwDisp);
				if (ERROR_SUCCESS == lRet)
					::RegSetValueEx(hKey, "입금가능권종", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(int));
				::RegCloseKey(hKey);	
			}

			SetShareData("OM_입금권종선택", strTemp);	
			m_pProfile->BRM.DepositCashKind = dwData;
			m_pProfile->PutBRMProfile();	
		}
		else
		{

			if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
			{
				dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;
				strTemp = "가능||가능||가능||가능||가능||가능||가능||가능";	
			}
			else
			if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
			{
				dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;	
				strTemp = "가능||가능||가능||가능||가능||가능||가능||가능";	
			}
			else
			if(m_pProfile->DEVICE.ETCDevice20 == KEB)
			{
				dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;	
				strTemp = "가능||가능||가능||가능||가능||가능||가능||가능";	
			}
			else
			{
				dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;
				strTemp = "가능||가능||가능||가능||가능||가능||가능||가능";	
			}

			RegSetInt(_REGKEY_BRMINFO, "입금가능권종", dwData);		
			SetShareData("OM_입금권종선택", strTemp);	
			m_pProfile->BRM.DepositCashKind = dwData;
			m_pProfile->PutBRMProfile();

		}
	}
	else
	{
		IniSetInt(_SP_DEVICE_INI, "BRM", "MaxCashOutCnt",		BRM_MAXCASHOUTCNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "MaxCashInCnt",		BRM_MAXCASHINCNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "DistinctionMode",		BrmCashMode);
		IniSetInt(_SP_DEVICE_INI, "BRM", "현금박스최대매수",	BRM_MAXRBCNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "RJB1최대매수",		BRM_MAXRJB1CNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "RJB1한도매수",		BRM_MAXRJB1CNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "RJB2최대매수",		BRM_MAXRJB2CNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "RJB2한도매수",		BRM_MANRJB2CNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "만원권잔량예비매수",	BRM_FIXCASHCNT);
		IniSetStr(_SP_DEVICE_INI, "BRM", "ShutterSpeed",		"고속");
		IniSetInt(_SP_DEVICE_INI, "BRM", "현금부족알림매수",	BRM_AREFILLCONDCNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "현금회수알림매수",	BRM_ATAKEUPCONDCNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "현금자동보충매수",	BRM_AREFILLCNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "현금자동회수매수",	BRM_ATAKEUPCNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "현금잔량소매수",		BRM_NEARCASHCNT);
		if (m_pProfile->BRM.DepositCashKind == 0)					// V200 : 입금권종제한
			IniSetInt(_SP_DEVICE_INI, "BRM", "RJB2만원권만입금가능매수", BRM_MAXRJB2DEPCNT);
		else
			IniSetInt(_SP_DEVICE_INI, "BRM", "RJB2만원권만입금가능매수", 0);
	}

	fnSCR_SetCurrentLangMode(KOR_MODE);	
	
	if(m_pProfile->DEVICE.MachineType == U3100K)
		SprRowOption = SPR_MAX_ROW;
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
		SprRowOption = SPR_MAX_ROW_U8100;
	else
		SprRowOption = SPR_MAX_ROW;

	AtmDoing			= FALSE;					// Atm실행여부
	AtmFirstDoingFlag	= FALSE;					// Atm최초실행Flag
	AtmStatus			= ATM_INIT;					// Atm상태변수 
	AtmStatusSave		= ATM_INIT;					// Atm이전상태변수 
	AtmStatusReEntryFlag= FALSE;					// Atm상태재진입Flag
	HostStatus			= HOST_INIT;				// Host상태변수 
	HostStatusSave		= HOST_INIT;				// Host이전상태변수 
	HostFirstOpenFlag	= FALSE;					// Host최초개국Flag
	HostOpenFlag		= FALSE;					// Host개국Flag
	HostFirstReadyFlag	= FALSE;					// Host최초상태Flag
	PowerOffSendFlag	= FALSE;					// PowerOFf송신Flag -> #CS0001
	TranStatus			= TRAN_IDLE;				// Tran상태변수 
	TranStatusSave		= TRAN_IDLE;				// Tran이전상태변수 
	TranProc			= TRAN_NULL;				// Tran거래처리
	TranResult			= FALSE;					// Tran거래결과

	DeviceDefine		= DEV_NONE;					// 기기에 정의된	Device Set
	DeviceNoDevice		= DEV_NONE;					// 미연결			Device Set
	DeviceDownDevice	= DEV_NONE;					// 다운				Device Set
	DeviceStatus		= DEV_NONE;					// 장애가 난		Device Set
	DeviceAutoOff		= DEV_NONE;					// 자동축퇴가 난	Device Set
	DeviceRetract		= DEV_NONE;					// 회수가 난		Device Set
	DeviceTran			= DEV_NONE;					// 최초 Event		Device Set 
	DeviceTimeout		= DEV_NONE;					// 시간초과			Device set
	DeviceLoss			= DEV_NONE;					// 미수취장애		Device Set

	ForceDownFlag		= FALSE;					
	RemoteDownFlag      = FALSE;				    
	ForceMissMatchDownFlag		= FALSE;					
		

	memset(m_pTranCmn->pArrDataFieldArr, 0x00, sizeof(m_pTranCmn->pArrDataFieldArr));
	m_pTranCmn->nSendDataIndex = 0;


	if (HBRM_SET == AtmDefine.CashHandler)			// U8100-USB장치의 경우 통신장애면 응답시간이 길어야함
	{
		SprRecvTime			= MAX_DEVRSP_TIME;		// Spr수신Time MIN_DEVRSP_TIME -> MAX_DEVRSP_TIME변경(U8100)
		JprRecvTime			= MAX_DEVRSP_TIME;		// Jpr수신Time MIN_DEVRSP_TIME -> MAX_DEVRSP_TIME변경(U8100)
		McuRecvTime			= MAX_DEVRSP_TIME;		// Mcu수신Time MIN_DEVRSP_TIME -> MAX_DEVRSP_TIME변경(U8100)
		BrmRecvTime			= MAX_DEVRSP_TIME;		// Brm수신Time MIN_DEVRSP_TIME -> MAX_DEVRSP_TIME변경(U8100)
		UcmRecvTime			= MID_DEVRSP_TIME;		// Ucm수신Time
		PbmRecvTime			= MAX_DEVRSP_TIME;		// Pbm수신Time MIN_DEVRSP_TIME -> MAX_DEVRSP_TIME변경(U8100)
		RfdRecvTime			= MIN_DEVRSP_TIME;		// Rfd수신Time
		IcmRecvTime			= MIN_DEVRSP_TIME;		// Icm수신Time
	}
	else
	{
		SprRecvTime			= MID_DEVRSP_TIME;		// Spr수신Time
		JprRecvTime			= MID_DEVRSP_TIME;		// Jpr수신Time
		McuRecvTime			= MID_DEVRSP_TIME;		// Mcu수신Time
		BrmRecvTime			= MID_DEVRSP_TIME;		// Brm수신Time
		UcmRecvTime			= MID_DEVRSP_TIME;		// Ucm수신Time
		PbmRecvTime			= MID_DEVRSP_TIME;		// Pbm수신Time
		RfdRecvTime			= MIN_DEVRSP_TIME;		// Rfd수신Time
		IcmRecvTime			= MIN_DEVRSP_TIME;		// Icm수신Time
	}

	
	OpenKey				= TRUE;						// '준비' '운영' 전환스위치 
	ClerkKey			= 0;						// 계원조작 키 상태
	PowerOffDetect		= POWER_NORMAL;				// Power 상태변수
	RecoveryType		= 0;						// 0:자동복구,1:수동복구
	TranType			= 0;						// 0:고객거래무,1:고객사용유(장애송신관련)

	CurErrCnt			= 0;						// 현재장애갯수
	memset(&CurErrBuff, 0, sizeof(CurErrBuff));		// 현재장애내역
	memset(&FirstErrBuff, 0, sizeof(FirstErrBuff));	// 최초장애내역
	memset(&LastErrBuff, 0, sizeof(LastErrBuff));	// 최종장애내역
													// V07-01-04-#02
	memset(OrignalErrorCode, 0, sizeof(OrignalErrorCode));
	CSTCnt				= 0;						// CST Count
	SCSTCntF            = 0;
	FstCSTValue			= 0;						// First CST Value
	SndCSTValue			= 0;						// Second CST Value
	TrdCSTValue			= 0;						// Third CST Value
	FthCSTValue			= 0;						// Fourth CST Value
	FstCSTSetCash		= 0;						// First CST Set Cash
	SndCSTSetCash		= 0;						// Second CST Set Cash
	TrdCSTSetCash		= 0;						// Third CST Set Cash
	FthCSTSetCash		= 0;						// Fourth CST Set Cash
	FstCSTCnt			= 0;						// First CST Count(ClerkInformation)
	SndCSTCnt			= 0;						// Second CST Count(ClerkInformation)
	TrdCSTCnt			= 0;						// Third CST Count(ClerkInformation)
	FthCSTCnt			= 0;						// Fourth CST Count(ClerkInformation)

	CshChangeLoc		= NO_CHANGE;				// Csh Change Location
	CshLoc				= NORMAL_POS;				// Csh Location

	BrmChangeLoc		= NO_CHANGE;				// Brm정위치변경상태
	BrmLoc				= NORMAL_POS;				// Brm정위치상태
	RejectCSTLoc		= NORMAL_POS;				// 회수함정위치상태
	RetractCSTLoc		= NORMAL_POS;				// 회수함정위치상태
	ClerkCSTLoc			= NORMAL_POS;				// 보충함정위치상태
	FstCSTLoc			= NORMAL_POS;				// 1카세트정위치상태(Cd)
	SndCSTLoc			= NORMAL_POS;				// 2카세트정위치상태(Cd)
	TrdCSTLoc			= NORMAL_POS;				// 3카세트정위치상태(Cd)
	FthCSTLoc			= NORMAL_POS;				// Fourth CST Location(Cd)
	UcmLoc				= NORMAL_POS;				// Ucm정위치상태
	DoorStatus			= DOOR_CLOSED;				// DoorStatus상태
	CabinetStatus		= DOOR_CLOSED;				// CabinetStatus상태

	SlipHeadStatus		= HEADER_NORMAL;			// 명세표Cover상태
	JnlHeadStatus		= HEADER_NORMAL;			// 저널Cover상태
	SlipStatus			= SLIP_NORMAL;				// 명세표상태
	JnlStatus			= JNL_NORMAL;				// 저널상태
	A4pStatus			= HEADER_NORMAL;			// A4용지상태
	A4pHeadStatus		= HEADER_NORMAL;			// A4용지상태

	GSensorStatus       = NORMAL_POS;				// 충격센서상태
	GExternalSensorStatus1 = NORMAL_POS;            // 외부센서1
	GExternalSensorStatus2 = NORMAL_POS;            // 외부센서2
	GAlarmStatus        = NORMAL_POS;				// 알람센서상태
	GSensorLelvel       = NORMAL_POS;				// 충격센서감도
	GBatteryStatus		= NORMAL_POS;				// 밧데리센서감도
	RejectCSTStatus		= CST_NORMAL;				// 회수카세트상태
	RetractCSTStatus	= CST_NORMAL;				// 회수카세트상태
	ClerkCSTStatus		= CST_NORMAL;				// 계원카세트함상태
	ClerkRejectBoxStatus= CST_NORMAL;				// 보충RejectBox상태		V07-00-00-#01 오만원권 지원
	RecycleBoxStatus	= CST_NORMAL;				// 현금상태
	RecycleBox50000Status = CST_NORMAL;				// 현금상태					V07-00-00-#01 오만원권 지원
	FstCSTStatus		= CST_NORMAL;				// 1카세트상태(Cd)
	SndCSTStatus		= CST_NORMAL;				// 2카세트상태(Cd)
	TrdCSTStatus		= CST_NORMAL;				// 3카세트상태(Cd)
	FthCSTStatus		= CST_NORMAL;				// 4카세트상태(Cd)
	CheckCSTStatus		= CST_NORMAL;				// 출금수표상태
	CheckCSTStatusSensor= CST_NORMAL;				// 출금수표상태(실물)
	CheckCSTStatusReg	= CST_NORMAL;				// 출금수표상태(등록)
	CassetteStatus		= CST_NORMAL;				// 입금수표상태

	CaptureLastIndex	= 0;						// 카메라마지막저장Index
	CameraStatus		= CST_NORMAL;				// 카메라상태

	
	DesBoardStatus		= FALSE;					// Des Board Status
	DesITKStatus		= FALSE;					// Des ITK Status
	DesTMKStatus		= FALSE;					// Des TMK Status
	memset(DesRPIN, 0, sizeof(DesRPIN));			// Des RPIN
	memset(UserDefineSetErrCode, ' ', sizeof(UserDefineSetErrCode));			// UserDefineSet Err (Van-CD Spec)
	DesTMKTryFlag		= FALSE;					// Des TMK Try Flag
	DesTranStatus		= FALSE;					// Des Tran Status
	DesOpenRetryCnt		= 0;						// Des Open Retry Count

	SeedEnStatus		= FALSE;					// Seed Enable Status
	RF_IrMD_Flg			= FALSE;					// RF 모바일 구분
	SeedTranStatus		= FALSE;					// Seed Tran Status
	SeedSendRetryCnt	= 0;						// Seed Send Retry Count
	FNG_Status			= FALSE;					// RF 모바일 구분


	TranAvail			= TRAN_ALL_NOT;				// 전체거래상태
	WithAvail			= TRAN_WITH_NOT;			// 지급거래상태
	DepAvail			= TRAN_DEP_NOT;				// 입금거래상태
	PbmAvail			= TRAN_PBM_NOT;				// 통장거래상태
	
	UserTranAvail		= TRAN_ALL_NOT;				// User전체거래상태 2003.12.15
	UserWithAvail		= TRAN_WITH_NOT;			// User지급거래상태
	UserDepAvail		= TRAN_DEP_NOT;				// User입금거래상태
	UserPbmAvail		= TRAN_PBM_NOT;				// User통장거래상태
	
	HostOpenRetryCnt	= 0;						// 개국시도횟수(준비완포함)

	bReadyCountFlg      = FALSE;
	
	OldCashDepMode		= TRUE;						// (TRUE:구권입금가능, FALSE:구권입금불가)
	OldCasetteType		= ON;						// (ON:구권Casette有,	OFF:구권Casette無)

	TTSEngineLoadFlg = FALSE;						// V07-01-11-#00

	memset(&Timer, 0, sizeof(Timer));				// 타임변수영역

	memset(&Ejr, 0, sizeof(Ejr));					// 전자저녈영역
	memset(&EjrAmount, 0, sizeof(EjrAmount));		// 전자저녈금액관련자료

	memset(&Scr, 0, sizeof(Scr));					// 화면변수영역

	ChangeBCFlag	= FALSE;
	Withdraw50TFlag = FALSE;
	BrmCshInfo      = FALSE;
	BrmCsmIn        = FALSE;

	IfmDirect			= FALSE;					// #NICCM01	ICCM 모듈 적용
	IfmDirect_Init		= FALSE;					// #NICCM01	ICCM 모듈 적용
	IfmResult			= FALSE;					// #NICCM01	ICCM 모듈 적용
	IsEmvChipPowerOn = FALSE;						// #NICCM01	ICCM 모듈 적용

	

	SightHandiCapped = FALSE;
	EarPhoneInserted = FALSE;

	// Om정보초기화
	Om.ProcCount		= " ";						// 진행카운터
	Om.ErrorCode		= "0000000";				// 장애코드
	Om.McuDeviceStatus	= "정상";					// Mcu장치상태(정상,장애)
	Om.McuProcCount		= " ";						// Mcu장애진행카운터(정상='0',' ','0','1','3','5','6','9')
	Om.McuErrorCode		= "0000000";				// Mcu장애코드(정상="0000000",장애="1234567")
	Om.SprDeviceStatus	= "정상";					// Spr장치상태
	Om.SprProcCount		= " ";						// Spr장애진행카운터
	Om.SprErrorCode		= "0000000";				// Spr장애코드
	Om.JprDeviceStatus	= "정상";					// Jpr장치상태
	Om.JprProcCount		= " ";						// Jpr장애진행카운터
	Om.JprErrorCode		= "0000000";				// Jpr장애코드
	Om.PbmDeviceStatus	= "정상";					// Pbm장치상태
	Om.PbmProcCount		= " ";						// Pbm장애진행카운터
	Om.PbmErrorCode		= "0000000";				// Pbm장애코드
	Om.UcmDeviceStatus	= "정상";					// Ucm장치상태
	Om.UcmProcCount		= " ";						// Ucm장애진행카운터
	Om.UcmErrorCode		= "0000000";				// Ucm장애코드
	Om.BrmDeviceStatus	= "정상";					// Brm장치상태
	Om.BrmProcCount		= " ";						// Brm장애진행카운터
	Om.BrmErrorCode		= "0000000";				// Brm장애코드
	Om.NetDeviceStatus	= "정상";					// Net장치상태
	Om.NetProcCount		= " ";						// Net장애진행카운터
	Om.NetErrorCode		= "0000000";				// Net장애코드
	Om.AppDeviceStatus	= "정상";					// App장치상태
	Om.AppProcCount		= " ";						// App장애진행카운터
	Om.AppErrorCode		= "0000000";				// App장애코드
	Om.A4pDeviceStatus	= "정상";					// A4p장치상태
	Om.A4pProcCount		= " ";						// A4p장애진행카운터
	Om.A4pErrorCode		= "0000000";				// A4p장애코드
	Om.SendRecvStatus	= "OFF";					// 송수신상태(ON,OFF)
	Om.OperationStatus	= "ON";						// 조작상태(ON,OFF)
	Om.CheckKind		= "10";						// 수표권종(10,50,100)

	strMsgArray.RemoveAll();              // Remote Ctrl 

	memset(ICSendData, 0, sizeof(ICSendData));		// IC전송자료
	memset(ICRecvData, 0, sizeof(ICRecvData));		// IC수신자료

	memset(&KCashInfo, 0, sizeof(KCashInfo));		// K-CASH변수영역
	memset(FICEPVersion, 0, sizeof(FICEPVersion));	// #NICCM01	AP B/D 단종 대응
	
/////////////////////////////////////////////////////////////////////////////
																// T2ATM : 2006.5.24 yscho
	m_nVfdDoing = FALSE;										// VFD Thread Doing
	VFDTotalCount = 0;											// VFD표시총문자갯수
	VFDCurCount   = 0;											// VFD표시현재위치
	VFDDisTerm	  = K_5_WAIT;									// VFD표시시간간격(초)
	m_nTrPollDoing = FALSE;
	m_SendRecvStaus = FALSE;

	RfdSeq		  = 0x01;										// RFID지원 V03-04-01(R1)
	RfdTestStatus = FALSE;										//          V03-04-01(R1)	
/////////////////////////////////////////////////////////////////////////////


	memset(RfdVersion, ' ', sizeof(RfdVersion));				// V05-01-02-#11:RF Version
	Rfd3GAvail = FALSE;											// V05-05-01-#01:RF 3G 지원여부
	memset(RfErrorCode, 0, sizeof(RfErrorCode));				// V06-06-01-#01
	CheckMachineType = TRUE;									// V05-06-03-#13:TRUE(정상), FALSE(비정상)

	

	if (HostLineMode == HOST_LOCAL_MODE)
		PrevInformFlg = TRUE;									// 사전알림 중단/재개 지시자

/////////////////////////////////////////////////////////////////////////////
	memset(VODCapturePath, 0x00, sizeof(VODCapturePath));		// U8100-VOD동영상저장을 위한 변수 초기화
	RealVodFile		= "";										// 적용후 1차 수정 (U8100 AP 변경내역 #13) - 카메라 동영상 수정
	
	VODFaceRecording = OFF;
	VODHandRecording = OFF;
	VODRoomRecording = OFF;
	bVODIdx			 = FALSE;

	PBMInkStatus	 =  "";										// U8100-Pbm Ink상태관련
	PBMInkCheckCnt	 = 0;
	CheckPbmInkStaus = PBMINK_NORMAL;							// PBM INK 상태전문관련

	bNearBranchSelectBtn = FALSE;								// 인근점포 지원 여부 확인	#N0172
	
	SetShareData("OM_화면상태", "후면");

	// 금융IC 데이타변수 초기화 2004.02.05
	FICCardType = FIC_NONE;							// 카드無
	FICDFType = 0;									// IC DF미선택
	FICAppletType = 0;											// IC카드 APPLET정보
	FICAccountCount = 0;							// 금융IC 계좌갯수
	memset(FICAccount, 0, sizeof(FICAccount));		// 금융IC계좌번호
	memset(FICBankCode, 0, sizeof(FICBankCode));	// 금융IC 은행번호

	memset(&ICTypeInfo, 0, sizeof(ICTypeInfo));					// IC CHIP TYPE정보 2004.11.03
	memset(&FinanceICInfo, 0, sizeof(FinanceICInfo));	// 금융IC 카드정보
	memset(&FinanceICISO3Info, 0, sizeof(FinanceICISO3Info));	// 금융IC계좌정보(ISO3)
	memset(&FinanceICISO2Info, 0, sizeof(FinanceICISO2Info));	// 금융IC계좌정보(ISO2)
	memset(&FinanceICDisp, 0, sizeof(FinanceICDisp));	// 금융IC 화면출력데이타

	memset(&EPBAccountInfo, 0, sizeof(EPBAccountInfo));	// e-모든통장 계좌정보
	memset(&FinanceICEPBDisp, 0, sizeof(FinanceICEPBDisp));
	memset(&FinanceICEPBDisp2, 0, sizeof(FinanceICEPBDisp2));

	memset(&IRDAInfo, 0, sizeof(IRDAInfo));			// 적외선 정보
	memset(&IRDAISO3Info, 0, sizeof(IRDAISO3Info));	// 적외선 계좌정보
	memset(&IRDAISO2Info, 0, sizeof(IRDAISO2Info));	// 적외선 계좌정보

/////////////////////////////////////////////////////////////////////////////
	
//	SeedFailCode		= 0;						// SeedFailCode
	RFDAnalError		= 0;						// RF분석오류
	memset(RFDStatus, 0, sizeof(RFDStatus));		// RF상태

/////////////////////////////////////////////////////////////////////////////
	AtmRestartInfo.bUseRestart		= RegGetInt(_REGKEY_ATMRESTART, "UseRestart", 1);
	AtmRestartInfo.strCheckStartTime= RegGetStr(_REGKEY_ATMRESTART, "CheckStartTime", "040000");
	AtmRestartInfo.strCheckEndTime	= RegGetStr(_REGKEY_ATMRESTART, "CheckEndTime", "050000");
	AtmRestartInfo.bReadyRestart	= RegGetInt(_REGKEY_ATMRESTART, "ReadyRestart", 0);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkOperation: CheckStartTime[%s] ~ CheckEndTime[%s]", AtmRestartInfo.strCheckStartTime, AtmRestartInfo.strCheckEndTime);

//////////////////////////////////////////////////////////////////////////
	m_nJournalMode = ATMS_JNL_PAPER;		
//	m_nJournalMode = ATMS_JNL_MIX;				
	m_nAtmsCommand = ATMS_CMD_IDLE;
	m_nAtmsDoing = FALSE;
	m_nJnlIndexLength = 0;
	memset(m_szJnlIndexBuff, 0, sizeof(m_szJnlIndexBuff));
	m_szSaveTime.Empty();
	m_nJnlType = 0;

	if ((OKI_SET == AtmDefine.CashHandler) || (HBRM_SET == AtmDefine.CashHandler)) 
	{
		if (!(m_pProfile->BRM.DepositCashKind & (DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD)))
			OldCashDepMode = FALSE;													
	}

/////////////////////////////////////////////////////////////////////////////
	// DeviceDefine 생성
	switch (AtmDefine.SHUHandler)					// 0:NOT INSTALL,	1:O, 2:A2, 3:T, 4:S
	{
		case SHU_MECHA_O:
		case SHU_MECHA_A2:
		case SHU_MECHA_T:
		case SHU_MECHA_S:
		case SHU_MECHA_NT:
		case SHU_MECHA_HS:
			DeviceDefine |= DEV_SPR;
			break;
		default:
			break;
	}

	switch (AtmDefine.MCUHandler)					// 0:NOT INSTALL,	1:O_TYPE, 2:S_TYPE, 3:D_TYPE;
	{
		case O_TYPE:
		case S_TYPE:
		case D_TYPE:
			DeviceDefine |= DEV_MCU;
			break;
		default:
			DeviceDefine |= DEV_MCU;
			break;
	}



	switch (AtmDefine.CashHandler)					// 0:NOT INSTALL,	1:BRM, 2:CDU
	{
		case BRM_SET:
			DeviceDefine |= DEV_CSH;
			break;
		case CDU_SET:
		case OKI_SET:								// T2ATM 2006.5.24 yscho
		case HBRM_SET:								// U8100 AP 변경내역 #04 - HBRM관련 변경내역
			DeviceDefine |= DEV_CSH;
			break;
		default:
			break;
	}

	switch (AtmDefine.CheckHandler)					// 0:NOT INSTALL,	1:CAM, 2:UCM
	{
		case CAM_SET:
		case UCM_SET:
			DeviceDefine |= DEV_UCM;
			break;
	}

	switch (AtmDefine.PBHandler)					// 0:NOT INSTALL,	1:LPBM, 2:LPBM(NO AUTO TURN)
	{
		case PBM_SET:
		case PBM_NAT_SET:
		case I_PBM_SET:								// IPBM 소모품 잔여정보 지원
			DeviceDefine |= DEV_PBM;
			break;
	}

	// 부속장치Define추가부분
	DeviceDefine |= DEV_SCR;						// SCREEN	: ADD Define(2005.01.18 TTW)
	DeviceDefine |= DEV_DOR;						// DOOR
	DeviceDefine |= DEV_LGT;						// LIGHT
	
	switch(AtmDefine.MachineType)
	{
	case U8100:
		DeviceDefine |= DEV_SPL;						// SPL
		break;
	case U3100K:
		DeviceDefine |= DEV_SNS;						// SENSOR : 2009.07.21
		break;
	default:
		break;
	}

	switch (AtmDefine.CameraDevice)					// 0:NOT INSTALL,	1:PCI CAMERA   2:USB CAMERA
	{
		case PCI_CAMERA_SET:
		case USB_CAMERA_SET:
			DeviceDefine |= DEV_CMR;
			break;
		case DVR_CAMERA_SET:
			DeviceDefine |= DEV_DVR;
			break;
	}

	switch (AtmDefine.TenKeyHandler)				// 0:NOT INSTALL,	1:INSTALL
	{
		case TENKEY_SET:
			DeviceDefine |= DEV_PIN;
//			DeviceDefine |= DEV_TNK;
			break;
		case HOTKEY_SET:							// 0:NOT INSTALL, 1:TENKEY SET, 2:HOTKEY SET 
			DeviceDefine |= DEV_PIN;				
			break;
	}

	switch (AtmDefine.ICCardDevice)					// 0:NOT INSTALL,	1:INSTALL
	{
		case ICCARD_SET:
			DeviceDefine |= DEV_MCU;
			break;
	}

	switch (AtmDefine.PowerHandler)					// 0:NOT INSTALL,	1:PNC_TYPE, 2:UPS_TYPE
	{
		case PNC_TYPE:
		case UPS_TYPE:
			DeviceDefine |= DEV_UPS;
			break;
	}

	switch (AtmDefine.DesHandler)					// 0:NOT INSTALL,	1:D_DES BOARD, 2:T_DES BOARD
	{
		case D_DES_SET:
		case T_DES_SET:
			DeviceDefine |= DEV_DES;
			break;
	}

	switch (AtmDefine.FingerHandler)				// 0:NOT INSTALL,	1:INSTALL
	{
		case FINGER_SET:
			DeviceDefine |= DEV_FNG;
			break;
	}

	switch (AtmDefine.IrdaHandler)					// 0:NOT INSTALL,	1:INSTALL
	{	
		case IRDA_SET:
			DeviceDefine |= DEV_IRD;
			break;
	}

	switch (AtmDefine.BarHandler)					// 0:NOT INSTALL,	1:INSTALL
	{	
		case BAR_SET:
			DeviceDefine |= DEV_BAR;
			break;
	}

	switch (AtmDefine.A4PHandler)					// 0:NOT INSTALL,	1:INSTALL
	{
		case A4P_SET:
			DeviceDefine |= DEV_A4P;
			break;
	}

	switch (AtmDefine.VfdHandler)					// 0:NOT INSTALL,	1:INSTALL 
	{												// T2ATM 2006.5.24 yscho
		case VFD_SET:
		case LED_SET:								// LED기능 지원
			DeviceDefine |= DEV_VFD;
			break;
	}

	switch (AtmDefine.RfdHandler)					// 0:NOT INSTALL,	1:INSTALL 
	{
		case RFD_SET:
			DeviceDefine |= DEV_RFD;				// RF 지원
			break;
	}

	switch (AtmDefine.GiroHandler)					// 0:NOT INSTALL,	1:INSTALL 
	{
		case GIRO_SET:
			DeviceDefine |= DEV_GRO;				// 지로지원
			break;
	}

	if (AtmDefine.MachineType == U8100)			
		DeviceDefine |= DEV_SNS;

	if (m_pProfile->DEVICE.MachineType == U8100)   
	{
		switch(AtmDefine.EarHandler)
		{
			case EAR_SET:
				DeviceDefine |= DEV_PIN;
				DeviceDefine |= DEV_SNS;
				break;
		}
		SightHandiCapped = FALSE;
		EarPhoneInserted = FALSE;
		fnSCR_SetDisplayData(EAR_KINDMODE, "off");
//		TTS_StartProc();							
	}


	switch (AtmDefine.ETCDevice01)					// 0:NOT INSTALL,	1:INSTALL 
	{
		case RPCLOCK_SET1:
		case RPCLOCK_SET2:
			DeviceDefine |= DEV_RDCTRL;				// 상,하 잠금장치 
			break;
	}

	switch (AtmDefine.ETCDevice02)					// 0:NOT INSTALL,	1:INSTALL 
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_InitializeDS():return(%d)", AtmDefine.ETCDevice02);
		case SPACK_SET:
			DeviceDefine |= DEV_SPG;				// G-Sensor 
			break;
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_InitializeDS():return");
	return TRUE;
}

// 장치올리기
int CDevCmn::fnAPL_LoadDevice()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_LoadDevice()");

	CString strVersionData("");
	strVersionData.Format("[청호이지캐쉬] %9.9s(%2.2s)]", m_pProfile->TRANS.ApVersion, REVISION);
	fnSCR_DisplayString(1, strVersionData.GetBuffer(0));		// 버젼출력

	fnSCR_DisplayString(3, "자동화기기를 준비하고 있습니다.");
	fnSCR_DisplayString(4, "잠시만 기다려 주십시오.");
	fnSCR_DisplayScreen(701);									// 준비중화면

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[POWER OFF : ON]"));
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[AP VER    : V%s]", APVERSION));

	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		if(HostLocalSvrMode == TRUE && BrmCashMode == BRM_TESTCASH_MODE && AtmDefine.CashHandler == HBRM_SET)
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_LoadDevice(BRM_TESTCASH_MODE) - dwBCMode(%d)", RegGetInt(_REGKEY_BRMINFO"\\DEVICE_INFORMATION\\BCCONFIG", "dwBCMode", 2));
			RegSetInt(_REGKEY_BRMINFO"\\DEVICE_INFORMATION\\BCCONFIG", "dwBCMode", 3);
			fnCSH_Initialize();
			fnCSH_Deinitialize();
			RegSetInt(_REGKEY_BRMINFO"\\DEVICE_INFORMATION\\BCCONFIG", "dwBCMode", 2);
		}
		else
		{
/*
			//***************Kim.Gi.Jin************************************888
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_LoadDevice(BRM_TESTCASH_MODE) - dwBCMode(%d)", RegGetInt(_REGKEY_BRMINFO"\\DEVICE_INFORMATION\\BCCONFIG", "dwBCMode", 2));
			RegSetInt(_REGKEY_BRMINFO"\\DEVICE_INFORMATION\\BCCONFIG", "dwBCMode", 3);
			fnCSH_Initialize();
			fnCSH_Deinitialize();
			RegSetInt(_REGKEY_BRMINFO"\\DEVICE_INFORMATION\\BCCONFIG", "dwBCMode", 2);
			RunExecProgram(LOCALDIREXE ,FALSE, FALSE); //Nice 테스트 빨리 - 지워야 함
*/
		}

	}
	
	if(m_pProfile->DEVICE.MachineType == U3100K || m_pProfile->DEVICE.MachineType == U8100)
	{
		m_pSnd		= new CVolume();
	}


	fnDOR_Initialize();											
	fnLGT_Initialize();
	fnSPL_Initialize();

//	fnJPR_Initialize();											// AtmHostCtrl에서 생성
//	fnSPR_Initialize();											// AtmHostCtrl에서 생성
//	fnMCU_Initialize();											// AtmHostCtrl에서 생성
//	fnUCM_Initialize();											// AtmHostCtrl에서 생성
//	fnBRM_Initialize();											// AtmHostCtrl에서 생성
//	fnPBM_Initialize();											// AtmHostCtrl에서 생성
	

// 	if(m_pProfile->DEVICE.MachineType == U3100K)
// 	{
// 		fnLDCtrl_Initialize();										// SLOCK Create
// 		fnSPACK_Initialize();
// 	}
	
	fnCMR_Initialize();											// 연결시작및초기화

	fnUPS_Initialize();

// 	if ((AtmDefine.MachineType == U8100))	// U8100
// 	{
// 		if (m_pDevCmn->fnAPL_GetDefineDevice(DEV_CMR))			// Camera Preview 변경
// 			AfxBeginThread(thrPreviewThread, this, THREAD_PRIORITY_NORMAL);
// 	}

	PBBarLineReading = FALSE;

	fnDES_Initialize();
	if (fnAPL_GetDefineDevice(DEV_DES))							// 2003.12.12-DES
		fnDES_Test();


	fnPIN_Initialize();
	fnSNS_Initialize();
	fnSCR_Initialize();	
	fnSCR_SetCurrentLangMode(KOR_MODE);							// 현재언어모드설정

	if (AtmDefine.MachineType == U8100 || AtmDefine.MachineType == U3100K)
	{
		//#N0256
		if(OSType == OS_WIN7)
			IniSetInt(_LCDINFO_INI, "LCDPOSITION", "ADMONITOR", 3);
		else
			IniSetInt(_LCDINFO_INI, "LCDPOSITION", "ADMONITOR", 2);

		HWND hAviWnd = _FIND_CFMON;
		if(!hAviWnd)
		{
			WinExec("C:\\T1atm\\CFMonitor\\SwfPlayer.exe", SW_SHOW);
			
			hAviWnd = _FIND_CFMON;
			if(!hAviWnd)
				WinExec("C:\\T1atm\\CFMonitor\\AviPlayer.exe", SW_SHOW);
																// 3번째 모니터에 동영상 재생
			if (GetShareData("OM_광고동영상") == "OFF")
			{
				hAviWnd = _FIND_CFMON;
				::PostMessage(hAviWnd, WUM_SOUND_OFF, 0, 0);
			}
		}

		if(AtmDefine.MachineType == U8100)
		{
			HWND hWnd;
			hWnd = ::FindWindow(NULL, "SwfPlayer");
			if ( hWnd )
				::SendMessage(hWnd, WUM_ADCONTENTS, 1, 0);	
		}

	}

	m_pNetWork = new CNetWork(m_pProfile->NETWORK);

	fnNET_Initialize(LINE_OPCL_TIME);											
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_LoadDevice():return");
	return TRUE;
}



// 장치내리기
int CDevCmn::fnAPL_UnLoadDevice(int nEndFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_UnLoadDevice()");

	m_nVfdDoing = FALSE;										// T2ATM 2006.5.24 yscho


	if(nEndFlag)
	{
		//2010.03.25 Kim.G.J
		Delay_Msg(1000);
		WinExec("C:\\T1atm\\App\\Main\\ClearCom.bat", SW_HIDE);
		Delay_Msg(2000);

		if (m_pNetWork)		{	delete m_pNetWork;		m_pNetWork	= NULL; }

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_UnLoadDevice(%d):return", nEndFlag);		
		return TRUE;
	}
	// 연결종료 : 2003.08.23
	fnSCR_Deinitialize();										// 연결종료
//	fnNET_DeInitialize();										// AtmModeCtrl에서 종료
	fnSNS_Deinitialize();
	fnRFD_Deinitialize();
	fnPIN_Deinitialize();
	fnBAR_Deinitialize();
	fnIRD_Deinitialize();
	fnFNG_Deinitialize();
	fnDES_Deinitialize();
	fnUPS_Deinitialize();
	fnCMR_Deinitialize();

	fnPBM_Deinitialize();
	fnCSH_Deinitialize();
	fnUCM_Deinitialize();
	fnMCU_Deinitialize();
	fnSPR_Deinitialize();
	fnJPR_Deinitialize();
	fnA4P_Deinitialize();
	fnAPL_CheckDeviceAction(DEV_MAIN);

	fnSPL_Deinitialize();										
	fnLGT_Deinitialize();
	fnDOR_Deinitialize();

	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		Delay_Msg(1000);
		fnLDCtrl_Deinitialize();
		fnSPACK_Deinitialize();
	}


	Delay_Msg(1000);
	WinExec("C:\\T1atm\\App\\Main\\ClearCom.bat", SW_HIDE);
	Delay_Msg(2000);

	if (m_pNetWork)		{	delete m_pNetWork;		m_pNetWork	= NULL; }

	if (m_pSnd)			{	delete m_pSnd;			m_pSnd		= NULL; }

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_UnLoadDevice():return");
	return TRUE;
}

// 정의된장치구하기
int	CDevCmn::fnAPL_GetDefineDevice(int nDevId)
{
	return (DeviceDefine & nDevId);
}

// 정의되고연결장치구하기
int	CDevCmn::fnAPL_GetYesDevice(int nDevId)
{
	return (fnAPL_GetDefineDevice(nDevId) & (~(fnAPL_GetNoDevice(nDevId))));
}

// 정의되고미연결장치구하기
int	CDevCmn::fnAPL_GetNoDevice(int nDevId)
{
	return (fnAPL_GetDefineDevice(nDevId) & (DeviceNoDevice & nDevId));
}

int	CDevCmn::fnAPL_GetDownDevice(int nDevId)
{
	return (fnAPL_GetDefineDevice(nDevId) & (DeviceDownDevice & nDevId));
}

// 사용가능한장치구하기
int	CDevCmn::fnAPL_GetAvailDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);						// 정의된장치구하기

	// 현금출금, 현금입금 불가
	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_WITH_CASH | TRAN_WITH_50TCASH | TRAN_DEP_CASH)))	
	{
		if (AtmDefine.CashHandler == CDU_SET)					
		{													
			if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CHECK))
				nDevId &= ~DEV_CSH;
		}
		else
			nDevId &= ~DEV_CSH;
	}
	// 수표입금, 수표출금 불가
	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_WITH_CHECK | TRAN_DEP_CHECK)))
		nDevId &= ~DEV_UCM;
	// 통장사용 불가
	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_PBM_OK)))
		nDevId &= ~DEV_PBM;

	nDevId &= ~DeviceAutoOff;									// 자동축퇴가 난 Device Set
	nDevId &= ~m_pProfile->TRANS.DeviceExecOff;					// 불가축퇴가 난 Device Set
	return (nDevId & (~(fnAPL_GetErrorDevice(nDevId))));		// 장애난장치구하기
}

// 사용하고장애난장치구하기
int	CDevCmn::fnAPL_GetAvailErrorDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);						// 정의된장치구하기

	// 현금출금, 현금입금 불가
	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_WITH_CASH | TRAN_WITH_50TCASH | TRAN_DEP_CASH)))	
	{
		if (AtmDefine.CashHandler == CDU_SET)					// CDU 수표만 출금가능 일 경우
		{														// 수표출금 안됨 대응 Logic Miss
			if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CHECK))
				nDevId &= ~DEV_CSH;
		}
		else
			nDevId &= ~DEV_CSH;
	}
	// 수표입금, 수표출금 불가
	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_WITH_CHECK | TRAN_DEP_CHECK)))
		nDevId &= ~DEV_UCM;
	// 통장사용 불가
	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_PBM_OK)))
		nDevId &= ~DEV_PBM;

	nDevId &= ~DeviceAutoOff;									// 자동축퇴가 난 Device Set
	nDevId &= ~m_pProfile->TRANS.DeviceExecOff;					// 불가축퇴가 난 Device Set
	return (fnAPL_GetErrorDevice(nDevId));						// 장애난장치구하기
}

// 장애난장치구하기
int	CDevCmn::fnAPL_GetErrorDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);						// 정의된장치구하기

	DeviceStatus |= m_pProfile->TRANS.DeviceExecOff;			// 불가축퇴가 난 Device Set
	return (DeviceStatus & nDevId);
}

int	CDevCmn::fnAPL_GetDownErrorDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);

	return (fnAPL_GetDownDevice(nDevId) | fnAPL_GetErrorDevice(nDevId));
}

// 현금매수구하기
int	CDevCmn::fnAPL_GetNumberOfCash()
{
	int		nTempNumberOfCash = 0;

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		switch (AtmDefine.CashHandler)
		{
			case BRM_SET:
			case HBRM_SET:										// HBRM관련 변경내역
				nTempNumberOfCash = fnCSH_GetNumberOfCash();	// 현금매수구하기
				break;

			case CDU_SET:
				nTempNumberOfCash = CDU_MAXCASHOUTCNT;
				break;
		}
	}

	return nTempNumberOfCash;
}

// 5만원수납박스로부터 현금매수 구하는 함수 -> #0050
int CDevCmn::fnAPL_GetNumberOf50000Cash()
{
	int		nTempNumberOfCash = 0;

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		switch (AtmDefine.CashHandler)
		{
			case OKI_SET:	
			case HBRM_SET:					
				nTempNumberOfCash = fnCSH_GetNumberOf50000Cash();// 현금매수구하기
				break;

			case CDU_SET:
				nTempNumberOfCash = CDU_MAXCASHOUTCNT/CASH_FIFTY_THOUSAND;
				break;
		}
	}

	return nTempNumberOfCash;
}


// 출금수표매수구하기
int	CDevCmn::fnAPL_GetNumberOfOutCheck()
{
	int		nTempNumberOfCheck = 0;

	if (fnAPL_GetDefineDevice(DEV_UCM)	||
		fnAPL_GetDefineDevice(DEV_CAM)	)
	{
		switch (AtmDefine.CheckHandler)
		{
			case CAM_SET:
				break;

			case UCM_SET:
				nTempNumberOfCheck = m_pProfile->UCM.CrntOutCheckCnt;
				break;											
		}
	}
	else
	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		nTempNumberOfCheck = m_pProfile->UCM.CrntOutCheckCnt;
	}

	return nTempNumberOfCheck;
}

// 출금수표2 매수구하기
int	CDevCmn::fnAPL_GetNumberOfOutCheck2()
{
	int		nTempNumberOfCheck = 0;

	if (fnAPL_GetDefineDevice(DEV_UCM)	||
		fnAPL_GetDefineDevice(DEV_CAM)	)
	{
		switch (AtmDefine.CheckHandler)
		{
			case CAM_SET:
				break;

			case UCM_SET:
				nTempNumberOfCheck = m_pProfile->UCM.CrntOutCheckCnt2;
				break;											
		}
	}
	else
	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		nTempNumberOfCheck = m_pProfile->UCM.CrntOutCheckCnt2;
	}

	return nTempNumberOfCheck;
}

// 출금수표번호구하기
CString	CDevCmn::fnAPL_GetNumberOfOutCheckNo()
{
	CString	strTempNumberOfCheckNo("00000000");

	if (fnAPL_GetDefineDevice(DEV_UCM)	||
		fnAPL_GetDefineDevice(DEV_CAM)	)
	{
		switch (AtmDefine.CheckHandler)
		{
			case CAM_SET:
				break;

			case UCM_SET:
				strTempNumberOfCheckNo.Format("%8.8s", m_pProfile->UCM.CrntOutCheckNo);
				break;											
		}
	}
	else
	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		strTempNumberOfCheckNo.Format("%8.8s", m_pProfile->UCM.CrntOutCheckNo);
	}

	return strTempNumberOfCheckNo;
}

// 출금수표2 번호구하기
CString	CDevCmn::fnAPL_GetNumberOfOutCheckNo2()
{
	CString	strTempNumberOfCheckNo("00000000");

	if (fnAPL_GetDefineDevice(DEV_UCM)	||
		fnAPL_GetDefineDevice(DEV_CAM)	)
	{
		switch (AtmDefine.CheckHandler)
		{
			case CAM_SET:
				break;

			case UCM_SET:
				strTempNumberOfCheckNo.Format("%8.8s", m_pProfile->UCM.CrntOutCheckNo2);
				break;											
		}
	}
	else
	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		strTempNumberOfCheckNo.Format("%8.8s", m_pProfile->UCM.CrntOutCheckNo2);
	}

	return strTempNumberOfCheckNo;
}

// 출금수표 은행코드구하기
CString	CDevCmn::fnAPL_GetOutCheckBankNo()
{
	CString	strTempNumberOfCheckNo("00");
	
	if (fnAPL_GetDefineDevice(DEV_UCM))
	{
		switch (AtmDefine.CheckHandler)
		{
		case CAM_SET:
			break;
			
		case UCM_SET:
			strTempNumberOfCheckNo.Format("%2.2s", m_pProfile->UCM.BankNo);
			break;											
		}
	}
	
	return strTempNumberOfCheckNo;
}

// 출금수표 은행코드구하기2
CString	CDevCmn::fnAPL_GetOutCheckBankNo2()
{
	CString	strTempNumberOfCheckNo("00");
	
	if (fnAPL_GetDefineDevice(DEV_UCM))
	{
		switch (AtmDefine.CheckHandler)
		{
		case CAM_SET:
			break;
			
		case UCM_SET:
			strTempNumberOfCheckNo.Format("%2.2s", m_pProfile->UCM.BankNo2);
			break;											
		}
	}
	
	return strTempNumberOfCheckNo;
}

int	CDevCmn::fnAPL_GetKindOfOutCheck()
{
	int		nTempNumberOfCheckKind = 0;

	if (fnAPL_GetDefineDevice(DEV_UCM))
	{
		switch (AtmDefine.CheckHandler)
		{
			case UCM_SET:
				nTempNumberOfCheckKind = m_pProfile->UCM.CrntOutCheckKind;
				break;											
		}
	}
	else
	if ((AtmDefine.CashHandler == CDU_SET)	&&					// SCDP
		(fnAPL_GetDefineDevice(DEV_CSH))	)
	{
		nTempNumberOfCheckKind = m_pProfile->UCM.CrntOutCheckKind;
	}

	return nTempNumberOfCheckKind;
}


// 입금수표매수구하기
int	CDevCmn::fnAPL_GetNumberOfInCheck()
{
	int		nTempNumberOfCheck = 0;

	if (fnAPL_GetDefineDevice(DEV_UCM)	||
		fnAPL_GetDefineDevice(DEV_CAM)	)
	{
		switch (AtmDefine.CheckHandler)
		{
			case CAM_SET:
				break;

			case UCM_SET:
				nTempNumberOfCheck = m_pProfile->UCM.CrntInCheckCnt;
				break;											
		}
	}

	return nTempNumberOfCheck;
}

// 현금매수설정
int	CDevCmn::fnAPL_SetNumberOfCash(int nCashCnt)
{
	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		switch (AtmDefine.CashHandler)
		{

			case HBRM_SET:									    // HBRM관련 변경내역
				break;
			case BRM_SET:
			case OKI_SET:										// for T2ATM : 2006.05.08
			case CDU_SET:
				m_pProfile->GetBRMProfile();
				m_pProfile->BRM.CrntCashCnt = nCashCnt;
				m_pProfile->PutBRMProfile();
				break;
		}
	}
	
	return nCashCnt;
}

// 출금수표매수설정
int	CDevCmn::fnAPL_SetNumberOfOutCheck(int nOutCheckCnt, int nOutCheckCnt2)
{
	if (fnAPL_GetDefineDevice(DEV_UCM)	||
		fnAPL_GetDefineDevice(DEV_CAM)	)
	{
		switch (AtmDefine.CheckHandler)
		{
			case CAM_SET:
				break;
				
			case UCM_SET:
				m_pProfile->GetUCMProfile();
				m_pProfile->UCM.CrntOutCheckCnt = nOutCheckCnt;
				m_pProfile->UCM.CrntOutCheckCnt2 = nOutCheckCnt2;
				m_pProfile->PutUCMProfile();
				break;											
		}
	}
	else
	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		m_pProfile->GetUCMProfile();
		m_pProfile->UCM.CrntOutCheckCnt = nOutCheckCnt;
		m_pProfile->UCM.CrntOutCheckCnt2 = nOutCheckCnt2;
		m_pProfile->PutUCMProfile();
	}
	
	return nOutCheckCnt;
}

// 출금수표번호설정
CString	CDevCmn::fnAPL_SetNumberOfOutCheckNo(LPCTSTR szOutCheckNo, LPCTSTR szOutCheckNo2)
{
	if (fnAPL_GetDefineDevice(DEV_UCM)	||
		fnAPL_GetDefineDevice(DEV_CAM)	)
	{
		switch (AtmDefine.CheckHandler)
		{
			case CAM_SET:
				break;
				
			case UCM_SET:
				m_pProfile->GetUCMProfile();
				memcpy(m_pProfile->UCM.CrntOutCheckNo, szOutCheckNo, 8);
				m_pProfile->UCM.CrntOutCheckNo[8] = NULL;
				memcpy(m_pProfile->UCM.CrntOutCheckNo2, szOutCheckNo2, 8);
				m_pProfile->UCM.CrntOutCheckNo2[8] = NULL;
				m_pProfile->PutUCMProfile();
				break;											
		}
	}
	else
	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		m_pProfile->GetUCMProfile();
		memcpy(m_pProfile->UCM.CrntOutCheckNo, szOutCheckNo, 8);
		m_pProfile->UCM.CrntOutCheckNo[8] = NULL;
		memcpy(m_pProfile->UCM.CrntOutCheckNo2, szOutCheckNo2, 8);
		m_pProfile->UCM.CrntOutCheckNo2[8] = NULL;
		m_pProfile->PutUCMProfile();
	}
	
	return szOutCheckNo;
}

// 출금수표은행코드 설정
CString	CDevCmn::fnAPL_SetOutCheckBankNo(LPCTSTR szBankNo, LPCTSTR szBankNo2)
{
	if (fnAPL_GetDefineDevice(DEV_UCM))
	{
		switch (AtmDefine.CheckHandler)
		{
		case CAM_SET:
			break;
			
		case UCM_SET:
			m_pProfile->GetUCMProfile();
			memcpy(m_pProfile->UCM.BankNo, szBankNo, 2);
			memcpy(m_pProfile->UCM.BankNo2, szBankNo2, 2);
			m_pProfile->UCM.BankNo[2] = NULL;
			m_pProfile->UCM.BankNo2[2] = NULL;
			m_pProfile->PutUCMProfile();
			break;											
		}
	}
	
	return szBankNo;
}

// 입금수표매수설정
int	CDevCmn::fnAPL_SetNumberOfInCheck(int nInCheckCnt)
{
	if (fnAPL_GetDefineDevice(DEV_UCM)	||
		fnAPL_GetDefineDevice(DEV_CAM)	)
	{
		switch (AtmDefine.CheckHandler)
		{
			case CAM_SET:
				break;
				
			case UCM_SET:
				m_pProfile->GetUCMProfile();
				m_pProfile->UCM.CrntInCheckCnt = nInCheckCnt;
				m_pProfile->PutUCMProfile();
				break;											
		}
	}
	
	return nInCheckCnt;
}

// 거래가능상태구하기
int CDevCmn::fnAPL_GetAvailTrans()
{
	WithAvail	= fnAPL_GetAvailWithdraw();						// 출금가능상태구하기
	DepAvail	= fnAPL_GetAvailDeposit();						// 입금가능상태구하기
	PbmAvail	= fnAPL_GetAvailPassbook();						// 통장가능상태구하기
	A4pAvail	= fnAPL_GetAvailA4printer();					// A4P 가능상태구하기
	TranAvail	= fnAPL_GetClerkTransOnOff();					// 계원거래모드구하기
	return TranAvail;
}

// 출금가능상태구하기
int CDevCmn::fnAPL_GetAvailWithdraw()
{
	int		nTempWithAvail = TRAN_WITH_NOT;						// 초기치는 출금불가능 상태
	int		nTempCashCstTot = 0;
	int		nTempWithMaxAmt = 0;

	if ((fnAPL_GetAvailDevice(DEV_CSH))	&&
		(AtmDefine.CashHandler != CDU_SET))						// ATM일 경우만 해당함수를 Call하도록 수정
	{
		switch (fnCSH_GetAvailWithdraw()) 
		{
			case WITH_CASH_ONLY:								// 현금만 출금가능
				nTempWithAvail |= TRAN_WITH_CASH;
				if (Withdraw50TFlag)							
					nTempWithAvail |= TRAN_WITH_50TCASH;		
				break;
			case WITH_FIFTY_TH:									// 5만원만 출금 가능
				nTempWithAvail |= TRAN_WITH_50TCASH;
				break;
			case WITH_TEN_TH:									// 만원만 출금 가능
				nTempWithAvail |= TRAN_WITH_CASH;			
				break;

			case WITH_NOT_AVAILABLE:							// 출금 불가능
				break;

			case WITH_CHECK_ONLY:								// 수표만 출금가능
				nTempWithAvail |= TRAN_WITH_CHECK;
				break;

			case WITH_BOTH_AVAILABLE:							// 현금+수표 출금가능
				nTempWithAvail |= TRAN_WITH_CASH;
				nTempWithAvail |= TRAN_WITH_CHECK;
				if (Withdraw50TFlag)							
					nTempWithAvail |= TRAN_WITH_50TCASH;		
				break;

			default:
				break;
		}
	}

	if (fnAPL_GetAvailDevice(DEV_UCM))
	{
		switch (fnUCM_GetAvailWithdraw()) 
		{
			case WITH_CHECK_ONLY:								// 수표만 출금가능
				nTempWithAvail |= TRAN_WITH_CHECK;
				break;

			case WITH_NOT_AVAILABLE:							// 출금 불가능
			case WITH_CASH_ONLY:								// 현금만 출금가능
			case WITH_BOTH_AVAILABLE:							// 현금+수표 출금가능
			default:
				break;
		}

		if (fnUCM_GetWithCSTStatus() == CST_EMPTY)				// 출금수표공검지
			nTempWithAvail &= ~TRAN_WITH_CHECK;
		if (!fnAPL_GetNumberOfOutCheck())						// 출금수표매수구하기
			nTempWithAvail &= ~TRAN_WITH_CHECK;
	}
	else
	if ((fnAPL_GetAvailDevice(DEV_CSH))		&&					// CDP는 fnAPL_GetAvailDevice()함수에서 현금(1만+5만)과 수표 포함한 가능결과를 리턴함
		(AtmDefine.CashHandler == CDU_SET)	)					// ATM은 현금에 관해서만 출금가능결과를 리턴함
	{
		switch (fnCSH_GetAvailWithdraw())					
		{
			case WITH_CASH_ONLY:								// 현금만(5만+만) 출금가능 : 1
				nTempWithAvail |= TRAN_WITH_CASH;
				if(Withdraw50TFlag)								
					nTempWithAvail |= TRAN_WITH_50TCASH;			
				break;
			case WITH_FIFTY_TH:									// 5만원만 출금가능 : 5
				nTempWithAvail |= TRAN_WITH_50TCASH;
				break;
			case WITH_TEN_TH:									// 만원만 출금가능 : 4
				nTempWithAvail |= TRAN_WITH_CASH;				
				break;				
			case (WITH_TEN_TH+WITH_CHECK_ONLY) :				// 만+수표 출금가능 : 6
				nTempWithAvail |= TRAN_WITH_CASH;
				nTempWithAvail |= TRAN_WITH_CHECK;				
				break;
			case (WITH_FIFTY_TH+WITH_CHECK_ONLY) :				// 5만+수표 출금가능 : 7
				nTempWithAvail |= TRAN_WITH_50TCASH;
				nTempWithAvail |= TRAN_WITH_CHECK;				
				break;				
			case WITH_CHECK_ONLY:								// 수표만 출금가능 : 
				nTempWithAvail |= TRAN_WITH_CHECK;
				break;
			case WITH_BOTH_AVAILABLE:							// 현금(5만+만)+수표 출금가능
				nTempWithAvail |= TRAN_WITH_CASH;
				nTempWithAvail |= TRAN_WITH_CHECK;
				if(Withdraw50TFlag)							
					nTempWithAvail |= TRAN_WITH_50TCASH;			
				break;
			case WITH_NOT_AVAILABLE:							// 출금 불가능
				break;				
			default:
				break;
		}

		if (!fnAPL_GetNumberOfOutCheck())						// 출금수표매수구하기
			nTempWithAvail &= ~TRAN_WITH_CHECK;
	}		
	

	return nTempWithAvail;
}

// 입금가능상태구하기
int CDevCmn::fnAPL_GetAvailDeposit()
{
	int		nTempDepAvail = TRAN_DEP_NOT;						// 초기치는 입금 불가능 상태

	if (fnAPL_GetAvailDevice(DEV_CSH))
	{
		switch (fnCSH_GetAvailDeposit()) 
		{
			case DEP_CASH_ONLY:									// 현금만 입금가능
			case DEP_TEN_TH:									
			case DEP_ONLY_TEN_TH:								// 만원권만 입금가능
				nTempDepAvail |= TRAN_DEP_CASH;					
				break;
			case DEP_FIFTY_TH:									// 5만원+천,오천(1만원 불가)
			case DEP_ONLY_FIFTY_TH:							
			case DEP_BOTH_AVAILABLE:							// 현금+수표 입금가능
				nTempDepAvail |= TRAN_DEP_CASH;
				break;
			case DEP_NOT_AVAILABLE:								// 입금 불가능
			case DEP_CHECK_ONLY:								// 수표만 입금가능
			default:
				break;
		}
	}

	if (fnAPL_GetAvailDevice(DEV_UCM))
	{
		switch (fnUCM_GetAvailDeposit()) 
		{
			case DEP_CHECK_ONLY:								// 수표만 입금가능
				nTempDepAvail |= TRAN_DEP_CHECK;
				break;

			case DEP_NOT_AVAILABLE:								// 입금 불가능
			case DEP_CASH_ONLY:									// 현금만 입금가능
			case DEP_BOTH_AVAILABLE:							// 현금+수표 입금가능
			case DEP_ONLY_TEN_TH:								// 만원권만 입금가능
			default:
				break;
		}
	}

	return nTempDepAvail;
}

// 통장가능상태구하기
int CDevCmn::fnAPL_GetAvailPassbook()
{
	int		nTempPbmAvail = TRAN_PBM_NOT;						// 초기치는 통장 불가능 상태
	int		PbmRetractCnt = 0;
	if (fnAPL_GetAvailDevice(DEV_PBM))							
	{															// 통장 가능
		nTempPbmAvail |= TRAN_PBM_OK;
		// 적용후 1차 수정 (U8100 AP 변경내역 #13)
		// PBMInk상태가 OUT, NOTSET의 경우 통장거래 안되도록 수정   
		if (m_pDevCmn->AtmDefine.PBHandler == I_PBM_SET)
		{
			if ((memcmp(PBMInkStatus, PBM_INK_OUT, PBMInkStatus.GetLength()) == 0)		||
				(memcmp(PBMInkStatus, PBM_INK_NOTSET, PBMInkStatus.GetLength()) == 0)	||
				(memcmp(PBMInkStatus, "00 %", PBMInkStatus.GetLength()) == 0))				// T3ATM AP 변경내역 #19 - 통장잉크 잔량이 0이면 사용불가
				nTempPbmAvail = TRAN_PBM_NOT;
		}
		
		//#0012 - 통장미수취 1회 (버튼 Disable)
		if (m_pProfile->DEVICE.MachineType == U3100K)	
		{
			PbmRetractCnt = RegGetInt(_REGKEY_PBMSTATUS, "PBRetractCnt", 0);
			if (PbmRetractCnt > 0)
				nTempPbmAvail = TRAN_PBM_NOT;
		}
	}

	return nTempPbmAvail;
}
// A4P프린터 가능상태구하기
int CDevCmn::fnAPL_GetAvailA4printer()
{
	int		nTempA4pAvail = TRAN_A4P_NOT;						// 초기치는 통장 불가능 상태

	if (fnAPL_GetAvailDevice(DEV_A4P))							
	{															// A4P 가능
		if (A4pStatus == A4P_NORMAL)
			nTempA4pAvail |= TRAN_A4P_OK;
	}

	return nTempA4pAvail;
}

// 계원거래모드구하기
int CDevCmn::fnAPL_GetClerkTransOnOff()
{
	int		nTempWithAvail	= WithAvail;						// 처리변수 생성
	int		nTempDepAvail	= DepAvail;
	int		nTempPbmAvail	= PbmAvail;

// 출금관련 처리 	
	switch (nTempWithAvail) 
	{
		case TRAN_WITH_CASH:									// 현금만 출금가능
			if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CASH))	
				nTempWithAvail = TRAN_WITH_NOT;
			break;
		case TRAN_WITH_50TCASH:								
			if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_50TCASH))	
				nTempWithAvail = TRAN_WITH_NOT;
			break;											
		case TRAN_WITH_CHECK:									// 수표만 출금가능
			if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CHECK))	
				nTempWithAvail = TRAN_WITH_NOT;
			break;

		case TRAN_WITH_CASHCHECK:							
			if ((!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CASH)) &&
				(!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CHECK)))
				nTempWithAvail = TRAN_WITH_NOT;
			else
			{
				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CASH))
					nTempWithAvail = TRAN_WITH_CHECK;
				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CHECK))
					nTempWithAvail = TRAN_WITH_CASH;
			}
			break;											
		case TRAN_WITH_50TCASHCHECK:							
			if ((!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_50TCASH)) &&
				(!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CHECK)))
				nTempWithAvail = TRAN_WITH_NOT;
			else
			{
				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_50TCASH))
					nTempWithAvail = TRAN_WITH_CHECK;
				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CHECK))
					nTempWithAvail = TRAN_WITH_50TCASH;
			}
			break;														
		case TRAN_WITH_2CASH:																
			if ((!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CASH)) &&
				(!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_50TCASH)))
				nTempWithAvail = TRAN_WITH_NOT;
			else 
			{
				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CASH))
					nTempWithAvail = TRAN_WITH_50TCASH;
				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_50TCASH))
						nTempWithAvail = TRAN_WITH_CASH;
			}
			break;												
		case TRAN_WITH_BOTH:									// 현금+수표 출금가능
			if ((!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CASH)) &&
				(!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_50TCASH)) &&		
				(!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CHECK)))
				nTempWithAvail = TRAN_WITH_NOT;
			else 
			{
				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CASH))					
					nTempWithAvail = TRAN_WITH_50TCASHCHECK;
				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_50TCASH))
					nTempWithAvail = TRAN_WITH_CASHCHECK;
				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CHECK))
					nTempWithAvail = TRAN_WITH_2CASH;

				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_2CASH))
					nTempWithAvail = TRAN_WITH_CHECK;
				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CASHCHECK))
					nTempWithAvail = TRAN_WITH_50TCASH;
				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_50TCASHCHECK))
					nTempWithAvail = TRAN_WITH_CASH;								
			}
			break;

		case TRAN_WITH_NOT:										// 출금 불가능
		default:
			nTempWithAvail = TRAN_WITH_NOT;
			break;
	}

// 입금관련 처리
	switch (nTempDepAvail) 
	{
		case TRAN_DEP_CASH:										// 현금만 입금가능
			if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_DEP_CASH))
				nTempDepAvail = TRAN_DEP_NOT;
			break;
		case TRAN_DEP_CHECK:									// 수표만 입금가능
			if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_DEP_CHECK))
				nTempDepAvail = TRAN_DEP_NOT;
			break;
		case TRAN_DEP_BOTH:										// 현금+수표 입금가능
			if ((!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_DEP_CASH)) &&
				(!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_DEP_CHECK)))
				nTempDepAvail = TRAN_DEP_NOT;
			else 
			{
				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_DEP_CASH))
					nTempDepAvail = TRAN_DEP_CHECK;
				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_DEP_CHECK))
					nTempDepAvail = TRAN_DEP_CASH;
			}
			break;

		case TRAN_DEP_NOT:										// 입금 불가능
		default:
			nTempDepAvail = TRAN_DEP_NOT;
			break;
	}

// 통장관련 처리
	switch (nTempPbmAvail) 
	{
		case TRAN_PBM_OK:										// 통장 가능
			if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_PBM_OK))
				nTempPbmAvail = TRAN_PBM_NOT;
			break;

		case TRAN_PBM_NOT:										// 통장 불가능
		default:
			nTempPbmAvail =TRAN_PBM_NOT;
			break;
	}

	WithAvail	= nTempWithAvail;								// 처리변수 보관
	DepAvail	= nTempDepAvail;
	PbmAvail	= nTempPbmAvail;
	return (nTempWithAvail | nTempDepAvail | nTempPbmAvail);
}

// 센서정보설정
int CDevCmn::fnAPL_SetSensorInfo()
{
	CString	strTempSensor("");
	CString strCst3Type("");

	if (AtmStatus == ATM_INIT) 									// 기기초기화 모드
		return TRUE;
	
	if (m_pProfile->DEVICE.MachineType == U8100)			
	{
		if (AtmStatus == ATM_READY || AtmStatus == ATM_ERROR || AtmStatus == ATM_CLERK)
			;
		else
		if ((AtmStatus == ATM_CUSTOM) && 
			((AtmStatusSave == ATM_CUSTOM) || (AtmStatusSave == ATM_TRAN)))
		{
			if (m_pProfile->DEVICE.EarHandler == EAR_SET)
				EarPhoneInserted = fnSNS_GetEarPhoneStatus();
		}
		else
		{
			if (EarPhoneInserted)
			{
				fnSCR_SetDisplayData(EAR_KINDMODE, "on");
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetSensorInfo : is EarPhoneInserted");
			}
			else
			{
				if (((AtmStatus == ATM_TRAN) && (AtmStatusSave == ATM_CUSTOM))	||
					((AtmStatus == ATM_CUSTOM) && (AtmStatusSave == ATM_TRAN)))
					;
				else
				{
					EarPhoneInserted = FALSE;
					fnSNS_ClearPhoneStatus();
					fnSCR_SetDisplayData(EAR_KINDMODE, "off");
				}
			}
		}
	}


	if (fnAPL_GetYesDevice(DEV_BRM))						
	{
		switch (AtmDefine.CashHandler)
		{
			case BRM_SET:
			case OKI_SET:									
			case HBRM_SET:	
				CameraStatus    = fnCMR_GetFaceCameraDeviceStatus();
				BrmChangeLoc	= fnCSH_GetChangeUnitLock();	// 탈착정보구하기
				BrmLoc			= fnCSH_GetPosition();			// 정위치구하기
				RejectCSTLoc	= (fnCSH_GetRejectCSTStatus() == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS; 	// 회수카세트상태구하기															
				RetractCSTLoc	= (fnCSH_GetRetractCSTStatus() == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS; 	// 회수카세트상태구하기															
				ClerkCSTLoc		= (fnCSH_GetClerkCSTStatus() == CST_SET_NG)		? ABNORMAL_POS : NORMAL_POS;    // 계원카세트상태구하기
																
				FstCSTLoc		= (fnCSH_GetRBStatus(CDU_CST_1) == CST_SET_NG)   ? ABNORMAL_POS : NORMAL_POS;    // 1카세트정위치상태(Cd)
				SndCSTLoc		= (fnCSH_GetRBStatus(CDU_CST_2) == CST_SET_NG)   ? ABNORMAL_POS : NORMAL_POS;					// 2카세트정위치상태(Cd)
				TrdCSTLoc		= (fnCSH_GetRBStatus(CDU_CST_3) == CST_SET_NG)   ? ABNORMAL_POS : NORMAL_POS;					// 3카세트정위치상태(Cd)
				FthCSTLoc		= (fnCSH_GetRBStatus(CDU_CST_4) == CST_SET_NG)   ? ABNORMAL_POS : NORMAL_POS;					// 4카세트정위치상태(Cd)

//				RejectCSTStatus	= (fnCSH_GetRejectCSTStatus() == CST_SET_NG)	? RejectCSTStatus : fnCSH_GetRejectCSTStatus(); // 리젝트카세트상태		
				RejectCSTStatus	= fnCSH_GetRejectCSTStatus();      // 리젝트카세트상태		
				RetractCSTStatus= fnCSH_GetRetractCSTStatus();     // 회수함카세트상태	
				if (m_pProfile->DEVICE.MachineType == U3100K)
				{
					FstCSTStatus       = fnCSH_GetRBStatus(CDU_CST_1);  // CST1
					SndCSTStatus       = fnCSH_GetRBStatus(CDU_CST_2);  // CST2
					TrdCSTStatus       = fnCSH_GetRBStatus(CDU_CST_3);  // CST3
					FthCSTStatus       = fnCSH_GetRBStatus(CDU_CST_4);  // CST4  //#N0134
				}
				else
				if (m_pProfile->DEVICE.MachineType == U8100) //#N0134
				{
					FstCSTStatus       = fnCSH_GetRecycleBox50000Status();  // CST1
					SndCSTStatus       = fnCSH_GetRecycleBoxStatus();  // CST2,3,4 		
					TrdCSTStatus       = CST_NORMAL;
					FthCSTStatus       = CST_NORMAL;
				}
				
				ClerkCSTStatus	= (fnCSH_GetClerkCSTStatus() == CST_SET_NG)		? ClerkCSTStatus : fnCSH_GetClerkCSTStatus(); // 계원카세트함상태																
				RecycleBoxStatus= (fnCSH_GetRecycleBoxStatus() == CST_SET_NG)	? RecycleBoxStatus : fnCSH_GetRecycleBoxStatus();
				ClerkRejectBoxStatus = (fnCSH_GetClerkRJBStatus() == CST_SET_NG) ? ClerkRejectBoxStatus : fnCSH_GetClerkRJBStatus();
				
				if ((ChangeBCFlag) && (Withdraw50TFlag))
				{
					RecycleBox50000Status = (fnCSH_GetRecycleBox50000Status() == CST_SET_NG) ? RecycleBox50000Status : fnCSH_GetRecycleBox50000Status();
																// 현금상태
					switch (RecycleBoxStatus)
					{
						case CST_NORMAL:
							RecycleBoxStatus = CST_NORMAL;
							break;
						case CST_NEAR:
							if (RecycleBox50000Status == CST_NORMAL)
								RecycleBoxStatus = CST_NORMAL;
							else
							if (RecycleBox50000Status == CST_NEAR)
								RecycleBoxStatus = CST_NEAR;
							else
							if (RecycleBox50000Status == CST_EMPTY)
								RecycleBoxStatus = CST_NEAR;
							else
							if (RecycleBox50000Status == CST_SET_NG)
								RecycleBoxStatus = RecycleBox50000Status;
							else
							if (RecycleBox50000Status == CST_FULL)
								RecycleBoxStatus = CST_NORMAL;
							break;
						case CST_EMPTY:
							if (RecycleBox50000Status == CST_NORMAL)
								RecycleBoxStatus = CST_NORMAL;
							else
							if (RecycleBox50000Status == CST_NEAR)
								RecycleBoxStatus = CST_NEAR;
							else
							if (RecycleBox50000Status == CST_EMPTY)
								RecycleBoxStatus = CST_EMPTY;
							else
							if (RecycleBox50000Status == CST_SET_NG)
								RecycleBoxStatus = RecycleBox50000Status;
							else
							if (RecycleBox50000Status == CST_FULL)
								RecycleBoxStatus = CST_NORMAL;
							break;
						case CST_FULL:
							if (RecycleBox50000Status == CST_NORMAL)
								RecycleBoxStatus = CST_NORMAL;
							else
							if (RecycleBox50000Status == CST_NEAR)
								RecycleBoxStatus = CST_NORMAL;
							else
							if (RecycleBox50000Status == CST_EMPTY)
								RecycleBoxStatus = CST_NORMAL;
							else
							if (RecycleBox50000Status == CST_SET_NG)
								RecycleBoxStatus = RecycleBox50000Status;
							else
							if (RecycleBox50000Status == CST_FULL)
								RecycleBoxStatus = CST_FULL;
							break;
						default:
							break;
					}
				}
				break;

			case CDU_SET:

				SFstCSTCnt = IniGetInt(_SP_CSTINFO_INI, "PHYCASHUNIT1", "Count", 0);		// First CST Count(ClerkInformation)
				SSndCSTCnt = IniGetInt(_SP_CSTINFO_INI, "PHYCASHUNIT2", "Count", 0);		// Second CST Count(ClerkInformation)
				STrdCSTCnt = IniGetInt(_SP_CSTINFO_INI, "PHYCASHUNIT3", "Count", 0);		// Third CST Count(ClerkInformation)
				SFthCSTCnt = IniGetInt(_SP_CSTINFO_INI, "PHYCASHUNIT4", "Count", 0);		// Fourth CST Count(ClerkInformation)

				BrmChangeLoc	= fnCSH_GetChangeUnitLock();
				BrmLoc			= fnCSH_GetPosition();
				RejectCSTLoc	= (fnCSH_GetRejectCSTStatus() == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
				ClerkCSTLoc		= (fnCSH_GetClerkCSTStatus() == CST_SET_NG)		? ABNORMAL_POS : NORMAL_POS;

				RejectCSTStatus	= (fnCSH_GetRejectCSTStatus() == CST_SET_NG)	? RejectCSTStatus : fnCSH_GetRejectCSTStatus();
				ClerkCSTStatus	= (fnCSH_GetClerkCSTStatus() == CST_SET_NG)		? ClerkCSTStatus : fnCSH_GetClerkCSTStatus();
				RecycleBoxStatus= (fnCSH_GetRecycleBoxStatus() == CST_SET_NG)	? RecycleBoxStatus : fnCSH_GetRecycleBoxStatus();
				if (CSTCnt >= 1)
				{
					FstCSTCnt = fnCSH_GetNumberOfCash(CDU_CST_1);		// First CST Count(ClerkInformation)
					FstCSTLoc	= (fnCSH_GetCSTStatus(CDU_CST_1) == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
					FstCSTStatus= fnCSH_GetCSTStatus(CDU_CST_1);
				}
				if (CSTCnt >= 2)
				{
					SndCSTCnt = fnCSH_GetNumberOfCash(CDU_CST_2);		// Second CST Count(ClerkInformation)
					SndCSTLoc	= (fnCSH_GetCSTStatus(CDU_CST_2) == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
					SndCSTStatus= fnCSH_GetCSTStatus(CDU_CST_2);
				}
				if (CSTCnt >= 3)
				{
					TrdCSTCnt = fnCSH_GetNumberOfCash(CDU_CST_3);		// Third CST Count(ClerkInformation)
					TrdCSTLoc	= (fnCSH_GetCSTStatus(CDU_CST_3) == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
					TrdCSTStatus= fnCSH_GetCSTStatus(CDU_CST_3);
					if (TrdCSTStatus != CST_SET_NG)
					{
						strCst3Type = IniGetStr(_SP_CDMINFO_INI, "CASHUNIT3", "CurrencyID");
						if (!strCst3Type.CompareNoCase("CHK"))
						{
							if ((fnAPL_GetNumberOfOutCheck() <= BRM_NEARCHECKCNT)	&&
								(fnAPL_GetNumberOfOutCheck() >= BRM_EMPTYCHECKCNT)	)
								TrdCSTStatus = CST_NEAR;
							else
							if (fnAPL_GetNumberOfOutCheck() < BRM_EMPTYCHECKCNT)
								TrdCSTStatus = CST_EMPTY;
						}
					}
				}
				if (CSTCnt >= 4)
				{
					FthCSTCnt = fnCSH_GetNumberOfCash(CDU_CST_4);		// Fourth CST Count(ClerkInformation)
					FthCSTLoc	= (fnCSH_GetCSTStatus(CDU_CST_4) == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
					FthCSTStatus= fnCSH_GetCSTStatus(CDU_CST_4);
				}

				break;
		}
	}

	if (fnAPL_GetYesDevice(DEV_UCM))
	{
		switch (AtmDefine.CheckHandler)
		{
			case CAM_SET:
				break;

			case UCM_SET:
				UcmLoc			= fnUCM_GetPosition();			// 정위치구하기

				if (fnUCM_GetWithCSTStatus() == CST_SET_NG);	// 출금수표상태작성
				else
				if (fnUCM_GetWithCSTStatus() == CST_EMPTY)		// 출금수표공검지
					CheckCSTStatus = CST_EMPTY;
				else
				if ((fnAPL_GetNumberOfOutCheck() < BRM_EMPTYCHECKCNT)	&&
					(fnAPL_GetNumberOfOutCheck2() < BRM_EMPTYCHECKCNT)	)
					CheckCSTStatus = CST_EMPTY;
				else
				if ((fnAPL_GetNumberOfOutCheck() < BRM_NEARCHECKCNT)	&&
					(fnAPL_GetNumberOfOutCheck2() < BRM_NEARCHECKCNT)	)
					CheckCSTStatus = CST_NEAR;
				else
					CheckCSTStatus = CST_NORMAL;

				if (fnUCM_GetWithCSTStatus() == CST_SET_NG);	// 출금수표상태(실물)작성
				else
				if (fnUCM_GetWithCSTStatus() == CST_EMPTY)		// 출금수표공검지
					CheckCSTStatusSensor = CST_EMPTY;
				else
					CheckCSTStatusSensor = CST_NORMAL;
																// 출금수표상태(등록)작성
				if ((fnAPL_GetNumberOfOutCheck() < BRM_EMPTYCHECKCNT)	&&
					(fnAPL_GetNumberOfOutCheck2() < BRM_EMPTYCHECKCNT)	)
					CheckCSTStatusReg = CST_EMPTY;				
				else
				if ((fnAPL_GetNumberOfOutCheck() < BRM_EMPTYCHECKCNT)	&&
					(fnAPL_GetNumberOfOutCheck2() < BRM_EMPTYCHECKCNT)	)
					CheckCSTStatusReg = CST_NEAR;
				else
					CheckCSTStatusReg = CST_NORMAL;

				if (fnUCM_GetDepCSTStatus() == CST_SET_NG);		// 입금수표상태작성
				else
				if (fnUCM_GetDepCSTStatus() == CST_FULL)
					CassetteStatus = CST_FULL;
				else
					CassetteStatus = CST_NORMAL;
				break;											
		}
	}

	if (fnAPL_GetYesDevice(DEV_DOR))
	{
		switch(m_pProfile->DEVICE.MachineType)
		{
		case U3100K:
			DoorStatus		= fnDOR_GetDoorStatus();
			CabinetStatus   = fnDOR_GetCabinetStatus();
			break;
		case U8100:
			if (IniGetStr(_SYSTEMCDINI, "SYSTEM", "NAME").Find("U8500"))		// #0405
			{
				DoorStatus		= fnDOR_GetDoorStatus();
				CabinetStatus   = fnDOR_GetCabinetStatus();
			}
			else
			{
				DoorStatus		= fnDOR_GetDoorStatus();
			}
			break;
		default:
			DoorStatus		= fnDOR_GetDoorStatus();				// 뒷문상태구하기
			break;
		}
	}

	if (fnAPL_GetYesDevice(DEV_SPR))
	{
		strTempSensor = fstrSPR_GetSensorInfo();				// 장치센서구하기
		if ((strTempSensor.Mid(11,1) == "1")	||				// 레버오픈
			(strTempSensor.Mid(9,1) == "1"))					// 반송로오픈
			SlipHeadStatus	= SLIP_HEAD_UP;
		else
			SlipHeadStatus	= HEADER_NORMAL;
		SlipStatus		= fnSPR_GetPaperStatus();				// 용지상태구하기
	}

	if (fnAPL_GetYesDevice(DEV_JPR))
	{
		strTempSensor = fstrJPR_GetSensorInfo();				// 장치센서구하기
		JnlHeadStatus	= HEADER_NORMAL;						// 저널Cover상태(향후)
		JnlStatus		= fnJPR_GetPaperStatus();				// 용지상태구하기
	}

	if (fnAPL_GetYesDevice(DEV_A4P))
	{
//		strTempSensor = fnA4P_GetPaperStatus();					// 장치센서구하기
//		if ((strTempSensor.Mid(22,1) == "1")	&&				// 레버오픈
//			(strTempSensor.Mid(23,1) == "1"))					// 반송로오픈
//			A4pHeadStatus	= HEADER_NORMAL;
//		else
//			A4pHeadStatus	= SLIP_HEAD_UP;

		if (!fnA4P_GetDeviceStatus())							// ST_DEVICEVSTATUS = 0 이면 정상
			A4pHeadStatus = HEADER_NORMAL;
		else
			A4pHeadStatus = DOWN;								// 장애 또는 NODEVICE

		if (fnA4P_GetPaperStatus() & A4P_EMPTY_PAPER)
			A4pStatus = A4P_EMPTY_PAPER;						// 트레이용지부족
		else if(fnA4P_GetPaperStatus() & A4P_LOW_END)
			A4pStatus = A4P_LOW_END;
		else
			A4pStatus = A4P_NORMAL;							    // 정상장착
	}

	if (fnAPL_GetYesDevice(DEV_SPG))
	{
		GAlarmStatus		= fnSPACK_GetAlarmStatus();				// Alarm Status
		GSensorLelvel		= fnSPACK_GetGSensorSensibility();		// G-Sensor 설정 값
		GBatteryStatus		= fnSPACK_GetBatteryLowStatus();		// Battery 설정 값
	}

	return TRUE;
}

// 거래일자설정
int CDevCmn::fnAPL_SetDate(LPCTSTR szDate)
{
	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.YYYYMMDD, szDate, 8);
	m_pProfile->TRANS.YYYYMMDD[8] = NULL;
	m_pProfile->PutTransProfile();

	return TRUE;
}

// 수표일자설정
int CDevCmn::fnAPL_SetCheckDate(LPCTSTR szCheckDate)
{
	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.CheckYYYYMMDD, szCheckDate, 8);
	m_pProfile->TRANS.CheckYYYYMMDD[8] = NULL;
	m_pProfile->PutTransProfile();

	CString strRegCheckData("");
	switch (m_pProfile->DEVICE.MachineType)
	{
		case U8100:												
		case U3100K:											
			break;
		default:
			strRegCheckData.Format("%4.4s %2.2s %2.2s", &szCheckDate[0], &szCheckDate[4], &szCheckDate[6]);
			RegSetStr("SOFTWARE\\ATM\\DEVINFO\\CDM", "CheckDate", strRegCheckData.GetBuffer(0));
			break;
	}

	return TRUE;
}

// 일련번호초기화
int CDevCmn::fnAPL_ClearSerialNo()
{
	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.SerialNo, "000001", 6);
	m_pProfile->TRANS.SerialNo[6] = NULL;
	m_pProfile->PutTransProfile();
	return TRUE;
}

// 일련번호증가
int CDevCmn::fnAPL_AddSerialNo()
{
	char	szTempSerialNo[256];
	
	memcpy(szTempSerialNo, m_pProfile->TRANS.SerialNo, 6);

	if (memcmp(szTempSerialNo, "999999", 6) == 0)
		memset(szTempSerialNo, '0', 6);
	AddString(szTempSerialNo, "000001", 6);

	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.SerialNo, szTempSerialNo, 6);
	m_pProfile->TRANS.SerialNo[6] = NULL;
	m_pProfile->PutTransProfile();
	return TRUE;
}

// 일련번호설정
int CDevCmn::fnAPL_SetSerialNo()
{
	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.SerialNo, m_pProfile->TRANS.StartSerialNo, 6);
	m_pProfile->TRANS.SerialNo[6] = NULL;
	m_pProfile->PutTransProfile();
	return TRUE;
}

// 진행번호설정
int CDevCmn::fnAPL_SetProcCount(char chProcCount)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[진행번호] : %1c", chProcCount);

	m_pProfile->GetTransProfile();
	m_pProfile->TRANS.ProcCount[0] = chProcCount;
	m_pProfile->TRANS.ProcCount[1] = NULL;
	m_pProfile->PutTransProfile();

	fnAPL_DisplayProcCountSegment();							// 운영Segment출력
	return TRUE;
}

// 거래진행중설정
int CDevCmn::fnAPL_SetOnTransaction(int nTransDoing)
{
	m_pProfile->GetTransProfile();
	m_SendRecvStaus = PIDILE;
	m_nTrPollDoing = FALSE;
	m_pProfile->TRANS.TransPowerOnOff = nTransDoing; 
	m_pProfile->PutTransProfile();
	return TRUE;
}

// 거래시작설정
int CDevCmn::fnAPL_SetTranStart()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_SetTranStart]");

	m_pProfile->GetTransProfile();
	m_pProfile->TRANS.ProcCount[0] = '1';
	m_pProfile->TRANS.ProcCount[1] = NULL;
	m_pProfile->TRANS.TransPowerOnOff = ON; 
	m_pProfile->PutTransProfile();

	m_nTrPollDoing = TRUE;
	m_SendRecvStaus = PIDILE;
	AfxBeginThread(thrPollThead, this, THREAD_PRIORITY_NORMAL);

	fnAPL_DisplayInformationLedOnOff(SPLLED_DOING_TRAN, TRUE);
	fnAPL_DisplayProcCountSegment();							// 운영Segment출력
	return TRUE;
}

// 장치운영불가정보초기화
int CDevCmn::fnAPL_ClearDeviceExecOffInfo(int nInitFlag)
{
	if ((nInitFlag) ||		
		((memcmp(m_pProfile->TRANS.YYYYMMDD, m_pProfile->TRANS.DeviceYYYYMMDD, 8) != 0)))
	{
		m_pProfile->GetTransProfile();
		m_pProfile->TRANS.DeviceExecOff = DEV_NONE;
		memcpy(m_pProfile->TRANS.DeviceYYYYMMDD, m_pProfile->TRANS.YYYYMMDD, 8);
		m_pProfile->TRANS.DeviceSprCnt = 0;						
		m_pProfile->TRANS.DeviceJprCnt = 0;
		m_pProfile->TRANS.DeviceMcuCnt = 0;
		m_pProfile->TRANS.DeviceBrmCnt = 0;
		m_pProfile->TRANS.DeviceUcmCnt = 0;
		m_pProfile->TRANS.DevicePbmCnt = 0;
		
		m_pProfile->PutTransProfile();
	}
	return TRUE;
}

// 장치운영불가정보초기화
int CDevCmn::fnAPL_CshRetactCntInfo(int nCnt, int nInitFlag)
{
	m_pProfile->GetTransProfile();

	if(nInitFlag) 
		m_pProfile->TRANS.DeviceCshCnt = 0;
	else
		m_pProfile->TRANS.DeviceCshCnt = nCnt;

	m_pProfile->PutTransProfile();
	return TRUE;
}

// Set Clerk Power Off
int CDevCmn::fnAPL_SetClerkPowerOff()
{
//	fnAPL_StackError("POWERCL", "CLERK POWER OFF !!!");
	PowerOffDetect |= CLERK_POWER_OFF;
	RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 0);	// 2007.12.08 (0: 정전시 On/off, 2: 정전시 수동 On)
	return TRUE;
}

// 파워상태검지
int CDevCmn::fnAPL_CheckPowerStatus(int nRealFlag)
{
	int		nTempPowerOffDetect;

	if ((AtmStatus == ATM_INIT) ||								// 기기초기화 모드
		(AtmStatus == ATM_TRAN))								// 고객거래중 모드
		return TRUE;
	
	if (AtmStatus != AtmStatusSave)								// 상태가동일시유효
		return TRUE;

	if (PowerOffDetect & (EXT_AC_OFF | INT_BATTERY_LOW | CLERK_POWER_OFF | POWER_ABNORMAL))
		return FALSE;

	nTempPowerOffDetect = fnAPL_GetPowerStatus();				// 전원상태구하기

	RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 0);		// 2007.12.08 (0: 정전시 On/off, 2: 정전시 수동 On)
	if (nTempPowerOffDetect & EXT_AC_OFF)						// 정전또는AC오프
	{
//		fnAPL_StackError("POWERAC", "정전또는AC오프");			// 정전인 경우 운영저널로 분류 - 동양종금
		PowerOffDetect |= EXT_AC_OFF;
		fnAPL_PrintJnlInfo("POWERAC : 정전또는AC오프", JNL_OPER);
																// 2004.02.25 전원부상태저널인자
		return FALSE;
	}

//	U8500 배터리 대신 RPC 장착시 LOW BATTERY 장애 대응 #N0255
/*	if (nTempPowerOffDetect & INT_BATTERY_LOW)					// 밧데리LOW
	{
		fnAPL_StackError("POWERBA", "밧데리LOW");
		PowerOffDetect |= INT_BATTERY_LOW;
		fnAPL_PrintJnlInfo("POWERBA  : 밧데리LOW", JNL_ERROR);	// 2004.02.25 전원부상태저널인자
		return FALSE;
	}
*/

	if (nTempPowerOffDetect & CLERK_POWER_OFF)					// 계원전원오프
	{
//		fnAPL_StackError("POWERCL", "계원전원오프");
		PowerOffDetect |= CLERK_POWER_OFF;
		fnAPL_PrintJnlInfo("POWERCL  : 계원전원오프", JNL_OPER);
																// 2004.02.25 전원부상태저널인자
		return FALSE;
	}

	if (nTempPowerOffDetect & POWER_ABNORMAL)					// UPS상태불량
	{
		fnAPL_StackError("POWERAB", "UPS상태불량");
		PowerOffDetect |= POWER_ABNORMAL;
		fnAPL_PrintJnlInfo("POWERAB  : UPS상태불량", JNL_ERROR);// 2004.02.25 전원부상태저널인자
		return FALSE;
	}

	return TRUE;
}

// 파워오프장애검지
int CDevCmn::fnAPL_CheckPowerOffError()
{
	if (m_pProfile->TRANS.TransPowerOnOff == ON) 
	{
		fnAPL_StackError("991@@91", "거래중전원오프");
		m_pProfile->GetTransProfile();
		m_pProfile->TRANS.TransPowerOnOff = OFF; 
		m_pProfile->PutTransProfile();
		return FALSE;
	}

	return TRUE;
}

// 장애정보초기화
int CDevCmn::fnAPL_ClearError()
{

	CurErrCnt = 0;												// 현재장애갯수
	m_pProfile->GetCrtErrStack(&CurErrBuff, TRUE);

	memset(OrignalErrorCode, 0, sizeof(OrignalErrorCode));		// V07-01-04-#02
/////////////////////////////////////////////////////////////////////////////
	// Om정보전달
	Om.ErrorCode		= "0000000";							// 장애코드
	Om.McuDeviceStatus	= "정상";								// Mcu장치상태(정상,장애)
	Om.McuProcCount		= " ";									// Mcu장애진행카운터(정상='0',' ','0','1','3','5','6','9')
	Om.McuErrorCode		= "0000000";							// Mcu장애코드(정상="0000000",장애="1234567")
	Om.SprDeviceStatus	= "정상";								// Spr장치상태
	Om.SprProcCount		= " ";									// Spr장애진행카운터
	Om.SprErrorCode		= "0000000";							// Spr장애코드
	Om.JprDeviceStatus	= "정상";								// Jpr장치상태
	Om.JprProcCount		= " ";									// Jpr장애진행카운터
	Om.JprErrorCode		= "0000000";							// Jpr장애코드
	Om.PbmDeviceStatus	= "정상";								// Pbm장치상태
	Om.PbmProcCount		= " ";									// Pbm장애진행카운터
	Om.PbmErrorCode		= "0000000";							// Pbm장애코드
	Om.UcmDeviceStatus	= "정상";								// Ucm장치상태
	Om.UcmProcCount		= " ";									// Ucm장애진행카운터
	Om.UcmErrorCode		= "0000000";							// Ucm장애코드
	Om.BrmDeviceStatus	= "정상";								// Brm장치상태
	Om.BrmProcCount		= " ";									// Brm장애진행카운터
	Om.BrmErrorCode		= "0000000";							// Brm장애코드
	Om.NetDeviceStatus	= "정상";								// Net장치상태
	Om.NetProcCount		= " ";									// Net장애진행카운터
	Om.NetErrorCode		= "0000000";							// Net장애코드
	Om.AppDeviceStatus	= "정상";								// App장치상태
	Om.AppProcCount		= " ";									// App장애진행카운터
	Om.AppErrorCode		= "0000000";							// App장애코드
	Om.A4pDeviceStatus	= "정상";								// A4p장치상태
	Om.A4pProcCount		= " ";									// A4p장애진행카운터
	Om.A4pErrorCode		= "0000000";							// A4p장애코드
	fnAPL_ClerkInformation();									// 계원정보
/////////////////////////////////////////////////////////////////////////////

	return TRUE;
}

// 장애검지
int CDevCmn::fnAPL_CheckError()
{
	CurErrCnt = m_pProfile->GetCrtErrStack(&CurErrBuff, FALSE); // 현재장애내역
	if (CurErrCnt)
	{
		m_pProfile->GetFirstErrStack(&FirstErrBuff);			// 최초장애내역
		m_pProfile->GetLastErrStack(&LastErrBuff);				// 최종장애내역
		return FALSE;											// 장애 유
	}
	else 
		return TRUE;											// 장애 무
}

// 장애코드인증검사
int CDevCmn::fnAPL_CheckErrorCodeValid(LPCTSTR szErrorCode)
{
	if ((IsSpace((LPSTR)szErrorCode, 7))				||		// 2005.05.18
		(IsZero((LPSTR)szErrorCode, 7))					||
		(FindChar((LPSTR)szErrorCode, 7, NULL) != 7))
		return FALSE;
	else 
		return TRUE;											// 인증 정상
}

// 장애설정및장애정보보관													
int CDevCmn::fnAPL_StackError(LPCTSTR szErrorCode, LPCTSTR szErrorString, int nDevId, int nTranType)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[장애발생] : %1c-%7.7s %s", m_pProfile->TRANS.ProcCount[0], szErrorCode, szErrorString);

	CString	strTempErrorcode(szErrorCode);
	

	// V07-01-04-#02:최초장애저장
	if (IsNull(OrignalErrorCode, 8))
	{
		memcpy(OrignalErrorCode, szErrorCode, 
								__min(strlen(szErrorCode), sizeof(OrignalErrorCode)));
	}
	//////////////////////////////////////////////////////////////////////////
	// U8100 ErrorCode Conversion
	if (m_pProfile->DEVICE.MachineType == U8100)
		strTempErrorcode = fnAPL_ErrorConv(szErrorCode);
	else
	if (m_pProfile->DEVICE.MachineType == U3100K)
		strTempErrorcode = fnAPL_ErrorU3100KConv(szErrorCode);

	//////////////////////////////////////////////////////////////////////////
	
	strTempErrorcode += "0000000";
	strTempErrorcode = strTempErrorcode.Left(7);

	// Om정보전달 : 2003.11.07
	if (nDevId & (DEV_JPR | DEV_SPR | DEV_MCU | DEV_PBM | DEV_CSH | DEV_UCM))
	{
		if (nDevId & DEV_JPR)
		{
			Om.JprDeviceStatus = "장애";
			Om.JprProcCount = m_pProfile->TRANS.ProcCount[0];
			Om.JprErrorCode.Format("%7.7s", strTempErrorcode);
		}
		if (nDevId & DEV_SPR)
		{
			Om.SprDeviceStatus = "장애";
			Om.SprProcCount = m_pProfile->TRANS.ProcCount[0];
			Om.SprErrorCode.Format("%7.7s", strTempErrorcode);
		}
		if (nDevId & DEV_MCU)
		{
			Om.McuDeviceStatus = "장애";
			Om.McuProcCount = m_pProfile->TRANS.ProcCount[0];
			Om.McuErrorCode.Format("%7.7s", strTempErrorcode);
		}
		if (nDevId & DEV_UCM)
		{
			Om.UcmDeviceStatus = "장애";
			Om.UcmProcCount = m_pProfile->TRANS.ProcCount[0];
			Om.UcmErrorCode.Format("%7.7s", strTempErrorcode);
		}
		if (nDevId & DEV_CSH)
		{
			Om.BrmDeviceStatus = "장애";
			Om.BrmProcCount = m_pProfile->TRANS.ProcCount[0];
			Om.BrmErrorCode.Format("%7.7s", strTempErrorcode);
		}
		if (nDevId & DEV_PBM)
		{
			Om.PbmDeviceStatus = "장애";
			Om.PbmProcCount = m_pProfile->TRANS.ProcCount[0];
			Om.PbmErrorCode.Format("%7.7s", strTempErrorcode);
		}

		if (nDevId & DEV_A4P)
		{
			Om.A4pDeviceStatus = "장애";
			Om.A4pProcCount = m_pProfile->TRANS.ProcCount[0];
			Om.A4pErrorCode.Format("%7.7s", strTempErrorcode);
		}

	}
	else
	if ((strTempErrorcode.Mid(0,2) == "77")	||
		(strTempErrorcode.Mid(0,2) == "88"))
	{
		Om.NetDeviceStatus = "장애";
		Om.NetProcCount = m_pProfile->TRANS.ProcCount[0];
		Om.NetErrorCode.Format("%7.7s", strTempErrorcode);
	}
	else
	{
		Om.AppDeviceStatus = "장애";
		Om.AppProcCount = m_pProfile->TRANS.ProcCount[0];
		Om.AppErrorCode.Format("%7.7s", strTempErrorcode);
	}
	fnAPL_ClerkInformation();									// 계원정보
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
	CString		strTempDbData("");
	CString		strTemp("");

	// 장애History기록
	// OM_ERRORHISTORY||
	strTempDbData = "OM_ERRORHISTORY";

	// 장애발생일(YYYYMMDD)||
	strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
	strTempDbData += "||" + strTemp;

	// 장애발생시간(HHMMSS)||
	strTemp.Format("%6.6s", GetTime().GetBuffer(0));
	strTempDbData += "||" + strTemp;

	// 장애코드(#######)||
	strTemp.Format("%7.7s", strTempErrorcode);
	strTempDbData += "||" + strTemp;

	// 진행카운트
	strTemp.Format("%1c", m_pProfile->TRANS.ProcCount[0]);
	strTempDbData += "||" + strTemp;
	
	fnAPL_EjrDbManagerSend(strTempDbData);			// 전자저널자료전송
/////////////////////////////////////////////////////////////////////////////
	if (!fnAPL_CheckErrorCodeValid(strTempErrorcode.GetBuffer(0)))	// V07-03-01-#04
	{
		m_pProfile->ErrStack(m_pProfile->TRANS.ProcCount[0], "UNKNOWN", "내부통신 장애");	// U8100 AP 변경내역 #11 - 기타항목...
	}
	else 
		m_pProfile->ErrStack(m_pProfile->TRANS.ProcCount[0], strTempErrorcode.GetBuffer(0), (LPSTR)szErrorString);

	fnAPL_DisplayErrorSegment(m_pProfile->TRANS.ProcCount[0], strTempErrorcode.GetBuffer(0));	// V07-03-01-#04
																// 장애Segment출력
	return TRUE;
}

// V07-01-04-#02:U8100 ErrorCode Conversion
CString CDevCmn::fnAPL_ErrorConv(LPCTSTR szErrorCode)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ErrorConv Before[%s]", szErrorCode);
	CString strReturn("");

	for (int i = 0; i < itemof(StandardErrorConvTbl); i++)
	{
		if (memcmp(szErrorCode, StandardErrorConvTbl[i].EPError, StandardErrorConvTbl[i].CmpLength) == 0)
		{
			strReturn.Format("%5.5s", StandardErrorConvTbl[i].StandardError);
			break;
		}
	}

	if (i == itemof(StandardErrorConvTbl))
		strReturn = szErrorCode;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ErrorConv After[%s]", strReturn);
	return strReturn;
}

CString CDevCmn::fnAPL_ErrorU3100KConv(LPCTSTR szErrorCode)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ErrorConv Before[%s]", szErrorCode);
	CString strReturn("");

	for (int i = 0; i < itemof(ErrorConvTbl_U3100K); i++)
	{
		if (memcmp(szErrorCode, ErrorConvTbl_U3100K[i].MTC, 5) == 0)
		{
			strReturn.Format("%5.5s", ErrorConvTbl_U3100K[i].MTC);
			break;
		}
	}

	if (i == itemof(ErrorConvTbl_U3100K))
		strReturn = szErrorCode;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ErrorConv After[%s]", strReturn);
	return strReturn;
}
// 호스트라인검지
int CDevCmn::fnAPL_CheckHostLine()
{
//	if (HostLineMode == HOST_LOCAL_MODE)
//		return TRUE;
	
	if ((HostStatus == HOST_OFFLINE) ||
		(HostStatus == HOST_WAITLINK))
		return FALSE;
	return TRUE;
}

// 호스트개국검지
int CDevCmn::fnAPL_CheckHostOpen()
{
//	if (HostLineMode == HOST_LOCAL_MODE)
//		return TRUE;

	if (HostStatus != HOST_ONLINE)
		return FALSE;
	return TRUE;
}

// 호스트라인폐국
int CDevCmn::fnAPL_CloseLine()
{
	HostStatus = HOST_OFFLINE;									// 교신불가
//	fnNET_Deinitialize(LINE_OPCL_TIME);							// Socket Close
	HostOpenRetryCnt = 0;										// 개국시도횟수(준비완포함)
	Timer.HostOpenRetryTime = 5;								// 개국재시도타임
	return TRUE;
}

// 호스트폐국
int CDevCmn::fnAPL_CloseHost()
{
	if (fnAPL_CheckHostOpen())									// 호스트개국검지
//		HostStatus = HOST_WAITONLINE;							// 준비완대기
		HostStatus = HOST_WAITREADY;							// 준비완대기
		HostOpenRetryCnt = 0;										// 개국시도횟수(준비완포함)
		Timer.HostOpenRetryTime = 5;								// 개국재시도타임
	return TRUE;
}

int CDevCmn::fnAPL_OpenLine()
{
	if (HostStatus == HOST_INIT)
		HostStatus = HOST_OFFLINE;

	return TRUE;
}

// 장치초기화
int CDevCmn::fnAPL_InitializeDevice(int nDevId, int nInitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[장치초기화] : %x,%d", nDevId, nInitFlag);

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[초기화   : START ]"));	
	int		nTempRecoveryDevice = DEV_NONE;
	int		nRBType = 0;									
	int		nEnabledNoteKind = 0;							
	int		nCSTType = 0;								

	CString	strCduCST1Currency("KRW");						
	CString	strCduCST2Currency("KRW");						
	CString	strCduCST3Currency("CHK");						
	CStringArray strTempRetractArray;							
	CString strTemp("0");									
	CStringArray strTempArray;		

	Mcu97933Error = FALSE;										// #NICCM01	ICCM 모듈 적용
	Mcu97934Error = FALSE;										// #NICCM01	ICCM 모듈 적용
	

	//#0023
	if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 701)
	{
		CString strVersionData("");
		strVersionData.Format("[청호이지캐쉬] U3100K/U8100 %9.9s(%2.2s)]", m_pProfile->TRANS.ApVersion, REVISION); //#0097
		fnSCR_DisplayString(1, strVersionData.GetBuffer(0));	
		fnSCR_DisplayString(3, "빠른 시간내에 복구예정이오니");
		fnSCR_DisplayString(4, "잠시만 기다려 주십시오");
		fnSCR_DisplayScreen(701);
	}


	nDevId = fnAPL_GetDefineDevice(nDevId);						// 정의된장치구하기
	nTempRecoveryDevice = fnAPL_GetAvailErrorDevice(nDevId);	// 사용하고장애난장치구하기
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[장치초기화:DefineDevice] : %x,%d", nDevId, nInitFlag);

	fnAPL_SetSegment("88888888");									// Segment설정(값)
	fnLGT_SetFlicker(KU_ALL_FLICKER, FLICKER_OFF);					// Flicker설정(인덱스,값) //#0143


	if (fnLDCtrl_GetDeviceStatus() == DOWN)
	{
		fnLDCtrl_Initialize();
	}
	
	
	if (fnCMR_GetDeviceStatus() == DOWN)						// 장애시
	{
		fnCMR_ClearErrorCode();
		fnCMR_Initialize();										// 연결시작및초기화
	}
	if (fnUPS_GetDeviceStatus() == DOWN)
	{
		fnUPS_ClearErrorCode();
		fnUPS_Initialize();
	}
	if (fnDES_GetDeviceStatus() == DOWN)
	{
		fnDES_ClearErrorCode();
		fnDES_Initialize();
	}
	if (fnFNG_GetDeviceStatus() == DOWN)
	{
		fnFNG_ClearErrorCode();
		fnFNG_Initialize();
	}
	if (fnIRD_GetDeviceStatus() == DOWN)
	{
		fnIRD_ClearErrorCode();
		fnIRD_Initialize();
	}
	if (fnBAR_GetDeviceStatus() == DOWN)
	{
		fnBAR_ClearErrorCode();
		fnBAR_Initialize();
	}
	if (fnSCR_GetDeviceStatus() == DOWN)
	{
		fnSCR_ClearErrorCode();
		fnSCR_Initialize();
	}


	if ((nInitFlag) && (m_pDevCmn->AtmDefine.CashHandler == HBRM_SET) && (m_pProfile->TRANS.RebootFlag == FALSE))
	{
		fnAPL_CheckError();
		if ((memcmp(CurErrBuff.ErrorCode, "97925", 5) == 0) || (memcmp(CurErrBuff.ErrorCode, "97926", 5) == 0))
			fnAPL_ClearError();
	}
	
	if (nDevId & DEV_PIN)										// Pin장애시
	{
		fnPIN_ClearErrorCode();									// 장애코드초기화
		fnPIN_Initialize();										// 연결시작및초기화
	}

	//  * 이는 SCDP통합중 EP가 통합EP로 인하여 초기화시 SPR의 LeftMargin/MaxColumn등의 설정값을
	//  JPR이 OverWrite하기때문에 이를 대비하여 순서 변경함.
	if (nDevId & DEV_JPR)										// Jpr장애시
	{
		fnJPR_ClearErrorCode();									// 장애코드초기화
		fnJPR_Initialize();										// 연결시작및초기화
	}

	if (nDevId & DEV_SPR)										// Spr장애시
	{
		fnSPR_ClearErrorCode();									// 장애코드초기화
		fnSPR_Initialize();										// 연결시작및초기화
	}

	if (nDevId & DEV_MCU)										// Mcu장애시
	{
		fnMCU_ClearErrorCode();									// 장애코드초기화

		if ((!nInitFlag) &&										// 최초실행이아닌경우
			(RecoveryType == 0))								// 자동복구
		{
			fnMCU_Initialize();									// 연결시작및초기화
			if (fnAPL_CheckDeviceAction(DEV_MCU))				// 동작대기및장애검지
			{
				if (fnMCU_GetMaterialInfo())					// 매체잔류정보구하기
				{
					DeviceTimeout &= ~DEV_MCU;					// 시간초과정보초기화
					fnMCU_Eject();								// 방출
					if (fnAPL_CheckDeviceAction(DEV_MCU))		// 동작대기및장애검지
						fnMCU_WaitTaken();						// 수취대기
					fnAPL_CheckDeviceAction(DEV_MCU);			// 동작대기및장애검지
				}	
			}
		}

		fnMCU_Initialize();										// 연결시작및초기화
		if (fnAPL_CheckDeviceAction(DEV_MCU))					// 동작대기및장애검지
		{
			if (fnMCU_GetMaterialInfo() & ST_SENSOR2)			// 매체잔류정보구하기
				fnMCU_Retract();								// 회수
		}
		if (fnAPL_CheckDeviceAction(DEV_MCU))					// 동작대기및장애검지
		{
			if ((!(DeviceTimeout & DEV_MCU))	&&				// 시간초과정보무
				(nTempRecoveryDevice == DEV_MCU))				// Mcu장애만존재시
				fnAPL_ClearError();								// 장애정보초기화(Nms관련)
		}

		if (AtmDefine.MachineType == U8100)					// #NICCM01	ICCM 모듈 적용
		{
			if (DeviceStatus & DEV_MCU)
				;
			else
			{
				if (fnMCU_ICCMOpenConnection())					// ICCM 초기화(SUCCESS is ZERO)
				{
					m_pDevCmn->fnAPL_StackError("97933@@", "ICCM Open Fail", DEV_MCU);
					DeviceStatus |= DEV_MCU;
					Mcu97933Error = TRUE;
				}
				else
					Mcu97933Error = FALSE;
			}
		}

	}

	if ((nDevId & DEV_CSH))										// 현금부장애시
	{
		fnCSH_ClearErrorCode();									// 장애코드초기화

		switch (AtmDefine.CashHandler)
		{
			case OKI_SET:									
			case BRM_SET:
			case HBRM_SET:										// U8100-AP변경
				if (AtmDefine.CashHandler == BRM_SET)			
				{
					fnCSH_Initialize();
					ChangeBCFlag = FALSE;						
					Withdraw50TFlag = FALSE;				

					fnAPL_CheckDeviceAction(DEV_BRM);			
				}
				else
				{														
					if (nInitFlag)								// 최초실행시만 
					{
						Delay_Msg(3000);						
//						fnCSH_Initialize();						// 연결만시도
						BrmCshInfo = fnCSH_Initialize();		// 연결만시도 - //#0109
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[장치초기화] : fnCSH_Initialize (CSM In Flag %d)", BrmCshInfo);
						ChangeBCFlag = FALSE;				
						Withdraw50TFlag = FALSE;
					}
					else
					{
						if (AtmDefine.CashHandler == HBRM_SET)	// U8100 AP 변경내역 #04 - HBRM관련 변경내역
						{
							if (DeviceNoDevice & DEV_BRM)					
								fnBRM_Initialize();				// 연결만시도
							else											
								fnBRM_Reset(TRUE);						
						}
						else									// HBRM-end
							fnBRM_Reset(TRUE);					// 강제리셋 for OKI BRM
					}

					fnAPL_CheckDeviceAction(DEV_BRM);

					// U8100-97925 장애발생시 리부팅 처리 : 초기 Initialize시에만 한번 합니다
					if ((nInitFlag) && (m_pDevCmn->AtmDefine.CashHandler == HBRM_SET))
					{
						fnAPL_CheckError();

						m_pProfile->GetTransProfile();
						if ((memcmp(CurErrBuff.ErrorCode, "97925", 5) == 0) && (m_pProfile->TRANS.RebootFlag == FALSE))
						{
							m_pProfile->TRANS.RebootFlag = TRUE;
							m_pProfile->PutTransProfile();
							return FALSE;
						}
					}
					// U8100-97925

					// V06-01-03-#01 ChangeBCFlag 초기 BRM 통신장애 값을 못 가져 올 수 있으므로 매번 체크
					if (!ChangeBCFlag)							// BC부 확인
					{
						nRBType = FALSE;					
						if(fnBRM_GetNoteKindOfRB1() == BRM_NOTEKIND_50000)
							nRBType = TRUE;
						else
						if(fnBRM_GetNoteKindOfRB2() == BRM_NOTEKIND_50000)
							nRBType = TRUE;
						else
						if(fnBRM_GetNoteKindOfRB3() == BRM_NOTEKIND_50000)
							nRBType = TRUE;
						else
						if(fnBRM_GetNoteKindOfRB4() == BRM_NOTEKIND_50000)
							nRBType = TRUE;

						ChangeBCFlag = fnBRM_GetLargeMoneySupport();						
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__,	"fnAPL_InitializeDevice:ChangeBCFlag[%d]", ChangeBCFlag);

						if (ChangeBCFlag)
						{
							Withdraw50TFlag = nRBType;
							RegSetStr(_REGKEY_SUPERVISOR, "LargeBill", "2");				// U8100-OM적용
						}
						else
						{
							Withdraw50TFlag = FALSE;		
							RegSetStr(_REGKEY_SUPERVISOR, "LargeBill", "1");				// U8100-OM적용
						}


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__,	"fnAPL_InitializeDevice:Withdraw50TFlag[%d]", Withdraw50TFlag);
					}									
				}
				break;
			case CDU_SET:
				fnCSH_Initialize();								// 연결시작및초기화:회수정보처리함
				fnAPL_CheckDeviceAction(DEV_CSH);			
				fnAPL_CheckDeviceAction(DEV_CSH);
				Delay_Msg(2000);							// Sensor Wait(2000ms)
				fnAPL_CheckDeviceAction(DEV_CSH);

				if (fnCSH_GetDeviceStatus() != NODEVICE)	
				{
					if (fnCSH_GetMaterialInfo())
					{
						DeviceRetract |= DEV_CSH;
						fnAPL_PrintJnlInfo(GetSprintf("CASH RETRACT"), JNL_OPER);
						fnCSH_Retract();
						fnAPL_CheckDeviceAction(DEV_CSH);
						fnCSH_Initialize();				
					}
				}
				break;

			default :
				break;
		}
	}

	if (nDevId & DEV_UCM)
	{
		fnUCM_ClearErrorCode();									// 장애코드초기화

		if ((fnUCM_GetMaterialInfo(UCM_DEPOSIT)) ||				// 매체잔류정보구하기(입출금구분)
			(fnUCM_GetMaterialInfo(UCM_WITHDRAW)))
			DeviceRetract |= DEV_UCM;							// 회수정보설정

		fnUCM_Initialize();										// 연결시작및초기화
	}

	if (nDevId & DEV_PBM)										// Pbm장애시
	{
		fnPBM_ClearErrorCode();									// 장애코드초기화
		
		if ((!nInitFlag) &&										// 최초실행이아닌경우
			(RecoveryType == 0))								// 자동복구
		{
			fnPBM_Initialize();									// 연결시작및초기화
			if (fnAPL_CheckDeviceAction(DEV_PBM))				// 동작대기및장애검지
			{
				if (fnPBM_GetMaterialInfo())					// 매체잔류정보구하기
				{
					DeviceTimeout &= ~DEV_PBM;					// 시간초과정보초기화
					fnPBM_Eject();								// 방출
					if (fnAPL_CheckDeviceAction(DEV_PBM))		// 동작대기및장애검지
						fnPBM_WaitTaken();						// 수취대기
					fnAPL_CheckDeviceAction(DEV_PBM);			// 동작대기및장애검지
				}	
			}
		}

		fnPBM_Initialize();										// 연결시작및초기화
		if (fnAPL_CheckDeviceAction(DEV_PBM))					// 동작대기및장애검지
		{
//			if (fnPBM_GetMaterialInfo() & ST_SENSOR2)			// 매체잔류정보구하기
			if (AtmDefine.MachineType == U3100K)                // #0012
			{
				if (fnPBM_GetMaterialInfo())
				{
					DeviceTimeout &= ~DEV_PBM;					// 시간초과정보초기화
					fnPBM_Eject();								// 방출
					if (fnAPL_CheckDeviceAction(DEV_PBM))		// 동작대기및장애검지
						fnPBM_WaitTaken();						// 수취대기
					fnAPL_CheckDeviceAction(DEV_PBM);			// 동작대기및장애검지
				}

				if (fnPBM_GetMaterialInfo())
				{
					fnPBM_Retract();							// 회수
					m_pDevCmn->fnAPL_StackError("0110591", "통장미수취", DEV_PBM);
				}
			}
			else
			{
				if (fnPBM_GetMaterialInfo())						
					fnPBM_Retract();							// 회수
			}

		}

		// U8100-97926 장애발생시 리부팅 처리 : 초기 Initialize시에만 한번 합니다
		if ((nInitFlag) && (m_pDevCmn->AtmDefine.CashHandler == HBRM_SET))
		{
			fnAPL_CheckError();
			
			if ((memcmp(CurErrBuff.ErrorCode, "97926", 5) == 0) && (m_pProfile->TRANS.RebootFlag == FALSE))
			{
				m_pProfile->TRANS.RebootFlag = TRUE;
				m_pProfile->PutTransProfile();
				return FALSE;
			}
			else
			{
				m_pProfile->TRANS.RebootFlag = FALSE;
				m_pProfile->PutTransProfile();
			}
		}
	
		if (fnAPL_CheckDeviceAction(DEV_PBM))					// 동작대기및장애검지
		{
			if ((!(DeviceTimeout & DEV_PBM))	&&				// 시간초과정보무
				(nTempRecoveryDevice == DEV_PBM))				// Pbm장애만존재시
				fnAPL_ClearError();								// 장애정보초기화(Nms관련)
		}
	}

	if (nDevId & DEV_A4P)										// A4Printer장애시
	{
		fnA4P_ClearErrorCode();									// 장애코드초기화
		fnA4P_Initialize();										// 연결시작및초기화
		fnAPL_CheckDeviceAction(DEV_A4P);
	}
	fnAPL_CheckDeviceAction(nDevId);							// 동작대기및장애검지

	if (fnDOR_GetDeviceStatus() == DOWN)						
	{
		fnDOR_ClearErrorCode();
		fnDOR_Initialize();
	}
	if (fnLGT_GetDeviceStatus() == DOWN)
	{
		fnLGT_ClearErrorCode();
		fnLGT_Initialize();
	}
	if (fnSPL_GetDeviceStatus() == DOWN)
	{
		fnSPL_ClearErrorCode();
		fnSPL_Initialize();
	}

	DeviceNoDevice = DEV_NONE;									// 미연결 Device초기화
	if (fnDOR_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_DOR;
	if (fnLGT_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_LGT;
	if (fnSPL_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_SPL;
	if (fnCMR_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_CMR;
	if (fnUPS_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_UPS;
	if (fnDES_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_DES;
	if (fnFNG_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_FNG;
	if (fnIRD_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_IRD;
	if (fnBAR_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_BAR;
	if (fnSCR_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_SCR;

	if (fnPIN_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_PIN;


	//#0031 - #0110
	if (nInitFlag)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[장치초기화] - xx: nInitFlag = True");
		if (fnPIN_GetDeviceStatus() == NORMAL)
			DeviceStatus &= ~DEV_PIN;

		if (fnSPR_GetDeviceStatus() == NORMAL)
			DeviceStatus &= ~DEV_SPR;
	}
	
	if (fnSPR_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_SPR;
	if (fnJPR_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_JPR;
	if (fnMCU_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_MCU;
	if (fnCSH_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_CSH;
	if (fnUCM_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_UCM;
	if (fnPBM_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_PBM;

	if (fnA4P_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_A4P;

	if (fnLDCtrl_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_RDCTRL;


	
	// CST Information Procedure
	CString	strMinCntData("");									// 2004.11.05
	CString	strMaxCntData("");
	if (fnCSH_GetDeviceStatus() != NODEVICE)					// 2004.04.19
	{
		if (!CSTCnt)											// CST Information Setting
		{
			fnAPL_CheckDeviceAction(DEV_CSH);					// 2004.08.13
			CSTCnt		= fnCSH_GetNumberOfCST();
			if (CSTCnt >= 1)									// 2004.08.23
			{
				FstCSTValue	= fnCSH_GetValueOfCash(CDU_CST_1);
				FstCSTCnt = fnCSH_GetNumberOfCash(CDU_CST_1);	// First CST Count(ClerkInformation)
				if (FstCSTValue == 50000)						// V06-02-01-#01
					nCSTType = TRUE;
			}
			if (CSTCnt >= 2)									// 2004.08.23
			{
				SndCSTValue	= fnCSH_GetValueOfCash(CDU_CST_2);
				SndCSTCnt = fnCSH_GetNumberOfCash(CDU_CST_2);	// Second CST Count(ClerkInformation)
				if (SndCSTValue == 50000)						// V06-02-01-#01
					nCSTType = TRUE;
			}
			if (CSTCnt >= 3)									// 2004.08.23
			{
				TrdCSTValue	= fnCSH_GetValueOfCash(CDU_CST_3);
				TrdCSTCnt = fnCSH_GetNumberOfCash(CDU_CST_3);	// Third CST Count(ClerkInformation)
				if (TrdCSTValue == 50000)						// V06-02-01-#01
					nCSTType = TRUE;
			}
			if (CSTCnt >= 4)									// 2004.08.23
			{
				FthCSTValue	= fnCSH_GetValueOfCash(CDU_CST_4);
				FthCSTCnt = fnCSH_GetNumberOfCash(CDU_CST_4);	// Fourth CST Count(ClerkInformation)
				if (FthCSTValue == 50000)						// V06-02-01-#01
					nCSTType = TRUE;
			}

			fnAPL_CheckDeviceAction(DEV_CSH);					// 2004.11.05

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "FstCSTValue[%d]", FstCSTValue);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SndCSTValue[%d]", SndCSTValue);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TrdCSTValue[%d]", TrdCSTValue);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "FthCSTValue[%d]", FthCSTValue);
			
			strMinCntData = "0";								// 2004.11.05

			strMaxCntData = Int2Asc(CDU_FULLREJECTCNT);

			if (CSTCnt >= 1)									// 2004.11.05
			{
				strMinCntData += "," + Int2Asc(CDU_NEARCASHCNT);
				
				strMaxCntData += ",0";
			}
			if (CSTCnt >= 2)
			{
				strMinCntData += "," + Int2Asc(CDU_NEARCASHCNT);

				strMaxCntData += ",0";
			}
			if (CSTCnt >= 3)
			{
				strMinCntData += "," + Int2Asc(CDU_NEARCASHCNT);
				strMaxCntData += ",0";
			}
			if (CSTCnt >= 4)
			{
				strMinCntData += "," + Int2Asc(CDU_NEARCASHCNT);
				strMaxCntData += ",0";
			}
			
			fnCSH_SetMinMaxCST(strMinCntData, strMaxCntData);	// 2004.11.05
			fnAPL_CheckDeviceAction(DEV_CSH);
		}
	}
	if (!CSTCnt)												// CST No Information : 2004.04.19
		DeviceNoDevice |= DEV_CSH;

	if (DeviceNoDevice & DEV_SPR)
		DeviceDefine = DeviceDefine & (~DEV_SPR); 

	if (DeviceNoDevice & DEV_JPR)
		DeviceDefine = DeviceDefine & (~DEV_JPR); 

	if (DeviceNoDevice & DEV_UCM)
		DeviceDefine = DeviceDefine & (~DEV_UCM); 

	if (DeviceNoDevice & DEV_PBM)
		DeviceDefine = DeviceDefine & (~DEV_PBM); 

	if (DeviceNoDevice & DEV_A4P)
		DeviceDefine = DeviceDefine & (~DEV_A4P); 


	//PanPad Check //#0092
	if(AtmDefine.MachineType == U3100K)
	{

		if (DeviceNoDevice & DEV_PIN)
			fnAPL_StackError("9799904", "PINPAD 연결상태 확인", DEV_PIN);


	}
	else // Touch Screen
	if(AtmDefine.MachineType == U8100 )
	{

		if (DeviceNoDevice & DEV_PIN)
			DeviceDefine = DeviceDefine & (~DEV_PIN); 

	}

	// NoDevice 장애 발생 : 2003.12.15
	DeviceStatus |= (DeviceNoDevice & fnAPL_GetDefineDevice(DEV_MAIN));
	nDevId = fnAPL_GetAvailDevice(DeviceNoDevice & DEV_MAIN);	
	if (nDevId & DEV_SPR)
		fnAPL_StackError("9799200", "명세표부장애", DEV_SPR);
	if (nDevId & DEV_JPR)
		fnAPL_StackError("9799100", "저널부장애", DEV_JPR);
	if (nDevId & DEV_MCU)
		fnAPL_StackError("9799300", "카드부장애", DEV_MCU);
	if (nDevId & DEV_CSH)
		fnAPL_StackError("9799500", "현금입출금부장애", DEV_CSH);
	if (nDevId & DEV_UCM)
		fnAPL_StackError("9799400", "수표입출금부장애", DEV_UCM);
	if (nDevId & DEV_PBM)
		fnAPL_StackError("9799600", "통장부장애", DEV_PBM);
	if (nDevId & DEV_A4P)
		fnAPL_StackError("9799601", "A4프린터장애", DEV_A4P);
	if (nDevId & DEV_PIN)
		fnAPL_StackError("9799904", "PINPAD장애", DEV_PIN);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[장치초기화] : return");
	if (fnAPL_GetErrorDevice(nDevId))							// 장애난장치구하기
	{
		return FALSE;
	}
	else
		return TRUE;
}

// 동작대기및장애검지
int CDevCmn::fnAPL_CheckDeviceAction(int nDevId)
{
	int		nMaxDevRspTime = MIN_DEVRSP_TIME;					// 장치최대대기시간
	CString	strTempAction("");
	CString	strTempErr("");
	int		nTempTimeoutDevice = DEV_NONE;
	int		nTempErrorDevice = DEV_NONE;
	
	if (HBRM_SET == AtmDefine.CashHandler)						// U8100-USB대기시간 변경
		nMaxDevRspTime = MAX_DEVRSP_TIME;						// 장치최대대기시간

	nDevId = fnAPL_GetDefineDevice(nDevId);						// 정의된장치구하기

	if (AtmDefine.CashHandler == HBRM_SET)						// V07-01-05-#01
	{
		nMaxDevRspTime = MAX_DEVRSP_TIME;
		if (nDevId & DEV_UCM)
			nMaxDevRspTime = __min(nMaxDevRspTime, UcmRecvTime);
		if (nDevId & DEV_SPR)                                                                                                  
			nMaxDevRspTime = SPR_DEVRSP_TIME; //VAN 특이 사양임.(65초) //#N0157
//			nMaxDevRspTime = __max(nMaxDevRspTime, SprRecvTime);
		if (nDevId & DEV_JPR)
			nMaxDevRspTime = __max(nMaxDevRspTime, JprRecvTime);
		if (nDevId & DEV_MCU)
			nMaxDevRspTime = __max(nMaxDevRspTime, McuRecvTime);
		if (nDevId & DEV_CSH)
			nMaxDevRspTime = __max(nMaxDevRspTime, BrmRecvTime);
		if (nDevId & DEV_PBM)
			nMaxDevRspTime = __max(nMaxDevRspTime, PbmRecvTime);
	}
	else
	{
		if (nDevId & DEV_SPR)                                                                                                 
			nMaxDevRspTime = SPR_DEVRSP_TIME; //VAN 특이 사양임.(65초)  //#N0157
//			nMaxDevRspTime = __max(nMaxDevRspTime, SprRecvTime);
		if (nDevId & DEV_JPR)
			nMaxDevRspTime = __max(nMaxDevRspTime, JprRecvTime);
		if (nDevId & DEV_MCU)
			nMaxDevRspTime = __max(nMaxDevRspTime, McuRecvTime);
		if (nDevId & DEV_BRM)
			nMaxDevRspTime = __max(nMaxDevRspTime, BrmRecvTime);
		if (nDevId & DEV_UCM)
			nMaxDevRspTime = __max(nMaxDevRspTime, UcmRecvTime);
		if (nDevId & DEV_PBM)
			nMaxDevRspTime = __max(nMaxDevRspTime, PbmRecvTime);
	}

	strTempAction = fstrCMN_CheckDeviceAction(nDevId, nMaxDevRspTime);
																// 동작대기및장애검지(장치Id,체크시간)
	if (strTempAction == CHK_NORMAL)							// 정상처리
	{
		if (nDevId & DEV_SPR)
			DeviceStatus &= ~DEV_SPR;
		if (nDevId & DEV_JPR)
			DeviceStatus &= ~DEV_JPR;
		if ((nDevId & DEV_MCU)	&&								// #NICCM01	ICCM 모듈 적용
			(!Mcu97933Error)	&&
			(!Mcu97934Error)	)
			DeviceStatus &= ~DEV_MCU;
		if (nDevId & DEV_CSH)
			DeviceStatus &= ~DEV_CSH;
		if (nDevId & DEV_UCM)
			DeviceStatus &= ~DEV_UCM;
		if (nDevId & DEV_PBM)
			DeviceStatus &= ~DEV_PBM;
		if (nDevId & DEV_VFD)
			DeviceStatus &= ~DEV_VFD;
		if (nDevId & DEV_A4P)
			DeviceStatus &= ~DEV_A4P;

	}
	else
	if (strTempAction == CHK_TIMEOUT)							// 장치시간초과
	{
		nTempTimeoutDevice = fnCMN_GetTimeoutDevice(nDevId);	// 시간초과장치구하기(장치Id)
		DeviceTimeout |= nTempTimeoutDevice;					// 시간초과정보 Device set
	}
	else														// 장치무응답
	{															// 처리중 장애발생
		nTempErrorDevice = fnCMN_GetErrorDevice(nDevId);		// 장애장치구하기(장치Id)
		switch (AtmDefine.CashHandler)
		{
			case OKI_SET:										// T2ATM 2006.5.24 yscho
			case BRM_SET:
			case HBRM_SET:										// U8100-AP 변경
				DeviceStatus |= (nTempErrorDevice & DEV_MAIN);	// 2003.12.15
				break;
				
			case CDU_SET:
				DeviceStatus |= (nTempErrorDevice & DEV_MAIN);	// 2005.01.20
				break;											
		}
		if (nTempErrorDevice & DEV_SPR)							// 2003.12.15
			fnAPL_StackError(fstrSPR_GetErrorCode(), "명세표부장애", DEV_SPR);
		if (nTempErrorDevice & DEV_JPR)
			fnAPL_StackError(fstrJPR_GetErrorCode(), "저널부장애", DEV_JPR);
		if (nTempErrorDevice & DEV_MCU)
			fnAPL_StackError(fstrMCU_GetErrorCode(), "카드부장애", DEV_MCU);
		if (nTempErrorDevice & DEV_CSH)
			fnAPL_StackError(fstrCSH_GetErrorCode(), "현금입출금부장애", DEV_CSH);
		if (nTempErrorDevice & DEV_UCM)
			fnAPL_StackError(fstrUCM_GetErrorCode(), "수표입출금부장애", DEV_UCM);
		if (nTempErrorDevice & DEV_PBM)
			fnAPL_StackError(fstrPBM_GetErrorCode(), "통장부장애", DEV_PBM);
		if (nTempErrorDevice & DEV_A4P)
			fnAPL_StackError(fstrA4P_GetErrorCode(), "A4프린터장애", DEV_A4P);
	}
	if (strTempAction != CHK_NORMAL)							// 2003.11.21 : 장애검지시 트레이스백업
	{
		fnCMN_ProcBackupTrace(GetSprintf("%s\\장애%s%s.blg", _TRACE_DIR, 
															  GetDate().GetBuffer(0),
															  GetTime().GetBuffer(0)));
																// Kal Trace Backup
	}

	if (fnAPL_GetErrorDevice(nDevId))							// 장애난장치구하기
		return FALSE;
	else
		return TRUE;
}

// 매체잔류검지
int CDevCmn::fnAPL_CheckMaterial(int nDevId)
{
	int		nTempReturn = TRUE;
	CStringArray strTempRetractArray;			
	nDevId = fnAPL_GetAvailDevice(nDevId);						// 사용가능한장치구하기
	nDevId &= ~DEV_MCU;											// 카드부 잔류장애 미검지
	nDevId &= ~DEV_PBM;											// 통장부 잔류장애 미검지

	if (nDevId & DEV_MCU)
	{
		if (fnMCU_GetMaterialInfo() & ST_SENSOR2)
		{
			fnAPL_StackError("8217091", "카드부카드잔류", DEV_MCU);
			DeviceStatus |= DEV_MCU;
			nTempReturn = FALSE;
		}
	}

	// V02-06-02-#06 82160 장애감소대책 - 잔류장애검지시 초기화처리
	//				 만약 정말 현금이 있었던 경우라면 현금은 초기화에서 회수된다.
	//				 그러나, 회수매수는 저널에 인자하지 않는다.(김명수과장과 이훈팀장 합의사항, 2005.09.07 재확인)
	if (nDevId & DEV_CSH)
	{
		if (DeviceLoss & DEV_CSH)								// 미수취장애
		{
			if (fnCSH_GetMaterialInfo()) 
			{
				if (fnAPL_CheckError())							// 다른 장애가 없을 경우에만	V02-07-01-#08 품질개선
					fnAPL_StackError("0110991", "현금입출금부잔류", DEV_CSH);

				DeviceStatus |= DEV_CSH;
				nTempReturn = FALSE;
			}
		}
		else
		if (((AtmStatus == ATM_TRAN) || (AtmStatus == ATM_CUSTOM)) &&
			(fnAPL_CheckError()))								// 다른 장애가 없을 경우에만	V02-07-01-#08 품질개선
		{
			if (fnCSH_GetMaterialInfo()) 
			{
				if (fnAPL_InitializeDevice(DEV_CSH))			// 초기화정상처리 : 아무처리없음
				{
MsgDump(MAC_TRACE_CODE_FILE("Log"), "[잔류장애검지] 현금부 - 장치초기화후 정상처리");
					//#0013
					if (fnAPL_GetYesDevice(DEV_CSH))						
					{
						SplitString(m_pDevCmn->fstrCSH_GetRetractResult(), ",", strTempRetractArray);
						if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler) 
						{
							// 회수결과구하기
							if ((Asc2Int(strTempRetractArray[0]) == 0)	&&	// 회수금액
								(Asc2Int(strTempRetractArray[1]) == 0)	&&	// 만원회수매수
								(Asc2Int(strTempRetractArray[2]) == 0)	&&	// 오천원회수매수
								(Asc2Int(strTempRetractArray[3]) == 0)	&&	// 천원회수매수
								(Asc2Int(strTempRetractArray[4]) == 0)	&&	// 오만원회수매수
								(Asc2Int(strTempRetractArray[5]) == 0)	&&	// 수표회수매수
								(Asc2Int(strTempRetractArray[6]) == 0));	// 불명권회수매수
							else 
							{
								DeviceRetract |= DEV_CSH;					// 회수결과를 처리함

								if ((!ChangeBCFlag) &&						// 미개체기기 회수금액처리
									(Asc2Int(strTempRetractArray[4]) != 0))
								{
									m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_RESET,	
													Asc2Int(strTempRetractArray[0])		// 회수금액
												-	Asc2Int(strTempRetractArray[4])*50000,
													Asc2Int(strTempRetractArray[1]),	// 만원회수매수 
													Asc2Int(strTempRetractArray[2]),	// 오천원회수매수 
													Asc2Int(strTempRetractArray[3]),	// 천원회수매수 
													Asc2Int(strTempRetractArray[5])		// 수표회수매수
												+	Asc2Int(strTempRetractArray[6])		// 불명권회수매수
												+	Asc2Int(strTempRetractArray[4]));	// 오만원회수매수
								}
								else
								{
									m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_RESET,	
													Asc2Int(strTempRetractArray[0]),	// 회수금액
													Asc2Int(strTempRetractArray[1]),	// 만원회수매수 
													Asc2Int(strTempRetractArray[2]),	// 오천원회수매수 
													Asc2Int(strTempRetractArray[3]),	// 천원회수매수 
													Asc2Int(strTempRetractArray[5])		// 수표회수매수
												+	Asc2Int(strTempRetractArray[6]),	// 불명권회수매수
													Asc2Int(strTempRetractArray[4]));	// 오만원회수매수
								}
							}
						}												
					}

				}
				else											// 초기화장애처리 : return FALSE
				{
					nTempReturn = FALSE;
MsgDump(MAC_TRACE_CODE_FILE("Log"), "[잔류장애검지] 현금부 - 장치초기화후 NG처리");
				}
			}
		}
		else
		{
			if (fnCSH_GetMaterialInfo()) 
			{
				fnAPL_StackError("8216091", "현금입출금부잔류", DEV_CSH);
				DeviceStatus |= DEV_CSH;
				nTempReturn = FALSE;
			}
		}
	}

	if (nDevId & DEV_UCM)
	{
		if (DeviceLoss & DEV_UCM)								// 미수취장애
		{
			if (fnUCM_GetMaterialInfo(UCM_DEPOSIT) ||
				fnUCM_GetMaterialInfo(UCM_WITHDRAW))			// 821@0장애  
			{
				if (fnAPL_CheckError())							// 다른 장애가 없을 경우에만	V02-07-01-#08 품질개선
					fnAPL_StackError("0110A91", "수표입출금부잔류", DEV_UCM);

				DeviceStatus |= DEV_UCM;
				nTempReturn = FALSE;
			}
		}
		else
		if (((AtmStatus == ATM_TRAN) || (AtmStatus == ATM_CUSTOM)) &&
			(fnAPL_CheckError()))								// 다른 장애가 없을 경우에만
		{

			if (fnUCM_GetMaterialInfo(UCM_DEPOSIT) ||
				fnUCM_GetMaterialInfo(UCM_WITHDRAW))			// 821@0장애  
			{
				if (fnAPL_InitializeDevice(DEV_UCM))
					;
				else
					nTempReturn = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "▷ 821@0장애 복구 - UCM Initialize");
			}
		}
		else
		{
			if (fnUCM_GetMaterialInfo(UCM_DEPOSIT)) 
			{
				fnAPL_StackError("821@091", "수표입금부잔류", DEV_UCM);
				DeviceStatus |= DEV_UCM;
				nTempReturn = FALSE;
			}

			if (fnUCM_GetMaterialInfo(UCM_WITHDRAW)) 
			{
				fnAPL_StackError("821@092", "수표출금부잔류", DEV_UCM);
				DeviceStatus |= DEV_UCM;
				nTempReturn = FALSE;
			}
		}
	}

	if (nDevId & DEV_PBM)
	{
		if (fnPBM_GetMaterialInfo() & ST_SENSOR2)
		{
			fnAPL_StackError("8218091", "통장부잔류", DEV_PBM);
			DeviceStatus |= DEV_PBM;
			nTempReturn = FALSE;
		}
	}

//	if (nDevId & DEV_A4P)
//	{
//		if (fnA4P_GetMaterialInfo())
//		{
//			fnAPL_StackError("8218091", "A4P프린터잔류", DEV_A4P);
//			DeviceStatus |= DEV_A4P;
//			nTempReturn = FALSE;
//		}
//	}
	return nTempReturn;
}

// 분실매체회수
int	CDevCmn::fnAPL_RetractMaterial(int nDevId)
{
	int		SprRetractCnt = 0;									// 2003.12.20
	int		McuRetractCnt = 0;
	int		PbmRetractCnt = 0;
	int		A4pRetractCnt = 0;
	CStringArray strTempRetractArray;
	
	if (fnAPL_GetYesDevice(DEV_SPR))							// 2003.12.20
		SprRetractCnt = Asc2Int(fstrSPR_GetRetractCnt());		// 회수매수구하기
	if (fnAPL_GetYesDevice(DEV_MCU))
		McuRetractCnt = Asc2Int(fstrMCU_GetRetractCnt());
	if (fnAPL_GetYesDevice(DEV_PBM))
		PbmRetractCnt = Asc2Int(fstrPBM_GetRetractCnt());
	if (fnAPL_GetYesDevice(DEV_A4P))
		A4pRetractCnt = Asc2Int(fstrA4P_GetRetractCnt());
	
	nDevId = fnAPL_GetAvailDevice(nDevId);						// 사용가능한장치구하기
	
	if (nDevId & DEV_SPR)
	{

		if(AtmDefine.MachineType == U3100K)
		{
			if (fnSPR_GetMaterialInfo())							// 매체잔류정보구하기
				fnSPR_Eject(K_45_WAIT);                             // 방출
		}
		else
		{
			if (fnSPR_GetMaterialInfo())							// 매체잔류정보구하기
				fnSPR_Retract();									// 회수
		}

	}

	
	if (nDevId & DEV_MCU)
	{
		if (fnMCU_GetMaterialInfo())							// 매체잔류정보구하기
			fnMCU_Retract();									// 회수
	}
	
	if (nDevId & DEV_PBM)
	{
		//#0012
		if (AtmDefine.MachineType == U3100K)
		{
			if (fnPBM_GetMaterialInfo())
			{
				DeviceTimeout &= ~DEV_PBM;					// 시간초과정보초기화
				fnPBM_Eject();								// 방출
				if (fnAPL_CheckDeviceAction(DEV_PBM))		// 동작대기및장애검지
					fnPBM_WaitTaken();						// 수취대기
				fnAPL_CheckDeviceAction(DEV_PBM);			// 동작대기및장애검지
			}

			if (fnPBM_GetMaterialInfo())
			{
				fnPBM_Retract();							// 회수
				m_pDevCmn->fnAPL_StackError("0110591", "통장미수취", DEV_PBM);
			}

		}
		else
		{
			if (fnPBM_GetMaterialInfo())						
				fnPBM_Retract();							// 회수
		}

	}
	
	if (nDevId & DEV_A4P)
	{
		if (fnA4P_GetMaterialInfo())							// 매체잔류정보구하기
			fnA4P_Retract();									// 회수
	}

	if (nDevId & DEV_CSH)
	{
		if (fnCSH_GetMaterialInfo())							// 매체잔류정보구하기
		{
			if ((AtmDefine.CashHandler == OKI_SET) || (AtmDefine.CashHandler == HBRM_SET))	// U8100 AP 변경내역 #04 - HBRM관련 변경내역
				fnBRM_Reset(TRUE);
			else
			if (AtmDefine.CashHandler == BRM_SET)
				fnBRM_Initialize();								// 전체회수기능포함
			else
			if (AtmDefine.CashHandler == CDU_SET)
			{
				if (fnCSH_GetMaterialInfo())
					fnCSH_Retract();							// 전체회수기능포함
			}

		}
	}
	
	if (nDevId & DEV_UCM)
	{
		if ((fnUCM_GetMaterialInfo(UCM_DEPOSIT)) ||				// 매체잔류정보구하기(입출금구분)
			(fnUCM_GetMaterialInfo(UCM_WITHDRAW)))
		{
			DeviceRetract |= DEV_UCM;							// 회수정보설정
			fnUCM_Initialize();									// 전체회수기능포함
		}
	}
	
	fnAPL_CheckDeviceAction(nDevId);							// 동작대기및장애검지

	if (nDevId & DEV_CSH)										// Brm장치초기화
	{
		fnAPL_CheckDeviceAction(DEV_CSH);						// 동작대기및장애검지
		Delay_Msg(2000);										// 검지대기(2000ms)
																// 회수내역대기
		fnAPL_CheckDeviceAction(DEV_CSH);						// 동작대기및장애검지
	}

	if (nDevId & DEV_MCU)
	{
		if (fnAPL_GetYesDevice(DEV_MCU))						// 2003.12.20
		{
			if (McuRetractCnt < Asc2Int(fstrMCU_GetRetractCnt()))
			{
				DeviceRetract |= DEV_MCU;
				fnAPL_PrintJnlInfo("카드회수", JNL_STATUS);	// 2004.09.07 문구변경
			}
		}
	}
	
	if (nDevId & DEV_PBM)
	{
		if (fnAPL_GetYesDevice(DEV_PBM))						// 2003.12.20
		{
			if (PbmRetractCnt < Asc2Int(fstrPBM_GetRetractCnt()))
			{
				DeviceRetract |= DEV_PBM;
				fnAPL_PrintJnlInfo("통장회수", JNL_STATUS);	// 2004.09.07 문구변경
			}
		}
	}
	
	if (nDevId & DEV_A4P)
	{
		if (fnAPL_GetYesDevice(DEV_A4P))						// 20041018
		{
			if (A4pRetractCnt < Asc2Int(fstrA4P_GetRetractCnt()))
			{
				DeviceRetract |= DEV_A4P;    
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("분실 거래명세서회수"), JNL_STATUS);
			}
		}
	}

	if (nDevId & DEV_CSH)
	{
		if (fnAPL_GetYesDevice(DEV_CSH))					
		{
			SplitString(fstrCSH_GetRetractResult(), ",", strTempRetractArray);

			// V07-00-00-#01 오만원권 지원 ──────────────────
			if ((OKI_SET == m_pDevCmn->AtmDefine.CashHandler) || (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler))	// U8100 AP 변경내역 #04 - HBRM관련 변경내역
			{
				// 회수결과구하기
				if ((Asc2Int(strTempRetractArray[0]) == 0)	&&	// 회수금액
					(Asc2Int(strTempRetractArray[1]) == 0)	&&	// 만원회수매수
					(Asc2Int(strTempRetractArray[2]) == 0)	&&	// 오천원회수매수
					(Asc2Int(strTempRetractArray[3]) == 0)	&&	// 천원회수매수
					(Asc2Int(strTempRetractArray[4]) == 0)	&&	// 오만원회수매수
					(Asc2Int(strTempRetractArray[5]) == 0)	&&	// 수표회수매수
					(Asc2Int(strTempRetractArray[6]) == 0));	// 불명권회수매수
				else 
				{
					DeviceRetract |= DEV_CSH;					// 회수결과를 처리함
					fnAPL_PrintCashMoveInfo(BRM_LOST,	
							Asc2Int(strTempRetractArray[0]),	// 회수금액
							Asc2Int(strTempRetractArray[1]),	// 만원회수매수 
							Asc2Int(strTempRetractArray[2]),	// 오천원회수매수 
							Asc2Int(strTempRetractArray[3]),	// 천원회수매수  
							Asc2Int(strTempRetractArray[5])		// 수표회수매수
						+	Asc2Int(strTempRetractArray[6]),	// 불명권회수매수
							Asc2Int(strTempRetractArray[4]));	// 오만원회수매수
				}
			}
			else
			{
			// ────────────────────────────────
				// 회수결과구하기
				if ((Asc2Int(strTempRetractArray[0]) == 0)	&&	// 회수금액
					(Asc2Int(strTempRetractArray[1]) == 0)	&&	// 만원회수매수
					(Asc2Int(strTempRetractArray[2]) == 0)	&&	// 오천원회수매수
					(Asc2Int(strTempRetractArray[3]) == 0)	&&	// 천원회수매수
					(Asc2Int(strTempRetractArray[4]) == 0)	&&	// 수표회수매수
					(Asc2Int(strTempRetractArray[5]) == 0));	// 불명권회수매수
				else 
				{
					DeviceRetract |= DEV_CSH;					// 회수결과를 처리함
					fnAPL_PrintCashMoveInfo(BRM_LOST,	
							Asc2Int(strTempRetractArray[0]),	// 회수금액
							Asc2Int(strTempRetractArray[1]),	// 만원회수매수 
							Asc2Int(strTempRetractArray[2]),	// 오천원회수매수 
							Asc2Int(strTempRetractArray[3]),	// 천원회수매수 
							Asc2Int(strTempRetractArray[4])		// 수표회수매수
						+	Asc2Int(strTempRetractArray[5]));	// 불명권회수매수
				}
			}
		}
	}
	
	if (nDevId & DEV_UCM)
	{
		if (fnAPL_GetYesDevice(DEV_UCM))						// 2003.12.20
		{
			if (DeviceRetract & DEV_UCM)
			{
				fnAPL_PrintJnlInfo("수표회수", JNL_STATUS);	// 2004.09.07 문구변경
			}
		}
	}

	return TRUE;
}

// 장애난장치자동축퇴													
int CDevCmn::fnAPL_AutoOffErrorDevice()
{

	if (fnAPL_GetAvailErrorDevice(DeviceStatus) == DEV_BRM)    
	{
		fnAPL_ClearError();
		DeviceAutoOff |= DEV_BRM;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AutoOffErrorDevice (%d)", DeviceAutoOff);

	}

	// 자동축퇴처리(DEV_BRM : DEV_PBM) //#0084
	if (fnAPL_GetAvailErrorDevice(DeviceStatus) == (DEV_BRM | DEV_PBM))  
	{
		fnAPL_ClearError();										// 장애정보초기화
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AutoOffErrorDevice0 (%x)", fnAPL_GetAvailErrorDevice(DeviceStatus));
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_BRM)
			DeviceAutoOff |= DEV_BRM;
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_PBM)
			DeviceAutoOff |= DEV_PBM;
	}
	
	// 자동축퇴처리(PBM만처리 : BRM,UCM은 시재관련하여 불가정책)
	if (fnAPL_GetAvailErrorDevice(DeviceStatus) == (DEV_PBM | DEV_A4P)) 
	{
		fnAPL_ClearError();										// 장애정보초기화
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AutoOffErrorDevice1 (%x)", fnAPL_GetAvailErrorDevice(DeviceStatus));
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_PBM)
			DeviceAutoOff |= DEV_PBM;
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_A4P)
			DeviceAutoOff |= DEV_A4P;
	}
	////#0032
	if (fnAPL_GetAvailErrorDevice(DeviceStatus) == (DEV_PBM | DEV_SPR))
	{
		fnAPL_ClearError();										// 장애정보초기화
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AutoOffErrorDevice2 (%x)", fnAPL_GetAvailErrorDevice(DeviceStatus));
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_PBM)
			DeviceAutoOff |= DEV_PBM;
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_SPR)
			DeviceAutoOff |= DEV_SPR;

	}

	if (fnAPL_GetAvailErrorDevice(DeviceStatus) == DEV_A4P)
	{
		fnAPL_ClearError();										// 장애정보초기화
		DeviceAutoOff |= DEV_A4P;
	}
    //#0032
	if (fnAPL_GetAvailErrorDevice(DeviceStatus) == DEV_PBM)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AutoOffErrorDevice3 (%x)", fnAPL_GetAvailErrorDevice(DeviceStatus));
		fnAPL_ClearError();										// 장애정보초기화
		DeviceAutoOff |= DEV_PBM;
	}
	
	if (fnAPL_GetAvailErrorDevice(DeviceStatus) == DEV_SPR)     // SPR 무시 Van-CD Spec 0105
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AutoOffErrorDevice4 (%x)", fnAPL_GetAvailErrorDevice(DeviceStatus));

		fnAPL_ClearError();
		DeviceAutoOff |= DEV_SPR;
	}


	return TRUE;
}

// 정상Segment출력
int CDevCmn::fnAPL_DisplayNormalSegment()
{
	CString	strTempNormalSegment("");
	int nTotalAmount = 0;										// V07-00-00-#01 오만원권 지원

	switch (m_pProfile->DEVICE.MachineType)						// TTW 대응
	{
		case U8100:												// U8100-AP변경
			// V07-00-00-#01 오만원권 지원 ──────────────────
			if (Withdraw50TFlag)
				nTotalAmount = (fnAPL_GetNumberOfCash() + (fnAPL_GetNumberOf50000Cash() * CASH_FIFTY_THOUSAND)) % 10000;
			else
				nTotalAmount = fnAPL_GetNumberOfCash();

			strTempNormalSegment.Format("%4.4d%4.4d", nTotalAmount,	fnAPL_GetNumberOfOutCheck());
			// ────────────────────────────────
			break;
		case U3100K:
			// 정상 Mode에서는 CD기의 경우에는 Segment에 진행카운트를 Display
			// fnAPL_DisplayErrorSegment(m_pProfile->TRANS.ProcCount[0], "0000000");
			strTempNormalSegment.Format("  %1.1c00000", m_pProfile->TRANS.ProcCount[0]);
			break;
	}

	fnAPL_SetSegment(strTempNormalSegment);						// Segment설정(값)
	return TRUE;
}

// 장애Segment출력
int CDevCmn::fnAPL_DisplayErrorSegment(char chProcCount, LPCTSTR szErrorCode)
{
	CString	strTempErrorSegment("");

/////////////////////////////////////////////////////////////////////////////
	// Om정보전달
	Om.ProcCount.Format("%c", chProcCount);
	Om.ErrorCode.Format("%7.7s", szErrorCode);
	fnAPL_ClerkInformation();									// 계원정보
/////////////////////////////////////////////////////////////////////////////

	// 장애 Mode에서는 Segment에 진행번호,장애코드 Display
	strTempErrorSegment.Format("%2.2s%1.1c%5.5s", fnAPL_GetLC(szErrorCode, TranProc), chProcCount, szErrorCode);

	fnAPL_SetSegment(strTempErrorSegment);						// Segment설정(값)
	return TRUE;
}

// 운영Segment출력													
int CDevCmn::fnAPL_DisplayProcCountSegment()
{
	if (!fnAPL_CheckError())									// 장애검지
		fnAPL_DisplayErrorSegment(m_pProfile->TRANS.ProcCount[0], CurErrBuff.ErrorCode);
	else
	{
		if ((m_pProfile->TRANS.ProcCount[0] == ' ') ||			// 계원조작
			(m_pProfile->TRANS.ProcCount[0] == '0'))			// 고객대기중
		{

/////////////////////////////////////////////////////////////////////////////
			// Om정보전달
			Om.ProcCount.Format("%c", m_pProfile->TRANS.ProcCount[0]);
			Om.ErrorCode.Format("%7.7s", "0000000");
			fnAPL_ClerkInformation();							// 계원정보
/////////////////////////////////////////////////////////////////////////////

			fnAPL_DisplayNormalSegment();						// 정상Segment출력
		}
		else
		{
			switch (m_pProfile->DEVICE.MachineType)				// TTW 대응
			{
				case U8100:										// U8100-AP 변경
					fnAPL_DisplayErrorSegment(m_pProfile->TRANS.ProcCount[0], "0000000");
					break;
			}
		}
	}

	return TRUE;
}

// 정보Led출력
// V03-01-01 SPL 표시분리처리(기종별)
int CDevCmn::fnAPL_DisplayInformationLed(int nInitFlag)
{
	static	CString	strSaveLed("000000000000");
	CString	strTempLed("000000000000");

	if (HBRM_SET == AtmDefine.CashHandler)						
		return TRUE;											


	switch (m_pProfile->DEVICE.MachineType)
	{
		case U8100:												// U8100-AP 변경
		case U3100K:
//			┌────────┬────────────────────┐
//			│	  동작 상태   │               경고 / 이상부위          │
//			├────────┼────────────────────┤
//			│  취 급 중[0]   │    카    드[4]         명 세 표[8]     │
//			│  경    고[1]   │    통    장[5]         저    널[9]     │
//			│  이    상[2]   │    지    폐[6]         입금수표[10]    │
//			│  DUMMY   [3]   │    출금수표[7]         회 수 함[11]    │
//			└────────┴────────────────────┘
			if ((AtmStatus == ATM_CUSTOM) ||					// 고객대기중	모드 
				(AtmStatus == ATM_TRAN))						// 고객거래중	모드
			{
				strTempLed.SetAt(0, '1');						// 취급중
			}

			if (fnAPL_GetErrorDevice(DEV_MCU))					// 장애난장치구하기
			{
				strTempLed.SetAt(4, '1');						// 카드
				strTempLed.SetAt(2, '1');						// 이상
			}

			if (!(TranAvail & TRAN_PBM_OK))						// 통장 불가능
			{
				strTempLed.SetAt(5, '1');						// 통장
				strTempLed.SetAt(1, '1');						// 경고
			}
			
			if ((!(TranAvail & (TRAN_WITH_CASH | TRAN_WITH_50TCASH))) ||	
				(!(TranAvail & TRAN_DEP_CASH)))					// 현금 입금 불가능
			{
				strTempLed.SetAt(6, '1');						// 지폐
				strTempLed.SetAt(1, '1');						// 경고
			}
			
			if (!(TranAvail & TRAN_WITH_CHECK))					// 수표 출금 불가능
			{
				strTempLed.SetAt(7, '1');						// 출금수표
				strTempLed.SetAt(1, '1');						// 경고
			}
			
			if ((fnAPL_GetErrorDevice(DEV_SPR))		||			// 장애난장치구하기
				(SlipHeadStatus != HEADER_NORMAL)	||			// 명세표부 COVER 열림
				(SlipStatus == SLIP_EMPTY_PAPER))				// 명세표 용지 요보충
			{
				strTempLed.SetAt(8, '1');						// 명세표
				strTempLed.SetAt(2, '1');						// 이상
			}
			else
			if (SlipStatus == SLIP_LOW_END)						// 명세표 용지 잔량소
			{
				strTempLed.SetAt(8, '1');						// 명세표
				strTempLed.SetAt(1, '1');						// 경고
			}

			if ((fnAPL_GetErrorDevice(DEV_JPR))		||			// 장애난장치구하기
				(JnlHeadStatus != HEADER_NORMAL)	||			// 저널부 COVER 열림
				(JnlStatus == JNL_EMPTY_PAPER))					// 저널 용지 요보충
			{
				strTempLed.SetAt(9, '1');						// 저널
				strTempLed.SetAt(2, '1');						// 이상
			}
			else
			if (JnlStatus == JNL_LOW_END)						// 저널 용지 잔량소
			{
				strTempLed.SetAt(9, '1');						// 저널
				strTempLed.SetAt(1, '1');						// 경고
			}

			if (!(TranAvail & TRAN_DEP_CHECK))					// 수표 입금 불가능
			{
				strTempLed.SetAt(10, '1');						// 입금수표
				strTempLed.SetAt(1, '1');						// 경고
			}
			
			if ((RejectCSTLoc != NORMAL_POS)		||			// 정상위치 아님
				(RejectCSTStatus == CST_FULL))					// FULL
			{
				strTempLed.SetAt(11, '1');						// 회수함
				strTempLed.SetAt(2, '1');						// 이상
			}
			else
			if (RejectCSTStatus == CST_NEAR)					// NEAR FULL
			{
				strTempLed.SetAt(11, '1');						// 회수함
				strTempLed.SetAt(1, '1');						// 경고
			}

			break;
	}

	if ((nInitFlag) ||											// 초기설정요구
		(strTempLed != strSaveLed))								// 변경시
	{
		strSaveLed = strTempLed;
		fnSPL_SetLed(strTempLed);								// Led설정(값)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_DisplayInformationLed[ATMStatus:%d], [Save:%s], [Temp:%s]", AtmStatus, strSaveLed, strTempLed);
	}

	return TRUE;
}

int CDevCmn::fnAPL_DisplayInformationLedOnOff(int nLedIndex, int bOnOff)
{

	return TRUE;
}

// 정보Indicator출력
int CDevCmn::fnAPL_DisplayInformationIndicator(int nInitFlag)
{
	static	int	nSaveIndicator = 0;
	int		nTempIndicator = 0;
	int		nTempIndicatorTranKind = 0;							// U8100-AP변경


	if ( U8100 == m_pProfile->DEVICE.MachineType || U3100K == m_pProfile->DEVICE.MachineType)
	{
		if ((AtmStatus == ATM_CUSTOM) ||						// 고객대기중	모드 
			(AtmStatus == ATM_TRAN))							// 고객거래중	모드
		{
			nTempIndicator = 0x01;								// 거래가능	켬
			
			if (UserTranAvail & (TRAN_WITH_CASH | TRAN_WITH_50TCASH))
				nTempIndicatorTranKind |= 0x01;
			if (UserTranAvail & (TRAN_DEP_CASH))
				nTempIndicatorTranKind |= 0x10;

			if (UserTranAvail & TRAN_DEP_CASH)
				nTempIndicator |= 0x04;
			if (UserTranAvail & (TRAN_WITH_CASH | TRAN_WITH_50TCASH))	
				nTempIndicator |= 0x08;

			if (UserTranAvail & (TRAN_WITH_CHECK))
				nTempIndicatorTranKind |= 0x02;
			if (UserTranAvail & (TRAN_DEP_CHECK))
				nTempIndicatorTranKind |= 0x20;

			if (UserTranAvail & TRAN_DEP_CHECK)
				nTempIndicator |= 0x10;
			if (UserTranAvail & TRAN_WITH_CHECK)
				nTempIndicator |= 0x20;
			nTempIndicator |= 0x40;								// 이체
			if (UserTranAvail & TRAN_PBM_OK)
				nTempIndicator |= 0x80;							// 통장
		}
		else
		{
			nTempIndicator = 0x02;								// 준비중(거래불가)	켬
		}

		if ((nInitFlag) ||										// 초기설정요구
			(nTempIndicator != nSaveIndicator))					// 변경시
		{
				nSaveIndicator = nTempIndicator;
			if (m_pDevCmn->AtmDefine.CashHandler == HBRM_SET)		
				;
			else
				fnLGT_SetIndicator(nTempIndicator);				// U8100 AP 변경내역 #08 - 사용가능램프 변경

			//────────────────────────────────────
			// CFMonitor 지원 2006.09.08(금) T2ATM LCD지원
			//────────────────────────────────────
			HWND hWnd;
			hWnd = _FIND_CFMON;
			if (OKI_SET == AtmDefine.CashHandler)					// T1-T2통합
			{
				if(hWnd) ::PostMessage(hWnd, WUM_INDICATOR, nTempIndicator, 0);	
			}
			else 
			if (AtmDefine.CashHandler == HBRM_SET)			// U8100-AP변경
			{
				if( U8100 == m_pProfile->DEVICE.MachineType)
				{
					if(hWnd) ::PostMessage(hWnd, WUM_INDICATOR, nTempIndicator, nTempIndicatorTranKind);	
				}
				else
				if( U3100K == m_pProfile->DEVICE.MachineType)
				{
					if(hWnd) ::PostMessage(hWnd, WUM_INDICATOR, nTempIndicator, 0);	
				}

			}
			//────────────────────────────────────
		}
	}

	return TRUE;
}

// 정보Rpl전송
int CDevCmn::fnAPL_SendInformationRpl(int nInitFlag)
{
	static	CString	strSaveRpl("000000");
	CString	strTempRpl("000000");

//		┌────────┬────────────────────┐
//		│	  INDEX       │               CONTENTS                 │
//		├────────┼────────────────────┤
//		│       [0]      │    NORMAL  - 취급중                    │
//		│       [1]      │    CASH    - 현금                      │ 
//		│       [2]      │    LOSS    - 분실                      │
//		│       [3]      │    ALARM   - 알람                      │
//		│       [4]      │    RECEIPT - 명세표                    │
//		│       [5]      │    CALL    - 호출                      │
//		└────────┴────────────────────┘
	if ((AtmStatus == ATM_CUSTOM) ||							// 고객대기중	모드 
		(AtmStatus == ATM_TRAN))								// 고객거래중	모드
	{
		strTempRpl.SetAt(0, '1');								// 취급중
	}

	if ((RecycleBoxStatus == CST_NEAR)	||						// 잔량소
		(RecycleBoxStatus == CST_EMPTY)	||						// 요보충
		(RecycleBoxStatus == CST_FULL))							// FULL
	{
		strTempRpl.SetAt(1, '1');								// 현금
	}

	if (DeviceLoss)												// 미수취장애
	{
		strTempRpl.SetAt(2, '1');								// 분실
	}

	if ((DoorStatus == DOOR_OPENED) ||							// 뒷문열림
		(AtmStatus == ATM_ERROR))								// 장애발생		모드
	{
		strTempRpl.SetAt(3, '1');								// 알람
	}

	if ((SlipStatus == SLIP_LOW_END)		||					// 명세표 용지 잔량소
		(SlipStatus == SLIP_EMPTY_PAPER)	||					// 명세표 용지 요보충
		(JnlStatus  == JNL_LOW_END)			||					// 저널 용지 잔량소
		(JnlStatus  == JNL_EMPTY_PAPER))						// 저널 용지 요보충
	{
		strTempRpl.SetAt(4, '1');								// 명세표
	}

	if (fnSPL_GetCallKey())										// 호출키구하기
	{
		strTempRpl.SetAt(5, '1');								// 호출
	}

	if ((nInitFlag) ||											// 초기설정요구
		(strTempRpl != strSaveRpl))								// 변경시
	{
		strSaveRpl = strTempRpl;
		fnSPL_SetRpl(strTempRpl);								// Rpl설정(값) 2004.07.13
	}

	return TRUE;
}

// 정보Inform전송
int CDevCmn::fnAPL_SendInformationInform(int nInitFlag)
{
	static	CString	strSaveInform("000000");
	CString	strTempInform("000000");

//		┌────────┬────────────────────┐
//		│	  INDEX       │               CONTENTS                 │
//		├────────┼────────────────────┤
//		│       [0]      │    LOSS    - 분실                      │
//		│       [1]      │    CASH    - 현금                      │ 
//		│       [2]      │    ALARM   - 알람                      │
//		│       [3]      │    RECEIPT - 명세표                    │
//		└────────┴────────────────────┘
	if (DeviceLoss)												// 미수취장애
	{
		strTempInform.SetAt(0, '1');							// 분실
	}

	if ((RecycleBoxStatus == CST_NEAR)	||						// 잔량소
		(RecycleBoxStatus == CST_EMPTY)	||						// 요보충
		(RecycleBoxStatus == CST_FULL))							// FULL
	{
		strTempInform.SetAt(1, '1');							// 현금
	}

	if ((DoorStatus == DOOR_OPENED) ||							// 뒷문열림
		(AtmStatus == ATM_ERROR))								// 장애발생		모드
	{
		strTempInform.SetAt(2, '1');							// 알람
	}

	if ((SlipStatus == SLIP_LOW_END)		||					// 명세표 용지 잔량소
		(SlipStatus == SLIP_EMPTY_PAPER)	||					// 명세표 용지 요보충
		(JnlStatus  == JNL_LOW_END)			||					// 저널 용지 잔량소
		(JnlStatus  == JNL_EMPTY_PAPER))						// 저널 용지 요보충
	{
		strTempInform.SetAt(3, '1');							// 명세표
	}

	if ((nInitFlag) ||											// 초기설정요구
		(strTempInform != strSaveInform))						// 변경시
	{
		strSaveInform = strTempInform;
		fnSPL_SetInform(strTempInform);							// Inform설정(값)

		if (AtmDefine.MachineType == U8100)			
		{
			// Inform 장애 신호 후, 해소 전문이 바로 전송될 경우 전송이 안됨(약 3초 이내)
			// Inform 신호 발생 후, Time Delay를 주어서 정상적으로 신호 접수토록 조치함.
			Delay_Msg(5 * 1000);
		}

	}

	return TRUE;
}

// 장애화면출력및인자
int CDevCmn::fnAPL_DisplayError(int nErrorStatus, int nInitFlag)
{
	static	CString	strSaveProcCount("");
	static	CString	strSaveErrorCode("");
	static	CString	strSaveClerkErrorMsg("");
	CString	strTempClerkErrorMsg("");
	static int		nRetryFlg = FALSE;
	CString GetKeyStr;

	switch (nErrorStatus)
	{
		case ATM_ERROR: 										// 장애발생 모드
			if (nInitFlag)
			{
				strSaveProcCount = "";
				strSaveErrorCode = "";
			}

			fnAPL_CheckError();									// 장애검지

			if ((fnSCR_GetCurrentScreenNo() != 701)			||	// 현재화면번호구하기
				(strSaveProcCount != CurErrBuff.ProcCount)	||
				(strSaveErrorCode != CurErrBuff.ErrorCode))
			{
				strSaveProcCount = CurErrBuff.ProcCount;
				strSaveErrorCode = CurErrBuff.ErrorCode;

				if (!fnAPL_CheckError())
				{
					CString strTmp("");
					fnAPL_DisplayErrorSegment(CurErrBuff.ProcCount[0], CurErrBuff.ErrorCode);
					fnSCR_DisplayString(1, GetSprintf("%5.5s(%2.2s)", CurErrBuff.ErrorCode, &CurErrBuff.ErrorCode[5]).GetBuffer(0));
					fnSCR_DisplayString(2, GetSprintf("%28.28s", CurErrBuff.ErrorString).GetBuffer(0));
					
					if ((fnSCR_GetCurrentScreenNo() != 701)	||
						(strTempClerkErrorMsg != strSaveClerkErrorMsg))
					{
						strTmp = IniGetStr(_TRANS_NEARBRANCH_INI, "1", "cname", "");		// #N0172
						if (!strTmp.IsEmpty())
							fnSCR_DisplayString(10, "인근IBK코너조회");
					}

					
					/*
					if (nTranStopFlg == TRUE)												//#N0264
					{
						fnSCR_DisplayString(1, "죄송합니다");
						fnSCR_DisplayString(2, "기기 점검(AP)으로 사용하실 수 없습니다");
						fnSCR_DisplayString(3, "다른 기기를 이용하여 주십시오");
					}
					*/

					fnSCR_DisplayScreen(701);

					if (nInitFlag)
					{
						if (!nRetryFlg)
						{
							nRetryFlg = TRUE;
							fnAPL_PrintError(DEV_JPR);			// 장애인자
						}
					}
				}
				else
				{
// 					fnSCR_DisplayString(2, GetString(m_pProfile->TRANS.GuideMsg1, sizeof(m_pProfile->TRANS.GuideMsg1), 28));
// 					fnSCR_DisplayString(3, GetString(m_pProfile->TRANS.GuideMsg2, sizeof(m_pProfile->TRANS.GuideMsg2), 28));
// 					fnSCR_DisplayString(4, GetString(m_pProfile->TRANS.GuideMsg3, sizeof(m_pProfile->TRANS.GuideMsg3), 28));
					fnSCR_DisplayString(2, "사용중지");
					fnSCR_DisplayString(3, "빠른시간내에 복구예정오이니");
					fnSCR_DisplayString(4, "잠시만 기다려 주십시오");
					fnSCR_DisplayScreen(701);					// 장애화면
				}
			}
			break;

		case ATM_CLERK:											// 계원 모드
		case ATM_READY:											// 준비 모드
			if (nInitFlag)
				strSaveClerkErrorMsg = "";

			if (nErrorStatus == ATM_READY)						// ATM_READY는 2회이상
				if (AtmStatusSave != ATM_READY)
					break;

			strTempClerkErrorMsg = "사용중지 상태 입니다";
			if (!OpenKey)
				strTempClerkErrorMsg = "지금은 기기 점검중입니다";
			else 
			if (DoorStatus == DOOR_OPENED)
				strTempClerkErrorMsg = "금고부 열림";
			else 
			if (CabinetStatus == DOOR_OPENED)
				strTempClerkErrorMsg = "상단부 열림";
			else
			if (SlipHeadStatus != HEADER_NORMAL)
				strTempClerkErrorMsg = "명세표부 COVER 확인요";
			else
			if (A4pHeadStatus != HEADER_NORMAL)
				strTempClerkErrorMsg = "A4P프린터 상태 확인요";
			else 
			if (JnlHeadStatus != HEADER_NORMAL)
				strTempClerkErrorMsg = "저널부 COVER 확인요";
			else 
			if (SlipStatus == SLIP_EMPTY_PAPER)
				strTempClerkErrorMsg = "명세표를 보충하여 주십시오";
			else 
			if (JnlStatus == JNL_EMPTY_PAPER)
				strTempClerkErrorMsg = "저널용지를 보충하여 주십시오";
			else 
			if (A4pStatus == SLIP_EMPTY_PAPER)
				strTempClerkErrorMsg = "A4P프린터 용지를 보충하여 주십시오";
			else 
			if (BrmLoc == RECOVERING_POS)				
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "BrmLoc == RECOVERING_POS 53301");
				return TRUE;
			}
			else
			if (ForceMissMatchDownFlag)
				strTempClerkErrorMsg = "카세트 [점퍼 설정] 오류!, 1,2카세트 점퍼 설정 확인!";
			else
			if (BrmLoc != NORMAL_POS)
				strTempClerkErrorMsg = "현금입출금부 정위치 확인요";
			else 
			if (RejectCSTLoc != NORMAL_POS) 
				strTempClerkErrorMsg = "현금입출금부 [REJECT]리젝트 정위치 확인요";
			else 
			if (RetractCSTLoc != NORMAL_POS)
				strTempClerkErrorMsg = "현금입출금부 [RETRACT]회수함 정위치 확인요";
			else 
			if (UcmLoc != NORMAL_POS)
				strTempClerkErrorMsg = "수표입출금부 정위치 확인요";
			else 
			if (CabinetStatus == DOOR_OPENED && DoorStatus == DOOR_OPENED)
				strTempClerkErrorMsg = "상단부,금고부 열림";
			else
			if (ForceDownFlag)
				strTempClerkErrorMsg = "현금입출금부 (잔액 확인) 직원 출동 예정 입니다";
			else
			if (RemoteDownFlag)
				strTempClerkErrorMsg = "REMOTE CONTRL ..........";
			else
			if (!fnAPL_CheckHostLine())
			{
				strTempClerkErrorMsg = "회선 연결중 입니다";
	
				if ((fnSCR_GetCurrentScreenNo() != 701)	||
					(strTempClerkErrorMsg != strSaveClerkErrorMsg))
				{
					CString strTmp("");
					strTmp = IniGetStr(_TRANS_NEARBRANCH_INI, "1", "cname", "");		// #N0172
					if (!strTmp.IsEmpty())
						fnSCR_DisplayString(10, "인근IBK코너조회");
				}
			}
			else
			if (!fnAPL_CheckHostOpen())
			{
				strTempClerkErrorMsg = "온라인 준비중 입니다";
			}
				
			if ((fnSCR_GetCurrentScreenNo() != 701)	||
				(strTempClerkErrorMsg != strSaveClerkErrorMsg))
			{
				strSaveClerkErrorMsg = strTempClerkErrorMsg;

				if(RemoteDownFlag)
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ReMoteCtrl MSG");
					SplitString("[ 안  내 ]|사용중지 상태 입니다|빠른 시간내에 복구예정이오니|잠시만 기다려 주십시오| | |", "|", strMsgArray);

					fnSCR_DisplayString(1, strMsgArray[0]);
					fnSCR_DisplayString(2, strMsgArray[1]);
					fnSCR_DisplayString(3, strMsgArray[2]);
					fnSCR_DisplayString(4, strMsgArray[3]);
// 					fnSCR_DisplayString(5, strMsgArray[5]);
					fnSCR_DisplayScreen(701);						// 사용중지
				}
				else
				{

					fnSCR_DisplayString(2, strTempClerkErrorMsg);
					fnSCR_DisplayString(3, "빠른 시간내에 복구예정이오니");
					fnSCR_DisplayString(4, "잠시만 기다려 주십시오");
					fnSCR_DisplayScreen(701);// 화면출력
//					fnSCR_DisplayScreen(701, K_NO_WAIT, PIN_PASSWORD_MODE, "", 6, 6, FALSE, "");// 화면출력
				}
			}
			break;

		default :
			break;
	}

	return TRUE;
}

// 장애인자
int CDevCmn::fnAPL_PrintError(int nDevId)
{
	if (!fnAPL_CheckError())									// 장애검지
	{
		fnAPL_EjrSpoolPrintStart(nDevId);						// 장애출력
		fnAPL_EjrSpoolData(nDevId, 0, 0, GetSprintf("LC:%1.1s-%2.2s  MTC:%5.5s(%2.2s)", 
													CurErrBuff.ProcCount,
													fnAPL_GetLC(CurErrBuff.ErrorCode, TranProc).GetBuffer(0),
													CurErrBuff.ErrorCode,
													&CurErrBuff.ErrorCode[5]));
		fnAPL_EjrSpoolData(nDevId, 1, 0, GetSprintf("%24.24s", 
													GetString(CurErrBuff.ErrorString, sizeof(CurErrBuff.ErrorString), 24).GetBuffer(0)));
		if (memcmp(CurErrBuff.ErrorCode, "0110", 4) == 0)
			fnAPL_EjrSpoolEnd(nDevId, JNL_STATUS);
//			fnAPL_EjrSpoolEnd(nDevId, JNL_OPER);
		else
			fnAPL_EjrSpoolEnd(nDevId, JNL_ERROR);

		return (fnAPL_CheckDeviceAction(nDevId));
	}

	return TRUE;
}

// 장치정위치인자
int CDevCmn::fnAPL_CheckPrintDevicePosition()
{
	int		nTempReturn = TRUE;

	if (!fnAPL_CheckPrintBrmPosition())							// Brm정위치인자
		nTempReturn = FALSE;
	if (!fnAPL_CheckPrintUcmPosition())							// Ucm정위치인자
		nTempReturn = FALSE;
	if (!fnAPL_CheckPrintRearDoorStatus())						// 둿문상태인자
		nTempReturn = FALSE;

	return nTempReturn;
}

// Brm정위치인자
int CDevCmn::fnAPL_CheckPrintBrmPosition()
{
	int		nTempReturn = TRUE;
	static	int nSaveBrmLoc			= NORMAL_POS;				// Brm정위치상태
	static	int	nSaveRejectCSTLoc	= NORMAL_POS;				// 회수함정위치상태
	static	int	nSaveClerkCSTLoc	= NORMAL_POS;				// 보충함정위치상태
	static	int	nSaveFstCSTLoc		= NORMAL_POS;				// 1카세트정위치상태(Cd)
	static	int	nSaveSndCSTLoc		= NORMAL_POS;				// 2카세트정위치상태(Cd)
	static	int	nSaveTrdCSTLoc		= NORMAL_POS;				// 3카세트정위치상태(Cd)
	static	int	nSaveFthCSTLoc		= NORMAL_POS;				// 4카세트정위치상태(Cd)

	static	int	nSaveRetractCSTLoc	= NORMAL_POS;			// 리젝트정위치상태



	char	szBrmPrint[][256]	= {"[  :  :  출금부 정위치 ]", "[  :  :  출금부 빠짐   ]"};
	char	szRJPrint[][256]	= {"[  :  :  리젝트함 정위치 ]", "[  :  :  리젝트함 빠짐   ]"};
	char	szRTPrint[][256]	= {"[  :  :  회수함 정위치 ]", "[  :  :  회수함 빠짐   ]"};
	char	szCCPrint[][256]	= {"[  :  :  보충함 정위치 ]", "[  :  :  보충함 빠짐   ]"};
	char	szFSTPrint[][256]	= {"[  :  :  카세트1 정위치 ]", "[  :  :  카세트1 빠짐   ]"};
	char	szSNDPrint[][256]	= {"[  :  :  카세트2 정위치 ]", "[  :  :  카세트2 빠짐   ]"};
	char	szTRDPrint[][256]	= {"[  :  :  카세트3 정위치 ]", "[  :  :  카세트3 빠짐   ]"};
	char	szFTHPrint[][256]	= {"[  :  :  카세트4 정위치 ]", "[  :  :  카세트4 빠짐   ]"};

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		switch (AtmDefine.CashHandler)
		{
			case BRM_SET:
			case OKI_SET:									
			case HBRM_SET:									
				if (nSaveBrmLoc != BrmLoc)
				{
					nSaveBrmLoc = BrmLoc;
					if (!fnAPL_PrintPosition(szBrmPrint[BrmLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveRejectCSTLoc != RejectCSTLoc)
				{
					nSaveRejectCSTLoc = RejectCSTLoc;
					if (!fnAPL_PrintPosition(szRJPrint[RejectCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveClerkCSTLoc != ClerkCSTLoc)
				{
					nSaveClerkCSTLoc = ClerkCSTLoc;
					if (!fnAPL_PrintPosition(szCCPrint[ClerkCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveRetractCSTLoc != RetractCSTLoc)
				{
					nSaveRejectCSTLoc = RejectCSTLoc;
					if (!fnAPL_PrintPosition(szRTPrint[RetractCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveFstCSTLoc != FstCSTLoc)
				{
					nSaveFstCSTLoc = FstCSTLoc;
					if (!fnAPL_PrintPosition(szFSTPrint[FstCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveSndCSTLoc != SndCSTLoc)
				{
					nSaveSndCSTLoc = SndCSTLoc;
					if (!fnAPL_PrintPosition(szSNDPrint[SndCSTLoc]))
						nTempReturn = FALSE;
				}

// 				if (nSaveTrdCSTLoc != TrdCSTLoc)
// 				{
// 					nSaveTrdCSTLoc = TrdCSTLoc;
// 					if (!fnAPL_PrintPosition(szTRDPrint[TrdCSTLoc]))
// 						nTempReturn = FALSE;
// 				}

				break;

			case CDU_SET:
				MsgDump(MAC_TRACE_DUMP_FILE("Log"), "fnAPL_CheckPrintBrmPosition() OLD [%d / %d / %d / %d / %d / %d ]", nSaveBrmLoc, nSaveRejectCSTLoc, nSaveFstCSTLoc, nSaveSndCSTLoc, nSaveTrdCSTLoc, nSaveFthCSTLoc);
				MsgDump(MAC_TRACE_DUMP_FILE("Log"), "fnAPL_CheckPrintBrmPosition() NEW [%d / %d / %d / %d / %d / %d ]",      BrmLoc,      RejectCSTLoc,      FstCSTLoc,      SndCSTLoc,      TrdCSTLoc,      FthCSTLoc);
				if (nSaveBrmLoc != BrmLoc)
				{
					nSaveBrmLoc = BrmLoc;
					if (!fnAPL_PrintPosition(szBrmPrint[BrmLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveRejectCSTLoc != RejectCSTLoc)
				{
					nSaveRejectCSTLoc = RejectCSTLoc;
					if (!fnAPL_PrintPosition(szRJPrint[RejectCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveFstCSTLoc != FstCSTLoc)
				{
					nSaveFstCSTLoc = FstCSTLoc;
					if (!fnAPL_PrintPosition(szFSTPrint[FstCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveSndCSTLoc != SndCSTLoc)
				{
					nSaveSndCSTLoc = SndCSTLoc;
					if (!fnAPL_PrintPosition(szSNDPrint[SndCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveTrdCSTLoc != TrdCSTLoc)
				{
					nSaveTrdCSTLoc = TrdCSTLoc;
					if (!fnAPL_PrintPosition(szTRDPrint[TrdCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveFthCSTLoc != FthCSTLoc)
				{
					nSaveFthCSTLoc = FthCSTLoc;
					if (!fnAPL_PrintPosition(szFTHPrint[FthCSTLoc]))
						nTempReturn = FALSE;
				}
				break;
		}
	}

	return nTempReturn;
}

// Ucm정위치인자
int CDevCmn::fnAPL_CheckPrintUcmPosition()
{
	int		nTempReturn = TRUE;
	static	int nSaveUcmLoc	= NORMAL_POS;						// Ucm정위치상태

	char	szUcmPrint[][256] = {"[  :  :  수표입금부정위치]", "[  :  :  수표입금부빠짐  ]"};

	if (fnAPL_GetDefineDevice(DEV_UCM))
	{
		if (nSaveUcmLoc != UcmLoc) 
		{
			nSaveUcmLoc = UcmLoc;
			if (!fnAPL_PrintPosition(szUcmPrint[UcmLoc]))
				nTempReturn = FALSE;
		}
	}

	return nTempReturn;
}

// 둿문상태인자
int CDevCmn::fnAPL_CheckPrintRearDoorStatus()
{
	int		nTempReturn = TRUE;
	static	int nSaveDoorStatus			= DOOR_CLOSED;					// DoorStatus상태
	static	int	nSaveCabinetStatus		= DOOR_CLOSED;

	char	szDoorPrint[][256] = {"[  :  :  뒷문 열림     ]",
		                          "[  :  :  뒷문 닫힘     ]"  };


	char	szCabinetPrint[][256]   = {"[  :  :  상단부 열림    ]",
								       "[  :  :  상단부 닫힘    ]" };

	if (nSaveDoorStatus != DoorStatus) 
	{
		if (DoorStatus == DOOR_OPENED)							// 뒷문개선
			fnAPL_DeviceEnDisable(DEV_MCU | DEV_PBM, DISABLE);

		nSaveDoorStatus = DoorStatus;
		if (!fnAPL_PrintPosition(szDoorPrint[DoorStatus]))
			nTempReturn = FALSE;
	}


	if (nSaveCabinetStatus != CabinetStatus) 
	{
		if (CabinetStatus == DOOR_OPENED)							// 뒷문개선
			fnAPL_DeviceEnDisable(DEV_MCU | DEV_PBM, DISABLE);

		nSaveCabinetStatus = CabinetStatus;
		
		if (!fnAPL_PrintPosition(szCabinetPrint[CabinetStatus]))
			nTempReturn = FALSE;
	}

	nSaveDoorStatus = DoorStatus;
	SaveCabinetStatus = CabinetStatus;

	
	
	return nTempReturn;
}

// 정위치인자
int CDevCmn::fnAPL_PrintPosition(LPCTSTR szPrintData)
{
	char	szTempPrintBuffer[256];
	char	szTempTimeBuffer[256];
	int		nTempBuffLen = 0;

	memset(szTempPrintBuffer,	0, sizeof(szTempPrintBuffer));
	memset(szTempTimeBuffer,	0, sizeof(szTempTimeBuffer));

	EjrTbl	EjrDataTemp;										
	memset(&EjrDataTemp, 0, sizeof(EjrDataTemp));				

	if (fnCMR_GetRoomCameraDeviceStatus() == NORMAL)	
	{
		if ((DoorStatus == DOOR_OPENED)		||					// U8100-AP변경 #01 - 3Camera, Preview지원
			(BrmLoc != NORMAL_POS) || (RejectCSTLoc != NORMAL_POS) || (ClerkCSTLoc != NORMAL_POS) || 
			(UcmLoc != NORMAL_POS))
		{
			;
		}
	}


	fnAPL_ECashEjrSpoolPrint(DEV_JPR,szPrintData);

	// BRM, UCM 탈착시 후방카메라를 Capture Start
	if (fnCMR_GetRoomCameraDeviceStatus() == NORMAL)	
	{
		if (DoorStatus == DOOR_OPENED)							// 3Camera, Preview지원
		{
			fnAPL_CaptureRoomSave(1, TRUE);						// 뒷문열렸을시 후방카메라를 Capture
			fnAPL_CaptureSaveEnd(TRUE);							// 4차수정 보완 (T3ATM AP 변경내역 #17) - 이전이미지 저장되는 경우가 있어 대책함
		}
		else
		if ((BrmLoc != NORMAL_POS) || (RejectCSTLoc != NORMAL_POS) || (ClerkCSTLoc != NORMAL_POS) || 
			(UcmLoc != NORMAL_POS))
		{
			fnAPL_CaptureRoomSave(1, TRUE);						// 뒷문열렸을시 후방카메라를 Capture
			fnAPL_CaptureSaveEnd(TRUE);							// 4차수정 보완 (T3ATM AP 변경내역 #17) - 이전이미지 저장되는 경우가 있어 대책함
		}
	}

	// BRM, UCM 탈착시 후방카메라를 Capture End
	if (m_pProfile->DEVICE.MachineType == U8100)	
		return (fnAPL_CheckDeviceAction(DEV_JPR | DEV_PBM | DEV_MCU));		// 통장부 투입모드설정 오류로 인한 거래시간지연 대책의 건 V04-01-01-#12
	else
		return (fnAPL_CheckDeviceAction(DEV_JPR));
}


//int CDevCmn::fnAPL_PrintCashMoveInfo(int nMoveFlag, int nRealMove, int nTenThRJT, int nFiveThRJT, int nOneThRJT, int nUnknownRJT, int nFiftyThRJT)
int CDevCmn::fnAPL_PrintCashMoveInfo(int nMoveFlag, int nRealMove, int nTenThRJT, int nFiveThRJT, int nOneThRJT, int nUnknownRJT, int nFiftyThRJT, int nTenThMoveCnt, int nFiftyThMoveCnt)
{
	int		i = 0;
	CString strTemp("");

	if (AtmDefine.CashHandler == CDU_SET)
		return TRUE;

	fnAPL_EjrSpoolPrintStart(DEV_JPR);

	// line 1
	if (nMoveFlag & BRM_AUTOLOAD)
	{
		if (nMoveFlag & REFILL_MOVE)
			fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("%2.2s:%2.2s [자동 지폐보충]", 
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2])));
		else
			fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("%2.2s:%2.2s [자동 지폐회수]", 
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2])));
	}
	else if (nMoveFlag & BRM_MANUALLOAD)
	{
		if (nMoveFlag & REFILL_MOVE)
			fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("%2.2s:%2.2s [계원 지폐보충]", 
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2])));
		else 
			fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("%2.2s:%2.2s [계원 지폐회수]", 
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2])));
	}
	else 
	if (nMoveFlag & BRM_RESET)
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("%2.2s:%2.2s [RESET 지폐회수]", 
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2])));
	else 
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("%2.2s:%2.2s [지폐회수]", 
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2])));
	if (nMoveFlag & (BRM_AUTOLOAD | BRM_MANUALLOAD))
	{
		if ((nMoveFlag & REFILL_MOVE) == REFILL_MOVE)
			fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("보충매수 : %5d매", nRealMove));
		else fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("회수매수 : %5d매", nRealMove)); 

		if (((OKI_SET == m_pDevCmn->AtmDefine.CashHandler) || (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)) &&	// U8100 AP 변경내역 #04 - HBRM관련 변경내역
			(m_pDevCmn->ChangeBCFlag))
		{
			if ((nMoveFlag & REFILL_MOVE) == REFILL_MOVE)
			{
				fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("오만 보충매수 : %5d매", nFiftyThMoveCnt));
				fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("만원 보충매수 : %5d매", nTenThMoveCnt));
			}
			else
			{
				fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("오만 회수매수 : %5d매", nFiftyThMoveCnt));
				fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("만원 회수매수 : %5d매", nTenThMoveCnt));
			}
		}
	}
	else fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("회수금액 : %10d원", nRealMove));


	if (((OKI_SET == m_pProfile->DEVICE.CashHandler) || (HBRM_SET == m_pProfile->DEVICE.CashHandler)) &&
		(ChangeBCFlag))											
	{
		// line 3						   123456789012345678901234
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, "------ REJECT 상세 -----");
		// line 4
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("오만원 REJECT : %5d매", nFiftyThRJT));
		// line 5
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("만원   REJECT : %5d매", nTenThRJT));
		// line 6
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("오천원 REJECT : %5d매", nFiveThRJT));
		// line 7
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("천원   REJECT : %5d매", nOneThRJT));
		// line 8
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("불명권 REJECT : %5d건", nUnknownRJT));

		if(0 < nUnknownRJT)											// V05-01-02-#12:2007.11.27 불명권 안내문구 개선 : COM07-17
		{
			// line 9
			fnAPL_EjrSpoolData(DEV_JPR, i++, 0, "불명권금액매수확인요망");
		}
	}
	else
	{
		// line 3						   123456789012345678901234
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, "------ REJECT 상세 -----");
		// line 4
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("만원   REJECT : %5d매", nTenThRJT));
		// line 5
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("오천원 REJECT : %5d매", nFiveThRJT));
		// line 6
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("천원   REJECT : %5d매", nOneThRJT));
		// line 7
		// V06-06-00-#01 불명권 안내문구 개선  ────────────────
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("불명권 REJECT : %5d건", nUnknownRJT + nFiftyThRJT));	// V07-01-01-#03 오만원권 미개체기기 오만원권 회수시 인자내역 수정
		// line 8
		if (0 < nUnknownRJT)
			fnAPL_EjrSpoolData(DEV_JPR, i++, 0, "불명권금액매수확인요망");
		// ──────────────────────────────────
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[FILLTAKE][%d|%d|%d|%d|%d|0|0|0|0|0|0|%d]", 
											nRealMove,			// 전체
											nOneThRJT,			// 천원보충(회수)매수
											nFiveThRJT,			// 오천원보충(회수)매수
																// 만원보충(회수)매수(T1ATM은 회수/보충매수가 전체매수와 동일하다 : 신/구만원권)
											(AtmDefine.CashHandler == BRM_SET) ? nRealMove : nTenThMoveCnt,
											nFiftyThMoveCnt,	// 오만원보충(회수)매수
											nUnknownRJT);		// 불명권보충(회수)매수

	fnAPL_BRMCashInfo();
	fnAPL_EjrSpoolEnd(DEV_JPR);
	return (fnAPL_CheckDeviceAction(DEV_JPR));
}

// 현금등록정보인자					
int CDevCmn::fnAPL_PrintCashInfo()
{
	fnAPL_EjrSpoolPrintStart(DEV_JPR);

	fnAPL_EjrSpoolData(DEV_JPR, 0, 0, "[현금정보]");
	fnAPL_EjrSpoolData(DEV_JPR, 1, 0, GetSprintf("현금매수 : %5d매", fnAPL_GetNumberOfCash()));

	fnAPL_EjrSpoolEnd(DEV_JPR);	
	return (fnAPL_CheckDeviceAction(DEV_JPR));
}

// 수표등록정보인자
// V02-08-01-R1 100만원권출금 - 저널인자 및 권종인자
int CDevCmn::fnAPL_PrintCheckInfo()
{
	fnAPL_EjrSpoolPrintStart(DEV_JPR);
	fnAPL_EjrSpoolData(DEV_JPR, 0, 0, "[수표등록]");

	fnAPL_EjrSpoolData(DEV_SPR | DEV_JPR, 1, 0, GetSprintf("수표번호1 : %8.8s", fnAPL_GetNumberOfOutCheckNo().GetBuffer(0)));
	fnAPL_EjrSpoolData(DEV_SPR | DEV_JPR, 2, 0, GetSprintf("수표매수1 : %4d매", fnAPL_GetNumberOfOutCheck()));

	// 2003.11.06 PSB
	fnAPL_EjrSpoolData(DEV_SPR | DEV_JPR, 3, 0, GetSprintf("수표번호2 : %8.8s", fnAPL_GetNumberOfOutCheckNo2().GetBuffer(0)));
	fnAPL_EjrSpoolData(DEV_SPR | DEV_JPR, 4, 0, GetSprintf("수표매수2 : %4d매", fnAPL_GetNumberOfOutCheck2()));

	fnAPL_EjrSpoolEnd(DEV_JPR, JNL_CASH);
	return (fnAPL_CheckDeviceAction(DEV_JPR));
}

// 저널인자
int CDevCmn::fnAPL_PrintJnlInfo(LPCTSTR szJnlInfoData, int JnlType, int nWaitFlag)
{

	if (!strlen(szJnlInfoData))
		return TRUE;

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_PrintJnlInfo JNL : %s", szJnlInfoData);	
	fnAPL_ECashEjrSpoolPrint(DEV_JPR,szJnlInfoData);	

// 	CString strTemp(szJnlInfoData);
// 	m_pDevCmn->fnUTL_ActionJnlPrint(strTemp);


	return TRUE;
}

// AutoLoad을 위한 화면설정 및 디바이스 동작체크
int CDevCmn::fnAPL_SprAutoload()
{

	char    Dispbuf[81];										
	memset(Dispbuf, 0x20, sizeof(Dispbuf));

	if (!m_pDevCmn->fnSPR_GetAutoloadCondition())				// 다시한번 체크
		return TRUE;

	if (!fnAPL_GetAvailDevice(DEV_SPR))							// 사용가능한장치구하기
		return TRUE;

	fnSCR_DisplayString(1, "명세표 보충중");
	fnSCR_DisplayString(2, "빠른 시간내에 복구예정이오니");
	fnSCR_DisplayString(3, "잠시만 기다려 주십시오");
	fnSCR_DisplayScreen(701);						// 사용중지

	fnAPL_CheckDeviceAction(DEV_SPR);					// 동작대기및장애검지
	fnSPR_Autoload();									// 명세표autoload

	if (!fnAPL_CheckDeviceAction(DEV_SPR))				// 동작대기및장애검지
		return FALSE;
	else
		return TRUE;
}

// Brm자동보충/회수
int CDevCmn::fnAPL_AutoRefillBrm(int nBrmRefillMode)
{
	CStringArray strTempRefillArray;
	CStringArray strTempTakeupArray;
	CStringArray strTempRetractArray;
	CString		 strBrmRefillModeBackup("");
	CString		 strReturnBRMErrCode("");						// V06-01-01-#18
	int			nRet = TRUE;
	
	if (!fnAPL_GetAvailDevice(DEV_CSH))							// 사용가능한장치구하기
		return TRUE;

	switch (nBrmRefillMode) 
	{
		case NORMAL:
			return TRUE;
			break;

		case NEED_REFILL:
			if(AtmDefine.MachineType == U3100K)  //2009.07.22 Kim.Gi.Jin
				break;

			fnSCR_DisplayString(1, "현금 자동 보충중입니다");
			fnSCR_DisplayString(2, "빠른 시간내에 복구예정이오니");
			fnSCR_DisplayString(3, "잠시만 기다려 주십시오");
			fnSCR_DisplayString(4, "                      ");   // #0093
			fnSCR_DisplayScreen(701);							// 사용중지
			fnAPL_CheckDeviceAction(DEV_CSH);					// 동작대기및장애검지
			fnCSH_RefillMoney(0);								// 보충(현금매수)
			if (!fnAPL_CheckDeviceAction(DEV_CSH))				// 동작대기및장애검지
				return FALSE;
			fnAPL_DisplayNormalSegment();						// 정상Segment출력
			SplitString(fstrCSH_GetRefillResult(), ",", strTempRefillArray);
																// 보충결과구하기
			if (OKI_SET == m_pDevCmn->AtmDefine.CashHandler)
			{
				nRet = fnAPL_PrintCashMoveInfo(BRM_AUTOLOAD | REFILL_MOVE,
								Asc2Int(strTempRefillArray[1]),	// 보충매수
								Asc2Int(strTempRefillArray[3]),	// 만원회수매수
								Asc2Int(strTempRefillArray[4]),	// 오천원회수매수
								Asc2Int(strTempRefillArray[5]),	// 천원회수매수
								Asc2Int(strTempRefillArray[7])	// 수표회수매수
							+	Asc2Int(strTempRefillArray[8]),	// 불명권회수매수
								Asc2Int(strTempRefillArray[6]),	// 오만원회수매수
								Asc2Int(strTempRefillArray[9]),	// 만원권 정상보충매수	
								Asc2Int(strTempRefillArray[10]));// 오만원권 정상보충매수	
																// 현금이동정보인자
				return nRet;
			}
			else
			{
				nRet = fnAPL_PrintCashMoveInfo(BRM_AUTOLOAD | REFILL_MOVE,
								Asc2Int(strTempRefillArray[1]),	// 보충매수
								Asc2Int(strTempRefillArray[3]),	// 만원회수매수
								Asc2Int(strTempRefillArray[4]),	// 오천원회수매수
								Asc2Int(strTempRefillArray[5]),	// 천원회수매수
								Asc2Int(strTempRefillArray[6])	// 수표회수매수
							+	Asc2Int(strTempRefillArray[7]));// 불명권회수매수
																// 현금이동정보인자
				return nRet;
			}
			break;
		case NEED_TAKEUP50000:									// V06-01-01-#07
		case NEED_TAKEUP:
		case NEED_TAKEUP_OLD:
		case NEED_TAKEUP_NEW:
			if ((m_pDevCmn->AtmDefine.CashHandler == OKI_SET)	||					// T2ATM
				(m_pDevCmn->AtmDefine.CashHandler == HBRM_SET)	)					// U8100
			{
				if (nBrmRefillMode == NEED_TAKEUP_OLD)			// 구권만회수
				{
					RegSetInt(_REGKEY_BRMINFO, "회수권종", BRM_NOTEKIND_10000_OLD);	// 0x30 구권만원
				}
				else
				if (nBrmRefillMode == NEED_TAKEUP_NEW)			// 신권만회수
				{
					RegSetInt(_REGKEY_BRMINFO, "회수권종", BRM_NOTEKIND_10000_NEW);	// 0x35 신권만원
				}
				else
				if (nBrmRefillMode == NEED_TAKEUP50000)			// 5만원권회수
				{
					RegSetInt(_REGKEY_BRMINFO, "회수권종", BRM_NOTEKIND_50000);	// 0x34 : 5만원권
				}
				else											// 신구권회수
				{
					RegSetInt(_REGKEY_BRMINFO, "회수권종", 0x00);// 0x00 : 모든 권종
				}
			}
			else
			{
				strBrmRefillModeBackup = IniGetStr(_SP_DEVICE_INI, "BRM", "신구권회수모드", "2");

				if (nBrmRefillMode == NEED_TAKEUP_OLD)			// 구권만회수
				{
					IniSetStr(_SP_DEVICE_INI, "BRM", "신구권회수모드", "0");
				}
				else
				if (nBrmRefillMode == NEED_TAKEUP_NEW)			// 신권만회수
				{
					IniSetStr(_SP_DEVICE_INI, "BRM", "신구권회수모드", "1");
				}
				else											// 신구권회수
				{
					IniSetStr(_SP_DEVICE_INI, "BRM", "신구권회수모드", "2");
				}
			}

			if(AtmDefine.MachineType == U3100K)  //2009.07.22 Kim.Gi.Jin
				break;


			fnSCR_DisplayString(1, "현금 자동 회수중입니다");
			fnSCR_DisplayString(2, "빠른 시간내에 복구예정이오니");
			fnSCR_DisplayString(3, "잠시만 기다려 주십시오");
			fnSCR_DisplayScreen(701);							// 사용중지
			fnAPL_CheckDeviceAction(DEV_CSH);					// 동작대기및장애검지
			fnCSH_TakeupMoney(BRM_ATAKEUPCONDCNT);				// 회수(현금매수) : T2ATM 매수지정

			IniSetStr(_SP_DEVICE_INI, "BRM", "신구권회수모드", strBrmRefillModeBackup);
			MsgDump(TRACE_FREE_MODE, "Log", __FILE__, __LINE__,  "[[신구권회수모드]]");

			if (!fnAPL_CheckDeviceAction(DEV_CSH))				// 동작대기및장애검지
				return FALSE;

			fnAPL_DisplayNormalSegment();						// 정상Segment출력
			SplitString(fstrCSH_GetTakeupResult(), ",", strTempTakeupArray);
																// 회수결과구하기

			if ((OKI_SET == m_pDevCmn->AtmDefine.CashHandler) || (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler))	// U8100 AP 변경내역 #04 - HBRM관련 변경내역
			{
				nRet = fnAPL_PrintCashMoveInfo(BRM_AUTOLOAD | TAKEUP_MOVE,
								Asc2Int(strTempTakeupArray[1]),	// 회수매수
								Asc2Int(strTempTakeupArray[3]),	// 만원회수매수
								Asc2Int(strTempTakeupArray[4]),	// 오천원회수매수
								Asc2Int(strTempTakeupArray[5]),	// 천원회수매수
								Asc2Int(strTempTakeupArray[7])	// 수표회수매수
							+	Asc2Int(strTempTakeupArray[8]),	// 불명권회수매수
								Asc2Int(strTempTakeupArray[6]),	// 오만원회수매수			// V06-01-01-#10
								Asc2Int(strTempTakeupArray[9]),	// 만원권 정상회수매수		// V06-02-01-#07
								Asc2Int(strTempTakeupArray[10]));// 오만원권 정상회수매수	// V06-02-01-#07
																// 현금이동정보인자
				return nRet;
			}
			else
			{
				nRet = fnAPL_PrintCashMoveInfo(BRM_AUTOLOAD | TAKEUP_MOVE,
								Asc2Int(strTempTakeupArray[1]),	// 회수매수
								Asc2Int(strTempTakeupArray[3]),	// 만원회수매수
								Asc2Int(strTempTakeupArray[4]),	// 오천원회수매수
								Asc2Int(strTempTakeupArray[5]),	// 천원회수매수
								Asc2Int(strTempTakeupArray[6])	// 수표회수매수
							+	Asc2Int(strTempTakeupArray[7]));// 불명권회수매수
																// 현금이동정보인자
				return nRet;
			}
			break;
		default:
			break;
	}

	return TRUE;
}

// Brm자동복구
int CDevCmn::fnAPL_AutoResetBrm()
{
	if (!fnAPL_GetAvailDevice(DEV_CSH))							// 사용가능한장치구하기
		return TRUE;
	fnAPL_CheckDeviceAction(DEV_CSH);							// 동작대기및장애검지

	if ((AtmDefine.CashHandler == BRM_SET)	||
		(AtmDefine.CashHandler == CDU_SET)	)
		fnCSH_Initialize();										// 연결시작및초기화
	else
	if ((AtmDefine.CashHandler == OKI_SET)	||
		(AtmDefine.CashHandler == HBRM_SET)	)					// U8100-AP변경
		fnBRM_Reset(TRUE);										// T2ATM 2006.5.24 yscho
	else
		return TRUE;
	
	return (fnAPL_CheckDeviceAction(DEV_CSH));					// 동작대기및장애검지
}

// Brm자동준비
int CDevCmn::fnAPL_AutoReadyBrm()
{	
	if (!fnAPL_GetAvailDevice(DEV_CSH))							// 사용가능한장치구하기
		return TRUE;

	fnCSH_Ready();												// 입출금준비해제및객교대복구

	return TRUE;
}

// 장치복구
int CDevCmn::fnAPL_ResetDevice(int nDevId, int nInitFlag)				
{

	int		SprRetractCnt = 0;									// 2003.12.20
	int		McuRetractCnt = 0;
	int		PbmRetractCnt = 0;
	int		A4pRetractCnt = 0;
	CStringArray strTempRetractArray;

	if (fnAPL_GetYesDevice(DEV_SPR))							// 2003.12.20
		SprRetractCnt = Asc2Int(fstrSPR_GetRetractCnt());		// 회수매수구하기
	if (fnAPL_GetYesDevice(DEV_MCU))
		McuRetractCnt = Asc2Int(fstrMCU_GetRetractCnt());
	if (fnAPL_GetYesDevice(DEV_PBM))
		PbmRetractCnt = Asc2Int(fstrPBM_GetRetractCnt());
	if (fnAPL_GetYesDevice(DEV_A4P))
		A4pRetractCnt = Asc2Int(fstrA4P_GetRetractCnt());

	nDevId = fnAPL_GetDefineDevice(nDevId);						// 정의된장치구하기
//**************************************************************************************** #KIM 0404
//	if (nDevId & DEV_CSH)										// Brm장치초기화
//		fnAPL_CheckDeviceAction(DEV_CSH);						// 동작대기및장애검지
//****************************************************************************************

	fnAPL_InitializeDevice(nDevId, nInitFlag);					// 장치초기화

	if (nDevId & DEV_CSH)										// Brm장치초기화
	{
		fnAPL_CheckDeviceAction(DEV_CSH);						// 동작대기및장애검지
		Delay_Msg(2000);										// 검지대기(2000ms)
																// 회수내역대기
		fnAPL_CheckDeviceAction(DEV_CSH);						// 동작대기및장애검지
	}

	fnAPL_CheckMaterial();										// 매체잔류검지

	if (nDevId & DEV_MCU)
	{
		if (fnAPL_GetYesDevice(DEV_MCU))						// 2003.12.20
		{
			if (McuRetractCnt < Asc2Int(fstrMCU_GetRetractCnt()))
			{
				DeviceRetract |= DEV_MCU;
				fnAPL_PrintJnlInfo("카드회수", JNL_STATUS);
			}
		}
	}

	
	if (nDevId & DEV_PBM)
	{
		if (fnAPL_GetYesDevice(DEV_PBM))						// 2003.12.20
		{
			if (PbmRetractCnt < Asc2Int(fstrPBM_GetRetractCnt()))
			{
				DeviceRetract |= DEV_PBM;
				fnAPL_PrintJnlInfo("통장회수", JNL_STATUS);
			}
		}
	}

	if (nDevId & DEV_CSH)
	{
		if (fnAPL_GetYesDevice(DEV_CSH))						// 2003.12.20
		{
			SplitString(fstrCSH_GetRetractResult(), ",", strTempRetractArray);

			if ((OKI_SET == m_pDevCmn->AtmDefine.CashHandler) || (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)) // U8100 AP 변경내역 #04 - HBRM관련 변경내역
			{
				// 회수결과구하기
				if ((Asc2Int(strTempRetractArray[0]) == 0)	&&	// 회수금액
					(Asc2Int(strTempRetractArray[1]) == 0)	&&	// 만원회수매수
					(Asc2Int(strTempRetractArray[2]) == 0)	&&	// 오천원회수매수
					(Asc2Int(strTempRetractArray[3]) == 0)	&&	// 천원회수매수
					(Asc2Int(strTempRetractArray[4]) == 0)	&&	// 오만원회수매수
					(Asc2Int(strTempRetractArray[5]) == 0)	&&	// 수표회수매수
					(Asc2Int(strTempRetractArray[6]) == 0));	// 불명권회수매수
				else 
				{
					DeviceRetract |= DEV_CSH;					// 회수결과를 처리함

					if ((!ChangeBCFlag) &&						// V06-02-01-#14:미개체기기 회수금액처리
						(Asc2Int(strTempRetractArray[4]) != 0))
					{
						fnAPL_PrintCashMoveInfo(BRM_RESET,	
							Asc2Int(strTempRetractArray[0])		// 회수금액
						-	Asc2Int(strTempRetractArray[4])*50000,
							Asc2Int(strTempRetractArray[1]),	// 만원회수매수 
							Asc2Int(strTempRetractArray[2]),	// 오천원회수매수 
							Asc2Int(strTempRetractArray[3]),	// 천원회수매수 
							Asc2Int(strTempRetractArray[5])		// 수표회수매수
						+	Asc2Int(strTempRetractArray[6])		// 불명권회수매수
						+	Asc2Int(strTempRetractArray[4]));	// 오만원회수매수
					}
					else
					{
						fnAPL_PrintCashMoveInfo(BRM_RESET,	
							Asc2Int(strTempRetractArray[0]),	// 회수금액
							Asc2Int(strTempRetractArray[1]),	// 만원회수매수 
							Asc2Int(strTempRetractArray[2]),	// 오천원회수매수 
							Asc2Int(strTempRetractArray[3]),	// 천원회수매수 
							Asc2Int(strTempRetractArray[5])		// 수표회수매수
						+	Asc2Int(strTempRetractArray[6]),	// 불명권회수매수
							Asc2Int(strTempRetractArray[4]));	// 오만원회수매수
					}
				}
			}												
			else
			if (BRM_SET == m_pDevCmn->AtmDefine.CashHandler)
			{
				// 회수결과구하기
				if ((Asc2Int(strTempRetractArray[0]) == 0)	&&	// 회수금액
					(Asc2Int(strTempRetractArray[1]) == 0)	&&	// 만원회수매수
					(Asc2Int(strTempRetractArray[2]) == 0)	&&	// 오천원회수매수
					(Asc2Int(strTempRetractArray[3]) == 0)	&&	// 천원회수매수
					(Asc2Int(strTempRetractArray[4]) == 0)	&&	// 수표회수매수
					(Asc2Int(strTempRetractArray[5]) == 0));	// 불명권회수매수
				else 
				{
					DeviceRetract |= DEV_CSH;					// 회수결과를 처리함
					fnAPL_PrintCashMoveInfo(BRM_RESET,	
						Asc2Int(strTempRetractArray[0]),		// 회수금액
						Asc2Int(strTempRetractArray[1]),		// 만원회수매수 
						Asc2Int(strTempRetractArray[2]),		// 오천원회수매수 
						Asc2Int(strTempRetractArray[3]),		// 천원회수매수 
						Asc2Int(strTempRetractArray[4])			// 수표회수매수
						+	Asc2Int(strTempRetractArray[5]));	// 불명권회수매수
				}
			}
			else
			if(CDU_SET == m_pDevCmn->AtmDefine.CashHandler)
			{
				;
			}
		}
	}
	
	if (nDevId & DEV_UCM)
	{
		if (fnAPL_GetYesDevice(DEV_UCM))						// 2003.12.20
		{
			if (DeviceRetract & DEV_UCM)
			{
				fnAPL_PrintJnlInfo("수표회수", JNL_STATUS);
			}
		}
	}

	if (nInitFlag)												// 최초실행시
	{
		if (fnAPL_GetYesDevice(DEV_SPR))						// 2003.12.20
			fnSPR_ClearRetractCnt();							// 회수매수초기화
		if (fnAPL_GetYesDevice(DEV_MCU))
			fnMCU_ClearRetractCnt();
		if (fnAPL_GetYesDevice(DEV_PBM))
			fnPBM_ClearRetractCnt();
		if (fnAPL_GetYesDevice(DEV_A4P))
			fnA4P_ClearRetractCnt();
	}

	if (fnAPL_GetAvailErrorDevice(nDevId))
		return FALSE;
	else
	{
		fnAPL_DisplayNormalSegment();							// 정상Segment출력
		return TRUE;
	}
}

// 사용중지
int CDevCmn::fnAPL_UnUsed()
{
	fnAPL_SetOnTransaction(OFF);								// 거래진행아님
	fnAPL_SetProcCount(' ');									// 진행번호설정
	fnSCR_SetCurrentLangMode(KOR_MODE);							// 현재언어모드설정
	fnAPL_DeviceEnDisable(DEV_MCU | DEV_PBM | DEV_PIN, DISABLE);			// 장치투입모드설정 
	fnLGT_SetFlicker(KU_ALL_FLICKER, FLICKER_OFF);					// Flicker설정(인덱스,값) 
	fnAPL_DisplayInformationLed(TRUE);							// 정보Led출력
	fnAPL_DisplayInformationIndicator(TRUE);					// 준비중
	return TRUE;
}

// 장치투입모드설정
//t int CDevCmn::fnAPL_DeviceEnDisable(int nDevId, int nAction, int nWaitFlag)
int CDevCmn::fnAPL_DeviceEnDisable(int nDevId, int nAction, int nWaitFlag				/* = FALSE				*/, 
															int nPinInputMode			/* = PIN_DISABLE_MODE	*/, 
															LPCTSTR szCardData			/* = ""					*/, 
															int nPinPassWordMin			/* = 4					*/, 
															int nPinPassWordMax			/* = 8					*/, 
															int nPinPassWordAuto		/* = FALSE				*/, 
															LPCTSTR szPinPassWordTerm	/* = "ENTER,CANCEL"		*/)
{
	int		nTempDevId = DEV_NONE;

	nDevId = fnAPL_GetAvailDevice(nDevId);						// 사용가능한장치구하기
//t	fnAPL_CheckDeviceAction(nDevId);							// 동작대기및장애검지
																// Think About : 여기서 PINPAD TIMEOVER발생가능
	if (nDevId != DEV_PIN)
		fnAPL_CheckDeviceAction(nDevId & (~DEV_PIN));			// 동작대기및장애검지

	// Mcu Enable/Disable 상태와 관련없이 동작처리됨
	if (nDevId & DEV_MCU)
	{
		if (nAction == ENABLE)
			fnMCU_EntryEnable();								// 투입허가
		else 
			fnMCU_EntryDisable();								// 투입금지
		nTempDevId |= DEV_MCU;
	}

	// Pbm Enable/Disable 상태와 관련없이 동작처리됨
	if (nDevId & DEV_PBM)
	{
		if (nAction == ENABLE)
			fnPBM_EntryEnable();							
		else 
			fnPBM_EntryDisable();							
		nTempDevId |= DEV_PBM;
	}

	if (nDevId & DEV_PIN)
	{
		if (nAction == ENABLE)
		{
			switch (m_pProfile->DEVICE.MachineType)			
			{
				case U8100:										
					MsgDump(TRACE_FREE_MODE, "Log", __FILE__, __LINE__,  "PIN_MODE_EarPhoneInserted[%d]", EarPhoneInserted);
					if (EarPhoneInserted)
						nPinInputMode = PIN_NUMERIC_MODE;
					break;
				case U3100K:
					if ((nPinInputMode != PIN_DISABLE_MODE)	&&
						(nPinInputMode != PIN_PASSWORD_MODE)&&
						(nPinInputMode != PIN_PASSWORD_MODE2)&&	
						(nPinInputMode != PIN_MENU_MODE)&&
						(nPinInputMode != PIN_MENU_MODE1)&&
						(nPinInputMode != PIN_MENU_MODE2)&&
						(nPinInputMode != PIN_AMOUNT_MODE)&&
						(nPinInputMode != PIN_NUMERIC_MODE)&&
						(nPinInputMode != PIN_PASSWORD_MODE3))
						nPinInputMode = PIN_DUMMY_MODE;
					break;
			}

			MsgDump(TRACE_FREE_MODE, "Log", __FILE__, __LINE__,  "PIN_MODE_nPinInputMode[%d]", nPinInputMode);
			switch (nPinInputMode)
			{
				case PIN_DISABLE_MODE:
					fnLGT_SetFlicker(KU_PIN_ATM_FLICKER, FLICKER_OFF); 
					fnPIN_EntryDisable();
					break;
				case PIN_MENU_MODE:
					fnLGT_SetFlicker(KU_PIN_ATM_FLICKER, FLICKER_OFF); //#0143
					fnPIN_EntryEnable(PINMODE_NORMAL,			// nEnableMode
										0,						// nMinKeyCount
										0,						// nMaxKeyCount
										FALSE,					// bAutoEnd
										PIN_MENU_PAD,			// szActiveKeys
										"",						// szTerminatorKeys
										"",						// szCardData
										K_WAIT);				// nTimeout
					break;
				case PIN_MENU_MODE1:
					fnLGT_SetFlicker(KU_PIN_ATM_FLICKER, FLICKER_OFF); //#0143
					fnPIN_EntryEnable(PINMODE_NORMAL,			// nEnableMode
										0,						// nMinKeyCount
										0,						// nMaxKeyCount
										FALSE,					// bAutoEnd
										PIN_MENU_PAD1,			// szActiveKeys
										"",						// szTerminatorKeys
										"",						// szCardData
										K_WAIT);				// nTimeout
					break;
				case PIN_MENU_MODE2:
					fnLGT_SetFlicker(KU_PIN_ATM_FLICKER, FLICKER_OFF); //#0143
					fnPIN_EntryEnable(PINMODE_NORMAL,			// nEnableMode
										0,						// nMinKeyCount
										0,						// nMaxKeyCount
										FALSE,					// bAutoEnd
										PIN_ALL_TOUCH,			// szActiveKeys
										"",						// szTerminatorKeys
										"",						// szCardData
										K_WAIT);				// nTimeout
					break;

				case PIN_NUMERIC_MODE:
					fnLGT_SetFlicker(KU_PIN_ATM_FLICKER, FLICKER_OFF); 
					fnPIN_EntryEnable(PINMODE_NORMAL,		
										0,					
										0,				
										FALSE,				
										PIN_NUMERIC_PAD,	
										"",					
										"",					
										K_WAIT);			
					break;
				case PIN_PASSWORD_MODE:
					fnLGT_SetFlicker(KU_PIN_ATM_FLICKER, FLICKER_ON_CONTINUOUS); 
					fnPIN_EntryEnable(PINMODE_NORMAL,			
										(nPinPassWordMin >= 0) ? nPinPassWordMin : 0,
										(nPinPassWordMax >= 0) ? nPinPassWordMax : 0,
										nPinPassWordAuto,	
										PIN_PASSWORD_PAD,	
										szPinPassWordTerm,	
										szCardData,			
										K_WAIT);	
					break;
				case PIN_PASSWORD_MODE2:
					fnLGT_SetFlicker(KU_PIN_ATM_FLICKER, FLICKER_ON_CONTINUOUS); 
					fnPIN_EntryEnable(PINMODE_NORMAL,			
										(nPinPassWordMin >= 0) ? nPinPassWordMin : 0,
										(nPinPassWordMax >= 0) ? nPinPassWordMax : 0,
										nPinPassWordAuto,	
										PIN_PASSWORD_PAD2,	
										szPinPassWordTerm,	
										szCardData,			
										K_WAIT);
					break;
				case PIN_PASSWORD_MODE3:
					fnLGT_SetFlicker(KU_PIN_ATM_FLICKER, FLICKER_ON_CONTINUOUS);
					if(m_pProfile->DEVICE.MachineType == U3100K) //#N0174
					{

						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_PAD,	// szActiveKeys
											szPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_WAIT);	
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,			
											(nPinPassWordMin >= 0) ? nPinPassWordMin : 0,
											(nPinPassWordMax >= 0) ? nPinPassWordMax : 0,
											nPinPassWordAuto,	
											PIN_PASSWORD_PAD3,	
											szPinPassWordTerm,	
											szCardData,			
											K_WAIT);
					}
					break;
				case PIN_AMOUNT_MODE:
					fnLGT_SetFlicker(KU_PIN_ATM_FLICKER, FLICKER_OFF); //#0143
					fnPIN_EntryEnable(PINMODE_NORMAL,		
										0,					
										0,				
										FALSE,				
										PIN_AMOUNT_PAD,		
										"",					
										"",					
										K_WAIT);			
					break;
				case PIN_INPUT_MODE:
					fnLGT_SetFlicker(KU_PIN_ATM_FLICKER, FLICKER_OFF); //#0143
					fnPIN_EntryEnable(PINMODE_NORMAL,		
										0,					
										0,				
										FALSE,				
										PIN_INPUT_PAD,		
										"",					
										"",					
										K_WAIT);			
					break;
				case PIN_ALL_MODE:
					fnLGT_SetFlicker(KU_PIN_ATM_FLICKER, FLICKER_OFF); //#0143
					fnPIN_EntryEnable(PINMODE_NORMAL,		
										0,					
										0,				
										FALSE,				
										PIN_ALL_PAD,		
										"",					
										"",					
										K_WAIT);			
					break;
				case PIN_DUMMY_MODE:							// V03-01-01 DUMMY MODE추가처리
					fnLGT_SetFlicker(KU_PIN_ATM_FLICKER, FLICKER_OFF); //#0143
					fnPIN_EntryEnable(PINMODE_NORMAL,			// nEnableMode
										0,						// nMinKeyCount
										0,						// nMaxKeyCount
										FALSE,					// bAutoEnd
										PIN_CANCEL_PAD,			// szActiveKeys
										"",						// szTerminatorKeys
										"",						// szCardData
										K_WAIT);				// nTimeout
					break;

				default:
					break;
			}
		}
		else 
		{
			fnPIN_EntryDisable();
		}
	}
	if (nWaitFlag)
		return (fnAPL_CheckDeviceAction(nTempDevId));
	else 
		return TRUE;
}

// Device Set Flicker
int CDevCmn::fnAPL_DeviceSetFlicker(int nDevId, int nAction)
{

	return TRUE;
}
// 선행장치구하기
int CDevCmn::fnAPL_GetDeviceEvent(int nDevId)
{
//////////////////////////////////////////////////////////////////////////////////////
	static	int		nSaveDevId = FALSE;

if (nSaveDevId != nDevId)
{
	nSaveDevId = nDevId;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDeviceEvent(nDevid[%x]) ", nDevId);
}
	int TranDev = DEV_NONE;

	TranDev = fnCMN_ScanDeviceAction(nDevId, 0, EVENT_IN);
	if (nDevId & TranDev)
	{
		DeviceTran = TranDev;
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
//      FILE    D:\EJR\TXTMMDD\DD.TXT(1년치 자료가 환영방식으로 저장됨)
//              D:\EJR\IMGMMDD\DDEJRSEQNN0.JPG
//                             DDEJRSEQNN1.JPG(0-9:출금,입금)
//
//		RECORD	2048 BYTE(EjrTbl참조)
/////////////////////////////////////////////////////////////////////////////
// 전자저널시작
int CDevCmn::fnAPL_EjrSpoolPrintStart(int nDevId, EjrTbl* pEjrData, EjrAmountTbl* pEjrAmountData)
{
	memset(&Ejr, ' ', sizeof(Ejr));								// 전자저녈영역
	memset(&EjrAmount, 0, sizeof(EjrAmount));					// 전자저녈금액관련자료 : 2003.12.15

																// Key Area Setting
	if (pEjrData)												// 지정시복사
		memcpy(&Ejr, pEjrData, sizeof(Ejr));
	if (pEjrAmountData)											// 지정시복사 : 2003.12.15
		memcpy(&EjrAmount, pEjrAmountData, sizeof(EjrAmount));

	Ejr.Dummy[itemof(Ejr.Dummy) - 1] = '|';
	Ejr.Cr = A_CR;
	Ejr.Lf = A_LF;

	for (int i = 0; i < SprRowOption; i++)						// Data Area Setting
	{
		Ejr.Data[i][JPR_MAX_COL]		= '|';
		Ejr.Data[i][JPR_MAX_COL + 1]	= A_CR;
		Ejr.Data[i][JPR_MAX_COL + 2]	= A_LF;
	}

//	Ejr.DataDummy[itemof(Ejr.DataDummy) - 1] = '|';
	Ejr.DataDummyCr = A_CR;
	Ejr.DataDummyLf = A_LF;
	return TRUE;
}

// 전자저널
int CDevCmn::fnAPL_EjrSpoolData(int nDevId, int nLine, int nSize, LPCTSTR szSpoolData)
{
	
	if (nLine < 0)												// 라인보정
		nLine = 0;
	if (nLine >= SPR_MAX_ROW)
		nLine = SPR_MAX_ROW - 1;
	
	if (nSize <= 0)												// 길이구하기&보정
		nSize = strlen(szSpoolData);
	if (nSize > JPR_MAX_COL)
		nSize = JPR_MAX_COL;

	memcpy(Ejr.Data[nLine], GetString((LPSTR)szSpoolData, nSize, nSize).GetBuffer(0), nSize);
																// 자료보정
	return TRUE;
}

// 전자저널종료&저장및인자처리													
int CDevCmn::fnAPL_EjrSpoolEnd(int nDevId, int JnlType, int nWaitFlag)
{
	CString		strTempPrintData("");
	CString		strTempUnPackPrintData("");
	CString		strTempSprPrintData("");
	CString		strTempJprPrintData("");
	int			i = 0;
	int			SprRowOptionSave = 0;

	// Ejr저장
	if (nDevId & DEV_JPR)										// 저널자료존재시
	{
		fnAPL_EjrAddSerialNo();									// 전자저널일련번호증가
		fnAPL_EjrSave(JnlType);									// 전자저널저장

		if (m_nJournalMode == ATMS_JNL_HDD)
		{
//			if (m_pDevCmn->TranStatus != TRAN_ERROR)
			if (AtmStatus != ATM_ERROR)
				nDevId &= ~DEV_JPR;
		}
	}

	/*
	if (TranProc == TRAN_HIPASS)		//#N0266 명세표 인자 라인
	{
		SprRowOptionSave = SprRowOption;
		SprRowOption = SPR_MAX_ROW;
	}
*/
	
	// 인자자료작성
	for (i = 0; i < SprRowOption; i++)							// 자료작성
	{
		if (nDevId & DEV_SPR)									// 명세표인자자료작성
		{
			strTempPrintData = GetString(Ejr.Data[i], SPR_MAX_COL, SPR_MAX_COL);
			strTempPrintData.TrimRight();
			if (strTempPrintData.GetLength())
			{
				strTempUnPackPrintData = MakeUnPack(strTempPrintData.GetBuffer(0), strTempPrintData.GetLength());
				if (!strTempSprPrintData.GetLength())
					strTempSprPrintData = strTempUnPackPrintData;
				else
					strTempSprPrintData += "," + strTempUnPackPrintData;
			}
			else
			{
				if (!strTempSprPrintData.GetLength())
					strTempSprPrintData = "20";
				else
					strTempSprPrintData += ",20";
			}
		}
		
		if (nDevId & DEV_JPR)									// 저널인자자료작성
		{
			strTempPrintData = GetString(Ejr.Data[i], JPR_MAX_COL, JPR_MAX_COL);
			strTempPrintData.TrimRight();
			if (strTempPrintData.GetLength())
			{
				strTempUnPackPrintData = MakeUnPack(strTempPrintData.GetBuffer(0), strTempPrintData.GetLength());
				if (!strTempJprPrintData.GetLength())
					strTempJprPrintData = strTempUnPackPrintData;
				else
					strTempJprPrintData += "," + strTempUnPackPrintData;
			}
		}
	}
	

	// 인자처리
	if (nDevId & DEV_SPR)										// 명세표인자
	{
		if (!strTempSprPrintData.GetLength())					// 명세표자료무
			strTempSprPrintData = "20";
		if (fnAPL_GetAvailDevice(DEV_SPR))						// 사용가능한장치구하기
			fnSPR_Print(strTempSprPrintData);					// 인자(인자자료)
	}

	if (nDevId & DEV_JPR)										// 저널인자
	{
		if (!strTempJprPrintData.GetLength())					// 저널자료무
			strTempJprPrintData = "20";
		strTempJprPrintData += ",2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A";
																// '*'라인출력
		if (fnAPL_GetAvailDevice(DEV_JPR))						// 사용가능한장치구하기
			fnJPR_Print(strTempJprPrintData);					// 인자(인자자료)
	}

	/*
	if (TranProc == TRAN_HIPASS)		//#N0266
	{
		SprRowOption = SprRowOptionSave;
	}
*/
	if (nWaitFlag)
		return (fnAPL_CheckDeviceAction(nDevId));
	else 
		return TRUE;
}

// 전자저널일련번호증가
int CDevCmn::fnAPL_EjrAddSerialNo()
{
	char	szTempEjrSerialNo[256];										

	memcpy(szTempEjrSerialNo, m_pProfile->TRANS.EjrSerialNo, 8);

	if (memcmp(szTempEjrSerialNo, "99999999", 8) == 0)
		memset(szTempEjrSerialNo, '0', 8);
	AddString(szTempEjrSerialNo, "00000001", 8);

	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.EjrSerialNo, szTempEjrSerialNo, 8);
	m_pProfile->TRANS.EjrSerialNo[8] = NULL;
	m_pProfile->PutTransProfile();
	return TRUE;
}



// 전자저널저장
int CDevCmn::fnAPL_EjrSave(int JnlType)
{
	char		szTempFileName[256],  szYYMMDD[9], szHHMMSS[13], szDummy0[1024], szDummy1[1024], szDummy2[1024];
	CFileStatus	TempFileStatus;
	char		szKey[512];
	char		szEncBuffer[20480] = {'0',};
	FILE*		TempFp;
	SYSTEMTIME sTime;
	CString strFileName, strTime;
	int			nEnc = 0;
	int         nEncOutlen = 0;
	CString		strTrim("");

	GetLocalTime(&sTime);

	memcpy(&EEjrD, &Ejr, sizeof(EEjrD));
	EEjrD.Fill1 = '[';
	sprintf(szYYMMDD, "%2.2s/%2.2s/%2.2s", &GetDate().GetBuffer(0)[2], &GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6]);
	memcpy(EEjrD.Date, szYYMMDD, sizeof(EEjrD.Date));
	sprintf(szHHMMSS, "%2.2s:%2.2s:%2.2s.%03d", &GetTime().GetBuffer(0)[0], &GetTime().GetBuffer(0)[2], &GetTime().GetBuffer(0)[4], sTime.wMilliseconds);
	memcpy(EEjrD.Time, szHHMMSS, sizeof(EEjrD.Time));
	EEjrD.Fill3 = ']';	

	sprintf(szDummy0, "[%8.8s %12.12s] ", szYYMMDD, szHHMMSS);


	_mkdir(_EJR_DIR);											// Dir생성
	_mkdir(_EJR_TXT);
	sprintf(szTempFileName, "%s\\%8.8s.JNL", _EJR_TXT, m_pProfile->TRANS.YYYYMMDD);
	
	if(!CFile::GetStatus(szTempFileName, TempFileStatus))		// File존재시날자검증
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ECashEjrSpoolPrint() File Err T_OK");
	}

	TempFp = fopen(szTempFileName, "a");						
	if(TempFp == NULL)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ECashEjrSpoolPrint() File NULL Pointer T_OK");
		return T_OK;
	}

	nEnc = RegGetInt(_REGKEY_REMOTECTRL, "ECash_JNL_EncFlag", 0);
	sprintf(szKey, "%8.8s",	GetDate().GetBuffer(0));
//	nEncOutlen = enc(&EEjrD.Fill1, szEncBuffer, (sizeof(EEjrD) + 16) * 2, szKey, &m_pProfile->NETWORK.AtmSerialNum[4]);
//	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_HNetEjrSpoolPrint PlanText", &EEjrD.Fill1, sizeof(EEjrD));
//	HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_HNetEjrSpoolPrint EncrText", szEncBuffer, sizeof(EEjrD));

	memset(&EEjrD.Dummy, '=', 40);
	if(nEnc)
	{
//		sprintf(szDummy1, "%34.34s", EEjrD.Dummy);
//		nEncOutlen = enc(szDummy1, szEncBuffer, (strlen(szDummy1) + 16) * 2, szKey, &m_pProfile->NETWORK.AtmSerialNum[4]);
//		memcpy(szDummy2, szDummy0, strlen(szDummy0));
//		memcpy(&szDummy2[strlen(szDummy0)], szEncBuffer, nEncOutlen);
//		szDummy2[strlen(szDummy0) + nEncOutlen] = A_CR;
//		szDummy2[strlen(szDummy0) + nEncOutlen + 1] = A_LF;	
//		fwrite(szDummy2, strlen(szDummy0) + nEncOutlen + 2, 1, TempFp);
		sprintf(szDummy1, "%40.40s%c%c", EEjrD.Dummy, A_CR, A_LF);
		memcpy(szDummy2, szDummy0, strlen(szDummy0));
		memcpy(&szDummy2[strlen(szDummy0)], szDummy1, strlen(szDummy1));
//		szDummy2[strlen(szDummy0) + strlen(szDummy1)] = A_CR;
//		szDummy2[strlen(szDummy0) + strlen(szDummy1) + 1] = A_LF;	
		fwrite(szDummy2, strlen(szDummy0) + strlen(szDummy1), 1, TempFp);

	}
	else
	{
		sprintf(szDummy1, "%40.40s%c%c", EEjrD.Dummy, A_CR, A_LF);
		memcpy(szDummy2, szDummy0, strlen(szDummy0));
		memcpy(&szDummy2[strlen(szDummy0)], szDummy1, strlen(szDummy1));
// 		szDummy2[strlen(szDummy0) + strlen(szDummy1)] = A_CR;
// 		szDummy2[strlen(szDummy0) + strlen(szDummy1) + 1] = A_LF;	
		fwrite(szDummy2, strlen(szDummy0) + strlen(szDummy1), 1, TempFp);
	}	
	fclose(TempFp);

	TempFp = fopen(szTempFileName, "a");						
	if(TempFp == NULL)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ECashEjrSpoolPrint() File NULL Pointer T_OK2");
		return T_OK;
	}

	for (int j = 0; j < SprRowOption; j++)							// 자료작성
	{
		sprintf(szDummy1, "%40.40s%c%c", Ejr.Data[j], A_CR, A_LF);
//		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_HNetEjrSpoolPrint PlanText", szDummy1, strlen(szDummy1));

		if(memcmp(szDummy1, SPACE16, strlen(SPACE16)) == 0)
			continue;
		
		strTrim.Format("%s", szDummy1);
		strTrim.TrimLeft();
		sprintf(szDummy1, "%s", strTrim.GetBuffer(0));
		
		if(nEnc)
		{
/*
			nEncOutlen = enc(szDummy1, szEncBuffer, (strlen(szDummy1) + 16) * 2, szKey, &m_pProfile->NETWORK.AtmSerialNum[4]);
			memcpy(szDummy2, szDummy0, strlen(szDummy0));
			memcpy(&szDummy2[strlen(szDummy0)], szEncBuffer, nEncOutlen);
//#H0001
			szDummy2[strlen(szDummy0) + nEncOutlen] = A_CR;
			szDummy2[strlen(szDummy0) + nEncOutlen + 1] = A_LF;	
//			HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_HNetEjrSpoolPrint EncrText", szDummy1, strlen(szDummy0) + nEncOutlen + 2);
			fwrite(szDummy2, strlen(szDummy0) + nEncOutlen + 2, 1, TempFp);
*/
			memcpy(szDummy2, szDummy0, strlen(szDummy0));
			memcpy(&szDummy2[strlen(szDummy0)], szDummy1, strlen(szDummy1));
//			szDummy2[strlen(szDummy0) + strlen(szDummy1)] = A_CR;
//			szDummy2[strlen(szDummy0) + strlen(szDummy1) + 1] = A_LF;	
			fwrite(szDummy2, strlen(szDummy0) + strlen(szDummy1), 1, TempFp);

		}
		else
		{
			memcpy(szDummy2, szDummy0, strlen(szDummy0));
			memcpy(&szDummy2[strlen(szDummy0)], szDummy1, strlen(szDummy1));
//			szDummy2[strlen(szDummy0) + strlen(szDummy1)] = A_CR;
//			szDummy2[strlen(szDummy0) + strlen(szDummy1) + 1] = A_LF;	
			fwrite(szDummy2, strlen(szDummy0) + strlen(szDummy1), 1, TempFp);
		}
	}
	
	memset(&EEjrD.Dummy, '=', 40);
	if(nEnc)
	{
//		sprintf(szDummy1, "%34.34s", EEjrD.Dummy);
//		nEncOutlen = enc(szDummy1, szEncBuffer, (strlen(szDummy1) + 16) * 2, szKey, &m_pProfile->NETWORK.AtmSerialNum[4]);
//		memcpy(szDummy2, szDummy0, strlen(szDummy0));
//		memcpy(&szDummy2[strlen(szDummy0)], szEncBuffer, nEncOutlen);
//		szDummy2[strlen(szDummy0) + nEncOutlen] = A_CR;
//		szDummy2[strlen(szDummy0) + nEncOutlen + 1] = A_LF;	
//		fwrite(szDummy2, strlen(szDummy0) + nEncOutlen + 2, 1, TempFp);

		sprintf(szDummy1, "%40.40s%c%c", EEjrD.Dummy, A_CR, A_LF);
		memcpy(szDummy2, szDummy0, strlen(szDummy0));
		memcpy(&szDummy2[strlen(szDummy0)], szDummy1, strlen(szDummy1));
// 		szDummy2[strlen(szDummy0) + strlen(szDummy1)] = A_CR;
// 		szDummy2[strlen(szDummy0) + strlen(szDummy1) + 1] = A_LF;	
		fwrite(szDummy2, strlen(szDummy0) + strlen(szDummy1), 1, TempFp);

	}
	else
	{
		sprintf(szDummy1, "%40.40s%c%c", EEjrD.Dummy, A_CR, A_LF);
		memcpy(szDummy2, szDummy0, strlen(szDummy0));
		memcpy(&szDummy2[strlen(szDummy0)], szDummy1, strlen(szDummy1));
// 		szDummy2[strlen(szDummy0) + strlen(szDummy1)] = A_CR;
// 		szDummy2[strlen(szDummy0) + strlen(szDummy1) + 1] = A_LF;	
		fwrite(szDummy2, strlen(szDummy0) + strlen(szDummy1), 1, TempFp);
	}	

	
	fclose(TempFp);


/////////////////////////////////////////////////////////////////////////////
	CString		strTempDbData("");
	CString		strTemp("");
	CString		strTempPath("");							
	char		szTemp[256];
	int			i = 0;

	// 전자저널입력
	// OM_ELECJNL||
	strTempDbData = "OM_ELECJNL";

	// 입력일(YYYYMMDD)||
	strTemp.Format("%8.8s", Ejr.Date);
	strTempDbData += "||" + strTemp;

	// 입력시간(HHMMSS)||
	strTemp.Format("%6.6s", Ejr.Time);
	strTempDbData += "||" + strTemp;

	// 일련번호||
	strTemp.Format("%6.6s", m_pProfile->TRANS.SerialNo);
	strTempDbData += "||" + strTemp;

	// 거래종류(0-기타,1-지급,2-통장정리,3-이체,4-입금,5-조회,6-IC)||
	strTempDbData += "||0";

	// 매체종류(0-카드,1-통장,2-무매체)||
	strTempDbData += "||";

	// 자타행구분(0-자행,1-타행)||
	strTempDbData += "||";

	// 마감후구분(0-마감전,1-마감후)||
	strTempDbData += "||";

	// 취소거래여부(0-정상,1-취소)||
	strTempDbData += "||";

	// 송금은행코드||
	strTempDbData += "||";

	// 모점 코드||
	strTempDbData += "||";


	// 계좌번호||
	strTemp.Format("%20.20s", "00000000000000000000");
	strTempDbData += "||" + strTemp;

	// Host의거래날짜||
	strTemp.Format("%8.8s", Ejr.Date);
	strTempDbData += "||" + strTemp;

	// 불능코드 (호스트오류코드)||
	strTempDbData += "||";

	// 거래금액||
	strTemp.Format("%08d", 0);
	strTempDbData += "||" + strTemp;

	// Host Retrieval Number||
	strTempDbData += "||";

	// 수수료||
	strTempDbData += "||";

	// 수표매수||
	strTempDbData += "||";

	// 수표일련번호||
	strTempDbData += "||";

	// 만원권 매수||오천원권 매수||천원권 매수||
	strTempDbData += "||||||";
// 	strTempDbData += "||" + Int2Asc(EjrAmount.Cash10000Num);	
// 	strTempDbData += "||" + Int2Asc(EjrAmount.Cash5000Num);		
// 	strTempDbData += "||" + Int2Asc(EjrAmount.Cash1000Num);		

	// 자행수표(10만원권)장수||자행수표(30만원권)장수||
	// 자행수표(50만원권)장수||자행수표(100만원권)장수||
	// 타행수표(10만원권)장수||타행수표(30만원권)장수||
	// 타행수표(50만원권)장수||타행수표(100만원권)장수||
	strTempDbData += "||||||||||||||||";
//	strTempDbData += "||" + Int2Asc(EjrAmount.MotherCheck10Num);
//	strTempDbData += "||" + Int2Asc(EjrAmount.MotherCheck30Num);
//	strTempDbData += "||" + Int2Asc(EjrAmount.MotherCheck50Num);
//	strTempDbData += "||" + Int2Asc(EjrAmount.MotherCheck100Num);
//	strTempDbData += "||" + Int2Asc(EjrAmount.OtherCheck10Num);	
//	strTempDbData += "||" + Int2Asc(EjrAmount.OtherCheck30Num);	
//	strTempDbData += "||" + Int2Asc(EjrAmount.OtherCheck50Num);	
//	strTempDbData += "||" + Int2Asc(EjrAmount.OtherCheck100Num);
//	strTempDbData += "||" + Int2Asc(EjrAmount.MotherCheck10Num);
//	strTempDbData += "||" + Int2Asc(EjrAmount.MotherCheck30Num);
//	strTempDbData += "||" + Int2Asc(EjrAmount.MotherCheck50Num);
//	strTempDbData += "||" + Int2Asc(EjrAmount.MotherCheck100Num);
//	strTempDbData += "||" + Int2Asc(EjrAmount.OtherCheck10Num);	
//	strTempDbData += "||" + Int2Asc(EjrAmount.OtherCheck30Num);	
//	strTempDbData += "||" + Int2Asc(EjrAmount.OtherCheck50Num);	
//	strTempDbData += "||" + Int2Asc(EjrAmount.OtherCheck100Num);


	// 진행 카운트||장애코드||
	strTempDbData += "||||";
	
	// Capture Image1손저장경로||Capture Image1얼굴저장경로||
	// Capture Image2손저장경로||Capture Image2얼굴저장경로||
	// Capture Image3손저장경로||Capture Image3얼굴저장경로||
	// Capture Image4손저장경로||Capture Image4얼굴저장경로||
	// U8100 AP 변경내역 #02 - 카메라 동영상 지원
	// 수취동영상 경로지정 - 고객이 수취부분을 동영상으로 해달라고 했을시 처리가 되어 있어야 함
	// DB에 저장된 내용을 보고 차세대OM에서 PLAY시켜줌
	strTempPath = GetSprintf("%s%4.4s\\%2.2s%8.8s", _EJR_IMG, &m_pProfile->TRANS.YYYYMMDD[4], &m_pProfile->TRANS.YYYYMMDD[6], m_pProfile->TRANS.EjrSerialNo);
	for (i = 1; i <= 8; i++)
	{
		if ((m_pTranCmn->TranProc == TRAN_WITH)	||
			(m_pTranCmn->TranProc == TRAN_DEP))
			strTempDbData += "||" + GetSprintf("%s%1.1d.JPG", strTempPath, i);
		else
		{
			if ((i%2) == 0)
				strTempDbData += "||" + GetSprintf("%s%1.1d.JPG", strTempPath, i);
			else
				strTempDbData += "||";
		}
	}															

MsgDump(TRACE_PLUS_MODE, "Log", __FILE__, __LINE__, "fnAPL_EjrSave()::Ejr.TranMaterial[%8.8s]", Ejr.TranMaterial);

	strTemp.Format("");										

	strTempDbData += "||" + strTemp;

	// 명세표라인1||명세표라인2||명세표라인3||명세표라인4||명세표라인5||
	// 명세표라인6||명세표라인7||명세표라인8||명세표라인9||명세표라인10||
	// 명세표라인11||명세표라인12||명세표라인13||명세표라인14||명세표라인15||
	// 명세표라인16||명세표라인17||// 명세표라인18||명세표라인19||
	for (i = 0; i < 19; i++)
	{
		sprintf(szTemp, "%%%d.%ds", JPR_MAX_COL, JPR_MAX_COL);
		strTemp.Format(szTemp, Ejr.Data[i]);
		strTempDbData += "||" + strTemp;
	}


	// 키번호                //01234567890123456789
	strTemp.Format("%20.20s", "                    ");
	strTempDbData += "||" + strTemp;

	fnAPL_EjrDbManagerSend(strTempDbData);						// 전자저널자료전송
	
	return TRUE;
}

// 전자저널자료전송
int CDevCmn::fnAPL_EjrDbManagerSend(LPCTSTR szDbData)
{

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_EjrDbManagerSend(szDbData[%s])", szDbData);

	COPYDATASTRUCT Cds;

	Cds.dwData = 0;
	Cds.cbData = strlen(szDbData) + 1;
	Cds.lpData = (LPSTR)szDbData;
	
	HWND hWnd = ::FindWindow(NULL, "[Hyosung T1Atm] OMDEMON");

	if (hWnd)
		::PostMessage(hWnd, WM_COPYDATA, (WPARAM)m_hWnd, (LPARAM)&Cds);

	return TRUE;
}											
/***************************************************************************************************************************************************************************/

// 손촬영
// 3Camera, Preview지원, 텍스트출력
// Capture된 화면에 텍스트를 기입하도록 변경
int CDevCmn::fnAPL_CaptureHandSave(int nIndex, int nFirstFlag, LPCTSTR szDefault)			// U8100-AP변경
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureHandSave(nIndex[%d])", nIndex);
	
	CString	szEditData("");										// U8100-AP변경
	CString	strTemp(szDefault);									// V07-03-01-#05
	int nCaptureResult = FALSE;

	if (nFirstFlag)												// 첫저장시삭제처리
	{
		fnAPL_CaptureSaveEnd();									// 촬영종료&저장처리
		for (int i = 0; i < 10; i++)
			DeleteFile(GetSprintf("%s%1.1d.JPG", _EJR_CAP, i));	
	}
	

	if (fnCMR_GetHandCameraDeviceStatus() == NORMAL)
	{
		CaptureLastIndex = nIndex;								// 카메라마지막저장Index
		szEditData = GetSprintf("%s%1.1d", _EJR_CAP, nIndex);	// U8100-AP변경
		if (strTemp != "")										// V07-03-01-#05  U8100-AP변경-start
		{
			szEditData += ",";
			szEditData += strTemp;								// V07-03-01-#05
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("손  촬영 : Index %02d", nIndex));
		fnCMR_CaptureHand(szEditData);	// 손촬영(파일이름) 
						
HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_CaptureHandSave(szEditData)", szEditData.GetBuffer(0), szEditData.GetLength());
																// 손촬영(파일이름)
	}
	else
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("손  촬영 : 불가(비정상)"));
		MsgDump(MAC_TRACE_FREE_FILE("Log"), "▶ABNORMAL◀ fnAPL_CaptureHandSave::수취구카메라 비정상으로인한 촬영불가");
	}
	
	return nCaptureResult;
}

// 얼굴촬영
// 3Camera, Preview지원, 텍스트출력
// Capture된 화면에 텍스트를 기입하도록 변경
int CDevCmn::fnAPL_CaptureFaceSave(int nIndex, int nFirstFlag, LPCTSTR szDefault)		// U8100-AP변경
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureFaceSave(nIndex[%d])", nIndex);

	CString	szEditData("");										
	CString	strTemp(szDefault);								
	int nCaptureResult = FALSE;

	if (nFirstFlag)												// 첫저장시삭제처리
	{
		fnAPL_CaptureSaveEnd();									// 촬영종료&저장처리
		for (int i = 0; i < 10; i++)
			DeleteFile(GetSprintf("%s%1.1d.JPG", _EJR_CAP, i));	
	}
	
	if (fnCMR_GetFaceCameraDeviceStatus() == NORMAL)
	{
		CaptureLastIndex = nIndex;								// 카메라마지막저장Index
		szEditData = GetSprintf("%s%1.1d", _EJR_CAP, nIndex);	
		if (strTemp != "")										
		{
			szEditData += ",";
			szEditData += strTemp;								
		}

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("얼굴촬영 : Index %02d", nIndex));		
		fnCMR_CaptureFace(szEditData);							
HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_CaptureFaceSave(szEditData)", szEditData.GetBuffer(0), szEditData.GetLength());
	}
	else
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("얼굴촬영 : 불가(비정상)"));
		MsgDump(MAC_TRACE_FREE_FILE("Log"), "▶ABNORMAL◀ fnAPL_CaptureHandSave::수취구카메라 비정상으로인한 촬영불가");
	}

	return nCaptureResult;
}

// 후면촬영
// 3Camera, Preview지원, 텍스트출력
// Capture된 화면에 텍스트를 기입하도록 변경
int CDevCmn::fnAPL_CaptureRoomSave(int nIndex, int nFirstFlag, LPCTSTR szDefault)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureRoomSave(nIndex[%d])", nIndex);

	CString	szEditData("");
	CString	strTemp(szDefault);									
	int		ret = 0;
// 적용후 1차 수정 (U8100 AP 변경내역 #13)
// U8100이 아닌경우에는 후방카메라를 쓰지 않는다
	if (m_pDevCmn->AtmDefine.CashHandler != HBRM_SET)
		return TRUE;

	if (nFirstFlag)												// 첫저장시삭제처리
	{
		fnAPL_CaptureSaveEnd(TRUE);								// 촬영종료&저장처리
		for (int i = 0; i < 10; i++)
			DeleteFile(GetSprintf("%s%1.1d.JPG", _EJR_CAP, i));	
	}

	if (fnCMR_GetRoomCameraDeviceStatus() == NORMAL)			// 후방카메라 비정상인 경우는 카메라를 찍지 않는다
	{
		CaptureLastIndex = nIndex;								// 카메라마지막저장Index
		szEditData = GetSprintf("%s%1.1d", _EJR_CAP, nIndex);
		
		if (strTemp != "")										
		{
			szEditData += ",";
			szEditData += strTemp;							
		}

		ret = fnCMR_CaptureRoom(szEditData);					// 후면촬영(파일이름)
	}
	else
	{
MsgDump(TRACE_CODE_MODE, "Log",  __FILE__, __LINE__, "▶ABNORMAL◀ fnAPL_CaptureFaceSave::후방카메라 비정상으로인한 촬영불가");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureRoomSave() return[%d]", ret);

	return TRUE;
}


// 적용후 1차 수정  - 변경부분 발생
// 동영상촬영	U8100-VOD-start
int CDevCmn::fnAPL_CaptureVODSave(int nStartEndFlag, int nCaptureVODPart)
{
	CString strFileName("");
	CString strVodFile("");
	//	CString strRealVodFile("");
	CFileFind	finder;
	
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureVODSave() start");
	
	if(nStartEndFlag == VOD_START)
	{
		switch(nCaptureVODPart)
		{
		case VOD_FACE:
			RealVodFile.Format("%s%d.mp4", VOD_FACE_FILE, bVODIdx);
			strVodFile.Format("P,%s%d.mp4", VOD_FACE_FILE, !bVODIdx);
			VODFaceRecording = ON;
			fnCMR_CaptureFace(strVodFile);
			break;
			
		case VOD_HAND:
			RealVodFile.Format("%s%d.mp4", VOD_HAND_FILE, bVODIdx);
			strVodFile.Format("P,%s%d.mp4", VOD_HAND_FILE, !bVODIdx);
			VODHandRecording = ON;
			fnCMR_CaptureHand(strVodFile);
			break;
			
		case VOD_ROOM:
			RealVodFile.Format("%s%d.mp4", VOD_ROOM_FILE, bVODIdx);
			strVodFile.Format("P,%s%d.mp4", VOD_ROOM_FILE, !bVODIdx);
			VODRoomRecording = ON;
			fnCMR_CaptureRoom(strVodFile);
			break;
		}
	}
	else if(nStartEndFlag == VOD_END)
	{
		switch(nCaptureVODPart)
		{
		case VOD_FACE:
			RealVodFile.Format("%s%d.mp4", VOD_FACE_FILE, bVODIdx);
			strVodFile.Format("S,%s%d.mp4", VOD_FACE_FILE, !bVODIdx);
			bVODIdx = !bVODIdx;
			VODFaceRecording = OFF;
			fnCMR_CaptureFace(strVodFile);
			break;
			
		case VOD_HAND:
			RealVodFile.Format("%s%d.mp4", VOD_HAND_FILE, bVODIdx);
			strVodFile.Format("S,%s%d.mp4", VOD_HAND_FILE, !bVODIdx);
			bVODIdx = !bVODIdx;
			VODHandRecording = OFF;
			fnCMR_CaptureHand(strVodFile);
			break;
			
		case VOD_ROOM:
			RealVodFile.Format("%s%d.mp4", VOD_ROOM_FILE, bVODIdx);
			strVodFile.Format("S,%s%d.mp4", VOD_ROOM_FILE, !bVODIdx);
			bVODIdx = !bVODIdx;
			VODRoomRecording = OFF;
			fnCMR_CaptureRoom(strVodFile);
			break;
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureVODSave()[VodFile %s]", strVodFile);
	
	if (VOD_START == nStartEndFlag)
		return TRUE;
	else											
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureVODSave()[strRealVodFile %d]", finder.FindFile(RealVodFile));
		
		if (!finder.FindFile(RealVodFile))
			return TRUE;
	}
	
	return TRUE;
}

// 촬영종료&저장처리
int CDevCmn::fnAPL_CaptureSaveEnd(int nSaveFlag)
{
	CString		strFileName = _T("");

	if (m_pDevCmn->fnAPL_GetDefineDevice(DEV_CMR))				// 속도개선
		;
	else
		return TRUE;

	if (!CaptureLastIndex)										// 카메라마지막저장Index
		return TRUE;

	if (!nSaveFlag)												// 카메라마지막저장종료대기
	{
		if (!PathFileExists(GetSprintf("%s%1.1d.JPG", _EJR_CAP, CaptureLastIndex)))
			return TRUE;
	}

	ULONG  WiatTime = TimerSet(K_2_WAIT);						// 카메라마지막저장종료10초대기
	while (TRUE)
	{
		Delay_Msg();											// Msg & Task
		if (PathFileExists(GetSprintf("%s%1.1d.JPG", _EJR_CAP, CaptureLastIndex)))
			break;												// 카메라마지막저장종료
		if (CheckTimer(WiatTime))
			break;
	}	

	CaptureLastIndex = 0;										// 재진입방지

	_mkdir(_EJR_DIR);											// Dir생성
	_mkdir(GetSprintf("%s", _EJR_IMG));
	_mkdir(GetSprintf("%s\\%4.4s", _EJR_IMG, &m_pProfile->TRANS.YYYYMMDD[4]));
	for (int i = 0; i < 10; i++)								
	{															// 1년전자료삭제
		DeleteFile(GetSprintf("%s\\%4.4s\\%8.8s%8.8s_%6.6s_%02d.JPG", _EJR_IMG, &m_pProfile->TRANS.YYYYMMDD[4], &m_pProfile->NETWORK.AtmSerialNum[0], &m_pProfile->TRANS.YYYYMMDD, m_pProfile->TRANS.SerialNo, i));
																// 자료저장(Move)
		MoveFile(GetSprintf("%s%1.1d.JPG", _EJR_CAP, i), GetSprintf("%s\\%4.4s\\%8.8s%8.8s_%6.6s_%02d.JPG", _EJR_IMG, &m_pProfile->TRANS.YYYYMMDD[4], &m_pProfile->NETWORK.AtmSerialNum[0], &m_pProfile->TRANS.YYYYMMDD, m_pProfile->TRANS.SerialNo, i));
	}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureSaveEnd():return");
	return TRUE;
}

// #0041 Kim.Gi.Jin 2010.01.11
// Facetured when ATM is shocked
int CDevCmn::fnAPL_CaptureFaceAtmShocked(int nStep)
{
	char	szTempSerialNo[256];

	memset(szTempSerialNo, 0, sizeof(szTempSerialNo));
	memcpy(szTempSerialNo, m_pProfile->TRANS.SerialNo, 6);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureFaceAtmShocked(nIndex[%d])", nStep);
	if(fnCMR_GetFaceCameraDeviceStatus() == NORMAL)
	{
		_mkdir(_EJR_DIR);
		_mkdir(GetSprintf("%s%4.4s", _EJR_AUTHLOCK, &m_pProfile->TRANS.YYYYMMDD[0]));
		_mkdir(GetSprintf("%s%4.4s\\%4.4s", _EJR_AUTHLOCK, &m_pProfile->TRANS.YYYYMMDD[0], &m_pProfile->TRANS.YYYYMMDD[4]));

		fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s\\%8.8s%4.4s%4.4s_%6.6sStep%2.2d.JPG", 
			_EJR_AUTHLOCK,
			&m_pProfile->TRANS.YYYYMMDD[0], 									
			&m_pProfile->TRANS.YYYYMMDD[4], 
			&m_pProfile->NETWORK.AtmSerialNum[0],
			&m_pProfile->TRANS.YYYYMMDD[4], 
			GetTime().GetBuffer(0),
			szTempSerialNo,
			nStep));
	}

	return TRUE;
}

// Capture Face when personal authentification verify to open the lock control
int CDevCmn::fnAPL_CaptureFaceAuthLockDoorOpen(int nStep)
{
	char	szTempSerialNo[256];

	memset(szTempSerialNo, 0, sizeof(szTempSerialNo));
	memcpy(szTempSerialNo, m_pProfile->TRANS.SerialNo, 6);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureFaceAuthLockDoorOpen(nIndex[%d])", nStep);
	if(fnCMR_GetFaceCameraDeviceStatus() == NORMAL)
	{
		_mkdir(_EJR_DIR);
		_mkdir(GetSprintf("%s%4.4s", _EJR_AUTHLOCK, &m_pProfile->TRANS.YYYYMMDD[0]));
		_mkdir(GetSprintf("%s%4.4s\\%4.4s", _EJR_AUTHLOCK, &m_pProfile->TRANS.YYYYMMDD[0], &m_pProfile->TRANS.YYYYMMDD[4]));

		fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s\\%8.8s%4.4s%4.4s_%6.6sStep%2.2d.JPG", 
			_EJR_AUTHLOCK,
			&m_pProfile->TRANS.YYYYMMDD[0], 									
			&m_pProfile->TRANS.YYYYMMDD[4], 
			&m_pProfile->NETWORK.AtmSerialNum[0],
			&m_pProfile->TRANS.YYYYMMDD[4], 
			GetTime().GetBuffer(0),
			szTempSerialNo,
			nStep));
	}

	return TRUE;
}

int CDevCmn::fnAPL_CaptureFaceCustomDoorOpen(int nClearFlag)
{

	static	int		GetCaptureFaceCnt = 0;							
	static	int		GetCaptureWaitCnt = 0;							
	
	char	szTempSerialNo[256];

	memset(szTempSerialNo, 0, sizeof(szTempSerialNo));
	memcpy(szTempSerialNo, m_pProfile->TRANS.SerialNo, 6);


	if (nClearFlag)
	{
		GetCaptureWaitCnt = 0;
		GetCaptureFaceCnt = 0;
		return T_OK;
	}

	int	nDootStatus = fnDOR_GetDoorStatus();

	if(nDootStatus == DOOR_OPENED)
	{
		if ((GetCaptureWaitCnt > K_5_WAIT) && (GetCaptureFaceCnt < K_20_WAIT))	// K_10_WAIT마다 5초씩 지연됨
		{

			if(fnCMR_GetFaceCameraDeviceStatus() == NORMAL)
			{
				_mkdir(_EJR_DIR);
				_mkdir(GetSprintf("%s%4.4s", _EJR_AUTHLOCK, &m_pProfile->TRANS.YYYYMMDD[0]));
				_mkdir(GetSprintf("%s%4.4s\\%4.4s", _EJR_AUTHLOCK, &m_pProfile->TRANS.YYYYMMDD[0], &m_pProfile->TRANS.YYYYMMDD[4]));

				fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s\\%8.8s%4.4s%4.4s_%4.4sDOOR%2.2d.JPG", 
					_EJR_AUTHLOCK,
					&m_pProfile->TRANS.YYYYMMDD[0], 									
					&m_pProfile->TRANS.YYYYMMDD[4], 
					&m_pProfile->NETWORK.AtmSerialNum[0],
					&m_pProfile->TRANS.YYYYMMDD[4], 
					GetTime().GetBuffer(0),
					szTempSerialNo,
					GetCaptureFaceCnt));
			}

			GetCaptureWaitCnt = 0;
			GetCaptureFaceCnt ++;
		}

		GetCaptureWaitCnt ++;
		
	}

	return T_OK;
}
// 장애코드종류구하기 (출동등급)
char CDevCmn::fnAPL_GetErrorCodeGrade(LPCTSTR szHsErrorCode)
{
	int		nTempErrorCodeKind = 0;
	int		nTempCmpLength = 0;
	char    szTempErrorCodeKind = '4';
	if (m_pProfile->DEVICE.MachineType == U3100K)			
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetErrorCodeGrade([%s])", szHsErrorCode);
	
		for (int i = 1; i < itemof(ErrorConvTbl_U3100K); i++) 
		{
			if (memcmp(szHsErrorCode, ErrorConvTbl_U3100K[i].MTC, ErrorConvTbl_U3100K[i].CmpLength) == 0)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetErrorCodeGrade([%s] [%s])", szHsErrorCode, ErrorConvTbl_U3100K[i].MTC);
				if (ErrorConvTbl_U3100K[i].CmpLength >= nTempCmpLength)	// Length가크고,나중인것고름
				{
					nTempCmpLength = ErrorConvTbl_U3100K[i].CmpLength;
					szTempErrorCodeKind = ErrorConvTbl_U3100K[i].ES[0];	// 0:미정의,1:기기장애,2:고객미숙,3:운영장애,4:전송장애
				}
			}
		}
	}
	else
	if (m_pProfile->DEVICE.MachineType == U8100)			
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetErrorCodeGrade_U8100([%s])", szHsErrorCode);	
		for (int i = 1; i < itemof(ErrorConvTbl_U8100); i++) 
		{
			if (memcmp(szHsErrorCode, ErrorConvTbl_U8100[i].MTC, ErrorConvTbl_U8100[i].CmpLength) == 0)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetErrorCodeGrade([%s] [%s])", szHsErrorCode, ErrorConvTbl_U8100[i].MTC);
				if (ErrorConvTbl_U8100[i].CmpLength >= nTempCmpLength)	// Length가크고,나중인것고름
				{
					nTempCmpLength = ErrorConvTbl_U8100[i].CmpLength;
					szTempErrorCodeKind = ErrorConvTbl_U8100[i].ES[0];	// 0:미정의,1:기기장애,2:고객미숙,3:운영장애,4:전송장애
				}
			}
		}

	}
	else
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetErrorCodeGrade_U8100([%s])", szHsErrorCode);
	
		for (int i = 1; i < itemof(ErrorConvTbl_U3100K); i++) 
		{
			if (memcmp(szHsErrorCode, ErrorConvTbl_U3100K[i].MTC, ErrorConvTbl_U3100K[i].CmpLength) == 0)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetErrorCodeGrade([%s] [%s])", szHsErrorCode, ErrorConvTbl_U3100K[i].MTC);
				if (ErrorConvTbl_U3100K[i].CmpLength >= nTempCmpLength)	// Length가크고,나중인것고름
				{
					nTempCmpLength = ErrorConvTbl_U3100K[i].CmpLength;
					szTempErrorCodeKind = ErrorConvTbl_U3100K[i].ES[0];	// 0:미정의,1:기기장애,2:고객미숙,3:운영장애,4:전송장애
				}
			}
		}

	}

	return szTempErrorCodeKind;
}

// 장애코드종류구하기
int	CDevCmn::fnAPL_GetErrorCodeKind(LPCTSTR szHsErrorCode)
{
	int		nTempErrorCodeKind = 0;
	int		nTempCmpLength = 0;
	if (m_pProfile->DEVICE.MachineType == U8100)				// U8100-AP변경
	{
		for (int i = 1; i < itemof(ErrorConvTbl_U8100); i++) 
		{
			if (memcmp(szHsErrorCode, ErrorConvTbl_U8100[i].MTC, ErrorConvTbl_U8100[i].CmpLength) == 0)
			{
				if (ErrorConvTbl_U8100[i].CmpLength >= nTempCmpLength)	// Length가크고,나중인것고름
				{
					nTempCmpLength = ErrorConvTbl_U8100[i].CmpLength;
					nTempErrorCodeKind = ErrorConvTbl_U8100[i].Kind;	// 0:미정의,1:기기장애,2:고객미숙,3:운영장애,4:전송장애
				}
			}
		}
	}
	else
	if (m_pProfile->DEVICE.MachineType == U3100K)				// U8100-AP변경
	{
		for (int i = 1; i < itemof(ErrorConvTbl_U3100K); i++) 
		{
			if (memcmp(szHsErrorCode, ErrorConvTbl_U3100K[i].MTC, ErrorConvTbl_U3100K[i].CmpLength) == 0)
			{
				if (ErrorConvTbl_U3100K[i].CmpLength >= nTempCmpLength)	// Length가크고,나중인것고름
				{
					nTempCmpLength = ErrorConvTbl_U3100K[i].CmpLength;
					nTempErrorCodeKind = ErrorConvTbl_U3100K[i].Kind;	// 0:미정의,1:기기장애,2:고객미숙,3:운영장애,4:전송장애
				}
			}
		}
	}
	else
	{
		for (int i = 1; i < itemof(ErrorConvTbl); i++) 
		{
			if (memcmp(szHsErrorCode, ErrorConvTbl[i].MTC, ErrorConvTbl[i].CmpLength) == 0)
			{
				if (ErrorConvTbl[i].CmpLength >= nTempCmpLength)	// Length가크고,나중인것고름
				{
					nTempCmpLength = ErrorConvTbl[i].CmpLength;
					nTempErrorCodeKind = ErrorConvTbl[i].Kind;		// 0:미정의,1:기기장애,2:고객미숙,3:운영장애,4:전송장애
				}
			}
		}
	}

	if (nTempErrorCodeKind == 0)								// 미정의
		nTempErrorCodeKind = 1;									// 기기장애

	return nTempErrorCodeKind;
}

// LC구하기
CString	CDevCmn::fnAPL_GetLC(LPCTSTR szHsErrorCode, int nTranProc)
{
	CString		strTempLC("ZZ"); //#0102
	int			nTempCmpLength = 0;


	if (m_pProfile->DEVICE.MachineType == U8100)			
	{
		for (int i = 1; i < itemof(ErrorConvTbl_U8100); i++) 
		{
			if (memcmp(szHsErrorCode, ErrorConvTbl_U8100[i].MTC, ErrorConvTbl_U8100[i].CmpLength) == 0)
			{													// Length가크고,나중인것고름
				if (ErrorConvTbl_U8100[i].CmpLength >= nTempCmpLength)
				{
					nTempCmpLength = ErrorConvTbl_U8100[i].CmpLength;
					Strcpy(strTempLC, ErrorConvTbl_U8100[i].LC, 2);
				}
			}
		}
	}
	else
	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		for (int i = 1; i < itemof(ErrorConvTbl_U3100K); i++) 
		{
			if (memcmp(szHsErrorCode, ErrorConvTbl_U3100K[i].MTC, ErrorConvTbl_U3100K[i].CmpLength) == 0)
			{													// Length가크고,나중인것고름
				if (ErrorConvTbl_U3100K[i].CmpLength >= nTempCmpLength)
				{
					nTempCmpLength = ErrorConvTbl_U3100K[i].CmpLength;
					Strcpy(strTempLC, ErrorConvTbl_U3100K[i].LC, 2);
				}
			}
		}
	}
	else
	{
		for (int i = 1; i < itemof(ErrorConvTbl); i++) 
		{
			if (memcmp(szHsErrorCode, ErrorConvTbl[i].MTC, ErrorConvTbl[i].CmpLength) == 0)
			{
				if (ErrorConvTbl[i].CmpLength >= nTempCmpLength)	// Length가크고,나중인것고름
				{
					nTempCmpLength = ErrorConvTbl[i].CmpLength;
					Strcpy(strTempLC, ErrorConvTbl[i].LC, 2);
				}
			}
		}
	}

	if (nTranProc == TRAN_NULL)									// Tran거래처리 지정 무
		nTranProc = TranProc;									// Tran거래처리

	if (strTempLC == "?2")
	{
		if (nTranProc == TRAN_DEP)								// 입금
			strTempLC = "82";
		else													// 출금,잔류
			strTempLC = "42";
	}

	if (strTempLC == "?4")										// 수표부장애시 : 2003.12.24
	{
		if (nTranProc == TRAN_DEP)								// 입금
			strTempLC = "92";
		else													// 출금,잔류
			strTempLC = "44";
	}

	if (strTempLC == "??")										// 미정의코드초기화
		strTempLC = "00";

	return strTempLC;
}

// 계원초기화모드설정
int	CDevCmn::fnAPL_ClerkInitModeSet()
{
	CString			strOmInformation("");
	CStringArray	strOmInformationArray;
	CString			strTemp("");
	int				i = 0;

	strOmInformation = GetShareData("OM_Main_CD_VanIII_Status");				// Status Information Setting
	SplitString(strOmInformation, "||", strOmInformationArray);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInitModeSet:strOmInformation_OM_Main_CD_VanIII_Status 01[%s]", strOmInformation);

	if (strOmInformationArray.GetSize() >= 24)
		;
	else
	{
		strOmInformation = "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||";
		SplitString(strOmInformation, "||", strOmInformationArray);
	}

	strOmInformationArray[0] = "INIT";							// INIT Setting
	strOmInformationArray[4] = "ON";							// ON Setting
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInitModeSet:strOmInformation_OM_Main_CD_VanIII_Status 02[%s]", strOmInformation);
	strOmInformationArray[7] = "TYS";															// SCR BANK NAME Setting
	strOmInformationArray[8] = "U3100K";
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInitModeSet:strOmInformation_OM_Main_CD_VanIII_Status 03[%s]", strOmInformation);

	for (i = 0; i < strOmInformationArray.GetSize(); i++)
	{
		if (i == 0)
			strTemp = strOmInformationArray[i];
		else
			strTemp += "||" + strOmInformationArray[i];
	}

	SetShareData("OM_Main_CD_VanIII_Status", strTemp);
	SetShareData("OM_APSTATUS", "APINIT");						// OP_APSTATUS Clear
	SetShareData("OM_APSTATUS", "APINIT");					
	SetShareData("OM_APSTATUS", "APINIT");					
	SetShareData("OP_MODE", "AP");

	strOmInformation = GetShareData("OM_메인헤더1");			// 메인정보(메인헤더1)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInitModeSet:strOmInformation[%s]", strOmInformation);
	SplitString(strOmInformation, "||", strOmInformationArray);

	if (strOmInformationArray.GetSize() >= 17)					// 2003.12.22
	{
		strOmInformationArray[0] = "INIT";						// INIT설정
		strOmInformationArray[16] = "ON";						// ON설정	: 2003.12.22 Index수정(15->16)

		for (i = 0; i < strOmInformationArray.GetSize(); i++)
		{
			if (i == 0)
				strTemp = strOmInformationArray[i];
			else
				strTemp += "||" + strOmInformationArray[i];
		}
		SetShareData("OM_메인헤더1", strTemp);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInitModeSet:strTemp[%s]", strTemp);
	}


	SetShareData("OM_RESULT", OM_NORMAL);						// 초기기본응답처리
	return TRUE;
}

// 계원정보
int	CDevCmn::fnAPL_ClerkInformation(int nInitFlag)
{

		
	CString	strTemp("");
	static int nFirstFlag = TRUE;
	int		nTotal = 0;
	int		nOutCheckKind = CHECKKIND_UNKNOWN;				

/////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************/
	// 메인정보(U8100)
	// OM_메인계원_U8100		
	Om.Main_CD_VanIII_Status = "";
	// 0:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN)
	if (AtmStatus == ATM_INIT)
		Om.Main_CD_VanIII_Status += "INIT";
	else
	if (AtmStatus == ATM_ERROR)
		Om.Main_CD_VanIII_Status += "ERROR";
	else
	if (AtmStatus == ATM_CLERK)
		Om.Main_CD_VanIII_Status += "CLERK";
	else
	if (AtmStatus == ATM_READY)
		Om.Main_CD_VanIII_Status += "READY";
	else
	if (AtmStatus == ATM_CUSTOM)
		Om.Main_CD_VanIII_Status += "CUSTOM";
	else
	if (AtmStatus == ATM_DOWN)
		Om.Main_CD_VanIII_Status += "DOWN";
	else
	if (AtmStatus == ATM_TRAN)
		Om.Main_CD_VanIII_Status += "TRAN";
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation: AtmStatus0[%d]", AtmStatus);
		Om.Main_CD_VanIII_Status += "UNKNOWN";
	}

	// 1:OPEN STATUS(ON,OFF)
	if (OpenKey)
		Om.Main_CD_VanIII_Status += "||ON";
	else
		Om.Main_CD_VanIII_Status += "||OFF";


	// 2:SEND RECV STATUS(ON,OFF)
	Om.Main_CD_VanIII_Status += "||" + Om.SendRecvStatus;

	// 3:TRANSACTION STATUS(ON,OFF)
	Om.Main_CD_VanIII_Status += "||" + Om.OperationStatus;

	// 4:POWER STATUS(ON,OFF)
	if (fnAPL_CheckPowerStatus())
		Om.Main_CD_VanIII_Status += "||ON";
	else
		Om.Main_CD_VanIII_Status += "||OFF";

	// 5:AS PASSWORD
	strTemp.Format("%6.6s", m_pProfile->TRANS.AsPassWord);
	strTemp.TrimRight();
	Om.Main_CD_VanIII_Status += "||"+ strTemp;
	// 6:OP PASSWORD
	strTemp.Format("%6.6s", m_pProfile->TRANS.OpPassWord);
	strTemp.TrimRight();
	Om.Main_CD_VanIII_Status += "||"+ strTemp;
	// 7:SCR BANK NAME
	strTemp.Format("%3.3s", USERNAME);
	Om.Main_CD_VanIII_Status += "||"+ strTemp;
	// 8:MACHINE KIND(VANII, VANIII)
	switch (AtmDefine.MachineType)
	{
		case U3100K:
			Om.Main_CD_VanIII_Status += "||U3100K";
			break;
		case U8100:
			Om.Main_CD_VanIII_Status += "||U8100";
			break;
		default:
			Om.Main_CD_VanIII_Status += "||UNKNO";
			break;
	}

	// 9:OPERATOR SWITCH STATUS(NONE,SUPERVISOR,RUN,SUPERVISORCHANGED,RUNCHANGED)
	switch (fnSNS_GetOperatorSwitch())
	{
		case OPER_NONE:
			Om.Main_CD_VanIII_Status += "||NONE";
			break;
		case OPER_SUPERVISOR:
			Om.Main_CD_VanIII_Status += "||SUPERVISOR";
			break;
		case OPER_RUN:
			Om.Main_CD_VanIII_Status += "||RUN";
			break;
		case OPER_SUPERVISORCHANGED:
			Om.Main_CD_VanIII_Status += "||SUPERVISORCHANGED";
			break;
		case OPER_RUNCHANGED:
			Om.Main_CD_VanIII_Status += "||RUNCHANGED";
			break;
		default:
			Om.Main_CD_VanIII_Status += "||UNKNO"; 
			break;
	}

	// 10:FTP IP(NNNNNNNNNNNN)
	strTemp.Format("%3.3s%3.3s%3.3s%3.3s", 
									m_pProfile->NETWORK.BpIpAddress3,
									&m_pProfile->NETWORK.BpIpAddress3[4],
									&m_pProfile->NETWORK.BpIpAddress3[8],
									&m_pProfile->NETWORK.BpIpAddress3[12]);
	Om.Main_CD_VanIII_Status += "||"+ strTemp;

	// 11:FTP PORT(NNNNN)
	strTemp.Format("%5.5s", m_pProfile->NETWORK.BpPortNo3);
	Om.Main_CD_VanIII_Status += "||"+ strTemp;

	// 12:ATM SERIAL NUMBER(NNNNNNNN)
	switch (AtmDefine.MachineType)
	{
		case U3100K:
		case U8100:
			strTemp.Format("%6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
			break;
		default:
			strTemp.Format("%6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
			break;

	}
	Om.Main_CD_VanIII_Status += "||"+ strTemp;

	// 13:BRANCH NUMBER(NNNNNNNN)
	strTemp.Format("%6.6s", m_pProfile->NETWORK.BranchNum);
	Om.Main_CD_VanIII_Status += "||"+ strTemp;
	// 14:AP VERSION(V00-00-00)
	strTemp.Format("%9.9s", m_pProfile->TRANS.ApVersion);
	Om.Main_CD_VanIII_Status += "||"+ strTemp;
	// 15:TRANSACTION DATE(YYYYMMDD)
	strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
	Om.Main_CD_VanIII_Status += "||"+ strTemp;

	// OM_메인계원_VANIII		
	Om.Main_CD_VanIII_ModeStatus = "";
										
	// 0:기번(4자리)
	strTemp.Format("%6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
	Om.Main_CD_VanIII_ModeStatus += strTemp;

	// 1:APVERSION
	strTemp.Format("%9.9s", m_pProfile->TRANS.ApVersion);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;

	// 2:ATMSTATUS(INIT,ERROR,CLERK,READY,CUSTOM,TRAN)
	if (AtmStatus == ATM_INIT)
		Om.Main_CD_VanIII_ModeStatus += "||INIT";
	else
	if (AtmStatus == ATM_ERROR)
		Om.Main_CD_VanIII_ModeStatus += "||ERROR";
	else
	if (AtmStatus == ATM_CLERK)
		Om.Main_CD_VanIII_ModeStatus += "||CLERK";
	else
	if (AtmStatus == ATM_READY)
		Om.Main_CD_VanIII_ModeStatus += "||READY";
	else
	if (AtmStatus == ATM_CUSTOM)
		Om.Main_CD_VanIII_ModeStatus += "||CUSTOM";
	else
	if (AtmStatus == ATM_DOWN)
		Om.Main_CD_VanIII_ModeStatus += "||DOWN";
	else
	if (AtmStatus == ATM_TRAN)
		Om.Main_CD_VanIII_ModeStatus += "||TRAN";
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation: AtmStatus1[%d]", AtmStatus);
		Om.Main_CD_VanIII_ModeStatus += "||UNKNOWN";
	}


	// 3:통신상태(ONLINE,OFFLINE)
	if (fnAPL_CheckHostLine())
		Om.Main_CD_VanIII_ModeStatus += "||ONLINE";
	else
		Om.Main_CD_VanIII_ModeStatus += "||OFFLINE";

	// 4:상단부상태(닫힘,열림)
	if (CabinetStatus == DOOR_OPENED)
		Om.Main_CD_VanIII_ModeStatus += "||열림";
	else
		Om.Main_CD_VanIII_ModeStatus += "||닫힘";
	
	// 5:뒷문상태(닫힘,열림)
	if (DoorStatus == DOOR_OPENED)
		Om.Main_CD_VanIII_ModeStatus += "||열림";
	else
		Om.Main_CD_VanIII_ModeStatus += "||닫힘";

	// 6:CAMERA STATUS
	if (fnAPL_GetDefineDevice(DEV_CMR))					
	{
		if (fnCMR_GetDeviceStatus() == NORMAL)		
			Om.Main_CD_VanIII_ModeStatus += "||NORMAL";
		else
			Om.Main_CD_VanIII_ModeStatus += "||ABNORMAL";
	}
	else
		Om.Main_CD_VanIII_ModeStatus += "||ABNORMAL";


	// 7:SPR(NORMAL, ERROR)
	if (!fnAPL_GetErrorDevice(DEV_SPR))
	{
		if ((!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))		||		// 2005.04.19
			(m_pDevCmn->fnAPL_GetAvailErrorDevice(DEV_SPR))	||
			(m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)	||
			(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER))
			Om.Main_CD_VanIII_ModeStatus += "||ABNORMAL";
		else
			Om.Main_CD_VanIII_ModeStatus += "||NORMAL";
	}
	else
	if (DeviceAutoOff & DEV_SPR)						// 2005.02.28
		Om.Main_CD_VanIII_ModeStatus += "||AUTO-OFF";
	else
	if (m_pProfile->TRANS.DeviceExecOff & DEV_SPR)		// 2004.11.05
		Om.Main_CD_VanIII_ModeStatus += "||OFF";
	else
		Om.Main_CD_VanIII_ModeStatus += "||ERROR";


	// 8:CST1(MISSING, NORMAL, LOW, EMPTY, FULL) = #0064-KIM*
	strTemp.Empty();
	strTemp = RegGetStr(_REGKEY_BRMSTATUS, "RB1Status");
	if(strTemp == "OK")
		Om.Main_CD_VanIII_ModeStatus += "||OK";
	else
	if(strTemp == "FULL" || strTemp == "HIGH")
		Om.Main_CD_VanIII_ModeStatus += "||FULL";
	else
	if(strTemp == "MISSING")
		Om.Main_CD_VanIII_ModeStatus += "||MISSING";
	else
	if(strTemp == "FATAL")
		Om.Main_CD_VanIII_ModeStatus += "||FATAL";
	else
	if(strTemp == "EMPTY")
		Om.Main_CD_VanIII_ModeStatus += "||EMPTY";
	else
	if(strTemp == "LOW")
		Om.Main_CD_VanIII_ModeStatus += "||LOW";
	else
		Om.Main_CD_VanIII_ModeStatus += "||OK";

	// 9:CST2(MISSING, NORMAL, LOW, EMPTY, FULL) = #0064-KIM*
	strTemp.Empty();
	strTemp = RegGetStr(_REGKEY_BRMSTATUS, "RB2Status");
	if(strTemp == "OK")
		Om.Main_CD_VanIII_ModeStatus += "||OK";
	else
	if(strTemp == "FULL" || strTemp == "HIGH")
		Om.Main_CD_VanIII_ModeStatus += "||FULL";
	else
	if(strTemp == "MISSING")
		Om.Main_CD_VanIII_ModeStatus += "||MISSING";
	else
	if(strTemp == "FATAL")
		Om.Main_CD_VanIII_ModeStatus += "||FATAL";
	else
	if(strTemp == "EMPTY")
		Om.Main_CD_VanIII_ModeStatus += "||EMPTY";
	else
	if(strTemp == "LOW")
		Om.Main_CD_VanIII_ModeStatus += "||LOW";
	else
		Om.Main_CD_VanIII_ModeStatus += "||OK";
		
	// 10:CSH(NORMAL, ERROR)						
	if (!fnAPL_GetErrorDevice(DEV_CSH))
	{
		if (!fnAPL_GetDownDevice(DEV_CSH))			
		{
			if (CshLoc != NORMAL_POS)					
				Om.Main_CD_VanIII_ModeStatus += "||MISSING";
			else
			if (RejectCSTLoc != NORMAL_POS)
				Om.Main_CD_VanIII_ModeStatus += "||MISSING";
			else
			if (RejectCSTStatus == CST_FULL)
				Om.Main_CD_VanIII_ModeStatus += "||REJFULL";
			else
				Om.Main_CD_VanIII_ModeStatus += "||NORMAL"; 
		}
		else
			Om.Main_CD_VanIII_ModeStatus += "||ABNORMAL";
	}
	else
	if (DeviceAutoOff & DEV_CSH)						
		Om.Main_CD_VanIII_ModeStatus += "||AUTO-OFF";
	else
	if (m_pProfile->TRANS.DeviceExecOff & DEV_CSH)	
		Om.Main_CD_VanIII_ModeStatus += "||OFF";
	else
		Om.Main_CD_VanIII_ModeStatus += "||ERROR";



	// 11:MCU(NORMAL, ERROR)
	if (!fnAPL_GetErrorDevice(DEV_MCU))
	{
		if (!fnAPL_GetDownDevice(DEV_MCU))				// 2004.11.05
			Om.Main_CD_VanIII_ModeStatus += "||NORMAL";
		else
			Om.Main_CD_VanIII_ModeStatus += "||ABNORMAL";
	}
	else
	if (DeviceAutoOff & DEV_MCU)						// 2005.02.28
		Om.Main_CD_VanIII_ModeStatus += "||AUTO-OFF";
	else
	if (m_pProfile->TRANS.DeviceExecOff & DEV_MCU)		// 2004.11.05
		Om.Main_CD_VanIII_ModeStatus += "||OFF";
	else
		Om.Main_CD_VanIII_ModeStatus += "||ERROR";


	// 12:CST3(MISSING, NORMAL, LOW, EMPTY, FULL) = #0064-KIM*
	strTemp.Empty();
	strTemp = RegGetStr(_REGKEY_BRMSTATUS, "RB3Status");
	if(strTemp == "OK")
		Om.Main_CD_VanIII_ModeStatus += "||OK";
	else
	if(strTemp == "FULL" || strTemp == "HIGH")
		Om.Main_CD_VanIII_ModeStatus += "||FULL";
	else
	if(strTemp == "MISSING")
		Om.Main_CD_VanIII_ModeStatus += "||MISSING";
	else
	if(strTemp == "FATAL")
		Om.Main_CD_VanIII_ModeStatus += "||FATAL";
	else
	if(strTemp == "EMPTY")
		Om.Main_CD_VanIII_ModeStatus += "||EMPTY";
	else
	if(strTemp == "LOW")
		Om.Main_CD_VanIII_ModeStatus += "||LOW";
	else
		Om.Main_CD_VanIII_ModeStatus += "||OK";

	// 13:Reject(MISSING, NORMAL, LOW, EMPTY, FULL) = #0064-KIM*
	strTemp.Empty();
	strTemp = RegGetStr(_REGKEY_BRMSTATUS, "RJB1Status");
	if(strTemp == "OK")
		Om.Main_CD_VanIII_ModeStatus += "||OK";
	else
	if(strTemp == "FULL" || strTemp == "HIGH")
		Om.Main_CD_VanIII_ModeStatus += "||FULL";
	else
	if(strTemp == "MISSING")
		Om.Main_CD_VanIII_ModeStatus += "||MISSING";
	else
	if(strTemp == "FATAL")
		Om.Main_CD_VanIII_ModeStatus += "||FATAL";
	else
	if(strTemp == "EMPTY")
		Om.Main_CD_VanIII_ModeStatus += "||EMPTY";
	else
	if(strTemp == "LOW")
		Om.Main_CD_VanIII_ModeStatus += "||LOW";
	else
		Om.Main_CD_VanIII_ModeStatus += "||OK";

	//14:회수박스(FULL,HIGH,LOW,EMPTY,MISSING,FATAL,ETC) = #0064-KIM*
	strTemp.Empty();
	strTemp = RegGetStr(_REGKEY_BRMSTATUS, "FRJBStatus");
	if(strTemp == "OK")
		Om.Main_CD_VanIII_ModeStatus += "||OK";
	else
	if(strTemp == "FULL" || strTemp == "HIGH")
		Om.Main_CD_VanIII_ModeStatus += "||FULL";
	else
	if(strTemp == "MISSING")
		Om.Main_CD_VanIII_ModeStatus += "||MISSING";
	else
	if(strTemp == "FATAL")
		Om.Main_CD_VanIII_ModeStatus += "||FATAL";
	else
	if(strTemp == "EMPTY")
		Om.Main_CD_VanIII_ModeStatus += "||EMPTY";
	else
	if(strTemp == "LOW")
		Om.Main_CD_VanIII_ModeStatus += "||LOW";
	else
		Om.Main_CD_VanIII_ModeStatus += "||OK";
	
	// 15:SPR(MISSING, NORMAL, LOW, EMPTY)
	if (SlipHeadStatus != HEADER_NORMAL)
		Om.Main_CD_VanIII_ModeStatus += "||MISSING";
	else
	if (SlipStatus == SLIP_NORMAL)
		Om.Main_CD_VanIII_ModeStatus += "||NORMAL";
	else
	if (SlipStatus == SLIP_LOW_END)
		Om.Main_CD_VanIII_ModeStatus += "||LOW";
	else
	if (SlipStatus == SLIP_EMPTY_PAPER)
		Om.Main_CD_VanIII_ModeStatus += "||EMPTY";
	else
		Om.Main_CD_VanIII_ModeStatus += "||UNKNOWN";



	strTemp.Empty();
	// 16:Cur Cassette1 
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt"));
	Om.Main_CD_VanIII_ModeStatus += "||"+ strTemp;

	strTemp.Empty();
	// 17:Cur Cassette2 
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt"));	
	Om.Main_CD_VanIII_ModeStatus += "||"+ strTemp;

	strTemp.Empty();
	// 18:Before Cassette1 //#N0135
	if (IniGetInt(_DEVICE_INI,DEV_SEC, "ETCDevice10", 0) == 1)	
		strTemp.Format("%d", IniGetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K"));
	else
		strTemp.Format("%d", IniGetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "NEW10K"));
	Om.Main_CD_VanIII_ModeStatus += "||"+ strTemp;


	strTemp.Empty();
	// 19:Before Cassette2  ////#N0135
	strTemp.Format("%d",  IniGetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K"));
	Om.Main_CD_VanIII_ModeStatus += "||"+ strTemp;

	strTemp.Empty();
	// 20:BP1IP주소(NNN.NNN.NNN.NNN)
	strTemp.Format("%15.15s", m_pProfile->NETWORK.BpIpAddress);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	
	strTemp.Empty();
	// 21:BP1PORT(NNNNN)
	strTemp.Format("%5.5s", m_pProfile->NETWORK.BpPortNo);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;

	strTemp.Empty();
	// 22:Cur Cassette3     //#0064-KIM*
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt"));	
	Om.Main_CD_VanIII_ModeStatus += "||"+ strTemp;

	strTemp.Empty();
	// 23:Before Cassette3  //#N0135
	strTemp.Format("%d",  IniGetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K"));
	Om.Main_CD_VanIII_ModeStatus += "||"+ strTemp;

	// 24:ATM_MOBILE_COMPORT(NNNnnnNNNnnn)
	Om.Main_CD_VanIII_ModeStatus += "||" + IniGetStr("D:\\INI\\MOBILECFG.ini","MOBILE", "COM","2");

	// 25:유동_IP_ID(123456)=> ID+1139(CD코드))
	Om.Main_CD_VanIII_ModeStatus += "||" + IniGetStr("D:\\INI\\ATM_CFG.ini", ATM_CFG_SEC, "ATM_MOVE_ID", "ID0000");
	// 26:유동_IP_PWD(CD코드+CD코드)=> 1139+1139
	Om.Main_CD_VanIII_ModeStatus += "||" + IniGetStr("D:\\INI\\ATM_CFG.ini", ATM_CFG_SEC, "ATM_MOVE_PWD", "00000000");
	// 27:기기코드 16 Byte
	strTemp = IniGetStr("D:\\INI\\NETWORK.INI", "NETWORK", "MachineMakeNum", ZERO16);
	strTemp = strTemp.Mid(1,16);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;

	// 28:지문인식 Status (정상, 장애, 미사용)
	if (fnAPL_GetDefineDevice(DEV_FNG))
	{
		if(fnFNG_GetDeviceStatus() == NORMAL)
			Om.Main_CD_VanIII_ModeStatus += "||정상";
		else
			Om.Main_CD_VanIII_ModeStatus += "||장애";
	}
	else
		Om.Main_CD_VanIII_ModeStatus += "||미사용";

	// 29:RPC + 자금장치 (정상, 장애)
	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		if (fnLDCtrl_GetDeviceStatus() != NORMAL)
			Om.Main_CD_VanIII_ModeStatus += "||장애";
		else
			Om.Main_CD_VanIII_ModeStatus += "||정상";
	}
	else
		Om.Main_CD_VanIII_ModeStatus += "||미사용";
 
	// 30:충격장치 (정상, 장애, 미사용)
	if (fnAPL_GetDefineDevice(DEV_SPG))
	{
		if(fnSPACK_GetDeviceStatus() == TRUE)
			Om.Main_CD_VanIII_ModeStatus += "||정상";
		else
			Om.Main_CD_VanIII_ModeStatus += "||장애";
	}
	else
		Om.Main_CD_VanIII_ModeStatus += "||미사용";

	//#0024 Kim.G.J
	// 31:편의점 설정(EVERYDAY)
	Om.Main_CD_VanIII_ModeStatus += "||" + IniGetStr("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice11", "0");
	// 32:브랜드 설정
	int BrandJehyu = IniGetInt(_TRANS_INI, TRANS_SEC, "BrandJehyu", 0);
	if(BrandJehyu == NONGHYUPDAN)
	{
		Om.Main_CD_VanIII_ModeStatus += "||" + IniGetStr(_TRANS_INI, TRANS_SEC, "BrandJehyu", "0");
	}
	else
	{
		Om.Main_CD_VanIII_ModeStatus += "||" + IniGetStr("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice20", "0");
	}
	// 33:MCR Type
	Om.Main_CD_VanIII_ModeStatus += "||" + IniGetStr("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice18", "0");
	// 34 ~ 35 Tmp -> "#0075"
	int nGetSt = IniGetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice19");
	if(nGetSt == 1)
	{
		// 34:SPACK 외부센서1
		if(fnSPACK_GetExternalSensorStatus_1())
			Om.Main_CD_VanIII_ModeStatus += "||1";
		else
			Om.Main_CD_VanIII_ModeStatus += "||0";

		// 35:SPACK 외부센서2
		Om.Main_CD_VanIII_ModeStatus += "||2";
	}
	else
	if(nGetSt == 2)
	{
		// 34:SPACK 외부센서1
		Om.Main_CD_VanIII_ModeStatus += "||2";

		// 35:SPACK 외부센서2
		if(fnSPACK_GetExternalSensorStatus_2())
			Om.Main_CD_VanIII_ModeStatus += "||1";
		else
			Om.Main_CD_VanIII_ModeStatus += "||0";
	}
	else
	if(nGetSt == 3)
	{
		// 34:SPACK 외부센서1
		if(fnSPACK_GetExternalSensorStatus_1())
			Om.Main_CD_VanIII_ModeStatus += "||1";
		else
			Om.Main_CD_VanIII_ModeStatus += "||0";

		// 35:SPACK 외부센서2
		if(fnSPACK_GetExternalSensorStatus_2())
			Om.Main_CD_VanIII_ModeStatus += "||1";
		else
			Om.Main_CD_VanIII_ModeStatus += "||0";
	}
	else
	{
		Om.Main_CD_VanIII_ModeStatus += "||2";
		Om.Main_CD_VanIII_ModeStatus += "||2";

	}
	//36:장애코드 (BRM/CDU,MCR,SPR) 
	if(!fnAPL_CheckError())
	{
		strTemp.Empty();
		strTemp = GetSprintf("%5.5s(%2.2s)", CurErrBuff.ErrorCode, &CurErrBuff.ErrorCode[5]);
		Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	}
	else
		Om.Main_CD_VanIII_ModeStatus += "||         ";


	//37:RPC (Port 설정-> 0:미사용, 7:SBC B/D = 8, 9:Ennowell B/D = 10)  
	//IniGetStr("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice01", "0")
	if(m_pProfile->DEVICE.ETCDevice01 == RPCLOCK_SET1)
	{
		strTemp.Format("%d", RegGetInt("SOFTWARE\\ATM\\DEVINFO\\RDCTRL", "ComPort", 0));
		Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	}
	else
		Om.Main_CD_VanIII_ModeStatus += "||0";	

	//38:DES B/D (0: Unuse, 1:use)  #0122
	Om.Main_CD_VanIII_ModeStatus += "||0";	
		

	//39:AMSHU : 0, K-SPR : 1 #0122
	Om.Main_CD_VanIII_ModeStatus += "||1";	
	
	//40:Reserve
	Om.Main_CD_VanIII_ModeStatus += "||";

	
	//41:현금출금상태(가능,불가)
	if (TranAvail & (TRAN_WITH_CASH | TRAN_WITH_50TCASH))		
		Om.Main_CD_VanIII_ModeStatus += "||가능";
	else
		Om.Main_CD_VanIII_ModeStatus += "||불가";

	//42:현금입금상태(가능,불가)
	if (TranAvail & TRAN_DEP_CASH)
		Om.Main_CD_VanIII_ModeStatus += "||가능";
	else
		Om.Main_CD_VanIII_ModeStatus += "||불가";
	
	//43:Brm정위치(정상,탈착)
	if (BrmLoc == NORMAL_POS)
		Om.Main_CD_VanIII_ModeStatus += "||정상";
	else
		Om.Main_CD_VanIII_ModeStatus += "||탈착";

	//44:Brm계원카세트상태(정상,탈착)
	if (ClerkCSTLoc == NORMAL_POS)
		Om.Main_CD_VanIII_ModeStatus += "||정상";
	else
		Om.Main_CD_VanIII_ModeStatus += "||탈착";

	//45:현금카세트상태(정상,잔량소,요보충,잔량만)
	if (RecycleBoxStatus == CST_NORMAL)
		Om.Main_CD_VanIII_ModeStatus += "||정상";
	else
	if (RecycleBoxStatus == CST_NEAR)
		Om.Main_CD_VanIII_ModeStatus += "||잔량소";
	else
	if (RecycleBoxStatus == CST_EMPTY)
		Om.Main_CD_VanIII_ModeStatus += "||요보충";
	else
		Om.Main_CD_VanIII_ModeStatus += "||잔량만";

	//46:회수박스(FULL,HIGH,LOW,EMPTY,MISSING,FATAL,ETC)
	strTemp.Empty();
	strTemp = RegGetStr(_REGKEY_BRMSTATUS, "FRJBStatus");
	if(strTemp == "FULL" || strTemp == "HIGH")
		Om.Main_CD_VanIII_ModeStatus += "||잔량만";
	else
	if(strTemp == "MISSING")
		Om.Main_CD_VanIII_ModeStatus += "||탈착";
	else
	if(strTemp == "FATAL")
		Om.Main_CD_VanIII_ModeStatus += "||비정상";
	else
	if(strTemp == "NOVALUES")
		Om.Main_CD_VanIII_ModeStatus += "||비정상";
	else
	if(strTemp == "NOREFERENCE")
		Om.Main_CD_VanIII_ModeStatus += "||비정상";
	else
	if(strTemp == "MANIP")
		Om.Main_CD_VanIII_ModeStatus += "||비정상";
	else
		Om.Main_CD_VanIII_ModeStatus += "||정상";

	//47:Reject(정상,잔량만,탈착)
	if (RejectCSTLoc == ABNORMAL_POS)
		Om.Main_CD_VanIII_ModeStatus += "||탈착";
	else
	if ((RejectCSTStatus == CST_EMPTY)	||
		(RejectCSTStatus == CST_NORMAL))
		Om.Main_CD_VanIII_ModeStatus += "||정상";
	else
		Om.Main_CD_VanIII_ModeStatus += "||잔량만";
	
	//48:입금매수
	strTemp.Empty();	
	strTemp.Format("%d",  RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "TotalCnt"));
	Om.Main_CD_VanIII_ModeStatus += "||"+ strTemp;
	//49:회수매수
	strTemp.Empty();	
	strTemp.Format("%d",  RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt"));
	Om.Main_CD_VanIII_ModeStatus += "||"+ strTemp;
	//50:리젝트매수
	strTemp.Empty();	
	strTemp.Format("%d",  RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt"));
	Om.Main_CD_VanIII_ModeStatus += "||"+ strTemp;
	//51:현재 VOLUME
	strTemp.Empty();	
	strTemp.Format("%d",  m_pProfile->DEVICE.SoundVolume);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//52:명세표설정
	strTemp.Empty();	
	strTemp.Format("%d",  m_pProfile->DEVICE.SHUHandler);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//53:일련번호(6자리)
	strTemp.Empty();	
	strTemp.Format("%s",  m_pProfile->TRANS.SerialNo);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//54:서비스거래설정(1,2,3)
	strTemp.Empty();	
	strTemp.Format("%d",  m_pProfile->TRANS.TranService);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//55:수수료조회(0,1)
	strTemp.Empty();	
	strTemp.Format("%d",  m_pProfile->TRANS.TranInqFee);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//56:자행최대금액(30,50,70)
	strTemp.Empty();	
	strTemp.Format("%d",  m_pProfile->TRANS.JaMaxDispAmt);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//57:타행최대금액(30,50,70)
	strTemp.Empty();	
	strTemp.Format("%d",  m_pProfile->TRANS.TaMaxDispAmt);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//58:리부팅시간설정||000000
	strTemp.Empty();	
	strTemp.Format("%6.6s",  m_pProfile->TRANS.RebootHHMMSS);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	
	//59:현재 업무 버전 (AMS)
	strTemp.Empty();	
	strTemp.Format("%6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
	//strTemp.Format("%s",  "????????");
	strTemp.TrimRight();
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	
	strTemp.Empty();	
	strTemp.Format("%s",  RegGetStr(_REGKEY_BRMSTATUS, "ShutterStatus","CLOSED"));
	//60:Reserve
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//61:Reserve //#0024
	strTemp.Empty();	
	if (m_pProfile->DEVICE.PBHandler != NOT_SET)
	{
		if (PbmAvail & TRAN_PBM_OK)
			Om.Main_CD_VanIII_ModeStatus += "||가능";
		else
			Om.Main_CD_VanIII_ModeStatus += "||불가";
	}
	else
		Om.Main_CD_VanIII_ModeStatus += "||미사용";
	
	//62:PBM 종류
	strTemp.Empty();
	strTemp.Format("%d", m_pProfile->DEVICE.PBHandler);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	
	//63:해외카드 최대 금액
	strTemp.Empty();
	strTemp.Format("%d", IniGetInt(_TRANS_INI, TRANS_SEC, "ForeginMaxDispAmt", 30));
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//64:장애인ATM 사용 유/무
	strTemp.Empty();
	strTemp.Format("%d", m_pProfile->DEVICE.EarHandler);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	strTemp.Empty();
	strTemp.Format("%d", IniGetInt(_TRANS_INI, TRANS_SEC, "foreginBrand", 0));	
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//66:바이로봇 제어 설정(0/1)
	strTemp.Empty();
	strTemp.Format("%d", IniGetInt(_TRANS_INI, TRANS_SEC, "viRobotCtrl", 0));
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//67:리부팅 요일 설정(0000000 -> 일월화수목금토) - 1010000 이면 일 화 리붓함.
	Om.Main_CD_VanIII_ModeStatus += "||" + IniGetStr(_TRANS_INI, TRANS_SEC, "rebootDate", "0101010");
	//68:Reserve
	Om.Main_CD_VanIII_ModeStatus += "||";
	//69:Reserve
	Om.Main_CD_VanIII_ModeStatus += "||";
	//70:Reserve
	Om.Main_CD_VanIII_ModeStatus += "||";
	//71:Reserve
	Om.Main_CD_VanIII_ModeStatus += "||";

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
	// Main_CD_VanIII_ApSpEpVersion		
	Om.Main_CD_VanIII_ApSpEpVersion = "";
	// 0:AP VERSION(V00-00-00)
	strTemp.Format("%9.9s", m_pProfile->TRANS.ApVersion);
	Om.Main_CD_VanIII_ApSpEpVersion += strTemp;
	// 1:TRANSACTION DATE(YYYYMMDD)
	strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;
	// 2:CDU/BRM SP VERSION
	if (fnAPL_GetDefineDevice(DEV_CSH))					
		strTemp = RegGetStr(_REGKEY_SPVERSION, "BRM", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 3:JPR SP VERSION
	if (fnAPL_GetDefineDevice(DEV_JPR))					
		strTemp = RegGetStr(_REGKEY_SPVERSION, "JPR", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;


	// 4:SPR SP VERSION
	if (fnAPL_GetDefineDevice(DEV_SPR))					
		strTemp = RegGetStr(_REGKEY_SPVERSION, "SPR", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 5:MCU SP VERSION
	if (fnAPL_GetDefineDevice(DEV_MCU))				
		strTemp = RegGetStr(_REGKEY_SPVERSION, "IDC", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 6:PIN SP VERSION
	if (fnAPL_GetDefineDevice(DEV_PIN))					
		strTemp = RegGetStr(_REGKEY_SPVERSION, "PINPAD", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 7:CAM SP VERSION
	if (fnAPL_GetDefineDevice(DEV_CMR))				
		strTemp = RegGetStr(_REGKEY_SPVERSION, "CAM", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 8:SIU SP VERSION
	if (fnAPL_GetDefineDevice(DEV_SNS))					
		strTemp = RegGetStr(_REGKEY_SPVERSION, "SIU", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 9:UPS SP VERSION
	if (fnAPL_GetDefineDevice(DEV_UPS))					
		strTemp = RegGetStr(_REGKEY_SPVERSION, "UPS", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 10:DEV_SLO SP VERSION
	if (fnAPL_GetDefineDevice(DEV_RDCTRL))					
		strTemp = RegGetStr(_REGKEY_SPVERSION, "RDCTRL", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 11:DEV_FNG SP VERSION
	if (fnAPL_GetDefineDevice(DEV_FNG))					
		strTemp = RegGetStr(_REGKEY_SPVERSION, "FGR", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 12:MWI SP VERSION
	strTemp = RegGetStr(_REGKEY_MWI, "VERSION", "V00.00.00");
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;
/*
	// 12:DEV_SPG SP VERSION
	if (fnAPL_GetDefineDevice(DEV_SPG))
		strTemp = RegGetStr(_REGKEY_SPVERSION, "SPACK", "V00.00.00");
	else
		strTemp = "V00.00.00";
*/
//    Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 13:CDU/BRM EP VERSION
	if (fnAPL_GetDefineDevice(DEV_CSH))	 //#0095
	{
		if (m_pProfile->DEVICE.MachineType == U8100)
			strTemp = RegGetStr(_REGKEY_EPVERSION, "BRM_EP", "V00.00.00");
		else
		if (m_pProfile->DEVICE.MachineType == U3100K)	
			strTemp = RegGetStr(_REGKEY_EPVERSION, "BRM", "V00.00.00");
		else
			strTemp = RegGetStr(_REGKEY_EPVERSION, "BRM", "V00.00.00");
	}
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 14:JPR EP VERSION
	if (fnAPL_GetDefineDevice(DEV_JPR))					
		strTemp = RegGetStr(_REGKEY_EPVERSION, "JPR", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 15:SPR EP VERSION
	if (fnAPL_GetDefineDevice(DEV_SPR))					
		strTemp = RegGetStr(_REGKEY_EPVERSION, "SPR", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 16:MCU EP VERSION
	if (fnAPL_GetDefineDevice(DEV_MCU))					
		strTemp = RegGetStr(_REGKEY_EPVERSION, "IDC", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 17:PIN EP VERSION
	if (fnAPL_GetDefineDevice(DEV_PIN))				
		strTemp = RegGetStr(_REGKEY_EPVERSION, "PINPAD", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 18:CAM EP VERSION
	if (fnAPL_GetDefineDevice(DEV_CMR))					
		strTemp = RegGetStr(_REGKEY_EPVERSION, "CAM", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 19:SIU EP VERSION
	if (fnAPL_GetDefineDevice(DEV_SNS))					
		strTemp = RegGetStr(_REGKEY_EPVERSION, "SIU", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 20:UPS EP VERSION
	if (fnAPL_GetDefineDevice(DEV_UPS))					
		strTemp = RegGetStr(_REGKEY_EPVERSION, "UPS", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 21:RDCTRL EP VERSION
	if (fnAPL_GetDefineDevice(DEV_RDCTRL))					
		strTemp = RegGetStr(_REGKEY_EPVERSION, "RDCTRL", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 22:DEV_FNG EP VERSION
	if (fnAPL_GetDefineDevice(DEV_FNG))					
		strTemp = RegGetStr(_REGKEY_EPVERSION, "FGR", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 23:DEV_PBM EP VERSION
	if (fnAPL_GetDefineDevice(DEV_PBM))					
		strTemp = RegGetStr(_REGKEY_SPVERSION, "PBM", "V00.00.00");
	else
		strTemp = "V00.00.00";
	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;

	// 24:DEV_PBM EP VERSION
	if (fnAPL_GetDefineDevice(DEV_PBM))					
		strTemp = RegGetStr(_REGKEY_EPVERSION, "PBM", "V00.00.00");
	else
		strTemp = "V00.00.00";

	// 25:DEV_SPG EP VERSION
	if (fnAPL_GetDefineDevice(DEV_SPG))					
		strTemp = RegGetStr(_REGKEY_EPVERSION, "SPACK", "V00.00.00");
	else
		strTemp = "V00.00.00";
	
	// 26:DEV_PBM EP VERSION
	if (fnAPL_GetDefineDevice(DEV_PBM))					
		strTemp = RegGetStr(_REGKEY_EPVERSION, "PBM", "V00.00.00");
	else
		strTemp = "E00.00.00";

	Om.Main_CD_VanIII_ApSpEpVersion += "||" + strTemp;
	//27 ~ 30
	Om.Main_CD_VanIII_ApSpEpVersion += "||";
	Om.Main_CD_VanIII_ApSpEpVersion += "||";
	Om.Main_CD_VanIII_ApSpEpVersion += "||";
	Om.Main_CD_VanIII_ApSpEpVersion += "||";
	Om.Main_CD_VanIII_ApSpEpVersion += "||";
	Om.Main_CD_VanIII_ApSpEpVersion += "||";
	Om.Main_CD_VanIII_ApSpEpVersion += "||";

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
	// 메인정보(메인헤더1)
	// OM_메인헤더1			
	Om.Main_1LineStatus = "";	
	
	// 0:ATMSTATUS(INIT,ERROR,CLERK,READY,CUSTOM,TRAN)
	if (AtmStatus == ATM_INIT)
		Om.Main_1LineStatus += "INIT";
	else
	if (AtmStatus == ATM_ERROR)
		Om.Main_1LineStatus += "ERROR";
	else
	if (AtmStatus == ATM_CLERK)
		Om.Main_1LineStatus += "CLERK";
	else
	if (AtmStatus == ATM_READY)
		Om.Main_1LineStatus += "READY";
	else
	if (AtmStatus == ATM_CUSTOM)
		Om.Main_1LineStatus += "CUSTOM";
	else
	if (AtmStatus == ATM_DOWN)
		Om.Main_1LineStatus += "DOWN";
	else
	if (AtmStatus == ATM_TRAN)
		Om.Main_1LineStatus += "TRAN";
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AxisClerkInformation: AtmStatus3[%d]", AtmStatus);
		Om.Main_1LineStatus += "UNKNOWN";
	}

	// 1:현금출금상태(가능,불가)
	if (UserTranAvail & (TRAN_WITH_CASH | TRAN_WITH_50TCASH))		
		Om.Main_1LineStatus += "||가능";
	else
		Om.Main_1LineStatus += "||불가";

	// 2:현금입금상태(가능,불가)
	if (UserTranAvail & TRAN_DEP_CASH)
		Om.Main_1LineStatus += "||가능";
	else
		Om.Main_1LineStatus += "||불가";

	// 3:수표출금상태(가능,불가)
	if (UserTranAvail & TRAN_WITH_CHECK)
		Om.Main_1LineStatus += "||가능";
	else
		Om.Main_1LineStatus += "||불가";

	// 4:수표입금상태(가능,불가)
	if (UserTranAvail & TRAN_DEP_CHECK)
		Om.Main_1LineStatus += "||가능";
	else
		Om.Main_1LineStatus += "||불가";

	// 5:통장거래상태(가능,불가)
	if (UserTranAvail & TRAN_PBM_OK)
		Om.Main_1LineStatus += "||가능";
	else
		Om.Main_1LineStatus += "||불가";

	// 6:전자화폐상태(가능,불가)
	if (m_pProfile->DEVICE.ICCardDevice != NOT_SET)
		Om.Main_1LineStatus += "||가능";
	else
		Om.Main_1LineStatus += "||불가";

	// 7:해외거래상태(가능,불가)
	if (m_pProfile->DEVICE.DesHandler != NOT_SET)
		Om.Main_1LineStatus += "||가능";
	else
		Om.Main_1LineStatus += "||불가";

	// 8:지문거래(가능,불가)
	Om.Main_1LineStatus += "||";						

	// 9:적외선거래상태(가능,불가)
	if ((m_pProfile->DEVICE.IrdaHandler != NOT_SET)	&&
		(m_pProfile->DEVICE.RfdHandler == NOT_SET)	)
		Om.Main_1LineStatus += "||가능";
	else
		Om.Main_1LineStatus += "||불가";

	// 10:바코드거래상태(가능,불가)							
	if (m_pProfile->DEVICE.BarHandler != NOT_SET)
		Om.Main_1LineStatus += "||가능";
	else
		Om.Main_1LineStatus += "||불가";

	// 11:RF거래상태(가능,불가)								
	// (m_pProfile->DEVICE.RfdHandler != NOT_SET)
	// Om.Main_1LineStatus += "||가능";
	// else
	//	Om.Main_1LineStatus += "||불가";

	// 11:암호화거래상태(가능,불가)
	if (m_pProfile->DEVICE.SeedHandler != NOT_SET)
		Om.Main_1LineStatus += "||가능";
	else
		Om.Main_1LineStatus += "||불가";

	// 12:배터리충전량
	strTemp = RegGetStr(_REGKEY_UPSSTATUS, "BattCharge", "0");
	if (Asc2Int(strTemp) > 95)									
		strTemp.Format("%d", 100);
	else
		strTemp.Format("%d", Asc2Int(strTemp));
	Om.Main_1LineStatus += "||" + strTemp;

	// 13:배터리전원
	if (RegGetStr(_REGKEY_UPSSTATUS, "UPSPowering", "FALSE") == "FALSE")
		Om.Main_1LineStatus += "||정상";
	else
		Om.Main_1LineStatus += "||단전";

	// 14:통신상태(ONLINE,OFFLINE)
	if (fnAPL_CheckHostLine())
		Om.Main_1LineStatus += "||ONLINE";
	else
		Om.Main_1LineStatus += "||OFFLINE";
	
	// 15:뒷문상태(닫힘,열림)
	if (DoorStatus == DOOR_OPENED)
		Om.Main_1LineStatus += "||열림";
	else
		Om.Main_1LineStatus += "||닫힘";

	// 16:계원스위치상태(ON,OFF)
	if (HBRM_SET == AtmDefine.CashHandler)		
	{																					
		if (fnUPS_GetPowerStatus() & (EXT_AC_OFF | INT_BATTERY_LOW | CLERK_POWER_OFF | POWER_ABNORMAL))
			Om.Main_1LineStatus += "||OFF";
		else
			Om.Main_1LineStatus += "||ON";
	}
	else
	{
		if (!fnAPL_CheckPowerStatus())
			Om.Main_1LineStatus += "||OFF";
		else
			Om.Main_1LineStatus += "||ON";
	}
	// 18:Kcash상태(가능,불가)									// 
//	if (m_pProfile->DEVICE.KcashHandler == KCASH_SET)
//		Om.Main_1LineStatus += "||가능";
//	else
//		Om.Main_1LineStatus += "||불가";

// 장애우ATM 관련 변경사항
// OM에 전맹인모드 가능여부를 전달								// 부가장치설정 14로 이동
//	if (m_pProfile->DEVICE.EarHandler == EAR_SET)
//		Om.Main_1LineStatus += "||가능";
//	else
//		Om.Main_1LineStatus += "||불가";

// 장애우ATM 관련 변경사항										// 거래정보설정 9로 이동
// OM에 화면확대 모드 가능여부를 전달
//	if (m_pProfile->TRANS.ZoomFlag == ZOOM_SET)
//		Om.Main_1LineStatus += "||가능";
//	else
//		Om.Main_1LineStatus += "||불가";

/////////////////////////////////////////////////////////////////////////////
	// 메인정보(메인헤더2)
	// OM_메인헤더2
	Om.Main_2LineStatus = "";			

	// 0:점번2(5자리)
	strTemp.Format("%5.5s", &m_pProfile->NETWORK.SubBranchNum[3]);
	Om.Main_2LineStatus += strTemp;
										
	// 1:기번(8자리)
	strTemp.Format("%6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
	Om.Main_2LineStatus += "||" + strTemp;


	// 2:통신종류(IOC,HDLC,SWP,SDLC,TCP/IP,CCL,BAS)
	if (m_pProfile->NETWORK.Interface == NET_IOC)
		Om.Main_2LineStatus += "||IOC";
	else
	if (m_pProfile->NETWORK.Interface == NET_HDLC)
		Om.Main_2LineStatus += "||HDLC";
	else
	if (m_pProfile->NETWORK.Interface == NET_SWP)
		Om.Main_2LineStatus += "||SWP";
	else
	if (m_pProfile->NETWORK.Interface == NET_SDLC)
		Om.Main_2LineStatus += "||SDLC";
	else
	if (m_pProfile->NETWORK.Interface == NET_TCPIP)
		Om.Main_2LineStatus += "||TCP/IP";
	else
	if (m_pProfile->NETWORK.Interface == NET_CCL)
		Om.Main_2LineStatus += "||CCL";
	else
		Om.Main_2LineStatus += "||BAS";
										
	// 3:APVERSION(ATM020000T)
	strTemp.Format("%9.9s", &m_pProfile->TRANS.ApVersion);
	Om.Main_2LineStatus += "||" + strTemp;

	// 4:LOCALMODE(ON,OFF)
	if (HostLineMode == HOST_LOCAL_MODE)
		Om.Main_2LineStatus += "||ON";
	else
		Om.Main_2LineStatus += "||OFF";
	
	// 5:진위권모드(진권,위권) : 
//////////////////////////////////////////////////////////////////////////

	if (HBRM_SET == AtmDefine.CashHandler)
	{
		if ("TEST_CASH" == fstrBRM_GetBCDistinctionMode())
			BrmCashMode = BRM_TESTCASH_MODE;
		else
			BrmCashMode = BRM_REALCASH_MODE;
	}
//////////////////////////////////////////////////////////////////////////
	if (BrmCashMode == BRM_TESTCASH_MODE)
		Om.Main_2LineStatus += "||위권";
	else
		Om.Main_2LineStatus += "||진권";

	// 6:진행카운터(' ','0','1','3','5','6','9')
	Om.Main_2LineStatus += "||" + Om.ProcCount;
										
	// 7:장애코드(1234567)
	Om.Main_2LineStatus += "||" + Om.ErrorCode;

	// 8:통장/A4 구분
	if ((OKI_SET == AtmDefine.CashHandler) || (HBRM_SET == AtmDefine.CashHandler))
		Om.Main_2LineStatus += "||통장";							

/////////////////////////////////////////////////////////////////////////////
	// 메인정보(메인현금보유현황) 
	// OM_메인현금보유현황
	Om.Main_CashStatus = "";			
	
	// 0:카세트1지폐수
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt"));
	Om.Main_CashStatus += strTemp;

	// 1:카세트2지폐수
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt"));
	Om.Main_CashStatus += "||" + strTemp;

	// 2:카세트3지폐수
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt"));
	Om.Main_CashStatus += "||" + strTemp;

	// 3:카세트4지폐수 (for OKI BRM)
	if (HBRM_SET == AtmDefine.CashHandler)
	{
		strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt"));
		Om.Main_CashStatus += "||" + strTemp;
	}

/////////////////////////////////////////////////////////////////////////////
	// 메인정보(메인잔량정보)
	// OM_메인잔량정보
	Om.Main_Status = "";

	// 0:명세표잔량상태(정상,잔량소,요보충)
	if (SlipStatus == SLIP_NORMAL)
		Om.Main_Status += "정상";
	else
	if (SlipStatus == SLIP_LOW_END)
		Om.Main_Status += "잔량소";
	else
		Om.Main_Status += "요보충";

	// 1:저널잔량상태(정상,잔량소,요보충)
	if (JnlStatus == JNL_NORMAL)
		Om.Main_Status += "||정상";
	else
	if (JnlStatus == JNL_LOW_END)
		Om.Main_Status += "||잔량소";
	else
		Om.Main_Status += "||요보충";
										
	// 2:출금수표잔량상태(정상,잔량소,요보충)
	if (CheckCSTStatus == CST_NORMAL)
		Om.Main_Status += "||정상";
	else
	if (CheckCSTStatus == CST_NEAR)
		Om.Main_Status += "||잔량소";
	else
		Om.Main_Status += "||요보충";

	// 3:입금수표잔량상태(정상,잔량만)
	if (CassetteStatus == CST_NORMAL)
		Om.Main_Status += "||정상";
	else
		Om.Main_Status += "||잔량만";

	// 4:현금잔량상태(정상,잔량소,요보충,잔량만)
	if (Withdraw50TFlag)
	{
		if ((RecycleBoxStatus == CST_EMPTY)		||
			(RecycleBox50000Status == CST_EMPTY))
			Om.Main_Status += "||요보충";
		else
		if ((RecycleBoxStatus == CST_NEAR)		||
			(RecycleBox50000Status == CST_NEAR)	)
			Om.Main_Status += "||잔량소";
		else
		if ((RecycleBoxStatus == CST_NORMAL)	||
			(RecycleBox50000Status == CST_NORMAL))
			Om.Main_Status += "||정상";
		else
			Om.Main_Status += "||잔량만";
	}
	else
	{
		if (RecycleBoxStatus == CST_NORMAL)
			Om.Main_Status += "||정상";
		else
		if (RecycleBoxStatus == CST_NEAR)
			Om.Main_Status += "||잔량소";
		else
		if (RecycleBoxStatus == CST_EMPTY)
			Om.Main_Status += "||요보충";
		else
			Om.Main_Status += "||잔량만";
	}
/////////////////////////////////////////////////////////////////////////////
	// 메인정보(메인수표보유현황)
	// OM_메인수표보유현황
	Om.Main_CheckStatus = "";			
	
	// 0:출금수표매수
	strTemp.Format("%d", fnAPL_GetNumberOfOutCheck());
	Om.Main_CheckStatus += strTemp;

	// 1:입금수표매수
	strTemp.Format("%d", fnAPL_GetNumberOfInCheck());
	Om.Main_CheckStatus += "||" + strTemp;

/////////////////////////////////////////////////////////////////////////////
	// 메인정보(메인현금회수현황) 
	// OM_메인현금회수현황
	Om.Main_CashRejectStatus = "";			
	if (HBRM_SET == AtmDefine.CashHandler)
	{
		// 0:분실회수함매수(분실,잔류)
		strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt"));
		Om.Main_CashRejectStatus += strTemp;
	
		// 1:하단1회수함매수(출금불량지폐,입금불량지폐,오천원,천원)
		strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt"));
		Om.Main_CashRejectStatus += "||" + strTemp;

		// 2:하단2회수함매수(출금불량지폐,입금불량지폐,오천원,천원) for OKI BRM
		strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RJB2NoteCnt"));
		Om.Main_CashRejectStatus += "||" + strTemp;

		// 3:계원회수함매수(보충/회수시 이상지폐) for OKI BRM
		strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "ClerkRJBNoteCnt"));
		Om.Main_CashRejectStatus += "||" + strTemp;
	}
	else
	{
		// 0:상단회수함매수(분실,잔류)
		//	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt"));
		strTemp.Format("%d", IniGetInt(_SP_DEVICE_INI, "BRM", "StackOfRJB1"));
		Om.Main_CashRejectStatus += strTemp;
	
		// 1:하단회수함매수(출금불량지폐,입금불량지폐,오천원,천원)
		//	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RJB2NoteCnt"));
		strTemp.Format("%d", IniGetInt(_SP_DEVICE_INI, "BRM", "StackOfRJB2"));
		Om.Main_CashRejectStatus += "||" + strTemp;
	
	}
/////////////////////////////////////////////////////////////////////////////
	// 메인정보(메인장치현황)
	// OM_메인장치현황
	Om.Main_DeviceStatus = "";
										
	// 0:Mcu상태(정상,장애)
	Om.Main_DeviceStatus += Om.McuDeviceStatus;

	// 1:Mcu장애진행카운터(정상='0',' ','0','1','3','5','6','9')
	Om.Main_DeviceStatus += "||" + Om.McuProcCount;
	
	// 2:Mcu장애코드(정상="0000000",장애="1234567")
	Om.Main_DeviceStatus += "||" + Om.McuErrorCode;
	
	// 3:Spr상태
	Om.Main_DeviceStatus += "||" + Om.SprDeviceStatus;
	
	// 4:Spr장애진행카운터
	Om.Main_DeviceStatus += "||" + Om.SprProcCount;
	
	// 5:Spr장애코드
	Om.Main_DeviceStatus += "||" + Om.SprErrorCode;
	
	// 6:Spr현재명세표잔량상태(정상,잔량소,요보충)
	strTemp = RegGetStr(_REGKEY_SPRSTATUS, "RcptPaperStat");
	if (!strTemp.CompareNoCase("FULL"))
		Om.Main_DeviceStatus += "||정상";
	else
	if (!strTemp.CompareNoCase("LOW"))
		Om.Main_DeviceStatus += "||잔량소";
	else
		Om.Main_DeviceStatus += "||요보충";
	
	// 7:Spr예비명세표잔량상태(유,무)
	if (RegGetInt(_REGKEY_SPRSTATUS, "SpareRcptStat"))
		Om.Main_DeviceStatus += "||유";
	else
		Om.Main_DeviceStatus += "||무";
	
	// 8:Jpr상태
	Om.Main_DeviceStatus += "||" + Om.JprDeviceStatus;
	
	// 9:Jpr장애진행카운터
	Om.Main_DeviceStatus += "||" + Om.JprProcCount;
	
	// 10:Jpr장애코드
	Om.Main_DeviceStatus += "||" + Om.JprErrorCode;
	
	// 11:Jpr저널1잔량상태(정상,잔량소,요보충)
	strTemp = RegGetStr(_REGKEY_JPRSTATUS, "JnlPaper1Stat");
	if (!strTemp.CompareNoCase("FULL"))
		Om.Main_DeviceStatus += "||정상";
	else
	if (!strTemp.CompareNoCase("LOW"))
		Om.Main_DeviceStatus += "||잔량소";
	else
		Om.Main_DeviceStatus += "||요보충";
	
	// 12:Jpr저널2잔량상태(정상,잔량소,요보충)
	strTemp = RegGetStr(_REGKEY_JPRSTATUS, "JnlPaper2Stat");
	if (!strTemp.CompareNoCase("FULL"))
		Om.Main_DeviceStatus += "||정상";
	else
	if (!strTemp.CompareNoCase("LOW"))
		Om.Main_DeviceStatus += "||잔량소";
	else
		Om.Main_DeviceStatus += "||요보충";
	
	// 13:Pbm상태
	Om.Main_DeviceStatus += "||" + Om.PbmDeviceStatus;
	
	// 14:Pbm장애진행카운터
	Om.Main_DeviceStatus += "||" + Om.PbmProcCount;
	
	// 15:Pbm장애코드
	Om.Main_DeviceStatus += "||" + Om.PbmErrorCode;	

	// 16:Ucm상태
	Om.Main_DeviceStatus += "||" + Om.UcmDeviceStatus;
	
	// 17:Ucm장애진행카운터
	Om.Main_DeviceStatus += "||" + Om.UcmProcCount;
	
	// 18:Ucm장애코드
	Om.Main_DeviceStatus += "||" + Om.UcmErrorCode;
	
	// 19:Ucm정위치(정상,탈착)
	if (UcmLoc == NORMAL_POS)
		Om.Main_DeviceStatus += "||정상";
	else
		Om.Main_DeviceStatus += "||탈착";

	// 20:Ucm출금수표잔량상태(정상,잔량소,요보충)
	if (CheckCSTStatus == CST_NORMAL)
		Om.Main_DeviceStatus += "||정상";
	else
	if (CheckCSTStatus == CST_NEAR)
		Om.Main_DeviceStatus += "||잔량소";
	else
		Om.Main_DeviceStatus += "||요보충";
	
	// 21:Ucm입금수표잔량상태(정상,잔량만)
	if (CassetteStatus == CST_NORMAL)
		Om.Main_DeviceStatus += "||정상";
	else
		Om.Main_DeviceStatus += "||잔량만";
	
	// 22:Brm상태
	Om.Main_DeviceStatus += "||" + Om.BrmDeviceStatus;
	
	// 23:Brm장애진행카운터
	Om.Main_DeviceStatus += "||" + Om.BrmProcCount;
	
	// 24:Brm장애코드
	Om.Main_DeviceStatus += "||" + Om.BrmErrorCode;
	
	// 25:Brm정위치(정상,탈착)
	if (BrmLoc == NORMAL_POS)
		Om.Main_DeviceStatus += "||정상";
	else
		Om.Main_DeviceStatus += "||탈착";
	
	// 26:Brm현금카세트상태(정상,잔량소,요보충,잔량만)
	if (RecycleBoxStatus == CST_NORMAL)
		Om.Main_DeviceStatus += "||정상";
	else
	if (RecycleBoxStatus == CST_NEAR)
		Om.Main_DeviceStatus += "||잔량소";
	else
	if (RecycleBoxStatus == CST_EMPTY)
		Om.Main_DeviceStatus += "||요보충";
	else
		Om.Main_DeviceStatus += "||잔량만";
	
	// 27:Brm계원카세트상태(정상,탈착)
	if (ClerkCSTLoc == NORMAL_POS)
		Om.Main_DeviceStatus += "||정상";
	else
		Om.Main_DeviceStatus += "||탈착";
	
	// 28:Brm회수함상태(정상,잔량만,탈착)
	if (RejectCSTLoc == ABNORMAL_POS)
		Om.Main_DeviceStatus += "||탈착";
	else
	if ((RejectCSTStatus == CST_EMPTY)	||
		(RejectCSTStatus == CST_NORMAL))
		Om.Main_DeviceStatus += "||정상";
	else
		Om.Main_DeviceStatus += "||잔량만";

	// 29:Brm램프정보
	Om.Main_DeviceStatus += "||" + IniGetStr(_SP_DEVICE_INI, "BRM", "램프정보");

	// 30:Net상태
	Om.Main_DeviceStatus += "||" + Om.NetDeviceStatus;
	
	// 31:Net장애진행카운터
	Om.Main_DeviceStatus += "||" + Om.NetProcCount;
	
	// 32:Net장애코드
	Om.Main_DeviceStatus += "||" + Om.NetErrorCode;
	
	// 33:App상태
	Om.Main_DeviceStatus += "||" + Om.AppDeviceStatus;
	
	// 34:App장애진행카운터
	Om.Main_DeviceStatus += "||" + Om.AppProcCount;
	
	// 35:App장애코드
	Om.Main_DeviceStatus += "||" + Om.AppErrorCode;

	// 36:BRM자동축퇴알림(3OUT 발생시 OM Display "보수요원 조치요망") MIC
	if (m_pProfile->TRANS.DeviceExecOff & DEV_BRM)
		Om.Main_DeviceStatus += "||장애";
	else
		Om.Main_DeviceStatus += "||정상";
	// 37:UCM자동축퇴알림 MIC
	if (m_pProfile->TRANS.DeviceExecOff & DEV_UCM)
		Om.Main_DeviceStatus += "||장애";
	else
		Om.Main_DeviceStatus += "||정상";

	// 38:PBM자동축퇴알림 MIC
	if (m_pProfile->TRANS.DeviceExecOff & DEV_PBM)
		Om.Main_DeviceStatus += "||장애";
	else
		Om.Main_DeviceStatus += "||정상";

	// 오만원권 지원 ──────────────────────
	// 36:Brm오만원권현금카세트상태(정상,잔량소,요보충,잔량만)
	if (Withdraw50TFlag)
	{
		if (RecycleBox50000Status == CST_EMPTY)
			Om.Main_DeviceStatus += "||요보충";
		else
		if(RecycleBox50000Status == CST_NEAR)
			Om.Main_DeviceStatus += "||잔량소";
		else
		if (RecycleBox50000Status == CST_NORMAL)
			Om.Main_DeviceStatus += "||정상";
		else
			Om.Main_DeviceStatus += "||잔량만";
	}
	else
	{
		Om.Main_DeviceStatus += "||미지원";
	}
	// ────────────────────────────────────


	// 00:U8100 개발관련 Pbm잉크상태잔량				
	Om.Main_DeviceStatus += "||";

	if(PBMInkCheckCnt % 100000 == 0)
	{
		Om.Main_DeviceStatus += PBMInkStatus;
		PBMInkCheckCnt = 0;
	}
	else
	{
		Om.Main_DeviceStatus += PBMInkStatus;
		PBMInkCheckCnt++;
	}
	// 00:Ucm출금잉크잔량								// UCM 수표잉크의 경우는 프로모션에서만 사용
	Om.Main_DeviceStatus += "||";

	// 00:Ucm입금잉크잔량								
	Om.Main_DeviceStatus += "||";
	

	Om.Main_DeviceStatus += "||";							// 39:A4프린터 상태
	Om.Main_DeviceStatus += "||";							// 40:A4프린터 장애 진행카운터	
	Om.Main_DeviceStatus += "||";							// 41:A4프린터 장애코드
	Om.Main_DeviceStatus += "||";							// 42:A4프린터 잔량상태
	Om.Main_DeviceStatus += "||";							// 43:카메라1	상태
	Om.Main_DeviceStatus += "||";							// 44:카메라2 상태
	Om.Main_DeviceStatus += "||";							// 45:BRM 자동축퇴알림
	Om.Main_DeviceStatus += "||";							// 46:UCM 자동축퇴알림
	Om.Main_DeviceStatus += "||";							// 47:PBM 자동축퇴알림
	
/////////////////////////////////////////////////////////////////////////////
	// 메인정보(메인부가정보)
	// OM_메인부가정보
	Om.Main_ReffInfomation = "";
										
	// 0:운영,준비
	if (OpenKey)
		Om.Main_ReffInfomation = "운영";
	else
		Om.Main_ReffInfomation = "준비";
		
	// 1:송수신상태(ON,OFF)
	Om.Main_ReffInfomation += "||" + Om.SendRecvStatus;

	// 2:조작가능상태(ON,OFF)
	Om.Main_ReffInfomation += "||" + Om.OperationStatus;

	// 3:수표권종(10,50,100)
	Om.Main_ReffInfomation += "||" + Om.CheckKind;

/////////////////////////////////////////////////////////////////////////////
	// 출금수표등록	
	// OM_출금수표등록 
	Om.Sub_WithCheckReg = "";

	// 0:발행은행1
	strTemp.Format("%2.2s", fnAPL_GetOutCheckBankNo().GetBuffer(0));
	Om.Sub_WithCheckReg += strTemp;
	
	// 1:출금수표번호1
	strTemp.Format("%8.8s", fnAPL_GetNumberOfOutCheckNo().GetBuffer(0));
	Om.Sub_WithCheckReg += "||" + strTemp;
										
	// 2:출금수표매수1
	strTemp.Format("%d", fnAPL_GetNumberOfOutCheck());
	Om.Sub_WithCheckReg += "||" + strTemp;
	
	// 3:발행은행2
	strTemp.Format("%2.2s", fnAPL_GetOutCheckBankNo2().GetBuffer(0));
	Om.Sub_WithCheckReg += "||" + strTemp;
	
	// 4:출금수표번호2
	strTemp.Format("%8.8s", fnAPL_GetNumberOfOutCheckNo2().GetBuffer(0));
	Om.Sub_WithCheckReg += "||" + strTemp;
										
	// 5:출금수표매수2
	strTemp.Format("%d", fnAPL_GetNumberOfOutCheck2());
	Om.Sub_WithCheckReg += "||" + strTemp;

	
/////////////////////////////////////////////////////////////////////////////
	// 설치정보
	// OM_설치정보
	Om.Sub_InstInformation = "";		
										
	// 0:설치날짜(YYYYMMDD)
	strTemp.Format("%8.8s", m_pProfile->NETWORK.MachineMakeDate);
	Om.Sub_InstInformation += strTemp;
	
	// 1:점번(NNNNNNNN)
	strTemp.Format("%8.8s", m_pProfile->NETWORK.BranchNum);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 2:기번(NNNNNNNN)
	strTemp.Format("%6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 3:배치장소(NNNNNNNN)
	strTemp.Format("%8.8s", m_pProfile->NETWORK.RoomNum);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 4:점내(ON,OFF)
	if (m_pProfile->NETWORK.BranchInOut)
		Om.Sub_InstInformation += "||ON";
	else
		Om.Sub_InstInformation += "||OFF";

	// 5:부점(NNNNNNNN)
	strTemp.Format("%8.8s", m_pProfile->NETWORK.SubBranchNum);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 6:무인점포(ON,OFF)
	if (m_pProfile->NETWORK.AutoBranchOnOff)
		Om.Sub_InstInformation += "||ON";
	else
		Om.Sub_InstInformation += "||OFF";
	
	// 7:24시간운영(ON,OFF)
	if (m_pProfile->NETWORK.All24OnOff)
		Om.Sub_InstInformation += "||ON";
	else
		Om.Sub_InstInformation += "||OFF";
	
	// 8:365운영(ON,OFF)
	if (m_pProfile->NETWORK.All365OnOff)
		Om.Sub_InstInformation += "||ON";
	else
		Om.Sub_InstInformation += "||OFF";
	
	// 9:CE사무소코드(NNNN)
	strTemp.Format("%4.4s", m_pProfile->NETWORK.CeOfficeCode);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 10:기기관리번호(NNNNNNNNNNNN)
	strTemp.Format("%12.12s", m_pProfile->NETWORK.MachineMakeNum);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 11:출장소(NNNNNNNN)
	strTemp.Format("%8.8s", m_pProfile->NETWORK.OutBranchNum);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 12:설치자(NNNNNNNNNN)
	strTemp.Format("%10.10s", m_pProfile->NETWORK.InstName);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 13:설치확인자(NNNNNNNNNN)
	strTemp.Format("%10.10s", m_pProfile->NETWORK.InstConfirmName);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 14:보수담당자(NNNNNNNNNN)
	strTemp.Format("%10.10s", m_pProfile->NETWORK.CeName);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 15:보수담당자연락처(NNNNNNNNNNNNNNNN)
	strTemp.Format("%16.16s", m_pProfile->NETWORK.CeTel);
	Om.Sub_InstInformation += "||" + strTemp;

	Om.Sub_InstInformation += "||";								// 16:단말기종
	Om.Sub_InstInformation += "||";								// 17:용역점
	Om.Sub_InstInformation += "||";								// 18:사용제한
	Om.Sub_InstInformation += "||";								// 19:모델명
	Om.Sub_InstInformation += "||";								// 20:설치확인자
	Om.Sub_InstInformation += "||";								// 21:시작일련번호
	Om.Sub_InstInformation += "||";								// 22:기기종류

/////////////////////////////////////////////////////////////////////////////
	// 부가장치설정
	// OM_부가장치설정
	Om.Sub_SubDeviceReg = "";			
										
	// 0:전자화폐(가능,불가)
	if (m_pProfile->DEVICE.ICCardDevice != NOT_SET)
		Om.Sub_SubDeviceReg += "가능";
	else
		Om.Sub_SubDeviceReg += "불가";

	// 1:해외거래(가능,불가) U8100 개발관련					
	if (m_pDevCmn->DesTranStatus != TRUE)
		Om.Sub_SubDeviceReg += "||불가";
	else
	if (m_pProfile->DEVICE.DesHandler == D_DES_SET)
		Om.Sub_SubDeviceReg += "||가능S";
	else
	if (m_pProfile->DEVICE.DesHandler == T_DES_SET)
		Om.Sub_SubDeviceReg += "||가능T";
	else
		Om.Sub_SubDeviceReg += "||불가";

	// 2:지문거래(가능,불가)								
	Om.Sub_SubDeviceReg += "||";
	
	// 3:적외선거래(가능,불가)
	if ( (m_pProfile->DEVICE.IrdaHandler != NOT_SET)	&&
		 (m_pProfile->DEVICE.RfdHandler == NOT_SET)	)
		Om.Sub_SubDeviceReg += "||가능";
	else
		Om.Sub_SubDeviceReg += "||불가";

	// 4:바코드거래(가능,불가)								
	if (m_pProfile->DEVICE.BarHandler != NOT_SET)
		Om.Sub_SubDeviceReg += "||가능";
	else
		Om.Sub_SubDeviceReg += "||불가";
	
	// 5:암호화거래(가능,불가)
	if (m_pProfile->DEVICE.SeedHandler != NOT_SET)
		Om.Sub_SubDeviceReg += "||가능";
	else
		Om.Sub_SubDeviceReg += "||불가";

	// 6:Vfd(가능,불가)
	if (m_pProfile->DEVICE.VfdHandler != NOT_SET)
		Om.Sub_SubDeviceReg += "||가능";
	else
		Om.Sub_SubDeviceReg += "||불가";

	// 7:RF거래(가능,불가) U8100 개발관련
	if (m_pDevCmn->RfdTestStatus != TRUE)
		Om.Sub_SubDeviceReg += "||불가";
	else
	if (m_pProfile->DEVICE.RfdHandler != NOT_SET)
		Om.Sub_SubDeviceReg += "||가능";
	else
		Om.Sub_SubDeviceReg += "||불가";

	// 8:통장 Auto Turn										
	if(m_pProfile->DEVICE.PBHandler == PBM_NAT_SET) 
		Om.Sub_SubDeviceReg += "||불가";
	else
		Om.Sub_SubDeviceReg += "||가능";

	Om.Sub_SubDeviceReg += "||";								// 9:A4통장선택

	// 10:금융IC(가능,불가)									
	if (m_pProfile->DEVICE.ICCardDevice != NOT_SET)
		Om.Sub_SubDeviceReg += "||가능";
	else
		Om.Sub_SubDeviceReg += "||불가";

	Om.Sub_SubDeviceReg += "||";								// 11:대전시공과금거래
	Om.Sub_SubDeviceReg += "||";								// 12:TTL사용여부
	Om.Sub_SubDeviceReg += "||";								// 13:MyBi

	// 14:장애우												
	if (m_pProfile->DEVICE.EarHandler != NOT_SET)			
		Om.Sub_SubDeviceReg += "||가능";
	else
		Om.Sub_SubDeviceReg += "||불가";

	// 15:프리폼(가능,불가)										// 프로모션에서만 사용하였으며 각고객은 사양에 맞추어 처리
// 	if (m_pProfile->DEVICE.FreeFormHandler != NOT_SET)
// 		Om.Sub_SubDeviceReg += "||가능";
// 	else
		Om.Sub_SubDeviceReg += "||불가";	

	Om.Sub_SubDeviceReg += "||";								// 16:AJMS거래
	Om.Sub_SubDeviceReg += "||";								// 17:교통카드거래

	
	Om.Sub_SubDeviceReg += "||";								// 18:지로거래(가능, 불가)				// V07-01-12-#05
	Om.Sub_SubDeviceReg += "||";								// 19:통장선택(IPBM, LPBM)				// V07-01-12-#05

	// 20:저시력자/화면확대(가능,불가)						
	if(m_pProfile->TRANS.ZoomFlag != NOT_SET)
		Om.Sub_SubDeviceReg += "||가능";
	else
		Om.Sub_SubDeviceReg += "||불가";

	// 21:근접센서(가능,불가)
	if (m_pProfile->DEVICE.Proximity != NOT_SET)			
		Om.Sub_SubDeviceReg += "||가능";
	else
		Om.Sub_SubDeviceReg += "||불가";

	
	Om.Sub_SubDeviceReg += "||";								// 22:수취구연속촬영				

/////////////////////////////////////////////////////////////////////////////
	// Lan설정정보
	// OM_LAN설정정보
	Om.Sub_LanInformation = "";			
										
	// 0:IP주소(NNN.NNN.NNN.NNN)
	strTemp.Format("%15.15s", m_pProfile->NETWORK.AtmIPAddress);
	Om.Sub_LanInformation += strTemp;
	
	// 1:서브넷마스크(NNN.NNN.NNN.NNN)
	strTemp.Format("%15.15s", m_pProfile->NETWORK.AtmSubnetMask);
	Om.Sub_LanInformation += "||" + strTemp;
	
	// 2:게이트웨이(NNN.NNN.NNN.NNN)
	strTemp.Format("%15.15s", m_pProfile->NETWORK.AtmGateWay);
	Om.Sub_LanInformation += "||" + strTemp;
	
	// 3:BP1IP주소(NNN.NNN.NNN.NNN)
	strTemp.Format("%15.15s", m_pProfile->NETWORK.BpIpAddress);
	Om.Sub_LanInformation += "||" + strTemp;
	
	// 4:BP1PORT(NNNNN)
	strTemp.Format("%5.5s", m_pProfile->NETWORK.BpPortNo);
	Om.Sub_LanInformation += "||" + strTemp;
	
	// 5:BP2IP주소(NNN.NNN.NNN.NNN)
	strTemp.Format("%15.15s", m_pProfile->NETWORK.BpIpAddress2);
	Om.Sub_LanInformation += "||" + strTemp;
	
	// 6:BP2PORT(NNNNN)
	strTemp.Format("%5.5s", m_pProfile->NETWORK.BpPortNo2);
	Om.Sub_LanInformation += "||" + strTemp;
	
	// 7:BP3IP주소(NNN.NNN.NNN.NNN)
	strTemp.Format("%15.15s", m_pProfile->NETWORK.BpIpAddress3);
	Om.Sub_LanInformation += "||" + strTemp;
	
	// 8:BP3PORT(NNNNN)
	strTemp.Format("%5.5s", m_pProfile->NETWORK.BpPortNo3);
	Om.Sub_LanInformation += "||" + strTemp;
	
	Om.Sub_LanInformation += "||";								// 9
	Om.Sub_LanInformation += "||";								// 10
	Om.Sub_LanInformation += "||";								// 11:FTP IP
	Om.Sub_LanInformation += "||";								// 12:FTP Port
	Om.Sub_LanInformation += "||";								// 13:FTP User Name
	Om.Sub_LanInformation += "||";								// 14:FTP Password
	Om.Sub_LanInformation += "||";								// 15:시스템재시작

/////////////////////////////////////////////////////////////////////////////
	// 회선정보
	// OM_회선정보
	Om.Sub_LineInformation = "";				
										
	// 0:통신종류(IOC,HDLC,SWP,SDLC,TCP/IP,CCL,BAS)
	if (m_pProfile->NETWORK.Interface == NET_IOC)
		Om.Sub_LineInformation += "IOC";
	else
	if (m_pProfile->NETWORK.Interface == NET_HDLC)
		Om.Sub_LineInformation += "HDLC";
	else
	if (m_pProfile->NETWORK.Interface == NET_SWP)
		Om.Sub_LineInformation += "SWP";
	else
	if (m_pProfile->NETWORK.Interface == NET_SDLC)
		Om.Sub_LineInformation += "SDLC";
	else
	if (m_pProfile->NETWORK.Interface == NET_TCPIP)
		Om.Sub_LineInformation += "TCP/IP";
	else
	if (m_pProfile->NETWORK.Interface == NET_CCL)
		Om.Sub_LineInformation += "CCL";
	else
		Om.Sub_LineInformation += "BAS";
	
	// 1:DA(XX)
	Om.Sub_LineInformation += "||" + Int2Asc(m_pProfile->NETWORK.DeviceAddr, 2, 16);
	
	// 2:SA(XX)
	Om.Sub_LineInformation += "||" + Int2Asc(m_pProfile->NETWORK.SelectAddr, 2, 16);
	
	// 3:송수신사이즈
	strTemp.Format("%d", m_pProfile->NETWORK.SendRecvSize);
	Om.Sub_LineInformation += "||" + strTemp;
	
	// 4:회선속도(1200,2400,4800,9600,19200)
	if (m_pProfile->NETWORK.LineSpeed == L1200)
		Om.Sub_LineInformation += "||1200";
	else
	if (m_pProfile->NETWORK.LineSpeed == L2400)
		Om.Sub_LineInformation += "||2400";
	else
	if (m_pProfile->NETWORK.LineSpeed == L4800)
		Om.Sub_LineInformation += "||4800";
	else
	if (m_pProfile->NETWORK.LineSpeed == L9600)
		Om.Sub_LineInformation += "||9600";
	else
		Om.Sub_LineInformation += "||19200";
	
	// 5:POLLCHECK(ON,OFF)
	if (m_pProfile->NETWORK.PollCheck == POLL_ON)
		Om.Sub_LineInformation += "||ON";
	else
		Om.Sub_LineInformation += "||OFF";
	
	// 6:POINT(PTP,MTP)
	if (m_pProfile->NETWORK.Point == PTP)
		Om.Sub_LineInformation += "||PTP";
	else
		Om.Sub_LineInformation += "||MTP";
	
	// 7:DUPLEX(HALF,FULL)
	if (m_pProfile->NETWORK.Duplex == HALF_DUPLEX)
		Om.Sub_LineInformation += "||HALF";
	else
		Om.Sub_LineInformation += "||FULL";
	
	// 8:TIMEFILLER(MTF,FTF)
	if (m_pProfile->NETWORK.TimeFiller == MTF)
		Om.Sub_LineInformation += "||MTF";
	else
		Om.Sub_LineInformation += "||FTF";
	
	// 9:RSTYPE(RS-422,RS-232C)
	if (m_pProfile->NETWORK.RsType == RS422)
		Om.Sub_LineInformation += "||RS-422";
	else
		Om.Sub_LineInformation += "||RS-232C";
	
	// 10:NRZ(NRZ,NRZI)
	if (m_pProfile->NETWORK.Nrz == NRZ)
		Om.Sub_LineInformation += "||NRZ";
	else
		Om.Sub_LineInformation += "||NRZI";
	
	// 11:CLOCK(INCLK,EXCLK)
	if (m_pProfile->NETWORK.Clk == INCLK)
		Om.Sub_LineInformation += "||INCLK";
	else
		Om.Sub_LineInformation += "||EXCLK";

/////////////////////////////////////////////////////////////////////////////
	// ATMS설정정보
	// OM_ATMS설정정보
	Om.Sub_ATMSServerInformation = "";			
										
	// 0:SERVER IP주소(NNN.NNN.NNN.NNN)
//	strTemp.Format("%15.15s", IniGetStr(_ATMS_FTP_INI, "SERVER_INFO", "IP"));
	strTemp.Format("%s", IniGetStr(_ATMS_FTP_INI, "TCP_IP", "IP", "211.196.190.170"));
	Om.Sub_ATMSServerInformation += strTemp;
	
	// 1:SERVER PORT(NNNNN)
//	strTemp.Format("%5.5s", IniGetStr(_ATMS_FTP_INI, "SERVER_INFO", "Port"));
	strTemp.Format("%s", IniGetStr(_ATMS_FTP_INI, "TCP_IP", "Port", "2005"));
	Om.Sub_ATMSServerInformation += "||" + strTemp;

	// 2:예비SERVER1 IP주소(NNN.NNN.NNN.NNN)
//	strTemp.Format("%15.15s", IniGetStr(_ATMS_FTP_INI, "SERVER_INFO", "IP_CNTONE"));
	strTemp.Format("%s", IniGetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTONE", "211.196.190.170"));
	Om.Sub_ATMSServerInformation += "||" + strTemp;
	
	// 3:예비SERVER2 IP주소(NNN.NNN.NNN.NNN)
//	strTemp.Format("%15.15s", IniGetStr(_ATMS_FTP_INI, "SERVER_INFO", "IP_CNTTWO"));
	strTemp.Format("%s", IniGetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTTWO", "211.196.190.170"));
	Om.Sub_ATMSServerInformation += "||" + strTemp;

	// 4:예비SERVER3 IP주소(NNN.NNN.NNN.NNN)
//	strTemp.Format("%15.15s", IniGetStr(_ATMS_FTP_INI, "SERVER_INFO", "IP_CNTTHREE"));
	strTemp.Format("%s", IniGetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTTHREE", "211.196.190.170"));
	Om.Sub_ATMSServerInformation += "||" + strTemp;

/////////////////////////////////////////////////////////////////////////////
	// 거래정보설정
	// OM_거래정보설정
	Om.Sub_TranInformationReg = "";		
										
	// 0:현금출금(가능,불가)
	if (m_pProfile->TRANS.ClerkTransOnOff & (TRAN_WITH_CASH | TRAN_WITH_50TCASH))	
		Om.Sub_TranInformationReg += "가능";
	else
		Om.Sub_TranInformationReg += "불가";

	// 1:현금입금(가능,불가)
	if (m_pProfile->TRANS.ClerkTransOnOff & TRAN_DEP_CASH)
		Om.Sub_TranInformationReg += "||가능";
	else
		Om.Sub_TranInformationReg += "||불가";
	
	// 2:수표출금(가능,불가)
	if (m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CHECK)
		Om.Sub_TranInformationReg += "||가능";
	else
		Om.Sub_TranInformationReg += "||불가";
	
	// 3:수표입금(가능,불가)
	if (m_pProfile->TRANS.ClerkTransOnOff & TRAN_DEP_CHECK)
		Om.Sub_TranInformationReg += "||가능";
	else
		Om.Sub_TranInformationReg += "||불가";
	
	// 4:통장거래(가능,불가)
	if (m_pProfile->TRANS.ClerkTransOnOff & TRAN_PBM_OK)
		Om.Sub_TranInformationReg += "||가능";
	else
		Om.Sub_TranInformationReg += "||불가";


	// 5:5만원권(가능,불가), 6:BC부교체(가능,불가)
	if (ChangeBCFlag)
	{
		if (m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_50TCASH)
		{
			Om.Sub_TranInformationReg += "||가능";			
			Om.Sub_TranInformationReg += "||가능";			
		}
		else												
		{
			Om.Sub_TranInformationReg += "||불가";			
			Om.Sub_TranInformationReg += "||가능";				
		}
	}
	else
	{
		m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_WITH_50TCASH;
		Om.Sub_TranInformationReg += "||불가";					
		Om.Sub_TranInformationReg += "||불가";				
	}
	
	Om.Sub_TranInformationReg += "||";											
	
	// 8: 장애우	 장애인ATM
	if (m_pProfile->DEVICE.EarHandler == EAR_SET)
		Om.Sub_TranInformationReg += "||가능";
	else
		Om.Sub_TranInformationReg += "||불가";

	// 9: 장애우 저시력	 	 장애인ATM								
	if (m_pProfile->TRANS.ZoomFlag == ZOOM_SET)
		Om.Sub_TranInformationReg += "||가능";
	else
		Om.Sub_TranInformationReg += "||불가";
	Om.Sub_TranInformationReg += "||";						

	// 10:통장PGU 불/가능
	Om.Sub_TranInformationReg += "||가능";

	// 11:근접센서 장애인ATM
	if (m_pProfile->DEVICE.Proximity != NOT_SET)				
		Om.Sub_SubDeviceReg += "||가능";
	else
		Om.Sub_SubDeviceReg += "||불가";

/////////////////////////////////////////////////////////////////////////////
	// 현금보충회수 :
	// OM_현금보충회수
	Om.Sub_CashRefillTakeup = "";		
										
	// 0:카세트1지폐수
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt"));
	Om.Sub_CashRefillTakeup += strTemp;

	// 1:카세트2지폐수
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt"));
	Om.Sub_CashRefillTakeup += "||" + strTemp;

	// 2:카세트3지폐수
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt"));
	Om.Sub_CashRefillTakeup += "||" + strTemp;

	if (HBRM_SET == AtmDefine.CashHandler)
	{
		// 3:카세트4지폐수
		strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt"));
		Om.Sub_CashRefillTakeup += "||" + strTemp;

		// 4:계원카세트RJB : for OKI BRM
		strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "ClerkRJBNoteCnt"));
		Om.Sub_CashRefillTakeup += "||" + strTemp;
	}

	// 5:Brm상태(정상,장애)
	Om.Sub_CashRefillTakeup += "||" + Om.BrmDeviceStatus;

	Om.Sub_DepKindReg.Empty();

	if ((m_pProfile->BRM.DepositCashKind & 0x01) == 0x01)	// 구만원권
		Om.Sub_DepKindReg += "가능";
	else
		Om.Sub_DepKindReg += "불가";

	if ((m_pProfile->BRM.DepositCashKind & 0x02) == 0x02)	// 구오천원권
		Om.Sub_DepKindReg += "||가능";
	else
		Om.Sub_DepKindReg += "||불가";

	if ((m_pProfile->BRM.DepositCashKind & 0x04) == 0x04)	// 구천원권
		Om.Sub_DepKindReg += "||가능";
	else
		Om.Sub_DepKindReg += "||불가";

	if ((m_pProfile->BRM.DepositCashKind & 0x10) == 0x10)	// 신만원권
		Om.Sub_DepKindReg += "||가능";
	else
		Om.Sub_DepKindReg += "||불가";

	if ((m_pProfile->BRM.DepositCashKind & 0x20) == 0x20)	// 신오천원권
		Om.Sub_DepKindReg += "||가능";
	else
		Om.Sub_DepKindReg += "||불가";

	if ((m_pProfile->BRM.DepositCashKind & 0x40) == 0x40)	// 신천원권
		Om.Sub_DepKindReg += "||가능";
	else
		Om.Sub_DepKindReg += "||불가";
	
	if (HBRM_SET == m_pProfile->DEVICE.CashHandler)			// 오만원
	{
		if (ChangeBCFlag)
		{
			if ((m_pProfile->BRM.DepositCashKind & 0x08) == 0x08)
				Om.Sub_DepKindReg += "||가능";
			else
				Om.Sub_DepKindReg += "||불가";

			Om.Sub_DepKindReg += "||가능";
		}
		else
		{
			Om.Sub_DepKindReg += "||불가";
			Om.Sub_DepKindReg += "||불가";
		}

		if (ChangeBCFlag)										// 7:BC부 교체여부(BRM_SET에 따라 달라야 하지 않나???) 
			Om.Sub_DepKindReg += "||가능";
		else
			Om.Sub_DepKindReg += "||불가";
	}
	else
		Om.Sub_DepKindReg += "||불가";

/////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
	int nType = 0;
	Om.CasetteType.Empty();

	if (HBRM_SET == AtmDefine.CashHandler)
	{
		nType = fnBRM_GetNoteKindOfRB1();
		Om.CasetteType += (nType == BRM_NOTEKIND_10000_OLD) ? "구만" : 
						  (nType == BRM_NOTEKIND_5000_OLD)  ? "구오천" : 
						  (nType == BRM_NOTEKIND_1000_OLD)  ? "구천" : 
						  (nType == BRM_NOTEKIND_10000_NEW) ? "신만" : 
						  (nType == BRM_NOTEKIND_5000_NEW)  ? "신오천" : 
						  (nType == BRM_NOTEKIND_1000_NEW)  ? "신천" :
						  (nType == BRM_NOTEKIND_DEP_OLD)   ? "입금구" :		
						  (nType == BRM_NOTEKIND_DEP_NEW)   ? "입금신" :	
						  (nType == BRM_NOTEKIND_50000)     ? "오만" : "";		

		nType = fnBRM_GetNoteKindOfRB2();
		Om.CasetteType += (nType == BRM_NOTEKIND_10000_OLD) ? "||구만" : 
						  (nType == BRM_NOTEKIND_5000_OLD)  ? "||구오천" : 
						  (nType == BRM_NOTEKIND_1000_OLD)  ? "||구천" : 
						  (nType == BRM_NOTEKIND_10000_NEW) ? "||신만" : 
						  (nType == BRM_NOTEKIND_5000_NEW)  ? "||신오천" : 
						  (nType == BRM_NOTEKIND_1000_NEW)  ? "||신천" :
						  (nType == BRM_NOTEKIND_DEP_OLD)   ? "||입금구" :		
						  (nType == BRM_NOTEKIND_DEP_NEW)   ? "||입금신" :		
						  (nType == BRM_NOTEKIND_50000)		? "||오만" : "";	

		nType = fnBRM_GetNoteKindOfRB3();
		Om.CasetteType += (nType == BRM_NOTEKIND_10000_OLD) ? "||구만" : 
						  (nType == BRM_NOTEKIND_5000_OLD)  ? "||구오천" : 
						  (nType == BRM_NOTEKIND_1000_OLD)  ? "||구천" : 
						  (nType == BRM_NOTEKIND_10000_NEW) ? "||신만" : 
						  (nType == BRM_NOTEKIND_5000_NEW)  ? "||신오천" : 
						  (nType == BRM_NOTEKIND_1000_NEW)  ? "||신천" :
						  (nType == BRM_NOTEKIND_DEP_OLD)   ? "||입금구" :		
						  (nType == BRM_NOTEKIND_DEP_NEW)   ? "||입금신" :		
						  (nType == BRM_NOTEKIND_50000)		? "||오만" : "";	

		nType = fnBRM_GetNoteKindOfRB4();
		Om.CasetteType += (nType == BRM_NOTEKIND_10000_OLD) ? "||구만" : 
						  (nType == BRM_NOTEKIND_5000_OLD)  ? "||구오천" : 
						  (nType == BRM_NOTEKIND_1000_OLD)  ? "||구천" : 
						  (nType == BRM_NOTEKIND_10000_NEW) ? "||신만" : 
						  (nType == BRM_NOTEKIND_5000_NEW)  ? "||신오천" : 
						  (nType == BRM_NOTEKIND_1000_NEW)  ? "||신천" :
						  (nType == BRM_NOTEKIND_DEP_OLD)   ? "||입금구" :		
						  (nType == BRM_NOTEKIND_DEP_NEW)   ? "||입금신" :		
						  (nType == BRM_NOTEKIND_50000)		? "||오만" : "";	
		
	}
	else
	{
		if (ChangeBCFlag)										
			Om.CasetteType += "||가능";
		else
			Om.CasetteType += "||불가";
	}
// nkh end
/////////////////////////////////////////////////////////////////////////////
	Om.Sub_AutoComputeNote = "";
	// 0:기기종류
	if (m_pProfile->BRM.PriorityNote == CASH_PRIORITY_1000TH)
		Om.Sub_AutoComputeNote += "10||";
	else
		Om.Sub_AutoComputeNote += "5||";

	if (Withdraw50TFlag)										
		Om.Sub_AutoComputeNote += "가능";
	else
		Om.Sub_AutoComputeNote += "불가";

/////////////////////////////////////////////////////////////////////////////
// T3ATM AP 변경내역 #19 - 카메라 3개의 상태를 모두 표시하기 위함
	Om.Sub_CamerStatus = "";
	
	if (fnCMR_GetFaceCameraDeviceStatus() == NORMAL)
		Om.Sub_CamerStatus += "정상||";
	else
		Om.Sub_CamerStatus += "에러||";
	
	if (fnCMR_GetHandCameraDeviceStatus() == NORMAL)
		Om.Sub_CamerStatus += "정상||";
	else
		Om.Sub_CamerStatus += "에러||";
	
	if (fnCMR_GetRoomCameraDeviceStatus() == NORMAL)
		Om.Sub_CamerStatus += "정상";
	else
		Om.Sub_CamerStatus += "에러";
	
	
	if ((nInitFlag) ||											
		(Om.Main_CD_VanIII_Status			!= OmSave.Main_CD_VanIII_Status))		
	{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_CD_VanIII_Status[%s]", Om.Main_CD_VanIII_Status);
		OmSave.Main_CD_VanIII_Status		=	Om.Main_CD_VanIII_Status;
		SetShareData("OM_Main_CD_VanIII_Status",		Om.Main_CD_VanIII_Status);
	}

	if ((nInitFlag) ||											
		(Om.Main_CD_VanIII_ApSpEpVersion			!= OmSave.Main_CD_VanIII_ApSpEpVersion))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_CD_VanIII_ApSpEpVersion[%s]", Om.Main_CD_VanIII_ApSpEpVersion);
		OmSave.Main_CD_VanIII_ApSpEpVersion		=	Om.Main_CD_VanIII_ApSpEpVersion;
		SetShareData("OM_Main_CD_VanIII_ApSpEpVersion",		Om.Main_CD_VanIII_ApSpEpVersion);
	}


	if ((nInitFlag) ||										
		(Om.Main_CD_VanIII_ModeStatus			!= OmSave.Main_CD_VanIII_ModeStatus))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_CD_VanIII_ModeStatus[%s]", Om.Main_CD_VanIII_ModeStatus);
		OmSave.Main_CD_VanIII_ModeStatus		=	Om.Main_CD_VanIII_ModeStatus;
		SetShareData("OM_Main_CD_VanIII_ModeStatus",		Om.Main_CD_VanIII_ModeStatus);
	}


/////////////////////////////////////////////////////////////////////////////
	if ((nInitFlag) ||											// 메인정보(메인헤더1)	
		(Om.Main_1LineStatus			!= OmSave.Main_1LineStatus))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_1LineStatus(%s)", Om.Main_1LineStatus);
		OmSave.Main_1LineStatus			=	Om.Main_1LineStatus;
		SetShareData("OM_메인헤더1",		Om.Main_1LineStatus);
	}

	if ((nInitFlag) ||											// 메인정보(메인헤더2)
		(Om.Main_2LineStatus			!= OmSave.Main_2LineStatus))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_2LineStatus(%s)", Om.Main_2LineStatus);
		OmSave.Main_2LineStatus			=	Om.Main_2LineStatus;
		SetShareData("OM_메인헤더2",		Om.Main_2LineStatus);
	}

	if ((nInitFlag) ||											// 메인정보(메인현금보유현황)
		(Om.Main_CashStatus				!= OmSave.Main_CashStatus))			
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_CashStatus(%s)", Om.Main_CashStatus);
		OmSave.Main_CashStatus			=	Om.Main_CashStatus;
		SetShareData("OM_메인현금보유현황",	Om.Main_CashStatus);
		
 		strTemp = Om.Main_CashStatus;
 		strTemp.Replace("||", "|");
	}

	if ((nInitFlag) ||											// 메인정보(메인잔량정보)
		(Om.Main_Status					!= OmSave.Main_Status))				
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_Status(%s)", Om.Main_Status);
		OmSave.Main_Status				=	Om.Main_Status;
		SetShareData("OM_메인잔량정보",		Om.Main_Status);
	}

	if ((nInitFlag) ||											// 메인정보(메인수표보유현황)
		(Om.Main_CheckStatus			!= OmSave.Main_CheckStatus))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_CheckStatus(%s)", Om.Main_CheckStatus);
		OmSave.Main_CheckStatus			=	Om.Main_CheckStatus;
		SetShareData("OM_메인수표보유현황",	Om.Main_CheckStatus);
	}

	if ((nInitFlag) ||											// 메인정보(메인현금회수현황)
		(Om.Main_CashRejectStatus		!= OmSave.Main_CashRejectStatus))	
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_CashRejectStatus(%s)", Om.Main_CashRejectStatus);
		OmSave.Main_CashRejectStatus	=	Om.Main_CashRejectStatus;
		SetShareData("OM_메인현금회수현황",	Om.Main_CashRejectStatus);
	}

	if ((nInitFlag) ||											// 메인정보(메인장치현황)
		(Om.Main_DeviceStatus			!= OmSave.Main_DeviceStatus))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_DeviceStatus(%s)", Om.Main_DeviceStatus);
		OmSave.Main_DeviceStatus		=	Om.Main_DeviceStatus;
		SetShareData("OM_메인장치현황",		Om.Main_DeviceStatus);
	}

	if ((nInitFlag) ||											// 메인정보(메인부가정보)
		(Om.Main_ReffInfomation			!= OmSave.Main_ReffInfomation))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_ReffInfomation(%s)", Om.Main_ReffInfomation);
		OmSave.Main_ReffInfomation		=	Om.Main_ReffInfomation;
		SetShareData("OM_메인부가정보",		Om.Main_ReffInfomation);
	}

	if ((nInitFlag) ||											// 출금수표등록
		(Om.Sub_WithCheckReg			!= OmSave.Sub_WithCheckReg))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_WithCheckReg(%s)", Om.Sub_WithCheckReg);
		OmSave.Sub_WithCheckReg			=	Om.Sub_WithCheckReg;
		SetShareData("OM_출금수표등록",		Om.Sub_WithCheckReg);
	}

	if ((nInitFlag) ||											// 설치정보
		(Om.Sub_InstInformation			!= OmSave.Sub_InstInformation))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_InstInformation(%s)", Om.Sub_InstInformation);
		OmSave.Sub_InstInformation		=	Om.Sub_InstInformation;
		SetShareData("OM_설치정보",			Om.Sub_InstInformation);
	}

	if ((nInitFlag) ||											// 부가장치설정
		(Om.Sub_SubDeviceReg			!= OmSave.Sub_SubDeviceReg))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_SubDeviceReg(%s)", Om.Sub_SubDeviceReg);
		OmSave.Sub_SubDeviceReg			=	Om.Sub_SubDeviceReg;
		SetShareData("OM_부가장치설정",		Om.Sub_SubDeviceReg);
	}

	if ((nInitFlag) ||											// Lan설정정보
		(Om.Sub_LanInformation			!= OmSave.Sub_LanInformation))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_LanInformation(%s)", Om.Sub_LanInformation);
		OmSave.Sub_LanInformation		=	Om.Sub_LanInformation;
		SetShareData("OM_LAN설정정보",		Om.Sub_LanInformation);
	}

	if ((nInitFlag) ||											// 회선정보
		(Om.Sub_LineInformation			!= OmSave.Sub_LineInformation))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_LineInformation(%s)", Om.Sub_LineInformation);
		OmSave.Sub_LineInformation		=	Om.Sub_LineInformation;
		SetShareData("OM_회선정보",			Om.Sub_LineInformation);
	}

	if ((nInitFlag) ||											// 거래정보설정
		(Om.Sub_TranInformationReg		!= OmSave.Sub_TranInformationReg))	
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_TranInformationReg(%s)", Om.Sub_TranInformationReg);
		OmSave.Sub_TranInformationReg	=	Om.Sub_TranInformationReg;
		SetShareData("OM_거래정보설정",		Om.Sub_TranInformationReg);
	}

	if ((nInitFlag) ||											// 현금보충회수
		(Om.Sub_CashRefillTakeup		!= OmSave.Sub_CashRefillTakeup))	
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_CashRefillTakeup(%s)", Om.Sub_CashRefillTakeup);
		OmSave.Sub_CashRefillTakeup		=	Om.Sub_CashRefillTakeup;
		SetShareData("OM_현금보충회수",		Om.Sub_CashRefillTakeup);
	}

	if ((nInitFlag) ||											// V200: 입금권종선택 
		(Om.Sub_DepKindReg		!= OmSave.Sub_DepKindReg))	
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_DepKindReg(%s)", Om.Sub_DepKindReg);
		OmSave.Sub_DepKindReg		=	Om.Sub_DepKindReg;
		SetShareData("OM_입금권종선택",		Om.Sub_DepKindReg);
	}

	if ((nInitFlag) ||											// 카세트권종선택 
		(Om.CasetteType		!= OmSave.CasetteType))	
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.CasetteType(%s)", Om.CasetteType);
		OmSave.CasetteType			=		Om.CasetteType;
		SetShareData("OM_카세트권종선택",	Om.CasetteType);
	}

	if ((nInitFlag) ||											// 기기설정
		(Om.Sub_AutoComputeNote				!= OmSave.Sub_AutoComputeNote))	
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_AutoComputeNote[%s]", Om.Sub_AutoComputeNote);
		OmSave.Sub_AutoComputeNote			=	Om.Sub_AutoComputeNote;
		SetShareData("OM_자동계산설정",			Om.Sub_AutoComputeNote);
	}

// T3ATM AP 변경내역 #19 - 카메라 3개의 상태를 모두 표시하기 위함
	if ((nInitFlag) ||											// 3카메라 상태
		(Om.Sub_CamerStatus		!= OmSave.Sub_CamerStatus))	
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_CamerStatus[%s]", Om.Sub_CamerStatus);
		OmSave.Sub_CamerStatus			=	Om.Sub_CamerStatus;
		SetShareData("OM_CMSTATUS",		Om.Sub_CamerStatus);
	}

	if (nFirstFlag)											
	{
		nFirstFlag = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OM_APSTATUS[%s]", GetShareData("OM_APSTATUS"));
		SetShareData("OM_APSTATUS", "APSTART");					// OP_APSTATUS Setting
		SetShareData("OM_APSTATUS", "APSTART");					
		SetShareData("OM_APSTATUS", "APSTART");					
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OM_APSTATUS[%s]", GetShareData("OM_APSTATUS"));
	}
	

	return TRUE;
}

// 계원실행검사
// V03-01-01 초기실행옵션 확인
int	CDevCmn::fnAPL_ClerkExecCheck()
{
	HWND	hWnd = NULL;
	
	switch (m_pProfile->DEVICE.MachineType)
	{
//		case U8100:												
		case U8500:
		case U3100K:
			hWnd = ::FindWindow(NULL, _PNAME_REARAP);
			break;

		default:
			hWnd = ::FindWindow(NULL, _PNAME_REARAP);
			break;
	}

	if (hWnd == NULL)
	{
		if (RegGetInt(_REGKEY_LOADER, "ShowOM", FALSE) == TRUE)
		{
			WinExec(RegGetStr(_REGKEY_LOADER, "RearAP", _DEFAULT_REARAP), SW_SHOW);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkExecCheck:C:\\T1ATM\\SUPERVISOR\\REARAP.EXE 재실행!!!");
		}
//t		WinExec("C:\\T1ATM\\SUPERVISOR\\REARAP.EXE", SW_SHOW);
	}
	

	switch (m_pProfile->DEVICE.MachineType)
	{
		case U8100:												
		case U3100K:
			hWnd = ::FindWindow(NULL, _PNAME_OMDEMON);
			if (hWnd == NULL)
			{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkExecCheck:C:\\T1ATM\\APP\\MAIN\\OMDEMON.EXE 재실행!!!");
				WinExec(RegGetStr(_REGKEY_LOADER, "OmDemon", _DEFAULT_OMDEMON), SW_SHOW);
		//t		WinExec("C:\\T1ATM\\APP\\MAIN\\OMDEMON.EXE", SW_SHOW);
			}
		default:
			break;
	}

	return TRUE;
}

// Segment설정 라우팅 함수
int CDevCmn::fnAPL_SetSegment(LPCTSTR szValue)
{
	switch (AtmDefine.MachineType)
	{
	case U8100:												
	case U3100K:
	default:
		fnSPL_SetSegment(szValue);
		break;
	}

	return TRUE;
}

int CDevCmn::fnAPL_PowerOff(int nWaitSec)
{
	switch (AtmDefine.MachineType)
	{
	case U3100K:
	default:
		fnUPS_PowerOff(nWaitSec);
		break;
	}
	return TRUE;
}

int CDevCmn::fnAPL_VFDDislpay(int nPos)							// T2ATM 2006.5.24 yscho
{
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strGetKey;
	CString			CurTime;
	char			strTemp[16]={0,};

	if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_VFD))
	{
		return TRUE;
	}

	if (VFDTotalCount <= 0)
		return TRUE;

	if (VFDCurCount >= VFDTotalCount)
		VFDCurCount = 0;

	if (!nPos)
	{
		strGetKey = IniGetStr(_VFD_INI, "DISPLAY", itoa(VFDCurCount, strTemp, 10), 0);

		SplitString(strGetKey, "||", strTempArray);

		strTempArray[0].Replace("\"","");
		strTempArray[0].TrimRight(" ");
	}

	switch (nPos)
	{
	case 0:
		if ((AtmStatus == ATM_CLERK)	||
			(AtmStatus == ATM_READY)	||
			(AtmStatus == ATM_ERROR)	)
			fnVFD_RealTimeTextDisplay("지금 사용하실 수 없습니다", 0, 7, 20, 25, 2, 0, "굴림");
		else
			fnVFD_RealTimeTextDisplay(strTempArray[0], 0, 7, 20, 25, 2, 0, "굴림");

		Delay_Msg(100);
		VFDCurCount++;
		break;
	case 1:
		fnVFD_BlinkDisplay(2, 45, 45, 5);
		Delay_Msg(100);
		break;
	case 2:
		fnVFD_ScrollDisplay(1, 1024, 1);
		Delay_Msg(100);
		break;
	default:
		break;
	}

	return TRUE;
}

// 광고문구검사
int CDevCmn::fnAPL_CheckCatchPlay()
{
	int				nCount = 0;
	int				x, y;
	int				nChangeFlg = FALSE;
	CStringArray	strTempArray;
	CString			strGetKey;
	CString			CurTime;
	CString			strStack("");
	char			TmpBuf[16]={0,};

	nCount = IniGetInt(_VFD_INI, "DISPLAY", "DisplayCount", 0);

	nChangeFlg = FALSE;
	y = 0;
	for (x = 0; x < nCount; x++)
	{
		strGetKey = IniGetStr(_VFD_INI, "DISPLAY", Int2Asc(x));
		
		SplitString(strGetKey, "||", strTempArray);
		if (strTempArray.GetSize() < 5)
			continue;

		if (memcmp(strTempArray[3].GetBuffer(0), m_pProfile->TRANS.YYYYMMDD, 8) < 0)
		{
			nChangeFlg = TRUE;
		}
		else
		if (memcmp(strTempArray[3].GetBuffer(0), m_pProfile->TRANS.YYYYMMDD, 8) == 0)
		{
			CurTime = GetTime();
			if (memcmp(strTempArray[4].GetBuffer(0), CurTime.GetBuffer(0), 4) <= 0)
			{
				nChangeFlg = TRUE;
			}
			else
			{
				strStack += strGetKey + ",";
				y++;
			}
		}
		else
		{
			strStack += strGetKey + ",";
			y++;
		}
	}

	if (nChangeFlg)
	{
		DeleteFile(_VFD_TMP_INI);
		DeleteFile("D:\\INI\\VFD_COPY.INI");

		IniSetInt(_VFD_TMP_INI, "DISPLAY", "DisplayCount", y);
		strTempArray.RemoveAll();
		SplitString(strStack, ",", strTempArray);
		for (x = 0; x < y; x++)
		{
			IniSetStr(_VFD_TMP_INI, "DISPLAY", itoa(x, TmpBuf, 10), strTempArray[x]);
		}

		Delay_Msg();
		CopyFile(_VFD_INI, "D:\\INI\\VFD_COPY.INI", FALSE);
		DeleteFile(_VFD_INI);
		if (!CopyFile(_VFD_TMP_INI, _VFD_INI, FALSE))
		{
			CopyFile(_VFD_TMP_INI, _VFD_INI, FALSE);
		}
	}

	return TRUE;
}

int CDevCmn::fnAPL_GetPowerStatus()
{
	int		nReturn = FALSE;

	nReturn = fnUPS_GetPowerStatus();

	return nReturn;
}

// Get Log Start ───────────────────────
// 로그파일받기 : 2006.11.22
int CDevCmn::fnAPL_GetLogFileStart(int nMonth, int nDate)
{
	CString strCommand = "";
	CString strCurrentDir = "";
	CString strDrive = "";
	DWORD	dwFreeSize = 0;

	// Flash Memory Stick을 찾는다.
	fnAPL_GetDriveInfo(strDrive, dwFreeSize, 'C');
	if (0 == dwFreeSize)
		return FALSE;

//////////////////////////////////////////////////////////////////////////
	CreateDirectory("D:\\TEMPTRACE", NULL);						// 임시폴더생성

//////////////////////////////////////////////////////////////////////////
// ① REGISTRY 수집
	strCommand.Format("c");
	WinExecWaitEnd(strCommand, SW_HIDE);

	strCommand.Format("REGEDIT /E /S D:\\TEMPTRACE\\WOSAREG.TXT HKEY_CLASSES_ROOT\\WOSA/XFS_ROOT");
	WinExecWaitEnd(strCommand, SW_HIDE);

//////////////////////////////////////////////////////////////////////////
// ② BRM LOG 수집(OKI)
	if (OKI_SET == AtmDefine.CashHandler)
	{
		fnBRM_BrmGetLog();										// GET OKI BRM LOG
		strCommand.Format("XCOPY C:\\ATMDAT\\*.* D:\\TEMPTRACE\\ATMDAT%02d%02d\\ /S /Y", nMonth, nDate);
		WinExecWaitEnd(strCommand, SW_HIDE);
	}
	
//////////////////////////////////////////////////////////////////////////
// ③ INI 수집
	strCommand.Format("XCOPY D:\\INI\\*.* D:\\TEMPTRACE\\INI%02d%02d\\ /S /Y", nMonth, nDate);
	WinExecWaitEnd(strCommand, SW_HIDE);

//////////////////////////////////////////////////////////////////////////
// ④ 전자저널(텍스트)
	strCurrentDir.Format("D:\\ECASH\\JNL%02d%02d", nMonth, nDate);
	if (SetCurrentDirectory(strCurrentDir))
	{
		strCommand.Format("XCOPY *.* D:\\TEMPTRACE\\ELECJNL%02d%02d\\ /S /Y", nMonth, nDate);
		WinExecWaitEnd(strCommand, SW_HIDE);
	}

//////////////////////////////////////////////////////////////////////////
// ⑤ UBIATOR LOG
	if (SetCurrentDirectory("c:\\Program Files\\Ubiator20\\Trace"))
	{
		strCommand.Format("XCOPY *.* D:\\TEMPTRACE\\UBIATOR%02d%02d\\ /S /Y", nMonth, nDate);
		WinExecWaitEnd(strCommand, SW_HIDE);
	}

//////////////////////////////////////////////////////////////////////////
// ⑥ AP LOG
	if (SetCurrentDirectory(_TRACE_DIR))
	{
		strCommand.Format("XCOPY *%02d.* D:\\TEMPTRACE\\APLOG%02d%02d\\ /S /Y", nDate, nMonth, nDate);
		WinExecWaitEnd(strCommand, SW_HIDE);

		strCommand.Format("XCOPY *%02d%02d*.* D:\\TEMPTRACE\\APLOG%02d%02d\\ /S /Y", nMonth, nDate, nMonth, nDate);
		WinExecWaitEnd(strCommand, SW_HIDE);
	}
	
//////////////////////////////////////////////////////////////////////////
// 압축하기(전체)
	SetCurrentDirectory("D:\\TEMPTRACE");
	strCommand.Format("C:\\PKZIP25 -ADD -DIR=CURRENT D:\\TEMPTRACE\\TRACE%02d%02d.ZIP", nMonth, nDate);
	WinExecWaitEnd(strCommand, SW_HIDE);

//////////////////////////////////////////////////////////////////////////
// 복사하기
	strCommand.Format("D:\\TEMPTRACE\\TRACE%02d%02d.ZIP", nMonth, nDate);
	strCurrentDir.Format("%sTRACE%3.3s%2.2s%02d%02d.ZIP", strDrive,
														  &m_pProfile->NETWORK.BranchNum[5],
														  &m_pProfile->NETWORK.AtmSerialNum[6],
														  nMonth, nDate);
	CopyFile(strCommand, strCurrentDir, FALSE);

//////////////////////////////////////////////////////////////////////////
// 파일지우기
	SetCurrentDirectory("C:\\T1ATM");
	fnAPL_DeleteFiles("D:\\TEMPTRACE");
	RemoveDirectory("D:\\TEMPTRACE");
//////////////////////////////////////////////////////////////////////////
	
	return TRUE;
}

// Remote Ctrl Log Start ───────────────────────
// 로그파일받기 : 2008.05.24
int CDevCmn::fnAPL_GetRemoteCtrlFileStart(CString strLocalPath, int nYear, int nMonth, int nDate, int nTran, int nSerialF)
{
	CString strCommand = "";
	CString strCurrentDir = "";
	CString strDrive = "";
	DWORD	dwFreeSize = 0;


//////////////////////////////////////////////////////////////////////////
	CreateDirectory("D:\\TEMPTRACE", NULL);						// 임시폴더생성
//////////////////////////////////////////////////////////////////////////

	if(nTran == ATMS_CMD_JNLMODE)
	{
		//  1. 전자저널(텍스트)
		strCurrentDir.Format("D:\\ECASH\\JNL%02d%02d", nMonth, nDate);
		if (SetCurrentDirectory(strCurrentDir))
		{
			strCommand.Format("XCOPY *.* D:\\TEMPTRACE\\ELECJNL%04d%02d%02d\\ /S /Y", nYear, nMonth, nDate);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetRemoteCtrlFileStart JNLUPLOAD[%s]", strCommand);

			WinExecWaitEnd(strCommand, SW_HIDE);
		}
		// 압축하기(전체)
		SetCurrentDirectory("D:\\TEMPTRACE");
		strCommand.Format("C:\\PKZIP25 -ADD -DIR=CURRENT %s", strLocalPath);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetRemoteCtrlFileStart strLocalPath1[%s]", strCommand);
		WinExecWaitEnd(strCommand, SW_HIDE);
	}
	else
	if(nTran == ATMS_CMD_CAMMODE)
	{
		//2. CAM_INFO GetSprintf("%s%4.4s\\%2.2s%8.8s%1.1d.JPG", _EJR_IMG, &m_pProfile->TRANS.YYYYMMDD[4])
		strCurrentDir.Format("D:\\ECASH\\IMG%02d%02d",nMonth, nDate);
		if (SetCurrentDirectory(strCurrentDir))
		{
			strCommand.Format("XCOPY %02d%08d?.JPG D:\\TEMPTRACE\\ELECIMG%08d%04d%02d%02d\\ /S /Y", nDate, nSerialF, nSerialF, nYear, nMonth, nDate);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetRemoteCtrlFileStart CAMUPLOAD[%s]", strCommand);
			WinExecWaitEnd(strCommand, SW_HIDE);
		}
		// 압축하기(전체)
		SetCurrentDirectory("D:\\TEMPTRACE");
		strCommand.Format("C:\\PKZIP25 -ADD -DIR=CURRENT %s", strLocalPath);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetRemoteCtrlFileStart strLocalPath2[%s]", strCommand);
		WinExecWaitEnd(strCommand, SW_HIDE);

	}
	else return TRUE;

//////////////////////////////////////////////////////////////////////////
	SetCurrentDirectory("C:\\T1ATM");
//////////////////////////////////////////////////////////////////////////
	
	return TRUE;
}

int CDevCmn::fnAPL_GetDriveInfo(CString& szDrive, DWORD& dwFreeSize, TCHAR cStartDirve)
{
	CString strTemp("");
	
	for (TCHAR cLetter = cStartDirve; cLetter <= 'Z'; cLetter++)
	{
		strTemp = cLetter;
		strTemp += ":\\";
		UINT nType = GetDriveType( strTemp );
		
		if (nType == DRIVE_REMOVABLE)							// 이동식 드라이브일때만
		{
			szDrive = strTemp;

			DWORD lpSectorsPerCluster = 0;						// sectors per cluster
			DWORD lpBytesPerSector = 0;							// bytes per sector
			DWORD lpNumberOfFreeClusters = 0;					// free clusters
			DWORD lpTotalNumberOfClusters = 0;					// total clusters
			
			BOOL bResult = GetDiskFreeSpace(
											szDrive,				// root path
											&lpSectorsPerCluster,	// sectors per cluster
											&lpBytesPerSector,		// bytes per sector
											&lpNumberOfFreeClusters,// free clusters
											&lpTotalNumberOfClusters// total clusters
											);
			
			// 남은 용량
			dwFreeSize = ( lpSectorsPerCluster * lpBytesPerSector * lpNumberOfFreeClusters);
			if ( 0 >= dwFreeSize ) continue;					// size가 0이하면 다시
			break;
		}
	}

	return TRUE;
}

int CDevCmn::fnAPL_DeleteFiles(LPCTSTR DirName)
{
	CFileFind	finder;

	CString dir, file;
	dir.Format("%s\\*.*",DirName);

    SYSTEMTIME  lTime;
    GetLocalTime(&lTime);

	BOOL bWorking = finder.FindFile(dir);
	CFileStatus	fileStatus;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		dir  = finder.GetFilePath();
		file = finder.GetFileName();

		if(	file == _T(".") ||
			file == _T(".."))	continue;

		if(finder.IsDirectory())
		{
			if(!fnAPL_DeleteFiles(dir))
			{
				return FALSE;
			}

			RemoveDirectory(dir);
			continue;
		}

		if (!CFile::GetStatus(dir, fileStatus))	
			continue;
		else
			DeleteFile(dir);
	}

	finder.Close();
	return TRUE;
}
// Get Log End ───────────────────────

/*==============================================================================
* Function Name : fnAPD_NetworkInfoSet()
* Description   : 네트워크 정보(IP, Subnet, Gateway)값을 시스템 IP로 설정한다.
* Parameters    : 
* Return        : T_OK
* version       : V02-04-00-#05
* ============================================================================*/
int CDevCmn::fnAPL_NetworkInfoSet(int nFixIp)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[Network설정] fnAPD_NetworkInfoSet"); 
	char szIpAddress[17] = {0,};
	CBIPConfig	clsIP;

	m_pProfile->GetNetworkProfile();
	
	//고정IP 설정 
	if(nFixIp)
	{
	
		// 01. Ip Address
		memset(szIpAddress, 0x00, sizeof(szIpAddress));
		Sprintf(szIpAddress, 16, "%d.%d.%d.%d", Asc2Int(m_pProfile->NETWORK.AtmIPAddress, 3),				
									  Asc2Int(&m_pProfile->NETWORK.AtmIPAddress[4], 3),
									  Asc2Int(&m_pProfile->NETWORK.AtmIPAddress[8], 3),
									  Asc2Int(&m_pProfile->NETWORK.AtmIPAddress[12], 3));
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[Network설정] 01. Ip Address [%s]", szIpAddress); 

		clsIP.AtmSetIpAddress(szIpAddress);

		// 02. SubnetMask
		memset(szIpAddress, 0x00, sizeof(szIpAddress));
		Sprintf(szIpAddress, 16, "%d.%d.%d.%d", Asc2Int(m_pProfile->NETWORK.AtmSubnetMask, 3),				
									  Asc2Int(&m_pProfile->NETWORK.AtmSubnetMask[4], 3),
									  Asc2Int(&m_pProfile->NETWORK.AtmSubnetMask[8], 3),
									  Asc2Int(&m_pProfile->NETWORK.AtmSubnetMask[12], 3));

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[Network설정] 02. SubnetMask [%s]", szIpAddress); 

		clsIP.AtmSetSubnetMask(szIpAddress);

		// 03. GateWay
		memset(szIpAddress, 0x00, sizeof(szIpAddress));
		Sprintf(szIpAddress, 16, "%d.%d.%d.%d", Asc2Int(m_pProfile->NETWORK.AtmGateWay, 3),						
									  Asc2Int(&m_pProfile->NETWORK.AtmGateWay[4], 3),
									  Asc2Int(&m_pProfile->NETWORK.AtmGateWay[8], 3),
									  Asc2Int(&m_pProfile->NETWORK.AtmGateWay[12], 3));

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[Network설정] 03. GateWay [%s]", szIpAddress);
		
		clsIP.AtmSetGateWay(szIpAddress);
	}
	else
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[Network설정] fnAPD_NetworkInfoSet Set Dhcp"); 

	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[Network설정] fnAPD_NetworkInfoSet return"); 
	return T_OK;
}


/*==============================================================================
* Function Name : fnAPL_CheckMachineType()
* Description   : 기종과 CashHandler비교
* Parameters    : 
* Return        : TRUE : 정상, FALSE : 비정상
* version       : V05-06-03-#13
* ============================================================================*/
int CDevCmn::fnAPL_CheckMachineType()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CheckMachineType()::MachineType[%d], CashHandler[%d]", 
													m_pProfile->DEVICE.MachineType, AtmDefine.CashHandler); 

	int	nReturn = TRUE;
	switch (m_pProfile->DEVICE.MachineType)
	{
	case U8100:										// U8100-AP변경
	case U3100K:
		if (AtmDefine.CashHandler != HBRM_SET)
			nReturn = FALSE;
		break;

	default:
		break;
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CheckMachineType()::nReturn[%d]", nReturn);
	return nReturn;
}

/*==============================================================================
* Function Name : fnAPL_SetComputerName()
* Description   : 컴퓨터이름변경
* Parameters    : 
* Return        : T_OK
* version       : V07-01-01
* ============================================================================*/
int CDevCmn::fnAPL_SetComputerName()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetComputerName()"); 
	int	nReturn = T_OK;
	CString	strComputerName("");

	// 점번(5) + 기번(3) + 업체(2:HS) + 기종(Max:5, 업체별상이함)
	// ex) 84201003HST2ATM or 20160011HST1ATM or 20202HSSCDP
	m_pProfile->GetNetworkProfile();

	strComputerName.Format("%2.2s%3.3s%3.3sHS%5.5s",
							(memcmp(&m_pProfile->NETWORK.OutBranchNum[6], "00", 2) == 0) ? "20" : "84",
							&m_pProfile->NETWORK.BranchNum[5],
							&m_pProfile->NETWORK.AtmSerialNum[5], 
							m_pProfile->DEVICE.MachineType == U8100 ? "U8100" : 
							m_pProfile->DEVICE.MachineType == U3100K ? "U3100K" : "UNKNO"
							);


	if (SetComputerName(strComputerName.GetBuffer(0)) != TRUE)
		nReturn = T_ERROR;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetComputerName()::nReturn[%d]", nReturn);
	return nReturn;
}

// 카세트의 현금보유정보
int CDevCmn::fnAPL_BRMCashInfo()
{
	CString strTemp("");

	if (AtmDefine.CashHandler == CDU_SET)
		return T_ERROR;

	strTemp.Format("%d|%d|%d|%d", 
							RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt"), 
							RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt"), 
							RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt"), 
							((AtmDefine.CashHandler == OKI_SET) || (AtmDefine.CashHandler == HBRM_SET)) ? RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt") : 0);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[SUNAPBOX][%s]", strTemp);

	strTemp.Empty();
	if ((AtmDefine.CashHandler == OKI_SET)	||
		(AtmDefine.CashHandler == HBRM_SET)	)					// U8100-AP변경
	{
		strTemp.Format("%d|%d|%d|%d", 
							RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt"), 
							RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt"), 
							RegGetInt(_REGKEY_BRMSTATUS, "RJB2NoteCnt"), 
							RegGetInt(_REGKEY_BRMSTATUS, "ClerkRJBNoteCnt"));
	}

	else
	{
		strTemp.Format("%d|%d|0|0", 
							IniGetInt(_SP_DEVICE_INI, "BRM", "StackOfRJB1"),
							IniGetInt(_SP_DEVICE_INI, "BRM", "StackOfRJB2"));
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[RJTBOX][%s]", strTemp);


	return T_OK;
}

//#0068 -> #N0144
int CDevCmn::fnAPL_BRMCstMatchInfo()
{

	static	int		StatusInCnt = 0;							// 재진입횟수조정
	int nRB1NoteKindUp = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKindUp");
	int nRB2NoteKindUp = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteKindUp");
	int nRB3NoteKindUp = RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteKindUp");
	int nNoteKind1     = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKind");
	int nNoteKind2     = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteKind");
	int nNoteKind3     = RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteKind");
	CString csrGetValue("");

	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		if ((nRB1NoteKindUp != nNoteKind1) || (nRB2NoteKindUp != nNoteKind2) || (nRB3NoteKindUp != nNoteKind3))
		{
			ForceMissMatchDownFlag = TRUE;

			if ((nRB1NoteKindUp != nNoteKind1) && (nRB2NoteKindUp != nNoteKind2) && (StatusInCnt != 10))
			{

				csrGetValue =  (nRB1NoteKindUp == BRM_NOTEKIND_10000_OLD) ? "구만" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_5000_OLD)  ? "구오천" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_1000_OLD)  ? "구천" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_10000_NEW) ? "신만" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_5000_NEW)  ? "신오천" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_1000_NEW)  ? "신천" :
							   (nRB1NoteKindUp == BRM_NOTEKIND_DEP_OLD)   ? "입금구" :	
							   (nRB1NoteKindUp == BRM_NOTEKIND_DEP_NEW)   ? "입금신" :	
							   (nRB1NoteKindUp == BRM_NOTEKIND_50000)     ? "오만" : "UNK";
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[1카세트 점퍼 설정 오류(오만 <-> %s)]", csrGetValue));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo [1카세트 점퍼 설정 오류(신만 <-> %s)]", csrGetValue);

				csrGetValue =  (nRB2NoteKindUp == BRM_NOTEKIND_10000_OLD) ? "구만" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_5000_OLD)  ? "구오천" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_1000_OLD)  ? "구천" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_10000_NEW) ? "신만" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_5000_NEW)  ? "신오천" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_1000_NEW)  ? "신천" :
							   (nRB2NoteKindUp == BRM_NOTEKIND_DEP_OLD)   ? "입금구" :	
							   (nRB2NoteKindUp == BRM_NOTEKIND_DEP_NEW)   ? "입금신" :	
							   (nRB2NoteKindUp == BRM_NOTEKIND_50000)     ? "오만" : "UNK";
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[2카세트 점퍼 설정 오류(신만 <-> %s)]", csrGetValue));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo [2카세트 점퍼 설정 오류(신만 <-> %s)]", csrGetValue);


				StatusInCnt = 10;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo RB Status MissMatch (m_pDevCmn->DepAvail[%d],m_pDevCmn->OpenKey[%d])", m_pDevCmn->WithAvail, m_pDevCmn->OpenKey);
			}
			else
			if ((nRB2NoteKindUp != nNoteKind2) && (nRB3NoteKindUp != nNoteKind3) && (StatusInCnt != 10))
			{
				csrGetValue =  (nRB2NoteKindUp == BRM_NOTEKIND_10000_OLD) ? "구만" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_5000_OLD)  ? "구오천" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_1000_OLD)  ? "구천" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_10000_NEW) ? "신만" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_5000_NEW)  ? "신오천" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_1000_NEW)  ? "신천" :
							   (nRB2NoteKindUp == BRM_NOTEKIND_DEP_OLD)   ? "입금구" :	
							   (nRB2NoteKindUp == BRM_NOTEKIND_DEP_NEW)   ? "입금신" :	
							   (nRB2NoteKindUp == BRM_NOTEKIND_50000)     ? "오만" : "UNK";

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[2카세트 점퍼 설정 오류(신만 <-> %s)]", csrGetValue));

				csrGetValue =  (nRB3NoteKindUp == BRM_NOTEKIND_10000_OLD) ? "구만" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_5000_OLD)  ? "구오천" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_1000_OLD)  ? "구천" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_10000_NEW) ? "신만" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_5000_NEW)  ? "신오천" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_1000_NEW)  ? "신천" :
							   (nRB3NoteKindUp == BRM_NOTEKIND_DEP_OLD)   ? "입금구" :	
							   (nRB3NoteKindUp == BRM_NOTEKIND_DEP_NEW)   ? "입금신" :	
							   (nRB3NoteKindUp == BRM_NOTEKIND_50000)     ? "오만" : "UNK";

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[3카세트 점퍼 설정 오류(신만 <-> %s)]", csrGetValue));

				StatusInCnt = 10;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo RB Status MissMatch (m_pDevCmn->DepAvail[%d],m_pDevCmn->OpenKey[%d])", m_pDevCmn->WithAvail, m_pDevCmn->OpenKey);
			}
			else
			if ((nRB1NoteKindUp != nNoteKind1) && (StatusInCnt != 10))
			{
				csrGetValue =  (nRB1NoteKindUp == BRM_NOTEKIND_10000_OLD) ? "구만" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_5000_OLD)  ? "구오천" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_1000_OLD)  ? "구천" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_10000_NEW) ? "신만" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_5000_NEW)  ? "신오천" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_1000_NEW)  ? "신천" :
							   (nRB1NoteKindUp == BRM_NOTEKIND_DEP_OLD)   ? "입금구" :	
							   (nRB1NoteKindUp == BRM_NOTEKIND_DEP_NEW)   ? "입금신" :	
							   (nRB1NoteKindUp == BRM_NOTEKIND_50000)     ? "오만" : "UNK";

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[1카세트 점퍼 설정 오류(신만 <-> %s)]", csrGetValue));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo [1카세트 점퍼 설정 오류(신만 <-> %s)]", csrGetValue);

				StatusInCnt = 10;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo RB Status MissMatch (m_pDevCmn->DepAvail[%d],m_pDevCmn->OpenKey[%d])", m_pDevCmn->WithAvail, m_pDevCmn->OpenKey);
			}
			else
			if ((nRB2NoteKindUp != nNoteKind2) && (StatusInCnt != 10))
			{
				csrGetValue =  (nRB2NoteKindUp == BRM_NOTEKIND_10000_OLD) ? "구만" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_5000_OLD)  ? "구오천" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_1000_OLD)  ? "구천" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_10000_NEW) ? "신만" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_5000_NEW)  ? "신오천" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_1000_NEW)  ? "신천" :
							   (nRB2NoteKindUp == BRM_NOTEKIND_DEP_OLD)   ? "입금구" :	
							   (nRB2NoteKindUp == BRM_NOTEKIND_DEP_NEW)   ? "입금신" :	
							   (nRB2NoteKindUp == BRM_NOTEKIND_50000)     ? "오만" : "UNK";

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[2카세트 점퍼 설정 오류(신만 <-> %s)]", csrGetValue));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo [2카세트 점퍼 설정 오류(신만 <-> %s)]", csrGetValue);

				StatusInCnt = 10;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo RB Status MissMatch (m_pDevCmn->DepAvail[%d],m_pDevCmn->OpenKey[%d])", m_pDevCmn->WithAvail, m_pDevCmn->OpenKey);
			}
			else
			if ((nRB3NoteKindUp != nNoteKind3) && (StatusInCnt != 10))
			{
				csrGetValue =  (nRB3NoteKindUp == BRM_NOTEKIND_10000_OLD) ? "구만" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_5000_OLD)  ? "구오천" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_1000_OLD)  ? "구천" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_10000_NEW) ? "신만" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_5000_NEW)  ? "신오천" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_1000_NEW)  ? "신천" :
							   (nRB3NoteKindUp == BRM_NOTEKIND_DEP_OLD)   ? "입금구" :	
							   (nRB3NoteKindUp == BRM_NOTEKIND_DEP_NEW)   ? "입금신" :	
							   (nRB3NoteKindUp == BRM_NOTEKIND_50000)     ? "오만" : "UNK";

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[3카세트 점퍼 설정 오류(신만 <-> %s)]", csrGetValue));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo [3카세트 점퍼 설정 오류(신만 <-> %s)]", csrGetValue);

				StatusInCnt = 10;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo RB Status MissMatch (m_pDevCmn->DepAvail[%d],m_pDevCmn->OpenKey[%d])", m_pDevCmn->WithAvail, m_pDevCmn->OpenKey);
			}

		}
		else
		{
			ForceMissMatchDownFlag = FALSE;
		}
	}
	else
		ForceMissMatchDownFlag = FALSE;


	return ForceMissMatchDownFlag;
}

//#0070
int CDevCmn::fnAPL_BRMCstSetInfo()
{

	static int nTmpDepAvail = TRAN_DEP_NOT;
	static int nTmpWithAvail = TRAN_WITH_NOT;
	
	int	nReturn = FALSE;
	int nBRMStatus = FALSE;
	int nRB1NoteKindUp = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKindUp");
	int nRB2NoteKindUp = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteKindUp");
	int nRB3NoteKindUp = RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteKindUp");
	int nNoteKind1     = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKind");
	int nNoteKind2     = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteKind");
	int nNoteKind3     = RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteKind");

	int	nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원
	int	nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
	int	nCurRB3Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);

	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		if ((nRB1NoteKindUp == nNoteKind1) && (nRB2NoteKindUp == nNoteKind2) && (nRB3NoteKindUp == nNoteKind3))
		{

			//#N0143
			if ((nTmpWithAvail != WithAvail) && (nTmpDepAvail != DepAvail) && (OpenKey))
			{
				nBRMStatus = fnCSH_GetDeviceStatus();
				if (nBRMStatus == DOWN)
				{
					fnSCR_DisplayString(2, "지금은 기기 점검중입니다");
					fnSCR_DisplayString(3, "빠른 시간내에 복구예정이오니");
					fnSCR_DisplayString(4, "잠시만 기다려 주십시오");
					fnSCR_DisplayScreen(701);// 화면출력

					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_BRMCstSetInfo] -1");
					fnCSH_Initialize();								
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_BRMCstSetInfo] -2");
					nBRMStatus = fnCSH_GetDeviceStatus();
					//#N0154
					fnAPL_SetSensorInfo();							// 센서정보설정
					fnAPL_GetAvailTrans();							// 거래가능상태구하기
					
				}
			}

			//#N0156
			IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K", nCurRB1Cnt);      
			IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", nCurRB2Cnt);
			IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K", nCurRB3Cnt);
			
			//#N0135
			if ((FstCSTStatus == CST_EMPTY) && (0 < nCurRB1Cnt))
			{
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K", 0);
				nReturn = TRUE;
			}
			else
			if((FstCSTStatus == CST_NORMAL) && (nCurRB1Cnt < 100))
			{
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K", 5000);
				nReturn = TRUE;
			}

			if ((SndCSTStatus == CST_EMPTY) && (0 < nCurRB2Cnt))
			{
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", 0);
				nReturn = TRUE;
			}
			else
			if((SndCSTStatus == CST_NORMAL) && (nCurRB2Cnt < 100))
			{
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", 5000);
				nReturn = TRUE;
			}

			if ((TrdCSTStatus == CST_EMPTY) && (0 < nCurRB3Cnt))
			{
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K", 0);
				nReturn = TRUE;
			}
			else
			if((TrdCSTStatus == CST_NORMAL) && (nCurRB3Cnt < 100))
			{
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K", 5000);
				nReturn = TRUE;
			}
			
			if ((nReturn == TRUE) && (nBRMStatus == NORMAL))
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_BRMCstSetInfo Status][Rb1 %d, Rb2 %d, Rb3 %d]", FstCSTStatus, SndCSTStatus, TrdCSTStatus);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_BRMCstSetInfo Cnt   ][Rb1 %d, Rb2 %d, Rb3 %d]", nCurRB1Cnt, nCurRB2Cnt, nCurRB3Cnt);

				fnSCR_DisplayString(2, "지금은 기기 점검중입니다");
				fnSCR_DisplayString(3, "빠른 시간내에 복구예정이오니");
				fnSCR_DisplayString(4, "잠시만 기다려 주십시오");
				fnSCR_DisplayScreen(701);// 화면출력

				fnUTL_IniSetInt(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedCstInfo", TRUE);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_BRMCstSetInfo] -1");
				fnCSH_Initialize();								
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_BRMCstSetInfo] -2");
				nReturn = TRUE;
				//#N0154
				fnAPL_SetSensorInfo();							// 센서정보설정
				fnAPL_GetAvailTrans();							// 거래가능상태구하기
			}

			nTmpWithAvail         =  WithAvail;
			nTmpDepAvail          =  DepAvail;

		}
	}

	return nReturn;
}

//#N0145
int CDevCmn::fnAPL_BRMCstSetInfo_Empty_On()
{

	int	nReturn = FALSE;
	int nRB1NoteKindUp = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKindUp");
	int nRB2NoteKindUp = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteKindUp");
	int nNoteKind1     = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteKind");
	int nNoteKind2     = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteKind");

	int	nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5만원
	int	nCurRB2Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt", 0);
	int	nCurRB3Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt", 0);

	if(m_pProfile->DEVICE.MachineType == U3100K)
	{
		if ((nRB1NoteKindUp == nNoteKind1) && (nRB2NoteKindUp == nNoteKind2))
		{
			//#N0135 -> #N0150 -> #N0166
			if (((FstCSTStatus == CST_NORMAL) || (FstCSTStatus == CST_FULL) || (FstCSTStatus == CST_NEAR)) && (0 == nCurRB1Cnt))
			{
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_A", "50K", 5000);
				nReturn = TRUE;
			}

			if (((SndCSTStatus == CST_NORMAL) || (SndCSTStatus == CST_FULL) || (SndCSTStatus == CST_NEAR)) && (0 == nCurRB2Cnt))
			{
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_B", "NEW10K", 5000);
				nReturn = TRUE;
			}

			if (((TrdCSTStatus == CST_NORMAL) || (TrdCSTStatus == CST_FULL) || (TrdCSTStatus == CST_NEAR)) && (0 == nCurRB3Cnt))
			{
				IniSetInt(_SP_BRM_CSTINFO_INI, "REFILL_C", "NEW10K", 5000);
				nReturn = TRUE;
			}


			if (nReturn == TRUE)
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_BRMCstSetInfo_Empty_On Status][Rb1 %d, Rb2 %d, Rb3 %d]", FstCSTStatus, SndCSTStatus, TrdCSTStatus);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_BRMCstSetInfo_Empty_On Cnt   ][Rb1 %d, Rb2 %d, Rb3 %d]", nCurRB1Cnt, nCurRB2Cnt, nCurRB3Cnt);
				
				fnSCR_DisplayString(2, "지금은 기기 점검중입니다");
				fnSCR_DisplayString(3, "빠른 시간내에 복구예정이오니");
				fnSCR_DisplayString(4, "잠시만 기다려 주십시오");
				m_pDevCmn->fnSCR_DisplayScreen(701);

				fnUTL_IniSetInt(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedCstInfo", TRUE);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_BRMCstSetInfo_Empty_On] -1");
				fnCSH_Initialize();								
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_BRMCstSetInfo_Empty_On] -2");
				nReturn = TRUE;
			}

		}
	}

	return nReturn;
}

// ATM기동시 초기 한번만 실행시켜줌
int CDevCmn::fnAPL_SetLedInformation()
{
	CString		strImgEffect("");
	CString		strScheduleEffect("");
	CStringArray	strTempArray;
	int		nEnterEffect = 0;
	int		nExitEffect  = 0;
	int		nImageCount	 = 0;
	int		i = 0;
	int		nIndex = 0;
	int		DisplayKind = 0;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetLedInformation");
// 밝기를 조절하는 함수
	fnVFD_SetBrightnessLevel(10);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetLedInformation2 VFDTotalCount[%d]", VFDTotalCount);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetLedInformation2 nEnterEffect[%d], nExitEffect[%d], nImageCount[%d]", nEnterEffect, nExitEffect, nImageCount);
	
	// fnVFD_RealTimeTextDisplay => 텍스트를 ROM에 업데이트 하여 준다
	if (VFDTotalCount)
	{
		nEnterEffect = IniGetInt(_VFD_INI, "LED", "EnterEffect", 0);
		nExitEffect = IniGetInt(_VFD_INI, "LED", "ExitEffect", 0);
		nImageCount = IniGetInt(_VFD_INI, "LED", "ImageCount", 0);
		
		if (VFDTotalCount > LED_TEXT_SAVE_COUNT)
			VFDTotalCount = LED_TEXT_SAVE_COUNT;
		
		if (nImageCount > LED_IMAGE_SAVE_COUNT)
			nImageCount = LED_IMAGE_SAVE_COUNT;
		
		for (i = 0; i < VFDTotalCount; i++)
		{
			strTempArray.Add(IniGetStr(_VFD_INI, "DISPLAY", Int2Asc(i), 0));
			strTempArray[i].TrimRight();
			strTempArray[i].Replace("\"","");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetLedInformation strTempArray[i] = [%s]", strTempArray[i].GetBuffer(0));
			fnVFD_RealTimeTextDisplay(strTempArray[i].GetBuffer(0), i+1, nEnterEffect, 0, 5, 5, 0, "굴림체");      
														//문자열, 인덱스, 입장효과, 용도, 유지시간, 효과속도, 깜빡임, 폰트 
		}
	}
	
	//	fnVFD_RealTimeBitImageDisplay => 이미지를 ROM에 업데이트하여 준다
	strScheduleEffect.Empty();
	
	if (nImageCount)
		DisplayKind = LED_IMAGE_SELECT;
	else
		DisplayKind = LED_TEXT_SELECT;
	
	nIndex = 1;
	for (i = 0; i < (nImageCount+VFDTotalCount); i++)
	{
		if (i == 0)
			strImgEffect.Format("%d/%d/%d/%d/%d/%d", DisplayKind, nIndex, nEnterEffect, 5, nExitEffect, 5);		// 각종효과 = 입장효과||유지시간||퇴장효과||효과속도
		else
			strImgEffect.Format("|%d/%d/%d/%d/%d/%d", DisplayKind, nIndex, nEnterEffect, 5, nExitEffect, 5);	// 각종효과 = 입장효과||유지시간||퇴장효과||효과속도
		
		strScheduleEffect = strScheduleEffect + strImgEffect;
		
		if (nIndex == nImageCount)
		{
			if (DisplayKind == LED_IMAGE_SELECT)
				DisplayKind = LED_TEXT_SELECT;
			nIndex = 1;
		}
		else
			nIndex++;
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetLedInformation strScheduleEffect = [%s]", strScheduleEffect.GetBuffer(0));
	
	if (VFDTotalCount+nImageCount)
		fnVFD_RealTimeTextBitImageDisplay(strScheduleEffect, (VFDTotalCount+nImageCount), 0, 0, 0, 0, 0, "", "", 0, 0);
	// 각종효과, 갯수(1 ~ 15개까지 지정하고 싶은 Text와 이미지 총 개수), 나머지는 기본값 SET
	return TRUE;
}






// OM에서 저장한 Data를 16바이트 단위로 나누어 저장한다
// 한번에 LED에 Display할 수 있는 데이터는 16바이트이다 한글은 8자 혼용시 글자짤림 현상 주의필요함
int CDevCmn::fnAPL_CheckLEDDipalyData(CString strDisplay)
{
	CStringArray	strTempArray;
	CString			strMessage = _T("");
	CString			strTemp;
	BYTE			szTemp[1024];
	INT				nLine		= 0;
	INT				nRow		= 0;
	INT				nCurrentLine= 0;
	char			TmpBuf[16]	= {0,};
	char			szBuffer[1024];
	BYTE			szText[3];
	
	strTempArray.RemoveAll();
	memset(szTemp, NULL, sizeof(szTemp));
	
	memcpy(szTemp, strDisplay.GetBuffer(0), strDisplay.GetLength());
	
	nLine = (strDisplay.GetLength()/16);
	
	if(strDisplay.GetLength() % 16)
		nLine++;
	
	for(int i=0; i<strDisplay.GetLength(); i++)
	{
		memset(szText, 0x00, sizeof(szText));
		
		if(szTemp[i] & 0x80)
		{
			szText[0] = szTemp[i++];
			szText[1] = szTemp[i];
			szText[2] = '\0';
			
			strTemp.Format("%s", szText);
			nRow = nRow + 2;
		}
		else
		{
			strTemp.Format("%c", szTemp[i]);
			nRow++;
		}
		
		strMessage += strTemp;
		
		if(nRow >= 15)
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CheckLEDDipalyData[%s]", strMessage);
			strTempArray.Add(strMessage);
			strMessage = _T("");
			nRow = 0;
			nCurrentLine++;
		}
	}
	
	if(nCurrentLine != nLine)
	{
		strTempArray.Add(strMessage);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CheckLEDDipalyData[%s]", strMessage);
	}
	
	
	DeleteFile(_VFD_INI);
	
	IniSetInt(_VFD_INI, "DISPLAY", "DisplayCount", nLine);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CheckLEDDipalyData[DisplayCount:%d]", nLine);
	
	for(i=0; i<strTempArray.GetSize(); i++)
	{
		sprintf(szBuffer, "%s", strTempArray[i]);
		itoa(i, TmpBuf, 10);
		//		m_pProfile->PutPStr("DISPLAY", TmpBuf, szBuffer, sizeof(szBuffer),	_VFD_INI);
		WritePrivateProfileString("DISPLAY", TmpBuf, strTempArray[i], _VFD_INI);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CheckLEDDipalyData[%s:%s]", TmpBuf, strTempArray[i]);
		
	}
	
	SetShareData("OM_LED", strDisplay);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CheckLEDDipalyData[strDisplay:%s]", strDisplay);
	
	return TRUE;
}


void CDevCmn::fnAPL_MakeVoicePlay(int InitFlg)
{
	int		Ret, i, j, l = 0;
	char	KeyName[4];
	char	Temp[1000];
	char	EditTemp[1000];

	memset(KeyName, 0x00, sizeof(KeyName));
	memset(Temp, 0x00, sizeof(Temp));
	memset(EditTemp, 0x00, sizeof(EditTemp));
	
	CString			HeadString("");
	CString			MiddleString("");
	CString			TailString("");
	CStringArray	EditString;
	CString			DispEditString("");

	if (InitFlg == TRUE)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_MakeVoicePlay : InitFlg(%d)", InitFlg);
		fnAPL_TTSplay();	
		return;
	}	

	Int2Asc(fnSCR_GetCurrentScreenNo(), KeyName, 3, 10, '0');

	Ret = GetPrivateProfileString("VOICE MESSAGE", KeyName, " ", Temp, sizeof(Temp), "C:\\T1Atm\\INI\\VoiceMsg.ini");
	
	if (Ret == 0)
	{		
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "voiceMsg.ini 파일 read실패");
		return;
	}
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "voiceMsg.ini 파일 KeyName:%s Temp:%s", KeyName, Temp);
	}

	int len = strlen(Temp);

	for (i=0;i<len;i++)
	{
		if (Temp[i] == '<')
		{	
			memcpy(EditTemp, Temp, i);
			HeadString = GetString(EditTemp, i, i);
			memset(EditTemp, 0x00, sizeof(EditTemp));
			i += 1;
			for (j=i;j<len;j++)
			{
				if (Temp[j] == '>')
				{
					memcpy(EditTemp, &Temp[i], j);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[!!!] EditTemp:%s", EditTemp);
					MiddleString = GetString(EditTemp, j-i, j-i);
					SplitString(MiddleString, ",", EditString);
					MiddleString = "";
					for (l=0; l<EditString.GetSize(); l++)
					{
// 305번의 화면은 < 와 > 안에 있는 ","의 수가 14개임
// 여기서 < 와 > 안에 있는 버튼은 max 12개로 제한하여 13번, 14번째 은행의 음성은 지움
// 즉, PdisableBtn[13] 과 PdisableBtn[14] 가 FALSE이며 이것을 버튼으로 인식한 것임.
// 따라서, 2개은행을 제외함.
						if (PdisableBtn[l+1] == TRUE)
						{
							MiddleString += EditString[l];
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[!!!] MiddleString:%s", MiddleString);

							if (l == EditString.GetSize())
								;
							else
								MiddleString += ',';
						}
						
					}
					MiddleString.TrimRight();
					j += 1;
					memset(EditTemp, 0x00, sizeof(EditTemp));
					memcpy(EditTemp, &Temp[j], strlen(Temp)-j);		
					TailString = GetString(EditTemp,strlen(EditTemp), strlen(EditTemp));
					break;
				}
			}
			break;
		}
	}

	if (HeadString.IsEmpty())
		HeadString = GetString(Temp, strlen(Temp), strlen(Temp));
	DispEditString += HeadString;
	DispEditString += MiddleString;
	DispEditString += TailString;
	
	int Rsp, Index = 0;
	BOOL StartTran=FALSE;
	CString TempSet,AccountSet,FinalSet;
	AccountSet.Empty();
	TempSet.Empty();
	FinalSet.Empty();
	
	for (int Finddisp=0;Finddisp<BackDispCnt;Finddisp++)
	{
		Rsp = 0;
		Rsp = DispEditString.Find("//", Index);
		if (Rsp != -1)
		{
			if (PbackDisp[Finddisp].Index)
			{
				DispEditString.Insert(Rsp,"/");
				DispEditString.Replace("///", PbackDisp[Finddisp].DispData);
				Index = Rsp;
			}
		}

	}
	
	////////////////////////////////////////////////////////////////////////////
	TempSet=DispEditString;
		
	if (TempSet.Find("&")!= -1)
	{
		for(i=0;i<TempSet.GetLength();i++)
		{
			if(TempSet.GetAt(i)=='&')	StartTran=!StartTran;
			
			if(StartTran)	AccountSet=AccountSet+TempSet.GetAt(i);
		}
		if( AccountSet.Find("-") == -1)
		{
			for(i=0;i<AccountSet.GetLength();i++)
				FinalSet=FinalSet+AccountSet.GetAt(i)+" ";
			DispEditString.Replace(AccountSet,FinalSet);
			DispEditString.Replace("&"," ");
		}

	}
	// 나이스 장애인 NG코드 수정
	if (TempSet.Find("@")!= -1)
	{
		DispEditString.Replace("0","0 ");
		DispEditString.Replace("1","1 ");
		DispEditString.Replace("2","2 ");
		DispEditString.Replace("3","3 ");
		DispEditString.Replace("4","4 ");
		DispEditString.Replace("5","5 ");
		DispEditString.Replace("6","6 ");
		DispEditString.Replace("7","7 ");
		DispEditString.Replace("8","8 ");
		DispEditString.Replace("9","9 ");

		DispEditString.Replace("@"," ");

		DispEditString.Replace("-", "다시");
	}
	DispEditString.Replace("&"," ");
	DispEditString.Replace("["," ");					// V09.43.00-#07
	DispEditString.Replace("]"," ");					// V09.43.00-#07
	/////////////////////////////////////////////////////////////////////////////
	
	// PowerTTS-S 음성엔진
	memset(m_pText, 0x00, sizeof(m_pText));
	memcpy(m_pText, DispEditString.GetBuffer(0),DispEditString.GetLength());
	fnAPL_TTSplay();	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "MakeVoice Return");

}


// 전자저널시작
int CDevCmn::fnAPL_ECashEjrSpoolPrint(int nDevId, LPCTSTR szSpoolData, NEjrTblDump* pEjrData, EjrAmountTbl* pEjrAmountData)
{

	char		szTempFileName[256], szYYMMDD[9], szHHMMSS[13], szDummy0[1024], szDummy1[1024], szDummy2[1024];
	char		szKey[512];
	char		szEncBuffer[20480] = {'0',};
	CFileStatus	TempFileStatus;
	FILE*		TempFp;
	int nSize = 0;
	int nEnc = 0;
	long nEncOutlen = 0;
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);	

	memset(&EEjrD, ' ', sizeof(EEjrD));								// 전자저녈영역
	memset(&EjrAmount, 0, sizeof(EjrAmount));					// 전자저녈금액관련자료 : 2003.12.15
																// Key Area Setting

	if (nSize <= 0)												// 길이구하기&보정
		nSize = strlen(szSpoolData);
	if (nSize > JNL_MAX_DATA)
		nSize = JNL_MAX_DATA;

	memcpy(EEjrD.Dummy, GetString((LPSTR)szSpoolData, nSize, nSize).GetBuffer(0), nSize);
	
	EEjrD.Cr = A_CR;
	EEjrD.Lf = A_LF;


	if (nDevId & DEV_JPR)										// 저널자료존재시
	{
		fnAPL_EjrAddSerialNo();									// 전자저널일련번호증가															// Key Area Setting
		EEjrD.Fill1 = '[';
		sprintf(szYYMMDD, "%2.2s/%2.2s/%2.2s", &GetDate().GetBuffer(0)[2], &GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6]);
		memcpy(EEjrD.Date, szYYMMDD, sizeof(EEjrD.Date));
		sprintf(szHHMMSS, "%2.2s:%2.2s:%2.2s.%03d", &GetTime().GetBuffer(0)[0], &GetTime().GetBuffer(0)[2], &GetTime().GetBuffer(0)[4], sTime.wMilliseconds);
		memcpy(EEjrD.Time, szHHMMSS, sizeof(EEjrD.Time));
		EEjrD.Fill3 = ']';	
		
		sprintf(szDummy0, "[%8.8s %12.12s] ", szYYMMDD, szHHMMSS);

		if (m_pTranCmn->MenuIrdaMode)
			memcpy(EEjrD.TranMaterial, " MOBILE TR", 9);
		else
		if (m_pTranCmn->CardTran & MENU_MCU3)
		{
			if (m_pTranCmn->MenuFICMode & MENU_IC)
				memcpy(EEjrD.TranMaterial, " IC CARD 3", 9);
			else
			{
				if (m_pTranCmn->CardTran & MENU_MCU2 && m_pTranCmn->CardTran & MENU_J3T3)
					memcpy(EEjrD.TranMaterial, " MSCARD23", 9);
				else
				if (m_pTranCmn->CardTran & MENU_J3T3)
					memcpy(EEjrD.TranMaterial, " MSCARD 3", 9);
				else
				if (m_pTranCmn->CardTran & MENU_MCU2)
					memcpy(EEjrD.TranMaterial, " MSCARD 2", 9);
				else
					memcpy(EEjrD.TranMaterial, " MSCARD -", 9);
			}
		}
		else
		if (m_pTranCmn->CardTran & MENU_MCU2)
			memcpy(EEjrD.TranMaterial, " MSCARD 2", 9);
		else
		if (m_pTranCmn->PbTran)
			memcpy(EEjrD.TranMaterial, " PBM TRAN", 9);
		else
			memcpy(EEjrD.TranMaterial, " 운영업무", 9);



		_mkdir(_EJR_DIR);											// Dir생성
		_mkdir(_EJR_TXT);
		sprintf(szTempFileName, "%s\\%8.8s.JNL", _EJR_TXT, m_pProfile->TRANS.YYYYMMDD);
		
		if(!CFile::GetStatus(szTempFileName, TempFileStatus))		// File존재시날자검증
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_HNetEjrSpoolPrint() File Err T_OK");
		}

		TempFp = fopen(szTempFileName, "a");						// #H0011	
		if(TempFp == NULL)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_HNetEjrSpoolPrint() File NULL Pointer T_OK");
			return T_OK;
		}

		nEnc = RegGetInt(_REGKEY_REMOTECTRL, "ECash_JNL_EncFlag", 0);
		sprintf(szKey, "%8.8s",	GetDate().GetBuffer(0));
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_HNetEjrSpoolPrint Key(%s), AtmNumber(%4.4s)", szKey, &m_pProfile->NETWORK.AtmSerialNum[4]);
//		nEncOutlen = enc(&EEjrD.Fill1, szEncBuffer, (sizeof(EEjrD) + 16) * 2, szKey, &m_pProfile->NETWORK.AtmSerialNum[4]);
//		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_HNetEjrSpoolPrint PlanText", &EEjrD.Fill1, sizeof(EEjrD));
//		HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_HNetEjrSpoolPrint EncrText", szEncBuffer, sizeof(EEjrD));
		
		if(nEnc)
		{

//			sprintf(szDummy1, "%50.50s", EEjrD.Dummy);
//			nEncOutlen = enc(szDummy1, szEncBuffer, (strlen(szDummy1) + 16) * 2, szKey, &m_pProfile->NETWORK.AtmSerialNum[4]);
//			memcpy(szDummy2, szDummy0, strlen(szDummy0));
//			memcpy(&szDummy2[strlen(szDummy0)], szEncBuffer, nEncOutlen);
//			szDummy2[strlen(szDummy0) + nEncOutlen] = A_CR;
//			szDummy2[strlen(szDummy0) + nEncOutlen + 1] = A_LF;	
//			fwrite(szDummy2, strlen(szDummy0) + nEncOutlen + 2, 1, TempFp);

			sprintf(szDummy1, "%50.50s%c%c", EEjrD.Dummy, EEjrD.Cr, EEjrD.Lf);
			memcpy(szDummy2, szDummy0, strlen(szDummy0));
			memcpy(&szDummy2[strlen(szDummy0)], szDummy1, strlen(szDummy1));
			fwrite(szDummy2, strlen(szDummy0) + strlen(szDummy1), 1, TempFp);

		}
		else
		{
			sprintf(szDummy1, "%50.50s%c%c", EEjrD.Dummy, EEjrD.Cr, EEjrD.Lf);
			memcpy(szDummy2, szDummy0, strlen(szDummy0));
			memcpy(&szDummy2[strlen(szDummy0)], szDummy1, strlen(szDummy1));
			fwrite(szDummy2, strlen(szDummy0) + strlen(szDummy1), 1, TempFp);
		}
			
		fclose(TempFp);

	}

	return TRUE;
}
