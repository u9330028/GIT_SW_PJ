//////////////////////////////////////////////////////////////////////
#if !defined(_CMNLIB_H_INCLUDED_)
#define _CMNLIB_H_INCLUDED_

//////////////////////////////////////////////////////////////////////
//	ASCII
//////////////////////////////////////////////////////////////////////
#define	STRBUFFSIZE			32768

//////////////////////////////////////////////////////////////////////
//	ASCII
//////////////////////////////////////////////////////////////////////
#define A_NUL				0x00
#define A_SOH				0x01
#define A_STX				0x02
#define A_ETX				0x03
#define A_EOT				0x04
#define A_ENQ				0x05
#define A_ACK				0x06
#define A_BEL				0x07
#define A_BS				0x08
#define A_HT				0x09
#define A_LF				0x0a
#define A_VT				0x0b
#define A_FF				0x0c
#define A_CR				0x0d
#define A_SO				0x0e
#define A_SI				0x0f
#define A_DLE				0x10
#define A_DC1				0x11
#define A_DC2				0x12
#define A_DC3				0x13
#define A_DC4				0x14
#define A_NAK				0x15
#define A_SYN				0x16
#define A_ETB				0x17
#define A_CAN				0x18
#define A_EM				0x19
#define A_SUB				0x1a
#define A_ESC				0x1b
#define A_FS				0x1c
#define A_GS				0x1d
#define A_RS				0x1e
#define A_US				0x1f

//////////////////////////////////////////////////////////////////////
//	ZERO & SPACE
//////////////////////////////////////////////////////////////////////
#define ZERO				"0"
#define ZERO2				"00"
#define ZERO3				"000"
#define ZERO4				"0000"
#define ZERO5				"00000"
#define ZERO6				"000000"
#define ZERO7				"0000000"
#define ZERO8				"00000000"
#define ZERO9				"000000000"
#define ZERO10				"0000000000"
#define ZERO11				"00000000000"
#define ZERO12				"000000000000"
#define ZERO13				"0000000000000"
#define ZERO14				"00000000000000"
#define ZERO15				"000000000000000"
#define ZERO16				"0000000000000000"

#define SPACE				" "
#define SPACE2				"  "
#define SPACE3				"   "
#define SPACE4				"    "
#define SPACE5				"     "
#define SPACE6				"      "
#define SPACE7				"       "
#define SPACE8				"        "
#define SPACE9				"         "
#define SPACE10				"          "
#define SPACE11				"           "
#define SPACE12				"            "
#define SPACE13				"             "
#define SPACE14				"              "
#define SPACE15				"               "
#define SPACE16				"                "

//////////////////////////////////////////////////////////////////////
//	str
//////////////////////////////////////////////////////////////////////
#define strTRUE				_T("TRUE")
#define strFALSE			_T("FALSE")

//////////////////////////////////////////////////////////////////////
//	단축
//////////////////////////////////////////////////////////////////////
#define itemof(item)		(sizeof(item) / sizeof(item[0]))
#define StrAllZero(Str)														\
{																			\
	memset(&Str, '0', sizeof(Str) - 1);										\
	((char*)&Str)[sizeof(Str) - 1] = NULL;									\
}

#define StrAllSpace(Str)													\
{																			\
	memset(&Str, ' ', sizeof(Str));											\
	((char*)&Str)[sizeof(Str) - 1] = NULL;									\
}

#define StrAllChar(Str, Char)												\
{																			\
	memset(&Str, Char, sizeof(Str));										\
	((char*)&Str)[sizeof(Str) - 1] = NULL;									\
}

//////////////////////////////////////////////////////////////////////
// Trace Mode
//////////////////////////////////////////////////////////////////////
#define TRACE_FREE_MODE			0							// 무조건(장애)		TRACE
#define TRACE_REAL_MODE			1							// 운영(최적운영)	TRACE
#define TRACE_CODE_MODE			3							// 개발(분석운영)	TRACE
#define TRACE_PLUS_MODE			4							// 개발상세(전문분석운영)	TRACE
#define TRACE_DUMP_MODE			5							// DUMP(대량출력)	TRACE

