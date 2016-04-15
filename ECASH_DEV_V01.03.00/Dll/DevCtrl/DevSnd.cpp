/////////////////////////////////////////////////////////////////////////////
//	DevSnd.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
/* ����������������������������������������������������������������������������
   �� ���Ϻ����̷�
  ������������������������������������������������������������������������������
  ��A P �� �� �� �������� ���׸��ȣ��  ���泻��(����)                        ��
  ������������������������������������������������������������������������������
    V03-01-01   2005.07.12            SPLŰ �� ó���� ���Ͽ� �Լ��߰�ó��
									   * �����Լ� : fnAPL_SplKeySound
   ����������������������������������������������������������������������������*/
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
//	����ó�� : Key Sound
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
//	SOUND�Լ�(SND) : VOL SET MODE
/////////////////////////////////////////////////////////////////////////////
// ����������
int CDevCmn::fnSND_SetCurrentVolume(int nVolume)
{
	m_pSnd->SetCurrentVolume(nVolume);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	SOUND�Լ�(SND) : VOL GET MODE
/////////////////////////////////////////////////////////////////////////////
// ��������
int CDevCmn::fnSND_GetCurrentVolume()
{
	int		nReturn = 0;

	nReturn = m_pSnd->GetCurrentVolume();

	return nReturn;
}
