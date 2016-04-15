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


// �����ʱ�ȭ
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

	if(IniGetStr(_SYSTEMCDINI, "SYSTEM", "NAME").Find("WIN7") >= 0)   // WIN7 ����
		OSType = OS_WIN7;
	else
		OSType = 0;
	
	//#0033
//	IniSetInt(_TRANS_INI,  TRANS_SEC, "TaMaxDispAmt", 30);					//���� 30
//	IniSetInt(_TRANS_INI,  TRANS_SEC, "JaMaxDispAmt", 100);					//���� 100
//	IniSetInt(_DEVICE_INI, DEV_SEC, "ETCDevice02", 1);						//�����ġ	

	int nMachineType = IniGetInt(_DEVICE_INI,DEV_SEC, "MachineType", 0);

	if(nMachineType == U3100K)
	{

		IniSetInt(_DEVICE_INI,DEV_SEC, "ETCDevice10", 1); //������ V15 Ver �̻� 5���� �ŷ� 
		RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwConvPCU2LCU", 15);
		RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwCompoundOptions", 2);
		//Kim.Gi.Jin 03.30 NICE ********************************************************************************
//		IniSetInt(_DEVICE_INI, DEV_SEC, "ETCDevice20", KIUP);					//KIUP Brand  TEST	#N0215 �������� U3100K ����
		RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwCuMove2RJIfCuEmpty", 3); //ī��Ʈ Empty ��� ����
		RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwMaxDispenseCount", 100); //�ִ����ݾ�
		IniSetInt(_DEVICE_INI, DEV_SEC, "FingerHandler", NOT_SET);					 //
		RegSetInt("SOFTWARE\\ATM\\DevInfo\\SIU\\CONF", "DOOR_CABINET_OPEN",  141); 
		RegSetInt("SOFTWARE\\ATM\\DevInfo\\SIU\\CONF", "DOOR_CABINET_CLOSE", 142); 
		RegSetInt("SOFTWARE\\ATM\\DevInfo\\SIU\\CONF", "DOOR_SAFE_OPEN",  59); 
		RegSetInt("SOFTWARE\\ATM\\DevInfo\\SIU\\CONF", "DOOR_SAFE_CLOSE", 60); 
		RegSetInt("SOFTWARE\\ATM\\DEVINFO\\IDC", "ICEntryTimeout", 4); 

		
		RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwResetControl", 0); //Mwi - V10.10.21 (CSM �� ��ü �ܷ� ����) -> �ڵ���� Flag //#0122

		int	nWithdraw50TFlag = IniGetInt(_DEVICE_INI,DEV_SEC, "ETCDevice10", 0); //#0057
		if (nWithdraw50TFlag)
		{
			nNoteKind1 = BRM_NOTEKIND_50000;				// �������� 
			nNoteKind2 = BRM_NOTEKIND_10000_NEW;			// �Ÿ����� 
			nNoteKind3 = BRM_NOTEKIND_10000_NEW;			// �Ÿ����� 
			nNoteKind4 = BRM_NOTEKIND_10000_OLD;			//  

			RegSetInt(_REGKEY_BRMSTATUS, "RB1NoteKind", nNoteKind1);
			RegSetInt(_REGKEY_BRMSTATUS, "RB2NoteKind", nNoteKind2);
			RegSetInt(_REGKEY_BRMSTATUS, "RB3NoteKind", nNoteKind3);
			RegSetInt(_REGKEY_BRMSTATUS, "RB4NoteKind", nNoteKind4);

			//#0057
			RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwAccStopIfTypeAllNotAvail", 0); //�̱��� Reject ó�� 
			RegSetInt("SOFTWARE\\ATM\\DevInfo\\BRM\\DEVICE_INFORMATION\\SYSCONFIG", "dwRejectControl", 0);

			RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwConvPCU2LCU", 15);
			RegSetInt("SOFTWARE\\ATM\\DEVINFO\\BRM\\COMMON", "dwCompoundOptions", 2);
			
			IniSetStr(_TRANS_INI, TRANS_SEC, "ClerkTransOnOff",  "(99)");  
		}
		else
		{
			nNoteKind1 = BRM_NOTEKIND_10000_NEW;			// �Ÿ����� 
			nNoteKind2 = BRM_NOTEKIND_10000_NEW;			// �Ÿ����� 
			nNoteKind3 = BRM_NOTEKIND_INPUT_DEP_ONLY;		// �Ա����� 
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
//	fnAPL_DeleteFiles(_NHNW30_TESTPAGE_DIR);        //#0053  - ���� ���� (Installer) - 20130301
//	fnAPL_DeleteFiles(_NHNW20_TESTPAGE_DIR);        //#0053  - ���� ���� (Installer) - 20130301
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
		if(OSType == OS_WIN7)					//#N0228 WIN7 ����
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
				if(OSType == OS_WIN7)									//#N0228 WIN7 ����
					RegSetValueEx(hKeyResult,"HungAppTimeout",0,REG_SZ,(CONST BYTE*)"10000",5);
				else
					RegSetValueEx(hKeyResult,"HungAppTimeout",0,REG_SZ,(CONST BYTE*)"1000",5);
		  }
	}
	// Close Key 
	RegCloseKey(hKeyResult);
	RegSetStr("SYSTEM\\CurrentControlSet\\Control", "WaitToKillServiceTimeout", "1000");
//////////////////////////////////////////////////////////////////////////////////////////////////////////

	fnAPL_ClerkInitModeSet();						// ����ʱ�ȭ��弳��
	DeleteFiles(_LOG_DIR);							// File����(1�ޱ���)
	DeleteFiles(_TRACE_DIR);

	fnAPL_DeleteFiles(_NHTM_DIR);
	fnAPL_DeleteFiles(_HBRM_DIR);	

	m_pProfile->InitProfileCheck();					// Profile��������üũ&����
    m_pProfile->GetDeviceProfile();					// Profile�б� 
	m_pProfile->GetBRMProfile();
	m_pProfile->GetUCMProfile();
	m_pProfile->GetNetworkProfile();
	m_pProfile->GetTransProfile();

	if (m_pProfile->DEVICE.MachineType == U8100)
		m_pProfile->GetLCDInfoProfile();						
	
	Strcpy(m_pProfile->TRANS.ApVersion, VERSION, 9); // Version��� : VNN-NN-NN(9�ڸ�Moveó��)
	

	strCode.Format("%8.8s", &m_pProfile->NETWORK.AtmSerialNum[0]);
	IniSetStr("D:\\INI\\ATM_CFG.ini", ATM_CFG_SEC, "ATM_ID", strCode);

	strTmp = IniGetStr("D:\\INI\\NETWORK.ini", "NETWORK", "BranchNum", "(00000096)");
	strCode.Format("%2.2s%8.8s", strTmp.Mid(7, 2), &m_pProfile->NETWORK.AtmSerialNum[0]);
	IniSetStr("D:\\INI\\ATM_CFG.ini", ATM_CFG_SEC, "F_ATM_ID", strCode);
	
	m_pProfile->PutTransProfile();
	m_pProfile->GetWithTotal();						// �հ�ǥ�б�				
	m_pProfile->GetTransTotal();
	m_pProfile->GetDepTotal();
	m_pProfile->GetMoneyTotal();								// �����/���հ�

	// Atm��ⱸ�� ���κ���(����Ұ�)
	// m_pProfile->DEVICE�� �����ܿ����� ��� �Ұ�
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

	// (SP����)
	if ((OKI_SET == AtmDefine.CashHandler) || (HBRM_SET == AtmDefine.CashHandler)) // U8100 AP ���泻�� #04 - HBRM���� ���泻��
	{

		CString			strOMDepositInfo("");
		CStringArray	strOMDepositInfoArray;
		CString			strTemp = "";
		DWORD			dwData = 0;
		LONG			lRet;
		HKEY			hKey;
		DWORD			dwDisp;
		DWORD			dwSize;

		if(AtmDefine.MachineType == U8100) //���� Brand - ��� 
		{

			RegSetInt(_REGKEY_BRMINFO, "�������ܷ�����ż�",	    0);
			RegSetInt(_REGKEY_BRMINFO, "�н�ȸ�����ִ�ż�",	OKI_MAXFRJBCNT);
			RegSetInt(_REGKEY_BRMINFO, "���ȸ����1�ִ�ż�",	HBRM_MAXRJB1CNT);
			RegSetInt(_REGKEY_BRMINFO, "���ȸ����1�ѵ��ż�",	HBRM_MINRJB1CNT);
			RegSetInt(_REGKEY_BRMINFO, "���ȸ����2�ִ�ż�",	HBRM_MAXRJB2CNT);
			RegSetInt(_REGKEY_BRMINFO, "���ȸ����2�ѵ��ż�",	HBRM_MINRJB2CNT);

			if(m_pProfile->DEVICE.ETCDevice20 == KWANGJU || m_pProfile->DEVICE.ETCDevice20 == KYONGNAM)	//#N0181 => #N0192  ����&�泲 �귣�� �Ա� �ִ�ż� 150
				RegSetInt(_REGKEY_BRMINFO, "�Ա��ִ�ż�",			BRM_MAXCASHINCNT_150);
			else
				RegSetInt(_REGKEY_BRMINFO, "�Ա��ִ�ż�",			OKI_MAXCASHINCNT);
			
			RegSetInt(_REGKEY_BRMINFO, "����ִ�ż�",			    100); ////#N0140
			RegSetInt(_REGKEY_BRMINFO, "���ݹڽ��ִ�ż�",		OKI_MAXRBCNT);
			RegSetInt(_REGKEY_BRMINFO, "���ݺ����˸��ż�",		BRM_AREFILLCONDCNT);
			RegSetInt(_REGKEY_BRMINFO, "�����ڵ�����ż�",		BRM_AREFILLCNT); //#0089
			RegSetInt(_REGKEY_BRMINFO, "�����ڵ�ȸ���ż�",		BRM_ATAKEUPCNT);

			RegSetInt(_REGKEY_BRMINFO, "���������ܷ�����ż�",	    0);
			RegSetInt(_REGKEY_BRMINFO, "������������ִ�ż�",	    20); //5*14 = 70���� -> #N0140 (��� 10����)
			RegSetInt(_REGKEY_BRMINFO, "�����������ݺ����˸��ż�",	60); //#N0140
			RegSetInt(_REGKEY_BRMINFO, "�������ǹڽ��ִ�ż�",	OKI_50T_MAXRBCNT);

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
			RegSetInt(_REGKEY_BRMINFO, "���ȸ����1�ִ�ż�",	800); //Reject
			RegSetInt(_REGKEY_BRMINFO, "���ȸ����1�ѵ��ż�",	800);
			RegSetInt(_REGKEY_BRMINFO, "���ȸ����2�ִ�ż�",	200);
			RegSetInt(_REGKEY_BRMINFO, "���ȸ����2�ѵ��ż�",	200);
			RegSetInt(_REGKEY_BRMINFO, "������������ִ�ż�",	OKI_50T_MAXCASHOUTCNT);
			RegSetInt(_REGKEY_BRMINFO, "���������ܷ�����ż�",	    0);
			
			RegSetInt(_REGKEY_BRMINFO, "�Ա��ִ�ż�",			OKI_MAXCASHINCNT);
			RegSetInt(_REGKEY_BRMINFO, "����ִ�ż�",			OKI_MAXCASHINCNT);
			RegSetInt(_REGKEY_BRMINFO, "�������ܷ�����ż�",	0);
			RegSetInt(_REGKEY_BRMINFO, "�н�ȸ�����ִ�ż�",	OKI_MAXFRJBCNT);

		}
		else
		{
			RegSetInt(_REGKEY_BRMINFO, "�������ܷ�����ż�",	OKI_REVISECASHCNT);
			RegSetInt(_REGKEY_BRMINFO, "�н�ȸ�����ִ�ż�",	OKI_MAXFRJBCNT);
			RegSetInt(_REGKEY_BRMINFO, "���ȸ����1�ִ�ż�",	HBRM_MAXRJB1CNT);
			RegSetInt(_REGKEY_BRMINFO, "���ȸ����1�ѵ��ż�",	HBRM_MINRJB1CNT);
			RegSetInt(_REGKEY_BRMINFO, "���ȸ����2�ִ�ż�",	HBRM_MAXRJB2CNT);
			RegSetInt(_REGKEY_BRMINFO, "���ȸ����2�ѵ��ż�",	HBRM_MINRJB2CNT);
			RegSetInt(_REGKEY_BRMINFO, "�Ա��ִ�ż�",			OKI_MAXCASHINCNT);
			RegSetInt(_REGKEY_BRMINFO, "����ִ�ż�",			OKI_MAXCASHOUTCNT);
			RegSetInt(_REGKEY_BRMINFO, "���ݹڽ��ִ�ż�",		OKI_MAXRBCNT);
			RegSetInt(_REGKEY_BRMINFO, "���ݺ����˸��ż�",		BRM_AREFILLCONDCNT);
			RegSetInt(_REGKEY_BRMINFO, "�����ڵ�����ż�",		BRM_AREFILLCNT);
			RegSetInt(_REGKEY_BRMINFO, "�����ڵ�ȸ���ż�",		BRM_ATAKEUPCNT);

			RegSetInt(_REGKEY_BRMINFO, "���������ܷ�����ż�",	OKI_50T_REVISECASHCNT);
			RegSetInt(_REGKEY_BRMINFO, "������������ִ�ż�",	OKI_50T_MAXCASHOUTCNT);
			RegSetInt(_REGKEY_BRMINFO, "�����������ݺ����˸��ż�",	OKI_50T_AREFILLCONDCNT); 
			RegSetInt(_REGKEY_BRMINFO, "�������ǹڽ��ִ�ż�",	OKI_50T_MAXRBCNT);
		}


		strOMDepositInfo = GetShareData("OM_�Աݱ�������");		// �Աݰ��ɱ���
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[OM_�Աݱ�������][%s]", strOMDepositInfo);
		if ("" == strOMDepositInfo)
		{
			lRet = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE,
									_REGKEY_BRMINFO,
									0, NULL, REG_OPTION_NON_VOLATILE,
									KEY_READ, NULL, &hKey, &dwDisp);
			if (ERROR_SUCCESS == lRet)
			{
				dwSize = sizeof(DWORD);
				lRet = ::RegQueryValueEx(hKey, "�Աݰ��ɱ���", 0, NULL, (LPBYTE)&dwData, &dwSize);
				::RegCloseKey(hKey);

				if (ERROR_SUCCESS == lRet)
				{
					if (dwData & DEP_AVAIL_10000_OLD)
						strTemp += "����||";
					else
						strTemp += "�Ұ�||";

					if (dwData & DEP_AVAIL_5000_OLD)
						strTemp += "����||";
					else
						strTemp += "�Ұ�||";

					if (dwData & DEP_AVAIL_1000_OLD)
						strTemp += "����||";
					else
						strTemp += "�Ұ�||";

					if (dwData & DEP_AVAIL_10000_NEW)
						strTemp += "����||";
					else
						strTemp += "�Ұ�||";

					if (dwData & DEP_AVAIL_5000_NEW)
						strTemp += "����||";
					else
						strTemp += "�Ұ�||";

					if (dwData & DEP_AVAIL_1000_NEW)
						strTemp += "����||";
					else
						strTemp += "�Ұ�||";

					if (dwData & DEP_AVAIL_50000)
						strTemp += "����||";
					else
						strTemp += "�Ұ�||";

					strTemp += "����";	
				}
				else
				{
					if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
					{
						dwData = DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;
						strTemp = "�Ұ�||�Ұ�||�Ұ�||�Ұ�||�Ұ�||�Ұ�||����||����";	
					}
					else
					if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
					{
						dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;	
						strTemp = "����||����||����||����||����||����||����||����";	
					}
					else
					{
						dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;
						strTemp = "����||����||����||����||����||����||����||����";	
					}

					lRet = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE,
											_REGKEY_BRMINFO,
											0, NULL, REG_OPTION_NON_VOLATILE,
											KEY_ALL_ACCESS, NULL, &hKey, &dwDisp);
					if (ERROR_SUCCESS == lRet)
						::RegSetValueEx(hKey, "�Աݰ��ɱ���", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(int));
					::RegCloseKey(hKey);

				}
			}
			else
			{
				if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
				{
					dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;
					strTemp = "����||����||����||����||����||����||����||����";	
				}
				else
				if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
				{
					dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;	
					strTemp = "����||����||����||����||����||����||����||����";	
				}
				else
				if(m_pProfile->DEVICE.ETCDevice20 == KEB)
				{
					dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;	
					strTemp = "����||����||����||����||����||����||����||����";	
				}
				else
				{
					dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;
					strTemp = "����||����||����||����||����||����||����||����";	
				}

				lRet = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE,
										_REGKEY_BRMINFO,
										0, NULL, REG_OPTION_NON_VOLATILE,
										KEY_ALL_ACCESS, NULL, &hKey, &dwDisp);
				if (ERROR_SUCCESS == lRet)
					::RegSetValueEx(hKey, "�Աݰ��ɱ���", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(int));
				::RegCloseKey(hKey);	
			}

			SetShareData("OM_�Աݱ�������", strTemp);	
			m_pProfile->BRM.DepositCashKind = dwData;
			m_pProfile->PutBRMProfile();	
		}
		else
		{

			if(m_pProfile->DEVICE.ETCDevice20 == NONGHYUP)
			{
				dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;
				strTemp = "����||����||����||����||����||����||����||����";	
			}
			else
			if(m_pProfile->DEVICE.ETCDevice20 == KIUP)
			{
				dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;	
				strTemp = "����||����||����||����||����||����||����||����";	
			}
			else
			if(m_pProfile->DEVICE.ETCDevice20 == KEB)
			{
				dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;	
				strTemp = "����||����||����||����||����||����||����||����";	
			}
			else
			{
				dwData = DEP_AVAIL_10000_OLD | DEP_AVAIL_5000_OLD | DEP_AVAIL_1000_OLD | DEP_AVAIL_10000_NEW | DEP_AVAIL_5000_NEW | DEP_AVAIL_1000_NEW | DEP_AVAIL_50000;
				strTemp = "����||����||����||����||����||����||����||����";	
			}

			RegSetInt(_REGKEY_BRMINFO, "�Աݰ��ɱ���", dwData);		
			SetShareData("OM_�Աݱ�������", strTemp);	
			m_pProfile->BRM.DepositCashKind = dwData;
			m_pProfile->PutBRMProfile();

		}
	}
	else
	{
		IniSetInt(_SP_DEVICE_INI, "BRM", "MaxCashOutCnt",		BRM_MAXCASHOUTCNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "MaxCashInCnt",		BRM_MAXCASHINCNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "DistinctionMode",		BrmCashMode);
		IniSetInt(_SP_DEVICE_INI, "BRM", "���ݹڽ��ִ�ż�",	BRM_MAXRBCNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "RJB1�ִ�ż�",		BRM_MAXRJB1CNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "RJB1�ѵ��ż�",		BRM_MAXRJB1CNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "RJB2�ִ�ż�",		BRM_MAXRJB2CNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "RJB2�ѵ��ż�",		BRM_MANRJB2CNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "�������ܷ�����ż�",	BRM_FIXCASHCNT);
		IniSetStr(_SP_DEVICE_INI, "BRM", "ShutterSpeed",		"���");
		IniSetInt(_SP_DEVICE_INI, "BRM", "���ݺ����˸��ż�",	BRM_AREFILLCONDCNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "����ȸ���˸��ż�",	BRM_ATAKEUPCONDCNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "�����ڵ�����ż�",	BRM_AREFILLCNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "�����ڵ�ȸ���ż�",	BRM_ATAKEUPCNT);
		IniSetInt(_SP_DEVICE_INI, "BRM", "�����ܷ��Ҹż�",		BRM_NEARCASHCNT);
		if (m_pProfile->BRM.DepositCashKind == 0)					// V200 : �Աݱ�������
			IniSetInt(_SP_DEVICE_INI, "BRM", "RJB2�����Ǹ��Աݰ��ɸż�", BRM_MAXRJB2DEPCNT);
		else
			IniSetInt(_SP_DEVICE_INI, "BRM", "RJB2�����Ǹ��Աݰ��ɸż�", 0);
	}

	fnSCR_SetCurrentLangMode(KOR_MODE);	
	
	if(m_pProfile->DEVICE.MachineType == U3100K)
		SprRowOption = SPR_MAX_ROW;
	else
	if(m_pProfile->DEVICE.MachineType == U8100)
		SprRowOption = SPR_MAX_ROW_U8100;
	else
		SprRowOption = SPR_MAX_ROW;

	AtmDoing			= FALSE;					// Atm���࿩��
	AtmFirstDoingFlag	= FALSE;					// Atm���ʽ���Flag
	AtmStatus			= ATM_INIT;					// Atm���º��� 
	AtmStatusSave		= ATM_INIT;					// Atm�������º��� 
	AtmStatusReEntryFlag= FALSE;					// Atm����������Flag
	HostStatus			= HOST_INIT;				// Host���º��� 
	HostStatusSave		= HOST_INIT;				// Host�������º��� 
	HostFirstOpenFlag	= FALSE;					// Host���ʰ���Flag
	HostOpenFlag		= FALSE;					// Host����Flag
	HostFirstReadyFlag	= FALSE;					// Host���ʻ���Flag
	PowerOffSendFlag	= FALSE;					// PowerOFf�۽�Flag -> #CS0001
	TranStatus			= TRAN_IDLE;				// Tran���º��� 
	TranStatusSave		= TRAN_IDLE;				// Tran�������º��� 
	TranProc			= TRAN_NULL;				// Tran�ŷ�ó��
	TranResult			= FALSE;					// Tran�ŷ����

	DeviceDefine		= DEV_NONE;					// ��⿡ ���ǵ�	Device Set
	DeviceNoDevice		= DEV_NONE;					// �̿���			Device Set
	DeviceDownDevice	= DEV_NONE;					// �ٿ�				Device Set
	DeviceStatus		= DEV_NONE;					// ��ְ� ��		Device Set
	DeviceAutoOff		= DEV_NONE;					// �ڵ����� ��	Device Set
	DeviceRetract		= DEV_NONE;					// ȸ���� ��		Device Set
	DeviceTran			= DEV_NONE;					// ���� Event		Device Set 
	DeviceTimeout		= DEV_NONE;					// �ð��ʰ�			Device set
	DeviceLoss			= DEV_NONE;					// �̼������		Device Set

	ForceDownFlag		= FALSE;					
	RemoteDownFlag      = FALSE;				    
	ForceMissMatchDownFlag		= FALSE;					
		

	memset(m_pTranCmn->pArrDataFieldArr, 0x00, sizeof(m_pTranCmn->pArrDataFieldArr));
	m_pTranCmn->nSendDataIndex = 0;


	if (HBRM_SET == AtmDefine.CashHandler)			// U8100-USB��ġ�� ��� �����ָ� ����ð��� ������
	{
		SprRecvTime			= MAX_DEVRSP_TIME;		// Spr����Time MIN_DEVRSP_TIME -> MAX_DEVRSP_TIME����(U8100)
		JprRecvTime			= MAX_DEVRSP_TIME;		// Jpr����Time MIN_DEVRSP_TIME -> MAX_DEVRSP_TIME����(U8100)
		McuRecvTime			= MAX_DEVRSP_TIME;		// Mcu����Time MIN_DEVRSP_TIME -> MAX_DEVRSP_TIME����(U8100)
		BrmRecvTime			= MAX_DEVRSP_TIME;		// Brm����Time MIN_DEVRSP_TIME -> MAX_DEVRSP_TIME����(U8100)
		UcmRecvTime			= MID_DEVRSP_TIME;		// Ucm����Time
		PbmRecvTime			= MAX_DEVRSP_TIME;		// Pbm����Time MIN_DEVRSP_TIME -> MAX_DEVRSP_TIME����(U8100)
		RfdRecvTime			= MIN_DEVRSP_TIME;		// Rfd����Time
		IcmRecvTime			= MIN_DEVRSP_TIME;		// Icm����Time
	}
	else
	{
		SprRecvTime			= MID_DEVRSP_TIME;		// Spr����Time
		JprRecvTime			= MID_DEVRSP_TIME;		// Jpr����Time
		McuRecvTime			= MID_DEVRSP_TIME;		// Mcu����Time
		BrmRecvTime			= MID_DEVRSP_TIME;		// Brm����Time
		UcmRecvTime			= MID_DEVRSP_TIME;		// Ucm����Time
		PbmRecvTime			= MID_DEVRSP_TIME;		// Pbm����Time
		RfdRecvTime			= MIN_DEVRSP_TIME;		// Rfd����Time
		IcmRecvTime			= MIN_DEVRSP_TIME;		// Icm����Time
	}

	
	OpenKey				= TRUE;						// '�غ�' '�' ��ȯ����ġ 
	ClerkKey			= 0;						// ������� Ű ����
	PowerOffDetect		= POWER_NORMAL;				// Power ���º���
	RecoveryType		= 0;						// 0:�ڵ�����,1:��������
	TranType			= 0;						// 0:���ŷ���,1:�������(��ּ۽Ű���)

	CurErrCnt			= 0;						// ������ְ���
	memset(&CurErrBuff, 0, sizeof(CurErrBuff));		// ������ֳ���
	memset(&FirstErrBuff, 0, sizeof(FirstErrBuff));	// ������ֳ���
	memset(&LastErrBuff, 0, sizeof(LastErrBuff));	// ������ֳ���
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

	BrmChangeLoc		= NO_CHANGE;				// Brm����ġ�������
	BrmLoc				= NORMAL_POS;				// Brm����ġ����
	RejectCSTLoc		= NORMAL_POS;				// ȸ��������ġ����
	RetractCSTLoc		= NORMAL_POS;				// ȸ��������ġ����
	ClerkCSTLoc			= NORMAL_POS;				// ����������ġ����
	FstCSTLoc			= NORMAL_POS;				// 1ī��Ʈ����ġ����(Cd)
	SndCSTLoc			= NORMAL_POS;				// 2ī��Ʈ����ġ����(Cd)
	TrdCSTLoc			= NORMAL_POS;				// 3ī��Ʈ����ġ����(Cd)
	FthCSTLoc			= NORMAL_POS;				// Fourth CST Location(Cd)
	UcmLoc				= NORMAL_POS;				// Ucm����ġ����
	DoorStatus			= DOOR_CLOSED;				// DoorStatus����
	CabinetStatus		= DOOR_CLOSED;				// CabinetStatus����

	SlipHeadStatus		= HEADER_NORMAL;			// ��ǥCover����
	JnlHeadStatus		= HEADER_NORMAL;			// ����Cover����
	SlipStatus			= SLIP_NORMAL;				// ��ǥ����
	JnlStatus			= JNL_NORMAL;				// ���λ���
	A4pStatus			= HEADER_NORMAL;			// A4��������
	A4pHeadStatus		= HEADER_NORMAL;			// A4��������

	GSensorStatus       = NORMAL_POS;				// ��ݼ�������
	GExternalSensorStatus1 = NORMAL_POS;            // �ܺμ���1
	GExternalSensorStatus2 = NORMAL_POS;            // �ܺμ���2
	GAlarmStatus        = NORMAL_POS;				// �˶���������
	GSensorLelvel       = NORMAL_POS;				// ��ݼ�������
	GBatteryStatus		= NORMAL_POS;				// �嵥����������
	RejectCSTStatus		= CST_NORMAL;				// ȸ��ī��Ʈ����
	RetractCSTStatus	= CST_NORMAL;				// ȸ��ī��Ʈ����
	ClerkCSTStatus		= CST_NORMAL;				// ���ī��Ʈ�Ի���
	ClerkRejectBoxStatus= CST_NORMAL;				// ����RejectBox����		V07-00-00-#01 �������� ����
	RecycleBoxStatus	= CST_NORMAL;				// ���ݻ���
	RecycleBox50000Status = CST_NORMAL;				// ���ݻ���					V07-00-00-#01 �������� ����
	FstCSTStatus		= CST_NORMAL;				// 1ī��Ʈ����(Cd)
	SndCSTStatus		= CST_NORMAL;				// 2ī��Ʈ����(Cd)
	TrdCSTStatus		= CST_NORMAL;				// 3ī��Ʈ����(Cd)
	FthCSTStatus		= CST_NORMAL;				// 4ī��Ʈ����(Cd)
	CheckCSTStatus		= CST_NORMAL;				// ��ݼ�ǥ����
	CheckCSTStatusSensor= CST_NORMAL;				// ��ݼ�ǥ����(�ǹ�)
	CheckCSTStatusReg	= CST_NORMAL;				// ��ݼ�ǥ����(���)
	CassetteStatus		= CST_NORMAL;				// �Աݼ�ǥ����

	CaptureLastIndex	= 0;						// ī�޶���������Index
	CameraStatus		= CST_NORMAL;				// ī�޶����

	
	DesBoardStatus		= FALSE;					// Des Board Status
	DesITKStatus		= FALSE;					// Des ITK Status
	DesTMKStatus		= FALSE;					// Des TMK Status
	memset(DesRPIN, 0, sizeof(DesRPIN));			// Des RPIN
	memset(UserDefineSetErrCode, ' ', sizeof(UserDefineSetErrCode));			// UserDefineSet Err (Van-CD Spec)
	DesTMKTryFlag		= FALSE;					// Des TMK Try Flag
	DesTranStatus		= FALSE;					// Des Tran Status
	DesOpenRetryCnt		= 0;						// Des Open Retry Count

	SeedEnStatus		= FALSE;					// Seed Enable Status
	RF_IrMD_Flg			= FALSE;					// RF ����� ����
	SeedTranStatus		= FALSE;					// Seed Tran Status
	SeedSendRetryCnt	= 0;						// Seed Send Retry Count
	FNG_Status			= FALSE;					// RF ����� ����


	TranAvail			= TRAN_ALL_NOT;				// ��ü�ŷ�����
	WithAvail			= TRAN_WITH_NOT;			// ���ްŷ�����
	DepAvail			= TRAN_DEP_NOT;				// �Աݰŷ�����
	PbmAvail			= TRAN_PBM_NOT;				// ����ŷ�����
	
	UserTranAvail		= TRAN_ALL_NOT;				// User��ü�ŷ����� 2003.12.15
	UserWithAvail		= TRAN_WITH_NOT;			// User���ްŷ�����
	UserDepAvail		= TRAN_DEP_NOT;				// User�Աݰŷ�����
	UserPbmAvail		= TRAN_PBM_NOT;				// User����ŷ�����
	
	HostOpenRetryCnt	= 0;						// �����õ�Ƚ��(�غ������)

	bReadyCountFlg      = FALSE;
	
	OldCashDepMode		= TRUE;						// (TRUE:�����Աݰ���, FALSE:�����ԱݺҰ�)
	OldCasetteType		= ON;						// (ON:����Casette��,	OFF:����Casette��)

	TTSEngineLoadFlg = FALSE;						// V07-01-11-#00

	memset(&Timer, 0, sizeof(Timer));				// Ÿ�Ӻ�������

	memset(&Ejr, 0, sizeof(Ejr));					// �������㿵��
	memset(&EjrAmount, 0, sizeof(EjrAmount));		// ��������ݾװ����ڷ�

	memset(&Scr, 0, sizeof(Scr));					// ȭ�麯������

	ChangeBCFlag	= FALSE;
	Withdraw50TFlag = FALSE;
	BrmCshInfo      = FALSE;
	BrmCsmIn        = FALSE;

	IfmDirect			= FALSE;					// #NICCM01	ICCM ��� ����
	IfmDirect_Init		= FALSE;					// #NICCM01	ICCM ��� ����
	IfmResult			= FALSE;					// #NICCM01	ICCM ��� ����
	IsEmvChipPowerOn = FALSE;						// #NICCM01	ICCM ��� ����

	

	SightHandiCapped = FALSE;
	EarPhoneInserted = FALSE;

	// Om�����ʱ�ȭ
	Om.ProcCount		= " ";						// ����ī����
	Om.ErrorCode		= "0000000";				// ����ڵ�
	Om.McuDeviceStatus	= "����";					// Mcu��ġ����(����,���)
	Om.McuProcCount		= " ";						// Mcu�������ī����(����='0',' ','0','1','3','5','6','9')
	Om.McuErrorCode		= "0000000";				// Mcu����ڵ�(����="0000000",���="1234567")
	Om.SprDeviceStatus	= "����";					// Spr��ġ����
	Om.SprProcCount		= " ";						// Spr�������ī����
	Om.SprErrorCode		= "0000000";				// Spr����ڵ�
	Om.JprDeviceStatus	= "����";					// Jpr��ġ����
	Om.JprProcCount		= " ";						// Jpr�������ī����
	Om.JprErrorCode		= "0000000";				// Jpr����ڵ�
	Om.PbmDeviceStatus	= "����";					// Pbm��ġ����
	Om.PbmProcCount		= " ";						// Pbm�������ī����
	Om.PbmErrorCode		= "0000000";				// Pbm����ڵ�
	Om.UcmDeviceStatus	= "����";					// Ucm��ġ����
	Om.UcmProcCount		= " ";						// Ucm�������ī����
	Om.UcmErrorCode		= "0000000";				// Ucm����ڵ�
	Om.BrmDeviceStatus	= "����";					// Brm��ġ����
	Om.BrmProcCount		= " ";						// Brm�������ī����
	Om.BrmErrorCode		= "0000000";				// Brm����ڵ�
	Om.NetDeviceStatus	= "����";					// Net��ġ����
	Om.NetProcCount		= " ";						// Net�������ī����
	Om.NetErrorCode		= "0000000";				// Net����ڵ�
	Om.AppDeviceStatus	= "����";					// App��ġ����
	Om.AppProcCount		= " ";						// App�������ī����
	Om.AppErrorCode		= "0000000";				// App����ڵ�
	Om.A4pDeviceStatus	= "����";					// A4p��ġ����
	Om.A4pProcCount		= " ";						// A4p�������ī����
	Om.A4pErrorCode		= "0000000";				// A4p����ڵ�
	Om.SendRecvStatus	= "OFF";					// �ۼ��Ż���(ON,OFF)
	Om.OperationStatus	= "ON";						// ���ۻ���(ON,OFF)
	Om.CheckKind		= "10";						// ��ǥ����(10,50,100)

	strMsgArray.RemoveAll();              // Remote Ctrl 

	memset(ICSendData, 0, sizeof(ICSendData));		// IC�����ڷ�
	memset(ICRecvData, 0, sizeof(ICRecvData));		// IC�����ڷ�

	memset(&KCashInfo, 0, sizeof(KCashInfo));		// K-CASH��������
	memset(FICEPVersion, 0, sizeof(FICEPVersion));	// #NICCM01	AP B/D ���� ����
	
/////////////////////////////////////////////////////////////////////////////
																// T2ATM : 2006.5.24 yscho
	m_nVfdDoing = FALSE;										// VFD Thread Doing
	VFDTotalCount = 0;											// VFDǥ���ѹ��ڰ���
	VFDCurCount   = 0;											// VFDǥ��������ġ
	VFDDisTerm	  = K_5_WAIT;									// VFDǥ�ýð�����(��)
	m_nTrPollDoing = FALSE;
	m_SendRecvStaus = FALSE;

	RfdSeq		  = 0x01;										// RFID���� V03-04-01(R1)
	RfdTestStatus = FALSE;										//          V03-04-01(R1)	
/////////////////////////////////////////////////////////////////////////////


	memset(RfdVersion, ' ', sizeof(RfdVersion));				// V05-01-02-#11:RF Version
	Rfd3GAvail = FALSE;											// V05-05-01-#01:RF 3G ��������
	memset(RfErrorCode, 0, sizeof(RfErrorCode));				// V06-06-01-#01
	CheckMachineType = TRUE;									// V05-06-03-#13:TRUE(����), FALSE(������)

	

	if (HostLineMode == HOST_LOCAL_MODE)
		PrevInformFlg = TRUE;									// �����˸� �ߴ�/�簳 ������

