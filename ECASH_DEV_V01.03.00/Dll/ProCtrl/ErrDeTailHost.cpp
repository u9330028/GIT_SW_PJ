/////////////////////////////////////////////////////////////////////////////
//	ErrErrDeTailHost.cpp
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Dll\Profile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

MainNameDH Main[]={
	"Loop"				,
	"현재 Count"		,
	"누적 Count"
};

ErrorNameDH ErrName[] = {
	"거래일자"	            ,
	"거래종류"				,
	"장애종류" 	            ,
	"Dummy000"		
};

static	PrintERRSTACK	FIRSTPERRSAVE;								// 최초장애보관
static	PrintERRSTACK	LASTPERRSAVE;								// 최종장애보관
/////////////////////////////////////////////////////////////////////////////
//	장애정보설정
/////////////////////////////////////////////////////////////////////////////
void CProfile::DetailHErrStack(PrintERRSTACK* pDHErrInfo)
{
	int		i = 0;
	char	gDate[9], gTime[7];
	char	SUB_SECT[81];
	MAININFO	MAIN;
	PrintERRSTACK	PERR;

	memset(&PERR, 0, sizeof(PrintERRSTACK));							// 초기화
	GetDateTime(gDate, gTime);		  //MM/DD-HH:MM-000000  0000100000

	if (FindChar(pDHErrInfo->DateTime_Serial_Amt, sizeof(PERR.DateTime_Serial_Amt), NULL) < sizeof(PERR.DateTime_Serial_Amt))
		memcpy(&PERR.DateTime_Serial_Amt, pDHErrInfo->DateTime_Serial_Amt, FindChar(pDHErrInfo->DateTime_Serial_Amt, sizeof(PERR.DateTime_Serial_Amt), NULL));
	else
		memcpy(&PERR.DateTime_Serial_Amt, pDHErrInfo->DateTime_Serial_Amt, sizeof(PERR.DateTime_Serial_Amt) - 1);
	PERR.DateTime_Serial_Amt[sizeof(PERR.DateTime_Serial_Amt)-1] = NULL;				

	if (FindChar(pDHErrInfo->TranType_Account, sizeof(PERR.TranType_Account), NULL) < sizeof(PERR.TranType_Account))
		memcpy(&PERR.TranType_Account, pDHErrInfo->TranType_Account, FindChar(pDHErrInfo->TranType_Account, sizeof(PERR.TranType_Account), NULL));
	else
		memcpy(&PERR.TranType_Account, pDHErrInfo->TranType_Account, sizeof(PERR.TranType_Account) - 1);
	PERR.TranType_Account[sizeof(PERR.TranType_Account)-1] = NULL;				

	if (FindChar(pDHErrInfo->ErrType_Result, sizeof(PERR.ErrType_Result), NULL) < sizeof(PERR.ErrType_Result))
		memcpy(&PERR.ErrType_Result, pDHErrInfo->ErrType_Result, FindChar(pDHErrInfo->ErrType_Result, sizeof(PERR.ErrType_Result), NULL));
	else
		memcpy(&PERR.ErrType_Result, pDHErrInfo->ErrType_Result, sizeof(PERR.ErrType_Result) - 1);
	PERR.ErrType_Result[sizeof(PERR.ErrType_Result)-1] = NULL;				

	
	

	i = 0;
	MAIN.Loop		= GetPInt(MAINC_SEC, Main[i++].name, 0, _ERRDETAILHOST_INI);
	MAIN.CrtCount	= GetPInt(MAINC_SEC, Main[i++].name, 0, _ERRDETAILHOST_INI);
	MAIN.StackCount = GetPInt(MAINC_SEC, Main[i++].name, 0, _ERRDETAILHOST_INI);
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "ErrStack] 1 : %d", MAIN.Loop);
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "ErrStack] 2 : %d", MAIN.CrtCount);
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "ErrStack] 3 : %d", MAIN.StackCount);

