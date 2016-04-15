// **************************************************************************
//
// (C)Nautilus Hyosung Inc. 2002 All Rights Reserved
//
// 파일명 : ResourceMonitor.cpp
// 개  요 : 리소스 모니터 클래스
//
// **************************************************************************
#include "StdAfx.h"
#include "ResourceMonitor.h"

#pragma comment(lib, "Pdh.lib")

/////////////////////////////////////////////////////////////////////////////
// CResourceMonitor

//
// 조 작 명 : 리소스 모니터 클래스 생성자
// 파라미터 : 없음
// 주의사항 : 없음 
//
CResourceMonitor::CResourceMonitor()
{
	m_pCpuUsageInfo = NULL;
	m_pMemUsageInfo = NULL;
	m_hPdhQuery     = NULL;
	m_nNextIndex    = 0;
}

//
// 조 작 명 : 리소스 모니터 클래스 소멸자
// 파라미터 : 없음 
// 주의사항 : 없음 
//
CResourceMonitor::~CResourceMonitor()
{
	if (m_pCpuUsageInfo != NULL) {
		PdhRemoveCounter(m_pCpuUsageInfo->hCounter);
		delete m_pCpuUsageInfo;
		m_pCpuUsageInfo = NULL;
	}

	if (m_pMemUsageInfo != NULL) {
		PdhRemoveCounter(m_pMemUsageInfo->hCounter);
		delete m_pMemUsageInfo;
		m_pMemUsageInfo = NULL;
	}
	
	if (m_hPdhQuery != NULL) {
		PdhCloseQuery(&m_hPdhQuery);
		m_hPdhQuery = NULL;
	}
}

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
BOOL
CResourceMonitor::Initialize()
{
	if (PdhOpenQuery(NULL, 1, &m_hPdhQuery) != ERROR_SUCCESS) {
//		SFXTRCLOG0(STL_LOG_KIND_ERROR, "CResourceMonitor::Initialize", 
//			"PdhOpenQuery() 에러");
		return FALSE;
	}

	if ((m_pCpuUsageInfo = new PDHCOUNTERSTRUCT()) != NULL) {
		m_pCpuUsageInfo->nIndex       = m_nNextIndex++;
		m_pCpuUsageInfo->lValue       = 0;
		m_pCpuUsageInfo->nNextIndex   = 0;
		m_pCpuUsageInfo->nOldestIndex = 0;
		m_pCpuUsageInfo->nRawCount    = 0;
	}
	else {
//		SFXTRCLOG0(STL_LOG_KIND_ERROR, "CResourceMonitor::Initialize", 
//			"PDHCOUNTERSTRUCT 생성 실패");
		return FALSE;
	}
	
	if (PdhAddCounter(m_hPdhQuery, CNTR_CPUINUSE_PERCENT, (DWORD)m_pCpuUsageInfo, 
			&(m_pCpuUsageInfo->hCounter)) != ERROR_SUCCESS) {
//		SFXTRCLOG0(STL_LOG_KIND_ERROR, "CResourceMonitor::Initialize", 
//			"PdhAddCounter() 에러");
		if (m_pCpuUsageInfo != NULL) {
			delete m_pCpuUsageInfo;
			m_pCpuUsageInfo = NULL;
		}
		return FALSE;
	}
	
	if ((m_pMemUsageInfo = new PDHCOUNTERSTRUCT()) != NULL) {
		m_pMemUsageInfo->nIndex       = m_nNextIndex++;
		m_pMemUsageInfo->lValue       = 0;
		m_pMemUsageInfo->nNextIndex   = 0;
		m_pMemUsageInfo->nOldestIndex = 0;
		m_pMemUsageInfo->nRawCount    = 0;
	}
	else {
//		SFXTRCLOG0(STL_LOG_KIND_ERROR, "CResourceMonitor::Initialize", 
//			"PDHCOUNTERSTRUCT 생성 실패");
		if (m_pCpuUsageInfo != NULL) {
			delete m_pCpuUsageInfo;
			m_pCpuUsageInfo = NULL;
		}
		return FALSE;
	}
	
	if (PdhAddCounter(m_hPdhQuery, CNTR_MEMINUSE_PERCENT, (DWORD)m_pMemUsageInfo, 
			&(m_pMemUsageInfo->hCounter)) != ERROR_SUCCESS) {
//		SFXTRCLOG0(STL_LOG_KIND_ERROR, "CResourceMonitor::Initialize", 
//			"PdhAddCounter() 에러");
		if (m_pCpuUsageInfo != NULL) {
			delete m_pCpuUsageInfo;
			m_pCpuUsageInfo = NULL;
		}
		if (m_pMemUsageInfo != NULL) {
			delete m_pMemUsageInfo;
			m_pMemUsageInfo = NULL;
		}
		return FALSE;
	}
		
	return TRUE;
}

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
BOOL
CResourceMonitor::GetResourceInfo(UINT& nCpuUsage, UINT& nMemUsage)
{
	PDH_FMT_COUNTERVALUE	sPdhFormattedValue;
	PPDH_RAW_COUNTER		pPdhRawCounter = NULL;		
	
	if (PdhCollectQueryData(m_hPdhQuery) != ERROR_SUCCESS) {
		return FALSE;
	}

	// CPU 사용률 취득
	if(m_pCpuUsageInfo != NULL)                            
	{
		if ((PdhGetFormattedCounterValue(m_pCpuUsageInfo->hCounter, 
				PDH_FMT_LONG, NULL, &sPdhFormattedValue) != ERROR_SUCCESS) || 
				(sPdhFormattedValue.CStatus != ERROR_SUCCESS)) {
		//		SFXTRCLOG0(STL_LOG_KIND_ERROR, "CResourceMonitor::Initialize", 
		//			"PdhGetFormattedCounterValue() 에러");
			return FALSE;
		}
		else {
			m_pCpuUsageInfo->lValue = sPdhFormattedValue.longValue;
		}

		pPdhRawCounter = &(m_pCpuUsageInfo->RawValue[m_pCpuUsageInfo->nNextIndex]);

		if (PdhGetRawCounterValue(m_pCpuUsageInfo->hCounter, NULL, 
				pPdhRawCounter) != ERROR_SUCCESS) {
	//		SFXTRCLOG0(STL_LOG_KIND_ERROR, "CResourceMonitor::Initialize", 
	//			"PdhGetRawCounterValue() 에러");
			return FALSE;
		}

		m_pCpuUsageInfo->nRawCount = min(m_pCpuUsageInfo->nRawCount + 1, MAX_RAW_VALUES);
		m_pCpuUsageInfo->nNextIndex = (m_pCpuUsageInfo->nNextIndex + 1) % MAX_RAW_VALUES;

		if (m_pCpuUsageInfo->nRawCount >= MAX_RAW_VALUES) {
			m_pCpuUsageInfo->nOldestIndex = m_pCpuUsageInfo->nNextIndex;
		}

		nCpuUsage = m_pCpuUsageInfo->lValue;
	}


	// MEM 사용률 취득
	if(m_pCpuUsageInfo != NULL)                            
	{
		if ((PdhGetFormattedCounterValue(m_pMemUsageInfo->hCounter, 
			PDH_FMT_LONG, NULL, &sPdhFormattedValue) != ERROR_SUCCESS) || 
				(sPdhFormattedValue.CStatus != ERROR_SUCCESS)) {
	//		SFXTRCLOG0(STL_LOG_KIND_ERROR, "CResourceMonitor::Initialize", 
	//			"PdhGetFormattedCounterValue() 에러");
			return FALSE;
		}
		else {
			m_pMemUsageInfo->lValue = sPdhFormattedValue.longValue;
		}
		
		pPdhRawCounter = &(m_pMemUsageInfo->RawValue[m_pMemUsageInfo->nNextIndex]);
		
		if (PdhGetRawCounterValue(m_pMemUsageInfo->hCounter, NULL, 
				pPdhRawCounter) != ERROR_SUCCESS) {
	//		SFXTRCLOG0(STL_LOG_KIND_ERROR, "CResourceMonitor::Initialize", 
	//			"PdhGetRawCounterValue() 에러");
			return FALSE;
		}
		
		m_pMemUsageInfo->nRawCount = min(m_pMemUsageInfo->nRawCount + 1, MAX_RAW_VALUES);
		m_pMemUsageInfo->nNextIndex = (m_pMemUsageInfo->nNextIndex + 1) % MAX_RAW_VALUES;
		
		if (m_pMemUsageInfo->nRawCount >= MAX_RAW_VALUES) {
			m_pMemUsageInfo->nOldestIndex = m_pMemUsageInfo->nNextIndex;
		}
		
		nMemUsage = m_pMemUsageInfo->lValue;
	}
	
	return TRUE;
}
