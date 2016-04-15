/*
	PowerTTS Header

	Copyright (C) 2001-2004 HCI Lab
*/

#define	MAX_SPEAKER		10			// 최대 화자수

#define	SAMPLE_8K		8000
#define SAMPLE_16K		16000

#define	E_PTTS_FILEOPEN			-1	// FILE OPEN 에러
#define	E_PTTS_LACKOFMEMORY		-2	// Working Memory 부족
#define E_PTTS_TTSTHREADFAIL	-3	// TTS Thread Generation 실패
#define E_PTTS_DATEEXPIRED		-4	// 사용 가능일 만료
#define E_PTTS_ENGINELOADFAIL   -5  // 엔진 로드 실패
#define E_PTTS_INVALIDPARAM		-6	// invalid parameter 사용
#define E_PTTS_INVALIDSPEAKER	-7	// invalid speaker 사용	(add at 2003-12-30 by hslee)

#define E_PTTS_INVALIDCHANNEL	-8	// 유효하지 않은 채널임
#define E_PTTS_NOMORECHANNEL	-9	// 허용채널초과
#define E_PTTS_NOENGINELOAD		-10	// 엔진이 로드되어 있지 않음
#define E_PTTS_OUTOFRANGE		-11	// Specified argument is out of range

#define E_PTTS_CHARSETMISMATCH	-21	// Character Set이 맞지 않는 경우
#define E_PTTS_VXMLTAGERROR		-22	// VXML TAG가 잘못된 경우

#define E_PTTS_FORMATMISMATCH	-31	// AudioMixing에 사용되는 두 개의 파일의 Format이 서로 다른 경우

#define E_PTTS_LICENSE_KEY_NOT_FOUND		-101
#define E_PTTS_LICENSE_DATE_EXPIRED			-102
#define E_PTTS_LICENSE_INVALID_SYSTEM		-103
#define E_PTTS_LICENSE_INVALID_KEY			-104

#define CODING_PCM				0	// TextToFile()의 Format
#define CODING_LINEAR8			1
#define CODING_MULAW			2
#define CODING_ALAW				3
#define CODING_VOX				4
#define CODING_ADPCM			5	// 4-bit CCITT g.721 ADPCM
#define CODING_WAVE				6

#define PTTS_FORMAT_WAV		    0	// TextToFileEx()의 Format
#define PTTS_FORMAT_AU			1
#define PTTS_FORMAT_PCM			2

#define PTTS_ENCODING_LINEAR16	0	// TextToFileEx()의 Encoding 
#define PTTS_ENCODING_LINEAR8	1	 
#define PTTS_ENCODING_MULAW		2
#define PTTS_ENCODING_ALAW		3
#define PTTS_ENCODING_VOX		4
#define PTTS_ENCODING_ADPCM		5	// 4-bit CCITT g.721 ADPCM

#define	PTTS_PLAY_START			1		// SOUND CARD API 에서
#define	PTTS_PLAY_PAUSE			2		// 합성기가 사용자에게 uUsrMsg 와 함께 wParam 에 주는 정보
#define PTTS_PLAY_RESTART		3
#define PTTS_PLAY_END			4
#define	PTTS_PLAY_ERROR			-1

#define	E_PTTS_USRDERR_LOAD			11	// 이미 사전이 로드되어 있는 경우
#define	E_PTTS_USRDERR_FILEOPEN		12	// 사용자 사전이 Open 되지 않는 경우
#define	E_PTTS_USRDERR_MEMALLOC		13	// 메모리가 없는 경우
#define	E_PTTS_USRDERR_INVALIDPARA	14	// 함수의 parameter 값이 정상적이지 않은 경우
#define	E_PTTS_USRDERR_ENTRYNUM		15	// 허용 Entry수 = MAX_USRDICT을 넘는 경우
#define	E_PTTS_USRDERR_BADFORMAT	16	// '{원단어리스트} {새단어리스트}' 형식이 아닌 경우
#define	E_PTTS_USRDERR_BADORGWORD1	17	// 원단어리스트가 USRDICT_WORDLIST_LEN(128)의 길이를 넘는 경우
#define	E_PTTS_USRDERR_BADORGWORD2	18	// 원단어리스트내 단어가 USRDICT_WORD_LEN(40)의 길이를 넘는 경우
#define	E_PTTS_USRDERR_BADORGWORD3	19	// 원단어리스트내 다른 에러가 있는 경우
#define	E_PTTS_USRDERR_BADNEWWORD1	20	// 새단어리스트가 USRDICT_WORDLIST_LEN(128)의 길이를 넘는 경우
#define	E_PTTS_USRDERR_BADNEWWORD2	21	// 새단어리스트내 단어가 USRDICT_WORD_LEN(40)의 길이를 넘는 경우
#define	E_PTTS_USRDERR_BADNEWWORD3	22	// 새단어리스트내 다른 에러가 있는 경우

