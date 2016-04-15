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
// �����Լ�
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

	void	GetLCDInfoProfile();								// U8100 AP ���泻�� #03 - ����LCD����ó��, ����
	void	PutLCDInfoProfile();

/////////////////////////////////////////////////////////////////////////////
// ErrStack
/////////////////////////////////////////////////////////////////////////////
public :
	void	ErrStack(char pProcCounter, char* pErrorCode, char* pErrorString);	
																// �����������
	void	ClearAllErrStack();									// ��������ʱ�ȭ
	int		GetCrtErrStack(ERRSTACK* pEditBuff, bool pCommand);	// �����������
	int		GetFirstErrStack(ERRSTACK* pEditBuff);				// �����������
	int		GetLastErrStack(ERRSTACK* pEditBuff);				// �����������
	int		GetAllErrStack(ERRSTACK* pEditBuff);				// ��ü�������
	int		GetDeviceErrStack(ERRSTACK* pEditBuff, int nDevID);	// ��ġ���������	

/////////////////////////////////////////////////////////////////////////////
// ErrDeTailHost
/////////////////////////////////////////////////////////////////////////////
public :
	void	DetailHErrStack(PrintERRSTACK* pDHErrInfo);	
																// �����������
	void	DetailHClearAllErrStack();									// ��������ʱ�ȭ
	int		DetailHGetCrtErrStack(PrintERRSTACK* pEditBuff, bool pCommand);	// �����������
	int		DetailHGetFirstErrStack(PrintERRSTACK* pEditBuff);				// �����������
	int		DetailHGetLastErrStack(PrintERRSTACK* pEditBuff);				// �����������
	int		DetailHGetAllErrStack(PrintERRSTACK* pEditBuff);				// ��ü�������
/////////////////////////////////////////////////////////////////////////////
// ErrHost
/////////////////////////////////////////////////////////////////////////////
public:
	void	ErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec);			// ���������������
	void	ClearAllErrSub(char* MainSec, char* SubSec);						// ������������ʱ�ȭ
	int		GetCrtErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec);		// ���������������
	int		GetAllErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec);		// ������ü�������

	void	ErrTran(DERRINFO* DErrInfo);										// �ŷ������������
	void	ClearAllErrTran();													// �ŷ���������ʱ�ȭ
	int		GetCrtErrTran(DERRINFO* DErrInfo);									// �ŷ������������
	int		GetAllErrTran(DERRINFO* DErrInfo);									// �ŷ���ü�������
	
	void	ErrCancel(DERRINFO* DErrInfo);										// ��������������
	void	ClearAllErrCancel();												// �����������ʱ�ȭ
	int		GetCrtErrCancel(DERRINFO* DErrInfo);								// ��������������
	int		GetAllErrCancel(DERRINFO* DErrInfo);								// �����ü�������

	void	ErrHost(DERRINFO* DErrInfo);										// ȣ��Ʈ�����������
	void	ClearAllErrHost();													// ȣ��Ʈ��������ʱ�ȭ
	int		GetCrtErrHost(DERRINFO* DErrInfo);									// ȣ��Ʈ�����������
	int		GetAllErrHost(DERRINFO* DErrInfo);									// ȣ��Ʈ��ü�������

	void	ErrKCash(DERRINFO* DErrInfo);										// KCash�����������
	void	ClearAllErrKCash();													// KCash��������ʱ�ȭ
	int		GetCrtErrKCash(DERRINFO* DErrInfo);									// KCash�����������
	int		GetAllErrKCash(DERRINFO* DErrInfo);									// KCash��ü�������

/////////////////////////////////////////////////////////////////////////////
// Total
/////////////////////////////////////////////////////////////////////////////
public:
	void	InitWithTotal(int nClear = TRUE);									// �����ʱ�ȭ
	void	InitDepTotal(int nClear = TRUE);										// �Ա��ʱ�ȭ
	void	InitTransTotal();									// ��ü�ʱ�ȭ
	void	InitDepErrTotal();									// �Ա�����ʱ�ȭ 
	void	InitMoneyTotal();									// ����ݾ��ʱ�ȭ
	void	InitWithErrTotal();									// �Ա�����ʱ�ȭ 
	void	GetWithTotal();										// �������
	void	GetDepTotal();										// �Ա�����
	void	GetTransTotal();									// ��ü����
	void	GetDepErrTotal();									// �Ա���� 
	void	GetMoneyTotal();									// �ǹ���ݾ�����
	void	GetWithErrTotal();									// 
	void	PutWithTotal();										// �����հ�
	void	PutDepTotal();										// �Ա��հ�
	void	PutTransTotal();									// ��ü�հ�
	void	PutMoneyTotal();									// �ǹ���ݾ��հ�
	void	PutDepErrTotal();									// �Ա����
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
