#include "StdAfx.h"
#include "..\..\H\Dll\PowerTTS.h" //#N0193-01
#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\Define.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"
#include "..\..\H\Dll\DevCmn.h"

FILE	*fptts;
int default_speaker;

DBINFO st_DBInfo[MAX_SPEAKER];
int GetDBInfo(DBINFO *stDBInfo);

int		DEFSamplingRate[2] = { 
	16000,
	48000
};

char *StrFind(char *pResult, FILE *pFile, char *pTargetString, char *pTargetData)
{
MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "StrFind");
	long   lCurFileLoc = ftell(pFile);
	int    nFileSize = 0;

	fseek(pFile, 0L, 0);
	do {
		nFileSize = fscanf(pFile, "%.128s", pResult);
		if (!strcmp(pResult, pTargetString)) {
			fscanf(pFile, "%.128s", pResult);				
			fseek(pFile, lCurFileLoc, 0);
			return pResult;
		}
	} while (nFileSize>0);

	strcpy(pResult, pTargetData);
MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "StrFind return");

	return pResult;
}

int GetDBInfo(DBINFO *stDBInfo)
{
	MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "GetDBInfo");
	char	szTemp[128] = {0x00, };
	char	Dir[128] = {0x00, };
	char	*pdest;
	int		nI, result, nLen, ch='.';
	FILE	*fpFile;
	
	char DirBuf[256];	/* 256 or 260 */
	
	memset(szTemp, 0x00, sizeof(szTemp));		// 2011.04.25 CodeSonar Detected (Uninitialized Variable)
	memset(Dir, 0x00, sizeof(Dir));				// 2011.04.25 CodeSonar Detected (Uninitialized Variable)
	memset(DirBuf, 0x00, sizeof(DirBuf));		// 2011.04.25 CodeSonar Detected (Uninitialized Variable)

// V04.05.07.#3 장애우ATM 2007.11.16 by KTY START
	LPCTSTR DirName = "C:\\Program files\\HCILAB\\PowerTTS-S";
	CFileFind	finder;

	CString dir, file;
	dir.Format("%s\\*.*",DirName);

	BOOL bWorking = finder.FindFile(dir);

	if(bWorking)
		;
	else
		return 1;

	finder.Close();
// V04.05.07.#3 장애우ATM 2007.11.16 by KTY END


	// 0> Default
	strcpy(DirBuf, ".\\");
	
	// 1> INI 에서 합성기 Install 정보 읽기
	fpFile  = fopen("c:\\T1Atm\\INI\\TTSDemo.ini", "rt");
	if (fpFile)
	{
		fclose(fpFile);
		GetPrivateProfileString("TTS", "DBDir",   ".\\",   DirBuf, 255, "c:\\T1Atm\\INI\\TTSDemo.ini");
	}
	MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "GetDBInfo : DirBuf[%s]", DirBuf);
	
	/* 화자별 config 파일 체크 */
	for(nI = 0; nI < MAX_SPEAKER; nI++) {
#ifdef	WIN32
		/* by okeoke : test (config.이름 변경) */
		sprintf(Dir, "%sconfig/config.%d", DirBuf, nI);
#else
		sprintf(Dir, "config.%d", i);
#endif
		if((fpFile=fopen(Dir, "rt"))!=NULL)
		{
			StrFind(szTemp, fpFile, "FILE_ENTRYa", ""); /*INDEX_NAME*/
			pdest = strchr( szTemp, ch );
			result = pdest - szTemp + 1;
			if( pdest != NULL )
			{
				nLen = strlen(szTemp);
				strncpy(stDBInfo[nI].szName, szTemp+result, nLen-result);
			}
			fclose(fpFile);
			stDBInfo[nI].nLoad=1;
		}
		else
			stDBInfo[nI].nLoad = 0;
	}	
	MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "GetDBInfo return");
	
	return 1;
}

int PCMCallBack(void *pInParam,SYNRES *pSynRes)
{
MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "PCMCallBack");
//	fptts = (FILE*)pInParam;
	
	if(pSynRes->NumBytes>0) 
	{
		fwrite(pSynRes->pData, 1, pSynRes->NumBytes, fptts);
	}
MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "PCMCallBack return");

	return 0; /* Return OK */
}


