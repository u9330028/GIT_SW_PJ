/////////////////////////////////////////////////////////////////////////////
//	ErrStack.cpp
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

MainName Main[]={
	"Loop"				,
	"���� Count"		,
	"���� Count"
};

ErrorName ErrName[] = {
	"Date"				,
	"Time"				,
	"����ī����" 	    ,
	"����ڵ�"			,
	"��ּ���"			
};

static	ERRSTACK	FIRSTERRSAVE;								// ������ֺ���
static	ERRSTACK	LASTERRSAVE;								// ������ֺ���

/////////////////////////////////////////////////////////////////////////////
//	�����������
/////////////////////////////////////////////////////////////////////////////
void CProfile::ErrStack(char pProcCount, char* pErrorCode, char* pErrorString)
{
	int		i = 0;
	char	gDate[9], gTime[7];
	char	SUB_SECT[81];
	MAININFO	MAIN;
	ERRSTACK	ERR;

	memset(&ERR, 0, sizeof(ERRSTACK));							// �ʱ�ȭ
	GetDateTime(gDate, gTime);									// YYYYMMDD,HHMMSS
	sprintf(ERR.Date, "%4.4s/%2.2s/%2.2s", &gDate[0], &gDate[4], &gDate[6]);
	sprintf(ERR.Time, "%2.2s:%2.2s:%2.2s", &gTime[0], &gTime[2], &gTime[4]);

	ERR.ProcCount[0] = pProcCount;
	ERR.ProcCount[1] = NULL;

	if (FindChar(pErrorCode, sizeof(ERR.ErrorCode), NULL) < sizeof(ERR.ErrorCode))
		memcpy(&ERR.ErrorCode[0], pErrorCode, FindChar(pErrorCode, sizeof(ERR.ErrorCode), NULL));
	else
		memcpy(&ERR.ErrorCode[0], pErrorCode, sizeof(ERR.ErrorCode) - 1);
	ERR.ErrorCode[sizeof(ERR.ErrorCode)-1] = NULL;				// V05-01-04-#06:codesonar ��������

	if (FindChar(pErrorString, sizeof(ERR.ErrorString), NULL) < sizeof(ERR.ErrorString))
		memcpy(&ERR.ErrorString[0], pErrorString, FindChar(pErrorString, sizeof(ERR.ErrorString), NULL));
	else														// V05-01-04-#06:codesonar ��������
		memcpy(&ERR.ErrorString[0], pErrorString, __min(strlen(pErrorString), (sizeof(ERR.ErrorString) - 1)));

	ERR.ErrorString[sizeof(ERR.ErrorString)-1] = NULL;			// V05-01-04-#06:codesonar ��������

	i = 0;
	MAIN.Loop		= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	MAIN.CrtCount	= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	MAIN.StackCount = GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "ErrStack] 1 : %d", MAIN.Loop);
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "ErrStack] 2 : %d", MAIN.CrtCount);
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "ErrStack] 3 : %d", MAIN.StackCount);

	if (MAIN.StackCount)										// ������� Stack(�ϳ����)
	{
		memcpy(&LASTERRSAVE, &ERR, sizeof(LASTERRSAVE));		// ������ֺ���
		return;
	}
	else
	{
		memcpy(&FIRSTERRSAVE, &ERR, sizeof(FIRSTERRSAVE));		// ������ֺ���
		memcpy(&LASTERRSAVE, &ERR, sizeof(LASTERRSAVE));		// ������ֺ���
	}

