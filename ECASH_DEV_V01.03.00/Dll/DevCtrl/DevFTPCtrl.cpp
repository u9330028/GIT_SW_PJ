/////////////////////////////////////////////////////////////////////////////
//	DevDBCtrl.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "direct.h"
#include "shlwapi.h"
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Dll\DevEtc.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 연결시작및초기화
int	CDevCmn::fnFTP_Initialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_Initialize()");

	m_hInternet = InternetOpen("FTPSEVER", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!m_hInternet)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_Connection()::FTP InternetOpen 실패");
		return FALSE;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_Initialize()::return");
	return TRUE;
}

// 연결시작및초기화
int CDevCmn::fnFTP_Connection(LPCTSTR szSvrIP, LPCTSTR szLoginID, LPCTSTR szLoginPwd)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_Connection(IP[%s], ID[%s], Passwd[%s])", szSvrIP, szLoginID, szLoginPwd);

	m_hFtp = InternetConnect(m_hInternet, szSvrIP, INTERNET_DEFAULT_FTP_PORT, szLoginID, szLoginPwd, INTERNET_SERVICE_FTP, 0, 0);
	if (!m_hFtp)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_Connection()::FTP 연결 실패");
		return FALSE;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_Connection()::return");
	return TRUE;
}

// 연결종료
int	CDevCmn::fnFTP_Deinitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_Deinitialize()");

	InternetCloseHandle(m_hInternet);
	InternetCloseHandle(m_hFtp);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_Deinitialize()");
	return TRUE;
}

// 파일찾기
int CDevCmn::fnFTP_FindFirstFile(LPCTSTR szFindFile)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_FindFirstFile()");
	HINTERNET	nReturn = NULL;
	WIN32_FIND_DATA		FindData;

	nReturn = FtpFindFirstFile(m_hFtp, szFindFile, (LPWIN32_FIND_DATA)&FindData, INTERNET_FLAG_RELOAD, 0);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_FindFirstFile()");
	return T_OK;
}

// 파일가져오기
int CDevCmn::fnFTP_GetFile(LPCTSTR szSvrDir, LPCTSTR szSvrFile)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_GetFile(szSvrDir[%s], szSvrFile[%s])", szSvrDir, szSvrFile);
	int		nReturn = FALSE;
	
	nReturn = FtpGetFile(m_hFtp, szSvrDir, szSvrFile, 0, 0, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_GetFile()::return[%d]", nReturn);
	return nReturn;
}

// 파일올리기
int CDevCmn::fnFTP_PutFile(LPCTSTR szLocalFile, LPCTSTR szSvrFile)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_PutFile(szLocalFile[%s], szSvrFile[%s])", szLocalFile, szSvrFile);
	int		nReturn = FALSE;

	nReturn = FtpPutFile(m_hFtp, szLocalFile, szSvrFile, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_PutFile()::return[%d]", nReturn);
	return nReturn;
}

// 파일지우기
int CDevCmn::fnFTP_DeleteFile(LPCTSTR szFile)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_DeleteFile(szFile[%s])", szFile);
	int		nReturn = FALSE;

	nReturn = FtpDeleteFile(m_hFtp, szFile);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_DeleteFile()::return[%d]", nReturn);
	return nReturn;
}

// 파일이름바꾸기
int CDevCmn::fnFTP_RenameFile(LPCTSTR szSrcFile, LPCTSTR szDstFile)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_RenameFile(szSrcFile[%s], szDstFile[%s])", szSrcFile, szDstFile);
	int		nReturn = FALSE;

	nReturn = FtpRenameFile(m_hFtp, szSrcFile, szDstFile);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_RenameFile()::return[%d]", nReturn);
	return nReturn;
}

//파일열기
int CDevCmn::fnFTP_OpenFile(LPCTSTR szFile, int nOption)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_OpenFile(szFile[%s], nOption[%d])", szFile, nOption);
	HINTERNET	nReturn = NULL;

	nReturn = FtpOpenFile(m_hFtp, szFile, nOption, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_OpenFile()::return[%d]", nReturn);
	return T_OK;
}

// 디렉토리만들기
int CDevCmn::fnFTP_CreateDir(LPCTSTR szDir)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_CreateDir(szDir)", szDir);
	int		nReturn = FALSE;

	nReturn = FtpCreateDirectory(m_hFtp, szDir);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_CreateDir()::return[%d]", nReturn);
	return nReturn;
}

// 디렉토리지우기
int CDevCmn::fnFTP_RemoveDir(LPCTSTR szDir)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_RemoveDir(szDir)", szDir);
	int		nReturn = FALSE;

	nReturn = FtpRemoveDirectory(m_hFtp, szDir);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_RemoveDir()::return[%d]", nReturn);
	return nReturn;
}

// 현재디렉토리바꾸기
int CDevCmn::fnFTP_SetCurDir(LPCTSTR szDir)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_SetCurDir(szDir)", szDir);
	int		nReturn = FALSE;

	nReturn = FtpSetCurrentDirectory(m_hFtp, szDir);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_SetCurDir()::return[%d]", nReturn);
	return nReturn;
}

// 현재디렉토리읽어오기
int CDevCmn::fnFTP_GetCurDir(LPCTSTR szDir)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_GetCurDir(szDir)", szDir);
	int		nReturn = FALSE;
	int		nLength = 0;

	nReturn = FtpGetCurrentDirectory(m_hFtp, (char *)szDir, (unsigned long *)nLength);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnFTP_GetCurDir()::return[%d]", nReturn);
	return nReturn;
}