// Codesonar Detected 쓰는데가 없어서 주석처리함	// V04.09.06.#8
/*
char *ReadFile(char *Filename, int *size)
{
MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "ReadFile");
	FILE	*fp;
	int     FileSize;
	char	*pBuffer;

	// Open File
	if ((fp = fopen(Filename, "rb")) == NULL)
		return NULL;

	// Set File Size
	fseek(fp, 0, SEEK_END);
	FileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Allocate Memory
	pBuffer = (char *)malloc(FileSize + 1);
	if (pBuffer == NULL)
		return NULL;

	// Read Data
	fread(pBuffer, 1, FileSize, fp);
	fclose(fp);

	pBuffer[FileSize] = '\0';
	*size = FileSize;
MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "ReadFile return");

	return pBuffer;
}
*/

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.

void CDevCmn::TTS_initialize()
{
//	m_nDB = 0;
//	m_nSRate = 0;
//	m_nSpeaker = 0;
//	m_bPaused = FALSE;
MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "TTS_initialize");

	int nI;

	/////////////////////////////////////////////////////////////////////
	// Loading TTS Engine's DB (16KHz, DiskAccess)
	char DBDirBuf[256];
	GetPrivateProfileString("TTS", "DBDir",   ".\\",   DBDirBuf, 255, "c:\\T1Atm\\INI\\TTSDemo.ini");

	if(DBDirBuf[strlen(DBDirBuf)-1]!='\\') strcat(DBDirBuf,"\\");

	GetDBInfo(st_DBInfo);

	int load_speaker_num = 0;
	int load_speaker[MAX_SPEAKER];

	for(nI=0; nI<MAX_SPEAKER; nI++) {
		if(st_DBInfo[nI].nLoad==1) 
		{
			load_speaker[nI] = nI;
			load_speaker_num++;
		}
		else
			load_speaker[nI] = 0;
	}

	if(load_speaker_num == 1)
	{
		for(nI=0; nI<MAX_SPEAKER; nI++) {
			if(load_speaker[nI] != 0)
			{
				default_speaker = nI;
				break;
			}
		}
	}

	else
	{
		for(nI=0; nI<MAX_SPEAKER; nI+=2) {
			if(load_speaker[nI] != 0)
			{
				default_speaker = nI;
				break;
			}
		}
		for(nI=1; nI<MAX_SPEAKER; nI+=2) {
			if(load_speaker[nI] != 0)
			{
				default_speaker = nI;
				break;
			}
		}
	}

HexDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "DBDirBuf", DBDirBuf, sizeof(DBDirBuf));
	// Loading TTS Engine's DB (16KHz, DiskAccess)
	if (int rr = PTTS_LoadEngine(DBDirBuf, 0))
		MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "TTS DB Loading Error! [%d]", rr);
	else														// V05-08-01-#07
	{
		MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "TTSEngineLoadFlg : TRUE");
		TTSEngineLoadFlg = TRUE;
	}

// 	pTTS=PTTS_CreateThread(NULL, PCMCallBack, default_speaker);
// 	if(!pTTS)
// 	{
// 		MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "PTTS_CreateThread Error");
// 	}

MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "TTS_initialize return");
}
void CDevCmn::fnAPL_TTSplay() 
{
MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "fnAPL_TTSplay : m_pText[%s]", m_pText);
	// [HCI Lab] Save Speech-File
	int res;
	CString csEmsg;
	int m_nSpeaker;
	
	m_nSpeaker = 6;

	res = PTTS_TextToFileEx(m_nSpeaker, PTTS_FORMAT_WAV, PTTS_ENCODING_LINEAR16, SAMPLE_16K,
							  m_pText, "d:\\test.wav", NULL);
	
	Delay_Msg(300);    //#N0228 WIN7 지원 장애인 거래시 음성 미출력 대책
	
	if (res)
	{
		if (res == E_PTTS_FILEOPEN)
		{
			csEmsg.Format("File Open Error : %s", "test.wave");
MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "fnAPL_TTSplay %s", csEmsg);
		}
	}
	else 
PlaySound("d:\\test.wav",NULL,SND_ASYNC|SND_NOSTOP);

MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "fnAPL_TTSPlay PlaySound");

	return ;

}


void CDevCmn::fnAPL_TTSstop()
{
	
	MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "MyWaveOutClose");

	Delay_Msg(300);    //#N0228 WIN7 지원 장애인 거래시 음성 미출력 대책

	PlaySound (NULL, 0, SND_ASYNC);

	return;
}
void CDevCmn::TTS_StartProc()
{
MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "TTS_StartProc");
	TTS_initialize();
//	GetDBInfo(st_DBInfo);
}

// V05-08-01-#07
void CDevCmn::TTS_EndProc()
{
	MsgDump(TRACE_CODE_MODE, "phone", __FILE__, __LINE__, "TTS_EndProc");
	PTTS_UnLoadEngine();
}


