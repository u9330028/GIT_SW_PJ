/////////////////////////////////////////////////////////////////////////////
//	DevCsh.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////////
//	CASH FUNCTION(CASH) : CASH SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnCSH_ClearErrorCode()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_ClearErrorCode();
			break;
		case CDU_SET:
			nReturn = fnCDU_ClearErrorCode();
			break;
	}

	return nReturn;
}

// Clear Dispense Information
int	CDevCmn::fnCSH_ClearDispenseInfo()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:										// T2ATM 2006.5.24 yscho
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			break;
		case CDU_SET:
			nReturn = fnCDU_ClearDispenseInfo();
			break;
	}

	return nReturn;
}


// Set Value Of Cash(Cash Value Data)
int	CDevCmn::fnCSH_SetValueOfCash(LPCTSTR szCashValueData)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			break;
		case CDU_SET:											// 2005.02.24
			nReturn = fnCDU_SetValueOfCash(szCashValueData);
			break;
	}

	return nReturn;
}


// Set MinMax Of CST(Min Count Data, Max Count Data)
int	CDevCmn::fnCSH_SetMinMaxCST(LPCTSTR szMinCntData, LPCTSTR szMaxCntData)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			break;
		case CDU_SET:											// 2004.11.05
			nReturn = fnCDU_SetMinMaxCST(szMinCntData, szMaxCntData);
			break;
	}

	return nReturn;
}

// Set Number Of Cash(Cash Count Data, Total Cash Count Data)
int	CDevCmn::fnCSH_SetNumberOfCash(LPCTSTR szCashCntData, LPCTSTR szTotalCashCntData)
{
	int		nReturn = FALSE;
	CString	strTotalCashCntData(szTotalCashCntData);			// 2004.11.05
	CStringArray strTotalCashCntDataArray;

	SplitString(strTotalCashCntData, ",", strTotalCashCntDataArray);

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			break;
		case CDU_SET:
			nReturn = fnCDU_SetNumberOfCash(szCashCntData);		// 2004.11.05

			if ((CSTCnt >= CDU_CST_1) &&						// 2004.11.05
				(strTotalCashCntDataArray.GetSize() > CDU_CST_1))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST1, 
										  Asc2Int(strTotalCashCntDataArray[CDU_CST_1])); 
			}
			if ((CSTCnt >= CDU_CST_2) &&
				(strTotalCashCntDataArray.GetSize() > CDU_CST_2))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST2, 
										  Asc2Int(strTotalCashCntDataArray[CDU_CST_2])); 
			}
			if ((CSTCnt >= CDU_CST_3) &&
				(strTotalCashCntDataArray.GetSize() > CDU_CST_3))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST3, 
										  Asc2Int(strTotalCashCntDataArray[CDU_CST_3])); 
			}
			if ((CSTCnt >= CDU_CST_4) &&
				(strTotalCashCntDataArray.GetSize() > CDU_CST_4))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST4, 
										  Asc2Int(strTotalCashCntDataArray[CDU_CST_4])); 
			}
			break;
	}

	return nReturn;
}

// Set Number Of Dispense Cash(Dispense Count Data)
int	CDevCmn::fnCSH_SetNumberOfDispenseCash(LPCTSTR szDispCntData)
{
	int		nReturn = FALSE;
	CString	strDispCntData(szDispCntData);						// 2004.11.05
	CStringArray strDispCntDataArray;

	SplitString(strDispCntData, ",", strDispCntDataArray);

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			break;
		case CDU_SET:
			if ((CSTCnt >= CDU_CST_1) &&						// 2004.11.05
				(strDispCntDataArray.GetSize() > CDU_CST_1))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST1, 
										  Asc2Int(strDispCntDataArray[CDU_CST_1])); 
			}
			if ((CSTCnt >= CDU_CST_2) &&
				(strDispCntDataArray.GetSize() > CDU_CST_2))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST2, 
										  Asc2Int(strDispCntDataArray[CDU_CST_2])); 
			}
			if ((CSTCnt >= CDU_CST_3) &&
				(strDispCntDataArray.GetSize() > CDU_CST_3))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST3, 
										  Asc2Int(strDispCntDataArray[CDU_CST_3])); 
			}
			if ((CSTCnt >= CDU_CST_4) &&
				(strDispCntDataArray.GetSize() > CDU_CST_4))
			{
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST4, 
										  Asc2Int(strDispCntDataArray[CDU_CST_4])); 
			}
			break;
	}

	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	CASH FUNCTION(CASH) : CASH GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrCSH_GetErrorCode()
{
	CString strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			strReturn = fstrBRM_GetErrorCode();
			break;
		case CDU_SET:
			strReturn = fstrCDU_GetErrorCode();
			break;
	}

	return strReturn;
}

