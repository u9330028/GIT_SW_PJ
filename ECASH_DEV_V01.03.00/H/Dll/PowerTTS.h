/*
	PowerTTS Header

	Copyright (C) 2001-2004 HCI Lab
*/

#define	MAX_SPEAKER		10			// �ִ� ȭ�ڼ�

#define	SAMPLE_8K		8000
#define SAMPLE_16K		16000

#define	E_PTTS_FILEOPEN			-1	// FILE OPEN ����
#define	E_PTTS_LACKOFMEMORY		-2	// Working Memory ����
#define E_PTTS_TTSTHREADFAIL	-3	// TTS Thread Generation ����
#define E_PTTS_DATEEXPIRED		-4	// ��� ������ ����
#define E_PTTS_ENGINELOADFAIL   -5  // ���� �ε� ����
#define E_PTTS_INVALIDPARAM		-6	// invalid parameter ���
#define E_PTTS_INVALIDSPEAKER	-7	// invalid speaker ���	(add at 2003-12-30 by hslee)

#define E_PTTS_INVALIDCHANNEL	-8	// ��ȿ���� ���� ä����
#define E_PTTS_NOMORECHANNEL	-9	// ���ä���ʰ�
#define E_PTTS_NOENGINELOAD		-10	// ������ �ε�Ǿ� ���� ����
#define E_PTTS_OUTOFRANGE		-11	// Specified argument is out of range

#define E_PTTS_CHARSETMISMATCH	-21	// Character Set�� ���� �ʴ� ���
#define E_PTTS_VXMLTAGERROR		-22	// VXML TAG�� �߸��� ���

#define E_PTTS_FORMATMISMATCH	-31	// AudioMixing�� ���Ǵ� �� ���� ������ Format�� ���� �ٸ� ���

#define E_PTTS_LICENSE_KEY_NOT_FOUND		-101
#define E_PTTS_LICENSE_DATE_EXPIRED			-102
#define E_PTTS_LICENSE_INVALID_SYSTEM		-103
#define E_PTTS_LICENSE_INVALID_KEY			-104

#define CODING_PCM				0	// TextToFile()�� Format
#define CODING_LINEAR8			1
#define CODING_MULAW			2
#define CODING_ALAW				3
#define CODING_VOX				4
#define CODING_ADPCM			5	// 4-bit CCITT g.721 ADPCM
#define CODING_WAVE				6

#define PTTS_FORMAT_WAV		    0	// TextToFileEx()�� Format
#define PTTS_FORMAT_AU			1
#define PTTS_FORMAT_PCM			2

#define PTTS_ENCODING_LINEAR16	0	// TextToFileEx()�� Encoding 
#define PTTS_ENCODING_LINEAR8	1	 
#define PTTS_ENCODING_MULAW		2
#define PTTS_ENCODING_ALAW		3
#define PTTS_ENCODING_VOX		4
#define PTTS_ENCODING_ADPCM		5	// 4-bit CCITT g.721 ADPCM

#define	PTTS_PLAY_START			1		// SOUND CARD API ����
#define	PTTS_PLAY_PAUSE			2		// �ռ��Ⱑ ����ڿ��� uUsrMsg �� �Բ� wParam �� �ִ� ����
#define PTTS_PLAY_RESTART		3
#define PTTS_PLAY_END			4
#define	PTTS_PLAY_ERROR			-1