/////////////////////////////////////////////////////////////////////////////
	memset(VODCapturePath, 0x00, sizeof(VODCapturePath));		// U8100-VOD������������ ���� ���� �ʱ�ȭ
	RealVodFile		= "";										// ������ 1�� ���� (U8100 AP ���泻�� #13) - ī�޶� ������ ����
	
	VODFaceRecording = OFF;
	VODHandRecording = OFF;
	VODRoomRecording = OFF;
	bVODIdx			 = FALSE;

	PBMInkStatus	 =  "";										// U8100-Pbm Ink���°���
	PBMInkCheckCnt	 = 0;
	CheckPbmInkStaus = PBMINK_NORMAL;							// PBM INK ������������

	bNearBranchSelectBtn = FALSE;								// �α����� ���� ���� Ȯ��	#N0172
	
	SetShareData("OM_ȭ�����", "�ĸ�");

	// ����IC ����Ÿ���� �ʱ�ȭ 2004.02.05
	FICCardType = FIC_NONE;							// ī����
	FICDFType = 0;									// IC DF�̼���
	FICAppletType = 0;											// ICī�� APPLET����
	FICAccountCount = 0;							// ����IC ���°���
	memset(FICAccount, 0, sizeof(FICAccount));		// ����IC���¹�ȣ
	memset(FICBankCode, 0, sizeof(FICBankCode));	// ����IC �����ȣ

	memset(&ICTypeInfo, 0, sizeof(ICTypeInfo));					// IC CHIP TYPE���� 2004.11.03
	memset(&FinanceICInfo, 0, sizeof(FinanceICInfo));	// ����IC ī������
	memset(&FinanceICISO3Info, 0, sizeof(FinanceICISO3Info));	// ����IC��������(ISO3)
	memset(&FinanceICISO2Info, 0, sizeof(FinanceICISO2Info));	// ����IC��������(ISO2)
	memset(&FinanceICDisp, 0, sizeof(FinanceICDisp));	// ����IC ȭ����µ���Ÿ

	memset(&EPBAccountInfo, 0, sizeof(EPBAccountInfo));	// e-������� ��������
	memset(&FinanceICEPBDisp, 0, sizeof(FinanceICEPBDisp));
	memset(&FinanceICEPBDisp2, 0, sizeof(FinanceICEPBDisp2));

	memset(&IRDAInfo, 0, sizeof(IRDAInfo));			// ���ܼ� ����
	memset(&IRDAISO3Info, 0, sizeof(IRDAISO3Info));	// ���ܼ� ��������
	memset(&IRDAISO2Info, 0, sizeof(IRDAISO2Info));	// ���ܼ� ��������

/////////////////////////////////////////////////////////////////////////////
	
//	SeedFailCode		= 0;						// SeedFailCode
	RFDAnalError		= 0;						// RF�м�����
	memset(RFDStatus, 0, sizeof(RFDStatus));		// RF����

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
	// DeviceDefine ����
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
		case HBRM_SET:								// U8100 AP ���泻�� #04 - HBRM���� ���泻��
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
		case I_PBM_SET:								// IPBM �Ҹ�ǰ �ܿ����� ����
			DeviceDefine |= DEV_PBM;
			break;
	}

	// �μ���ġDefine�߰��κ�
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
		case LED_SET:								// LED��� ����
			DeviceDefine |= DEV_VFD;
			break;
	}

	switch (AtmDefine.RfdHandler)					// 0:NOT INSTALL,	1:INSTALL 
	{
		case RFD_SET:
			DeviceDefine |= DEV_RFD;				// RF ����
			break;
	}

	switch (AtmDefine.GiroHandler)					// 0:NOT INSTALL,	1:INSTALL 
	{
		case GIRO_SET:
			DeviceDefine |= DEV_GRO;				// ��������
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
			DeviceDefine |= DEV_RDCTRL;				// ��,�� �����ġ 
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

// ��ġ�ø���
int CDevCmn::fnAPL_LoadDevice()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_LoadDevice()");

	CString strVersionData("");
	strVersionData.Format("[ûȣ����ĳ��] %9.9s(%2.2s)]", m_pProfile->TRANS.ApVersion, REVISION);
	fnSCR_DisplayString(1, strVersionData.GetBuffer(0));		// �������

	fnSCR_DisplayString(3, "�ڵ�ȭ��⸦ �غ��ϰ� �ֽ��ϴ�.");
	fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�.");
	fnSCR_DisplayScreen(701);									// �غ���ȭ��

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
			RunExecProgram(LOCALDIREXE ,FALSE, FALSE); //Nice �׽�Ʈ ���� - ������ ��
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

//	fnJPR_Initialize();											// AtmHostCtrl���� ����
//	fnSPR_Initialize();											// AtmHostCtrl���� ����
//	fnMCU_Initialize();											// AtmHostCtrl���� ����
//	fnUCM_Initialize();											// AtmHostCtrl���� ����
//	fnBRM_Initialize();											// AtmHostCtrl���� ����
//	fnPBM_Initialize();											// AtmHostCtrl���� ����
	

// 	if(m_pProfile->DEVICE.MachineType == U3100K)
// 	{
// 		fnLDCtrl_Initialize();										// SLOCK Create
// 		fnSPACK_Initialize();
// 	}
	
	fnCMR_Initialize();											// ������۹��ʱ�ȭ

	fnUPS_Initialize();

// 	if ((AtmDefine.MachineType == U8100))	// U8100
// 	{
// 		if (m_pDevCmn->fnAPL_GetDefineDevice(DEV_CMR))			// Camera Preview ����
// 			AfxBeginThread(thrPreviewThread, this, THREAD_PRIORITY_NORMAL);
// 	}

	PBBarLineReading = FALSE;

	fnDES_Initialize();
	if (fnAPL_GetDefineDevice(DEV_DES))							// 2003.12.12-DES
		fnDES_Test();


	fnPIN_Initialize();
	fnSNS_Initialize();
	fnSCR_Initialize();	
	fnSCR_SetCurrentLangMode(KOR_MODE);							// �������弳��

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
																// 3��° ����Ϳ� ������ ���
			if (GetShareData("OM_��������") == "OFF")
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



// ��ġ������
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
	// �������� : 2003.08.23
	fnSCR_Deinitialize();										// ��������
//	fnNET_DeInitialize();										// AtmModeCtrl���� ����
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

// ���ǵ���ġ���ϱ�
int	CDevCmn::fnAPL_GetDefineDevice(int nDevId)
{
	return (DeviceDefine & nDevId);
}

// ���ǵǰ�����ġ���ϱ�
int	CDevCmn::fnAPL_GetYesDevice(int nDevId)
{
	return (fnAPL_GetDefineDevice(nDevId) & (~(fnAPL_GetNoDevice(nDevId))));
}

// ���ǵǰ�̿�����ġ���ϱ�
int	CDevCmn::fnAPL_GetNoDevice(int nDevId)
{
	return (fnAPL_GetDefineDevice(nDevId) & (DeviceNoDevice & nDevId));
}

int	CDevCmn::fnAPL_GetDownDevice(int nDevId)
{
	return (fnAPL_GetDefineDevice(nDevId) & (DeviceDownDevice & nDevId));
}

// ��밡������ġ���ϱ�
int	CDevCmn::fnAPL_GetAvailDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);						// ���ǵ���ġ���ϱ�

	// �������, �����Ա� �Ұ�
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
	// ��ǥ�Ա�, ��ǥ��� �Ұ�
	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_WITH_CHECK | TRAN_DEP_CHECK)))
		nDevId &= ~DEV_UCM;
	// ������ �Ұ�
	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_PBM_OK)))
		nDevId &= ~DEV_PBM;

	nDevId &= ~DeviceAutoOff;									// �ڵ����� �� Device Set
	nDevId &= ~m_pProfile->TRANS.DeviceExecOff;					// �Ұ����� �� Device Set
	return (nDevId & (~(fnAPL_GetErrorDevice(nDevId))));		// ��ֳ���ġ���ϱ�
}

// ����ϰ���ֳ���ġ���ϱ�
int	CDevCmn::fnAPL_GetAvailErrorDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);						// ���ǵ���ġ���ϱ�

	// �������, �����Ա� �Ұ�
	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_WITH_CASH | TRAN_WITH_50TCASH | TRAN_DEP_CASH)))	
	{
		if (AtmDefine.CashHandler == CDU_SET)					// CDU ��ǥ�� ��ݰ��� �� ���
		{														// ��ǥ��� �ȵ� ���� Logic Miss
			if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CHECK))
				nDevId &= ~DEV_CSH;
		}
		else
			nDevId &= ~DEV_CSH;
	}
	// ��ǥ�Ա�, ��ǥ��� �Ұ�
	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_WITH_CHECK | TRAN_DEP_CHECK)))
		nDevId &= ~DEV_UCM;
	// ������ �Ұ�
	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_PBM_OK)))
		nDevId &= ~DEV_PBM;

	nDevId &= ~DeviceAutoOff;									// �ڵ����� �� Device Set
	nDevId &= ~m_pProfile->TRANS.DeviceExecOff;					// �Ұ����� �� Device Set
	return (fnAPL_GetErrorDevice(nDevId));						// ��ֳ���ġ���ϱ�
}

// ��ֳ���ġ���ϱ�
int	CDevCmn::fnAPL_GetErrorDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);						// ���ǵ���ġ���ϱ�

	DeviceStatus |= m_pProfile->TRANS.DeviceExecOff;			// �Ұ����� �� Device Set
	return (DeviceStatus & nDevId);
}

int	CDevCmn::fnAPL_GetDownErrorDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);

	return (fnAPL_GetDownDevice(nDevId) | fnAPL_GetErrorDevice(nDevId));
}

// ���ݸż����ϱ�
int	CDevCmn::fnAPL_GetNumberOfCash()
{
	int		nTempNumberOfCash = 0;

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		switch (AtmDefine.CashHandler)
		{
			case BRM_SET:
			case HBRM_SET:										// HBRM���� ���泻��
				nTempNumberOfCash = fnCSH_GetNumberOfCash();	// ���ݸż����ϱ�
				break;

			case CDU_SET:
				nTempNumberOfCash = CDU_MAXCASHOUTCNT;
				break;
		}
	}

	return nTempNumberOfCash;
}

// 5���������ڽ��κ��� ���ݸż� ���ϴ� �Լ� -> #0050
int CDevCmn::fnAPL_GetNumberOf50000Cash()
{
	int		nTempNumberOfCash = 0;

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		switch (AtmDefine.CashHandler)
		{
			case OKI_SET:	
			case HBRM_SET:					
				nTempNumberOfCash = fnCSH_GetNumberOf50000Cash();// ���ݸż����ϱ�
				break;

			case CDU_SET:
				nTempNumberOfCash = CDU_MAXCASHOUTCNT/CASH_FIFTY_THOUSAND;
				break;
		}
	}

	return nTempNumberOfCash;
}


// ��ݼ�ǥ�ż����ϱ�
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

// ��ݼ�ǥ2 �ż����ϱ�
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

// ��ݼ�ǥ��ȣ���ϱ�
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

// ��ݼ�ǥ2 ��ȣ���ϱ�
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

// ��ݼ�ǥ �����ڵ屸�ϱ�
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

// ��ݼ�ǥ �����ڵ屸�ϱ�2
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


// �Աݼ�ǥ�ż����ϱ�
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

// ���ݸż�����
int	CDevCmn::fnAPL_SetNumberOfCash(int nCashCnt)
{
	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		switch (AtmDefine.CashHandler)
		{

			case HBRM_SET:									    // HBRM���� ���泻��
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

// ��ݼ�ǥ�ż�����
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

// ��ݼ�ǥ��ȣ����
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

// ��ݼ�ǥ�����ڵ� ����
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

// �Աݼ�ǥ�ż�����
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

// �ŷ����ɻ��±��ϱ�
int CDevCmn::fnAPL_GetAvailTrans()
{
	WithAvail	= fnAPL_GetAvailWithdraw();						// ��ݰ��ɻ��±��ϱ�
	DepAvail	= fnAPL_GetAvailDeposit();						// �Աݰ��ɻ��±��ϱ�
	PbmAvail	= fnAPL_GetAvailPassbook();						// ���尡�ɻ��±��ϱ�
	A4pAvail	= fnAPL_GetAvailA4printer();					// A4P ���ɻ��±��ϱ�
	TranAvail	= fnAPL_GetClerkTransOnOff();					// ����ŷ���屸�ϱ�
	return TranAvail;
}

// ��ݰ��ɻ��±��ϱ�
int CDevCmn::fnAPL_GetAvailWithdraw()
{
	int		nTempWithAvail = TRAN_WITH_NOT;						// �ʱ�ġ�� ��ݺҰ��� ����
	int		nTempCashCstTot = 0;
	int		nTempWithMaxAmt = 0;

	if ((fnAPL_GetAvailDevice(DEV_CSH))	&&
		(AtmDefine.CashHandler != CDU_SET))						// ATM�� ��츸 �ش��Լ��� Call�ϵ��� ����
	{
		switch (fnCSH_GetAvailWithdraw()) 
		{
			case WITH_CASH_ONLY:								// ���ݸ� ��ݰ���
				nTempWithAvail |= TRAN_WITH_CASH;
				if (Withdraw50TFlag)							
					nTempWithAvail |= TRAN_WITH_50TCASH;		
				break;
			case WITH_FIFTY_TH:									// 5������ ��� ����
				nTempWithAvail |= TRAN_WITH_50TCASH;
				break;
			case WITH_TEN_TH:									// ������ ��� ����
				nTempWithAvail |= TRAN_WITH_CASH;			
				break;

			case WITH_NOT_AVAILABLE:							// ��� �Ұ���
				break;

			case WITH_CHECK_ONLY:								// ��ǥ�� ��ݰ���
				nTempWithAvail |= TRAN_WITH_CHECK;
				break;

			case WITH_BOTH_AVAILABLE:							// ����+��ǥ ��ݰ���
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
			case WITH_CHECK_ONLY:								// ��ǥ�� ��ݰ���
				nTempWithAvail |= TRAN_WITH_CHECK;
				break;

			case WITH_NOT_AVAILABLE:							// ��� �Ұ���
			case WITH_CASH_ONLY:								// ���ݸ� ��ݰ���
			case WITH_BOTH_AVAILABLE:							// ����+��ǥ ��ݰ���
			default:
				break;
		}

		if (fnUCM_GetWithCSTStatus() == CST_EMPTY)				// ��ݼ�ǥ������
			nTempWithAvail &= ~TRAN_WITH_CHECK;
		if (!fnAPL_GetNumberOfOutCheck())						// ��ݼ�ǥ�ż����ϱ�
			nTempWithAvail &= ~TRAN_WITH_CHECK;
	}
	else
	if ((fnAPL_GetAvailDevice(DEV_CSH))		&&					// CDP�� fnAPL_GetAvailDevice()�Լ����� ����(1��+5��)�� ��ǥ ������ ���ɰ���� ������
		(AtmDefine.CashHandler == CDU_SET)	)					// ATM�� ���ݿ� ���ؼ��� ��ݰ��ɰ���� ������
	{
		switch (fnCSH_GetAvailWithdraw())					
		{
			case WITH_CASH_ONLY:								// ���ݸ�(5��+��) ��ݰ��� : 1
				nTempWithAvail |= TRAN_WITH_CASH;
				if(Withdraw50TFlag)								
					nTempWithAvail |= TRAN_WITH_50TCASH;			
				break;
			case WITH_FIFTY_TH:									// 5������ ��ݰ��� : 5
				nTempWithAvail |= TRAN_WITH_50TCASH;
				break;
			case WITH_TEN_TH:									// ������ ��ݰ��� : 4
				nTempWithAvail |= TRAN_WITH_CASH;				
				break;				
			case (WITH_TEN_TH+WITH_CHECK_ONLY) :				// ��+��ǥ ��ݰ��� : 6
				nTempWithAvail |= TRAN_WITH_CASH;
				nTempWithAvail |= TRAN_WITH_CHECK;				
				break;
			case (WITH_FIFTY_TH+WITH_CHECK_ONLY) :				// 5��+��ǥ ��ݰ��� : 7
				nTempWithAvail |= TRAN_WITH_50TCASH;
				nTempWithAvail |= TRAN_WITH_CHECK;				
				break;				
			case WITH_CHECK_ONLY:								// ��ǥ�� ��ݰ��� : 
				nTempWithAvail |= TRAN_WITH_CHECK;
				break;
			case WITH_BOTH_AVAILABLE:							// ����(5��+��)+��ǥ ��ݰ���
				nTempWithAvail |= TRAN_WITH_CASH;
				nTempWithAvail |= TRAN_WITH_CHECK;
				if(Withdraw50TFlag)							
					nTempWithAvail |= TRAN_WITH_50TCASH;			
				break;
			case WITH_NOT_AVAILABLE:							// ��� �Ұ���
				break;				
			default:
				break;
		}

		if (!fnAPL_GetNumberOfOutCheck())						// ��ݼ�ǥ�ż����ϱ�
			nTempWithAvail &= ~TRAN_WITH_CHECK;
	}		
	

	return nTempWithAvail;
}

// �Աݰ��ɻ��±��ϱ�
int CDevCmn::fnAPL_GetAvailDeposit()
{
	int		nTempDepAvail = TRAN_DEP_NOT;						// �ʱ�ġ�� �Ա� �Ұ��� ����

	if (fnAPL_GetAvailDevice(DEV_CSH))
	{
		switch (fnCSH_GetAvailDeposit()) 
		{
			case DEP_CASH_ONLY:									// ���ݸ� �Աݰ���
			case DEP_TEN_TH:									
			case DEP_ONLY_TEN_TH:								// �����Ǹ� �Աݰ���
				nTempDepAvail |= TRAN_DEP_CASH;					
				break;
			case DEP_FIFTY_TH:									// 5����+õ,��õ(1���� �Ұ�)
			case DEP_ONLY_FIFTY_TH:							
			case DEP_BOTH_AVAILABLE:							// ����+��ǥ �Աݰ���
				nTempDepAvail |= TRAN_DEP_CASH;
				break;
			case DEP_NOT_AVAILABLE:								// �Ա� �Ұ���
			case DEP_CHECK_ONLY:								// ��ǥ�� �Աݰ���
			default:
				break;
		}
	}

	if (fnAPL_GetAvailDevice(DEV_UCM))
	{
		switch (fnUCM_GetAvailDeposit()) 
		{
			case DEP_CHECK_ONLY:								// ��ǥ�� �Աݰ���
				nTempDepAvail |= TRAN_DEP_CHECK;
				break;

			case DEP_NOT_AVAILABLE:								// �Ա� �Ұ���
			case DEP_CASH_ONLY:									// ���ݸ� �Աݰ���
			case DEP_BOTH_AVAILABLE:							// ����+��ǥ �Աݰ���
			case DEP_ONLY_TEN_TH:								// �����Ǹ� �Աݰ���
			default:
				break;
		}
	}

	return nTempDepAvail;
}

// ���尡�ɻ��±��ϱ�
int CDevCmn::fnAPL_GetAvailPassbook()
{
	int		nTempPbmAvail = TRAN_PBM_NOT;						// �ʱ�ġ�� ���� �Ұ��� ����
	int		PbmRetractCnt = 0;
	if (fnAPL_GetAvailDevice(DEV_PBM))							
	{															// ���� ����
		nTempPbmAvail |= TRAN_PBM_OK;
		// ������ 1�� ���� (U8100 AP ���泻�� #13)
		// PBMInk���°� OUT, NOTSET�� ��� ����ŷ� �ȵǵ��� ����   
		if (m_pDevCmn->AtmDefine.PBHandler == I_PBM_SET)
		{
			if ((memcmp(PBMInkStatus, PBM_INK_OUT, PBMInkStatus.GetLength()) == 0)		||
				(memcmp(PBMInkStatus, PBM_INK_NOTSET, PBMInkStatus.GetLength()) == 0)	||
				(memcmp(PBMInkStatus, "00 %", PBMInkStatus.GetLength()) == 0))				// T3ATM AP ���泻�� #19 - ������ũ �ܷ��� 0�̸� ���Ұ�
				nTempPbmAvail = TRAN_PBM_NOT;
		}
		
		//#0012 - ����̼��� 1ȸ (��ư Disable)
		if (m_pProfile->DEVICE.MachineType == U3100K)	
		{
			PbmRetractCnt = RegGetInt(_REGKEY_PBMSTATUS, "PBRetractCnt", 0);
			if (PbmRetractCnt > 0)
				nTempPbmAvail = TRAN_PBM_NOT;
		}
	}

	return nTempPbmAvail;
}
// A4P������ ���ɻ��±��ϱ�
int CDevCmn::fnAPL_GetAvailA4printer()
{
	int		nTempA4pAvail = TRAN_A4P_NOT;						// �ʱ�ġ�� ���� �Ұ��� ����

	if (fnAPL_GetAvailDevice(DEV_A4P))							
	{															// A4P ����
		if (A4pStatus == A4P_NORMAL)
			nTempA4pAvail |= TRAN_A4P_OK;
	}

	return nTempA4pAvail;
}

// ����ŷ���屸�ϱ�
int CDevCmn::fnAPL_GetClerkTransOnOff()
{
	int		nTempWithAvail	= WithAvail;						// ó������ ����
	int		nTempDepAvail	= DepAvail;
	int		nTempPbmAvail	= PbmAvail;

// ��ݰ��� ó�� 	
	switch (nTempWithAvail) 
	{
		case TRAN_WITH_CASH:									// ���ݸ� ��ݰ���
			if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CASH))	
				nTempWithAvail = TRAN_WITH_NOT;
			break;
		case TRAN_WITH_50TCASH:								
			if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_50TCASH))	
				nTempWithAvail = TRAN_WITH_NOT;
			break;											
		case TRAN_WITH_CHECK:									// ��ǥ�� ��ݰ���
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
		case TRAN_WITH_BOTH:									// ����+��ǥ ��ݰ���
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

		case TRAN_WITH_NOT:										// ��� �Ұ���
		default:
			nTempWithAvail = TRAN_WITH_NOT;
			break;
	}

// �Աݰ��� ó��
	switch (nTempDepAvail) 
	{
		case TRAN_DEP_CASH:										// ���ݸ� �Աݰ���
			if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_DEP_CASH))
				nTempDepAvail = TRAN_DEP_NOT;
			break;
		case TRAN_DEP_CHECK:									// ��ǥ�� �Աݰ���
			if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_DEP_CHECK))
				nTempDepAvail = TRAN_DEP_NOT;
			break;
		case TRAN_DEP_BOTH:										// ����+��ǥ �Աݰ���
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

		case TRAN_DEP_NOT:										// �Ա� �Ұ���
		default:
			nTempDepAvail = TRAN_DEP_NOT;
			break;
	}

// ������� ó��
	switch (nTempPbmAvail) 
	{
		case TRAN_PBM_OK:										// ���� ����
			if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_PBM_OK))
				nTempPbmAvail = TRAN_PBM_NOT;
			break;

		case TRAN_PBM_NOT:										// ���� �Ұ���
		default:
			nTempPbmAvail =TRAN_PBM_NOT;
			break;
	}

	WithAvail	= nTempWithAvail;								// ó������ ����
	DepAvail	= nTempDepAvail;
	PbmAvail	= nTempPbmAvail;
	return (nTempWithAvail | nTempDepAvail | nTempPbmAvail);
}

// ������������
int CDevCmn::fnAPL_SetSensorInfo()
{
	CString	strTempSensor("");
	CString strCst3Type("");

	if (AtmStatus == ATM_INIT) 									// ����ʱ�ȭ ���
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
				BrmChangeLoc	= fnCSH_GetChangeUnitLock();	// Ż���������ϱ�
				BrmLoc			= fnCSH_GetPosition();			// ����ġ���ϱ�
				RejectCSTLoc	= (fnCSH_GetRejectCSTStatus() == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS; 	// ȸ��ī��Ʈ���±��ϱ�															
				RetractCSTLoc	= (fnCSH_GetRetractCSTStatus() == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS; 	// ȸ��ī��Ʈ���±��ϱ�															
				ClerkCSTLoc		= (fnCSH_GetClerkCSTStatus() == CST_SET_NG)		? ABNORMAL_POS : NORMAL_POS;    // ���ī��Ʈ���±��ϱ�
																
				FstCSTLoc		= (fnCSH_GetRBStatus(CDU_CST_1) == CST_SET_NG)   ? ABNORMAL_POS : NORMAL_POS;    // 1ī��Ʈ����ġ����(Cd)
				SndCSTLoc		= (fnCSH_GetRBStatus(CDU_CST_2) == CST_SET_NG)   ? ABNORMAL_POS : NORMAL_POS;					// 2ī��Ʈ����ġ����(Cd)
				TrdCSTLoc		= (fnCSH_GetRBStatus(CDU_CST_3) == CST_SET_NG)   ? ABNORMAL_POS : NORMAL_POS;					// 3ī��Ʈ����ġ����(Cd)
				FthCSTLoc		= (fnCSH_GetRBStatus(CDU_CST_4) == CST_SET_NG)   ? ABNORMAL_POS : NORMAL_POS;					// 4ī��Ʈ����ġ����(Cd)

//				RejectCSTStatus	= (fnCSH_GetRejectCSTStatus() == CST_SET_NG)	? RejectCSTStatus : fnCSH_GetRejectCSTStatus(); // ����Ʈī��Ʈ����		
				RejectCSTStatus	= fnCSH_GetRejectCSTStatus();      // ����Ʈī��Ʈ����		
				RetractCSTStatus= fnCSH_GetRetractCSTStatus();     // ȸ����ī��Ʈ����	
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
				
				ClerkCSTStatus	= (fnCSH_GetClerkCSTStatus() == CST_SET_NG)		? ClerkCSTStatus : fnCSH_GetClerkCSTStatus(); // ���ī��Ʈ�Ի���																
				RecycleBoxStatus= (fnCSH_GetRecycleBoxStatus() == CST_SET_NG)	? RecycleBoxStatus : fnCSH_GetRecycleBoxStatus();
				ClerkRejectBoxStatus = (fnCSH_GetClerkRJBStatus() == CST_SET_NG) ? ClerkRejectBoxStatus : fnCSH_GetClerkRJBStatus();
				
				if ((ChangeBCFlag) && (Withdraw50TFlag))
				{
					RecycleBox50000Status = (fnCSH_GetRecycleBox50000Status() == CST_SET_NG) ? RecycleBox50000Status : fnCSH_GetRecycleBox50000Status();
																// ���ݻ���
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
				UcmLoc			= fnUCM_GetPosition();			// ����ġ���ϱ�

				if (fnUCM_GetWithCSTStatus() == CST_SET_NG);	// ��ݼ�ǥ�����ۼ�
				else
				if (fnUCM_GetWithCSTStatus() == CST_EMPTY)		// ��ݼ�ǥ������
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

				if (fnUCM_GetWithCSTStatus() == CST_SET_NG);	// ��ݼ�ǥ����(�ǹ�)�ۼ�
				else
				if (fnUCM_GetWithCSTStatus() == CST_EMPTY)		// ��ݼ�ǥ������
					CheckCSTStatusSensor = CST_EMPTY;
				else
					CheckCSTStatusSensor = CST_NORMAL;
																// ��ݼ�ǥ����(���)�ۼ�
				if ((fnAPL_GetNumberOfOutCheck() < BRM_EMPTYCHECKCNT)	&&
					(fnAPL_GetNumberOfOutCheck2() < BRM_EMPTYCHECKCNT)	)
					CheckCSTStatusReg = CST_EMPTY;				
				else
				if ((fnAPL_GetNumberOfOutCheck() < BRM_EMPTYCHECKCNT)	&&
					(fnAPL_GetNumberOfOutCheck2() < BRM_EMPTYCHECKCNT)	)
					CheckCSTStatusReg = CST_NEAR;
				else
					CheckCSTStatusReg = CST_NORMAL;

				if (fnUCM_GetDepCSTStatus() == CST_SET_NG);		// �Աݼ�ǥ�����ۼ�
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
			DoorStatus		= fnDOR_GetDoorStatus();				// �޹����±��ϱ�
			break;
		}
	}

	if (fnAPL_GetYesDevice(DEV_SPR))
	{
		strTempSensor = fstrSPR_GetSensorInfo();				// ��ġ�������ϱ�
		if ((strTempSensor.Mid(11,1) == "1")	||				// ��������
			(strTempSensor.Mid(9,1) == "1"))					// �ݼ۷ο���
			SlipHeadStatus	= SLIP_HEAD_UP;
		else
			SlipHeadStatus	= HEADER_NORMAL;
		SlipStatus		= fnSPR_GetPaperStatus();				// �������±��ϱ�
	}

	if (fnAPL_GetYesDevice(DEV_JPR))
	{
		strTempSensor = fstrJPR_GetSensorInfo();				// ��ġ�������ϱ�
		JnlHeadStatus	= HEADER_NORMAL;						// ����Cover����(����)
		JnlStatus		= fnJPR_GetPaperStatus();				// �������±��ϱ�
	}

	if (fnAPL_GetYesDevice(DEV_A4P))
	{
//		strTempSensor = fnA4P_GetPaperStatus();					// ��ġ�������ϱ�
//		if ((strTempSensor.Mid(22,1) == "1")	&&				// ��������
//			(strTempSensor.Mid(23,1) == "1"))					// �ݼ۷ο���
//			A4pHeadStatus	= HEADER_NORMAL;
//		else
//			A4pHeadStatus	= SLIP_HEAD_UP;

		if (!fnA4P_GetDeviceStatus())							// ST_DEVICEVSTATUS = 0 �̸� ����
			A4pHeadStatus = HEADER_NORMAL;
		else
			A4pHeadStatus = DOWN;								// ��� �Ǵ� NODEVICE

		if (fnA4P_GetPaperStatus() & A4P_EMPTY_PAPER)
			A4pStatus = A4P_EMPTY_PAPER;						// Ʈ���̿�������
		else if(fnA4P_GetPaperStatus() & A4P_LOW_END)
			A4pStatus = A4P_LOW_END;
		else
			A4pStatus = A4P_NORMAL;							    // ��������
	}

	if (fnAPL_GetYesDevice(DEV_SPG))
	{
		GAlarmStatus		= fnSPACK_GetAlarmStatus();				// Alarm Status
		GSensorLelvel		= fnSPACK_GetGSensorSensibility();		// G-Sensor ���� ��
		GBatteryStatus		= fnSPACK_GetBatteryLowStatus();		// Battery ���� ��
	}

	return TRUE;
}

// �ŷ����ڼ���
int CDevCmn::fnAPL_SetDate(LPCTSTR szDate)
{
	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.YYYYMMDD, szDate, 8);
	m_pProfile->TRANS.YYYYMMDD[8] = NULL;
	m_pProfile->PutTransProfile();

	return TRUE;
}

// ��ǥ���ڼ���
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

// �Ϸù�ȣ�ʱ�ȭ
int CDevCmn::fnAPL_ClearSerialNo()
{
	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.SerialNo, "000001", 6);
	m_pProfile->TRANS.SerialNo[6] = NULL;
	m_pProfile->PutTransProfile();
	return TRUE;
}

// �Ϸù�ȣ����
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

// �Ϸù�ȣ����
int CDevCmn::fnAPL_SetSerialNo()
{
	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.SerialNo, m_pProfile->TRANS.StartSerialNo, 6);
	m_pProfile->TRANS.SerialNo[6] = NULL;
	m_pProfile->PutTransProfile();
	return TRUE;
}

// �����ȣ����
int CDevCmn::fnAPL_SetProcCount(char chProcCount)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[�����ȣ] : %1c", chProcCount);

	m_pProfile->GetTransProfile();
	m_pProfile->TRANS.ProcCount[0] = chProcCount;
	m_pProfile->TRANS.ProcCount[1] = NULL;
	m_pProfile->PutTransProfile();

	fnAPL_DisplayProcCountSegment();							// �Segment���
	return TRUE;
}

// �ŷ������߼���
int CDevCmn::fnAPL_SetOnTransaction(int nTransDoing)
{
	m_pProfile->GetTransProfile();
	m_SendRecvStaus = PIDILE;
	m_nTrPollDoing = FALSE;
	m_pProfile->TRANS.TransPowerOnOff = nTransDoing; 
	m_pProfile->PutTransProfile();
	return TRUE;
}

// �ŷ����ۼ���
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
	fnAPL_DisplayProcCountSegment();							// �Segment���
	return TRUE;
}

// ��ġ��Ұ������ʱ�ȭ
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

// ��ġ��Ұ������ʱ�ȭ
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
	RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 0);	// 2007.12.08 (0: ������ On/off, 2: ������ ���� On)
	return TRUE;
}

// �Ŀ����°���
int CDevCmn::fnAPL_CheckPowerStatus(int nRealFlag)
{
	int		nTempPowerOffDetect;

	if ((AtmStatus == ATM_INIT) ||								// ����ʱ�ȭ ���
		(AtmStatus == ATM_TRAN))								// ���ŷ��� ���
		return TRUE;
	
	if (AtmStatus != AtmStatusSave)								// ���°����Ͻ���ȿ
		return TRUE;

	if (PowerOffDetect & (EXT_AC_OFF | INT_BATTERY_LOW | CLERK_POWER_OFF | POWER_ABNORMAL))
		return FALSE;

	nTempPowerOffDetect = fnAPL_GetPowerStatus();				// �������±��ϱ�

	RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 0);		// 2007.12.08 (0: ������ On/off, 2: ������ ���� On)
	if (nTempPowerOffDetect & EXT_AC_OFF)						// �����Ǵ�AC����
	{
//		fnAPL_StackError("POWERAC", "�����Ǵ�AC����");			// ������ ��� ����η� �з� - ��������
		PowerOffDetect |= EXT_AC_OFF;
		fnAPL_PrintJnlInfo("POWERAC : �����Ǵ�AC����", JNL_OPER);
																// 2004.02.25 �����λ�����������
		return FALSE;
	}

//	U8500 ���͸� ��� RPC ������ LOW BATTERY ��� ���� #N0255
/*	if (nTempPowerOffDetect & INT_BATTERY_LOW)					// �嵥��LOW
	{
		fnAPL_StackError("POWERBA", "�嵥��LOW");
		PowerOffDetect |= INT_BATTERY_LOW;
		fnAPL_PrintJnlInfo("POWERBA  : �嵥��LOW", JNL_ERROR);	// 2004.02.25 �����λ�����������
		return FALSE;
	}
*/

	if (nTempPowerOffDetect & CLERK_POWER_OFF)					// �����������
	{
//		fnAPL_StackError("POWERCL", "�����������");
		PowerOffDetect |= CLERK_POWER_OFF;
		fnAPL_PrintJnlInfo("POWERCL  : �����������", JNL_OPER);
																// 2004.02.25 �����λ�����������
		return FALSE;
	}

	if (nTempPowerOffDetect & POWER_ABNORMAL)					// UPS���ºҷ�
	{
		fnAPL_StackError("POWERAB", "UPS���ºҷ�");
		PowerOffDetect |= POWER_ABNORMAL;
		fnAPL_PrintJnlInfo("POWERAB  : UPS���ºҷ�", JNL_ERROR);// 2004.02.25 �����λ�����������
		return FALSE;
	}

	return TRUE;
}

// �Ŀ�������ְ���
int CDevCmn::fnAPL_CheckPowerOffError()
{
	if (m_pProfile->TRANS.TransPowerOnOff == ON) 
	{
		fnAPL_StackError("991@@91", "�ŷ�����������");
		m_pProfile->GetTransProfile();
		m_pProfile->TRANS.TransPowerOnOff = OFF; 
		m_pProfile->PutTransProfile();
		return FALSE;
	}

	return TRUE;
}