// Get Sensor Information
CString CDevCmn::fstrCSH_GetSensorInfo()
{
	CString	strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			strReturn = fstrBRM_GetSensorInfo();
			break;
		case CDU_SET:
			strReturn = fstrCDU_GetSensorInfo();
			break;
	}

	return strReturn;
}

// Get Device Status
int	CDevCmn::fnCSH_GetDeviceStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_GetDeviceStatus();
			break;
		case CDU_SET:
			nReturn = fnCDU_GetDeviceStatus();
			break;
	}

	return nReturn;
}

// Get Position
int	CDevCmn::fnCSH_GetPosition()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_GetPosition();
			break;
	}

	return nReturn;
}

// Get Change Unit Lock
int	CDevCmn::fnCSH_GetChangeUnitLock()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_GetChangeUnitLock();
			break;
	}

	return nReturn;
}

// Get Clerk CST Status
int	CDevCmn::fnCSH_GetClerkCSTStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_GetClerkCSTStatus();
			break;
		case CDU_SET:
			nReturn = CST_NORMAL;
			break;
	}

	return nReturn;
}

// Get Reject CST Status
int	CDevCmn::fnCSH_GetRejectCSTStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											
			nReturn = fnBRM_GetRejectCSTStatus();
			break;
		case CDU_SET:
			nReturn = fnCDU_GetRejectCSTStatus();
			break;
	}

	return nReturn;
}

// Get Retract CST Status (Brm21 Only)
int	CDevCmn::fnCSH_GetRetractCSTStatus()
{
	static	int		nSaveReturn = FALSE;
	int		nReturn = CST_NORMAL;

	CString cstrGetStatus("");

//  //#N0134
	cstrGetStatus = RegGetStr(_REGKEY_BRMSTATUS, "FRJBStatus");
	if(AtmDefine.MachineType == U3100K)
	{
		if     (cstrGetStatus == "OK")         nReturn = CST_NORMAL;
		else if(cstrGetStatus == "LOW")        nReturn = CST_NEAR;
		else if(cstrGetStatus == "HIGH")       nReturn = CST_FULL;
		else if(cstrGetStatus == "FULL")       nReturn = CST_FULL;
		else if(cstrGetStatus == "EMPTY")      nReturn = CST_EMPTY; 
		else if(cstrGetStatus == "MISSING")    nReturn = CST_SET_NG; 
		else                                   nReturn = CST_SET_NG;
	}
	else
	if(AtmDefine.MachineType == U8100)
	{
		if     (cstrGetStatus == "CST_NORMAL")     nReturn = CST_NORMAL;
		else if(cstrGetStatus == "CST_NEAR")       nReturn = CST_NEAR;
		else if(cstrGetStatus == "CST_HIGH")       nReturn = CST_FULL;
		else if(cstrGetStatus == "CST_FULL")       nReturn = CST_FULL;
		else if(cstrGetStatus == "CST_EMPTY")      nReturn = CST_EMPTY; 
		else if(cstrGetStatus == "CST_SET_NG")     nReturn = CST_SET_NG; 
		else                                       nReturn = CST_SET_NG;
	}
	else
	{
		if     (cstrGetStatus == "CST_NORMAL")     nReturn = CST_NORMAL;
		else if(cstrGetStatus == "CST_NEAR")       nReturn = CST_NEAR;
		else if(cstrGetStatus == "CST_HIGH")       nReturn = CST_FULL;
		else if(cstrGetStatus == "CST_FULL")       nReturn = CST_FULL;
		else if(cstrGetStatus == "CST_EMPTY")      nReturn = CST_EMPTY; 
		else if(cstrGetStatus == "CST_SET_NG")     nReturn = CST_SET_NG; 
		else                                       nReturn = CST_SET_NG;
	}
	
		
	

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetRetractCSTStatus():return(%d)", nReturn);
}

	return nReturn;
}
// Get Recycle Box Status
int	CDevCmn::fnCSH_GetRecycleBoxStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_GetRecycleBoxStatus();
			break;
		case CDU_SET:
			nReturn = fnCDU_GetRecycleBoxStatus();
			break;
	}

	return nReturn;
}