#define	E_PTTS_USRDERR_LOAD			11	// �̹� ������ �ε�Ǿ� �ִ� ���
#define	E_PTTS_USRDERR_FILEOPEN		12	// ����� ������ Open ���� �ʴ� ���
#define	E_PTTS_USRDERR_MEMALLOC		13	// �޸𸮰� ���� ���
#define	E_PTTS_USRDERR_INVALIDPARA	14	// �Լ��� parameter ���� ���������� ���� ���
#define	E_PTTS_USRDERR_ENTRYNUM		15	// ��� Entry�� = MAX_USRDICT�� �Ѵ� ���
#define	E_PTTS_USRDERR_BADFORMAT	16	// '{���ܾ��Ʈ} {���ܾ��Ʈ}' ������ �ƴ� ���
#define	E_PTTS_USRDERR_BADORGWORD1	17	// ���ܾ��Ʈ�� USRDICT_WORDLIST_LEN(128)�� ���̸� �Ѵ� ���
#define	E_PTTS_USRDERR_BADORGWORD2	18	// ���ܾ��Ʈ�� �ܾ USRDICT_WORD_LEN(40)�� ���̸� �Ѵ� ���
#define	E_PTTS_USRDERR_BADORGWORD3	19	// ���ܾ��Ʈ�� �ٸ� ������ �ִ� ���
#define	E_PTTS_USRDERR_BADNEWWORD1	20	// ���ܾ��Ʈ�� USRDICT_WORDLIST_LEN(128)�� ���̸� �Ѵ� ���
#define	E_PTTS_USRDERR_BADNEWWORD2	21	// ���ܾ��Ʈ�� �ܾ USRDICT_WORD_LEN(40)�� ���̸� �Ѵ� ���
#define	E_PTTS_USRDERR_BADNEWWORD3	22	// ���ܾ��Ʈ�� �ٸ� ������ �ִ� ���

#define PHONE_TYPE_DEFAULT	0	// ������-������ �϶�
#define PHONE_TYPE_CVC		1	// ������-�������� �϶�, ��������
#define PHONE_TYPE_CV		2
#define PHONE_TYPE_VC		3
#define PHONE_TYPE_V		4
#define PHONE_TYPE_PAUSE	5

typedef struct tagLIPSYNCINFO {
	int     phType;				// �� Ÿ��
	char	phone[3];			// ��
	char	bComplete;			// �Ϸ� ���� : OutBufSize != 0 �϶�, [1]complete [0]not-complete (default=1)
	int		nStPos;				// ���� ��ġ : sample ����
	int		nDur;				// �� ����   : sample ����
} LIPSYNCINFO;

typedef struct tagHLIGHTINFO {
	int		nTextPos_Start;
	int		nTextPos_End;
} HLIGHTINFO;

typedef struct tagMARKINFO {
	int			nTotalPhoneNum;		// ��ü �� ��
	int			nCurrentPhoneNum;	// ���� �� ��
	LIPSYNCINFO	*LipSyncData;		// ����ũ ����
	HLIGHTINFO	HLight;				// Highlight ����
} MARKINFO;

typedef struct tagSYNRES {			// �ռ��⿡�� ������ ��� [��������� callback���� ���]
	unsigned int   NumBytes;		// �ռ����� ũ��
	void        *pData;				// �ռ���
	void        *pReserved;			//
	short		Status;				// Buffer�� ���¸� ��Ÿ�� (0:Pcm Buffer ������, 1:����ڰ� ������ TTS_READ_MODE�� ��, 2:��ü �ؽ�Ʈ �ռ� ����, 3:STOP ��ɿ� ���� ����)
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
// Speaker ��ȭ�� ���� ���� �߰� (2003-09-15)
int PTTS_TextToFile(int Speaker, int Format, char *Text, char *FileName, char *tagString, int SamplingRate);

// TextToFile() �Լ� Ȯ�� : format / encoding / sampling rate �� ���� (2003-12-12)
int PTTS_TextToFileEx(int Speaker, int Format, int Encoding, int SamplingRate,
					  char *TextBuf, char *OutFileName, char *tagString);

// TextToFileEx() �Լ� Ȯ�� : ����� ���� ���� (2004-04-21)
int PTTS_TextToFileEx2(int Speaker, int Format, int Encoding, int SamplingRate,
					  char *TextBuf, char *OutFileName, char *tagString, char *UserDictFileName);

/*===========================================================================*/
int  PTTS_NameToSpeech(void *pTTSThread, char *Text, int bTaggedText);
int  PTTS_NameToPcmBuf(void *pTTSThread, char *Text, int bTaggedText, int OutBufSize);
int  PTTS_NameToFile(int Speaker, int Format, int Encoding, int SamplingRate,
					   char *TextBuf, char *OutFileName, char *tagString);
/**********/

/*===========================================================================*/
/* �߰� ���� �Լ� */
int PTTS_SetTextFormat(void *pTTSThread, char chTextFormat);	// 0 : naormal text(default),	1 : VXML
int PTTS_SetCharSet(void *pTTSThread, char chCharSet);			// 0 : KSC-5601+Ȯ��ϼ��� (default), 1: UTF-8
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
