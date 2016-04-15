/////////////////////////////////////////////////////////////////////////////
//	ErrHost.cpp
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

HErrName Main[]={
	"Loop"				,
	"현재 Count"		
};

DErrName ErrName[]={
     "내용"
};

/////////////////////////////////////////////////////////////////////////////
//	장애정보설정
/////////////////////////////////////////////////////////////////////////////
void CProfile::ErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec)
{
	int		i = 0;
	char	SUB_SECT[81];
	HERRINFO	MAIN;
	DERRINFO	ERR;

	memcpy(&ERR.Remark[0], &DErrInfo->Remark[0], sizeof(ERR.Remark) - 1);
	ERR.Remark[sizeof(ERR.Remark)-1] = NULL;					// V05-01-04-#06:codesonar 오류지적

	i = 0;
	MAIN.Loop		= GetPInt(MainSec, Main[i++].name, 0, _ERRHOST_INI);
	MAIN.Count		= GetPInt(MainSec, Main[i++].name, 0, _ERRHOST_INI);

	sprintf(SUB_SECT, "%s %d", SubSec, MAIN.Count);

	i = 0;
	PutPStr(SUB_SECT, ErrName[i++].name, ERR.Remark, sizeof(ERR.Remark), _ERRHOST_INI);

	MAIN.Count++;												// 위치정보
	if (MAIN.Count >= ErrHost_MaxNumber) 
	{
		MAIN.Loop = 1;											// 환영
		PutPInt(MainSec, Main[0].name, MAIN.Loop,	_ERRHOST_INI);
		MAIN.Count = 0;
	}

	PutPInt(MainSec, Main[1].name, MAIN.Count,	_ERRHOST_INI);
}

/////////////////////////////////////////////////////////////////////////////
//	장애정보초기화
/////////////////////////////////////////////////////////////////////////////
void CProfile::ClearAllErrSub(char* MainSec, char* SubSec)
{
	int		i = 0;
	char	SUB_SECT[81];
	HERRINFO	MAIN;
	DERRINFO	ERR;

	memset(&MAIN, 0, sizeof(HERRINFO));
	i = 0;
	PutPInt(MainSec, Main[0].name, MAIN.Loop,	_ERRHOST_INI);
	PutPInt(MainSec, Main[1].name, MAIN.Count,	_ERRHOST_INI);

	memset(&ERR,  0, sizeof(DERRINFO));
	StrAllSpace(ERR.Remark);

	for (int j = 0; j < ErrHost_MaxNumber; j++) 
	{
		sprintf(SUB_SECT, "%s %d", SubSec, j);

		i = 0;
		PutPStr(SUB_SECT, ErrName[i++].name, ERR.Remark, sizeof(ERR.Remark), _ERRHOST_INI);
	}
}

/////////////////////////////////////////////////////////////////////////////
//	현재장애정보
/////////////////////////////////////////////////////////////////////////////
int CProfile::GetCrtErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec)
{
	int		i = 0, ii = 0;
	char	SUB_SECT[81];
	HERRINFO	MAIN;
	DERRINFO	ERR;

	i = 0;
	MAIN.Loop	= GetPInt(MainSec, Main[i++].name, 0, _ERRHOST_INI);
	MAIN.Count	= GetPInt(MainSec, Main[i++].name, 0, _ERRHOST_INI);
	
	if (MAIN.Loop || MAIN.Count)								// 장애 있슴
	{
		if (MAIN.Count == 0)
			MAIN.Count = ErrHost_MaxNumber - 1;
		else MAIN.Count = MAIN.Count - 1;

		sprintf(SUB_SECT, "%s %d", SubSec, MAIN.Count);

		i = 0;
		GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Remark, sizeof(ERR.Remark), _ERRHOST_INI);
		return 1;
	}
	else return 0;
}