// V07-00-00-#01 오만원권 지원 ────────────────────────
// Get Recycle Box Status
int	CDevCmn::fnCSH_GetRecycleBox50000Status()
{
	int		nReturn = FALSE;
	
	switch (AtmDefine.CashHandler)
	{
	case OKI_SET:
	case HBRM_SET:												// U8100-AP변경
		nReturn = fnBRM_GetRecycleBox50000Status();
		break;
	case BRM_SET:
	case CDU_SET:
		nReturn = FALSE;
		break;
	}
	
	return nReturn;
}
// V06-01-01-#04-end

// Get Number Of Cash
int	CDevCmn::fnCSH_GetNumberOfCash()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_GetNumberOfCash();
			break;
		case CDU_SET:
			nReturn = 0;
			break;
	}

	return nReturn;
}


int CDevCmn::fnCSH_GetNumberOf50000Cash()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_GetNumberOf50000Cash();
			break;
		case BRM_SET:															
		case CDU_SET:
			nReturn = 0;
			break;
	}

	return nReturn;
}

int CDevCmn::fnCSH_GetClerkRJBStatus()
{
	int		nReturn = FALSE;
	
	switch (AtmDefine.CashHandler)
	{
	case OKI_SET:
	case HBRM_SET:												
		nReturn = fnBRM_GetClerkRJBStatus();
		break;
	case BRM_SET:
	case CDU_SET:
		nReturn = 0;
		break;
	}
	
	return nReturn;
}
// V06-01-01-#18-end


// Get Autoload Condition
int	CDevCmn::fnCSH_GetAutoloadCondition()
{
	int		nReturn = FALSE;

	//[정리필요반듯이]
	return NEED_NONE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_GetAutoloadCondition();
			break;
		case CDU_SET:
			nReturn = NEED_NONE;
			break;
	}

	return nReturn;
}

// Get Material Information
int	CDevCmn::fnCSH_GetMaterialInfo()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_GetMaterialInfo();
			break;
		case CDU_SET:
			nReturn = fnCDU_GetMaterialInfo();
			break;
	}

	return nReturn;
}

// Get Avail Deposit
int	CDevCmn::fnCSH_GetAvailDeposit()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_GetAvailDeposit();
			break;
		case CDU_SET:
			nReturn = DEP_NOT_AVAILABLE;
			break;
	}

	return nReturn;
}

// Get Avail Withdraw
int	CDevCmn::fnCSH_GetAvailWithdraw()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_GetAvailWithdraw();
			break;
		case CDU_SET:
			nReturn = fnCDU_GetAvailWithdraw();
			if (!fnAPL_GetNumberOfOutCheck())					// 출금수표매수구하기
				nReturn &= ~WITH_CHECK_ONLY;
			break;
	}

	return nReturn;
}

// Get Accept Count Status
int	CDevCmn::fnCSH_GetAcceptCountStatus()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_GetAcceptCountStatus();
			break;
		case CDU_SET:
			nReturn = NORMAL;
			break;
	}

	return nReturn;
}

// Get Count Result
CString CDevCmn::fstrCSH_GetCountResult()
{
	CString strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			strReturn = fstrBRM_GetCountResult();
			break;
		case CDU_SET:
			strReturn = "0,0,0,0,0,0";
			break;
	}

	return strReturn;
}

// Get Retract Result
CString CDevCmn::fstrCSH_GetRetractResult()
{
	CString strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			strReturn = fstrBRM_GetRetractResult();
			break;
		case CDU_SET:
			strReturn = "0,0,0,0,0,0";
			break;
	}

	return strReturn;
}