// ��������ʱ�ȭ
int CDevCmn::fnAPL_ClearError()
{

	CurErrCnt = 0;												// ������ְ���
	m_pProfile->GetCrtErrStack(&CurErrBuff, TRUE);

	memset(OrignalErrorCode, 0, sizeof(OrignalErrorCode));		// V07-01-04-#02
/////////////////////////////////////////////////////////////////////////////
	// Om��������
	Om.ErrorCode		= "0000000";							// ����ڵ�
	Om.McuDeviceStatus	= "����";								// Mcu��ġ����(����,���)
	Om.McuProcCount		= " ";									// Mcu�������ī����(����='0',' ','0','1','3','5','6','9')
	Om.McuErrorCode		= "0000000";							// Mcu����ڵ�(����="0000000",���="1234567")
	Om.SprDeviceStatus	= "����";								// Spr��ġ����
	Om.SprProcCount		= " ";									// Spr�������ī����
	Om.SprErrorCode		= "0000000";							// Spr����ڵ�
	Om.JprDeviceStatus	= "����";								// Jpr��ġ����
	Om.JprProcCount		= " ";									// Jpr�������ī����
	Om.JprErrorCode		= "0000000";							// Jpr����ڵ�
	Om.PbmDeviceStatus	= "����";								// Pbm��ġ����
	Om.PbmProcCount		= " ";									// Pbm�������ī����
	Om.PbmErrorCode		= "0000000";							// Pbm����ڵ�
	Om.UcmDeviceStatus	= "����";								// Ucm��ġ����
	Om.UcmProcCount		= " ";									// Ucm�������ī����
	Om.UcmErrorCode		= "0000000";							// Ucm����ڵ�
	Om.BrmDeviceStatus	= "����";								// Brm��ġ����
	Om.BrmProcCount		= " ";									// Brm�������ī����
	Om.BrmErrorCode		= "0000000";							// Brm����ڵ�
	Om.NetDeviceStatus	= "����";								// Net��ġ����
	Om.NetProcCount		= " ";									// Net�������ī����
	Om.NetErrorCode		= "0000000";							// Net����ڵ�
	Om.AppDeviceStatus	= "����";								// App��ġ����
	Om.AppProcCount		= " ";									// App�������ī����
	Om.AppErrorCode		= "0000000";							// App����ڵ�
	Om.A4pDeviceStatus	= "����";								// A4p��ġ����
	Om.A4pProcCount		= " ";									// A4p�������ī����
	Om.A4pErrorCode		= "0000000";							// A4p����ڵ�
	fnAPL_ClerkInformation();									// �������
/////////////////////////////////////////////////////////////////////////////

	return TRUE;
}

// ��ְ���
int CDevCmn::fnAPL_CheckError()
{
	CurErrCnt = m_pProfile->GetCrtErrStack(&CurErrBuff, FALSE); // ������ֳ���
	if (CurErrCnt)
	{
		m_pProfile->GetFirstErrStack(&FirstErrBuff);			// ������ֳ���
		m_pProfile->GetLastErrStack(&LastErrBuff);				// ������ֳ���
		return FALSE;											// ��� ��
	}
	else 
		return TRUE;											// ��� ��
}

// ����ڵ������˻�
int CDevCmn::fnAPL_CheckErrorCodeValid(LPCTSTR szErrorCode)
{
	if ((IsSpace((LPSTR)szErrorCode, 7))				||		// 2005.05.18
		(IsZero((LPSTR)szErrorCode, 7))					||
		(FindChar((LPSTR)szErrorCode, 7, NULL) != 7))
		return FALSE;
	else 
		return TRUE;											// ���� ����
}

// ��ּ����������������													
int CDevCmn::fnAPL_StackError(LPCTSTR szErrorCode, LPCTSTR szErrorString, int nDevId, int nTranType)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[��ֹ߻�] : %1c-%7.7s %s", m_pProfile->TRANS.ProcCount[0], szErrorCode, szErrorString);

	CString	strTempErrorcode(szErrorCode);
	

	// V07-01-04-#02:�����������
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

	// Om�������� : 2003.11.07
	if (nDevId & (DEV_JPR | DEV_SPR | DEV_MCU | DEV_PBM | DEV_CSH | DEV_UCM))
	{
		if (nDevId & DEV_JPR)
		{
			Om.JprDeviceStatus = "���";
			Om.JprProcCount = m_pProfile->TRANS.ProcCount[0];
			Om.JprErrorCode.Format("%7.7s", strTempErrorcode);
		}
		if (nDevId & DEV_SPR)
		{
			Om.SprDeviceStatus = "���";
			Om.SprProcCount = m_pProfile->TRANS.ProcCount[0];
			Om.SprErrorCode.Format("%7.7s", strTempErrorcode);
		}
		if (nDevId & DEV_MCU)
		{
			Om.McuDeviceStatus = "���";
			Om.McuProcCount = m_pProfile->TRANS.ProcCount[0];
			Om.McuErrorCode.Format("%7.7s", strTempErrorcode);
		}
		if (nDevId & DEV_UCM)
		{
			Om.UcmDeviceStatus = "���";
			Om.UcmProcCount = m_pProfile->TRANS.ProcCount[0];
			Om.UcmErrorCode.Format("%7.7s", strTempErrorcode);
		}
		if (nDevId & DEV_CSH)
		{
			Om.BrmDeviceStatus = "���";
			Om.BrmProcCount = m_pProfile->TRANS.ProcCount[0];
			Om.BrmErrorCode.Format("%7.7s", strTempErrorcode);
		}
		if (nDevId & DEV_PBM)
		{
			Om.PbmDeviceStatus = "���";
			Om.PbmProcCount = m_pProfile->TRANS.ProcCount[0];
			Om.PbmErrorCode.Format("%7.7s", strTempErrorcode);
		}

		if (nDevId & DEV_A4P)
		{
			Om.A4pDeviceStatus = "���";
			Om.A4pProcCount = m_pProfile->TRANS.ProcCount[0];
			Om.A4pErrorCode.Format("%7.7s", strTempErrorcode);
		}

	}
	else
	if ((strTempErrorcode.Mid(0,2) == "77")	||
		(strTempErrorcode.Mid(0,2) == "88"))
	{
		Om.NetDeviceStatus = "���";
		Om.NetProcCount = m_pProfile->TRANS.ProcCount[0];
		Om.NetErrorCode.Format("%7.7s", strTempErrorcode);
	}
	else
	{
		Om.AppDeviceStatus = "���";
		Om.AppProcCount = m_pProfile->TRANS.ProcCount[0];
		Om.AppErrorCode.Format("%7.7s", strTempErrorcode);
	}
	fnAPL_ClerkInformation();									// �������
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
	CString		strTempDbData("");
	CString		strTemp("");

	// ���History���
	// OM_ERRORHISTORY||
	strTempDbData = "OM_ERRORHISTORY";

	// ��ֹ߻���(YYYYMMDD)||
	strTemp.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
	strTempDbData += "||" + strTemp;

	// ��ֹ߻��ð�(HHMMSS)||
	strTemp.Format("%6.6s", GetTime().GetBuffer(0));
	strTempDbData += "||" + strTemp;

	// ����ڵ�(#######)||
	strTemp.Format("%7.7s", strTempErrorcode);
	strTempDbData += "||" + strTemp;

	// ����ī��Ʈ
	strTemp.Format("%1c", m_pProfile->TRANS.ProcCount[0]);
	strTempDbData += "||" + strTemp;
	
	fnAPL_EjrDbManagerSend(strTempDbData);			// ���������ڷ�����
/////////////////////////////////////////////////////////////////////////////
	if (!fnAPL_CheckErrorCodeValid(strTempErrorcode.GetBuffer(0)))	// V07-03-01-#04
	{
		m_pProfile->ErrStack(m_pProfile->TRANS.ProcCount[0], "UNKNOWN", "������� ���");	// U8100 AP ���泻�� #11 - ��Ÿ�׸�...
	}
	else 
		m_pProfile->ErrStack(m_pProfile->TRANS.ProcCount[0], strTempErrorcode.GetBuffer(0), (LPSTR)szErrorString);

	fnAPL_DisplayErrorSegment(m_pProfile->TRANS.ProcCount[0], strTempErrorcode.GetBuffer(0));	// V07-03-01-#04
																// ���Segment���
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
// ȣ��Ʈ���ΰ���
int CDevCmn::fnAPL_CheckHostLine()
{
//	if (HostLineMode == HOST_LOCAL_MODE)
//		return TRUE;
	
	if ((HostStatus == HOST_OFFLINE) ||
		(HostStatus == HOST_WAITLINK))
		return FALSE;
	return TRUE;
}

// ȣ��Ʈ��������
int CDevCmn::fnAPL_CheckHostOpen()
{
//	if (HostLineMode == HOST_LOCAL_MODE)
//		return TRUE;

	if (HostStatus != HOST_ONLINE)
		return FALSE;
	return TRUE;
}

// ȣ��Ʈ������
int CDevCmn::fnAPL_CloseLine()
{
	HostStatus = HOST_OFFLINE;									// ���źҰ�
//	fnNET_Deinitialize(LINE_OPCL_TIME);							// Socket Close
	HostOpenRetryCnt = 0;										// �����õ�Ƚ��(�غ������)
	Timer.HostOpenRetryTime = 5;								// ������õ�Ÿ��
	return TRUE;
}

// ȣ��Ʈ��
int CDevCmn::fnAPL_CloseHost()
{
	if (fnAPL_CheckHostOpen())									// ȣ��Ʈ��������
//		HostStatus = HOST_WAITONLINE;							// �غ�ϴ��
		HostStatus = HOST_WAITREADY;							// �غ�ϴ��
		HostOpenRetryCnt = 0;										// �����õ�Ƚ��(�غ������)
		Timer.HostOpenRetryTime = 5;								// ������õ�Ÿ��
	return TRUE;
}

int CDevCmn::fnAPL_OpenLine()
{
	if (HostStatus == HOST_INIT)
		HostStatus = HOST_OFFLINE;

	return TRUE;
}

// ��ġ�ʱ�ȭ
int CDevCmn::fnAPL_InitializeDevice(int nDevId, int nInitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[��ġ�ʱ�ȭ] : %x,%d", nDevId, nInitFlag);

	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[�ʱ�ȭ   : START ]"));	
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

	Mcu97933Error = FALSE;										// #NICCM01	ICCM ��� ����
	Mcu97934Error = FALSE;										// #NICCM01	ICCM ��� ����
	

	//#0023
	if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 701)
	{
		CString strVersionData("");
		strVersionData.Format("[ûȣ����ĳ��] U3100K/U8100 %9.9s(%2.2s)]", m_pProfile->TRANS.ApVersion, REVISION); //#0097
		fnSCR_DisplayString(1, strVersionData.GetBuffer(0));	
		fnSCR_DisplayString(3, "���� �ð����� ���������̿���");
		fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
		fnSCR_DisplayScreen(701);
	}


	nDevId = fnAPL_GetDefineDevice(nDevId);						// ���ǵ���ġ���ϱ�
	nTempRecoveryDevice = fnAPL_GetAvailErrorDevice(nDevId);	// ����ϰ���ֳ���ġ���ϱ�
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[��ġ�ʱ�ȭ:DefineDevice] : %x,%d", nDevId, nInitFlag);

	fnAPL_SetSegment("88888888");									// Segment����(��)
	fnLGT_SetFlicker(KU_ALL_FLICKER, FLICKER_OFF);					// Flicker����(�ε���,��) //#0143


	if (fnLDCtrl_GetDeviceStatus() == DOWN)
	{
		fnLDCtrl_Initialize();
	}
	
	
	if (fnCMR_GetDeviceStatus() == DOWN)						// ��ֽ�
	{
		fnCMR_ClearErrorCode();
		fnCMR_Initialize();										// ������۹��ʱ�ȭ
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
	
	if (nDevId & DEV_PIN)										// Pin��ֽ�
	{
		fnPIN_ClearErrorCode();									// ����ڵ��ʱ�ȭ
		fnPIN_Initialize();										// ������۹��ʱ�ȭ
	}

	//  * �̴� SCDP������ EP�� ����EP�� ���Ͽ� �ʱ�ȭ�� SPR�� LeftMargin/MaxColumn���� ��������
	//  JPR�� OverWrite�ϱ⶧���� �̸� ����Ͽ� ���� ������.
	if (nDevId & DEV_JPR)										// Jpr��ֽ�
	{
		fnJPR_ClearErrorCode();									// ����ڵ��ʱ�ȭ
		fnJPR_Initialize();										// ������۹��ʱ�ȭ
	}

	if (nDevId & DEV_SPR)										// Spr��ֽ�
	{
		fnSPR_ClearErrorCode();									// ����ڵ��ʱ�ȭ
		fnSPR_Initialize();										// ������۹��ʱ�ȭ
	}

	if (nDevId & DEV_MCU)										// Mcu��ֽ�
	{
		fnMCU_ClearErrorCode();									// ����ڵ��ʱ�ȭ

		if ((!nInitFlag) &&										// ���ʽ����̾ƴѰ��
			(RecoveryType == 0))								// �ڵ�����
		{
			fnMCU_Initialize();									// ������۹��ʱ�ȭ
			if (fnAPL_CheckDeviceAction(DEV_MCU))				// ���۴�����ְ���
			{
				if (fnMCU_GetMaterialInfo())					// ��ü�ܷ��������ϱ�
				{
					DeviceTimeout &= ~DEV_MCU;					// �ð��ʰ������ʱ�ȭ
					fnMCU_Eject();								// ����
					if (fnAPL_CheckDeviceAction(DEV_MCU))		// ���۴�����ְ���
						fnMCU_WaitTaken();						// ������
					fnAPL_CheckDeviceAction(DEV_MCU);			// ���۴�����ְ���
				}	
			}
		}

		fnMCU_Initialize();										// ������۹��ʱ�ȭ
		if (fnAPL_CheckDeviceAction(DEV_MCU))					// ���۴�����ְ���
		{
			if (fnMCU_GetMaterialInfo() & ST_SENSOR2)			// ��ü�ܷ��������ϱ�
				fnMCU_Retract();								// ȸ��
		}
		if (fnAPL_CheckDeviceAction(DEV_MCU))					// ���۴�����ְ���
		{
			if ((!(DeviceTimeout & DEV_MCU))	&&				// �ð��ʰ�������
				(nTempRecoveryDevice == DEV_MCU))				// Mcu��ָ������
				fnAPL_ClearError();								// ��������ʱ�ȭ(Nms����)
		}

		if (AtmDefine.MachineType == U8100)					// #NICCM01	ICCM ��� ����
		{
			if (DeviceStatus & DEV_MCU)
				;
			else
			{
				if (fnMCU_ICCMOpenConnection())					// ICCM �ʱ�ȭ(SUCCESS is ZERO)
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

	if ((nDevId & DEV_CSH))										// ���ݺ���ֽ�
	{
		fnCSH_ClearErrorCode();									// ����ڵ��ʱ�ȭ

		switch (AtmDefine.CashHandler)
		{
			case OKI_SET:									
			case BRM_SET:
			case HBRM_SET:										// U8100-AP����
				if (AtmDefine.CashHandler == BRM_SET)			
				{
					fnCSH_Initialize();
					ChangeBCFlag = FALSE;						
					Withdraw50TFlag = FALSE;				

					fnAPL_CheckDeviceAction(DEV_BRM);			
				}
				else
				{														
					if (nInitFlag)								// ���ʽ���ø� 
					{
						Delay_Msg(3000);						
//						fnCSH_Initialize();						// ���Ḹ�õ�
						BrmCshInfo = fnCSH_Initialize();		// ���Ḹ�õ� - //#0109
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[��ġ�ʱ�ȭ] : fnCSH_Initialize (CSM In Flag %d)", BrmCshInfo);
						ChangeBCFlag = FALSE;				
						Withdraw50TFlag = FALSE;
					}
					else
					{
						if (AtmDefine.CashHandler == HBRM_SET)	// U8100 AP ���泻�� #04 - HBRM���� ���泻��
						{
							if (DeviceNoDevice & DEV_BRM)					
								fnBRM_Initialize();				// ���Ḹ�õ�
							else											
								fnBRM_Reset(TRUE);						
						}
						else									// HBRM-end
							fnBRM_Reset(TRUE);					// �������� for OKI BRM
					}

					fnAPL_CheckDeviceAction(DEV_BRM);

					// U8100-97925 ��ֹ߻��� ������ ó�� : �ʱ� Initialize�ÿ��� �ѹ� �մϴ�
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

					// V06-01-03-#01 ChangeBCFlag �ʱ� BRM ������ ���� �� ���� �� �� �����Ƿ� �Ź� üũ
					if (!ChangeBCFlag)							// BC�� Ȯ��
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
							RegSetStr(_REGKEY_SUPERVISOR, "LargeBill", "2");				// U8100-OM����
						}
						else
						{
							Withdraw50TFlag = FALSE;		
							RegSetStr(_REGKEY_SUPERVISOR, "LargeBill", "1");				// U8100-OM����
						}


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__,	"fnAPL_InitializeDevice:Withdraw50TFlag[%d]", Withdraw50TFlag);
					}									
				}
				break;
			case CDU_SET:
				fnCSH_Initialize();								// ������۹��ʱ�ȭ:ȸ������ó����
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
		fnUCM_ClearErrorCode();									// ����ڵ��ʱ�ȭ

		if ((fnUCM_GetMaterialInfo(UCM_DEPOSIT)) ||				// ��ü�ܷ��������ϱ�(����ݱ���)
			(fnUCM_GetMaterialInfo(UCM_WITHDRAW)))
			DeviceRetract |= DEV_UCM;							// ȸ����������

		fnUCM_Initialize();										// ������۹��ʱ�ȭ
	}

	if (nDevId & DEV_PBM)										// Pbm��ֽ�
	{
		fnPBM_ClearErrorCode();									// ����ڵ��ʱ�ȭ
		
		if ((!nInitFlag) &&										// ���ʽ����̾ƴѰ��
			(RecoveryType == 0))								// �ڵ�����
		{
			fnPBM_Initialize();									// ������۹��ʱ�ȭ
			if (fnAPL_CheckDeviceAction(DEV_PBM))				// ���۴�����ְ���
			{
				if (fnPBM_GetMaterialInfo())					// ��ü�ܷ��������ϱ�
				{
					DeviceTimeout &= ~DEV_PBM;					// �ð��ʰ������ʱ�ȭ
					fnPBM_Eject();								// ����
					if (fnAPL_CheckDeviceAction(DEV_PBM))		// ���۴�����ְ���
						fnPBM_WaitTaken();						// ������
					fnAPL_CheckDeviceAction(DEV_PBM);			// ���۴�����ְ���
				}	
			}
		}

		fnPBM_Initialize();										// ������۹��ʱ�ȭ
		if (fnAPL_CheckDeviceAction(DEV_PBM))					// ���۴�����ְ���
		{
//			if (fnPBM_GetMaterialInfo() & ST_SENSOR2)			// ��ü�ܷ��������ϱ�
			if (AtmDefine.MachineType == U3100K)                // #0012
			{
				if (fnPBM_GetMaterialInfo())
				{
					DeviceTimeout &= ~DEV_PBM;					// �ð��ʰ������ʱ�ȭ
					fnPBM_Eject();								// ����
					if (fnAPL_CheckDeviceAction(DEV_PBM))		// ���۴�����ְ���
						fnPBM_WaitTaken();						// ������
					fnAPL_CheckDeviceAction(DEV_PBM);			// ���۴�����ְ���
				}

				if (fnPBM_GetMaterialInfo())
				{
					fnPBM_Retract();							// ȸ��
					m_pDevCmn->fnAPL_StackError("0110591", "����̼���", DEV_PBM);
				}
			}
			else
			{
				if (fnPBM_GetMaterialInfo())						
					fnPBM_Retract();							// ȸ��
			}

		}

		// U8100-97926 ��ֹ߻��� ������ ó�� : �ʱ� Initialize�ÿ��� �ѹ� �մϴ�
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
	
		if (fnAPL_CheckDeviceAction(DEV_PBM))					// ���۴�����ְ���
		{
			if ((!(DeviceTimeout & DEV_PBM))	&&				// �ð��ʰ�������
				(nTempRecoveryDevice == DEV_PBM))				// Pbm��ָ������
				fnAPL_ClearError();								// ��������ʱ�ȭ(Nms����)
		}
	}

	if (nDevId & DEV_A4P)										// A4Printer��ֽ�
	{
		fnA4P_ClearErrorCode();									// ����ڵ��ʱ�ȭ
		fnA4P_Initialize();										// ������۹��ʱ�ȭ
		fnAPL_CheckDeviceAction(DEV_A4P);
	}
	fnAPL_CheckDeviceAction(nDevId);							// ���۴�����ְ���

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

	DeviceNoDevice = DEV_NONE;									// �̿��� Device�ʱ�ȭ
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
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[��ġ�ʱ�ȭ] - xx: nInitFlag = True");
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
			fnAPL_StackError("9799904", "PINPAD ������� Ȯ��", DEV_PIN);


	}
	else // Touch Screen
	if(AtmDefine.MachineType == U8100 )
	{

		if (DeviceNoDevice & DEV_PIN)
			DeviceDefine = DeviceDefine & (~DEV_PIN); 

	}

	// NoDevice ��� �߻� : 2003.12.15
	DeviceStatus |= (DeviceNoDevice & fnAPL_GetDefineDevice(DEV_MAIN));
	nDevId = fnAPL_GetAvailDevice(DeviceNoDevice & DEV_MAIN);	
	if (nDevId & DEV_SPR)
		fnAPL_StackError("9799200", "��ǥ�����", DEV_SPR);
	if (nDevId & DEV_JPR)
		fnAPL_StackError("9799100", "���κ����", DEV_JPR);
	if (nDevId & DEV_MCU)
		fnAPL_StackError("9799300", "ī������", DEV_MCU);
	if (nDevId & DEV_CSH)
		fnAPL_StackError("9799500", "��������ݺ����", DEV_CSH);
	if (nDevId & DEV_UCM)
		fnAPL_StackError("9799400", "��ǥ����ݺ����", DEV_UCM);
	if (nDevId & DEV_PBM)
		fnAPL_StackError("9799600", "��������", DEV_PBM);
	if (nDevId & DEV_A4P)
		fnAPL_StackError("9799601", "A4���������", DEV_A4P);
	if (nDevId & DEV_PIN)
		fnAPL_StackError("9799904", "PINPAD���", DEV_PIN);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[��ġ�ʱ�ȭ] : return");
	if (fnAPL_GetErrorDevice(nDevId))							// ��ֳ���ġ���ϱ�
	{
		return FALSE;
	}
	else
		return TRUE;
}

// ���۴�����ְ���
int CDevCmn::fnAPL_CheckDeviceAction(int nDevId)
{
	int		nMaxDevRspTime = MIN_DEVRSP_TIME;					// ��ġ�ִ���ð�
	CString	strTempAction("");
	CString	strTempErr("");
	int		nTempTimeoutDevice = DEV_NONE;
	int		nTempErrorDevice = DEV_NONE;
	
	if (HBRM_SET == AtmDefine.CashHandler)						// U8100-USB���ð� ����
		nMaxDevRspTime = MAX_DEVRSP_TIME;						// ��ġ�ִ���ð�

	nDevId = fnAPL_GetDefineDevice(nDevId);						// ���ǵ���ġ���ϱ�

	if (AtmDefine.CashHandler == HBRM_SET)						// V07-01-05-#01
	{
		nMaxDevRspTime = MAX_DEVRSP_TIME;
		if (nDevId & DEV_UCM)
			nMaxDevRspTime = __min(nMaxDevRspTime, UcmRecvTime);
		if (nDevId & DEV_SPR)                                                                                                  
			nMaxDevRspTime = SPR_DEVRSP_TIME; //VAN Ư�� �����.(65��) //#N0157
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
			nMaxDevRspTime = SPR_DEVRSP_TIME; //VAN Ư�� �����.(65��)  //#N0157
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
																// ���۴�����ְ���(��ġId,üũ�ð�)
	if (strTempAction == CHK_NORMAL)							// ����ó��
	{
		if (nDevId & DEV_SPR)
			DeviceStatus &= ~DEV_SPR;
		if (nDevId & DEV_JPR)
			DeviceStatus &= ~DEV_JPR;
		if ((nDevId & DEV_MCU)	&&								// #NICCM01	ICCM ��� ����
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
	if (strTempAction == CHK_TIMEOUT)							// ��ġ�ð��ʰ�
	{
		nTempTimeoutDevice = fnCMN_GetTimeoutDevice(nDevId);	// �ð��ʰ���ġ���ϱ�(��ġId)
		DeviceTimeout |= nTempTimeoutDevice;					// �ð��ʰ����� Device set
	}
	else														// ��ġ������
	{															// ó���� ��ֹ߻�
		nTempErrorDevice = fnCMN_GetErrorDevice(nDevId);		// �����ġ���ϱ�(��ġId)
		switch (AtmDefine.CashHandler)
		{
			case OKI_SET:										// T2ATM 2006.5.24 yscho
			case BRM_SET:
			case HBRM_SET:										// U8100-AP ����
				DeviceStatus |= (nTempErrorDevice & DEV_MAIN);	// 2003.12.15
				break;
				
			case CDU_SET:
				DeviceStatus |= (nTempErrorDevice & DEV_MAIN);	// 2005.01.20
				break;											
		}
		if (nTempErrorDevice & DEV_SPR)							// 2003.12.15
			fnAPL_StackError(fstrSPR_GetErrorCode(), "��ǥ�����", DEV_SPR);
		if (nTempErrorDevice & DEV_JPR)
			fnAPL_StackError(fstrJPR_GetErrorCode(), "���κ����", DEV_JPR);
		if (nTempErrorDevice & DEV_MCU)
			fnAPL_StackError(fstrMCU_GetErrorCode(), "ī������", DEV_MCU);
		if (nTempErrorDevice & DEV_CSH)
			fnAPL_StackError(fstrCSH_GetErrorCode(), "��������ݺ����", DEV_CSH);
		if (nTempErrorDevice & DEV_UCM)
			fnAPL_StackError(fstrUCM_GetErrorCode(), "��ǥ����ݺ����", DEV_UCM);
		if (nTempErrorDevice & DEV_PBM)
			fnAPL_StackError(fstrPBM_GetErrorCode(), "��������", DEV_PBM);
		if (nTempErrorDevice & DEV_A4P)
			fnAPL_StackError(fstrA4P_GetErrorCode(), "A4���������", DEV_A4P);
	}
	if (strTempAction != CHK_NORMAL)							// 2003.11.21 : ��ְ����� Ʈ���̽����
	{
		fnCMN_ProcBackupTrace(GetSprintf("%s\\���%s%s.blg", _TRACE_DIR, 
															  GetDate().GetBuffer(0),
															  GetTime().GetBuffer(0)));
																// Kal Trace Backup
	}

	if (fnAPL_GetErrorDevice(nDevId))							// ��ֳ���ġ���ϱ�
		return FALSE;
	else
		return TRUE;
}

// ��ü�ܷ�����
int CDevCmn::fnAPL_CheckMaterial(int nDevId)
{
	int		nTempReturn = TRUE;
	CStringArray strTempRetractArray;			
	nDevId = fnAPL_GetAvailDevice(nDevId);						// ��밡������ġ���ϱ�
	nDevId &= ~DEV_MCU;											// ī��� �ܷ���� �̰���
	nDevId &= ~DEV_PBM;											// ����� �ܷ���� �̰���

	if (nDevId & DEV_MCU)
	{
		if (fnMCU_GetMaterialInfo() & ST_SENSOR2)
		{
			fnAPL_StackError("8217091", "ī���ī���ܷ�", DEV_MCU);
			DeviceStatus |= DEV_MCU;
			nTempReturn = FALSE;
		}
	}

	// V02-06-02-#06 82160 ��ְ��Ҵ�å - �ܷ���ְ����� �ʱ�ȭó��
	//				 ���� ���� ������ �־��� ����� ������ �ʱ�ȭ���� ȸ���ȴ�.
	//				 �׷���, ȸ���ż��� ���ο� �������� �ʴ´�.(��������� �������� ���ǻ���, 2005.09.07 ��Ȯ��)
	if (nDevId & DEV_CSH)
	{
		if (DeviceLoss & DEV_CSH)								// �̼������
		{
			if (fnCSH_GetMaterialInfo()) 
			{
				if (fnAPL_CheckError())							// �ٸ� ��ְ� ���� ��쿡��	V02-07-01-#08 ǰ������
					fnAPL_StackError("0110991", "��������ݺ��ܷ�", DEV_CSH);

				DeviceStatus |= DEV_CSH;
				nTempReturn = FALSE;
			}
		}
		else
		if (((AtmStatus == ATM_TRAN) || (AtmStatus == ATM_CUSTOM)) &&
			(fnAPL_CheckError()))								// �ٸ� ��ְ� ���� ��쿡��	V02-07-01-#08 ǰ������
		{
			if (fnCSH_GetMaterialInfo()) 
			{
				if (fnAPL_InitializeDevice(DEV_CSH))			// �ʱ�ȭ����ó�� : �ƹ�ó������
				{
MsgDump(MAC_TRACE_CODE_FILE("Log"), "[�ܷ���ְ���] ���ݺ� - ��ġ�ʱ�ȭ�� ����ó��");
					//#0013
					if (fnAPL_GetYesDevice(DEV_CSH))						
					{
						SplitString(m_pDevCmn->fstrCSH_GetRetractResult(), ",", strTempRetractArray);
						if (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler) 
						{
							// ȸ��������ϱ�
							if ((Asc2Int(strTempRetractArray[0]) == 0)	&&	// ȸ���ݾ�
								(Asc2Int(strTempRetractArray[1]) == 0)	&&	// ����ȸ���ż�
								(Asc2Int(strTempRetractArray[2]) == 0)	&&	// ��õ��ȸ���ż�
								(Asc2Int(strTempRetractArray[3]) == 0)	&&	// õ��ȸ���ż�
								(Asc2Int(strTempRetractArray[4]) == 0)	&&	// ������ȸ���ż�
								(Asc2Int(strTempRetractArray[5]) == 0)	&&	// ��ǥȸ���ż�
								(Asc2Int(strTempRetractArray[6]) == 0));	// �Ҹ��ȸ���ż�
							else 
							{
								DeviceRetract |= DEV_CSH;					// ȸ������� ó����

								if ((!ChangeBCFlag) &&						// �̰�ü��� ȸ���ݾ�ó��
									(Asc2Int(strTempRetractArray[4]) != 0))
								{
									m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_RESET,	
													Asc2Int(strTempRetractArray[0])		// ȸ���ݾ�
												-	Asc2Int(strTempRetractArray[4])*50000,
													Asc2Int(strTempRetractArray[1]),	// ����ȸ���ż� 
													Asc2Int(strTempRetractArray[2]),	// ��õ��ȸ���ż� 
													Asc2Int(strTempRetractArray[3]),	// õ��ȸ���ż� 
													Asc2Int(strTempRetractArray[5])		// ��ǥȸ���ż�
												+	Asc2Int(strTempRetractArray[6])		// �Ҹ��ȸ���ż�
												+	Asc2Int(strTempRetractArray[4]));	// ������ȸ���ż�
								}
								else
								{
									m_pDevCmn->fnAPL_PrintCashMoveInfo(BRM_RESET,	
													Asc2Int(strTempRetractArray[0]),	// ȸ���ݾ�
													Asc2Int(strTempRetractArray[1]),	// ����ȸ���ż� 
													Asc2Int(strTempRetractArray[2]),	// ��õ��ȸ���ż� 
													Asc2Int(strTempRetractArray[3]),	// õ��ȸ���ż� 
													Asc2Int(strTempRetractArray[5])		// ��ǥȸ���ż�
												+	Asc2Int(strTempRetractArray[6]),	// �Ҹ��ȸ���ż�
													Asc2Int(strTempRetractArray[4]));	// ������ȸ���ż�
								}
							}
						}												
					}

				}
				else											// �ʱ�ȭ���ó�� : return FALSE
				{
					nTempReturn = FALSE;
MsgDump(MAC_TRACE_CODE_FILE("Log"), "[�ܷ���ְ���] ���ݺ� - ��ġ�ʱ�ȭ�� NGó��");
				}
			}
		}
		else
		{
			if (fnCSH_GetMaterialInfo()) 
			{
				fnAPL_StackError("8216091", "��������ݺ��ܷ�", DEV_CSH);
				DeviceStatus |= DEV_CSH;
				nTempReturn = FALSE;
			}
		}
	}

	if (nDevId & DEV_UCM)
	{
		if (DeviceLoss & DEV_UCM)								// �̼������
		{
			if (fnUCM_GetMaterialInfo(UCM_DEPOSIT) ||
				fnUCM_GetMaterialInfo(UCM_WITHDRAW))			// 821@0���  
			{
				if (fnAPL_CheckError())							// �ٸ� ��ְ� ���� ��쿡��	V02-07-01-#08 ǰ������
					fnAPL_StackError("0110A91", "��ǥ����ݺ��ܷ�", DEV_UCM);

				DeviceStatus |= DEV_UCM;
				nTempReturn = FALSE;
			}
		}
		else
		if (((AtmStatus == ATM_TRAN) || (AtmStatus == ATM_CUSTOM)) &&
			(fnAPL_CheckError()))								// �ٸ� ��ְ� ���� ��쿡��
		{

			if (fnUCM_GetMaterialInfo(UCM_DEPOSIT) ||
				fnUCM_GetMaterialInfo(UCM_WITHDRAW))			// 821@0���  
			{
				if (fnAPL_InitializeDevice(DEV_UCM))
					;
				else
					nTempReturn = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "�� 821@0��� ���� - UCM Initialize");
			}
		}
		else
		{
			if (fnUCM_GetMaterialInfo(UCM_DEPOSIT)) 
			{
				fnAPL_StackError("821@091", "��ǥ�Աݺ��ܷ�", DEV_UCM);
				DeviceStatus |= DEV_UCM;
				nTempReturn = FALSE;
			}

			if (fnUCM_GetMaterialInfo(UCM_WITHDRAW)) 
			{
				fnAPL_StackError("821@092", "��ǥ��ݺ��ܷ�", DEV_UCM);
				DeviceStatus |= DEV_UCM;
				nTempReturn = FALSE;
			}
		}
	}

	if (nDevId & DEV_PBM)
	{
		if (fnPBM_GetMaterialInfo() & ST_SENSOR2)
		{
			fnAPL_StackError("8218091", "������ܷ�", DEV_PBM);
			DeviceStatus |= DEV_PBM;
			nTempReturn = FALSE;
		}
	}

//	if (nDevId & DEV_A4P)
//	{
//		if (fnA4P_GetMaterialInfo())
//		{
//			fnAPL_StackError("8218091", "A4P�������ܷ�", DEV_A4P);
//			DeviceStatus |= DEV_A4P;
//			nTempReturn = FALSE;
//		}
//	}
	return nTempReturn;
}

// �нǸ�üȸ��
int	CDevCmn::fnAPL_RetractMaterial(int nDevId)
{
	int		SprRetractCnt = 0;									// 2003.12.20
	int		McuRetractCnt = 0;
	int		PbmRetractCnt = 0;
	int		A4pRetractCnt = 0;
	CStringArray strTempRetractArray;
	
	if (fnAPL_GetYesDevice(DEV_SPR))							// 2003.12.20
		SprRetractCnt = Asc2Int(fstrSPR_GetRetractCnt());		// ȸ���ż����ϱ�
	if (fnAPL_GetYesDevice(DEV_MCU))
		McuRetractCnt = Asc2Int(fstrMCU_GetRetractCnt());
	if (fnAPL_GetYesDevice(DEV_PBM))
		PbmRetractCnt = Asc2Int(fstrPBM_GetRetractCnt());
	if (fnAPL_GetYesDevice(DEV_A4P))
		A4pRetractCnt = Asc2Int(fstrA4P_GetRetractCnt());
	
	nDevId = fnAPL_GetAvailDevice(nDevId);						// ��밡������ġ���ϱ�
	
	if (nDevId & DEV_SPR)
	{

		if(AtmDefine.MachineType == U3100K)
		{
			if (fnSPR_GetMaterialInfo())							// ��ü�ܷ��������ϱ�
				fnSPR_Eject(K_45_WAIT);                             // ����
		}
		else
		{
			if (fnSPR_GetMaterialInfo())							// ��ü�ܷ��������ϱ�
				fnSPR_Retract();									// ȸ��
		}

	}

	
	if (nDevId & DEV_MCU)
	{
		if (fnMCU_GetMaterialInfo())							// ��ü�ܷ��������ϱ�
			fnMCU_Retract();									// ȸ��
	}
	
	if (nDevId & DEV_PBM)
	{
		//#0012
		if (AtmDefine.MachineType == U3100K)
		{
			if (fnPBM_GetMaterialInfo())
			{
				DeviceTimeout &= ~DEV_PBM;					// �ð��ʰ������ʱ�ȭ
				fnPBM_Eject();								// ����
				if (fnAPL_CheckDeviceAction(DEV_PBM))		// ���۴�����ְ���
					fnPBM_WaitTaken();						// ������
				fnAPL_CheckDeviceAction(DEV_PBM);			// ���۴�����ְ���
			}

			if (fnPBM_GetMaterialInfo())
			{
				fnPBM_Retract();							// ȸ��
				m_pDevCmn->fnAPL_StackError("0110591", "����̼���", DEV_PBM);
			}

		}
		else
		{
			if (fnPBM_GetMaterialInfo())						
				fnPBM_Retract();							// ȸ��
		}

	}
	
	if (nDevId & DEV_A4P)
	{
		if (fnA4P_GetMaterialInfo())							// ��ü�ܷ��������ϱ�
			fnA4P_Retract();									// ȸ��
	}

	if (nDevId & DEV_CSH)
	{
		if (fnCSH_GetMaterialInfo())							// ��ü�ܷ��������ϱ�
		{
			if ((AtmDefine.CashHandler == OKI_SET) || (AtmDefine.CashHandler == HBRM_SET))	// U8100 AP ���泻�� #04 - HBRM���� ���泻��
				fnBRM_Reset(TRUE);
			else
			if (AtmDefine.CashHandler == BRM_SET)
				fnBRM_Initialize();								// ��üȸ���������
			else
			if (AtmDefine.CashHandler == CDU_SET)
			{
				if (fnCSH_GetMaterialInfo())
					fnCSH_Retract();							// ��üȸ���������
			}

		}
	}
	
	if (nDevId & DEV_UCM)
	{
		if ((fnUCM_GetMaterialInfo(UCM_DEPOSIT)) ||				// ��ü�ܷ��������ϱ�(����ݱ���)
			(fnUCM_GetMaterialInfo(UCM_WITHDRAW)))
		{
			DeviceRetract |= DEV_UCM;							// ȸ����������
			fnUCM_Initialize();									// ��üȸ���������
		}
	}
	
	fnAPL_CheckDeviceAction(nDevId);							// ���۴�����ְ���

	if (nDevId & DEV_CSH)										// Brm��ġ�ʱ�ȭ
	{
		fnAPL_CheckDeviceAction(DEV_CSH);						// ���۴�����ְ���
		Delay_Msg(2000);										// �������(2000ms)
																// ȸ���������
		fnAPL_CheckDeviceAction(DEV_CSH);						// ���۴�����ְ���
	}

	if (nDevId & DEV_MCU)
	{
		if (fnAPL_GetYesDevice(DEV_MCU))						// 2003.12.20
		{
			if (McuRetractCnt < Asc2Int(fstrMCU_GetRetractCnt()))
			{
				DeviceRetract |= DEV_MCU;
				fnAPL_PrintJnlInfo("ī��ȸ��", JNL_STATUS);	// 2004.09.07 ��������
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
				fnAPL_PrintJnlInfo("����ȸ��", JNL_STATUS);	// 2004.09.07 ��������
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
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("�н� �ŷ�����ȸ��"), JNL_STATUS);
			}
		}
	}

	if (nDevId & DEV_CSH)
	{
		if (fnAPL_GetYesDevice(DEV_CSH))					
		{
			SplitString(fstrCSH_GetRetractResult(), ",", strTempRetractArray);

			// V07-00-00-#01 �������� ���� ������������������������������������
			if ((OKI_SET == m_pDevCmn->AtmDefine.CashHandler) || (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler))	// U8100 AP ���泻�� #04 - HBRM���� ���泻��
			{
				// ȸ��������ϱ�
				if ((Asc2Int(strTempRetractArray[0]) == 0)	&&	// ȸ���ݾ�
					(Asc2Int(strTempRetractArray[1]) == 0)	&&	// ����ȸ���ż�
					(Asc2Int(strTempRetractArray[2]) == 0)	&&	// ��õ��ȸ���ż�
					(Asc2Int(strTempRetractArray[3]) == 0)	&&	// õ��ȸ���ż�
					(Asc2Int(strTempRetractArray[4]) == 0)	&&	// ������ȸ���ż�
					(Asc2Int(strTempRetractArray[5]) == 0)	&&	// ��ǥȸ���ż�
					(Asc2Int(strTempRetractArray[6]) == 0));	// �Ҹ��ȸ���ż�
				else 
				{
					DeviceRetract |= DEV_CSH;					// ȸ������� ó����
					fnAPL_PrintCashMoveInfo(BRM_LOST,	
							Asc2Int(strTempRetractArray[0]),	// ȸ���ݾ�
							Asc2Int(strTempRetractArray[1]),	// ����ȸ���ż� 
							Asc2Int(strTempRetractArray[2]),	// ��õ��ȸ���ż� 
							Asc2Int(strTempRetractArray[3]),	// õ��ȸ���ż�  
							Asc2Int(strTempRetractArray[5])		// ��ǥȸ���ż�
						+	Asc2Int(strTempRetractArray[6]),	// �Ҹ��ȸ���ż�
							Asc2Int(strTempRetractArray[4]));	// ������ȸ���ż�
				}
			}
			else
			{
			// ����������������������������������������������������������������
				// ȸ��������ϱ�
				if ((Asc2Int(strTempRetractArray[0]) == 0)	&&	// ȸ���ݾ�
					(Asc2Int(strTempRetractArray[1]) == 0)	&&	// ����ȸ���ż�
					(Asc2Int(strTempRetractArray[2]) == 0)	&&	// ��õ��ȸ���ż�
					(Asc2Int(strTempRetractArray[3]) == 0)	&&	// õ��ȸ���ż�
					(Asc2Int(strTempRetractArray[4]) == 0)	&&	// ��ǥȸ���ż�
					(Asc2Int(strTempRetractArray[5]) == 0));	// �Ҹ��ȸ���ż�
				else 
				{
					DeviceRetract |= DEV_CSH;					// ȸ������� ó����
					fnAPL_PrintCashMoveInfo(BRM_LOST,	
							Asc2Int(strTempRetractArray[0]),	// ȸ���ݾ�
							Asc2Int(strTempRetractArray[1]),	// ����ȸ���ż� 
							Asc2Int(strTempRetractArray[2]),	// ��õ��ȸ���ż� 
							Asc2Int(strTempRetractArray[3]),	// õ��ȸ���ż� 
							Asc2Int(strTempRetractArray[4])		// ��ǥȸ���ż�
						+	Asc2Int(strTempRetractArray[5]));	// �Ҹ��ȸ���ż�
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
				fnAPL_PrintJnlInfo("��ǥȸ��", JNL_STATUS);	// 2004.09.07 ��������
			}
		}
	}

	return TRUE;
}

// ��ֳ���ġ�ڵ�����													
int CDevCmn::fnAPL_AutoOffErrorDevice()
{

	if (fnAPL_GetAvailErrorDevice(DeviceStatus) == DEV_BRM)    
	{
		fnAPL_ClearError();
		DeviceAutoOff |= DEV_BRM;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AutoOffErrorDevice (%d)", DeviceAutoOff);

	}

	// �ڵ�����ó��(DEV_BRM : DEV_PBM) //#0084
	if (fnAPL_GetAvailErrorDevice(DeviceStatus) == (DEV_BRM | DEV_PBM))  
	{
		fnAPL_ClearError();										// ��������ʱ�ȭ
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AutoOffErrorDevice0 (%x)", fnAPL_GetAvailErrorDevice(DeviceStatus));
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_BRM)
			DeviceAutoOff |= DEV_BRM;
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_PBM)
			DeviceAutoOff |= DEV_PBM;
	}
	
	// �ڵ�����ó��(PBM��ó�� : BRM,UCM�� ��������Ͽ� �Ұ���å)
	if (fnAPL_GetAvailErrorDevice(DeviceStatus) == (DEV_PBM | DEV_A4P)) 
	{
		fnAPL_ClearError();										// ��������ʱ�ȭ
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AutoOffErrorDevice1 (%x)", fnAPL_GetAvailErrorDevice(DeviceStatus));
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_PBM)
			DeviceAutoOff |= DEV_PBM;
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_A4P)
			DeviceAutoOff |= DEV_A4P;
	}
	////#0032
	if (fnAPL_GetAvailErrorDevice(DeviceStatus) == (DEV_PBM | DEV_SPR))
	{
		fnAPL_ClearError();										// ��������ʱ�ȭ
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AutoOffErrorDevice2 (%x)", fnAPL_GetAvailErrorDevice(DeviceStatus));
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_PBM)
			DeviceAutoOff |= DEV_PBM;
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_SPR)
			DeviceAutoOff |= DEV_SPR;

	}

	if (fnAPL_GetAvailErrorDevice(DeviceStatus) == DEV_A4P)
	{
		fnAPL_ClearError();										// ��������ʱ�ȭ
		DeviceAutoOff |= DEV_A4P;
	}
    //#0032
	if (fnAPL_GetAvailErrorDevice(DeviceStatus) == DEV_PBM)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AutoOffErrorDevice3 (%x)", fnAPL_GetAvailErrorDevice(DeviceStatus));
		fnAPL_ClearError();										// ��������ʱ�ȭ
		DeviceAutoOff |= DEV_PBM;
	}
	
	if (fnAPL_GetAvailErrorDevice(DeviceStatus) == DEV_SPR)     // SPR ���� Van-CD Spec 0105
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AutoOffErrorDevice4 (%x)", fnAPL_GetAvailErrorDevice(DeviceStatus));

		fnAPL_ClearError();
		DeviceAutoOff |= DEV_SPR;
	}


	return TRUE;
}