#define PHONE_TYPE_DEFAULT	0	// 립정보-폰단위 일때
#define PHONE_TYPE_CVC		1	// 립정보-음절단위 일때, 음절구조
#define PHONE_TYPE_CV		2
#define PHONE_TYPE_VC		3
#define PHONE_TYPE_V		4
#define PHONE_TYPE_PAUSE	5

typedef struct tagLIPSYNCINFO {
	int     phType;				// 폰 타입
	char	phone[3];			// 폰
	char	bComplete;			// 완료 여부 : OutBufSize != 0 일때, [1]complete [0]not-complete (default=1)
	int		nStPos;				// 시작 위치 : sample 단위
	int		nDur;				// 폰 길이   : sample 단위
} LIPSYNCINFO;

typedef struct tagHLIGHTINFO {
	int		nTextPos_Start;
	int		nTextPos_End;
} HLIGHTINFO;

typedef struct tagMARKINFO {
	int			nTotalPhoneNum;		// 전체 폰 수
	int			nCurrentPhoneNum;	// 현재 폰 수
	LIPSYNCINFO	*LipSyncData;		// 립싱크 정보
	HLIGHTINFO	HLight;				// Highlight 정보
} MARKINFO;

typedef struct tagSYNRES {			// 합성기에서 만들어내는 결과 [사용자정의 callback에서 사용]
	unsigned int   NumBytes;		// 합성음의 크기
	void        *pData;				// 합성음
	void        *pReserved;			//
	short		Status;				// Buffer의 상태를 나타냄 (0:Pcm Buffer 생성중, 1:사용자가 선택한 TTS_READ_MODE의 끝, 2:전체 텍스트 합성 종료, 3:STOP 명령에 의한 종료)
} SYNRES;

typedef struct {
	char	szProduct[40];
	char	szVersion[40];
	char	szLanguage[20];
	char	chCodingScheme;
	char	szAPIVersion[40];
} PowerTTSENGINEINFO;

typedef struct {
	int		nSpeakerID;
	char	szSpeaker[20];
	char	szLanguage[20];
	int		nSamplingRate;
} PowerTTSDBINFO;

typedef struct tagFILTERBUF 
{
	short sSource[11];
	float fFltBuf[11];
} FILTERBUF;

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*/
/* Basic API */
int  PTTS_LoadEngine(char *DbDir, int bLoadInMemory);
void PTTS_UnLoadEngine(void);

void *PTTS_CreateThread(void *pInParam, int (*CallBack)(void *pInParam, SYNRES *pResult), int Speaker);
void PTTS_DeleteThread(void *pTTSThread);
int  PTTS_TextToSpeech(void *pTTSThread, char *Text, int bTaggedText);
int  PTTS_TextToPcmBuf(void *pTTSThread, char *Text, int bTaggedText, int OutBufSize);
int  PTTS_StopTextToSpeech(void *pTTSThread);
int  PTTS_ChangeAttr(void *pTTSThread, char *tagString);

/* Extension Functions */
int PTTS_GetPitchDefault(int speaker, int *Min, int *Max);
int PTTS_GetVolumeDefault(int *Min, int *Max);
int PTTS_GetSpeedDefault(int *Min, int *Max);
int PTTS_ChangeSpeaker(void *pTTSThread, int Speaker);
void PTTS_GetEngineInfo(PowerTTSENGINEINFO *st_TTSEngineInfo);
int PTTS_SetSpeed(void *pTTSThread, int Speed);
int PTTS_SetVolume(void *pTTSThread, int Volume);
int	PTTS_SetPitch(void *pTTSThread, int Pitch);

/* Extension Functions 2 for New Server*/
void PTTS_GetChannelInfo(int *UsingCh, int *MaxCh);
int PTTS_MakeWaveHeader(int nEncoding, int nSamplingRate, int nByteNum, char *szHeader);
int PTTS_MakeAuHeader(int nEncoding, int nSamplingRate, int nByteNum, char *szHeader);
void PTTS_GetDBInfo(int SpeakerID, PowerTTSDBINFO *st_TTSDBInfo);
int PTTS_GetError();