// Get Refill Result
CString CDevCmn::fstrCSH_GetRefillResult()
{
	CString strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			strReturn = fstrBRM_GetRefillResult();
			break;
		case CDU_SET:
			strReturn = "0,0,0,0,0,0,0,0";
			break;
	}

	return strReturn;
}

// Get Takeup Result
CString CDevCmn::fstrCSH_GetTakeupResult()
{
	CString strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			strReturn = fstrBRM_GetTakeupResult();
			break;
		case CDU_SET:
			strReturn = "0,0,0,0,0,0,0,0";
			break;
	}

	return strReturn;
}

// Get Number Of CST
int	CDevCmn::fnCSH_GetNumberOfCST()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = 1;
			break;
		case CDU_SET:
			nReturn = fnCDU_GetNumberOfCST();
			break;
	}

	return nReturn;
}

// Get Amount Of CST
int	CDevCmn::fnCSH_GetAmountOfCST()
{
	int		nAmount = 0;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nAmount = 0;
			break;
		case CDU_SET:
			if ((CSTCnt >= 1) &&								// 2004.08.23
				(FstCSTLoc == NORMAL_POS) &&					// 2004.11.05
				((FstCSTStatus == CST_NORMAL) || (FstCSTStatus == CST_NEAR)) &&
				(fnCSH_GetNumberOfCash(CDU_CST_1) > CSH_EMPTYCASHCNT))
			{
				if (FstCSTValue == CASH_KOREA_10000)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_KOREA_10000;
				else
				if (FstCSTValue == CASH_KOREA_5000)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_KOREA_5000;
				else
				if (FstCSTValue == CASH_KOREA_1000)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_KOREA_1000;
			}
			if ((CSTCnt >= 2) &&
				(SndCSTLoc == NORMAL_POS) &&					// 2004.11.05
				((SndCSTStatus == CST_NORMAL) || (SndCSTStatus == CST_NEAR)) &&
				(fnCSH_GetNumberOfCash(CDU_CST_2) > CSH_EMPTYCASHCNT))
			{
				if (FstCSTValue == CASH_KOREA_10000)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_KOREA_10000;
				else
				if (FstCSTValue == CASH_KOREA_5000)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_KOREA_5000;
				else
				if (FstCSTValue == CASH_KOREA_1000)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_KOREA_1000;
			}
			if ((CSTCnt >= 3) &&
				(TrdCSTLoc == NORMAL_POS) &&					// 2004.11.05
				((TrdCSTStatus == CST_NORMAL) || (TrdCSTStatus == CST_NEAR)) &&
				(fnCSH_GetNumberOfCash(CDU_CST_3) > CSH_EMPTYCASHCNT))
			{
				if (FstCSTValue == CASH_KOREA_10000)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_KOREA_10000;
				else
				if (FstCSTValue == CASH_KOREA_5000)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_KOREA_5000;
				else
				if (FstCSTValue == CASH_KOREA_1000)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_KOREA_1000;
			}
			if ((CSTCnt >= 4) &&
				(FthCSTLoc == NORMAL_POS) &&					// 2004.11.05
				((FthCSTStatus == CST_NORMAL) || (FthCSTStatus == CST_NEAR)) &&
				(fnCSH_GetNumberOfCash(CDU_CST_4) > CSH_EMPTYCASHCNT))
			{
				if (FstCSTValue == CASH_KOREA_10000)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_KOREA_10000;
				else
				if (FstCSTValue == CASH_KOREA_5000)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_KOREA_5000;
				else
				if (FstCSTValue == CASH_KOREA_1000)
					nAmount += fnCSH_GetNumberOfCash(CDU_CST_1) * CASH_KOREA_1000;
			}
			break;
		default:
			break;
	}

	return nAmount;
}