// ����Segment���
int CDevCmn::fnAPL_DisplayNormalSegment()
{
	CString	strTempNormalSegment("");
	int nTotalAmount = 0;										// V07-00-00-#01 �������� ����

	switch (m_pProfile->DEVICE.MachineType)						// TTW ����
	{
		case U8100:												// U8100-AP����
			// V07-00-00-#01 �������� ���� ������������������������������������
			if (Withdraw50TFlag)
				nTotalAmount = (fnAPL_GetNumberOfCash() + (fnAPL_GetNumberOf50000Cash() * CASH_FIFTY_THOUSAND)) % 10000;
			else
				nTotalAmount = fnAPL_GetNumberOfCash();

			strTempNormalSegment.Format("%4.4d%4.4d", nTotalAmount,	fnAPL_GetNumberOfOutCheck());
			// ����������������������������������������������������������������
			break;
		case U3100K:
			// ���� Mode������ CD���� ��쿡�� Segment�� ����ī��Ʈ�� Display
			// fnAPL_DisplayErrorSegment(m_pProfile->TRANS.ProcCount[0], "0000000");
			strTempNormalSegment.Format("  %1.1c00000", m_pProfile->TRANS.ProcCount[0]);
			break;
	}

	fnAPL_SetSegment(strTempNormalSegment);						// Segment����(��)
	return TRUE;
}

// ���Segment���
int CDevCmn::fnAPL_DisplayErrorSegment(char chProcCount, LPCTSTR szErrorCode)
{
	CString	strTempErrorSegment("");

/////////////////////////////////////////////////////////////////////////////
	// Om��������
	Om.ProcCount.Format("%c", chProcCount);
	Om.ErrorCode.Format("%7.7s", szErrorCode);
	fnAPL_ClerkInformation();									// �������
/////////////////////////////////////////////////////////////////////////////

	// ��� Mode������ Segment�� �����ȣ,����ڵ� Display
	strTempErrorSegment.Format("%2.2s%1.1c%5.5s", fnAPL_GetLC(szErrorCode, TranProc), chProcCount, szErrorCode);

	fnAPL_SetSegment(strTempErrorSegment);						// Segment����(��)
	return TRUE;
}

// �Segment���													
int CDevCmn::fnAPL_DisplayProcCountSegment()
{
	if (!fnAPL_CheckError())									// ��ְ���
		fnAPL_DisplayErrorSegment(m_pProfile->TRANS.ProcCount[0], CurErrBuff.ErrorCode);
	else
	{
		if ((m_pProfile->TRANS.ProcCount[0] == ' ') ||			// �������
			(m_pProfile->TRANS.ProcCount[0] == '0'))			// �������
		{

/////////////////////////////////////////////////////////////////////////////
			// Om��������
			Om.ProcCount.Format("%c", m_pProfile->TRANS.ProcCount[0]);
			Om.ErrorCode.Format("%7.7s", "0000000");
			fnAPL_ClerkInformation();							// �������
/////////////////////////////////////////////////////////////////////////////

			fnAPL_DisplayNormalSegment();						// ����Segment���
		}
		else
		{
			switch (m_pProfile->DEVICE.MachineType)				// TTW ����
			{
				case U8100:										// U8100-AP ����
					fnAPL_DisplayErrorSegment(m_pProfile->TRANS.ProcCount[0], "0000000");
					break;
			}
		}
	}

	return TRUE;
}

// ����Led���
// V03-01-01 SPL ǥ�úи�ó��(������)
int CDevCmn::fnAPL_DisplayInformationLed(int nInitFlag)
{
	static	CString	strSaveLed("000000000000");
	CString	strTempLed("000000000000");

	if (HBRM_SET == AtmDefine.CashHandler)						
		return TRUE;											


	switch (m_pProfile->DEVICE.MachineType)
	{
		case U8100:												// U8100-AP ����
		case U3100K:
//			��������������������������������������������������������������
//			��	  ���� ����   ��               ��� / �̻����          ��
//			��������������������������������������������������������������
//			��  �� �� ��[0]   ��    ī    ��[4]         �� �� ǥ[8]     ��
//			��  ��    ��[1]   ��    ��    ��[5]         ��    ��[9]     ��
//			��  ��    ��[2]   ��    ��    ��[6]         �Աݼ�ǥ[10]    ��
//			��  DUMMY   [3]   ��    ��ݼ�ǥ[7]         ȸ �� ��[11]    ��
//			��������������������������������������������������������������
			if ((AtmStatus == ATM_CUSTOM) ||					// �������	��� 
				(AtmStatus == ATM_TRAN))						// ���ŷ���	���
			{
				strTempLed.SetAt(0, '1');						// �����
			}

			if (fnAPL_GetErrorDevice(DEV_MCU))					// ��ֳ���ġ���ϱ�
			{
				strTempLed.SetAt(4, '1');						// ī��
				strTempLed.SetAt(2, '1');						// �̻�
			}

			if (!(TranAvail & TRAN_PBM_OK))						// ���� �Ұ���
			{
				strTempLed.SetAt(5, '1');						// ����
				strTempLed.SetAt(1, '1');						// ���
			}
			
			if ((!(TranAvail & (TRAN_WITH_CASH | TRAN_WITH_50TCASH))) ||	
				(!(TranAvail & TRAN_DEP_CASH)))					// ���� �Ա� �Ұ���
			{
				strTempLed.SetAt(6, '1');						// ����
				strTempLed.SetAt(1, '1');						// ���
			}
			
			if (!(TranAvail & TRAN_WITH_CHECK))					// ��ǥ ��� �Ұ���
			{
				strTempLed.SetAt(7, '1');						// ��ݼ�ǥ
				strTempLed.SetAt(1, '1');						// ���
			}
			
			if ((fnAPL_GetErrorDevice(DEV_SPR))		||			// ��ֳ���ġ���ϱ�
				(SlipHeadStatus != HEADER_NORMAL)	||			// ��ǥ�� COVER ����
				(SlipStatus == SLIP_EMPTY_PAPER))				// ��ǥ ���� �亸��
			{
				strTempLed.SetAt(8, '1');						// ��ǥ
				strTempLed.SetAt(2, '1');						// �̻�
			}
			else
			if (SlipStatus == SLIP_LOW_END)						// ��ǥ ���� �ܷ���
			{
				strTempLed.SetAt(8, '1');						// ��ǥ
				strTempLed.SetAt(1, '1');						// ���
			}

			if ((fnAPL_GetErrorDevice(DEV_JPR))		||			// ��ֳ���ġ���ϱ�
				(JnlHeadStatus != HEADER_NORMAL)	||			// ���κ� COVER ����
				(JnlStatus == JNL_EMPTY_PAPER))					// ���� ���� �亸��
			{
				strTempLed.SetAt(9, '1');						// ����
				strTempLed.SetAt(2, '1');						// �̻�
			}
			else
			if (JnlStatus == JNL_LOW_END)						// ���� ���� �ܷ���
			{
				strTempLed.SetAt(9, '1');						// ����
				strTempLed.SetAt(1, '1');						// ���
			}

			if (!(TranAvail & TRAN_DEP_CHECK))					// ��ǥ �Ա� �Ұ���
			{
				strTempLed.SetAt(10, '1');						// �Աݼ�ǥ
				strTempLed.SetAt(1, '1');						// ���
			}
			
			if ((RejectCSTLoc != NORMAL_POS)		||			// ������ġ �ƴ�
				(RejectCSTStatus == CST_FULL))					// FULL
			{
				strTempLed.SetAt(11, '1');						// ȸ����
				strTempLed.SetAt(2, '1');						// �̻�
			}
			else
			if (RejectCSTStatus == CST_NEAR)					// NEAR FULL
			{
				strTempLed.SetAt(11, '1');						// ȸ����
				strTempLed.SetAt(1, '1');						// ���
			}

			break;
	}

	if ((nInitFlag) ||											// �ʱ⼳���䱸
		(strTempLed != strSaveLed))								// �����
	{
		strSaveLed = strTempLed;
		fnSPL_SetLed(strTempLed);								// Led����(��)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_DisplayInformationLed[ATMStatus:%d], [Save:%s], [Temp:%s]", AtmStatus, strSaveLed, strTempLed);
	}

	return TRUE;
}

int CDevCmn::fnAPL_DisplayInformationLedOnOff(int nLedIndex, int bOnOff)
{

	return TRUE;
}

// ����Indicator���
int CDevCmn::fnAPL_DisplayInformationIndicator(int nInitFlag)
{
	static	int	nSaveIndicator = 0;
	int		nTempIndicator = 0;
	int		nTempIndicatorTranKind = 0;							// U8100-AP����


	if ( U8100 == m_pProfile->DEVICE.MachineType || U3100K == m_pProfile->DEVICE.MachineType)
	{
		if ((AtmStatus == ATM_CUSTOM) ||						// �������	��� 
			(AtmStatus == ATM_TRAN))							// ���ŷ���	���
		{
			nTempIndicator = 0x01;								// �ŷ�����	��
			
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
			nTempIndicator |= 0x40;								// ��ü
			if (UserTranAvail & TRAN_PBM_OK)
				nTempIndicator |= 0x80;							// ����
		}
		else
		{
			nTempIndicator = 0x02;								// �غ���(�ŷ��Ұ�)	��
		}

		if ((nInitFlag) ||										// �ʱ⼳���䱸
			(nTempIndicator != nSaveIndicator))					// �����
		{
				nSaveIndicator = nTempIndicator;
			if (m_pDevCmn->AtmDefine.CashHandler == HBRM_SET)		
				;
			else
				fnLGT_SetIndicator(nTempIndicator);				// U8100 AP ���泻�� #08 - ��밡�ɷ��� ����

			//������������������������������������������������������������������������
			// CFMonitor ���� 2006.09.08(��) T2ATM LCD����
			//������������������������������������������������������������������������
			HWND hWnd;
			hWnd = _FIND_CFMON;
			if (OKI_SET == AtmDefine.CashHandler)					// T1-T2����
			{
				if(hWnd) ::PostMessage(hWnd, WUM_INDICATOR, nTempIndicator, 0);	
			}
			else 
			if (AtmDefine.CashHandler == HBRM_SET)			// U8100-AP����
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
			//������������������������������������������������������������������������
		}
	}

	return TRUE;
}

// ����Rpl����
int CDevCmn::fnAPL_SendInformationRpl(int nInitFlag)
{
	static	CString	strSaveRpl("000000");
	CString	strTempRpl("000000");

//		��������������������������������������������������������������
//		��	  INDEX       ��               CONTENTS                 ��
//		��������������������������������������������������������������
//		��       [0]      ��    NORMAL  - �����                    ��
//		��       [1]      ��    CASH    - ����                      �� 
//		��       [2]      ��    LOSS    - �н�                      ��
//		��       [3]      ��    ALARM   - �˶�                      ��
//		��       [4]      ��    RECEIPT - ��ǥ                    ��
//		��       [5]      ��    CALL    - ȣ��                      ��
//		��������������������������������������������������������������
	if ((AtmStatus == ATM_CUSTOM) ||							// �������	��� 
		(AtmStatus == ATM_TRAN))								// ���ŷ���	���
	{
		strTempRpl.SetAt(0, '1');								// �����
	}

	if ((RecycleBoxStatus == CST_NEAR)	||						// �ܷ���
		(RecycleBoxStatus == CST_EMPTY)	||						// �亸��
		(RecycleBoxStatus == CST_FULL))							// FULL
	{
		strTempRpl.SetAt(1, '1');								// ����
	}

	if (DeviceLoss)												// �̼������
	{
		strTempRpl.SetAt(2, '1');								// �н�
	}

	if ((DoorStatus == DOOR_OPENED) ||							// �޹�����
		(AtmStatus == ATM_ERROR))								// ��ֹ߻�		���
	{
		strTempRpl.SetAt(3, '1');								// �˶�
	}

	if ((SlipStatus == SLIP_LOW_END)		||					// ��ǥ ���� �ܷ���
		(SlipStatus == SLIP_EMPTY_PAPER)	||					// ��ǥ ���� �亸��
		(JnlStatus  == JNL_LOW_END)			||					// ���� ���� �ܷ���
		(JnlStatus  == JNL_EMPTY_PAPER))						// ���� ���� �亸��
	{
		strTempRpl.SetAt(4, '1');								// ��ǥ
	}

	if (fnSPL_GetCallKey())										// ȣ��Ű���ϱ�
	{
		strTempRpl.SetAt(5, '1');								// ȣ��
	}

	if ((nInitFlag) ||											// �ʱ⼳���䱸
		(strTempRpl != strSaveRpl))								// �����
	{
		strSaveRpl = strTempRpl;
		fnSPL_SetRpl(strTempRpl);								// Rpl����(��) 2004.07.13
	}

	return TRUE;
}

// ����Inform����
int CDevCmn::fnAPL_SendInformationInform(int nInitFlag)
{
	static	CString	strSaveInform("000000");
	CString	strTempInform("000000");

//		��������������������������������������������������������������
//		��	  INDEX       ��               CONTENTS                 ��
//		��������������������������������������������������������������
//		��       [0]      ��    LOSS    - �н�                      ��
//		��       [1]      ��    CASH    - ����                      �� 
//		��       [2]      ��    ALARM   - �˶�                      ��
//		��       [3]      ��    RECEIPT - ��ǥ                    ��
//		��������������������������������������������������������������
	if (DeviceLoss)												// �̼������
	{
		strTempInform.SetAt(0, '1');							// �н�
	}

	if ((RecycleBoxStatus == CST_NEAR)	||						// �ܷ���
		(RecycleBoxStatus == CST_EMPTY)	||						// �亸��
		(RecycleBoxStatus == CST_FULL))							// FULL
	{
		strTempInform.SetAt(1, '1');							// ����
	}

	if ((DoorStatus == DOOR_OPENED) ||							// �޹�����
		(AtmStatus == ATM_ERROR))								// ��ֹ߻�		���
	{
		strTempInform.SetAt(2, '1');							// �˶�
	}

	if ((SlipStatus == SLIP_LOW_END)		||					// ��ǥ ���� �ܷ���
		(SlipStatus == SLIP_EMPTY_PAPER)	||					// ��ǥ ���� �亸��
		(JnlStatus  == JNL_LOW_END)			||					// ���� ���� �ܷ���
		(JnlStatus  == JNL_EMPTY_PAPER))						// ���� ���� �亸��
	{
		strTempInform.SetAt(3, '1');							// ��ǥ
	}

	if ((nInitFlag) ||											// �ʱ⼳���䱸
		(strTempInform != strSaveInform))						// �����
	{
		strSaveInform = strTempInform;
		fnSPL_SetInform(strTempInform);							// Inform����(��)

		if (AtmDefine.MachineType == U8100)			
		{
			// Inform ��� ��ȣ ��, �ؼ� ������ �ٷ� ���۵� ��� ������ �ȵ�(�� 3�� �̳�)
			// Inform ��ȣ �߻� ��, Time Delay�� �־ ���������� ��ȣ ������� ��ġ��.
			Delay_Msg(5 * 1000);
		}

	}

	return TRUE;
}

// ���ȭ����¹�����
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
		case ATM_ERROR: 										// ��ֹ߻� ���
			if (nInitFlag)
			{
				strSaveProcCount = "";
				strSaveErrorCode = "";
			}

			fnAPL_CheckError();									// ��ְ���

			if ((fnSCR_GetCurrentScreenNo() != 701)			||	// ����ȭ���ȣ���ϱ�
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
							fnSCR_DisplayString(10, "�α�IBK�ڳ���ȸ");
					}

					
					/*
					if (nTranStopFlg == TRUE)												//#N0264
					{
						fnSCR_DisplayString(1, "�˼��մϴ�");
						fnSCR_DisplayString(2, "��� ����(AP)���� ����Ͻ� �� �����ϴ�");
						fnSCR_DisplayString(3, "�ٸ� ��⸦ �̿��Ͽ� �ֽʽÿ�");
					}
					*/

					fnSCR_DisplayScreen(701);

					if (nInitFlag)
					{
						if (!nRetryFlg)
						{
							nRetryFlg = TRUE;
							fnAPL_PrintError(DEV_JPR);			// �������
						}
					}
				}
				else
				{
// 					fnSCR_DisplayString(2, GetString(m_pProfile->TRANS.GuideMsg1, sizeof(m_pProfile->TRANS.GuideMsg1), 28));
// 					fnSCR_DisplayString(3, GetString(m_pProfile->TRANS.GuideMsg2, sizeof(m_pProfile->TRANS.GuideMsg2), 28));
// 					fnSCR_DisplayString(4, GetString(m_pProfile->TRANS.GuideMsg3, sizeof(m_pProfile->TRANS.GuideMsg3), 28));
					fnSCR_DisplayString(2, "�������");
					fnSCR_DisplayString(3, "�����ð����� �����������̴�");
					fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
					fnSCR_DisplayScreen(701);					// ���ȭ��
				}
			}
			break;

		case ATM_CLERK:											// ��� ���
		case ATM_READY:											// �غ� ���
			if (nInitFlag)
				strSaveClerkErrorMsg = "";

			if (nErrorStatus == ATM_READY)						// ATM_READY�� 2ȸ�̻�
				if (AtmStatusSave != ATM_READY)
					break;

			strTempClerkErrorMsg = "������� ���� �Դϴ�";
			if (!OpenKey)
				strTempClerkErrorMsg = "������ ��� �������Դϴ�";
			else 
			if (DoorStatus == DOOR_OPENED)
				strTempClerkErrorMsg = "�ݰ�� ����";
			else 
			if (CabinetStatus == DOOR_OPENED)
				strTempClerkErrorMsg = "��ܺ� ����";
			else
			if (SlipHeadStatus != HEADER_NORMAL)
				strTempClerkErrorMsg = "��ǥ�� COVER Ȯ�ο�";
			else
			if (A4pHeadStatus != HEADER_NORMAL)
				strTempClerkErrorMsg = "A4P������ ���� Ȯ�ο�";
			else 
			if (JnlHeadStatus != HEADER_NORMAL)
				strTempClerkErrorMsg = "���κ� COVER Ȯ�ο�";
			else 
			if (SlipStatus == SLIP_EMPTY_PAPER)
				strTempClerkErrorMsg = "��ǥ�� �����Ͽ� �ֽʽÿ�";
			else 
			if (JnlStatus == JNL_EMPTY_PAPER)
				strTempClerkErrorMsg = "���ο����� �����Ͽ� �ֽʽÿ�";
			else 
			if (A4pStatus == SLIP_EMPTY_PAPER)
				strTempClerkErrorMsg = "A4P������ ������ �����Ͽ� �ֽʽÿ�";
			else 
			if (BrmLoc == RECOVERING_POS)				
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "BrmLoc == RECOVERING_POS 53301");
				return TRUE;
			}
			else
			if (ForceMissMatchDownFlag)
				strTempClerkErrorMsg = "ī��Ʈ [���� ����] ����!, 1,2ī��Ʈ ���� ���� Ȯ��!";
			else
			if (BrmLoc != NORMAL_POS)
				strTempClerkErrorMsg = "��������ݺ� ����ġ Ȯ�ο�";
			else 
			if (RejectCSTLoc != NORMAL_POS) 
				strTempClerkErrorMsg = "��������ݺ� [REJECT]����Ʈ ����ġ Ȯ�ο�";
			else 
			if (RetractCSTLoc != NORMAL_POS)
				strTempClerkErrorMsg = "��������ݺ� [RETRACT]ȸ���� ����ġ Ȯ�ο�";
			else 
			if (UcmLoc != NORMAL_POS)
				strTempClerkErrorMsg = "��ǥ����ݺ� ����ġ Ȯ�ο�";
			else 
			if (CabinetStatus == DOOR_OPENED && DoorStatus == DOOR_OPENED)
				strTempClerkErrorMsg = "��ܺ�,�ݰ�� ����";
			else
			if (ForceDownFlag)
				strTempClerkErrorMsg = "��������ݺ� (�ܾ� Ȯ��) ���� �⵿ ���� �Դϴ�";
			else
			if (RemoteDownFlag)
				strTempClerkErrorMsg = "REMOTE CONTRL ..........";
			else
			if (!fnAPL_CheckHostLine())
			{
				strTempClerkErrorMsg = "ȸ�� ������ �Դϴ�";
	
				if ((fnSCR_GetCurrentScreenNo() != 701)	||
					(strTempClerkErrorMsg != strSaveClerkErrorMsg))
				{
					CString strTmp("");
					strTmp = IniGetStr(_TRANS_NEARBRANCH_INI, "1", "cname", "");		// #N0172
					if (!strTmp.IsEmpty())
						fnSCR_DisplayString(10, "�α�IBK�ڳ���ȸ");
				}
			}
			else
			if (!fnAPL_CheckHostOpen())
			{
				strTempClerkErrorMsg = "�¶��� �غ��� �Դϴ�";
			}
				
			if ((fnSCR_GetCurrentScreenNo() != 701)	||
				(strTempClerkErrorMsg != strSaveClerkErrorMsg))
			{
				strSaveClerkErrorMsg = strTempClerkErrorMsg;

				if(RemoteDownFlag)
				{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ReMoteCtrl MSG");
					SplitString("[ ��  �� ]|������� ���� �Դϴ�|���� �ð����� ���������̿���|��ø� ��ٷ� �ֽʽÿ�| | |", "|", strMsgArray);

					fnSCR_DisplayString(1, strMsgArray[0]);
					fnSCR_DisplayString(2, strMsgArray[1]);
					fnSCR_DisplayString(3, strMsgArray[2]);
					fnSCR_DisplayString(4, strMsgArray[3]);
// 					fnSCR_DisplayString(5, strMsgArray[5]);
					fnSCR_DisplayScreen(701);						// �������
				}
				else
				{

					fnSCR_DisplayString(2, strTempClerkErrorMsg);
					fnSCR_DisplayString(3, "���� �ð����� ���������̿���");
					fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
					fnSCR_DisplayScreen(701);// ȭ�����
//					fnSCR_DisplayScreen(701, K_NO_WAIT, PIN_PASSWORD_MODE, "", 6, 6, FALSE, "");// ȭ�����
				}
			}
			break;

		default :
			break;
	}

	return TRUE;
}

