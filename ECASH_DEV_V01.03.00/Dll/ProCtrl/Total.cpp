/////////////////////////////////////////////////////////////////////////////
//	Total.cpp
/////////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Dll\Profile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

WithName WITHNAME[] = {
//   0123456789012345678912345
	"0:마감전현금출금        :",
	"1:마감전자점수표출금    :",
	"2:마감전타점수표출금    :",
	"3:마감전출금취소        :",
	"4:DUMMY                 :",
	"5:마감후현금출금        :",
	"6:마감후자점수표출금    :",
	"7:마감후타점수표출금    :",
	"8:마감후출금취소        :",
	"9:DUMMY                 :",
	"10:DUMMY                :"
};

DepName DEPNAME[] = {
//   01234567890123456789012345
	"0:마감전현금입금금액     :",
	"1:마감전자행수표입금금액 :",
	"2:마감전타행수표입금금액 :",
	"3:DUMMY                  :",
	"4:마감후현금입금금액     :",
	"5:마감후자행수표입금금액 :",
	"6:마감후타행수표입금금액 :",
	"7:DUMMY                  :",
	"8:DUMMY                  :"
};

TransName TRANSNAME[] = {
//   01234567890123456789012345
	"0:마감전이체금액         :",
	"1:마감후이체금액         :",
	"2:DUMMY                  :",
	"3:DUMMY                  :",
	"4:DUMMY                  :"
};

MoneyName MONEYNAME[]={
//   01234567890123456789
    "0:현금방출총액		      :",
    "1:수표방출총액           :",
    "2:DUMMY                  :"
};

DepErrName DEPERRNAME[]={
//   01234567890123456789
    "0:입금통신장애총액      :",
    "1:입금계수장애총액      :",
    "2:입금수납장애총액      :",
    "3:입금미수취총액        :",
    "4:DUMMY                 :"
};

DepErrName WITHERRNAME[]={
//   01234567890123456789
    "0:입금통신장애총액      :",
    "1:입금계수장애총액      :",
    "2:입금수납장애총액      :",
    "3:입금미수취총액        :",
    "4:DUMMY                 :"
};

/////////////////////////////////////////////////////////////////////////////
//	지급초기화
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitWithTotal(int nClear)
{

	for (int i = 0; i < itemof(WITHTOTAL.DATA); i++)
	{
		// 1:Cash SumClear, 2:Check SumClear
		if(nClear == 1)
		{
			switch(i) 
			{
			case 0:
			case 3:
			case 4:
			case 5:
			case 8:
			case 9:
			case 10:
				memset(&WITHTOTAL.DATA[i], 0, sizeof(WITHTOTAL.DATA[i]));
				StrAllZero(WITHTOTAL.DATA[i]);
				break;
			case 1:
			case 2:
			case 6:
			case 7:
				break;
			default:
				break;
			}
		}
		else
		if(nClear == 2)
		{
			switch(i) 
			{
			case 1:
			case 2:
			case 6:
			case 7:
				memset(&WITHTOTAL.DATA[i], 0, sizeof(WITHTOTAL.DATA[i]));
				StrAllZero(WITHTOTAL.DATA[i]);
				break;
			case 0:
			case 3:
			case 4:
			case 5:
			case 8:
			case 9:
			case 10:
				break;
			default:
				break;
			}
		}
		else 
			StrAllZero(WITHTOTAL.DATA[i]);
	}
	PutWithTotal();
}

/////////////////////////////////////////////////////////////////////////////
//	입금초기화
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitDepTotal(int nClear)
{
//	memset(&DEPTOTAL, 0, sizeof(DEPTOTAL));

	for (int i = 0; i < itemof(DEPTOTAL.DATA); i++)
	{
		// 1:Cash SumClear, 2:Check SumClear
		if(nClear == 1)
		{
			switch(i) 
			{
			case 0:
			case 3:
			case 4:
			case 7:
			case 8:
				memset(&DEPTOTAL.DATA[i], 0, sizeof(DEPTOTAL.DATA[i]));
				StrAllZero(DEPTOTAL.DATA[i]);
				break;
			case 1:
			case 2:
			case 5:
			case 6:
				break;
			default:
				break;
			}
		}
		else
		if(nClear == 2)
		{
			switch(i) 
			{
			case 1:
			case 2:
			case 5:
			case 6:
				memset(&DEPTOTAL.DATA[i], 0, sizeof(DEPTOTAL.DATA[i]));
				StrAllZero(DEPTOTAL.DATA[i]);
				break;
			case 0:
			case 3:
			case 4:
			case 7:
			case 8:
				break;
			default:
				break;
			}
		}
		else 
			StrAllZero(DEPTOTAL.DATA[i]);
	}
	PutDepTotal();
}