//////////////////////////////////////////////////////////////////////
//	CodeCvt.cpp
//////////////////////////////////////////////////////////////////////
// 한글 변환 Attribute Table Define
#define	NOTCONV				0
#define	ASCEBC				1
#define	EBCASC				2	
#define	KSIBM				3
#define	IBMKS				4
#define	NBIBM				5
#define	NBKS				6
#define NBKS_PSB_DISP		7
#define NBKS_PSB_PBM		8
#define EBCASC_KNB			9
#define EBCASC_KNB_P		10
#define EBCASC_BOC			11

DllUseport	int		WINAPI CodeCvt(void* inbuffb, void* outbuffb, int length, int attr); 
DllUseport	int		WINAPI KSC2IBM2(unsigned char *src,unsigned char *dst,int len);

DllUseport	CString	WINAPI EmSpace2NormalSpace(void* inBuff, int length);
//////////////////////////////////////////////////////////////////////
//	Delay.cpp
//////////////////////////////////////////////////////////////////////
DllUseport	int		WINAPI Delay_Msg(DWORD time = 10);
DllUseport	ULONG	WINAPI TimerSet(int nSecond);
DllUseport	int		WINAPI CheckTimer(ULONG nSetTime);

//////////////////////////////////////////////////////////////////////
//	Profile.cpp
//////////////////////////////////////////////////////////////////////
DllUseport	int		WINAPI GetIniName(LPTSTR IniName, LPCTSTR strIniFileName);
DllUseport	int		WINAPI IniGetInt(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, int nDefault = NULL);
DllUseport	CString	WINAPI IniGetStr(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, LPCTSTR szDefault = NULL);

DllUseport	int		WINAPI IniSetInt(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, int nValue);
DllUseport	int		WINAPI IniSetStr(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, LPCTSTR szValue);

//////////////////////////////////////////////////////////////////////
//	Registry.cpp
//////////////////////////////////////////////////////////////////////
#define REG_BIN				REG_BINARY
#define REG_INT				REG_DWORD_LITTLE_ENDIAN
#define REG_STR				REG_SZ
#define	REG_MULTI			REG_MULTI_SZ

DllUseport	int		WINAPI RegQueryValueExt(	LPCTSTR hKeyName, 
												LPCTSTR lpszValueName, 
												DWORD dwType,
					 							DWORD dwLen, 
												LPVOID lpszData);

DllUseport	int		WINAPI RegSetValueExt(		LPCTSTR hKeyName, 
												LPCTSTR lpszValueName, 
												DWORD dwType, 
												DWORD dwLen, 
												LPVOID lpszData);

DllUseport	int		WINAPI RegGetInt(LPCTSTR hKeyName, LPCTSTR lpszValueName, int InitValue = NULL);
DllUseport	CString	WINAPI RegGetStr(LPCTSTR hKeyName, LPCTSTR lpszValueName, LPCTSTR InitValue = NULL);

DllUseport	int		WINAPI RegSetInt(LPCTSTR hKeyName, LPCTSTR lpszValueName, int SetValue);
DllUseport	int		WINAPI RegSetStr(LPCTSTR hKeyName, LPCTSTR lpszValueName, CString SetValue);

DllUseport	int		WINAPI AtmGetIpAddress(	char* szIpAddress);
DllUseport	int		WINAPI AtmGetGateWay(	char* szGateWay);
DllUseport	int		WINAPI AtmGetSubnetMask(char* szSubnetMask);

DllUseport	int		WINAPI AtmSetIpAddress(	char* szIpAddress);
DllUseport	int		WINAPI AtmSetGateWay(	char* szGateWay);
DllUseport	int		WINAPI AtmSetSubnetMask(char* szSubnetMask);