/*===========================================================================*/
/* File API */
// Speaker 변화를 위한 변수 추가 (2003-09-15)
int PTTS_TextToFile(int Speaker, int Format, char *Text, char *FileName, char *tagString, int SamplingRate);

// TextToFile() 함수 확장 : format / encoding / sampling rate 별 저장 (2003-12-12)
int PTTS_TextToFileEx(int Speaker, int Format, int Encoding, int SamplingRate,
					  char *TextBuf, char *OutFileName, char *tagString);

// TextToFileEx() 함수 확장 : 사용자 사전 지원 (2004-04-21)
int PTTS_TextToFileEx2(int Speaker, int Format, int Encoding, int SamplingRate,
					  char *TextBuf, char *OutFileName, char *tagString, char *UserDictFileName);

/*===========================================================================*/
int  PTTS_NameToSpeech(void *pTTSThread, char *Text, int bTaggedText);
int  PTTS_NameToPcmBuf(void *pTTSThread, char *Text, int bTaggedText, int OutBufSize);
int  PTTS_NameToFile(int Speaker, int Format, int Encoding, int SamplingRate,
					   char *TextBuf, char *OutFileName, char *tagString);
/**********/

/*===========================================================================*/
/* 추가 설정 함수 */
int PTTS_SetTextFormat(void *pTTSThread, char chTextFormat);	// 0 : naormal text(default),	1 : VXML
int PTTS_SetCharSet(void *pTTSThread, char chCharSet);			// 0 : KSC-5601+확장완성형 (default), 1: UTF-8
int PTTS_SetLipSync(void *pTTSThread, char chMode);			// 0 : Off (default), 1: On
int PTTS_SetHighLight(void *pTTSThread, char chMode);			// 0 : Off (default), 1: On
int PTTS_SetByteSwap(void *pTTSThread, char chMode);			// 0 : Off (default), 1: On

int PTTS_AudioMixer(char *in_Audio1, char *in_Audio2, char *out_Audio, int SamplingRate, int Format, int Encoding, int nMode);

int PTTS_SMSToSpeech(void *pTTSThread, char *Text, int bTaggedText);
int PTTS_SMSToPcmBuf(void *pTTSThread, char *Text, int bTaggedText, int OutBufSize);
int PTTS_SMSToFile(int Speaker, int Format, int Encoding, int SamplingRate,
					   char *TextBuf, char *OutFileName, char *tagString);

/*===========================================================================*/
/* SOUND CARD API */
#if defined(WIN32)
int PTTS_PlayTTS(HWND hUsrWnd, UINT uUsrMsg, char *TextBuf, char *tagString, int Speaker);
int PTTS_StopTTS(void);
int PTTS_PauseTTS(void);
int PTTS_RestartTTS(void);
#endif

/* Encoding Functions */
int PTTS_uLawEnCoding(int NumData, short *Data, unsigned char *OutBuf);
int PTTS_aLawEnCoding(int NumData, short *Data, unsigned char *OutBuf);
int PTTS_ADPCMEnCoding(int NumData, short *Data, unsigned char *OutBuf);
int PTTS_ADPCMDeCoding(int NumData, unsigned char *Data, short *OutBuf);
int PTTS_VOXADPCMEnCoding(int NumData,short *Data,unsigned char *OutBuf);
int PTTS_ConvSamplingRate(int NumData, short *Data, short *OutBuf, int SrcSRate, int DestSRate);
//int PTTS_ConvSamplingRateBuf(int NumData, short *Data, short *OutBuf, int nFlag, FILTERBUF *st_FltBuf);
int PTTS_ConvSamplingRateBuf(int NumData, short *Data, short *OutBuf, int SrcSRate, int DestSRate, int nFlag, FILTERBUF *st_FltBuf);
int PTTS_Linear8EnCoding(int NumData, short *Data, unsigned char *OutBuf);
int PTTS_Linear8UnsignedEnCoding(int NumData, short *Data, unsigned char *OutBuf);

int PTTS_LoadUserDict(void *pTTSThread, char *UsrDictFileName, int *nErrorType, int *nErrorLine);
void PTTS_UnLoadUserDict(void *pTTSThread);
int PTTS_ChangeUserDict(void *pTTSThread, char *UsrDictFileName, int *nErrorType, int *nErrorLine);

void PTTS_RegisterUserDict(char *UsrDictFileName);
void PTTS_DeleteUserDict();

#ifdef __cplusplus
};
#endif