/*
	if (MAIN.StackCount)										// 최초장애 Stack(하나사양)
	{
		memcpy(&LASTPERRSAVE, &PERR, sizeof(LASTPERRSAVE));		// 최종장애보관
		return;
	}
	else
	{
		memcpy(&FIRSTPERRSAVE, &PERR, sizeof(FIRSTPERRSAVE));		// 최초장애보관
		memcpy(&LASTPERRSAVE, &PERR, sizeof(LASTPERRSAVE));		// 최종장애보관
	}

*/
	sprintf(SUB_SECT, "%s %d", DETAILS_SEC, MAIN.CrtCount);

	i = 0;
	PutPStr(SUB_SECT, ErrName[i++].name, PERR.DateTime_Serial_Amt,	sizeof(PERR.DateTime_Serial_Amt),   _ERRDETAILHOST_INI);
	PutPStr(SUB_SECT, ErrName[i++].name, PERR.TranType_Account,		sizeof(PERR.TranType_Account),		_ERRDETAILHOST_INI);
 	PutPStr(SUB_SECT, ErrName[i++].name, PERR.ErrType_Result,		sizeof(PERR.ErrType_Result),	    _ERRDETAILHOST_INI);


	MAIN.StackCount++;											// 누적정보
	if (MAIN.StackCount > ErrSDeTailHost_MaxNumber)
		MAIN.StackCount = ErrSDeTailHost_MaxNumber;

	MAIN.CrtCount++;											// 위치정보
	if (MAIN.CrtCount >= ErrSDeTailHost_MaxNumber) 
	{
		MAIN.Loop = 1;											// 환영
		PutPInt(MAINC_SEC, Main[0].name, MAIN.Loop, _ERRDETAILHOST_INI);
		MAIN.CrtCount = 0;
	}

	PutPInt(MAINC_SEC, Main[1].name, MAIN.CrtCount,		_ERRDETAILHOST_INI);
	PutPInt(MAINC_SEC, Main[2].name, MAIN.StackCount,	_ERRDETAILHOST_INI);
}

/////////////////////////////////////////////////////////////////////////////
//	장애정보초기화
/////////////////////////////////////////////////////////////////////////////
void CProfile::DetailHClearAllErrStack()
{
	int		i = 0;
	char	SUB_SECT[81];
	MAININFO	MAIN;
	PrintERRSTACK	PERR;

	memset(&MAIN, 0, sizeof(MAININFO));
	i = 0;
	PutPInt(MAINC_SEC, Main[0].name, MAIN.Loop,			_ERRDETAILHOST_INI);
	PutPInt(MAINC_SEC, Main[1].name, MAIN.CrtCount,		_ERRDETAILHOST_INI);
	PutPInt(MAINC_SEC, Main[2].name, MAIN.StackCount,	_ERRDETAILHOST_INI);

	memset(&PERR,  0, sizeof(ERRSTACK));
	StrAllSpace(PERR.DateTime_Serial_Amt);
	StrAllSpace(PERR.TranType_Account);
	StrAllSpace(PERR.ErrType_Result);


	for (int j = 0; j < ErrSDeTailHost_MaxNumber; j++) 
	{
		sprintf(SUB_SECT, "%s %d", DETAILS_SEC, j);

		i = 0;
		PutPStr(SUB_SECT, ErrName[i++].name, PERR.DateTime_Serial_Amt,	sizeof(PERR.DateTime_Serial_Amt),   _ERRDETAILHOST_INI);
		PutPStr(SUB_SECT, ErrName[i++].name, PERR.TranType_Account,	sizeof(PERR.TranType_Account),		_ERRDETAILHOST_INI);
 		PutPStr(SUB_SECT, ErrName[i++].name, PERR.ErrType_Result,		sizeof(PERR.ErrType_Result),	    _ERRDETAILHOST_INI);

	}
}