//////////////////////////////////////////////////////////////////////
//	RunExec.cpp
//////////////////////////////////////////////////////////////////////
DllUseport	int		WINAPI RunExecProgram(char* prog,BOOL bWait,char* cmd);
DllUseport	int		WINAPI WinExecWaitEnd(LPCTSTR lpCmdLind, UINT uCmdShow);

//////////////////////////////////////////////////////////////////////
//	Trace.cpp
//////////////////////////////////////////////////////////////////////
DllUseport	int		WINAPI GetDumpName(LPTSTR DumpName, LPCTSTR TraceName);
DllUseport	int		WINAPI HexDump(int nLevel, LPCTSTR strTraceFileName, LPCTSTR FileName, int LineNum, char* title, void* lpData, int len, int Code = NOTCONV);
DllUseport	int		WINAPI HexDump(int nLevel, LPCTSTR strTraceFileName, LPCTSTR strSubSystem, LPCTSTR strOperation, char* title, void* lpData, int len, int Code = NOTCONV);
DllUseport	int		WINAPI HexDumpPack(int nLevel, LPCTSTR strTraceFileName, LPCTSTR FileName, int LineNum, char* title, void* lpData, int len, int Code = NOTCONV);
DllUseport	int		WINAPI HexDumpPack(int nLevel, LPCTSTR strTraceFileName, LPCTSTR strSubSystem, LPCTSTR strOperation, char* title, void* lpData, int len, int Code = NOTCONV);
DllUseport	int		WINAPI MsgDump(int nLevel, LPCTSTR strTraceFileName, LPCTSTR FileName, int LineNum, LPCTSTR fmt,...);
DllUseport	int		WINAPI MsgDump(int nLevel, LPCTSTR strTraceFileName, LPCTSTR strSubSystem, LPCTSTR strOperation, LPCTSTR fmt,...);

DllUseport	int		WINAPI GetDumpNameAnal(LPTSTR DumpName, LPCTSTR TraceName);
DllUseport	int		WINAPI HexDumpAnal(int nLevel, LPCTSTR strTraceFileName, char* title, void* lpData, int len, int Code = NOTCONV);
DllUseport	int		WINAPI MsgDumpAnal(int nLevel, LPCTSTR strTraceFileName, LPCTSTR fmt,...);

DllUseport	int		WINAPI DeleteFiles(LPCTSTR DirName, int DeleteDays = 31);

DllUseport	int		WINAPI LogDecrypt(LPCSTR pSrc, LPSTR pDest, LPCSTR pszPassword = NULL, int nIndex = 0, HWND hWnd = NULL);


//////////////////////////////////////////////////////////////////////
//	TypeCvt.cpp
//////////////////////////////////////////////////////////////////////
DllUseport	char*	WINAPI Strcpy(void* dBuffc, void* sBuffc);
DllUseport	char*	WINAPI Strcpy(void* dBuffc, LPCTSTR sBuffc);
DllUseport	char*	WINAPI Strcpy(void* dBuffc, void* sBuffc,   int slen);
DllUseport	char*	WINAPI Strcpy(void* dBuffc, LPCTSTR sBuffc, int slen);
DllUseport	char*	WINAPI StrcpyRight(void* dBuffc, void* sBuffc,   int slen, int dlen, char InitChar = '0');
DllUseport	char*	WINAPI StrcpyRight(void* dBuffc, LPCTSTR sBuffc, int slen, int dlen, char InitChar = '0');
DllUseport	LPCTSTR WINAPI Strcpy(CString& dBuffc, void* sBuffc);
DllUseport	LPCTSTR WINAPI Strcpy(CString& dBuffc, void* sBuffc, int slen);
DllUseport	int		WINAPI Strlen(void* sBuffc);

DllUseport	char*	WINAPI AddString(void* sBuffc, void* kBuffc, int klen, void* dBuffc = NULL);
DllUseport	char*	WINAPI SubString(void* sBuffc, void* kBuffc, int klen, void* dBuffc = NULL);