// �������
int CDevCmn::fnAPL_PrintError(int nDevId)
{
	if (!fnAPL_CheckError())									// ��ְ���
	{
		fnAPL_EjrSpoolPrintStart(nDevId);						// ������
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

// ��ġ����ġ����
int CDevCmn::fnAPL_CheckPrintDevicePosition()
{
	int		nTempReturn = TRUE;

	if (!fnAPL_CheckPrintBrmPosition())							// Brm����ġ����
		nTempReturn = FALSE;
	if (!fnAPL_CheckPrintUcmPosition())							// Ucm����ġ����
		nTempReturn = FALSE;
	if (!fnAPL_CheckPrintRearDoorStatus())						// �x����������
		nTempReturn = FALSE;

	return nTempReturn;
}

// Brm����ġ����
int CDevCmn::fnAPL_CheckPrintBrmPosition()
{
	int		nTempReturn = TRUE;
	static	int nSaveBrmLoc			= NORMAL_POS;				// Brm����ġ����
	static	int	nSaveRejectCSTLoc	= NORMAL_POS;				// ȸ��������ġ����
	static	int	nSaveClerkCSTLoc	= NORMAL_POS;				// ����������ġ����
	static	int	nSaveFstCSTLoc		= NORMAL_POS;				// 1ī��Ʈ����ġ����(Cd)
	static	int	nSaveSndCSTLoc		= NORMAL_POS;				// 2ī��Ʈ����ġ����(Cd)
	static	int	nSaveTrdCSTLoc		= NORMAL_POS;				// 3ī��Ʈ����ġ����(Cd)
	static	int	nSaveFthCSTLoc		= NORMAL_POS;				// 4ī��Ʈ����ġ����(Cd)

	static	int	nSaveRetractCSTLoc	= NORMAL_POS;			// ����Ʈ����ġ����



	char	szBrmPrint[][256]	= {"[  :  :  ��ݺ� ����ġ ]", "[  :  :  ��ݺ� ����   ]"};
	char	szRJPrint[][256]	= {"[  :  :  ����Ʈ�� ����ġ ]", "[  :  :  ����Ʈ�� ����   ]"};
	char	szRTPrint[][256]	= {"[  :  :  ȸ���� ����ġ ]", "[  :  :  ȸ���� ����   ]"};
	char	szCCPrint[][256]	= {"[  :  :  ������ ����ġ ]", "[  :  :  ������ ����   ]"};
	char	szFSTPrint[][256]	= {"[  :  :  ī��Ʈ1 ����ġ ]", "[  :  :  ī��Ʈ1 ����   ]"};
	char	szSNDPrint[][256]	= {"[  :  :  ī��Ʈ2 ����ġ ]", "[  :  :  ī��Ʈ2 ����   ]"};
	char	szTRDPrint[][256]	= {"[  :  :  ī��Ʈ3 ����ġ ]", "[  :  :  ī��Ʈ3 ����   ]"};
	char	szFTHPrint[][256]	= {"[  :  :  ī��Ʈ4 ����ġ ]", "[  :  :  ī��Ʈ4 ����   ]"};

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

// Ucm����ġ����
int CDevCmn::fnAPL_CheckPrintUcmPosition()
{
	int		nTempReturn = TRUE;
	static	int nSaveUcmLoc	= NORMAL_POS;						// Ucm����ġ����

	char	szUcmPrint[][256] = {"[  :  :  ��ǥ�Աݺ�����ġ]", "[  :  :  ��ǥ�Աݺκ���  ]"};

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

// �x����������
int CDevCmn::fnAPL_CheckPrintRearDoorStatus()
{
	int		nTempReturn = TRUE;
	static	int nSaveDoorStatus			= DOOR_CLOSED;					// DoorStatus����
	static	int	nSaveCabinetStatus		= DOOR_CLOSED;

	char	szDoorPrint[][256] = {"[  :  :  �޹� ����     ]",
		                          "[  :  :  �޹� ����     ]"  };


	char	szCabinetPrint[][256]   = {"[  :  :  ��ܺ� ����    ]",
								       "[  :  :  ��ܺ� ����    ]" };

	if (nSaveDoorStatus != DoorStatus) 
	{
		if (DoorStatus == DOOR_OPENED)							// �޹�����
			fnAPL_DeviceEnDisable(DEV_MCU | DEV_PBM, DISABLE);

		nSaveDoorStatus = DoorStatus;
		if (!fnAPL_PrintPosition(szDoorPrint[DoorStatus]))
			nTempReturn = FALSE;
	}


	if (nSaveCabinetStatus != CabinetStatus) 
	{
		if (CabinetStatus == DOOR_OPENED)							// �޹�����
			fnAPL_DeviceEnDisable(DEV_MCU | DEV_PBM, DISABLE);

		nSaveCabinetStatus = CabinetStatus;
		
		if (!fnAPL_PrintPosition(szCabinetPrint[CabinetStatus]))
			nTempReturn = FALSE;
	}

	nSaveDoorStatus = DoorStatus;
	SaveCabinetStatus = CabinetStatus;

	
	
	return nTempReturn;
}

// ����ġ����
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
		if ((DoorStatus == DOOR_OPENED)		||					// U8100-AP���� #01 - 3Camera, Preview����
			(BrmLoc != NORMAL_POS) || (RejectCSTLoc != NORMAL_POS) || (ClerkCSTLoc != NORMAL_POS) || 
			(UcmLoc != NORMAL_POS))
		{
			;
		}
	}


	fnAPL_ECashEjrSpoolPrint(DEV_JPR,szPrintData);

	// BRM, UCM Ż���� �Ĺ�ī�޶� Capture Start
	if (fnCMR_GetRoomCameraDeviceStatus() == NORMAL)	
	{
		if (DoorStatus == DOOR_OPENED)							// 3Camera, Preview����
		{
			fnAPL_CaptureRoomSave(1, TRUE);						// �޹��������� �Ĺ�ī�޶� Capture
			fnAPL_CaptureSaveEnd(TRUE);							// 4������ ���� (T3ATM AP ���泻�� #17) - �����̹��� ����Ǵ� ��찡 �־� ��å��
		}
		else
		if ((BrmLoc != NORMAL_POS) || (RejectCSTLoc != NORMAL_POS) || (ClerkCSTLoc != NORMAL_POS) || 
			(UcmLoc != NORMAL_POS))
		{
			fnAPL_CaptureRoomSave(1, TRUE);						// �޹��������� �Ĺ�ī�޶� Capture
			fnAPL_CaptureSaveEnd(TRUE);							// 4������ ���� (T3ATM AP ���泻�� #17) - �����̹��� ����Ǵ� ��찡 �־� ��å��
		}
	}

	// BRM, UCM Ż���� �Ĺ�ī�޶� Capture End
	if (m_pProfile->DEVICE.MachineType == U8100)	
		return (fnAPL_CheckDeviceAction(DEV_JPR | DEV_PBM | DEV_MCU));		// ����� ���Ը�弳�� ������ ���� �ŷ��ð����� ��å�� �� V04-01-01-#12
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
			fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("%2.2s:%2.2s [�ڵ� ������]", 
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2])));
		else
			fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("%2.2s:%2.2s [�ڵ� ����ȸ��]", 
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2])));
	}
	else if (nMoveFlag & BRM_MANUALLOAD)
	{
		if (nMoveFlag & REFILL_MOVE)
			fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("%2.2s:%2.2s [��� ������]", 
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2])));
		else 
			fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("%2.2s:%2.2s [��� ����ȸ��]", 
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2])));
	}
	else 
	if (nMoveFlag & BRM_RESET)
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("%2.2s:%2.2s [RESET ����ȸ��]", 
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2])));
	else 
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("%2.2s:%2.2s [����ȸ��]", 
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2])));
	if (nMoveFlag & (BRM_AUTOLOAD | BRM_MANUALLOAD))
	{
		if ((nMoveFlag & REFILL_MOVE) == REFILL_MOVE)
			fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("����ż� : %5d��", nRealMove));
		else fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("ȸ���ż� : %5d��", nRealMove)); 

		if (((OKI_SET == m_pDevCmn->AtmDefine.CashHandler) || (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)) &&	// U8100 AP ���泻�� #04 - HBRM���� ���泻��
			(m_pDevCmn->ChangeBCFlag))
		{
			if ((nMoveFlag & REFILL_MOVE) == REFILL_MOVE)
			{
				fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("���� ����ż� : %5d��", nFiftyThMoveCnt));
				fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("���� ����ż� : %5d��", nTenThMoveCnt));
			}
			else
			{
				fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("���� ȸ���ż� : %5d��", nFiftyThMoveCnt));
				fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("���� ȸ���ż� : %5d��", nTenThMoveCnt));
			}
		}
	}
	else fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("ȸ���ݾ� : %10d��", nRealMove));


	if (((OKI_SET == m_pProfile->DEVICE.CashHandler) || (HBRM_SET == m_pProfile->DEVICE.CashHandler)) &&
		(ChangeBCFlag))											
	{
		// line 3						   123456789012345678901234
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, "------ REJECT �� -----");
		// line 4
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("������ REJECT : %5d��", nFiftyThRJT));
		// line 5
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("����   REJECT : %5d��", nTenThRJT));
		// line 6
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("��õ�� REJECT : %5d��", nFiveThRJT));
		// line 7
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("õ��   REJECT : %5d��", nOneThRJT));
		// line 8
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("�Ҹ�� REJECT : %5d��", nUnknownRJT));

		if(0 < nUnknownRJT)											// V05-01-02-#12:2007.11.27 �Ҹ�� �ȳ����� ���� : COM07-17
		{
			// line 9
			fnAPL_EjrSpoolData(DEV_JPR, i++, 0, "�Ҹ�Ǳݾ׸ż�Ȯ�ο��");
		}
	}
	else
	{
		// line 3						   123456789012345678901234
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, "------ REJECT �� -----");
		// line 4
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("����   REJECT : %5d��", nTenThRJT));
		// line 5
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("��õ�� REJECT : %5d��", nFiveThRJT));
		// line 6
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("õ��   REJECT : %5d��", nOneThRJT));
		// line 7
		// V06-06-00-#01 �Ҹ�� �ȳ����� ����  ��������������������������������
		fnAPL_EjrSpoolData(DEV_JPR, i++, 0, GetSprintf("�Ҹ�� REJECT : %5d��", nUnknownRJT + nFiftyThRJT));	// V07-01-01-#03 �������� �̰�ü��� �������� ȸ���� ���ڳ��� ����
		// line 8
		if (0 < nUnknownRJT)
			fnAPL_EjrSpoolData(DEV_JPR, i++, 0, "�Ҹ�Ǳݾ׸ż�Ȯ�ο��");
		// ��������������������������������������������������������������������
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[FILLTAKE][%d|%d|%d|%d|%d|0|0|0|0|0|0|%d]", 
											nRealMove,			// ��ü
											nOneThRJT,			// õ������(ȸ��)�ż�
											nFiveThRJT,			// ��õ������(ȸ��)�ż�
																// ��������(ȸ��)�ż�(T1ATM�� ȸ��/����ż��� ��ü�ż��� �����ϴ� : ��/��������)
											(AtmDefine.CashHandler == BRM_SET) ? nRealMove : nTenThMoveCnt,
											nFiftyThMoveCnt,	// ����������(ȸ��)�ż�
											nUnknownRJT);		// �Ҹ�Ǻ���(ȸ��)�ż�

	fnAPL_BRMCashInfo();
	fnAPL_EjrSpoolEnd(DEV_JPR);
	return (fnAPL_CheckDeviceAction(DEV_JPR));
}

// ���ݵ����������					
int CDevCmn::fnAPL_PrintCashInfo()
{
	fnAPL_EjrSpoolPrintStart(DEV_JPR);

	fnAPL_EjrSpoolData(DEV_JPR, 0, 0, "[��������]");
	fnAPL_EjrSpoolData(DEV_JPR, 1, 0, GetSprintf("���ݸż� : %5d��", fnAPL_GetNumberOfCash()));

	fnAPL_EjrSpoolEnd(DEV_JPR);	
	return (fnAPL_CheckDeviceAction(DEV_JPR));
}

// ��ǥ�����������
// V02-08-01-R1 100��������� - �������� �� ��������
int CDevCmn::fnAPL_PrintCheckInfo()
{
	fnAPL_EjrSpoolPrintStart(DEV_JPR);
	fnAPL_EjrSpoolData(DEV_JPR, 0, 0, "[��ǥ���]");

	fnAPL_EjrSpoolData(DEV_SPR | DEV_JPR, 1, 0, GetSprintf("��ǥ��ȣ1 : %8.8s", fnAPL_GetNumberOfOutCheckNo().GetBuffer(0)));
	fnAPL_EjrSpoolData(DEV_SPR | DEV_JPR, 2, 0, GetSprintf("��ǥ�ż�1 : %4d��", fnAPL_GetNumberOfOutCheck()));

	// 2003.11.06 PSB
	fnAPL_EjrSpoolData(DEV_SPR | DEV_JPR, 3, 0, GetSprintf("��ǥ��ȣ2 : %8.8s", fnAPL_GetNumberOfOutCheckNo2().GetBuffer(0)));
	fnAPL_EjrSpoolData(DEV_SPR | DEV_JPR, 4, 0, GetSprintf("��ǥ�ż�2 : %4d��", fnAPL_GetNumberOfOutCheck2()));

	fnAPL_EjrSpoolEnd(DEV_JPR, JNL_CASH);
	return (fnAPL_CheckDeviceAction(DEV_JPR));
}

// ��������
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

// AutoLoad�� ���� ȭ�鼳�� �� ����̽� ����üũ
int CDevCmn::fnAPL_SprAutoload()
{

	char    Dispbuf[81];										
	memset(Dispbuf, 0x20, sizeof(Dispbuf));

	if (!m_pDevCmn->fnSPR_GetAutoloadCondition())				// �ٽ��ѹ� üũ
		return TRUE;

	if (!fnAPL_GetAvailDevice(DEV_SPR))							// ��밡������ġ���ϱ�
		return TRUE;

	fnSCR_DisplayString(1, "��ǥ ������");
	fnSCR_DisplayString(2, "���� �ð����� ���������̿���");
	fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
	fnSCR_DisplayScreen(701);						// �������

	fnAPL_CheckDeviceAction(DEV_SPR);					// ���۴�����ְ���
	fnSPR_Autoload();									// ��ǥautoload

	if (!fnAPL_CheckDeviceAction(DEV_SPR))				// ���۴�����ְ���
		return FALSE;
	else
		return TRUE;
}

// Brm�ڵ�����/ȸ��
int CDevCmn::fnAPL_AutoRefillBrm(int nBrmRefillMode)
{
	CStringArray strTempRefillArray;
	CStringArray strTempTakeupArray;
	CStringArray strTempRetractArray;
	CString		 strBrmRefillModeBackup("");
	CString		 strReturnBRMErrCode("");						// V06-01-01-#18
	int			nRet = TRUE;
	
	if (!fnAPL_GetAvailDevice(DEV_CSH))							// ��밡������ġ���ϱ�
		return TRUE;

	switch (nBrmRefillMode) 
	{
		case NORMAL:
			return TRUE;
			break;

		case NEED_REFILL:
			if(AtmDefine.MachineType == U3100K)  //2009.07.22 Kim.Gi.Jin
				break;

			fnSCR_DisplayString(1, "���� �ڵ� �������Դϴ�");
			fnSCR_DisplayString(2, "���� �ð����� ���������̿���");
			fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
			fnSCR_DisplayString(4, "                      ");   // #0093
			fnSCR_DisplayScreen(701);							// �������
			fnAPL_CheckDeviceAction(DEV_CSH);					// ���۴�����ְ���
			fnCSH_RefillMoney(0);								// ����(���ݸż�)
			if (!fnAPL_CheckDeviceAction(DEV_CSH))				// ���۴�����ְ���
				return FALSE;
			fnAPL_DisplayNormalSegment();						// ����Segment���
			SplitString(fstrCSH_GetRefillResult(), ",", strTempRefillArray);
																// ���������ϱ�
			if (OKI_SET == m_pDevCmn->AtmDefine.CashHandler)
			{
				nRet = fnAPL_PrintCashMoveInfo(BRM_AUTOLOAD | REFILL_MOVE,
								Asc2Int(strTempRefillArray[1]),	// ����ż�
								Asc2Int(strTempRefillArray[3]),	// ����ȸ���ż�
								Asc2Int(strTempRefillArray[4]),	// ��õ��ȸ���ż�
								Asc2Int(strTempRefillArray[5]),	// õ��ȸ���ż�
								Asc2Int(strTempRefillArray[7])	// ��ǥȸ���ż�
							+	Asc2Int(strTempRefillArray[8]),	// �Ҹ��ȸ���ż�
								Asc2Int(strTempRefillArray[6]),	// ������ȸ���ż�
								Asc2Int(strTempRefillArray[9]),	// ������ ������ż�	
								Asc2Int(strTempRefillArray[10]));// �������� ������ż�	
																// �����̵���������
				return nRet;
			}
			else
			{
				nRet = fnAPL_PrintCashMoveInfo(BRM_AUTOLOAD | REFILL_MOVE,
								Asc2Int(strTempRefillArray[1]),	// ����ż�
								Asc2Int(strTempRefillArray[3]),	// ����ȸ���ż�
								Asc2Int(strTempRefillArray[4]),	// ��õ��ȸ���ż�
								Asc2Int(strTempRefillArray[5]),	// õ��ȸ���ż�
								Asc2Int(strTempRefillArray[6])	// ��ǥȸ���ż�
							+	Asc2Int(strTempRefillArray[7]));// �Ҹ��ȸ���ż�
																// �����̵���������
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
				if (nBrmRefillMode == NEED_TAKEUP_OLD)			// ���Ǹ�ȸ��
				{
					RegSetInt(_REGKEY_BRMINFO, "ȸ������", BRM_NOTEKIND_10000_OLD);	// 0x30 ���Ǹ���
				}
				else
				if (nBrmRefillMode == NEED_TAKEUP_NEW)			// �űǸ�ȸ��
				{
					RegSetInt(_REGKEY_BRMINFO, "ȸ������", BRM_NOTEKIND_10000_NEW);	// 0x35 �űǸ���
				}
				else
				if (nBrmRefillMode == NEED_TAKEUP50000)			// 5������ȸ��
				{
					RegSetInt(_REGKEY_BRMINFO, "ȸ������", BRM_NOTEKIND_50000);	// 0x34 : 5������
				}
				else											// �ű���ȸ��
				{
					RegSetInt(_REGKEY_BRMINFO, "ȸ������", 0x00);// 0x00 : ��� ����
				}
			}
			else
			{
				strBrmRefillModeBackup = IniGetStr(_SP_DEVICE_INI, "BRM", "�ű���ȸ�����", "2");

				if (nBrmRefillMode == NEED_TAKEUP_OLD)			// ���Ǹ�ȸ��
				{
					IniSetStr(_SP_DEVICE_INI, "BRM", "�ű���ȸ�����", "0");
				}
				else
				if (nBrmRefillMode == NEED_TAKEUP_NEW)			// �űǸ�ȸ��
				{
					IniSetStr(_SP_DEVICE_INI, "BRM", "�ű���ȸ�����", "1");
				}
				else											// �ű���ȸ��
				{
					IniSetStr(_SP_DEVICE_INI, "BRM", "�ű���ȸ�����", "2");
				}
			}

			if(AtmDefine.MachineType == U3100K)  //2009.07.22 Kim.Gi.Jin
				break;


			fnSCR_DisplayString(1, "���� �ڵ� ȸ�����Դϴ�");
			fnSCR_DisplayString(2, "���� �ð����� ���������̿���");
			fnSCR_DisplayString(3, "��ø� ��ٷ� �ֽʽÿ�");
			fnSCR_DisplayScreen(701);							// �������
			fnAPL_CheckDeviceAction(DEV_CSH);					// ���۴�����ְ���
			fnCSH_TakeupMoney(BRM_ATAKEUPCONDCNT);				// ȸ��(���ݸż�) : T2ATM �ż�����

			IniSetStr(_SP_DEVICE_INI, "BRM", "�ű���ȸ�����", strBrmRefillModeBackup);
			MsgDump(TRACE_FREE_MODE, "Log", __FILE__, __LINE__,  "[[�ű���ȸ�����]]");

			if (!fnAPL_CheckDeviceAction(DEV_CSH))				// ���۴�����ְ���
				return FALSE;

			fnAPL_DisplayNormalSegment();						// ����Segment���
			SplitString(fstrCSH_GetTakeupResult(), ",", strTempTakeupArray);
																// ȸ��������ϱ�

			if ((OKI_SET == m_pDevCmn->AtmDefine.CashHandler) || (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler))	// U8100 AP ���泻�� #04 - HBRM���� ���泻��
			{
				nRet = fnAPL_PrintCashMoveInfo(BRM_AUTOLOAD | TAKEUP_MOVE,
								Asc2Int(strTempTakeupArray[1]),	// ȸ���ż�
								Asc2Int(strTempTakeupArray[3]),	// ����ȸ���ż�
								Asc2Int(strTempTakeupArray[4]),	// ��õ��ȸ���ż�
								Asc2Int(strTempTakeupArray[5]),	// õ��ȸ���ż�
								Asc2Int(strTempTakeupArray[7])	// ��ǥȸ���ż�
							+	Asc2Int(strTempTakeupArray[8]),	// �Ҹ��ȸ���ż�
								Asc2Int(strTempTakeupArray[6]),	// ������ȸ���ż�			// V06-01-01-#10
								Asc2Int(strTempTakeupArray[9]),	// ������ ����ȸ���ż�		// V06-02-01-#07
								Asc2Int(strTempTakeupArray[10]));// �������� ����ȸ���ż�	// V06-02-01-#07
																// �����̵���������
				return nRet;
			}
			else
			{
				nRet = fnAPL_PrintCashMoveInfo(BRM_AUTOLOAD | TAKEUP_MOVE,
								Asc2Int(strTempTakeupArray[1]),	// ȸ���ż�
								Asc2Int(strTempTakeupArray[3]),	// ����ȸ���ż�
								Asc2Int(strTempTakeupArray[4]),	// ��õ��ȸ���ż�
								Asc2Int(strTempTakeupArray[5]),	// õ��ȸ���ż�
								Asc2Int(strTempTakeupArray[6])	// ��ǥȸ���ż�
							+	Asc2Int(strTempTakeupArray[7]));// �Ҹ��ȸ���ż�
																// �����̵���������
				return nRet;
			}
			break;
		default:
			break;
	}

	return TRUE;
}

// Brm�ڵ�����
int CDevCmn::fnAPL_AutoResetBrm()
{
	if (!fnAPL_GetAvailDevice(DEV_CSH))							// ��밡������ġ���ϱ�
		return TRUE;
	fnAPL_CheckDeviceAction(DEV_CSH);							// ���۴�����ְ���

	if ((AtmDefine.CashHandler == BRM_SET)	||
		(AtmDefine.CashHandler == CDU_SET)	)
		fnCSH_Initialize();										// ������۹��ʱ�ȭ
	else
	if ((AtmDefine.CashHandler == OKI_SET)	||
		(AtmDefine.CashHandler == HBRM_SET)	)					// U8100-AP����
		fnBRM_Reset(TRUE);										// T2ATM 2006.5.24 yscho
	else
		return TRUE;
	
	return (fnAPL_CheckDeviceAction(DEV_CSH));					// ���۴�����ְ���
}

// Brm�ڵ��غ�
int CDevCmn::fnAPL_AutoReadyBrm()
{	
	if (!fnAPL_GetAvailDevice(DEV_CSH))							// ��밡������ġ���ϱ�
		return TRUE;

	fnCSH_Ready();												// ������غ������װ����뺹��

	return TRUE;
}

// ��ġ����
int CDevCmn::fnAPL_ResetDevice(int nDevId, int nInitFlag)				
{

	int		SprRetractCnt = 0;									// 2003.12.20
	int		McuRetractCnt = 0;
	int		PbmRetractCnt = 0;
	int		A4pRetractCnt = 0;
	CStringArray strTempRetractArray;

	if (fnAPL_GetYesDevice(DEV_SPR))							// 2003.12.20
		SprRetractCnt = Asc2Int(fstrSPR_GetRetractCnt());		// ȸ���ż����ϱ�
	if (fnAPL_GetYesDevice(DEV_MCU))
		McuRetractCnt = Asc2Int(fstrMCU_GetRetractCnt());
	if (fnAPL_GetYesDevice(DEV_PBM))
		PbmRetractCnt = Asc2Int(fstrPBM_GetRetractCnt());
	if (fnAPL_GetYesDevice(DEV_A4P))
		A4pRetractCnt = Asc2Int(fstrA4P_GetRetractCnt());

	nDevId = fnAPL_GetDefineDevice(nDevId);						// ���ǵ���ġ���ϱ�
//**************************************************************************************** #KIM 0404
//	if (nDevId & DEV_CSH)										// Brm��ġ�ʱ�ȭ
//		fnAPL_CheckDeviceAction(DEV_CSH);						// ���۴�����ְ���
//****************************************************************************************

	fnAPL_InitializeDevice(nDevId, nInitFlag);					// ��ġ�ʱ�ȭ

	if (nDevId & DEV_CSH)										// Brm��ġ�ʱ�ȭ
	{
		fnAPL_CheckDeviceAction(DEV_CSH);						// ���۴�����ְ���
		Delay_Msg(2000);										// �������(2000ms)
																// ȸ���������
		fnAPL_CheckDeviceAction(DEV_CSH);						// ���۴�����ְ���
	}

	fnAPL_CheckMaterial();										// ��ü�ܷ�����

	if (nDevId & DEV_MCU)
	{
		if (fnAPL_GetYesDevice(DEV_MCU))						// 2003.12.20
		{
			if (McuRetractCnt < Asc2Int(fstrMCU_GetRetractCnt()))
			{
				DeviceRetract |= DEV_MCU;
				fnAPL_PrintJnlInfo("ī��ȸ��", JNL_STATUS);
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
				fnAPL_PrintJnlInfo("����ȸ��", JNL_STATUS);
			}
		}
	}

	if (nDevId & DEV_CSH)
	{
		if (fnAPL_GetYesDevice(DEV_CSH))						// 2003.12.20
		{
			SplitString(fstrCSH_GetRetractResult(), ",", strTempRetractArray);

			if ((OKI_SET == m_pDevCmn->AtmDefine.CashHandler) || (HBRM_SET == m_pDevCmn->AtmDefine.CashHandler)) // U8100 AP ���泻�� #04 - HBRM���� ���泻��
			{
				// ȸ��������ϱ�
				if ((Asc2Int(strTempRetractArray[0]) == 0)	&&	// ȸ���ݾ�
					(Asc2Int(strTempRetractArray[1]) == 0)	&&	// ����ȸ���ż�
					(Asc2Int(strTempRetractArray[2]) == 0)	&&	// ��õ��ȸ���ż�
					(Asc2Int(strTempRetractArray[3]) == 0)	&&	// õ��ȸ���ż�
					(Asc2Int(strTempRetractArray[4]) == 0)	&&	// ������ȸ���ż�
					(Asc2Int(strTempRetractArray[5]) == 0)	&&	// ��ǥȸ���ż�
					(Asc2Int(strTempRetractArray[6]) == 0));	// �Ҹ��ȸ���ż�
				else 
				{
					DeviceRetract |= DEV_CSH;					// ȸ������� ó����

					if ((!ChangeBCFlag) &&						// V06-02-01-#14:�̰�ü��� ȸ���ݾ�ó��
						(Asc2Int(strTempRetractArray[4]) != 0))
					{
						fnAPL_PrintCashMoveInfo(BRM_RESET,	
							Asc2Int(strTempRetractArray[0])		// ȸ���ݾ�
						-	Asc2Int(strTempRetractArray[4])*50000,
							Asc2Int(strTempRetractArray[1]),	// ����ȸ���ż� 
							Asc2Int(strTempRetractArray[2]),	// ��õ��ȸ���ż� 
							Asc2Int(strTempRetractArray[3]),	// õ��ȸ���ż� 
							Asc2Int(strTempRetractArray[5])		// ��ǥȸ���ż�
						+	Asc2Int(strTempRetractArray[6])		// �Ҹ��ȸ���ż�
						+	Asc2Int(strTempRetractArray[4]));	// ������ȸ���ż�
					}
					else
					{
						fnAPL_PrintCashMoveInfo(BRM_RESET,	
							Asc2Int(strTempRetractArray[0]),	// ȸ���ݾ�
							Asc2Int(strTempRetractArray[1]),	// ����ȸ���ż� 
							Asc2Int(strTempRetractArray[2]),	// ��õ��ȸ���ż� 
							Asc2Int(strTempRetractArray[3]),	// õ��ȸ���ż� 
							Asc2Int(strTempRetractArray[5])		// ��ǥȸ���ż�
						+	Asc2Int(strTempRetractArray[6]),	// �Ҹ��ȸ���ż�
							Asc2Int(strTempRetractArray[4]));	// ������ȸ���ż�
					}
				}
			}												
			else
			if (BRM_SET == m_pDevCmn->AtmDefine.CashHandler)
			{
				// ȸ��������ϱ�
				if ((Asc2Int(strTempRetractArray[0]) == 0)	&&	// ȸ���ݾ�
					(Asc2Int(strTempRetractArray[1]) == 0)	&&	// ����ȸ���ż�
					(Asc2Int(strTempRetractArray[2]) == 0)	&&	// ��õ��ȸ���ż�
					(Asc2Int(strTempRetractArray[3]) == 0)	&&	// õ��ȸ���ż�
					(Asc2Int(strTempRetractArray[4]) == 0)	&&	// ��ǥȸ���ż�
					(Asc2Int(strTempRetractArray[5]) == 0));	// �Ҹ��ȸ���ż�
				else 
				{
					DeviceRetract |= DEV_CSH;					// ȸ������� ó����
					fnAPL_PrintCashMoveInfo(BRM_RESET,	
						Asc2Int(strTempRetractArray[0]),		// ȸ���ݾ�
						Asc2Int(strTempRetractArray[1]),		// ����ȸ���ż� 
						Asc2Int(strTempRetractArray[2]),		// ��õ��ȸ���ż� 
						Asc2Int(strTempRetractArray[3]),		// õ��ȸ���ż� 
						Asc2Int(strTempRetractArray[4])			// ��ǥȸ���ż�
						+	Asc2Int(strTempRetractArray[5]));	// �Ҹ��ȸ���ż�
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
				fnAPL_PrintJnlInfo("��ǥȸ��", JNL_STATUS);
			}
		}
	}

	if (nInitFlag)												// ���ʽ����
	{
		if (fnAPL_GetYesDevice(DEV_SPR))						// 2003.12.20
			fnSPR_ClearRetractCnt();							// ȸ���ż��ʱ�ȭ
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
		fnAPL_DisplayNormalSegment();							// ����Segment���
		return TRUE;
	}
}

// �������
int CDevCmn::fnAPL_UnUsed()
{
	fnAPL_SetOnTransaction(OFF);								// �ŷ�����ƴ�
	fnAPL_SetProcCount(' ');									// �����ȣ����
	fnSCR_SetCurrentLangMode(KOR_MODE);							// �������弳��
	fnAPL_DeviceEnDisable(DEV_MCU | DEV_PBM | DEV_PIN, DISABLE);			// ��ġ���Ը�弳�� 
	fnLGT_SetFlicker(KU_ALL_FLICKER, FLICKER_OFF);					// Flicker����(�ε���,��) 
	fnAPL_DisplayInformationLed(TRUE);							// ����Led���
	fnAPL_DisplayInformationIndicator(TRUE);					// �غ���
	return TRUE;
}

// ��ġ���Ը�弳��
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

	nDevId = fnAPL_GetAvailDevice(nDevId);						// ��밡������ġ���ϱ�
//t	fnAPL_CheckDeviceAction(nDevId);							// ���۴�����ְ���
																// Think About : ���⼭ PINPAD TIMEOVER�߻�����
	if (nDevId != DEV_PIN)
		fnAPL_CheckDeviceAction(nDevId & (~DEV_PIN));			// ���۴�����ְ���

	// Mcu Enable/Disable ���¿� ���þ��� ����ó����
	if (nDevId & DEV_MCU)
	{
		if (nAction == ENABLE)
			fnMCU_EntryEnable();								// �����㰡
		else 
			fnMCU_EntryDisable();								// ���Ա���
		nTempDevId |= DEV_MCU;
	}

	// Pbm Enable/Disable ���¿� ���þ��� ����ó����
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
				case PIN_DUMMY_MODE:							// V03-01-01 DUMMY MODE�߰�ó��
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
// ������ġ���ϱ�
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
//      FILE    D:\EJR\TXTMMDD\DD.TXT(1��ġ �ڷᰡ ȯ��������� �����)
//              D:\EJR\IMGMMDD\DDEJRSEQNN0.JPG
//                             DDEJRSEQNN1.JPG(0-9:���,�Ա�)
//
//		RECORD	2048 BYTE(EjrTbl����)
/////////////////////////////////////////////////////////////////////////////
// �������ν���
int CDevCmn::fnAPL_EjrSpoolPrintStart(int nDevId, EjrTbl* pEjrData, EjrAmountTbl* pEjrAmountData)
{
	memset(&Ejr, ' ', sizeof(Ejr));								// �������㿵��
	memset(&EjrAmount, 0, sizeof(EjrAmount));					// ��������ݾװ����ڷ� : 2003.12.15

																// Key Area Setting
	if (pEjrData)												// �����ú���
		memcpy(&Ejr, pEjrData, sizeof(Ejr));
	if (pEjrAmountData)											// �����ú��� : 2003.12.15
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

// ��������
int CDevCmn::fnAPL_EjrSpoolData(int nDevId, int nLine, int nSize, LPCTSTR szSpoolData)
{
	
	if (nLine < 0)												// ���κ���
		nLine = 0;
	if (nLine >= SPR_MAX_ROW)
		nLine = SPR_MAX_ROW - 1;
	
	if (nSize <= 0)												// ���̱��ϱ�&����
		nSize = strlen(szSpoolData);
	if (nSize > JPR_MAX_COL)
		nSize = JPR_MAX_COL;

	memcpy(Ejr.Data[nLine], GetString((LPSTR)szSpoolData, nSize, nSize).GetBuffer(0), nSize);
																// �ڷẸ��
	return TRUE;
}

// ������������&���������ó��													
int CDevCmn::fnAPL_EjrSpoolEnd(int nDevId, int JnlType, int nWaitFlag)
{
	CString		strTempPrintData("");
	CString		strTempUnPackPrintData("");
	CString		strTempSprPrintData("");
	CString		strTempJprPrintData("");
	int			i = 0;
	int			SprRowOptionSave = 0;

	// Ejr����
	if (nDevId & DEV_JPR)										// �����ڷ������
	{
		fnAPL_EjrAddSerialNo();									// ���������Ϸù�ȣ����
		fnAPL_EjrSave(JnlType);									// ������������

		if (m_nJournalMode == ATMS_JNL_HDD)
		{
//			if (m_pDevCmn->TranStatus != TRAN_ERROR)
			if (AtmStatus != ATM_ERROR)
				nDevId &= ~DEV_JPR;
		}
	}

	/*
	if (TranProc == TRAN_HIPASS)		//#N0266 ��ǥ ���� ����
	{
		SprRowOptionSave = SprRowOption;
		SprRowOption = SPR_MAX_ROW;
	}
*/
	
	// �����ڷ��ۼ�
	for (i = 0; i < SprRowOption; i++)							// �ڷ��ۼ�
	{
		if (nDevId & DEV_SPR)									// ��ǥ�����ڷ��ۼ�
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
		
		if (nDevId & DEV_JPR)									// ���������ڷ��ۼ�
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
	

	// ����ó��
	if (nDevId & DEV_SPR)										// ��ǥ����
	{
		if (!strTempSprPrintData.GetLength())					// ��ǥ�ڷṫ
			strTempSprPrintData = "20";
		if (fnAPL_GetAvailDevice(DEV_SPR))						// ��밡������ġ���ϱ�
			fnSPR_Print(strTempSprPrintData);					// ����(�����ڷ�)
	}

	if (nDevId & DEV_JPR)										// ��������
	{
		if (!strTempJprPrintData.GetLength())					// �����ڷṫ
			strTempJprPrintData = "20";
		strTempJprPrintData += ",2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A";
																// '*'�������
		if (fnAPL_GetAvailDevice(DEV_JPR))						// ��밡������ġ���ϱ�
			fnJPR_Print(strTempJprPrintData);					// ����(�����ڷ�)
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

// ���������Ϸù�ȣ����
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



// ������������
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


	_mkdir(_EJR_DIR);											// Dir����
	_mkdir(_EJR_TXT);
	sprintf(szTempFileName, "%s\\%8.8s.JNL", _EJR_TXT, m_pProfile->TRANS.YYYYMMDD);
	
	if(!CFile::GetStatus(szTempFileName, TempFileStatus))		// File����ó��ڰ���
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

	for (int j = 0; j < SprRowOption; j++)							// �ڷ��ۼ�
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

	// ���������Է�
	// OM_ELECJNL||
	strTempDbData = "OM_ELECJNL";

	// �Է���(YYYYMMDD)||
	strTemp.Format("%8.8s", Ejr.Date);
	strTempDbData += "||" + strTemp;

	// �Է½ð�(HHMMSS)||
	strTemp.Format("%6.6s", Ejr.Time);
	strTempDbData += "||" + strTemp;

	// �Ϸù�ȣ||
	strTemp.Format("%6.6s", m_pProfile->TRANS.SerialNo);
	strTempDbData += "||" + strTemp;

	// �ŷ�����(0-��Ÿ,1-����,2-��������,3-��ü,4-�Ա�,5-��ȸ,6-IC)||
	strTempDbData += "||0";

	// ��ü����(0-ī��,1-����,2-����ü)||
	strTempDbData += "||";

	// ��Ÿ�౸��(0-����,1-Ÿ��)||
	strTempDbData += "||";

	// �����ı���(0-������,1-������)||
	strTempDbData += "||";

	// ��Ұŷ�����(0-����,1-���)||
	strTempDbData += "||";

	// �۱������ڵ�||
	strTempDbData += "||";

	// ���� �ڵ�||
	strTempDbData += "||";


	// ���¹�ȣ||
	strTemp.Format("%20.20s", "00000000000000000000");
	strTempDbData += "||" + strTemp;

	// Host�ǰŷ���¥||
	strTemp.Format("%8.8s", Ejr.Date);
	strTempDbData += "||" + strTemp;

	// �Ҵ��ڵ� (ȣ��Ʈ�����ڵ�)||
	strTempDbData += "||";

	// �ŷ��ݾ�||
	strTemp.Format("%08d", 0);
	strTempDbData += "||" + strTemp;

	// Host Retrieval Number||
	strTempDbData += "||";

	// ������||
	strTempDbData += "||";

	// ��ǥ�ż�||
	strTempDbData += "||";

	// ��ǥ�Ϸù�ȣ||
	strTempDbData += "||";

	// ������ �ż�||��õ���� �ż�||õ���� �ż�||
	strTempDbData += "||||||";
// 	strTempDbData += "||" + Int2Asc(EjrAmount.Cash10000Num);	
// 	strTempDbData += "||" + Int2Asc(EjrAmount.Cash5000Num);		
// 	strTempDbData += "||" + Int2Asc(EjrAmount.Cash1000Num);		

	// �����ǥ(10������)���||�����ǥ(30������)���||
	// �����ǥ(50������)���||�����ǥ(100������)���||
	// Ÿ���ǥ(10������)���||Ÿ���ǥ(30������)���||
	// Ÿ���ǥ(50������)���||Ÿ���ǥ(100������)���||
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


	// ���� ī��Ʈ||����ڵ�||
	strTempDbData += "||||";
	
	// Capture Image1��������||Capture Image1��������||
	// Capture Image2��������||Capture Image2��������||
	// Capture Image3��������||Capture Image3��������||
	// Capture Image4��������||Capture Image4��������||
	// U8100 AP ���泻�� #02 - ī�޶� ������ ����
	// ���뵿���� ������� - ���� ����κ��� ���������� �ش޶�� ������ ó���� �Ǿ� �־�� ��
	// DB�� ����� ������ ���� ������OM���� PLAY������
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

	// ��ǥ����1||��ǥ����2||��ǥ����3||��ǥ����4||��ǥ����5||
	// ��ǥ����6||��ǥ����7||��ǥ����8||��ǥ����9||��ǥ����10||
	// ��ǥ����11||��ǥ����12||��ǥ����13||��ǥ����14||��ǥ����15||
	// ��ǥ����16||��ǥ����17||// ��ǥ����18||��ǥ����19||
	for (i = 0; i < 19; i++)
	{
		sprintf(szTemp, "%%%d.%ds", JPR_MAX_COL, JPR_MAX_COL);
		strTemp.Format(szTemp, Ejr.Data[i]);
		strTempDbData += "||" + strTemp;
	}


	// Ű��ȣ                //01234567890123456789
	strTemp.Format("%20.20s", "                    ");
	strTempDbData += "||" + strTemp;

	fnAPL_EjrDbManagerSend(strTempDbData);						// ���������ڷ�����
	
	return TRUE;
}

// ���������ڷ�����
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

// ���Կ�
// 3Camera, Preview����, �ؽ�Ʈ���
// Capture�� ȭ�鿡 �ؽ�Ʈ�� �����ϵ��� ����
int CDevCmn::fnAPL_CaptureHandSave(int nIndex, int nFirstFlag, LPCTSTR szDefault)			// U8100-AP����
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureHandSave(nIndex[%d])", nIndex);
	
	CString	szEditData("");										// U8100-AP����
	CString	strTemp(szDefault);									// V07-03-01-#05
	int nCaptureResult = FALSE;

	if (nFirstFlag)												// ù����û���ó��
	{
		fnAPL_CaptureSaveEnd();									// �Կ�����&����ó��
		for (int i = 0; i < 10; i++)
			DeleteFile(GetSprintf("%s%1.1d.JPG", _EJR_CAP, i));	
	}
	

	if (fnCMR_GetHandCameraDeviceStatus() == NORMAL)
	{
		CaptureLastIndex = nIndex;								// ī�޶���������Index
		szEditData = GetSprintf("%s%1.1d", _EJR_CAP, nIndex);	// U8100-AP����
		if (strTemp != "")										// V07-03-01-#05  U8100-AP����-start
		{
			szEditData += ",";
			szEditData += strTemp;								// V07-03-01-#05
		}
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("��  �Կ� : Index %02d", nIndex));
		fnCMR_CaptureHand(szEditData);	// ���Կ�(�����̸�) 
						
HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_CaptureHandSave(szEditData)", szEditData.GetBuffer(0), szEditData.GetLength());
																// ���Կ�(�����̸�)
	}
	else
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("��  �Կ� : �Ұ�(������)"));
		MsgDump(MAC_TRACE_FREE_FILE("Log"), "��ABNORMAL�� fnAPL_CaptureHandSave::���뱸ī�޶� �������������� �Կ��Ұ�");
	}
	
	return nCaptureResult;
}