//	if (MAIN.StackCount)										// ������� Stack(������)
//	{												
//		memcpy(&LASTERRSAVE, &ERR, sizeof(LASTERRSAVE));		// ������ֺ���
//		if (MAIN.CrtCount == 0)
//			MAIN.CrtCount = ErrStack_MaxNumber - 1;
//		else MAIN.CrtCount = MAIN.CrtCount - 1;
//	}
//	else
//	{
//		memcpy(&FIRSTERRSAVE, &ERR, sizeof(FIRSTERRSAVE));		// ������ֺ���
//		memcpy(&LASTERRSAVE, &ERR, sizeof(LASTERRSAVE));		// ������ֺ���
//	}

	sprintf(SUB_SECT, "%s %d", ERR_SEC, MAIN.CrtCount);

	i = 0;
	PutPStr(SUB_SECT, ErrName[i++].name, ERR.Date,			sizeof(ERR.Date),		_ERRSTACK_INI);
	PutPStr(SUB_SECT, ErrName[i++].name, ERR.Time,			sizeof(ERR.Time),		_ERRSTACK_INI);
 	PutPStr(SUB_SECT, ErrName[i++].name, ERR.ProcCount,		sizeof(ERR.ProcCount),	_ERRSTACK_INI);
	PutPStr(SUB_SECT, ErrName[i++].name, ERR.ErrorCode,		sizeof(ERR.ErrorCode),	_ERRSTACK_INI);
	PutPStr(SUB_SECT, ErrName[i++].name, ERR.ErrorString,	sizeof(ERR.ErrorString),_ERRSTACK_INI);

	MAIN.StackCount++;											// ��������
	if (MAIN.StackCount > ErrStack_MaxNumber)
		MAIN.StackCount = ErrStack_MaxNumber;

	MAIN.CrtCount++;											// ��ġ����
	if (MAIN.CrtCount >= ErrStack_MaxNumber) 
	{
		MAIN.Loop = 1;											// ȯ��
		PutPInt(MAIN_SEC, Main[0].name, MAIN.Loop, _ERRSTACK_INI);
		MAIN.CrtCount = 0;
	}

	PutPInt(MAIN_SEC, Main[1].name, MAIN.CrtCount,		_ERRSTACK_INI);
	PutPInt(MAIN_SEC, Main[2].name, MAIN.StackCount,	_ERRSTACK_INI);
}

/////////////////////////////////////////////////////////////////////////////
//	��������ʱ�ȭ
/////////////////////////////////////////////////////////////////////////////
void CProfile::ClearAllErrStack()
{
	int		i = 0;
	char	SUB_SECT[81];
	MAININFO	MAIN;
	ERRSTACK	ERR;

	memset(&MAIN, 0, sizeof(MAININFO));
	i = 0;
	PutPInt(MAIN_SEC, Main[0].name, MAIN.Loop,			_ERRSTACK_INI);
	PutPInt(MAIN_SEC, Main[1].name, MAIN.CrtCount,		_ERRSTACK_INI);
	PutPInt(MAIN_SEC, Main[2].name, MAIN.StackCount,	_ERRSTACK_INI);

	memset(&ERR,  0, sizeof(ERRSTACK));
	StrAllSpace(ERR.Date);
	StrAllSpace(ERR.Time);
	StrAllSpace(ERR.ProcCount);
	StrAllSpace(ERR.ErrorCode);
	StrAllSpace(ERR.ErrorString);

	for (int j = 0; j < ErrStack_MaxNumber; j++) 
	{
		sprintf(SUB_SECT, "%s %d", ERR_SEC, j);

		i = 0;
		PutPStr(SUB_SECT, ErrName[i++].name, ERR.Date,			sizeof(ERR.Date),		_ERRSTACK_INI);
		PutPStr(SUB_SECT, ErrName[i++].name, ERR.Time,			sizeof(ERR.Time),		_ERRSTACK_INI);
	 	PutPStr(SUB_SECT, ErrName[i++].name, ERR.ProcCount,		sizeof(ERR.ProcCount),	_ERRSTACK_INI);
		PutPStr(SUB_SECT, ErrName[i++].name, ERR.ErrorCode,		sizeof(ERR.ErrorCode),	_ERRSTACK_INI);
		PutPStr(SUB_SECT, ErrName[i++].name, ERR.ErrorString,	sizeof(ERR.ErrorString),_ERRSTACK_INI);
	}
}

