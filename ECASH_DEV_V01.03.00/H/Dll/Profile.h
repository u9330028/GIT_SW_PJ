/////////////////////////////////////////////////////////////////////////////
// Profile.h: interface for the CProfile class.
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_PROFILE_H__2203AD10_FC4B_11D4_A80E_00E09880ACB0__INCLUDED_)
#define AFX_PROFILE_H__2203AD10_FC4B_11D4_A80E_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProfileDef.h"

class AFX_EXT_CLASS CProfile  
{
public:
	CProfile();
	virtual ~CProfile();

private:
	static	CRITICAL_SECTION m_csLock;

public:
	DEVICEINFO			DEVICE;
	BRMINFO				BRM;
	UCMINFO				UCM;
	NETWORKINFO			NETWORK;
	TRANSINFO			TRANS;
	WITHTOTALINFO		WITHTOTAL;
	DEPTOTALINFO		DEPTOTAL;
	TRANSTOTALINFO		TRANSTOTAL;
	MONEYTOTALINFO		MONEYTOTAL;
	DEPERRTOTALINFO     DEPERRTOTAL;
	WITHERRTOTALINFO    WITHERRTOTAL;
	ECASHTOTALINFO		ECASHTOTAL;			
	LCDINFO				LCD;				
	ADINFO				AD;

public:
/////////////////////////////////////////////////////////////////////////////
// 내부함수
/////////////////////////////////////////////////////////////////////////////
	int		GetPInt(char* SectName, char* KeyName, int nDef, char* FileName);
	void	GetPStr(char* SectName, char* KeyName, char sDef, void *cDest, int nSize, char* FileName);
	void	GetPByten(char* SectName, char* KeyName, BYTE *Dest, int nSize, char* FileName);
	void	PutPInt(char* SectName, char* KeyName, int Src, char* FileName);
	void	PutPStr(char* SectName, char* KeyName, void *cSrc, int nSize, char* FileName);
	void	PutPByten(char* SectName, char* KeyName, BYTE *Src, int nSize, char* FileName);

/////////////////////////////////////////////////////////////////////////////
// Device/Network/Trans
/////////////////////////////////////////////////////////////////////////////
	void	InitProfileCheck();
	void	InitProfile(int type);

	void	InitDeviceProfile();
	void	InitBRMProfile();
	void	InitUCMProfile();
	void	InitNetworkProfile();
	void	InitTransProfile();
	void	InitADProfile();

	void	GetDeviceProfile();
	void	GetBRMProfile();
	void	GetUCMProfile();
	void	GetNetworkProfile();
	void	GetTransProfile();
	void	GetADProfile();

	void	PutDeviceProfile();
	void	PutBRMProfile();
	void	PutUCMProfile();
	void	PutNetworkProfile();
	void	PutTransProfile();
	void	PutADProfile();

