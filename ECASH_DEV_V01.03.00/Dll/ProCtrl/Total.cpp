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
	"0:�������������        :",
	"1:������������ǥ���    :",
	"2:������Ÿ����ǥ���    :",
	"3:������������        :",
	"4:DUMMY                 :",
	"5:�������������        :",
	"6:������������ǥ���    :",
	"7:������Ÿ����ǥ���    :",
	"8:������������        :",
	"9:DUMMY                 :",
	"10:DUMMY                :"
};

DepName DEPNAME[] = {
//   01234567890123456789012345
	"0:�����������Աݱݾ�     :",
	"1:�����������ǥ�Աݱݾ� :",
	"2:������Ÿ���ǥ�Աݱݾ� :",
	"3:DUMMY                  :",
	"4:�����������Աݱݾ�     :",
	"5:�����������ǥ�Աݱݾ� :",
	"6:������Ÿ���ǥ�Աݱݾ� :",
	"7:DUMMY                  :",
	"8:DUMMY                  :"
};

TransName TRANSNAME[] = {
//   01234567890123456789012345
	"0:��������ü�ݾ�         :",
	"1:��������ü�ݾ�         :",
	"2:DUMMY                  :",
	"3:DUMMY                  :",
	"4:DUMMY                  :"
};

MoneyName MONEYNAME[]={
//   01234567890123456789
    "0:���ݹ����Ѿ�		      :",
    "1:��ǥ�����Ѿ�           :",
    "2:DUMMY                  :"
};

DepErrName DEPERRNAME[]={
//   01234567890123456789
    "0:�Ա��������Ѿ�      :",
    "1:�Աݰ������Ѿ�      :",
    "2:�Աݼ�������Ѿ�      :",
    "3:�Աݹ̼����Ѿ�        :",
    "4:DUMMY                 :"
};

DepErrName WITHERRNAME[]={
//   01234567890123456789
    "0:�Ա��������Ѿ�      :",
    "1:�Աݰ������Ѿ�      :",
    "2:�Աݼ�������Ѿ�      :",
    "3:�Աݹ̼����Ѿ�        :",
    "4:DUMMY                 :"
};

/////////////////////////////////////////////////////////////////////////////
//	�����ʱ�ȭ
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
//	�Ա��ʱ�ȭ
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
//	��ü�ʱ�ȭ
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitTransTotal()
{
	memset(&TRANSTOTAL, 0, sizeof(TRANSTOTAL));

	for (int i = 0; i < itemof(TRANSTOTAL.DATA); i++)
		StrAllZero(TRANSTOTAL.DATA[i]);

	PutTransTotal();
}

//////////////////////////////////////////////////////////////////////
// ����ݾ��ʱ�ȭ
void CProfile::InitMoneyTotal()
{
	memset(&MONEYTOTAL, 0, sizeof(MONEYTOTAL));

	for (int i = 0; i < itemof(MONEYTOTAL.DATA); i++)
		StrAllZero(MONEYTOTAL.DATA[i]);

	PutMoneyTotal();
}

//////////////////////////////////////////////////////////////////////
// �Ա���ֱݾ��ʱ�ȭ
void CProfile::InitDepErrTotal()
{
	memset(&DEPERRTOTAL, 0, sizeof(DEPERRTOTAL));

	for (int i = 0; i < itemof(DEPERRTOTAL.DATA); i++)
		StrAllZero(DEPERRTOTAL.DATA[i]);

	PutDepErrTotal();
}

//////////////////////////////////////////////////////////////////////
// �Ա���ֱݾ��ʱ�ȭ
void CProfile::InitWithErrTotal()
{
	memset(&WITHERRTOTAL, 0, sizeof(WITHERRTOTAL));

	for (int i = 0; i < itemof(WITHERRTOTAL.DATA); i++)
		StrAllZero(WITHERRTOTAL.DATA[i]);

	PutWithErrTotal();
}
/////////////////////////////////////////////////////////////////////////////
//	�������
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetWithTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(WITHTOTAL.DATA); i++)
		GetPStr(WITHTOTAL_SEC, WITHNAME[i].name, '?', &WITHTOTAL.DATA[i], sizeof(WITHTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	�Ա�����
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetDepTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(DEPTOTAL.DATA); i++)
		GetPStr(DEPTOTAL_SEC, DEPNAME[i].name, '?', &DEPTOTAL.DATA[i], sizeof(DEPTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	��ü����
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetTransTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(TRANSTOTAL.DATA); i++)
		GetPStr(TRANSTOTAL_SEC, TRANSNAME[i].name, '?', &TRANSTOTAL.DATA[i], sizeof(TRANSTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}


//////////////////////////////////////////////////////////////////////
// �ǹ���ݾ�����
void CProfile::GetMoneyTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(MONEYTOTAL.DATA); i++)
		GetPStr(MONEYTOTAL_SEC, MONEYNAME[i].name, '?', &MONEYTOTAL.DATA[i], sizeof(MONEYTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

//////////////////////////////////////////////////////////////////////
// �Ա�����ѱݾ�����
void CProfile::GetDepErrTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(DEPERRTOTAL.DATA); i++)
		GetPStr(DEPERRTOTAL_SEC, DEPERRNAME[i].name, '?', &DEPERRTOTAL.DATA[i], sizeof(DEPERRTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}
//////////////////////////////////////////////////////////////////////
// �������ѱݾ�����
void CProfile::GetWithErrTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(WITHERRTOTAL.DATA); i++)
		GetPStr(WITHERRTOTAL_SEC, WITHERRNAME[i].name, '?', &WITHERRTOTAL.DATA[i], sizeof(WITHERRTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}
/////////////////////////////////////////////////////////////////////////////
//	�����հ�
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutWithTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(WITHTOTAL.DATA); i++)
		PutPStr(WITHTOTAL_SEC, WITHNAME[i].name, &WITHTOTAL.DATA[i], sizeof(WITHTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	�Ա��հ�
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutDepTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(DEPTOTAL.DATA); i++)
		PutPStr(DEPTOTAL_SEC, DEPNAME[i].name, &DEPTOTAL.DATA[i], sizeof(DEPTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	��ü�հ�
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutTransTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(TRANSTOTAL.DATA); i++)
		PutPStr(TRANSTOTAL_SEC, TRANSNAME[i].name, &TRANSTOTAL.DATA[i], sizeof(TRANSTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}

//////////////////////////////////////////////////////////////////////
// �� ���� �ݾ� �հ�
void CProfile::PutMoneyTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(MONEYTOTAL.DATA); i++)
		PutPStr(MONEYTOTAL_SEC, MONEYNAME[i].name, &MONEYTOTAL.DATA[i], sizeof(MONEYTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}
//////////////////////////////////////////////////////////////////////
// �Ա�����ѱݾ�����
void CProfile::PutDepErrTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(DEPERRTOTAL.DATA); i++)
		PutPStr(DEPERRTOTAL_SEC, DEPERRNAME[i].name, &DEPERRTOTAL.DATA[i], sizeof(DEPERRTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}
//////////////////////////////////////////////////////////////////////
// �Ա�����ѱݾ�����
void CProfile::PutWithErrTotal()
{
	EnterCriticalSection(&m_csLock);

	for (int i = 0; i < itemof(WITHERRTOTAL.DATA); i++)
		PutPStr(WITHERRTOTAL_SEC, WITHERRNAME[i].name, &WITHERRTOTAL.DATA[i], sizeof(WITHERRTOTAL.DATA[0]), _ATMTOTAL_INI);

	LeaveCriticalSection(&m_csLock);
}