/////////////////////////////////////////////////////////////////////////////
//	전체장애정보
/////////////////////////////////////////////////////////////////////////////
int CProfile::GetAllErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec)
{
	int		i = 0, ii = 0;
	char	SUB_SECT[81];
	HERRINFO	MAIN;
	DERRINFO	ERR;

	i = 0;
	MAIN.Loop	= GetPInt(MainSec, Main[i++].name, 0, _ERRHOST_INI);
	MAIN.Count	= GetPInt(MainSec, Main[i++].name, 0, _ERRHOST_INI);
	
	if (MAIN.Loop || MAIN.Count)								// 장애 있슴
	{
		if (MAIN.Count)
		{
			for (int j = (MAIN.Count - 1); j >= 0; j--)			// 최근순서부터
			{
				sprintf(SUB_SECT, "%s %d", SubSec, j);

				i = 0;
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Remark, sizeof(ERR.Remark), _ERRHOST_INI);

				memcpy(&DErrInfo[ii++], &ERR, sizeof(DERRINFO));
			}
		}

		if (MAIN.Loop)
		{
			for (int j = (ErrHost_MaxNumber - 1); j >= MAIN.Count; j--) 
			{
				sprintf(SUB_SECT, "%s %d", SubSec, j);

				i = 0;
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Remark, sizeof(ERR.Remark), _ERRHOST_INI);

				memcpy(&DErrInfo[ii++], &ERR, sizeof(DERRINFO));
			}
			return ErrHost_MaxNumber;
		}
		else return MAIN.Count;
	}
	else return 0;
}

/////////////////////////////////////////////////////////////////////////////
//	거래장애정보
/////////////////////////////////////////////////////////////////////////////
void CProfile::ErrTran(DERRINFO* DErrInfo)
{
	ErrSub(DErrInfo, TRANMAIN_SEC, TRANERR_SEC);
	return;
}
void CProfile::ClearAllErrTran()
{
	ClearAllErrSub(TRANMAIN_SEC, TRANERR_SEC);
	return;
}
int CProfile::GetCrtErrTran(DERRINFO* DErrInfo)
{
	return (GetCrtErrSub(DErrInfo, TRANMAIN_SEC, TRANERR_SEC));
}
int CProfile::GetAllErrTran(DERRINFO* DErrInfo)
{
	return (GetAllErrSub(DErrInfo, TRANMAIN_SEC, TRANERR_SEC));
}
	
/////////////////////////////////////////////////////////////////////////////
//	취소장애정보
/////////////////////////////////////////////////////////////////////////////
void CProfile::ErrCancel(DERRINFO* DErrInfo)
{
	ErrSub(DErrInfo, CANCELMAIN_SEC, CANCELERR_SEC);
	return;
}

void CProfile::ClearAllErrCancel()
{
	ClearAllErrSub(CANCELMAIN_SEC, CANCELERR_SEC);
	return;
}
int CProfile::GetCrtErrCancel(DERRINFO* DErrInfo)
{
	return (GetCrtErrSub(DErrInfo, CANCELMAIN_SEC, CANCELERR_SEC));
}
int CProfile::GetAllErrCancel(DERRINFO* DErrInfo)
{
	return (GetAllErrSub(DErrInfo, CANCELMAIN_SEC, CANCELERR_SEC));
}

/////////////////////////////////////////////////////////////////////////////
//	호스트장애정보
/////////////////////////////////////////////////////////////////////////////
void CProfile::ErrHost(DERRINFO* DErrInfo)
{
	ErrSub(DErrInfo, HOSTMAIN_SEC, HOSTERR_SEC);
	return;
}
void CProfile::ClearAllErrHost()
{
	ClearAllErrSub(HOSTMAIN_SEC, HOSTERR_SEC);
	return;
}
int CProfile::GetCrtErrHost(DERRINFO* DErrInfo)
{
	return (GetCrtErrSub(DErrInfo, HOSTMAIN_SEC, HOSTERR_SEC));
}
int CProfile::GetAllErrHost(DERRINFO* DErrInfo)
{
	return (GetAllErrSub(DErrInfo, HOSTMAIN_SEC, HOSTERR_SEC));
}

/////////////////////////////////////////////////////////////////////////////
//	KCash장애정보
/////////////////////////////////////////////////////////////////////////////
void CProfile::ErrKCash(DERRINFO* DErrInfo)
{
	ErrSub(DErrInfo, KCASHMAIN_SEC, KCASHERR_SEC);
	return;
}
void CProfile::ClearAllErrKCash()
{
	ClearAllErrSub(KCASHMAIN_SEC, KCASHERR_SEC);
	return;
}
int CProfile::GetCrtErrKCash(DERRINFO* DErrInfo)
{
	return (GetCrtErrSub(DErrInfo, KCASHMAIN_SEC, KCASHERR_SEC));
}
int CProfile::GetAllErrKCash(DERRINFO* DErrInfo)
{
	return (GetAllErrSub(DErrInfo, KCASHMAIN_SEC, KCASHERR_SEC));
}