// ���Կ�
// 3Camera, Preview����, �ؽ�Ʈ���
// Capture�� ȭ�鿡 �ؽ�Ʈ�� �����ϵ��� ����
int CDevCmn::fnAPL_CaptureFaceSave(int nIndex, int nFirstFlag, LPCTSTR szDefault)		// U8100-AP����
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureFaceSave(nIndex[%d])", nIndex);

	CString	szEditData("");										
	CString	strTemp(szDefault);								
	int nCaptureResult = FALSE;

	if (nFirstFlag)												// ù����û���ó��
	{
		fnAPL_CaptureSaveEnd();									// �Կ�����&����ó��
		for (int i = 0; i < 10; i++)
			DeleteFile(GetSprintf("%s%1.1d.JPG", _EJR_CAP, i));	
	}
	
	if (fnCMR_GetFaceCameraDeviceStatus() == NORMAL)
	{
		CaptureLastIndex = nIndex;								// ī�޶���������Index
		szEditData = GetSprintf("%s%1.1d", _EJR_CAP, nIndex);	
		if (strTemp != "")										
		{
			szEditData += ",";
			szEditData += strTemp;								
		}

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���Կ� : Index %02d", nIndex));		
		fnCMR_CaptureFace(szEditData);							
HexDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPL_CaptureFaceSave(szEditData)", szEditData.GetBuffer(0), szEditData.GetLength());
	}
	else
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("���Կ� : �Ұ�(������)"));
		MsgDump(MAC_TRACE_FREE_FILE("Log"), "��ABNORMAL�� fnAPL_CaptureHandSave::���뱸ī�޶� �������������� �Կ��Ұ�");
	}

	return nCaptureResult;
}

// �ĸ��Կ�
// 3Camera, Preview����, �ؽ�Ʈ���
// Capture�� ȭ�鿡 �ؽ�Ʈ�� �����ϵ��� ����
int CDevCmn::fnAPL_CaptureRoomSave(int nIndex, int nFirstFlag, LPCTSTR szDefault)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureRoomSave(nIndex[%d])", nIndex);

	CString	szEditData("");
	CString	strTemp(szDefault);									
	int		ret = 0;
// ������ 1�� ���� (U8100 AP ���泻�� #13)
// U8100�� �ƴѰ�쿡�� �Ĺ�ī�޶� ���� �ʴ´�
	if (m_pDevCmn->AtmDefine.CashHandler != HBRM_SET)
		return TRUE;

	if (nFirstFlag)												// ù����û���ó��
	{
		fnAPL_CaptureSaveEnd(TRUE);								// �Կ�����&����ó��
		for (int i = 0; i < 10; i++)
			DeleteFile(GetSprintf("%s%1.1d.JPG", _EJR_CAP, i));	
	}

	if (fnCMR_GetRoomCameraDeviceStatus() == NORMAL)			// �Ĺ�ī�޶� �������� ���� ī�޶� ���� �ʴ´�
	{
		CaptureLastIndex = nIndex;								// ī�޶���������Index
		szEditData = GetSprintf("%s%1.1d", _EJR_CAP, nIndex);
		
		if (strTemp != "")										
		{
			szEditData += ",";
			szEditData += strTemp;							
		}

		ret = fnCMR_CaptureRoom(szEditData);					// �ĸ��Կ�(�����̸�)
	}
	else
	{
MsgDump(TRACE_CODE_MODE, "Log",  __FILE__, __LINE__, "��ABNORMAL�� fnAPL_CaptureFaceSave::�Ĺ�ī�޶� �������������� �Կ��Ұ�");
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureRoomSave() return[%d]", ret);

	return TRUE;
}


// ������ 1�� ����  - ����κ� �߻�
// �������Կ�	U8100-VOD-start
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

// �Կ�����&����ó��
int CDevCmn::fnAPL_CaptureSaveEnd(int nSaveFlag)
{
	CString		strFileName = _T("");

	if (m_pDevCmn->fnAPL_GetDefineDevice(DEV_CMR))				// �ӵ�����
		;
	else
		return TRUE;

	if (!CaptureLastIndex)										// ī�޶���������Index
		return TRUE;

	if (!nSaveFlag)												// ī�޶���������������
	{
		if (!PathFileExists(GetSprintf("%s%1.1d.JPG", _EJR_CAP, CaptureLastIndex)))
			return TRUE;
	}

	ULONG  WiatTime = TimerSet(K_2_WAIT);						// ī�޶�������������10�ʴ��
	while (TRUE)
	{
		Delay_Msg();											// Msg & Task
		if (PathFileExists(GetSprintf("%s%1.1d.JPG", _EJR_CAP, CaptureLastIndex)))
			break;												// ī�޶�������������
		if (CheckTimer(WiatTime))
			break;
	}	

	CaptureLastIndex = 0;										// �����Թ���

	_mkdir(_EJR_DIR);											// Dir����
	_mkdir(GetSprintf("%s", _EJR_IMG));
	_mkdir(GetSprintf("%s\\%4.4s", _EJR_IMG, &m_pProfile->TRANS.YYYYMMDD[4]));
	for (int i = 0; i < 10; i++)								
	{															// 1�����ڷ����
		DeleteFile(GetSprintf("%s\\%4.4s\\%8.8s%8.8s_%6.6s_%02d.JPG", _EJR_IMG, &m_pProfile->TRANS.YYYYMMDD[4], &m_pProfile->NETWORK.AtmSerialNum[0], &m_pProfile->TRANS.YYYYMMDD, m_pProfile->TRANS.SerialNo, i));
																// �ڷ�����(Move)
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
		if ((GetCaptureWaitCnt > K_5_WAIT) && (GetCaptureFaceCnt < K_20_WAIT))	// K_10_WAIT���� 5�ʾ� ������
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
// ����ڵ��������ϱ� (�⵿���)
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
				if (ErrorConvTbl_U3100K[i].CmpLength >= nTempCmpLength)	// Length��ũ��,�����ΰͰ�
				{
					nTempCmpLength = ErrorConvTbl_U3100K[i].CmpLength;
					szTempErrorCodeKind = ErrorConvTbl_U3100K[i].ES[0];	// 0:������,1:������,2:���̼�,3:����,4:�������
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
				if (ErrorConvTbl_U8100[i].CmpLength >= nTempCmpLength)	// Length��ũ��,�����ΰͰ�
				{
					nTempCmpLength = ErrorConvTbl_U8100[i].CmpLength;
					szTempErrorCodeKind = ErrorConvTbl_U8100[i].ES[0];	// 0:������,1:������,2:���̼�,3:����,4:�������
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
				if (ErrorConvTbl_U3100K[i].CmpLength >= nTempCmpLength)	// Length��ũ��,�����ΰͰ�
				{
					nTempCmpLength = ErrorConvTbl_U3100K[i].CmpLength;
					szTempErrorCodeKind = ErrorConvTbl_U3100K[i].ES[0];	// 0:������,1:������,2:���̼�,3:����,4:�������
				}
			}
		}

	}

	return szTempErrorCodeKind;
}

// ����ڵ��������ϱ�
int	CDevCmn::fnAPL_GetErrorCodeKind(LPCTSTR szHsErrorCode)
{
	int		nTempErrorCodeKind = 0;
	int		nTempCmpLength = 0;
	if (m_pProfile->DEVICE.MachineType == U8100)				// U8100-AP����
	{
		for (int i = 1; i < itemof(ErrorConvTbl_U8100); i++) 
		{
			if (memcmp(szHsErrorCode, ErrorConvTbl_U8100[i].MTC, ErrorConvTbl_U8100[i].CmpLength) == 0)
			{
				if (ErrorConvTbl_U8100[i].CmpLength >= nTempCmpLength)	// Length��ũ��,�����ΰͰ�
				{
					nTempCmpLength = ErrorConvTbl_U8100[i].CmpLength;
					nTempErrorCodeKind = ErrorConvTbl_U8100[i].Kind;	// 0:������,1:������,2:���̼�,3:����,4:�������
				}
			}
		}
	}
	else
	if (m_pProfile->DEVICE.MachineType == U3100K)				// U8100-AP����
	{
		for (int i = 1; i < itemof(ErrorConvTbl_U3100K); i++) 
		{
			if (memcmp(szHsErrorCode, ErrorConvTbl_U3100K[i].MTC, ErrorConvTbl_U3100K[i].CmpLength) == 0)
			{
				if (ErrorConvTbl_U3100K[i].CmpLength >= nTempCmpLength)	// Length��ũ��,�����ΰͰ�
				{
					nTempCmpLength = ErrorConvTbl_U3100K[i].CmpLength;
					nTempErrorCodeKind = ErrorConvTbl_U3100K[i].Kind;	// 0:������,1:������,2:���̼�,3:����,4:�������
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
				if (ErrorConvTbl[i].CmpLength >= nTempCmpLength)	// Length��ũ��,�����ΰͰ�
				{
					nTempCmpLength = ErrorConvTbl[i].CmpLength;
					nTempErrorCodeKind = ErrorConvTbl[i].Kind;		// 0:������,1:������,2:���̼�,3:����,4:�������
				}
			}
		}
	}

	if (nTempErrorCodeKind == 0)								// ������
		nTempErrorCodeKind = 1;									// ������

	return nTempErrorCodeKind;
}

// LC���ϱ�
CString	CDevCmn::fnAPL_GetLC(LPCTSTR szHsErrorCode, int nTranProc)
{
	CString		strTempLC("ZZ"); //#0102
	int			nTempCmpLength = 0;


	if (m_pProfile->DEVICE.MachineType == U8100)			
	{
		for (int i = 1; i < itemof(ErrorConvTbl_U8100); i++) 
		{
			if (memcmp(szHsErrorCode, ErrorConvTbl_U8100[i].MTC, ErrorConvTbl_U8100[i].CmpLength) == 0)
			{													// Length��ũ��,�����ΰͰ�
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
			{													// Length��ũ��,�����ΰͰ�
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
				if (ErrorConvTbl[i].CmpLength >= nTempCmpLength)	// Length��ũ��,�����ΰͰ�
				{
					nTempCmpLength = ErrorConvTbl[i].CmpLength;
					Strcpy(strTempLC, ErrorConvTbl[i].LC, 2);
				}
			}
		}
	}

	if (nTranProc == TRAN_NULL)									// Tran�ŷ�ó�� ���� ��
		nTranProc = TranProc;									// Tran�ŷ�ó��

	if (strTempLC == "?2")
	{
		if (nTranProc == TRAN_DEP)								// �Ա�
			strTempLC = "82";
		else													// ���,�ܷ�
			strTempLC = "42";
	}

	if (strTempLC == "?4")										// ��ǥ����ֽ� : 2003.12.24
	{
		if (nTranProc == TRAN_DEP)								// �Ա�
			strTempLC = "92";
		else													// ���,�ܷ�
			strTempLC = "44";
	}

	if (strTempLC == "??")										// �������ڵ��ʱ�ȭ
		strTempLC = "00";

	return strTempLC;
}

// ����ʱ�ȭ��弳��
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

	strOmInformation = GetShareData("OM_�������1");			// ��������(�������1)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInitModeSet:strOmInformation[%s]", strOmInformation);
	SplitString(strOmInformation, "||", strOmInformationArray);

	if (strOmInformationArray.GetSize() >= 17)					// 2003.12.22
	{
		strOmInformationArray[0] = "INIT";						// INIT����
		strOmInformationArray[16] = "ON";						// ON����	: 2003.12.22 Index����(15->16)

		for (i = 0; i < strOmInformationArray.GetSize(); i++)
		{
			if (i == 0)
				strTemp = strOmInformationArray[i];
			else
				strTemp += "||" + strOmInformationArray[i];
		}
		SetShareData("OM_�������1", strTemp);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInitModeSet:strTemp[%s]", strTemp);
	}


	SetShareData("OM_RESULT", OM_NORMAL);						// �ʱ�⺻����ó��
	return TRUE;
}

// �������
int	CDevCmn::fnAPL_ClerkInformation(int nInitFlag)
{

		
	CString	strTemp("");
	static int nFirstFlag = TRUE;
	int		nTotal = 0;
	int		nOutCheckKind = CHECKKIND_UNKNOWN;				

/////////////////////////////////////////////////////////////////////////////
/**********************************************************************************************************************/
	// ��������(U8100)
	// OM_���ΰ��_U8100		
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

	// OM_���ΰ��_VANIII		
	Om.Main_CD_VanIII_ModeStatus = "";
										
	// 0:���(4�ڸ�)
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


	// 3:��Ż���(ONLINE,OFFLINE)
	if (fnAPL_CheckHostLine())
		Om.Main_CD_VanIII_ModeStatus += "||ONLINE";
	else
		Om.Main_CD_VanIII_ModeStatus += "||OFFLINE";

	// 4:��ܺλ���(����,����)
	if (CabinetStatus == DOOR_OPENED)
		Om.Main_CD_VanIII_ModeStatus += "||����";
	else
		Om.Main_CD_VanIII_ModeStatus += "||����";
	
	// 5:�޹�����(����,����)
	if (DoorStatus == DOOR_OPENED)
		Om.Main_CD_VanIII_ModeStatus += "||����";
	else
		Om.Main_CD_VanIII_ModeStatus += "||����";

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

	//14:ȸ���ڽ�(FULL,HIGH,LOW,EMPTY,MISSING,FATAL,ETC) = #0064-KIM*
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
	// 20:BP1IP�ּ�(NNN.NNN.NNN.NNN)
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

	// 25:����_IP_ID(123456)=> ID+1139(CD�ڵ�))
	Om.Main_CD_VanIII_ModeStatus += "||" + IniGetStr("D:\\INI\\ATM_CFG.ini", ATM_CFG_SEC, "ATM_MOVE_ID", "ID0000");
	// 26:����_IP_PWD(CD�ڵ�+CD�ڵ�)=> 1139+1139
	Om.Main_CD_VanIII_ModeStatus += "||" + IniGetStr("D:\\INI\\ATM_CFG.ini", ATM_CFG_SEC, "ATM_MOVE_PWD", "00000000");
	// 27:����ڵ� 16 Byte
	strTemp = IniGetStr("D:\\INI\\NETWORK.INI", "NETWORK", "MachineMakeNum", ZERO16);
	strTemp = strTemp.Mid(1,16);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;

	// 28:�����ν� Status (����, ���, �̻��)
	if (fnAPL_GetDefineDevice(DEV_FNG))
	{
		if(fnFNG_GetDeviceStatus() == NORMAL)
			Om.Main_CD_VanIII_ModeStatus += "||����";
		else
			Om.Main_CD_VanIII_ModeStatus += "||���";
	}
	else
		Om.Main_CD_VanIII_ModeStatus += "||�̻��";

	// 29:RPC + �ڱ���ġ (����, ���)
	if(fnAPL_GetDefineDevice(DEV_RDCTRL))
	{
		if (fnLDCtrl_GetDeviceStatus() != NORMAL)
			Om.Main_CD_VanIII_ModeStatus += "||���";
		else
			Om.Main_CD_VanIII_ModeStatus += "||����";
	}
	else
		Om.Main_CD_VanIII_ModeStatus += "||�̻��";
 
	// 30:�����ġ (����, ���, �̻��)
	if (fnAPL_GetDefineDevice(DEV_SPG))
	{
		if(fnSPACK_GetDeviceStatus() == TRUE)
			Om.Main_CD_VanIII_ModeStatus += "||����";
		else
			Om.Main_CD_VanIII_ModeStatus += "||���";
	}
	else
		Om.Main_CD_VanIII_ModeStatus += "||�̻��";

	//#0024 Kim.G.J
	// 31:������ ����(EVERYDAY)
	Om.Main_CD_VanIII_ModeStatus += "||" + IniGetStr("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice11", "0");
	// 32:�귣�� ����
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
		// 34:SPACK �ܺμ���1
		if(fnSPACK_GetExternalSensorStatus_1())
			Om.Main_CD_VanIII_ModeStatus += "||1";
		else
			Om.Main_CD_VanIII_ModeStatus += "||0";

		// 35:SPACK �ܺμ���2
		Om.Main_CD_VanIII_ModeStatus += "||2";
	}
	else
	if(nGetSt == 2)
	{
		// 34:SPACK �ܺμ���1
		Om.Main_CD_VanIII_ModeStatus += "||2";

		// 35:SPACK �ܺμ���2
		if(fnSPACK_GetExternalSensorStatus_2())
			Om.Main_CD_VanIII_ModeStatus += "||1";
		else
			Om.Main_CD_VanIII_ModeStatus += "||0";
	}
	else
	if(nGetSt == 3)
	{
		// 34:SPACK �ܺμ���1
		if(fnSPACK_GetExternalSensorStatus_1())
			Om.Main_CD_VanIII_ModeStatus += "||1";
		else
			Om.Main_CD_VanIII_ModeStatus += "||0";

		// 35:SPACK �ܺμ���2
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
	//36:����ڵ� (BRM/CDU,MCR,SPR) 
	if(!fnAPL_CheckError())
	{
		strTemp.Empty();
		strTemp = GetSprintf("%5.5s(%2.2s)", CurErrBuff.ErrorCode, &CurErrBuff.ErrorCode[5]);
		Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	}
	else
		Om.Main_CD_VanIII_ModeStatus += "||         ";


	//37:RPC (Port ����-> 0:�̻��, 7:SBC B/D = 8, 9:Ennowell B/D = 10)  
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

	
	//41:������ݻ���(����,�Ұ�)
	if (TranAvail & (TRAN_WITH_CASH | TRAN_WITH_50TCASH))		
		Om.Main_CD_VanIII_ModeStatus += "||����";
	else
		Om.Main_CD_VanIII_ModeStatus += "||�Ұ�";

	//42:�����Աݻ���(����,�Ұ�)
	if (TranAvail & TRAN_DEP_CASH)
		Om.Main_CD_VanIII_ModeStatus += "||����";
	else
		Om.Main_CD_VanIII_ModeStatus += "||�Ұ�";
	
	//43:Brm����ġ(����,Ż��)
	if (BrmLoc == NORMAL_POS)
		Om.Main_CD_VanIII_ModeStatus += "||����";
	else
		Om.Main_CD_VanIII_ModeStatus += "||Ż��";

	//44:Brm���ī��Ʈ����(����,Ż��)
	if (ClerkCSTLoc == NORMAL_POS)
		Om.Main_CD_VanIII_ModeStatus += "||����";
	else
		Om.Main_CD_VanIII_ModeStatus += "||Ż��";

	//45:����ī��Ʈ����(����,�ܷ���,�亸��,�ܷ���)
	if (RecycleBoxStatus == CST_NORMAL)
		Om.Main_CD_VanIII_ModeStatus += "||����";
	else
	if (RecycleBoxStatus == CST_NEAR)
		Om.Main_CD_VanIII_ModeStatus += "||�ܷ���";
	else
	if (RecycleBoxStatus == CST_EMPTY)
		Om.Main_CD_VanIII_ModeStatus += "||�亸��";
	else
		Om.Main_CD_VanIII_ModeStatus += "||�ܷ���";

	//46:ȸ���ڽ�(FULL,HIGH,LOW,EMPTY,MISSING,FATAL,ETC)
	strTemp.Empty();
	strTemp = RegGetStr(_REGKEY_BRMSTATUS, "FRJBStatus");
	if(strTemp == "FULL" || strTemp == "HIGH")
		Om.Main_CD_VanIII_ModeStatus += "||�ܷ���";
	else
	if(strTemp == "MISSING")
		Om.Main_CD_VanIII_ModeStatus += "||Ż��";
	else
	if(strTemp == "FATAL")
		Om.Main_CD_VanIII_ModeStatus += "||������";
	else
	if(strTemp == "NOVALUES")
		Om.Main_CD_VanIII_ModeStatus += "||������";
	else
	if(strTemp == "NOREFERENCE")
		Om.Main_CD_VanIII_ModeStatus += "||������";
	else
	if(strTemp == "MANIP")
		Om.Main_CD_VanIII_ModeStatus += "||������";
	else
		Om.Main_CD_VanIII_ModeStatus += "||����";

	//47:Reject(����,�ܷ���,Ż��)
	if (RejectCSTLoc == ABNORMAL_POS)
		Om.Main_CD_VanIII_ModeStatus += "||Ż��";
	else
	if ((RejectCSTStatus == CST_EMPTY)	||
		(RejectCSTStatus == CST_NORMAL))
		Om.Main_CD_VanIII_ModeStatus += "||����";
	else
		Om.Main_CD_VanIII_ModeStatus += "||�ܷ���";
	
	//48:�Աݸż�
	strTemp.Empty();	
	strTemp.Format("%d",  RegGetInt(_REGKEY_BRMINFO"\\CASHIN_ONLY", "TotalCnt"));
	Om.Main_CD_VanIII_ModeStatus += "||"+ strTemp;
	//49:ȸ���ż�
	strTemp.Empty();	
	strTemp.Format("%d",  RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt"));
	Om.Main_CD_VanIII_ModeStatus += "||"+ strTemp;
	//50:����Ʈ�ż�
	strTemp.Empty();	
	strTemp.Format("%d",  RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt"));
	Om.Main_CD_VanIII_ModeStatus += "||"+ strTemp;
	//51:���� VOLUME
	strTemp.Empty();	
	strTemp.Format("%d",  m_pProfile->DEVICE.SoundVolume);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//52:��ǥ����
	strTemp.Empty();	
	strTemp.Format("%d",  m_pProfile->DEVICE.SHUHandler);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//53:�Ϸù�ȣ(6�ڸ�)
	strTemp.Empty();	
	strTemp.Format("%s",  m_pProfile->TRANS.SerialNo);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//54:���񽺰ŷ�����(1,2,3)
	strTemp.Empty();	
	strTemp.Format("%d",  m_pProfile->TRANS.TranService);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//55:��������ȸ(0,1)
	strTemp.Empty();	
	strTemp.Format("%d",  m_pProfile->TRANS.TranInqFee);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//56:�����ִ�ݾ�(30,50,70)
	strTemp.Empty();	
	strTemp.Format("%d",  m_pProfile->TRANS.JaMaxDispAmt);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//57:Ÿ���ִ�ݾ�(30,50,70)
	strTemp.Empty();	
	strTemp.Format("%d",  m_pProfile->TRANS.TaMaxDispAmt);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//58:�����ýð�����||000000
	strTemp.Empty();	
	strTemp.Format("%6.6s",  m_pProfile->TRANS.RebootHHMMSS);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	
	//59:���� ���� ���� (AMS)
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
			Om.Main_CD_VanIII_ModeStatus += "||����";
		else
			Om.Main_CD_VanIII_ModeStatus += "||�Ұ�";
	}
	else
		Om.Main_CD_VanIII_ModeStatus += "||�̻��";
	
	//62:PBM ����
	strTemp.Empty();
	strTemp.Format("%d", m_pProfile->DEVICE.PBHandler);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	
	//63:�ؿ�ī�� �ִ� �ݾ�
	strTemp.Empty();
	strTemp.Format("%d", IniGetInt(_TRANS_INI, TRANS_SEC, "ForeginMaxDispAmt", 30));
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//64:�����ATM ��� ��/��
	strTemp.Empty();
	strTemp.Format("%d", m_pProfile->DEVICE.EarHandler);
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	strTemp.Empty();
	strTemp.Format("%d", IniGetInt(_TRANS_INI, TRANS_SEC, "foreginBrand", 0));	
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//66:���̷κ� ���� ����(0/1)
	strTemp.Empty();
	strTemp.Format("%d", IniGetInt(_TRANS_INI, TRANS_SEC, "viRobotCtrl", 0));
	Om.Main_CD_VanIII_ModeStatus += "||" + strTemp;
	//67:������ ���� ����(0000000 -> �Ͽ�ȭ�������) - 1010000 �̸� �� ȭ ������.
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
	// ��������(�������1)
	// OM_�������1			
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

	// 1:������ݻ���(����,�Ұ�)
	if (UserTranAvail & (TRAN_WITH_CASH | TRAN_WITH_50TCASH))		
		Om.Main_1LineStatus += "||����";
	else
		Om.Main_1LineStatus += "||�Ұ�";

	// 2:�����Աݻ���(����,�Ұ�)
	if (UserTranAvail & TRAN_DEP_CASH)
		Om.Main_1LineStatus += "||����";
	else
		Om.Main_1LineStatus += "||�Ұ�";

	// 3:��ǥ��ݻ���(����,�Ұ�)
	if (UserTranAvail & TRAN_WITH_CHECK)
		Om.Main_1LineStatus += "||����";
	else
		Om.Main_1LineStatus += "||�Ұ�";

	// 4:��ǥ�Աݻ���(����,�Ұ�)
	if (UserTranAvail & TRAN_DEP_CHECK)
		Om.Main_1LineStatus += "||����";
	else
		Om.Main_1LineStatus += "||�Ұ�";

	// 5:����ŷ�����(����,�Ұ�)
	if (UserTranAvail & TRAN_PBM_OK)
		Om.Main_1LineStatus += "||����";
	else
		Om.Main_1LineStatus += "||�Ұ�";

	// 6:����ȭ�����(����,�Ұ�)
	if (m_pProfile->DEVICE.ICCardDevice != NOT_SET)
		Om.Main_1LineStatus += "||����";
	else
		Om.Main_1LineStatus += "||�Ұ�";

	// 7:�ؿܰŷ�����(����,�Ұ�)
	if (m_pProfile->DEVICE.DesHandler != NOT_SET)
		Om.Main_1LineStatus += "||����";
	else
		Om.Main_1LineStatus += "||�Ұ�";

	// 8:�����ŷ�(����,�Ұ�)
	Om.Main_1LineStatus += "||";						

	// 9:���ܼ��ŷ�����(����,�Ұ�)
	if ((m_pProfile->DEVICE.IrdaHandler != NOT_SET)	&&
		(m_pProfile->DEVICE.RfdHandler == NOT_SET)	)
		Om.Main_1LineStatus += "||����";
	else
		Om.Main_1LineStatus += "||�Ұ�";

	// 10:���ڵ�ŷ�����(����,�Ұ�)							
	if (m_pProfile->DEVICE.BarHandler != NOT_SET)
		Om.Main_1LineStatus += "||����";
	else
		Om.Main_1LineStatus += "||�Ұ�";

	// 11:RF�ŷ�����(����,�Ұ�)								
	// (m_pProfile->DEVICE.RfdHandler != NOT_SET)
	// Om.Main_1LineStatus += "||����";
	// else
	//	Om.Main_1LineStatus += "||�Ұ�";

	// 11:��ȣȭ�ŷ�����(����,�Ұ�)
	if (m_pProfile->DEVICE.SeedHandler != NOT_SET)
		Om.Main_1LineStatus += "||����";
	else
		Om.Main_1LineStatus += "||�Ұ�";

	// 12:���͸�������
	strTemp = RegGetStr(_REGKEY_UPSSTATUS, "BattCharge", "0");
	if (Asc2Int(strTemp) > 95)									
		strTemp.Format("%d", 100);
	else
		strTemp.Format("%d", Asc2Int(strTemp));
	Om.Main_1LineStatus += "||" + strTemp;

	// 13:���͸�����
	if (RegGetStr(_REGKEY_UPSSTATUS, "UPSPowering", "FALSE") == "FALSE")
		Om.Main_1LineStatus += "||����";
	else
		Om.Main_1LineStatus += "||����";

	// 14:��Ż���(ONLINE,OFFLINE)
	if (fnAPL_CheckHostLine())
		Om.Main_1LineStatus += "||ONLINE";
	else
		Om.Main_1LineStatus += "||OFFLINE";
	
	// 15:�޹�����(����,����)
	if (DoorStatus == DOOR_OPENED)
		Om.Main_1LineStatus += "||����";
	else
		Om.Main_1LineStatus += "||����";

	// 16:�������ġ����(ON,OFF)
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
	// 18:Kcash����(����,�Ұ�)									// 
//	if (m_pProfile->DEVICE.KcashHandler == KCASH_SET)
//		Om.Main_1LineStatus += "||����";
//	else
//		Om.Main_1LineStatus += "||�Ұ�";

// ��ֿ�ATM ���� �������
// OM�� �����θ�� ���ɿ��θ� ����								// �ΰ���ġ���� 14�� �̵�
//	if (m_pProfile->DEVICE.EarHandler == EAR_SET)
//		Om.Main_1LineStatus += "||����";
//	else
//		Om.Main_1LineStatus += "||�Ұ�";

// ��ֿ�ATM ���� �������										// �ŷ��������� 9�� �̵�
// OM�� ȭ��Ȯ�� ��� ���ɿ��θ� ����
//	if (m_pProfile->TRANS.ZoomFlag == ZOOM_SET)
//		Om.Main_1LineStatus += "||����";
//	else
//		Om.Main_1LineStatus += "||�Ұ�";

/////////////////////////////////////////////////////////////////////////////
	// ��������(�������2)
	// OM_�������2
	Om.Main_2LineStatus = "";			

	// 0:����2(5�ڸ�)
	strTemp.Format("%5.5s", &m_pProfile->NETWORK.SubBranchNum[3]);
	Om.Main_2LineStatus += strTemp;
										
	// 1:���(8�ڸ�)
	strTemp.Format("%6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
	Om.Main_2LineStatus += "||" + strTemp;


	// 2:�������(IOC,HDLC,SWP,SDLC,TCP/IP,CCL,BAS)
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
	
	// 5:�����Ǹ��(����,����) : 
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
		Om.Main_2LineStatus += "||����";
	else
		Om.Main_2LineStatus += "||����";

	// 6:����ī����(' ','0','1','3','5','6','9')
	Om.Main_2LineStatus += "||" + Om.ProcCount;
										
	// 7:����ڵ�(1234567)
	Om.Main_2LineStatus += "||" + Om.ErrorCode;

	// 8:����/A4 ����
	if ((OKI_SET == AtmDefine.CashHandler) || (HBRM_SET == AtmDefine.CashHandler))
		Om.Main_2LineStatus += "||����";							

/////////////////////////////////////////////////////////////////////////////
	// ��������(�������ݺ�����Ȳ) 
	// OM_�������ݺ�����Ȳ
	Om.Main_CashStatus = "";			
	
	// 0:ī��Ʈ1�����
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt"));
	Om.Main_CashStatus += strTemp;

	// 1:ī��Ʈ2�����
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt"));
	Om.Main_CashStatus += "||" + strTemp;

	// 2:ī��Ʈ3�����
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt"));
	Om.Main_CashStatus += "||" + strTemp;

	// 3:ī��Ʈ4����� (for OKI BRM)
	if (HBRM_SET == AtmDefine.CashHandler)
	{
		strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt"));
		Om.Main_CashStatus += "||" + strTemp;
	}

/////////////////////////////////////////////////////////////////////////////
	// ��������(�����ܷ�����)
	// OM_�����ܷ�����
	Om.Main_Status = "";

	// 0:��ǥ�ܷ�����(����,�ܷ���,�亸��)
	if (SlipStatus == SLIP_NORMAL)
		Om.Main_Status += "����";
	else
	if (SlipStatus == SLIP_LOW_END)
		Om.Main_Status += "�ܷ���";
	else
		Om.Main_Status += "�亸��";

	// 1:�����ܷ�����(����,�ܷ���,�亸��)
	if (JnlStatus == JNL_NORMAL)
		Om.Main_Status += "||����";
	else
	if (JnlStatus == JNL_LOW_END)
		Om.Main_Status += "||�ܷ���";
	else
		Om.Main_Status += "||�亸��";
										
	// 2:��ݼ�ǥ�ܷ�����(����,�ܷ���,�亸��)
	if (CheckCSTStatus == CST_NORMAL)
		Om.Main_Status += "||����";
	else
	if (CheckCSTStatus == CST_NEAR)
		Om.Main_Status += "||�ܷ���";
	else
		Om.Main_Status += "||�亸��";

	// 3:�Աݼ�ǥ�ܷ�����(����,�ܷ���)
	if (CassetteStatus == CST_NORMAL)
		Om.Main_Status += "||����";
	else
		Om.Main_Status += "||�ܷ���";

	// 4:�����ܷ�����(����,�ܷ���,�亸��,�ܷ���)
	if (Withdraw50TFlag)
	{
		if ((RecycleBoxStatus == CST_EMPTY)		||
			(RecycleBox50000Status == CST_EMPTY))
			Om.Main_Status += "||�亸��";
		else
		if ((RecycleBoxStatus == CST_NEAR)		||
			(RecycleBox50000Status == CST_NEAR)	)
			Om.Main_Status += "||�ܷ���";
		else
		if ((RecycleBoxStatus == CST_NORMAL)	||
			(RecycleBox50000Status == CST_NORMAL))
			Om.Main_Status += "||����";
		else
			Om.Main_Status += "||�ܷ���";
	}
	else
	{
		if (RecycleBoxStatus == CST_NORMAL)
			Om.Main_Status += "||����";
		else
		if (RecycleBoxStatus == CST_NEAR)
			Om.Main_Status += "||�ܷ���";
		else
		if (RecycleBoxStatus == CST_EMPTY)
			Om.Main_Status += "||�亸��";
		else
			Om.Main_Status += "||�ܷ���";
	}
/////////////////////////////////////////////////////////////////////////////
	// ��������(���μ�ǥ������Ȳ)
	// OM_���μ�ǥ������Ȳ
	Om.Main_CheckStatus = "";			
	
	// 0:��ݼ�ǥ�ż�
	strTemp.Format("%d", fnAPL_GetNumberOfOutCheck());
	Om.Main_CheckStatus += strTemp;

	// 1:�Աݼ�ǥ�ż�
	strTemp.Format("%d", fnAPL_GetNumberOfInCheck());
	Om.Main_CheckStatus += "||" + strTemp;

/////////////////////////////////////////////////////////////////////////////
	// ��������(��������ȸ����Ȳ) 
	// OM_��������ȸ����Ȳ
	Om.Main_CashRejectStatus = "";			
	if (HBRM_SET == AtmDefine.CashHandler)
	{
		// 0:�н�ȸ���Ըż�(�н�,�ܷ�)
		strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "FRJBNoteCnt"));
		Om.Main_CashRejectStatus += strTemp;
	
		// 1:�ϴ�1ȸ���Ըż�(��ݺҷ�����,�Աݺҷ�����,��õ��,õ��)
		strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt"));
		Om.Main_CashRejectStatus += "||" + strTemp;

		// 2:�ϴ�2ȸ���Ըż�(��ݺҷ�����,�Աݺҷ�����,��õ��,õ��) for OKI BRM
		strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RJB2NoteCnt"));
		Om.Main_CashRejectStatus += "||" + strTemp;

		// 3:���ȸ���Ըż�(����/ȸ���� �̻�����) for OKI BRM
		strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "ClerkRJBNoteCnt"));
		Om.Main_CashRejectStatus += "||" + strTemp;
	}
	else
	{
		// 0:���ȸ���Ըż�(�н�,�ܷ�)
		//	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RJB1NoteCnt"));
		strTemp.Format("%d", IniGetInt(_SP_DEVICE_INI, "BRM", "StackOfRJB1"));
		Om.Main_CashRejectStatus += strTemp;
	
		// 1:�ϴ�ȸ���Ըż�(��ݺҷ�����,�Աݺҷ�����,��õ��,õ��)
		//	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RJB2NoteCnt"));
		strTemp.Format("%d", IniGetInt(_SP_DEVICE_INI, "BRM", "StackOfRJB2"));
		Om.Main_CashRejectStatus += "||" + strTemp;
	
	}