/////////////////////////////////////////////////////////////////////////////
//	이체초기화
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitTransTotal()
{
	memset(&TRANSTOTAL, 0, sizeof(TRANSTOTAL));

	for (int i = 0; i < itemof(TRANSTOTAL.DATA); i++)
		StrAllZero(TRANSTOTAL.DATA[i]);

	PutTransTotal();
}

//////////////////////////////////////////////////////////////////////
// 방출금액초기화
void CProfile::InitMoneyTotal()
{
	memset(&MONEYTOTAL, 0, sizeof(MONEYTOTAL));

	for (int i = 0; i < itemof(MONEYTOTAL.DATA); i++)
		StrAllZero(MONEYTOTAL.DATA[i]);

	PutMoneyTotal();
}

//////////////////////////////////////////////////////////////////////
// 입금장애금액초기화
void CProfile::InitDepErrTotal()
{
	memset(&DEPERRTOTAL, 0, sizeof(DEPERRTOTAL));

	for (int i = 0; i < itemof(DEPERRTOTAL.DATA); i++)
		StrAllZero(DEPERRTOTAL.DATA[i]);

	PutDepErrTotal();
}

//////////////////////////////////////////////////////////////////////
// 입금장애금액초기화
void CProfile::InitWithErrTotal()
{
	memset(&WITHERRTOTAL, 0, sizeof(WITHERRTOTAL));

	for (int i = 0; i < itemof(WITHERRTOTAL.DATA); i++)
		StrAllZero(WITHERRTOTAL.DATA[i]);

	PutWithErrTotal();
}
/////////////////////////////////////////////////////////////////////////////
//	출금정보
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetWithTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(WITHTOTAL.DATA); i++)
		GetPStr(WITHTOTAL_SEC, WITHNAME[i].name, '?', &WITHTOTAL.DATA[i], sizeof(WITHTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	입금정보
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetDepTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(DEPTOTAL.DATA); i++)
		GetPStr(DEPTOTAL_SEC, DEPNAME[i].name, '?', &DEPTOTAL.DATA[i], sizeof(DEPTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	이체정보
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetTransTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(TRANSTOTAL.DATA); i++)
		GetPStr(TRANSTOTAL_SEC, TRANSNAME[i].name, '?', &TRANSTOTAL.DATA[i], sizeof(TRANSTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}


//////////////////////////////////////////////////////////////////////
// 실방출금액정보
void CProfile::GetMoneyTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(MONEYTOTAL.DATA); i++)
		GetPStr(MONEYTOTAL_SEC, MONEYNAME[i].name, '?', &MONEYTOTAL.DATA[i], sizeof(MONEYTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

//////////////////////////////////////////////////////////////////////
// 입금장애총금액정보
void CProfile::GetDepErrTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(DEPERRTOTAL.DATA); i++)
		GetPStr(DEPERRTOTAL_SEC, DEPERRNAME[i].name, '?', &DEPERRTOTAL.DATA[i], sizeof(DEPERRTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}
//////////////////////////////////////////////////////////////////////
// 출금장애총금액정보
void CProfile::GetWithErrTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(WITHERRTOTAL.DATA); i++)
		GetPStr(WITHERRTOTAL_SEC, WITHERRNAME[i].name, '?', &WITHERRTOTAL.DATA[i], sizeof(WITHERRTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}
/////////////////////////////////////////////////////////////////////////////
//	지급합계
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutWithTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(WITHTOTAL.DATA); i++)
		PutPStr(WITHTOTAL_SEC, WITHNAME[i].name, &WITHTOTAL.DATA[i], sizeof(WITHTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	입금합계
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutDepTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(DEPTOTAL.DATA); i++)
		PutPStr(DEPTOTAL_SEC, DEPNAME[i].name, &DEPTOTAL.DATA[i], sizeof(DEPTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	이체합계
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutTransTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(TRANSTOTAL.DATA); i++)
		PutPStr(TRANSTOTAL_SEC, TRANSNAME[i].name, &TRANSTOTAL.DATA[i], sizeof(TRANSTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

//////////////////////////////////////////////////////////////////////
// 실 방출 금액 합계
void CProfile::PutMoneyTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(MONEYTOTAL.DATA); i++)
		PutPStr(MONEYTOTAL_SEC, MONEYNAME[i].name, &MONEYTOTAL.DATA[i], sizeof(MONEYTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}
//////////////////////////////////////////////////////////////////////
// 입금장애총금액정보
void CProfile::PutDepErrTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(DEPERRTOTAL.DATA); i++)
		PutPStr(DEPERRTOTAL_SEC, DEPERRNAME[i].name, &DEPERRTOTAL.DATA[i], sizeof(DEPERRTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}
//////////////////////////////////////////////////////////////////////
// 입금장애총금액정보
void CProfile::PutWithErrTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(WITHERRTOTAL.DATA); i++)
		PutPStr(WITHERRTOTAL_SEC, WITHERRNAME[i].name, &WITHERRTOTAL.DATA[i], sizeof(WITHERRTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}