/////////////////////////////////////////////////////////////////////////////
//	�����������
/////////////////////////////////////////////////////////////////////////////
int CProfile::GetCrtErrStack(ERRSTACK* pEditBuff, bool pClearCommand)
{
	int		i = 0;
	char	SUB_SECT[81];
	MAININFO	MAIN;
	ERRSTACK	ERR;

	i = 1;
	MAIN.CrtCount	= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	MAIN.StackCount = GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	
	if (MAIN.StackCount)										// ��� �ֽ�
	{
		if (MAIN.CrtCount == 0)
			MAIN.CrtCount = ErrStack_MaxNumber - 1;
		else MAIN.CrtCount = MAIN.CrtCount - 1;
		
		sprintf(SUB_SECT, "%s %d", ERR_SEC, MAIN.CrtCount);

		i = 0;
		GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Date,			sizeof(ERR.Date), 		_ERRSTACK_INI);
		GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Time,			sizeof(ERR.Time),		_ERRSTACK_INI);
		GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ProcCount,	sizeof(ERR.ProcCount),	_ERRSTACK_INI);
		GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorCode,	sizeof(ERR.ErrorCode),	_ERRSTACK_INI);
		GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorString,	sizeof(ERR.ErrorString),_ERRSTACK_INI);

		if (pClearCommand)										// Clear Count
		{
			MAIN.StackCount = 0;
			PutPInt(MAIN_SEC, Main[2].name, MAIN.StackCount,	_ERRSTACK_INI);
		}

		memcpy(pEditBuff, &ERR, sizeof(ERRSTACK));
		return 1;
	}
	else														// ��� ����
	{
		StrAllSpace(ERR.Date);
		StrAllSpace(ERR.Time);
		StrAllSpace(ERR.ProcCount);
		StrAllSpace(ERR.ErrorCode);
		StrAllSpace(ERR.ErrorString);

		memcpy(pEditBuff, &ERR, sizeof(ERRSTACK));
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	�����������
/////////////////////////////////////////////////////////////////////////////
int CProfile::GetFirstErrStack(ERRSTACK* pEditBuff)
{
	int		i = 0;
	MAININFO	MAIN;
	ERRSTACK	ERR;

	i = 1;
	MAIN.CrtCount	= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	MAIN.StackCount = GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	
	if (MAIN.StackCount)										// ��� �ֽ�
	{
		memcpy(pEditBuff, &FIRSTERRSAVE, sizeof(FIRSTERRSAVE));	// ������ֺ���
		return 1;
	}
	else														// ��� ����
	{
		StrAllSpace(ERR.Date);
		StrAllSpace(ERR.Time);
		StrAllSpace(ERR.ProcCount);
		StrAllSpace(ERR.ErrorCode);
		StrAllSpace(ERR.ErrorString);

		memcpy(pEditBuff, &ERR, sizeof(ERRSTACK));
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	�����������
/////////////////////////////////////////////////////////////////////////////
int CProfile::GetLastErrStack(ERRSTACK* pEditBuff)
{
	int		i = 0;
	MAININFO	MAIN;
	ERRSTACK	ERR;

	i = 1;
	MAIN.CrtCount	= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	MAIN.StackCount = GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	
	if (MAIN.StackCount)										// ��� �ֽ�
	{
		memcpy(pEditBuff, &LASTERRSAVE, sizeof(LASTERRSAVE));	// ������ֺ���
		return 1;
	}
	else														// ��� ����
	{
		StrAllSpace(ERR.Date);
		StrAllSpace(ERR.Time);
		StrAllSpace(ERR.ProcCount);
		StrAllSpace(ERR.ErrorCode);
		StrAllSpace(ERR.ErrorString);

		memcpy(pEditBuff, &ERR, sizeof(ERRSTACK));
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	��ü�������
/////////////////////////////////////////////////////////////////////////////
int CProfile::GetAllErrStack(ERRSTACK* pEditBuff)
{
	int		i = 0, ii = 0;
	char	SUB_SECT[81];
	MAININFO	MAIN;
	ERRSTACK	ERR;

	i = 0;
	MAIN.Loop		= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	MAIN.CrtCount	= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	
	if (MAIN.Loop || MAIN.CrtCount)								// ��� �ֽ�
	{	
		if (MAIN.CrtCount)
		{
			for (int j = (MAIN.CrtCount - 1); j >= 0; j--)		// �ֱټ�������
			{
				sprintf(SUB_SECT, "%s %d", ERR_SEC, j);

				i = 0;
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Date,			sizeof(ERR.Date), 		_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Time,			sizeof(ERR.Time),		_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ProcCount,	sizeof(ERR.ProcCount),	_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorCode,	sizeof(ERR.ErrorCode),	_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorString,	sizeof(ERR.ErrorString),_ERRSTACK_INI);

				memcpy(&pEditBuff[ii++], &ERR, sizeof(ERRSTACK));
			}
		}

		if (MAIN.Loop)
		{
			for (int j = (ErrStack_MaxNumber - 1); j >= MAIN.CrtCount; j--) 
			{
				sprintf(SUB_SECT, "%s %d", ERR_SEC, j);

				i = 0;
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Date,			sizeof(ERR.Date), 		_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Time,			sizeof(ERR.Time),		_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ProcCount,	sizeof(ERR.ProcCount),	_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorCode,	sizeof(ERR.ErrorCode),	_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorString,	sizeof(ERR.ErrorString),_ERRSTACK_INI);

				memcpy(&pEditBuff[ii++], &ERR, sizeof(ERRSTACK));
			}
			return ErrStack_MaxNumber;
		}
		else return MAIN.CrtCount;
	}
	else return 0;
}

/////////////////////////////////////////////////////////////////////////////
//	��ġ���������
//	V02-10-01-R2 �űǴ���_��ġ�����������ȸ
/////////////////////////////////////////////////////////////////////////////
int CProfile::GetDeviceErrStack(ERRSTACK* pEditBuff, int nDevID)
{
	int		i = 0, ii = 0;
	char	SUB_SECT[81];
	MAININFO	MAIN;
	ERRSTACK	ERR;

	i = 0;
	MAIN.Loop		= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	MAIN.CrtCount	= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	
	if (MAIN.Loop || MAIN.CrtCount)								// ��� �ֽ�
	{	
		if (MAIN.CrtCount)
		{
			for (int j = (MAIN.CrtCount - 1); j >= 0; j--)		// �ֱټ�������
			{
				sprintf(SUB_SECT, "%s %d", ERR_SEC, j);

				i = 0;
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Date,			sizeof(ERR.Date), 		_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Time,			sizeof(ERR.Time),		_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ProcCount,	sizeof(ERR.ProcCount),	_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorCode,	sizeof(ERR.ErrorCode),	_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorString,	sizeof(ERR.ErrorString),_ERRSTACK_INI);

				if (nDevID == DEV_JPR)
				{
					if ((ERR.ErrorCode[0] != '1')				&&
						(memcmp(ERR.ErrorCode, "9721", 4) != 0)	)
						continue;
				}
				else
				if (nDevID == DEV_SPR)
				{
					if ((ERR.ErrorCode[0] != '2')				&&
						(memcmp(ERR.ErrorCode, "9722", 4) != 0)	)
						continue;
				}
				else
				if (nDevID == DEV_MCU)
				{
					if ((ERR.ErrorCode[0] != '3')				&&
						(memcmp(ERR.ErrorCode, "9723", 4) != 0)	)
						continue;
				}
				else
				if (nDevID == DEV_UCM)
				{
					if (ERR.ErrorCode[0] == '4')
					{
						if ((ERR.ErrorCode[1] != '4')	&&
							(ERR.ErrorCode[1] != '5')	&&
							(ERR.ErrorCode[1] != '6')	)
							continue;
					}
					else
					{
						if (memcmp(ERR.ErrorCode, "974", 3) != 0)
							continue;
					}
				}
				else
				if (nDevID == DEV_CSH)
				{
					GetDeviceProfile();
					if (DEVICE.CashHandler == CDU_SET)			// CDU����
					{
						if ((ERR.ErrorCode[0] != '4')			&&
							(memcmp(ERR.ErrorCode, "971", 3) != 0))
							continue;
					}
					else										// BRM����
					{
						if ((ERR.ErrorCode[0] != '4')	&&
							(ERR.ErrorCode[0] != '5')	&&
							(memcmp(ERR.ErrorCode, "971", 3) != 0))
							continue;
						else
						if (ERR.ErrorCode[0] == '4')
						{
							if ((ERR.ErrorCode[1] == '4')	||
								(ERR.ErrorCode[1] == '5')	||
								(ERR.ErrorCode[1] == '6')	)
								continue;
						}
					}
				}
				else
				if (nDevID == DEV_PBM)
				{
					if ((ERR.ErrorCode[0] != '6')				&&
						(memcmp(ERR.ErrorCode, "975", 3) != 0)	)
						continue;
				}
				else											// ��ü : Filtering���� ����
				{
					;
				}

				memcpy(&pEditBuff[ii++], &ERR, sizeof(ERRSTACK));
			}
		}

		if (MAIN.Loop)
		{
			for (int j = (ErrStack_MaxNumber - 1); j >= MAIN.CrtCount; j--) 
			{
				sprintf(SUB_SECT, "%s %d", ERR_SEC, j);

				i = 0;
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Date,			sizeof(ERR.Date), 		_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Time,			sizeof(ERR.Time),		_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ProcCount,	sizeof(ERR.ProcCount),	_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorCode,	sizeof(ERR.ErrorCode),	_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorString,	sizeof(ERR.ErrorString),_ERRSTACK_INI);

				if (nDevID == DEV_JPR)
				{
					if ((ERR.ErrorCode[0] != '1')				&&
						(memcmp(ERR.ErrorCode, "9721", 4) != 0)	)
						continue;
				}
				else
				if (nDevID == DEV_SPR)
				{
					if ((ERR.ErrorCode[0] != '2')				&&
						(memcmp(ERR.ErrorCode, "9722", 4) != 0)	)
						continue;
				}
				else
				if (nDevID == DEV_MCU)
				{
					if ((ERR.ErrorCode[0] != '2')				&&
						(memcmp(ERR.ErrorCode, "9723", 4) != 0)	)
						continue;
				}
				else
				if (nDevID == DEV_UCM)
				{
					if (ERR.ErrorCode[0] == '4')
					{
						if ((ERR.ErrorCode[1] != '4')	&&
							(ERR.ErrorCode[1] != '5')	&&
							(ERR.ErrorCode[1] != '6')	)
							continue;
					}
					else
					{
						if (memcmp(ERR.ErrorCode, "974", 3) != 0)
							continue;
					}
				}
				else
				if (nDevID == DEV_CSH)
				{
					GetDeviceProfile();
					if (DEVICE.CashHandler == CDU_SET)			// CDU����
					{
						if ((ERR.ErrorCode[0] != '4')			&&
							(memcmp(ERR.ErrorCode, "971", 3) != 0))
							continue;
					}
					else										// BRM����
					{
						if ((ERR.ErrorCode[0] != '4')	&&
							(ERR.ErrorCode[0] != '5')	&&
							(memcmp(ERR.ErrorCode, "971", 3) != 0))
							continue;
						else
						if (ERR.ErrorCode[0] == '4')
						{
							if ((ERR.ErrorCode[1] == '4')	||
								(ERR.ErrorCode[1] == '5')	||
								(ERR.ErrorCode[1] == '6')	)
								continue;
						}
					}
				}
				else
				if (nDevID == DEV_PBM)
				{
					if ((ERR.ErrorCode[0] != '6')				&&
						(memcmp(ERR.ErrorCode, "975", 3) != 0)	)
						continue;
				}
				else											// ��ü : Filtering���� ����
				{
					;
				}

				memcpy(&pEditBuff[ii++], &ERR, sizeof(ERRSTACK));
			}
			return ii;
		}
		else return ii;
	}
	else return 0;
}

