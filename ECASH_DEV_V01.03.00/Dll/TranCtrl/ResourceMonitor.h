// **************************************************************************
//
// (C)Nautilus Hyosung Inc. 2002 All Rights Reserved
//
// ���ϸ� : ResourceMonitor.h
// ��  �� : ���ҽ� ����� Ŭ����
// ��  �� : 2002/10/22 �ű��ۼ�(����ȣ)
//
// **************************************************************************
#ifndef _RESOURCEMONITOR_H_
#define _RESOURCEMONITOR_H_

#include <pdh.h>
#include <pdhmsg.h>

// **************************************************************************
// ��� ����
// **************************************************************************

#define MAX_RAW_VALUES			20
#define CNTR_CPUINUSE_PERCENT	"\\Processor(_Total)\\% Processor Time"
#define CNTR_MEMINUSE_PERCENT	"\\Memory\\% Committed Bytes In Use"

/////////////////////////////////////////////////////////////////////////////
// CResourceMonitor

typedef struct _PDHCOUNTERSTRUCT {
	int				nIndex;						// The index of this counter, returned by AddCounter()
	LONG			lValue;						// The current value of this counter
    HCOUNTER		hCounter;					// Handle to the counter - given to use by PDH Library
    int				nNextIndex;					// element to get the next raw value
    int				nOldestIndex;				// element containing the oldes raw value
    int				nRawCount;					// number of elements containing raw values
    PDH_RAW_COUNTER	RawValue[MAX_RAW_VALUES];	// Ring buffer to contain raw values
} PDHCOUNTERSTRUCT, *PPDHCOUNTERSTRUCT;

//
// Ŭ������ : ���ҽ� ����� Ŭ����
// ��    �� : CPU �� �޸𸮵��� ���ҽ��� ������ϴ� Ŭ�����Դϴ�.
// ���ǻ��� : ����
//
class CResourceMonitor {

protected:

	PPDHCOUNTERSTRUCT	m_pCpuUsageInfo;	// CPU ���ҽ� ����
	PPDHCOUNTERSTRUCT	m_pMemUsageInfo;	// MEM ���ҽ� ����
	HQUERY				m_hPdhQuery;		// PDH ���� �ڵ�
	UINT				m_nNextIndex;		// ������ �ε���

public:

	//
	// �� �� �� : ���ҽ� ����� Ŭ���� ������
	// �Ķ���� : ����
	// ���ǻ��� : ���� 
	//
	CResourceMonitor();

	//
	// �� �� �� : ���ҽ� ����� Ŭ���� �Ҹ���
	// �Ķ���� : ���� 
	// ���ǻ��� : ���� 
	//
	~CResourceMonitor();

	//
	// �� �� �� : ���ҽ� ����� �ʱ�ȭ ó��
	// ��    �� : ���ҽ� ����� �ʱ�ȭ ó��
	// �� �� �� : BOOL
	// �� �� �� : Initialize
	// �Ķ���� : ���� 
	// �� �� �� : ���� : TRUE
	//          : ���� : FALSE
	// ���ǻ��� : ���� 
	//
	BOOL Initialize();

	//
	// �� �� �� : ���ҽ� ���� ���
	// ��    �� : ���ҽ� ���� ���
	// �� �� �� : BOOL
	// �� �� �� : Initialize
	// �Ķ���� : nCpuUsage		: CPU ����
	//          : nMemUsage		: MEM ����
	// �� �� �� : ���� : TRUE
	//          : ���� : FALSE
	// ���ǻ��� : ���� 
	//
	BOOL GetResourceInfo(UINT& nCpuUsage, UINT& nMemUsage);
};

#endif	// _RESOURCEMONITOR_H_
