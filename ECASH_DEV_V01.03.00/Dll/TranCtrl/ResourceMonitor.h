// **************************************************************************
//
// (C)Nautilus Hyosung Inc. 2002 All Rights Reserved
//
// 파일명 : ResourceMonitor.h
// 개  요 : 리소스 모니터 클래스
// 이  력 : 2002/10/22 신규작성(서동호)
//
// **************************************************************************
#ifndef _RESOURCEMONITOR_H_
#define _RESOURCEMONITOR_H_

#include <pdh.h>
#include <pdhmsg.h>

// **************************************************************************
// 상수 정의
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
// 클래스명 : 리소스 모니터 클래스
// 개    요 : CPU 및 메모리등의 리소스를 모니터하는 클래스입니다.
// 주의사항 : 없음
//
class CResourceMonitor {

protected:

	PPDHCOUNTERSTRUCT	m_pCpuUsageInfo;	// CPU 리소스 정보
	PPDHCOUNTERSTRUCT	m_pMemUsageInfo;	// MEM 리소스 정보
	HQUERY				m_hPdhQuery;		// PDH 쿼리 핸들
	UINT				m_nNextIndex;		// 데이터 인덱스

public:

	//
	// 조 작 명 : 리소스 모니터 클래스 생성자
	// 파라미터 : 없음
	// 주의사항 : 없음 
	//
	CResourceMonitor();

	//
	// 조 작 명 : 리소스 모니터 클래스 소멸자
	// 파라미터 : 없음 
	// 주의사항 : 없음 
	//
	~CResourceMonitor();

	//
	// 조 작 명 : 리소스 모니터 초기화 처리
	// 개    요 : 리소스 모니터 초기화 처리
	// 함 수 형 : BOOL
	// 함 수 명 : Initialize
	// 파라미터 : 없음 
	// 리 턴 값 : 성공 : TRUE
	//          : 실패 : FALSE
	// 주의사항 : 없음 
	//
	BOOL Initialize();

	//
	// 조 작 명 : 리소스 정보 취득
	// 개    요 : 리소스 정보 취득
	// 함 수 형 : BOOL
	// 함 수 명 : Initialize
	// 파라미터 : nCpuUsage		: CPU 사용률
	//          : nMemUsage		: MEM 사용률
	// 리 턴 값 : 성공 : TRUE
	//          : 실패 : FALSE
	// 주의사항 : 없음 
	//
	BOOL GetResourceInfo(UINT& nCpuUsage, UINT& nMemUsage);
};

#endif	// _RESOURCEMONITOR_H_