// Get BRM RB CST Status(CST No)
int	CDevCmn::fnCSH_GetRBStatus(int nCSTNo)
{
	static	int		nSaveReturn1 = FALSE;
	static	int		nSaveReturn2 = FALSE;
	static	int		nSaveReturn3 = FALSE;
	static	int		nSaveReturn4 = FALSE;
	int		nReturn = CST_NORMAL;
	int		nReturn1 = CST_NORMAL;
	int		nReturn2 = CST_NORMAL;
	int		nReturn3 = CST_NORMAL;
	int		nReturn4 = CST_NORMAL;
	CString cstrGetStatus("");

	//#N0134
	//if(AtmDefine.MachineType != U3100K) return nReturn;

	
	switch(nCSTNo) {
	case CDU_CST_1:
		cstrGetStatus = RegGetStr(_REGKEY_BRMSTATUS, "RB1Status");	
		if     (cstrGetStatus == "OK")         nReturn1 = CST_NORMAL;
		else if(cstrGetStatus == "LOW")        nReturn1 = CST_NEAR;
		else if(cstrGetStatus == "HIGH")       nReturn1 = CST_FULL;
		else if(cstrGetStatus == "FULL")       nReturn1 = CST_FULL;
		else if(cstrGetStatus == "EMPTY")      nReturn1 = CST_EMPTY; 
		else if(cstrGetStatus == "MISSING")    nReturn1 = CST_SET_NG; 
		else                                   nReturn1 = CST_SET_NG;
		nReturn = nReturn1; 
if (nReturn1 != nSaveReturn1)
{
	nSaveReturn1 = nReturn1;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetRBStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn1);
}
		break;
	case CDU_CST_2:
		cstrGetStatus = RegGetStr(_REGKEY_BRMSTATUS, "RB2Status");	
		if     (cstrGetStatus == "OK")         nReturn2 = CST_NORMAL;
		else if(cstrGetStatus == "LOW")        nReturn2 = CST_NEAR;
		else if(cstrGetStatus == "HIGH")       nReturn2 = CST_FULL;
		else if(cstrGetStatus == "FULL")       nReturn2 = CST_FULL;
		else if(cstrGetStatus == "EMPTY")      nReturn2 = CST_EMPTY; 
		else if(cstrGetStatus == "MISSING")    nReturn2 = CST_SET_NG; 
		else                                   nReturn2 = CST_SET_NG;
		nReturn = nReturn2; 
if (nReturn2 != nSaveReturn2)
{
	nSaveReturn2 = nReturn2;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetRBStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn2);
}
		break;
	case CDU_CST_3:
		cstrGetStatus = RegGetStr(_REGKEY_BRMSTATUS, "RB3Status");	
		if     (cstrGetStatus == "OK")         nReturn3 = CST_NORMAL;
		else if(cstrGetStatus == "LOW")        nReturn3 = CST_NEAR;
		else if(cstrGetStatus == "HIGH")       nReturn3 = CST_FULL;
		else if(cstrGetStatus == "FULL")       nReturn3 = CST_FULL;
		else if(cstrGetStatus == "EMPTY")      nReturn3 = CST_EMPTY; 
		else if(cstrGetStatus == "MISSING")    nReturn3 = CST_SET_NG; 
		else                                   nReturn3 = CST_SET_NG;
		nReturn = nReturn3; 
if (nReturn3 != nSaveReturn3)
{
	nSaveReturn3 = nReturn3;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetRBStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn3);
}
		break;
	case CDU_CST_4:
		cstrGetStatus = RegGetStr(_REGKEY_BRMSTATUS, "RB4Status");	
		if     (cstrGetStatus == "OK")         nReturn4 = CST_NORMAL;
		else if(cstrGetStatus == "LOW")        nReturn4 = CST_NEAR;
		else if(cstrGetStatus == "HIGH")       nReturn4 = CST_FULL;
		else if(cstrGetStatus == "FULL")       nReturn4 = CST_FULL;
		else if(cstrGetStatus == "EMPTY")      nReturn4 = CST_EMPTY; 
		else if(cstrGetStatus == "MISSING")    nReturn4 = CST_SET_NG; 
		else                                   nReturn4 = CST_SET_NG;
		nReturn = nReturn4; 
if (nReturn4 != nSaveReturn4)
{
	nSaveReturn4 = nReturn4;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnCSH_GetRBStatus(nCSTNo[%d]):return(%d)", nCSTNo, nReturn4);
}
		break;
	default:
		break;
	}



	return nReturn;
}

// Get CST Status(CST No)
int	CDevCmn::fnCSH_GetCSTStatus(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											
			nReturn = CST_NORMAL;
			break;
		case CDU_SET:
			nReturn = fnCDU_GetCSTStatus(nCSTNo);
			break;
	}

	return nReturn;
}