DllUseport	int		WINAPI Asc2Int(void* lpvoid, int length);
DllUseport	int		WINAPI Asc2Int(LPCTSTR lpvoid, int length);
DllUseport	int		WINAPI Asc2Int(LPCTSTR lpvoid);
DllUseport	int		WINAPI Asc2IntHighOrder(LPCTSTR lpvoid, void* kBuffc, int inLen, int outLen);
DllUseport	double	WINAPI Asc2Float(void* lpvoid, int length);
DllUseport	double	WINAPI Asc2Float(LPCTSTR lpvoid, int length);
DllUseport	double	WINAPI Asc2Float(LPCTSTR lpvoid);
DllUseport	CString	WINAPI Asc2Amt(void* inbuff, int inLen,int outLen,char MarkChar = ' ');
DllUseport	CString	WINAPI Asc2Amt(LPCTSTR inbuff, int inLen,int outLen,char MarkChar = ' ');

DllUseport	CString	WINAPI Asc2String(void* lpvoid, int inLen,int outLen);
DllUseport	char*	WINAPI Int2Asc(int Value, void* Target, int len, int mode = 10, char InitChar = '0');
DllUseport	CString	WINAPI Int2Asc(int Value, int len = 0, int mode = 10, char InitChar = '0');
DllUseport  char*   WINAPI ULong2Asc(ULONG Value, void *Target, int len, int mode, char InitChar = '0');

DllUseport	int		WINAPI SplitString(CString cstrSrc, CString cstrSplit, CStringArray& acstrRet);
DllUseport	CString	WINAPI GetString(void* inbuff, int inLen,int outLen);
DllUseport	CString	WINAPI GetString2Half(void* inbuff,int inLen,int outLen);
DllUseport	int		WINAPI Compress(void* sBuff, int slen);
DllUseport	int		WINAPI EditAmount(void* inbuff,void* outbuff,int inLen,int outLen);
DllUseport	int		WINAPI MarkAmount(void* inbuff,int inLen, char Value);
DllUseport	int		WINAPI EditString(void* inBufc, int inLen, char* fmt, void* outBufc, char MoveChar = '#' , char HiddenChar = '*' , char HidenMoveChar = '*' , int HiddenFlg = TRUE);
DllUseport	int		WINAPI EditString(void* inBufc, int inLen, char* fmt, CString& outBufc, char MoveChar = '#' , char HiddenChar = '*' , char HidenMoveChar = '*' , int HiddenFlg = TRUE);
DllUseport	int		WINAPI EditString(LPCTSTR inBufc, char* fmt, CString& outBufc, char MoveChar = '#' , char HiddenChar = '*' , char HidenMoveChar = '*' , int HiddenFlg = TRUE);
DllUseport	CString	WINAPI EditString(void* inBufc, int inLen, char* fmt, char MoveChar = '#' , char HiddenChar = '*' , char HidenMoveChar = '*' , int HiddenFlg = TRUE);
DllUseport	CString	WINAPI EditString(LPCTSTR inBufc, char* fmt, char MoveChar = '#' , char HiddenChar = '*' , char HidenMoveChar = '*' , int HiddenFlg = TRUE);
DllUseport	int		WINAPI Sprintf(void* dst,int len,char* fmt,...);
DllUseport	CString	WINAPI GetSprintf(char* fmt,...);
DllUseport	int		WINAPI FindChar(void* inbuf,int len, char chr);
DllUseport	int		WINAPI FindNotNum(void* inbuf,int len);
DllUseport	int		WINAPI IsChar(void* inbuf,int len, char chr);
DllUseport	int		WINAPI IsSpace(void* inbuf,int len);
DllUseport	int		WINAPI IsZero(void* inbuf,int len);
DllUseport	int		WINAPI IsNum(void* inbuf,int len);
DllUseport	int		WINAPI IsUnPack(void* inbuf,int len);
DllUseport	int		WINAPI Zero2Space(void* sBuff, int slen);
DllUseport	int		WINAPI IsNull(void *inbuf,int len);
DllUseport	int		WINAPI Lower2Upper(void* sBuff, int slen);
DllUseport	int		WINAPI Upper2Lower(void* sBuff, int slen);
DllUseport  int		WINAPI LTrim(void *inbuf, int len, int ch);

