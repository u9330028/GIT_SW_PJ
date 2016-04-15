// Volume.h: interface for the CVolume class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VOLUME_H__E02949A1_013E_11D5_A80E_00E09880ACB0__INCLUDED_)
#define AFX_VOLUME_H__E02949A1_013E_11D5_A80E_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IVolume.h"

class AFX_EXT_CLASS CVolume : public IVolume
{
//////////////////////////////////////////////////////////////////////
// IVolume interface
//////////////////////////////////////////////////////////////////////
public:
	virtual bool	IsAvailable();
	virtual void	Enable();
	virtual void	Disable();
	virtual DWORD	GetVolumeMetric();
	virtual DWORD	GetMinimalVolume();
	virtual DWORD	GetMaximalVolume();
	virtual DWORD	GetCurrentVolume();
	virtual void	SetCurrentVolume( DWORD dwValue );

public:
	CVolume();
	virtual ~CVolume();

private:
	bool	Init();
	void	Done();

	bool	Initialize();
	void	EnableLine( bool bEnable = true );

private:
	// Status Info
	bool	m_bOK;
	bool	m_bInitialized;
	bool	m_bAvailable;

	// Mixer Info
	UINT	m_uMixerID;
	DWORD	m_dwMixerHandle;

	DWORD	m_dwLineID;
	DWORD	m_dwVolumeControlID;
	int		m_nChannelCount;
	
	DWORD	m_dwMinimalVolume;
	DWORD	m_dwMaximalVolume;
	DWORD	m_dwVolumeStep;
};

#endif // !defined(AFX_VOLUME_H__E02949A1_013E_11D5_A80E_00E09880ACB0__INCLUDED_)