// Get Value Of Cash(CST No)
int	CDevCmn::fnCSH_GetValueOfCash(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = 0;
			break;
		case CDU_SET:
			nReturn = fnCDU_GetValueOfCash(nCSTNo);
			break;
	}

	return nReturn;
}

// Get Number Of Cash(CST No)
int	CDevCmn::fnCSH_GetNumberOfCash(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = 0;
			break;
		case CDU_SET:
			if (nCSTNo == CDU_CST_REJECT)
				nReturn = fnCDU_GetNumberOfCash(nCSTNo);
			else
			if (CSTCnt >= nCSTNo)
			{
				nReturn = fnCDU_GetNumberOfCash(nCSTNo);		// 2004.11.05
			}
			break;
	}

	return nReturn;
}

// Get Amount Of Cash(CST No)
int	CDevCmn::fnCSH_GetAmountOfCash(int nCSTNo)
{
	int		nAmount = 0;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nAmount = 0;
			break;
		case CDU_SET:
			if (CSTCnt >= nCSTNo)
			{
				if ((nCSTNo == CDU_CST_1) &&					// 2004.08.23
					(FstCSTLoc == NORMAL_POS) &&				// 2004.11.05
					((FstCSTStatus == CST_NORMAL) || (FstCSTStatus == CST_NEAR)) &&
					(fnCSH_GetNumberOfCash(nCSTNo) > CSH_EMPTYCASHCNT))
				{
					if (FstCSTValue == CASH_KOREA_10000)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_KOREA_10000;
					else
					if (FstCSTValue == CASH_KOREA_5000)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_KOREA_5000;
					else
					if (FstCSTValue == CASH_KOREA_1000)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_KOREA_1000;
				}
				if ((nCSTNo == CDU_CST_2) &&
					(SndCSTLoc == NORMAL_POS) &&				// 2004.11.05
					((SndCSTStatus == CST_NORMAL) || (SndCSTStatus == CST_NEAR)) &&
					(fnCSH_GetNumberOfCash(nCSTNo) > CSH_EMPTYCASHCNT))
				{
					if (FstCSTValue == CASH_KOREA_10000)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_KOREA_10000;
					else
					if (FstCSTValue == CASH_KOREA_5000)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_KOREA_5000;
					else
					if (FstCSTValue == CASH_KOREA_1000)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_KOREA_1000;
				}
				if ((nCSTNo == CDU_CST_3) &&
					(TrdCSTLoc == NORMAL_POS) &&				// 2004.11.05
					((TrdCSTStatus == CST_NORMAL) || (TrdCSTStatus == CST_NEAR)) &&
					(fnCSH_GetNumberOfCash(nCSTNo) > CSH_EMPTYCASHCNT))
				{
					if (FstCSTValue == CASH_KOREA_10000)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_KOREA_10000;
					else
					if (FstCSTValue == CASH_KOREA_5000)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_KOREA_5000;
					else
					if (FstCSTValue == CASH_KOREA_1000)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_KOREA_1000;
				}
				if ((nCSTNo == CDU_CST_4) &&
					(FthCSTLoc == NORMAL_POS) &&				// 2004.11.05
					((FthCSTStatus == CST_NORMAL) || (FthCSTStatus == CST_NEAR)) &&
					(fnCSH_GetNumberOfCash(nCSTNo) > CSH_EMPTYCASHCNT))
				{
					if (FstCSTValue == CASH_KOREA_10000)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_KOREA_10000;
					else
					if (FstCSTValue == CASH_KOREA_5000)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_KOREA_5000;
					else
					if (FstCSTValue == CASH_KOREA_1000)
						nAmount += fnCSH_GetNumberOfCash(nCSTNo) * CASH_KOREA_1000;
				}
			}
			break;
		default:
			break;
	}

	return nAmount;
}

// Get Number Of Set Cash(CST No)
int	CDevCmn::fnCSH_GetNumberOfSetCash(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = 0;
			break;
		case CDU_SET:
			if (nCSTNo == CDU_CST_REJECT)
				nReturn = fnCDU_GetNumberOfSetCash(nCSTNo);
			else
			if (CSTCnt >= nCSTNo)
			{
				nReturn = fnCDU_GetNumberOfSetCash(nCSTNo);		// 2004.11.05
			}
			break;
	}

	return nReturn;
}

