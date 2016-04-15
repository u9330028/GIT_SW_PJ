/////////////////////////////////////////////////////////////////////////////
//	DevSnd.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
/* ──────────────────────────────────────
   ▷ 파일변경이력
  ┌─────┬─────┬────┬────────────────────┐
  │A P 버 전 │ 변경일자 │항목번호│  변경내용(사유)                        │
  └─────┴─────┴────┴────────────────────┘
    V03-01-01   2005.07.12            SPL키 음 처리를 위하여 함수추가처리
									   * 적용함수 : fnAPL_SplKeySound
   ──────────────────────────────────────*/
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "direct.h"
#include "shlwapi.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "mmsystem.h"

#include "..\..\H\Dll\DevEtc.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"

#pragma comment( lib, "Winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	음성처리 : Key Sound
/////////////////////////////////////////////////////////////////////////////
int CDevCmn::fnSND_KeySound(LPCTSTR fname)
{
	int flag = SND_FILENAME;
	flag |= SND_ASYNC;

	CString strSndFile("");
	strSndFile.Empty();

	strSndFile.Format("%s\\%s", _SOUND_DIR, (fname == NULL) ? _SOUND_DDING : fname);

	return sndPlaySound(strSndFile, flag);     //V01-01-02-#06COM07-17 
}

/////////////////////////////////////////////////////////////////////////////
//	SOUND함수(SND) : VOL SET MODE
/////////////////////////////////////////////////////////////////////////////
// 볼륨얻어오기
int CDevCmn::fnSND_SetCurrentVolume(int nVolume)
{
	m_pSnd->SetCurrentVolume(nVolume);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	SOUND함수(SND) : VOL GET MODE
/////////////////////////////////////////////////////////////////////////////
// 볼륨조정
int CDevCmn::fnSND_GetCurrentVolume()
{
	int		nReturn = 0;

	nReturn = m_pSnd->GetCurrentVolume();

	return nReturn;
}