	void	GetLCDInfoProfile();								// U8100 AP 변경내역 #03 - 광고LCD위젯처리, 변경
	void	PutLCDInfoProfile();

/////////////////////////////////////////////////////////////////////////////
// ErrStack
/////////////////////////////////////////////////////////////////////////////
public :
	void	ErrStack(char pProcCounter, char* pErrorCode, char* pErrorString);	
																// 장애정보설정
	void	ClearAllErrStack();									// 장애정보초기화
	int		GetCrtErrStack(ERRSTACK* pEditBuff, bool pCommand);	// 현재장애정보
	int		GetFirstErrStack(ERRSTACK* pEditBuff);				// 최초장애정보
	int		GetLastErrStack(ERRSTACK* pEditBuff);				// 최종장애정보
	int		GetAllErrStack(ERRSTACK* pEditBuff);				// 전체장애정보
	int		GetDeviceErrStack(ERRSTACK* pEditBuff, int nDevID);	// 장치별장애정보	

/////////////////////////////////////////////////////////////////////////////
// ErrDeTailHost
/////////////////////////////////////////////////////////////////////////////
public :
	void	DetailHErrStack(PrintERRSTACK* pDHErrInfo);	
																// 장애정보설정
	void	DetailHClearAllErrStack();									// 장애정보초기화
	int		DetailHGetCrtErrStack(PrintERRSTACK* pEditBuff, bool pCommand);	// 현재장애정보
	int		DetailHGetFirstErrStack(PrintERRSTACK* pEditBuff);				// 최초장애정보
	int		DetailHGetLastErrStack(PrintERRSTACK* pEditBuff);				// 최종장애정보
	int		DetailHGetAllErrStack(PrintERRSTACK* pEditBuff);				// 전체장애정보
/////////////////////////////////////////////////////////////////////////////
// ErrHost
/////////////////////////////////////////////////////////////////////////////
public:
	void	ErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec);			// 서브장애정보설정
	void	ClearAllErrSub(char* MainSec, char* SubSec);						// 서브장애정보초기화
	int		GetCrtErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec);		// 서브현재장애정보
	int		GetAllErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec);		// 서브전체장애정보

	void	ErrTran(DERRINFO* DErrInfo);										// 거래장애정보설정
	void	ClearAllErrTran();													// 거래장애정보초기화
	int		GetCrtErrTran(DERRINFO* DErrInfo);									// 거래현재장애정보
	int		GetAllErrTran(DERRINFO* DErrInfo);									// 거래전체장애정보
	
	void	ErrCancel(DERRINFO* DErrInfo);										// 취소장애정보설정
	void	ClearAllErrCancel();												// 취소장애정보초기화
	int		GetCrtErrCancel(DERRINFO* DErrInfo);								// 취소현재장애정보
	int		GetAllErrCancel(DERRINFO* DErrInfo);								// 취소전체장애정보

	void	ErrHost(DERRINFO* DErrInfo);										// 호스트장애정보설정
	void	ClearAllErrHost();													// 호스트장애정보초기화
	int		GetCrtErrHost(DERRINFO* DErrInfo);									// 호스트현재장애정보
	int		GetAllErrHost(DERRINFO* DErrInfo);									// 호스트전체장애정보

	void	ErrKCash(DERRINFO* DErrInfo);										// KCash장애정보설정
	void	ClearAllErrKCash();													// KCash장애정보초기화
	int		GetCrtErrKCash(DERRINFO* DErrInfo);									// KCash현재장애정보
	int		GetAllErrKCash(DERRINFO* DErrInfo);									// KCash전체장애정보

/////////////////////////////////////////////////////////////////////////////
// Total
/////////////////////////////////////////////////////////////////////////////
public:
	void	InitWithTotal(int nClear = TRUE);									// 지급초기화
	void	InitDepTotal(int nClear = TRUE);										// 입금초기화
	void	InitTransTotal();									// 이체초기화
	void	InitDepErrTotal();									// 입금장애초기화 
	void	InitMoneyTotal();									// 방출금액초기화
	void	InitWithErrTotal();									// 입금장애초기화 
	void	GetWithTotal();										// 출금정보
	void	GetDepTotal();										// 입금정보
	void	GetTransTotal();									// 이체정보
	void	GetDepErrTotal();									// 입금장애 
	void	GetMoneyTotal();									// 실방출금액정보
	void	GetWithErrTotal();									// 
	void	PutWithTotal();										// 지급합계
	void	PutDepTotal();										// 입금합계
	void	PutTransTotal();									// 이체합계
	void	PutMoneyTotal();									// 실방출금액합계
	void	PutDepErrTotal();									// 입금장애
	void	PutWithErrTotal();										// 

/////////////////////////////////////////////////////////////////////////////
// Interactive AD
/////////////////////////////////////////////////////////////////////////////
public :
	void	RemoveAllADItem();
	int		GetCurInteractiveADPath(int nAD, CString &strReturn);
	int		GetCurInteractiveADCmd(CString &strReturn);
	int		NextInteractiveAD();

	void	MakeMainADList();
	CString	GetMainADList();
	int		InitBannerAD();
	int		LoadBannerADItem(CString strSection, BANNERADITEM *pItem);
	int		GetBannerAD(int nScreenNum, CString &strReturn);
	int		GetBannerBrandAD(int nScreenNum, CString &strReturn);
	int		GetBannerTargetAD(int nTargetOrg, CString strGubun, int nScreenNum, CString &strReturn);
	int		NextBannerAD();
	int		InitReceiptAD();
	int		GetReceiptADText(int nLine, CString &strReturn);
	int		GetReceiptADImagePath(int nScreenNum, CString &strReturn);

};

#endif // !defined(AFX_PROFILE_H__2203AD10_FC4B_11D4_A80E_00E09880ACB0__INCLUDED_)