// Get Dispense Of CST(Cash Amount)
CString CDevCmn::fstrCSH_GetDispenseOfCST(int nCashAmt)
{
	CString strReturn("");

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			strReturn = "0,0,0,0";
			break;
		case CDU_SET:
			strReturn = fstrCDU_GetDispenseOfCST(nCashAmt);
			break;
	}

	return strReturn;
}

// Get Number Of Dispense(CST No)
int	CDevCmn::fnCSH_GetNumberOfDispense(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = 0;
			break;
		case CDU_SET:
			nReturn = fnCDU_GetNumberOfDispense(nCSTNo);
			break;
	}

	return nReturn;
}

// Get Number Of Reject(CST No)
int	CDevCmn::fnCSH_GetNumberOfReject(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = 0;
			break;
		case CDU_SET:
			nReturn = fnCDU_GetNumberOfReject(nCSTNo);
			break;
	}

	return nReturn;
}

// Get Number Of Total Set Cash(CST No)
int	CDevCmn::fnCSH_GetNumberOfTotalSetCash(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = 0;
			break;
		case CDU_SET:
			if (nCSTNo == CDU_CST_REJECT)
				nReturn = 0;
			else
			if (CSTCnt >= nCSTNo)								// 2004.08.23
			{
				if (nCSTNo == CDU_CST_1)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST1);
				if (nCSTNo == CDU_CST_2)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST2);
				if (nCSTNo == CDU_CST_3)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST3);
				if (nCSTNo == CDU_CST_4)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_TOTALSETCASHCST4);
			}
			break;
	}

	return nReturn;
}

// Get Number Of Dispense Cash(CST No)
int	CDevCmn::fnCSH_GetNumberOfDispenseCash(int nCSTNo)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = 0;
			break;
		case CDU_SET:
			if (nCSTNo == CDU_CST_REJECT)
				nReturn = 0;
			else
			if (CSTCnt >= nCSTNo)								// 2004.08.23
			{
				if (nCSTNo == CDU_CST_1)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST1);
				if (nCSTNo == CDU_CST_2)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST2);
				if (nCSTNo == CDU_CST_3)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST3);
				if (nCSTNo == CDU_CST_4)
					nReturn = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_DISPCASHCST4);
			}
			break;
	}

	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	CASH FUNCTION(CASH) : CASH OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnCSH_Initialize()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_Initialize();
			break;
		case CDU_SET:
			nReturn = fnCDU_Initialize();
			break;
	}

	return nReturn;
}

// Deinitialize
int	CDevCmn::fnCSH_Deinitialize()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_Deinitialize();
			break;
		case CDU_SET:
			nReturn = fnCDU_Deinitialize();
			break;
	}

	return nReturn;
}

// Open Shutter
int	CDevCmn::fnCSH_OpenShutter()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_OpenShutter();
			break;
		case CDU_SET:
			nReturn = fnCDU_OpenShutter();
			break;
	}

	return nReturn;
}

// Close Shutter
int	CDevCmn::fnCSH_CloseShutter()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_CloseShutter();
			break;
		case CDU_SET:
			nReturn = fnCDU_CloseShutter();
			break;
	}

	return nReturn;
}

// Ready
int	CDevCmn::fnCSH_Ready()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_Ready();
			break;
		case CDU_SET:
			break;
	}

	return nReturn;
}

// Ready Count
int	CDevCmn::fnCSH_ReadyCount()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_ReadyCount();
			break;
		case CDU_SET:
			break;
	}

	return nReturn;
}

// Accept(Avail Deposit Count)
int	CDevCmn::fnCSH_Accept(int nCashCnt)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_Accept(nCashCnt);
			break;
		case CDU_SET:
			break;
	}

	return nReturn;
}

// V06-01-01-#01-start
int	CDevCmn::fnCSH_AcceptLarge(int nCashCnt, int nAmount)
{
	int		nReturn = FALSE;
	
	switch (AtmDefine.CashHandler)
	{
	case OKI_SET:
	case HBRM_SET:												// U8100-AP변경
		nReturn = fnBRM_AcceptLarge(nCashCnt, nAmount);
		break;
	case BRM_SET:												
	case CDU_SET:
		break;
	}
	
	return nReturn;
}
// V06-01-01-#01-end
// Count
int	CDevCmn::fnCSH_Count()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_Count();
			break;
		case CDU_SET:
			break;
	}

	return nReturn;
}