DllUseport	void	WINAPI Byte2Asc(BYTE byteptr, void* strptrc, int mode);
DllUseport	CString	WINAPI Byte2Asc(BYTE byteptr, int mode);
DllUseport	void	WINAPI Byte2Ascn(BYTE *byteptr, void* strptrc, int blen, int mode);
DllUseport	CString	WINAPI Byte2Ascn(BYTE *byteptr, int blen, int mode);
DllUseport  int		WINAPI ByteHighOrder2Int(BYTE* Src, int inLen);
DllUseport	void	WINAPI Asc2Byte(void* ascptrc, BYTE *byteptr);
DllUseport	void	WINAPI Asc2Byten(void* ascptrc, BYTE *byteptr, int blen);

DllUseport	BYTE	WINAPI MakeBcc(void* asc, int length);
DllUseport	int		WINAPI MakePack(void* Src, void* Dest, int Slen, char OperMode = '=', int LowNibbleFlg = FALSE);
DllUseport	int		WINAPI MakeUnPack(void* Src, void* Dest, int Slen, int AscFlg = TRUE);
DllUseport	int		WINAPI MakeUnPack(void* Src, CString& Dest, int Slen, int AscFlg = TRUE);
DllUseport	CString	WINAPI MakeUnPack(void* Src, int Slen, int AscFlg = TRUE);

DllUseport	void	WINAPI GetDate(void* gDate);
DllUseport	CString	WINAPI GetDate();
DllUseport	void	WINAPI GetTime(void* gTime);
DllUseport	CString	WINAPI GetTime();
DllUseport	void	WINAPI GetDateTime(void* gDate, void* gTime);
DllUseport	void	WINAPI PutDate(void* pDate);
DllUseport	void	WINAPI PutDate(LPCTSTR pDate);
DllUseport	void	WINAPI PutTime(void* pTime);
DllUseport	void	WINAPI PutTime(LPCTSTR pTime);
DllUseport	void	WINAPI PutDateTime(void* pDate, void* pTime);
DllUseport	void	WINAPI PutDateTime(LPCTSTR pDate, LPCTSTR pTime);

DllUseport	BYTE	WINAPI A2E(BYTE Src);
DllUseport	CString	WINAPI A2E(void* vSrc, int Length);
DllUseport	CString	WINAPI A2E(LPCTSTR vSrc, int Length);
DllUseport	CString	WINAPI A2E(void* vSrc);
DllUseport	CString	WINAPI A2E(LPCTSTR vSrc);
DllUseport	BYTE	WINAPI E2A(BYTE Src);
DllUseport	CString	WINAPI E2A(void* vSrc, int Length);
DllUseport	CString	WINAPI E2A(LPCTSTR vSrc, int Length);
DllUseport	CString	WINAPI E2A(void* vSrc);
DllUseport	CString	WINAPI E2A(LPCTSTR vSrc);

DllUseport	int		WINAPI JuminCheck(void* inbuf);
DllUseport	int		WINAPI JuminCheck(LPCTSTR inbuf);
DllUseport	int		WINAPI AccountCheck(void* inbuf);
DllUseport	int		WINAPI AccountCheck(LPCTSTR inbuf);

DllUseport	int		WINAPI OpenShareData();
DllUseport	int		WINAPI CloseShareData();
DllUseport	int		WINAPI ClearShareData();
DllUseport	CString WINAPI GetShareData(CString szGetDataName);
DllUseport	int		WINAPI SetShareData(CString szSetDataName, CString szSetDataValue);

//////////////////////////////////////////////////////////////////////
#endif