/////////////////////////////////////////////////////////////////////////////
	// ��������(������ġ��Ȳ)
	// OM_������ġ��Ȳ
	Om.Main_DeviceStatus = "";
										
	// 0:Mcu����(����,���)
	Om.Main_DeviceStatus += Om.McuDeviceStatus;

	// 1:Mcu�������ī����(����='0',' ','0','1','3','5','6','9')
	Om.Main_DeviceStatus += "||" + Om.McuProcCount;
	
	// 2:Mcu����ڵ�(����="0000000",���="1234567")
	Om.Main_DeviceStatus += "||" + Om.McuErrorCode;
	
	// 3:Spr����
	Om.Main_DeviceStatus += "||" + Om.SprDeviceStatus;
	
	// 4:Spr�������ī����
	Om.Main_DeviceStatus += "||" + Om.SprProcCount;
	
	// 5:Spr����ڵ�
	Om.Main_DeviceStatus += "||" + Om.SprErrorCode;
	
	// 6:Spr�����ǥ�ܷ�����(����,�ܷ���,�亸��)
	strTemp = RegGetStr(_REGKEY_SPRSTATUS, "RcptPaperStat");
	if (!strTemp.CompareNoCase("FULL"))
		Om.Main_DeviceStatus += "||����";
	else
	if (!strTemp.CompareNoCase("LOW"))
		Om.Main_DeviceStatus += "||�ܷ���";
	else
		Om.Main_DeviceStatus += "||�亸��";
	
	// 7:Spr�����ǥ�ܷ�����(��,��)
	if (RegGetInt(_REGKEY_SPRSTATUS, "SpareRcptStat"))
		Om.Main_DeviceStatus += "||��";
	else
		Om.Main_DeviceStatus += "||��";
	
	// 8:Jpr����
	Om.Main_DeviceStatus += "||" + Om.JprDeviceStatus;
	
	// 9:Jpr�������ī����
	Om.Main_DeviceStatus += "||" + Om.JprProcCount;
	
	// 10:Jpr����ڵ�
	Om.Main_DeviceStatus += "||" + Om.JprErrorCode;
	
	// 11:Jpr����1�ܷ�����(����,�ܷ���,�亸��)
	strTemp = RegGetStr(_REGKEY_JPRSTATUS, "JnlPaper1Stat");
	if (!strTemp.CompareNoCase("FULL"))
		Om.Main_DeviceStatus += "||����";
	else
	if (!strTemp.CompareNoCase("LOW"))
		Om.Main_DeviceStatus += "||�ܷ���";
	else
		Om.Main_DeviceStatus += "||�亸��";
	
	// 12:Jpr����2�ܷ�����(����,�ܷ���,�亸��)
	strTemp = RegGetStr(_REGKEY_JPRSTATUS, "JnlPaper2Stat");
	if (!strTemp.CompareNoCase("FULL"))
		Om.Main_DeviceStatus += "||����";
	else
	if (!strTemp.CompareNoCase("LOW"))
		Om.Main_DeviceStatus += "||�ܷ���";
	else
		Om.Main_DeviceStatus += "||�亸��";
	
	// 13:Pbm����
	Om.Main_DeviceStatus += "||" + Om.PbmDeviceStatus;
	
	// 14:Pbm�������ī����
	Om.Main_DeviceStatus += "||" + Om.PbmProcCount;
	
	// 15:Pbm����ڵ�
	Om.Main_DeviceStatus += "||" + Om.PbmErrorCode;	

	// 16:Ucm����
	Om.Main_DeviceStatus += "||" + Om.UcmDeviceStatus;
	
	// 17:Ucm�������ī����
	Om.Main_DeviceStatus += "||" + Om.UcmProcCount;
	
	// 18:Ucm����ڵ�
	Om.Main_DeviceStatus += "||" + Om.UcmErrorCode;
	
	// 19:Ucm����ġ(����,Ż��)
	if (UcmLoc == NORMAL_POS)
		Om.Main_DeviceStatus += "||����";
	else
		Om.Main_DeviceStatus += "||Ż��";

	// 20:Ucm��ݼ�ǥ�ܷ�����(����,�ܷ���,�亸��)
	if (CheckCSTStatus == CST_NORMAL)
		Om.Main_DeviceStatus += "||����";
	else
	if (CheckCSTStatus == CST_NEAR)
		Om.Main_DeviceStatus += "||�ܷ���";
	else
		Om.Main_DeviceStatus += "||�亸��";
	
	// 21:Ucm�Աݼ�ǥ�ܷ�����(����,�ܷ���)
	if (CassetteStatus == CST_NORMAL)
		Om.Main_DeviceStatus += "||����";
	else
		Om.Main_DeviceStatus += "||�ܷ���";
	
	// 22:Brm����
	Om.Main_DeviceStatus += "||" + Om.BrmDeviceStatus;
	
	// 23:Brm�������ī����
	Om.Main_DeviceStatus += "||" + Om.BrmProcCount;
	
	// 24:Brm����ڵ�
	Om.Main_DeviceStatus += "||" + Om.BrmErrorCode;
	
	// 25:Brm����ġ(����,Ż��)
	if (BrmLoc == NORMAL_POS)
		Om.Main_DeviceStatus += "||����";
	else
		Om.Main_DeviceStatus += "||Ż��";
	
	// 26:Brm����ī��Ʈ����(����,�ܷ���,�亸��,�ܷ���)
	if (RecycleBoxStatus == CST_NORMAL)
		Om.Main_DeviceStatus += "||����";
	else
	if (RecycleBoxStatus == CST_NEAR)
		Om.Main_DeviceStatus += "||�ܷ���";
	else
	if (RecycleBoxStatus == CST_EMPTY)
		Om.Main_DeviceStatus += "||�亸��";
	else
		Om.Main_DeviceStatus += "||�ܷ���";
	
	// 27:Brm���ī��Ʈ����(����,Ż��)
	if (ClerkCSTLoc == NORMAL_POS)
		Om.Main_DeviceStatus += "||����";
	else
		Om.Main_DeviceStatus += "||Ż��";
	
	// 28:Brmȸ���Ի���(����,�ܷ���,Ż��)
	if (RejectCSTLoc == ABNORMAL_POS)
		Om.Main_DeviceStatus += "||Ż��";
	else
	if ((RejectCSTStatus == CST_EMPTY)	||
		(RejectCSTStatus == CST_NORMAL))
		Om.Main_DeviceStatus += "||����";
	else
		Om.Main_DeviceStatus += "||�ܷ���";

	// 29:Brm��������
	Om.Main_DeviceStatus += "||" + IniGetStr(_SP_DEVICE_INI, "BRM", "��������");

	// 30:Net����
	Om.Main_DeviceStatus += "||" + Om.NetDeviceStatus;
	
	// 31:Net�������ī����
	Om.Main_DeviceStatus += "||" + Om.NetProcCount;
	
	// 32:Net����ڵ�
	Om.Main_DeviceStatus += "||" + Om.NetErrorCode;
	
	// 33:App����
	Om.Main_DeviceStatus += "||" + Om.AppDeviceStatus;
	
	// 34:App�������ī����
	Om.Main_DeviceStatus += "||" + Om.AppProcCount;
	
	// 35:App����ڵ�
	Om.Main_DeviceStatus += "||" + Om.AppErrorCode;

	// 36:BRM�ڵ�����˸�(3OUT �߻��� OM Display "������� ��ġ���") MIC
	if (m_pProfile->TRANS.DeviceExecOff & DEV_BRM)
		Om.Main_DeviceStatus += "||���";
	else
		Om.Main_DeviceStatus += "||����";
	// 37:UCM�ڵ�����˸� MIC
	if (m_pProfile->TRANS.DeviceExecOff & DEV_UCM)
		Om.Main_DeviceStatus += "||���";
	else
		Om.Main_DeviceStatus += "||����";

	// 38:PBM�ڵ�����˸� MIC
	if (m_pProfile->TRANS.DeviceExecOff & DEV_PBM)
		Om.Main_DeviceStatus += "||���";
	else
		Om.Main_DeviceStatus += "||����";

	// �������� ���� ��������������������������������������������
	// 36:Brm������������ī��Ʈ����(����,�ܷ���,�亸��,�ܷ���)
	if (Withdraw50TFlag)
	{
		if (RecycleBox50000Status == CST_EMPTY)
			Om.Main_DeviceStatus += "||�亸��";
		else
		if(RecycleBox50000Status == CST_NEAR)
			Om.Main_DeviceStatus += "||�ܷ���";
		else
		if (RecycleBox50000Status == CST_NORMAL)
			Om.Main_DeviceStatus += "||����";
		else
			Om.Main_DeviceStatus += "||�ܷ���";
	}
	else
	{
		Om.Main_DeviceStatus += "||������";
	}
	// ������������������������������������������������������������������������


	// 00:U8100 ���߰��� Pbm��ũ�����ܷ�				
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
	// 00:Ucm�����ũ�ܷ�								// UCM ��ǥ��ũ�� ���� ���θ�ǿ����� ���
	Om.Main_DeviceStatus += "||";

	// 00:Ucm�Ա���ũ�ܷ�								
	Om.Main_DeviceStatus += "||";
	

	Om.Main_DeviceStatus += "||";							// 39:A4������ ����
	Om.Main_DeviceStatus += "||";							// 40:A4������ ��� ����ī����	
	Om.Main_DeviceStatus += "||";							// 41:A4������ ����ڵ�
	Om.Main_DeviceStatus += "||";							// 42:A4������ �ܷ�����
	Om.Main_DeviceStatus += "||";							// 43:ī�޶�1	����
	Om.Main_DeviceStatus += "||";							// 44:ī�޶�2 ����
	Om.Main_DeviceStatus += "||";							// 45:BRM �ڵ�����˸�
	Om.Main_DeviceStatus += "||";							// 46:UCM �ڵ�����˸�
	Om.Main_DeviceStatus += "||";							// 47:PBM �ڵ�����˸�
	
/////////////////////////////////////////////////////////////////////////////
	// ��������(���κΰ�����)
	// OM_���κΰ�����
	Om.Main_ReffInfomation = "";
										
	// 0:�,�غ�
	if (OpenKey)
		Om.Main_ReffInfomation = "�";
	else
		Om.Main_ReffInfomation = "�غ�";
		
	// 1:�ۼ��Ż���(ON,OFF)
	Om.Main_ReffInfomation += "||" + Om.SendRecvStatus;

	// 2:���۰��ɻ���(ON,OFF)
	Om.Main_ReffInfomation += "||" + Om.OperationStatus;

	// 3:��ǥ����(10,50,100)
	Om.Main_ReffInfomation += "||" + Om.CheckKind;

/////////////////////////////////////////////////////////////////////////////
	// ��ݼ�ǥ���	
	// OM_��ݼ�ǥ��� 
	Om.Sub_WithCheckReg = "";

	// 0:��������1
	strTemp.Format("%2.2s", fnAPL_GetOutCheckBankNo().GetBuffer(0));
	Om.Sub_WithCheckReg += strTemp;
	
	// 1:��ݼ�ǥ��ȣ1
	strTemp.Format("%8.8s", fnAPL_GetNumberOfOutCheckNo().GetBuffer(0));
	Om.Sub_WithCheckReg += "||" + strTemp;
										
	// 2:��ݼ�ǥ�ż�1
	strTemp.Format("%d", fnAPL_GetNumberOfOutCheck());
	Om.Sub_WithCheckReg += "||" + strTemp;
	
	// 3:��������2
	strTemp.Format("%2.2s", fnAPL_GetOutCheckBankNo2().GetBuffer(0));
	Om.Sub_WithCheckReg += "||" + strTemp;
	
	// 4:��ݼ�ǥ��ȣ2
	strTemp.Format("%8.8s", fnAPL_GetNumberOfOutCheckNo2().GetBuffer(0));
	Om.Sub_WithCheckReg += "||" + strTemp;
										
	// 5:��ݼ�ǥ�ż�2
	strTemp.Format("%d", fnAPL_GetNumberOfOutCheck2());
	Om.Sub_WithCheckReg += "||" + strTemp;

	
/////////////////////////////////////////////////////////////////////////////
	// ��ġ����
	// OM_��ġ����
	Om.Sub_InstInformation = "";		
										
	// 0:��ġ��¥(YYYYMMDD)
	strTemp.Format("%8.8s", m_pProfile->NETWORK.MachineMakeDate);
	Om.Sub_InstInformation += strTemp;
	
	// 1:����(NNNNNNNN)
	strTemp.Format("%8.8s", m_pProfile->NETWORK.BranchNum);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 2:���(NNNNNNNN)
	strTemp.Format("%6.6s%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 3:��ġ���(NNNNNNNN)
	strTemp.Format("%8.8s", m_pProfile->NETWORK.RoomNum);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 4:����(ON,OFF)
	if (m_pProfile->NETWORK.BranchInOut)
		Om.Sub_InstInformation += "||ON";
	else
		Om.Sub_InstInformation += "||OFF";

	// 5:����(NNNNNNNN)
	strTemp.Format("%8.8s", m_pProfile->NETWORK.SubBranchNum);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 6:��������(ON,OFF)
	if (m_pProfile->NETWORK.AutoBranchOnOff)
		Om.Sub_InstInformation += "||ON";
	else
		Om.Sub_InstInformation += "||OFF";
	
	// 7:24�ð��(ON,OFF)
	if (m_pProfile->NETWORK.All24OnOff)
		Om.Sub_InstInformation += "||ON";
	else
		Om.Sub_InstInformation += "||OFF";
	
	// 8:365�(ON,OFF)
	if (m_pProfile->NETWORK.All365OnOff)
		Om.Sub_InstInformation += "||ON";
	else
		Om.Sub_InstInformation += "||OFF";
	
	// 9:CE�繫���ڵ�(NNNN)
	strTemp.Format("%4.4s", m_pProfile->NETWORK.CeOfficeCode);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 10:��������ȣ(NNNNNNNNNNNN)
	strTemp.Format("%12.12s", m_pProfile->NETWORK.MachineMakeNum);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 11:�����(NNNNNNNN)
	strTemp.Format("%8.8s", m_pProfile->NETWORK.OutBranchNum);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 12:��ġ��(NNNNNNNNNN)
	strTemp.Format("%10.10s", m_pProfile->NETWORK.InstName);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 13:��ġȮ����(NNNNNNNNNN)
	strTemp.Format("%10.10s", m_pProfile->NETWORK.InstConfirmName);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 14:���������(NNNNNNNNNN)
	strTemp.Format("%10.10s", m_pProfile->NETWORK.CeName);
	Om.Sub_InstInformation += "||" + strTemp;
	
	// 15:��������ڿ���ó(NNNNNNNNNNNNNNNN)
	strTemp.Format("%16.16s", m_pProfile->NETWORK.CeTel);
	Om.Sub_InstInformation += "||" + strTemp;

	Om.Sub_InstInformation += "||";								// 16:�ܸ�����
	Om.Sub_InstInformation += "||";								// 17:�뿪��
	Om.Sub_InstInformation += "||";								// 18:�������
	Om.Sub_InstInformation += "||";								// 19:�𵨸�
	Om.Sub_InstInformation += "||";								// 20:��ġȮ����
	Om.Sub_InstInformation += "||";								// 21:�����Ϸù�ȣ
	Om.Sub_InstInformation += "||";								// 22:�������

/////////////////////////////////////////////////////////////////////////////
	// �ΰ���ġ����
	// OM_�ΰ���ġ����
	Om.Sub_SubDeviceReg = "";			
										
	// 0:����ȭ��(����,�Ұ�)
	if (m_pProfile->DEVICE.ICCardDevice != NOT_SET)
		Om.Sub_SubDeviceReg += "����";
	else
		Om.Sub_SubDeviceReg += "�Ұ�";

	// 1:�ؿܰŷ�(����,�Ұ�) U8100 ���߰���					
	if (m_pDevCmn->DesTranStatus != TRUE)
		Om.Sub_SubDeviceReg += "||�Ұ�";
	else
	if (m_pProfile->DEVICE.DesHandler == D_DES_SET)
		Om.Sub_SubDeviceReg += "||����S";
	else
	if (m_pProfile->DEVICE.DesHandler == T_DES_SET)
		Om.Sub_SubDeviceReg += "||����T";
	else
		Om.Sub_SubDeviceReg += "||�Ұ�";

	// 2:�����ŷ�(����,�Ұ�)								
	Om.Sub_SubDeviceReg += "||";
	
	// 3:���ܼ��ŷ�(����,�Ұ�)
	if ( (m_pProfile->DEVICE.IrdaHandler != NOT_SET)	&&
		 (m_pProfile->DEVICE.RfdHandler == NOT_SET)	)
		Om.Sub_SubDeviceReg += "||����";
	else
		Om.Sub_SubDeviceReg += "||�Ұ�";

	// 4:���ڵ�ŷ�(����,�Ұ�)								
	if (m_pProfile->DEVICE.BarHandler != NOT_SET)
		Om.Sub_SubDeviceReg += "||����";
	else
		Om.Sub_SubDeviceReg += "||�Ұ�";
	
	// 5:��ȣȭ�ŷ�(����,�Ұ�)
	if (m_pProfile->DEVICE.SeedHandler != NOT_SET)
		Om.Sub_SubDeviceReg += "||����";
	else
		Om.Sub_SubDeviceReg += "||�Ұ�";

	// 6:Vfd(����,�Ұ�)
	if (m_pProfile->DEVICE.VfdHandler != NOT_SET)
		Om.Sub_SubDeviceReg += "||����";
	else
		Om.Sub_SubDeviceReg += "||�Ұ�";

	// 7:RF�ŷ�(����,�Ұ�) U8100 ���߰���
	if (m_pDevCmn->RfdTestStatus != TRUE)
		Om.Sub_SubDeviceReg += "||�Ұ�";
	else
	if (m_pProfile->DEVICE.RfdHandler != NOT_SET)
		Om.Sub_SubDeviceReg += "||����";
	else
		Om.Sub_SubDeviceReg += "||�Ұ�";

	// 8:���� Auto Turn										
	if(m_pProfile->DEVICE.PBHandler == PBM_NAT_SET) 
		Om.Sub_SubDeviceReg += "||�Ұ�";
	else
		Om.Sub_SubDeviceReg += "||����";

	Om.Sub_SubDeviceReg += "||";								// 9:A4���弱��

	// 10:����IC(����,�Ұ�)									
	if (m_pProfile->DEVICE.ICCardDevice != NOT_SET)
		Om.Sub_SubDeviceReg += "||����";
	else
		Om.Sub_SubDeviceReg += "||�Ұ�";

	Om.Sub_SubDeviceReg += "||";								// 11:�����ð����ݰŷ�
	Om.Sub_SubDeviceReg += "||";								// 12:TTL��뿩��
	Om.Sub_SubDeviceReg += "||";								// 13:MyBi

	// 14:��ֿ�												
	if (m_pProfile->DEVICE.EarHandler != NOT_SET)			
		Om.Sub_SubDeviceReg += "||����";
	else
		Om.Sub_SubDeviceReg += "||�Ұ�";

	// 15:������(����,�Ұ�)										// ���θ�ǿ����� ����Ͽ����� ������ ��翡 ���߾� ó��
// 	if (m_pProfile->DEVICE.FreeFormHandler != NOT_SET)
// 		Om.Sub_SubDeviceReg += "||����";
// 	else
		Om.Sub_SubDeviceReg += "||�Ұ�";	

	Om.Sub_SubDeviceReg += "||";								// 16:AJMS�ŷ�
	Om.Sub_SubDeviceReg += "||";								// 17:����ī��ŷ�

	
	Om.Sub_SubDeviceReg += "||";								// 18:���ΰŷ�(����, �Ұ�)				// V07-01-12-#05
	Om.Sub_SubDeviceReg += "||";								// 19:���弱��(IPBM, LPBM)				// V07-01-12-#05

	// 20:���÷���/ȭ��Ȯ��(����,�Ұ�)						
	if(m_pProfile->TRANS.ZoomFlag != NOT_SET)
		Om.Sub_SubDeviceReg += "||����";
	else
		Om.Sub_SubDeviceReg += "||�Ұ�";

	// 21:��������(����,�Ұ�)
	if (m_pProfile->DEVICE.Proximity != NOT_SET)			
		Om.Sub_SubDeviceReg += "||����";
	else
		Om.Sub_SubDeviceReg += "||�Ұ�";

	
	Om.Sub_SubDeviceReg += "||";								// 22:���뱸�����Կ�				

/////////////////////////////////////////////////////////////////////////////
	// Lan��������
	// OM_LAN��������
	Om.Sub_LanInformation = "";			
										
	// 0:IP�ּ�(NNN.NNN.NNN.NNN)
	strTemp.Format("%15.15s", m_pProfile->NETWORK.AtmIPAddress);
	Om.Sub_LanInformation += strTemp;
	
	// 1:����ݸ���ũ(NNN.NNN.NNN.NNN)
	strTemp.Format("%15.15s", m_pProfile->NETWORK.AtmSubnetMask);
	Om.Sub_LanInformation += "||" + strTemp;
	
	// 2:����Ʈ����(NNN.NNN.NNN.NNN)
	strTemp.Format("%15.15s", m_pProfile->NETWORK.AtmGateWay);
	Om.Sub_LanInformation += "||" + strTemp;
	
	// 3:BP1IP�ּ�(NNN.NNN.NNN.NNN)
	strTemp.Format("%15.15s", m_pProfile->NETWORK.BpIpAddress);
	Om.Sub_LanInformation += "||" + strTemp;
	
	// 4:BP1PORT(NNNNN)
	strTemp.Format("%5.5s", m_pProfile->NETWORK.BpPortNo);
	Om.Sub_LanInformation += "||" + strTemp;
	
	// 5:BP2IP�ּ�(NNN.NNN.NNN.NNN)
	strTemp.Format("%15.15s", m_pProfile->NETWORK.BpIpAddress2);
	Om.Sub_LanInformation += "||" + strTemp;
	
	// 6:BP2PORT(NNNNN)
	strTemp.Format("%5.5s", m_pProfile->NETWORK.BpPortNo2);
	Om.Sub_LanInformation += "||" + strTemp;
	
	// 7:BP3IP�ּ�(NNN.NNN.NNN.NNN)
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
	Om.Sub_LanInformation += "||";								// 15:�ý��������

/////////////////////////////////////////////////////////////////////////////
	// ȸ������
	// OM_ȸ������
	Om.Sub_LineInformation = "";				
										
	// 0:�������(IOC,HDLC,SWP,SDLC,TCP/IP,CCL,BAS)
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
	
	// 3:�ۼ��Ż�����
	strTemp.Format("%d", m_pProfile->NETWORK.SendRecvSize);
	Om.Sub_LineInformation += "||" + strTemp;
	
	// 4:ȸ���ӵ�(1200,2400,4800,9600,19200)
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
	// ATMS��������
	// OM_ATMS��������
	Om.Sub_ATMSServerInformation = "";			
										
	// 0:SERVER IP�ּ�(NNN.NNN.NNN.NNN)
//	strTemp.Format("%15.15s", IniGetStr(_ATMS_FTP_INI, "SERVER_INFO", "IP"));
	strTemp.Format("%s", IniGetStr(_ATMS_FTP_INI, "TCP_IP", "IP", "211.196.190.170"));
	Om.Sub_ATMSServerInformation += strTemp;
	
	// 1:SERVER PORT(NNNNN)
//	strTemp.Format("%5.5s", IniGetStr(_ATMS_FTP_INI, "SERVER_INFO", "Port"));
	strTemp.Format("%s", IniGetStr(_ATMS_FTP_INI, "TCP_IP", "Port", "2005"));
	Om.Sub_ATMSServerInformation += "||" + strTemp;

	// 2:����SERVER1 IP�ּ�(NNN.NNN.NNN.NNN)
//	strTemp.Format("%15.15s", IniGetStr(_ATMS_FTP_INI, "SERVER_INFO", "IP_CNTONE"));
	strTemp.Format("%s", IniGetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTONE", "211.196.190.170"));
	Om.Sub_ATMSServerInformation += "||" + strTemp;
	
	// 3:����SERVER2 IP�ּ�(NNN.NNN.NNN.NNN)
//	strTemp.Format("%15.15s", IniGetStr(_ATMS_FTP_INI, "SERVER_INFO", "IP_CNTTWO"));
	strTemp.Format("%s", IniGetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTTWO", "211.196.190.170"));
	Om.Sub_ATMSServerInformation += "||" + strTemp;

	// 4:����SERVER3 IP�ּ�(NNN.NNN.NNN.NNN)
//	strTemp.Format("%15.15s", IniGetStr(_ATMS_FTP_INI, "SERVER_INFO", "IP_CNTTHREE"));
	strTemp.Format("%s", IniGetStr(_ATMS_FTP_INI, "TCP_IP", "IP_CNTTHREE", "211.196.190.170"));
	Om.Sub_ATMSServerInformation += "||" + strTemp;

/////////////////////////////////////////////////////////////////////////////
	// �ŷ���������
	// OM_�ŷ���������
	Om.Sub_TranInformationReg = "";		
										
	// 0:�������(����,�Ұ�)
	if (m_pProfile->TRANS.ClerkTransOnOff & (TRAN_WITH_CASH | TRAN_WITH_50TCASH))	
		Om.Sub_TranInformationReg += "����";
	else
		Om.Sub_TranInformationReg += "�Ұ�";

	// 1:�����Ա�(����,�Ұ�)
	if (m_pProfile->TRANS.ClerkTransOnOff & TRAN_DEP_CASH)
		Om.Sub_TranInformationReg += "||����";
	else
		Om.Sub_TranInformationReg += "||�Ұ�";
	
	// 2:��ǥ���(����,�Ұ�)
	if (m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_CHECK)
		Om.Sub_TranInformationReg += "||����";
	else
		Om.Sub_TranInformationReg += "||�Ұ�";
	
	// 3:��ǥ�Ա�(����,�Ұ�)
	if (m_pProfile->TRANS.ClerkTransOnOff & TRAN_DEP_CHECK)
		Om.Sub_TranInformationReg += "||����";
	else
		Om.Sub_TranInformationReg += "||�Ұ�";
	
	// 4:����ŷ�(����,�Ұ�)
	if (m_pProfile->TRANS.ClerkTransOnOff & TRAN_PBM_OK)
		Om.Sub_TranInformationReg += "||����";
	else
		Om.Sub_TranInformationReg += "||�Ұ�";


	// 5:5������(����,�Ұ�), 6:BC�α�ü(����,�Ұ�)
	if (ChangeBCFlag)
	{
		if (m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_50TCASH)
		{
			Om.Sub_TranInformationReg += "||����";			
			Om.Sub_TranInformationReg += "||����";			
		}
		else												
		{
			Om.Sub_TranInformationReg += "||�Ұ�";			
			Om.Sub_TranInformationReg += "||����";				
		}
	}
	else
	{
		m_pProfile->TRANS.ClerkTransOnOff &= ~TRAN_WITH_50TCASH;
		Om.Sub_TranInformationReg += "||�Ұ�";					
		Om.Sub_TranInformationReg += "||�Ұ�";				
	}
	
	Om.Sub_TranInformationReg += "||";											
	
	// 8: ��ֿ�	 �����ATM
	if (m_pProfile->DEVICE.EarHandler == EAR_SET)
		Om.Sub_TranInformationReg += "||����";
	else
		Om.Sub_TranInformationReg += "||�Ұ�";

	// 9: ��ֿ� ���÷�	 	 �����ATM								
	if (m_pProfile->TRANS.ZoomFlag == ZOOM_SET)
		Om.Sub_TranInformationReg += "||����";
	else
		Om.Sub_TranInformationReg += "||�Ұ�";
	Om.Sub_TranInformationReg += "||";						

	// 10:����PGU ��/����
	Om.Sub_TranInformationReg += "||����";

	// 11:�������� �����ATM
	if (m_pProfile->DEVICE.Proximity != NOT_SET)				
		Om.Sub_SubDeviceReg += "||����";
	else
		Om.Sub_SubDeviceReg += "||�Ұ�";

/////////////////////////////////////////////////////////////////////////////
	// ���ݺ���ȸ�� :
	// OM_���ݺ���ȸ��
	Om.Sub_CashRefillTakeup = "";		
										
	// 0:ī��Ʈ1�����
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt"));
	Om.Sub_CashRefillTakeup += strTemp;

	// 1:ī��Ʈ2�����
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB2NoteCnt"));
	Om.Sub_CashRefillTakeup += "||" + strTemp;

	// 2:ī��Ʈ3�����
	strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB3NoteCnt"));
	Om.Sub_CashRefillTakeup += "||" + strTemp;

	if (HBRM_SET == AtmDefine.CashHandler)
	{
		// 3:ī��Ʈ4�����
		strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "RB4NoteCnt"));
		Om.Sub_CashRefillTakeup += "||" + strTemp;

		// 4:���ī��ƮRJB : for OKI BRM
		strTemp.Format("%d", RegGetInt(_REGKEY_BRMSTATUS, "ClerkRJBNoteCnt"));
		Om.Sub_CashRefillTakeup += "||" + strTemp;
	}

	// 5:Brm����(����,���)
	Om.Sub_CashRefillTakeup += "||" + Om.BrmDeviceStatus;

	Om.Sub_DepKindReg.Empty();

	if ((m_pProfile->BRM.DepositCashKind & 0x01) == 0x01)	// ��������
		Om.Sub_DepKindReg += "����";
	else
		Om.Sub_DepKindReg += "�Ұ�";

	if ((m_pProfile->BRM.DepositCashKind & 0x02) == 0x02)	// ����õ����
		Om.Sub_DepKindReg += "||����";
	else
		Om.Sub_DepKindReg += "||�Ұ�";

	if ((m_pProfile->BRM.DepositCashKind & 0x04) == 0x04)	// ��õ����
		Om.Sub_DepKindReg += "||����";
	else
		Om.Sub_DepKindReg += "||�Ұ�";

	if ((m_pProfile->BRM.DepositCashKind & 0x10) == 0x10)	// �Ÿ�����
		Om.Sub_DepKindReg += "||����";
	else
		Om.Sub_DepKindReg += "||�Ұ�";

	if ((m_pProfile->BRM.DepositCashKind & 0x20) == 0x20)	// �ſ�õ����
		Om.Sub_DepKindReg += "||����";
	else
		Om.Sub_DepKindReg += "||�Ұ�";

	if ((m_pProfile->BRM.DepositCashKind & 0x40) == 0x40)	// ��õ����
		Om.Sub_DepKindReg += "||����";
	else
		Om.Sub_DepKindReg += "||�Ұ�";
	
	if (HBRM_SET == m_pProfile->DEVICE.CashHandler)			// ������
	{
		if (ChangeBCFlag)
		{
			if ((m_pProfile->BRM.DepositCashKind & 0x08) == 0x08)
				Om.Sub_DepKindReg += "||����";
			else
				Om.Sub_DepKindReg += "||�Ұ�";

			Om.Sub_DepKindReg += "||����";
		}
		else
		{
			Om.Sub_DepKindReg += "||�Ұ�";
			Om.Sub_DepKindReg += "||�Ұ�";
		}

		if (ChangeBCFlag)										// 7:BC�� ��ü����(BRM_SET�� ���� �޶�� ���� �ʳ�???) 
			Om.Sub_DepKindReg += "||����";
		else
			Om.Sub_DepKindReg += "||�Ұ�";
	}
	else
		Om.Sub_DepKindReg += "||�Ұ�";

/////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
	int nType = 0;
	Om.CasetteType.Empty();

	if (HBRM_SET == AtmDefine.CashHandler)
	{
		nType = fnBRM_GetNoteKindOfRB1();
		Om.CasetteType += (nType == BRM_NOTEKIND_10000_OLD) ? "����" : 
						  (nType == BRM_NOTEKIND_5000_OLD)  ? "����õ" : 
						  (nType == BRM_NOTEKIND_1000_OLD)  ? "��õ" : 
						  (nType == BRM_NOTEKIND_10000_NEW) ? "�Ÿ�" : 
						  (nType == BRM_NOTEKIND_5000_NEW)  ? "�ſ�õ" : 
						  (nType == BRM_NOTEKIND_1000_NEW)  ? "��õ" :
						  (nType == BRM_NOTEKIND_DEP_OLD)   ? "�Աݱ�" :		
						  (nType == BRM_NOTEKIND_DEP_NEW)   ? "�Աݽ�" :	
						  (nType == BRM_NOTEKIND_50000)     ? "����" : "";		

		nType = fnBRM_GetNoteKindOfRB2();
		Om.CasetteType += (nType == BRM_NOTEKIND_10000_OLD) ? "||����" : 
						  (nType == BRM_NOTEKIND_5000_OLD)  ? "||����õ" : 
						  (nType == BRM_NOTEKIND_1000_OLD)  ? "||��õ" : 
						  (nType == BRM_NOTEKIND_10000_NEW) ? "||�Ÿ�" : 
						  (nType == BRM_NOTEKIND_5000_NEW)  ? "||�ſ�õ" : 
						  (nType == BRM_NOTEKIND_1000_NEW)  ? "||��õ" :
						  (nType == BRM_NOTEKIND_DEP_OLD)   ? "||�Աݱ�" :		
						  (nType == BRM_NOTEKIND_DEP_NEW)   ? "||�Աݽ�" :		
						  (nType == BRM_NOTEKIND_50000)		? "||����" : "";	

		nType = fnBRM_GetNoteKindOfRB3();
		Om.CasetteType += (nType == BRM_NOTEKIND_10000_OLD) ? "||����" : 
						  (nType == BRM_NOTEKIND_5000_OLD)  ? "||����õ" : 
						  (nType == BRM_NOTEKIND_1000_OLD)  ? "||��õ" : 
						  (nType == BRM_NOTEKIND_10000_NEW) ? "||�Ÿ�" : 
						  (nType == BRM_NOTEKIND_5000_NEW)  ? "||�ſ�õ" : 
						  (nType == BRM_NOTEKIND_1000_NEW)  ? "||��õ" :
						  (nType == BRM_NOTEKIND_DEP_OLD)   ? "||�Աݱ�" :		
						  (nType == BRM_NOTEKIND_DEP_NEW)   ? "||�Աݽ�" :		
						  (nType == BRM_NOTEKIND_50000)		? "||����" : "";	

		nType = fnBRM_GetNoteKindOfRB4();
		Om.CasetteType += (nType == BRM_NOTEKIND_10000_OLD) ? "||����" : 
						  (nType == BRM_NOTEKIND_5000_OLD)  ? "||����õ" : 
						  (nType == BRM_NOTEKIND_1000_OLD)  ? "||��õ" : 
						  (nType == BRM_NOTEKIND_10000_NEW) ? "||�Ÿ�" : 
						  (nType == BRM_NOTEKIND_5000_NEW)  ? "||�ſ�õ" : 
						  (nType == BRM_NOTEKIND_1000_NEW)  ? "||��õ" :
						  (nType == BRM_NOTEKIND_DEP_OLD)   ? "||�Աݱ�" :		
						  (nType == BRM_NOTEKIND_DEP_NEW)   ? "||�Աݽ�" :		
						  (nType == BRM_NOTEKIND_50000)		? "||����" : "";	
		
	}
	else
	{
		if (ChangeBCFlag)										
			Om.CasetteType += "||����";
		else
			Om.CasetteType += "||�Ұ�";
	}
// nkh end
/////////////////////////////////////////////////////////////////////////////
	Om.Sub_AutoComputeNote = "";
	// 0:�������
	if (m_pProfile->BRM.PriorityNote == CASH_PRIORITY_1000TH)
		Om.Sub_AutoComputeNote += "10||";
	else
		Om.Sub_AutoComputeNote += "5||";

	if (Withdraw50TFlag)										
		Om.Sub_AutoComputeNote += "����";
	else
		Om.Sub_AutoComputeNote += "�Ұ�";

/////////////////////////////////////////////////////////////////////////////
// T3ATM AP ���泻�� #19 - ī�޶� 3���� ���¸� ��� ǥ���ϱ� ����
	Om.Sub_CamerStatus = "";
	
	if (fnCMR_GetFaceCameraDeviceStatus() == NORMAL)
		Om.Sub_CamerStatus += "����||";
	else
		Om.Sub_CamerStatus += "����||";
	
	if (fnCMR_GetHandCameraDeviceStatus() == NORMAL)
		Om.Sub_CamerStatus += "����||";
	else
		Om.Sub_CamerStatus += "����||";
	
	if (fnCMR_GetRoomCameraDeviceStatus() == NORMAL)
		Om.Sub_CamerStatus += "����";
	else
		Om.Sub_CamerStatus += "����";
	
	
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
	if ((nInitFlag) ||											// ��������(�������1)	
		(Om.Main_1LineStatus			!= OmSave.Main_1LineStatus))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_1LineStatus(%s)", Om.Main_1LineStatus);
		OmSave.Main_1LineStatus			=	Om.Main_1LineStatus;
		SetShareData("OM_�������1",		Om.Main_1LineStatus);
	}

	if ((nInitFlag) ||											// ��������(�������2)
		(Om.Main_2LineStatus			!= OmSave.Main_2LineStatus))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_2LineStatus(%s)", Om.Main_2LineStatus);
		OmSave.Main_2LineStatus			=	Om.Main_2LineStatus;
		SetShareData("OM_�������2",		Om.Main_2LineStatus);
	}

	if ((nInitFlag) ||											// ��������(�������ݺ�����Ȳ)
		(Om.Main_CashStatus				!= OmSave.Main_CashStatus))			
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_CashStatus(%s)", Om.Main_CashStatus);
		OmSave.Main_CashStatus			=	Om.Main_CashStatus;
		SetShareData("OM_�������ݺ�����Ȳ",	Om.Main_CashStatus);
		
 		strTemp = Om.Main_CashStatus;
 		strTemp.Replace("||", "|");
	}

	if ((nInitFlag) ||											// ��������(�����ܷ�����)
		(Om.Main_Status					!= OmSave.Main_Status))				
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_Status(%s)", Om.Main_Status);
		OmSave.Main_Status				=	Om.Main_Status;
		SetShareData("OM_�����ܷ�����",		Om.Main_Status);
	}

	if ((nInitFlag) ||											// ��������(���μ�ǥ������Ȳ)
		(Om.Main_CheckStatus			!= OmSave.Main_CheckStatus))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_CheckStatus(%s)", Om.Main_CheckStatus);
		OmSave.Main_CheckStatus			=	Om.Main_CheckStatus;
		SetShareData("OM_���μ�ǥ������Ȳ",	Om.Main_CheckStatus);
	}

	if ((nInitFlag) ||											// ��������(��������ȸ����Ȳ)
		(Om.Main_CashRejectStatus		!= OmSave.Main_CashRejectStatus))	
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_CashRejectStatus(%s)", Om.Main_CashRejectStatus);
		OmSave.Main_CashRejectStatus	=	Om.Main_CashRejectStatus;
		SetShareData("OM_��������ȸ����Ȳ",	Om.Main_CashRejectStatus);
	}

	if ((nInitFlag) ||											// ��������(������ġ��Ȳ)
		(Om.Main_DeviceStatus			!= OmSave.Main_DeviceStatus))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_DeviceStatus(%s)", Om.Main_DeviceStatus);
		OmSave.Main_DeviceStatus		=	Om.Main_DeviceStatus;
		SetShareData("OM_������ġ��Ȳ",		Om.Main_DeviceStatus);
	}

	if ((nInitFlag) ||											// ��������(���κΰ�����)
		(Om.Main_ReffInfomation			!= OmSave.Main_ReffInfomation))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Main_ReffInfomation(%s)", Om.Main_ReffInfomation);
		OmSave.Main_ReffInfomation		=	Om.Main_ReffInfomation;
		SetShareData("OM_���κΰ�����",		Om.Main_ReffInfomation);
	}

	if ((nInitFlag) ||											// ��ݼ�ǥ���
		(Om.Sub_WithCheckReg			!= OmSave.Sub_WithCheckReg))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_WithCheckReg(%s)", Om.Sub_WithCheckReg);
		OmSave.Sub_WithCheckReg			=	Om.Sub_WithCheckReg;
		SetShareData("OM_��ݼ�ǥ���",		Om.Sub_WithCheckReg);
	}

	if ((nInitFlag) ||											// ��ġ����
		(Om.Sub_InstInformation			!= OmSave.Sub_InstInformation))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_InstInformation(%s)", Om.Sub_InstInformation);
		OmSave.Sub_InstInformation		=	Om.Sub_InstInformation;
		SetShareData("OM_��ġ����",			Om.Sub_InstInformation);
	}

	if ((nInitFlag) ||											// �ΰ���ġ����
		(Om.Sub_SubDeviceReg			!= OmSave.Sub_SubDeviceReg))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_SubDeviceReg(%s)", Om.Sub_SubDeviceReg);
		OmSave.Sub_SubDeviceReg			=	Om.Sub_SubDeviceReg;
		SetShareData("OM_�ΰ���ġ����",		Om.Sub_SubDeviceReg);
	}

	if ((nInitFlag) ||											// Lan��������
		(Om.Sub_LanInformation			!= OmSave.Sub_LanInformation))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_LanInformation(%s)", Om.Sub_LanInformation);
		OmSave.Sub_LanInformation		=	Om.Sub_LanInformation;
		SetShareData("OM_LAN��������",		Om.Sub_LanInformation);
	}

	if ((nInitFlag) ||											// ȸ������
		(Om.Sub_LineInformation			!= OmSave.Sub_LineInformation))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_LineInformation(%s)", Om.Sub_LineInformation);
		OmSave.Sub_LineInformation		=	Om.Sub_LineInformation;
		SetShareData("OM_ȸ������",			Om.Sub_LineInformation);
	}

	if ((nInitFlag) ||											// �ŷ���������
		(Om.Sub_TranInformationReg		!= OmSave.Sub_TranInformationReg))	
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_TranInformationReg(%s)", Om.Sub_TranInformationReg);
		OmSave.Sub_TranInformationReg	=	Om.Sub_TranInformationReg;
		SetShareData("OM_�ŷ���������",		Om.Sub_TranInformationReg);
	}

	if ((nInitFlag) ||											// ���ݺ���ȸ��
		(Om.Sub_CashRefillTakeup		!= OmSave.Sub_CashRefillTakeup))	
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_CashRefillTakeup(%s)", Om.Sub_CashRefillTakeup);
		OmSave.Sub_CashRefillTakeup		=	Om.Sub_CashRefillTakeup;
		SetShareData("OM_���ݺ���ȸ��",		Om.Sub_CashRefillTakeup);
	}

	if ((nInitFlag) ||											// V200: �Աݱ������� 
		(Om.Sub_DepKindReg		!= OmSave.Sub_DepKindReg))	
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_DepKindReg(%s)", Om.Sub_DepKindReg);
		OmSave.Sub_DepKindReg		=	Om.Sub_DepKindReg;
		SetShareData("OM_�Աݱ�������",		Om.Sub_DepKindReg);
	}

	if ((nInitFlag) ||											// ī��Ʈ�������� 
		(Om.CasetteType		!= OmSave.CasetteType))	
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.CasetteType(%s)", Om.CasetteType);
		OmSave.CasetteType			=		Om.CasetteType;
		SetShareData("OM_ī��Ʈ��������",	Om.CasetteType);
	}

	if ((nInitFlag) ||											// ��⼳��
		(Om.Sub_AutoComputeNote				!= OmSave.Sub_AutoComputeNote))	
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:Om.Sub_AutoComputeNote[%s]", Om.Sub_AutoComputeNote);
		OmSave.Sub_AutoComputeNote			=	Om.Sub_AutoComputeNote;
		SetShareData("OM_�ڵ���꼳��",			Om.Sub_AutoComputeNote);
	}