/////////////////////////////////////////////////////////////////////////////
//	현재장애정보
/////////////////////////////////////////////////////////////////////////////
int CProfile::DetailHGetCrtErrStack(PrintERRSTACK* pEditBuff, bool pClearCommand)
{
	int		i = 0;
	char	SUB_SECT[81];
	MAININFO	MAIN;
	PrintERRSTACK	PERR;

	i = 1;
	MAIN.CrtCount	= GetPInt(MAINC_SEC, Main[i++].name, 0, _ERRDETAILHOST_INI);
	MAIN.StackCount = GetPInt(MAINC_SEC, Main[i++].name, 0, _ERRDETAILHOST_INI);
	
	if (MAIN.StackCount)										// 장애 있슴
	{
		if (MAIN.CrtCount == 0)
			MAIN.CrtCount = ErrSDeTailHost_MaxNumber - 1;
		else MAIN.CrtCount = MAIN.CrtCount - 1;
		
		sprintf(SUB_SECT, "%s %d", DETAILS_SEC, MAIN.CrtCount);

		i = 0;
		GetPStr(SUB_SECT, ErrName[i++].name, '?', PERR.DateTime_Serial_Amt,	sizeof(PERR.DateTime_Serial_Amt),   _ERRDETAILHOST_INI);
		GetPStr(SUB_SECT, ErrName[i++].name, '?', PERR.TranType_Account,    sizeof(PERR.TranType_Account),		_ERRDETAILHOST_INI);
		GetPStr(SUB_SECT, ErrName[i++].name, '?', PERR.ErrType_Result,	    sizeof(PERR.ErrType_Result),	        _ERRDETAILHOST_INI);

		if (pClearCommand)										// Clear Count
		{
			MAIN.StackCount = 0;
			PutPInt(MAINC_SEC, Main[2].name, MAIN.StackCount,	_ERRDETAILHOST_INI);
		}

		memcpy(pEditBuff, &PERR, sizeof(PrintERRSTACK));
		return 1;
	}
	else														// 장애 없슴
	{
		StrAllSpace(PERR.DateTime_Serial_Amt);
		StrAllSpace(PERR.TranType_Account);
		StrAllSpace(PERR.ErrType_Result);

		memcpy(pEditBuff, &PERR, sizeof(PrintERRSTACK));
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	최초장애정보
/////////////////////////////////////////////////////////////////////////////
int CProfile::DetailHGetFirstErrStack(PrintERRSTACK* pEditBuff)
{
	int		i = 0;
	MAININFO	MAIN;
	PrintERRSTACK	PERR;

	i = 1;
	MAIN.CrtCount	= GetPInt(MAINC_SEC, Main[i++].name, 0, _ERRDETAILHOST_INI);
	MAIN.StackCount = GetPInt(MAINC_SEC, Main[i++].name, 0, _ERRDETAILHOST_INI);
	
	if (MAIN.StackCount)										// 장애 있슴
	{
		memcpy(pEditBuff, &FIRSTPERRSAVE, sizeof(FIRSTPERRSAVE));	// 최초장애보관
		return 1;
	}
	else														// 장애 없슴
	{
		StrAllSpace(PERR.DateTime_Serial_Amt);
		StrAllSpace(PERR.TranType_Account);
		StrAllSpace(PERR.ErrType_Result);

		memcpy(pEditBuff, &PERR, sizeof(PrintERRSTACK));
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	최종장애정보
/////////////////////////////////////////////////////////////////////////////
int CProfile::DetailHGetLastErrStack(PrintERRSTACK* pEditBuff)
{
	int		i = 0;
	MAININFO	MAIN;
	PrintERRSTACK	PERR;

	i = 1;
	MAIN.CrtCount	= GetPInt(MAINC_SEC, Main[i++].name, 0, _ERRDETAILHOST_INI);
	MAIN.StackCount = GetPInt(MAINC_SEC, Main[i++].name, 0, _ERRDETAILHOST_INI);
	
	if (MAIN.StackCount)										// 장애 있슴
	{
		memcpy(pEditBuff, &LASTPERRSAVE, sizeof(LASTPERRSAVE));	// 최종장애보관
		return 1;
	}
	else														// 장애 없슴
	{
		StrAllSpace(PERR.DateTime_Serial_Amt);
		StrAllSpace(PERR.TranType_Account);
		StrAllSpace(PERR.ErrType_Result);

		memcpy(pEditBuff, &PERR, sizeof(PrintERRSTACK));
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	전체장애정보
/////////////////////////////////////////////////////////////////////////////
int CProfile::DetailHGetAllErrStack(PrintERRSTACK* pEditBuff)
{
	int		i = 0, ii = 0;
	char	SUB_SECT[81];
	MAININFO	MAIN;
	PrintERRSTACK	PERR;

	i = 0;
	MAIN.Loop		= GetPInt(MAINC_SEC, Main[i++].name, 0, _ERRDETAILHOST_INI);
	MAIN.CrtCount	= GetPInt(MAINC_SEC, Main[i++].name, 0, _ERRDETAILHOST_INI);
	
	if (MAIN.Loop || MAIN.CrtCount)								// 장애 있슴
	{	
		if (MAIN.CrtCount)
		{
			for (int j = (MAIN.CrtCount - 1); j >= 0; j--)		// 최근순서부터
			{
				sprintf(SUB_SECT, "%s %d", DETAILS_SEC, j);

				i = 0;
				GetPStr(SUB_SECT, ErrName[i++].name, '?', PERR.DateTime_Serial_Amt,	sizeof(PERR.DateTime_Serial_Amt),   _ERRDETAILHOST_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', PERR.TranType_Account,    sizeof(PERR.TranType_Account),		_ERRDETAILHOST_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', PERR.ErrType_Result,	    sizeof(PERR.ErrType_Result),	        _ERRDETAILHOST_INI);

				memcpy(&pEditBuff[ii++], &PERR, sizeof(PrintERRSTACK));
			}
		}

		if (MAIN.Loop)
		{
			for (int j = (ErrSDeTailHost_MaxNumber - 1); j >= MAIN.CrtCount; j--) 
			{
				sprintf(SUB_SECT, "%s %d", DETAILS_SEC, j);

				i = 0;
				GetPStr(SUB_SECT, ErrName[i++].name, '?', PERR.DateTime_Serial_Amt,	sizeof(PERR.DateTime_Serial_Amt),   _ERRDETAILHOST_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', PERR.TranType_Account,    sizeof(PERR.TranType_Account),		_ERRDETAILHOST_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', PERR.ErrType_Result,	    sizeof(PERR.ErrType_Result),	        _ERRDETAILHOST_INI);

				memcpy(&pEditBuff[ii++], &PERR, sizeof(PrintERRSTACK));
			}
			return ErrSDeTailHost_MaxNumber;
		}
		else return MAIN.CrtCount;
	}
	else return 0;
}