// Stack
int	CDevCmn::fnCSH_Stack()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_Stack();
			break;
		case CDU_SET:
			break;
	}

	return nReturn;
}

// Reject(Wait Time)
int	CDevCmn::fnCSH_Reject(int nWaitSec)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_Reject(nWaitSec);
			break;
		case CDU_SET:
			break;
	}

	return nReturn;
}

// Wait Lift
int	CDevCmn::fnCSH_WaitLift()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_WaitLift();
			break;
		case CDU_SET:
			break;
	}

	return nReturn;
}

// Wait Taken
int	CDevCmn::fnCSH_WaitTaken()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_WaitTaken();
			break;
		case CDU_SET:
			nReturn = fnCDU_WaitTaken();
			break;
	}

	return nReturn;
}

// Cancel Wait Taken
int	CDevCmn::fnCSH_CancelWaitTaken()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_CancelWaitTaken();
			break;
		case CDU_SET:
			nReturn = fnCDU_CancelWaitTaken();
			break;
	}

	return nReturn;
}

// Retract
int	CDevCmn::fnCSH_Retract()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_Retract();
			break;
		case CDU_SET:
			nReturn = fnCDU_Retract();
			break;
	}

	return nReturn;
}

// Present2(Wait Time):Not Close
int	CDevCmn::fnCSH_Present2(int nWaitSec)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_Present2(nWaitSec);
			break;
		case CDU_SET:
			break;
	}

	return nReturn;
}

// Ready Dispense
int	CDevCmn::fnCSH_ReadyDispense()
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_ReadyDispense();
			break;
		case CDU_SET:
			break;
	}

	return nReturn;
}

// Dispense(Cash Amount)
int	CDevCmn::fnCSH_Dispense(int nCashAmt)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_Dispense(nCashAmt);
			break;
		case CDU_SET:
			nReturn = fnCDU_Dispense(nCashAmt * 10000);
			break;
	}

	return nReturn;
}
// V06-01-01-#01-start
int	CDevCmn::fnCSH_DispenseLarge(int nCash100TCNT, int nCash50TCNT, int nCash10TCNT, int nCheckCNT)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_DispenseLarge(nCash100TCNT, nCash50TCNT, nCash10TCNT);
			break;
		case CDU_SET:
			nReturn = fnCDU_DispenseLarge(nCash50TCNT, nCash10TCNT, nCheckCNT);
			break;
	}

	return nReturn;
}
// V06-01-01-#01-end
// Present(Wait Time)
int	CDevCmn::fnCSH_Present(int nWaitSec)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			nReturn = fnBRM_Present(nWaitSec);
			break;
		case CDU_SET:
			nReturn = fnCDU_Present(nWaitSec);
			break;
	}

	return nReturn;
}

// Dispense(CashCount, CheckCount)
int	CDevCmn::fnCSH_DispenseCashCheckCount(int nCashCnt, int nCheckCnt)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			break;

		case CDU_SET:
			nReturn = fnCDU_DispenseCashCheckCount(nCashCnt, nCheckCnt);
			break;
	}

	return nReturn;
}




// Refill Money(Cash Count)
int	CDevCmn::fnCSH_RefillMoney(int nCashCnt)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			fnAPL_CheckDeviceAction(DEV_CSH);
			nReturn = fnBRM_RefillMoney(nCashCnt);
			break;
		case CDU_SET:
			break;
	}

	return nReturn;
}

// Takeup Money(Cash Count)
int	CDevCmn::fnCSH_TakeupMoney(int nCashCnt)
{
	int		nReturn = FALSE;

	switch (AtmDefine.CashHandler)
	{
		case OKI_SET:
		case BRM_SET:
		case HBRM_SET:											// U8100-AP변경
			fnAPL_CheckDeviceAction(DEV_CSH);
			nReturn = fnBRM_TakeupMoney(nCashCnt);
			break;
		case CDU_SET:
			break;
	}

	return nReturn;
}