// T3ATM AP ���泻�� #19 - ī�޶� 3���� ���¸� ��� ǥ���ϱ� ����
	if ((nInitFlag) ||											// 3ī�޶� ����
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

// �������˻�
// V03-01-01 �ʱ����ɼ� Ȯ��
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
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkExecCheck:C:\\T1ATM\\SUPERVISOR\\REARAP.EXE �����!!!");
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
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkExecCheck:C:\\T1ATM\\APP\\MAIN\\OMDEMON.EXE �����!!!");
				WinExec(RegGetStr(_REGKEY_LOADER, "OmDemon", _DEFAULT_OMDEMON), SW_SHOW);
		//t		WinExec("C:\\T1ATM\\APP\\MAIN\\OMDEMON.EXE", SW_SHOW);
			}
		default:
			break;
	}

	return TRUE;
}

// Segment���� ����� �Լ�
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
			fnVFD_RealTimeTextDisplay("���� ����Ͻ� �� �����ϴ�", 0, 7, 20, 25, 2, 0, "����");
		else
			fnVFD_RealTimeTextDisplay(strTempArray[0], 0, 7, 20, 25, 2, 0, "����");

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

// �������˻�
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

// Get Log Start ����������������������������������������������
// �α����Ϲޱ� : 2006.11.22
int CDevCmn::fnAPL_GetLogFileStart(int nMonth, int nDate)
{
	CString strCommand = "";
	CString strCurrentDir = "";
	CString strDrive = "";
	DWORD	dwFreeSize = 0;

	// Flash Memory Stick�� ã�´�.
	fnAPL_GetDriveInfo(strDrive, dwFreeSize, 'C');
	if (0 == dwFreeSize)
		return FALSE;

//////////////////////////////////////////////////////////////////////////
	CreateDirectory("D:\\TEMPTRACE", NULL);						// �ӽ���������

//////////////////////////////////////////////////////////////////////////
// �� REGISTRY ����
	strCommand.Format("c");
	WinExecWaitEnd(strCommand, SW_HIDE);

	strCommand.Format("REGEDIT /E /S D:\\TEMPTRACE\\WOSAREG.TXT HKEY_CLASSES_ROOT\\WOSA/XFS_ROOT");
	WinExecWaitEnd(strCommand, SW_HIDE);

//////////////////////////////////////////////////////////////////////////
// �� BRM LOG ����(OKI)
	if (OKI_SET == AtmDefine.CashHandler)
	{
		fnBRM_BrmGetLog();										// GET OKI BRM LOG
		strCommand.Format("XCOPY C:\\ATMDAT\\*.* D:\\TEMPTRACE\\ATMDAT%02d%02d\\ /S /Y", nMonth, nDate);
		WinExecWaitEnd(strCommand, SW_HIDE);
	}
	
//////////////////////////////////////////////////////////////////////////
// �� INI ����
	strCommand.Format("XCOPY D:\\INI\\*.* D:\\TEMPTRACE\\INI%02d%02d\\ /S /Y", nMonth, nDate);
	WinExecWaitEnd(strCommand, SW_HIDE);

//////////////////////////////////////////////////////////////////////////
// �� ��������(�ؽ�Ʈ)
	strCurrentDir.Format("D:\\ECASH\\JNL%02d%02d", nMonth, nDate);
	if (SetCurrentDirectory(strCurrentDir))
	{
		strCommand.Format("XCOPY *.* D:\\TEMPTRACE\\ELECJNL%02d%02d\\ /S /Y", nMonth, nDate);
		WinExecWaitEnd(strCommand, SW_HIDE);
	}

//////////////////////////////////////////////////////////////////////////
// �� UBIATOR LOG
	if (SetCurrentDirectory("c:\\Program Files\\Ubiator20\\Trace"))
	{
		strCommand.Format("XCOPY *.* D:\\TEMPTRACE\\UBIATOR%02d%02d\\ /S /Y", nMonth, nDate);
		WinExecWaitEnd(strCommand, SW_HIDE);
	}

//////////////////////////////////////////////////////////////////////////
// �� AP LOG
	if (SetCurrentDirectory(_TRACE_DIR))
	{
		strCommand.Format("XCOPY *%02d.* D:\\TEMPTRACE\\APLOG%02d%02d\\ /S /Y", nDate, nMonth, nDate);
		WinExecWaitEnd(strCommand, SW_HIDE);

		strCommand.Format("XCOPY *%02d%02d*.* D:\\TEMPTRACE\\APLOG%02d%02d\\ /S /Y", nMonth, nDate, nMonth, nDate);
		WinExecWaitEnd(strCommand, SW_HIDE);
	}
	
//////////////////////////////////////////////////////////////////////////
// �����ϱ�(��ü)
	SetCurrentDirectory("D:\\TEMPTRACE");
	strCommand.Format("C:\\PKZIP25 -ADD -DIR=CURRENT D:\\TEMPTRACE\\TRACE%02d%02d.ZIP", nMonth, nDate);
	WinExecWaitEnd(strCommand, SW_HIDE);

//////////////////////////////////////////////////////////////////////////
// �����ϱ�
	strCommand.Format("D:\\TEMPTRACE\\TRACE%02d%02d.ZIP", nMonth, nDate);
	strCurrentDir.Format("%sTRACE%3.3s%2.2s%02d%02d.ZIP", strDrive,
														  &m_pProfile->NETWORK.BranchNum[5],
														  &m_pProfile->NETWORK.AtmSerialNum[6],
														  nMonth, nDate);
	CopyFile(strCommand, strCurrentDir, FALSE);

//////////////////////////////////////////////////////////////////////////
// ���������
	SetCurrentDirectory("C:\\T1ATM");
	fnAPL_DeleteFiles("D:\\TEMPTRACE");
	RemoveDirectory("D:\\TEMPTRACE");
//////////////////////////////////////////////////////////////////////////
	
	return TRUE;
}

// Remote Ctrl Log Start ����������������������������������������������
// �α����Ϲޱ� : 2008.05.24
int CDevCmn::fnAPL_GetRemoteCtrlFileStart(CString strLocalPath, int nYear, int nMonth, int nDate, int nTran, int nSerialF)
{
	CString strCommand = "";
	CString strCurrentDir = "";
	CString strDrive = "";
	DWORD	dwFreeSize = 0;


//////////////////////////////////////////////////////////////////////////
	CreateDirectory("D:\\TEMPTRACE", NULL);						// �ӽ���������
//////////////////////////////////////////////////////////////////////////

	if(nTran == ATMS_CMD_JNLMODE)
	{
		//  1. ��������(�ؽ�Ʈ)
		strCurrentDir.Format("D:\\ECASH\\JNL%02d%02d", nMonth, nDate);
		if (SetCurrentDirectory(strCurrentDir))
		{
			strCommand.Format("XCOPY *.* D:\\TEMPTRACE\\ELECJNL%04d%02d%02d\\ /S /Y", nYear, nMonth, nDate);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetRemoteCtrlFileStart JNLUPLOAD[%s]", strCommand);

			WinExecWaitEnd(strCommand, SW_HIDE);
		}
		// �����ϱ�(��ü)
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
		// �����ϱ�(��ü)
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
		
		if (nType == DRIVE_REMOVABLE)							// �̵��� ����̺��϶���
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
			
			// ���� �뷮
			dwFreeSize = ( lpSectorsPerCluster * lpBytesPerSector * lpNumberOfFreeClusters);
			if ( 0 >= dwFreeSize ) continue;					// size�� 0���ϸ� �ٽ�
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
// Get Log End ����������������������������������������������

/*==============================================================================
* Function Name : fnAPD_NetworkInfoSet()
* Description   : ��Ʈ��ũ ����(IP, Subnet, Gateway)���� �ý��� IP�� �����Ѵ�.
* Parameters    : 
* Return        : T_OK
* version       : V02-04-00-#05
* ============================================================================*/
int CDevCmn::fnAPL_NetworkInfoSet(int nFixIp)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[Network����] fnAPD_NetworkInfoSet"); 
	char szIpAddress[17] = {0,};
	CBIPConfig	clsIP;

	m_pProfile->GetNetworkProfile();
	
	//����IP ���� 
	if(nFixIp)
	{
	
		// 01. Ip Address
		memset(szIpAddress, 0x00, sizeof(szIpAddress));
		Sprintf(szIpAddress, 16, "%d.%d.%d.%d", Asc2Int(m_pProfile->NETWORK.AtmIPAddress, 3),				
									  Asc2Int(&m_pProfile->NETWORK.AtmIPAddress[4], 3),
									  Asc2Int(&m_pProfile->NETWORK.AtmIPAddress[8], 3),
									  Asc2Int(&m_pProfile->NETWORK.AtmIPAddress[12], 3));
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[Network����] 01. Ip Address [%s]", szIpAddress); 

		clsIP.AtmSetIpAddress(szIpAddress);

		// 02. SubnetMask
		memset(szIpAddress, 0x00, sizeof(szIpAddress));
		Sprintf(szIpAddress, 16, "%d.%d.%d.%d", Asc2Int(m_pProfile->NETWORK.AtmSubnetMask, 3),				
									  Asc2Int(&m_pProfile->NETWORK.AtmSubnetMask[4], 3),
									  Asc2Int(&m_pProfile->NETWORK.AtmSubnetMask[8], 3),
									  Asc2Int(&m_pProfile->NETWORK.AtmSubnetMask[12], 3));

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[Network����] 02. SubnetMask [%s]", szIpAddress); 

		clsIP.AtmSetSubnetMask(szIpAddress);

		// 03. GateWay
		memset(szIpAddress, 0x00, sizeof(szIpAddress));
		Sprintf(szIpAddress, 16, "%d.%d.%d.%d", Asc2Int(m_pProfile->NETWORK.AtmGateWay, 3),						
									  Asc2Int(&m_pProfile->NETWORK.AtmGateWay[4], 3),
									  Asc2Int(&m_pProfile->NETWORK.AtmGateWay[8], 3),
									  Asc2Int(&m_pProfile->NETWORK.AtmGateWay[12], 3));

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[Network����] 03. GateWay [%s]", szIpAddress);
		
		clsIP.AtmSetGateWay(szIpAddress);
	}
	else
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[Network����] fnAPD_NetworkInfoSet Set Dhcp"); 

	}
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[Network����] fnAPD_NetworkInfoSet return"); 
	return T_OK;
}


/*==============================================================================
* Function Name : fnAPL_CheckMachineType()
* Description   : ������ CashHandler��
* Parameters    : 
* Return        : TRUE : ����, FALSE : ������
* version       : V05-06-03-#13
* ============================================================================*/
int CDevCmn::fnAPL_CheckMachineType()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CheckMachineType()::MachineType[%d], CashHandler[%d]", 
													m_pProfile->DEVICE.MachineType, AtmDefine.CashHandler); 

	int	nReturn = TRUE;
	switch (m_pProfile->DEVICE.MachineType)
	{
	case U8100:										// U8100-AP����
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
* Description   : ��ǻ���̸�����
* Parameters    : 
* Return        : T_OK
* version       : V07-01-01
* ============================================================================*/
int CDevCmn::fnAPL_SetComputerName()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetComputerName()"); 
	int	nReturn = T_OK;
	CString	strComputerName("");

	// ����(5) + ���(3) + ��ü(2:HS) + ����(Max:5, ��ü��������)
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

// ī��Ʈ�� ���ݺ�������
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
		(AtmDefine.CashHandler == HBRM_SET)	)					// U8100-AP����
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

	static	int		StatusInCnt = 0;							// ������Ƚ������
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

				csrGetValue =  (nRB1NoteKindUp == BRM_NOTEKIND_10000_OLD) ? "����" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_5000_OLD)  ? "����õ" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_1000_OLD)  ? "��õ" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_10000_NEW) ? "�Ÿ�" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_5000_NEW)  ? "�ſ�õ" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_1000_NEW)  ? "��õ" :
							   (nRB1NoteKindUp == BRM_NOTEKIND_DEP_OLD)   ? "�Աݱ�" :	
							   (nRB1NoteKindUp == BRM_NOTEKIND_DEP_NEW)   ? "�Աݽ�" :	
							   (nRB1NoteKindUp == BRM_NOTEKIND_50000)     ? "����" : "UNK";
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[1ī��Ʈ ���� ���� ����(���� <-> %s)]", csrGetValue));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo [1ī��Ʈ ���� ���� ����(�Ÿ� <-> %s)]", csrGetValue);

				csrGetValue =  (nRB2NoteKindUp == BRM_NOTEKIND_10000_OLD) ? "����" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_5000_OLD)  ? "����õ" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_1000_OLD)  ? "��õ" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_10000_NEW) ? "�Ÿ�" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_5000_NEW)  ? "�ſ�õ" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_1000_NEW)  ? "��õ" :
							   (nRB2NoteKindUp == BRM_NOTEKIND_DEP_OLD)   ? "�Աݱ�" :	
							   (nRB2NoteKindUp == BRM_NOTEKIND_DEP_NEW)   ? "�Աݽ�" :	
							   (nRB2NoteKindUp == BRM_NOTEKIND_50000)     ? "����" : "UNK";
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[2ī��Ʈ ���� ���� ����(�Ÿ� <-> %s)]", csrGetValue));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo [2ī��Ʈ ���� ���� ����(�Ÿ� <-> %s)]", csrGetValue);


				StatusInCnt = 10;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo RB Status MissMatch (m_pDevCmn->DepAvail[%d],m_pDevCmn->OpenKey[%d])", m_pDevCmn->WithAvail, m_pDevCmn->OpenKey);
			}
			else
			if ((nRB2NoteKindUp != nNoteKind2) && (nRB3NoteKindUp != nNoteKind3) && (StatusInCnt != 10))
			{
				csrGetValue =  (nRB2NoteKindUp == BRM_NOTEKIND_10000_OLD) ? "����" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_5000_OLD)  ? "����õ" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_1000_OLD)  ? "��õ" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_10000_NEW) ? "�Ÿ�" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_5000_NEW)  ? "�ſ�õ" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_1000_NEW)  ? "��õ" :
							   (nRB2NoteKindUp == BRM_NOTEKIND_DEP_OLD)   ? "�Աݱ�" :	
							   (nRB2NoteKindUp == BRM_NOTEKIND_DEP_NEW)   ? "�Աݽ�" :	
							   (nRB2NoteKindUp == BRM_NOTEKIND_50000)     ? "����" : "UNK";

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[2ī��Ʈ ���� ���� ����(�Ÿ� <-> %s)]", csrGetValue));

				csrGetValue =  (nRB3NoteKindUp == BRM_NOTEKIND_10000_OLD) ? "����" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_5000_OLD)  ? "����õ" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_1000_OLD)  ? "��õ" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_10000_NEW) ? "�Ÿ�" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_5000_NEW)  ? "�ſ�õ" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_1000_NEW)  ? "��õ" :
							   (nRB3NoteKindUp == BRM_NOTEKIND_DEP_OLD)   ? "�Աݱ�" :	
							   (nRB3NoteKindUp == BRM_NOTEKIND_DEP_NEW)   ? "�Աݽ�" :	
							   (nRB3NoteKindUp == BRM_NOTEKIND_50000)     ? "����" : "UNK";

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[3ī��Ʈ ���� ���� ����(�Ÿ� <-> %s)]", csrGetValue));

				StatusInCnt = 10;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo RB Status MissMatch (m_pDevCmn->DepAvail[%d],m_pDevCmn->OpenKey[%d])", m_pDevCmn->WithAvail, m_pDevCmn->OpenKey);
			}
			else
			if ((nRB1NoteKindUp != nNoteKind1) && (StatusInCnt != 10))
			{
				csrGetValue =  (nRB1NoteKindUp == BRM_NOTEKIND_10000_OLD) ? "����" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_5000_OLD)  ? "����õ" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_1000_OLD)  ? "��õ" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_10000_NEW) ? "�Ÿ�" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_5000_NEW)  ? "�ſ�õ" : 
							   (nRB1NoteKindUp == BRM_NOTEKIND_1000_NEW)  ? "��õ" :
							   (nRB1NoteKindUp == BRM_NOTEKIND_DEP_OLD)   ? "�Աݱ�" :	
							   (nRB1NoteKindUp == BRM_NOTEKIND_DEP_NEW)   ? "�Աݽ�" :	
							   (nRB1NoteKindUp == BRM_NOTEKIND_50000)     ? "����" : "UNK";

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[1ī��Ʈ ���� ���� ����(�Ÿ� <-> %s)]", csrGetValue));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo [1ī��Ʈ ���� ���� ����(�Ÿ� <-> %s)]", csrGetValue);

				StatusInCnt = 10;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo RB Status MissMatch (m_pDevCmn->DepAvail[%d],m_pDevCmn->OpenKey[%d])", m_pDevCmn->WithAvail, m_pDevCmn->OpenKey);
			}
			else
			if ((nRB2NoteKindUp != nNoteKind2) && (StatusInCnt != 10))
			{
				csrGetValue =  (nRB2NoteKindUp == BRM_NOTEKIND_10000_OLD) ? "����" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_5000_OLD)  ? "����õ" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_1000_OLD)  ? "��õ" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_10000_NEW) ? "�Ÿ�" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_5000_NEW)  ? "�ſ�õ" : 
							   (nRB2NoteKindUp == BRM_NOTEKIND_1000_NEW)  ? "��õ" :
							   (nRB2NoteKindUp == BRM_NOTEKIND_DEP_OLD)   ? "�Աݱ�" :	
							   (nRB2NoteKindUp == BRM_NOTEKIND_DEP_NEW)   ? "�Աݽ�" :	
							   (nRB2NoteKindUp == BRM_NOTEKIND_50000)     ? "����" : "UNK";

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[2ī��Ʈ ���� ���� ����(�Ÿ� <-> %s)]", csrGetValue));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo [2ī��Ʈ ���� ���� ����(�Ÿ� <-> %s)]", csrGetValue);

				StatusInCnt = 10;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo RB Status MissMatch (m_pDevCmn->DepAvail[%d],m_pDevCmn->OpenKey[%d])", m_pDevCmn->WithAvail, m_pDevCmn->OpenKey);
			}
			else
			if ((nRB3NoteKindUp != nNoteKind3) && (StatusInCnt != 10))
			{
				csrGetValue =  (nRB3NoteKindUp == BRM_NOTEKIND_10000_OLD) ? "����" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_5000_OLD)  ? "����õ" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_1000_OLD)  ? "��õ" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_10000_NEW) ? "�Ÿ�" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_5000_NEW)  ? "�ſ�õ" : 
							   (nRB3NoteKindUp == BRM_NOTEKIND_1000_NEW)  ? "��õ" :
							   (nRB3NoteKindUp == BRM_NOTEKIND_DEP_OLD)   ? "�Աݱ�" :	
							   (nRB3NoteKindUp == BRM_NOTEKIND_DEP_NEW)   ? "�Աݽ�" :	
							   (nRB3NoteKindUp == BRM_NOTEKIND_50000)     ? "����" : "UNK";

				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("[3ī��Ʈ ���� ���� ����(�Ÿ� <-> %s)]", csrGetValue));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BRMCstMatchInfo [3ī��Ʈ ���� ���� ����(�Ÿ� <-> %s)]", csrGetValue);

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

	int	nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����
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
					fnSCR_DisplayString(2, "������ ��� �������Դϴ�");
					fnSCR_DisplayString(3, "���� �ð����� ���������̿���");
					fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
					fnSCR_DisplayScreen(701);// ȭ�����

					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_BRMCstSetInfo] -1");
					fnCSH_Initialize();								
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_BRMCstSetInfo] -2");
					nBRMStatus = fnCSH_GetDeviceStatus();
					//#N0154
					fnAPL_SetSensorInfo();							// ������������
					fnAPL_GetAvailTrans();							// �ŷ����ɻ��±��ϱ�
					
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

				fnSCR_DisplayString(2, "������ ��� �������Դϴ�");
				fnSCR_DisplayString(3, "���� �ð����� ���������̿���");
				fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
				fnSCR_DisplayScreen(701);// ȭ�����

				fnUTL_IniSetInt(_SP_BRM_CSTINFO_INI, "SET_CSTINFO", "ChangedCstInfo", TRUE);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_BRMCstSetInfo] -1");
				fnCSH_Initialize();								
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[fnAPL_BRMCstSetInfo] -2");
				nReturn = TRUE;
				//#N0154
				fnAPL_SetSensorInfo();							// ������������
				fnAPL_GetAvailTrans();							// �ŷ����ɻ��±��ϱ�
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

	int	nCurRB1Cnt  = RegGetInt(_REGKEY_BRMSTATUS, "RB1NoteCnt", 0); //5����
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
				
				fnSCR_DisplayString(2, "������ ��� �������Դϴ�");
				fnSCR_DisplayString(3, "���� �ð����� ���������̿���");
				fnSCR_DisplayString(4, "��ø� ��ٷ� �ֽʽÿ�");
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

// ATM�⵿�� �ʱ� �ѹ��� ���������
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
// ��⸦ �����ϴ� �Լ�
	fnVFD_SetBrightnessLevel(10);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetLedInformation2 VFDTotalCount[%d]", VFDTotalCount);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetLedInformation2 nEnterEffect[%d], nExitEffect[%d], nImageCount[%d]", nEnterEffect, nExitEffect, nImageCount);
	
	// fnVFD_RealTimeTextDisplay => �ؽ�Ʈ�� ROM�� ������Ʈ �Ͽ� �ش�
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
			fnVFD_RealTimeTextDisplay(strTempArray[i].GetBuffer(0), i+1, nEnterEffect, 0, 5, 5, 0, "����ü");      
														//���ڿ�, �ε���, ����ȿ��, �뵵, �����ð�, ȿ���ӵ�, ������, ��Ʈ 
		}
	}
	
	//	fnVFD_RealTimeBitImageDisplay => �̹����� ROM�� ������Ʈ�Ͽ� �ش�
	strScheduleEffect.Empty();
	
	if (nImageCount)
		DisplayKind = LED_IMAGE_SELECT;
	else
		DisplayKind = LED_TEXT_SELECT;
	
	nIndex = 1;
	for (i = 0; i < (nImageCount+VFDTotalCount); i++)
	{
		if (i == 0)
			strImgEffect.Format("%d/%d/%d/%d/%d/%d", DisplayKind, nIndex, nEnterEffect, 5, nExitEffect, 5);		// ����ȿ�� = ����ȿ��||�����ð�||����ȿ��||ȿ���ӵ�
		else
			strImgEffect.Format("|%d/%d/%d/%d/%d/%d", DisplayKind, nIndex, nEnterEffect, 5, nExitEffect, 5);	// ����ȿ�� = ����ȿ��||�����ð�||����ȿ��||ȿ���ӵ�
		
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
	// ����ȿ��, ����(1 ~ 15������ �����ϰ� ���� Text�� �̹��� �� ����), �������� �⺻�� SET
	return TRUE;
}






// OM���� ������ Data�� 16����Ʈ ������ ������ �����Ѵ�
// �ѹ��� LED�� Display�� �� �ִ� �����ʹ� 16����Ʈ�̴� �ѱ��� 8�� ȥ��� ����©�� ���� �����ʿ���
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
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "voiceMsg.ini ���� read����");
		return;
	}
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "voiceMsg.ini ���� KeyName:%s Temp:%s", KeyName, Temp);
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
// 305���� ȭ���� < �� > �ȿ� �ִ� ","�� ���� 14����
// ���⼭ < �� > �ȿ� �ִ� ��ư�� max 12���� �����Ͽ� 13��, 14��° ������ ������ ����
// ��, PdisableBtn[13] �� PdisableBtn[14] �� FALSE�̸� �̰��� ��ư���� �ν��� ����.
// ����, 2�������� ������.
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
	// ���̽� ����� NG�ڵ� ����
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

		DispEditString.Replace("-", "�ٽ�");
	}
	DispEditString.Replace("&"," ");
	DispEditString.Replace("["," ");					// V09.43.00-#07
	DispEditString.Replace("]"," ");					// V09.43.00-#07
	/////////////////////////////////////////////////////////////////////////////
	
	// PowerTTS-S ��������
	memset(m_pText, 0x00, sizeof(m_pText));
	memcpy(m_pText, DispEditString.GetBuffer(0),DispEditString.GetLength());
	fnAPL_TTSplay();	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "MakeVoice Return");

}


// �������ν���
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

	memset(&EEjrD, ' ', sizeof(EEjrD));								// �������㿵��
	memset(&EjrAmount, 0, sizeof(EjrAmount));					// ��������ݾװ����ڷ� : 2003.12.15
																// Key Area Setting

	if (nSize <= 0)												// ���̱��ϱ�&����
		nSize = strlen(szSpoolData);
	if (nSize > JNL_MAX_DATA)
		nSize = JNL_MAX_DATA;

	memcpy(EEjrD.Dummy, GetString((LPSTR)szSpoolData, nSize, nSize).GetBuffer(0), nSize);
	
	EEjrD.Cr = A_CR;
	EEjrD.Lf = A_LF;


	if (nDevId & DEV_JPR)										// �����ڷ������
	{
		fnAPL_EjrAddSerialNo();									// ���������Ϸù�ȣ����															// Key Area Setting
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
			memcpy(EEjrD.TranMaterial, " �����", 9);



		_mkdir(_EJR_DIR);											// Dir����
		_mkdir(_EJR_TXT);
		sprintf(szTempFileName, "%s\\%8.8s.JNL", _EJR_TXT, m_pProfile->TRANS.YYYYMMDD);
		
		if(!CFile::GetStatus(szTempFileName, TempFileStatus))		// File����ó��ڰ���
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
