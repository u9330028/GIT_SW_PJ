/******************************************************************************
 *	File Name	:	DataFile.h												  *
 *	Description	:   The CDataFile class is used to create and modify    	  *	
 *					data files such as *.ini and *.cfg.						  *
 *	Creator		:   Laughsky(LiQintao)										  *
 *  Create time	:	2001/9/25												  *
 *	Bug Report	:	laugh_sky@hotmail.com || muzilee@263.net				  *
 ******************************************************************************/
//////////////////////////////////////////////////////////////////////

#if !defined(_DevDataFile_h_include_)
#define _DevDataFile_h_include_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////
// Kim.G.J 2008.05.09 File Access Function
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
#define FILE_MAX_LENGTH			1024*50
#define CREATE_MODE_REBUILD		CFile::modeCreate
#define CREATE_MODE_NOREBUILD	CFile::modeNoTruncate | CFile::modeCreate

#define OPEN_MODE_READ			CFile::modeRead
#define OPEN_MODE_WRITE			CFile::modeReadWrite
#define OPEN_MODE_READWRITE		CFile::modeReadWrite

#define	WRITE_MODE_INSERT		2000
#define	WRITE_MODE_OVERWRITE	2001
#define	WRITE_MODE_DELETE		2002

#define FLAG_FILE_EXIST		3000
#define FLAG_FILE_NOEXIST	3001

public:
	int	InitDataFile();
	void DeleteFile(LPCTSTR lpszFileName);
	void WriteLine(CString& wString, UINT nFlag);
	BOOL MoveToLine(UINT line);

	UINT GetLines();
	void CloseFile();
	BOOL OpenFile(LPCTSTR lpszFileName, UINT nFlags);
	INT CheckFileExsist(LPCTSTR lpszFileName);
	BOOL ReadLine(CString& rString);
	BOOL CreateFile(LPCTSTR lpszFileName, UINT nFlags);


	void MoveEnd();
	void MoveStart();
	CString FileName;	// Save the specified file name
	UINT moveLine;		// Save the number of line at current pointer position 
	BOOL IsFileOpen;	// If file has been opened,it is TRUE
	CStdioFile file;

#endif // !defined(_DevDataFile_h_include_)
