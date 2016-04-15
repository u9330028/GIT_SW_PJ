/****************************************************************************

	한글 입력 오토마타

	오후 1:30 01-05-28
	j22000@netian.com

***************************************************************************/

#define HANINPUT_C


#ifndef HANINPUT_H
	#include "..\..\H\Dll\\HanInput.h"
#endif
#ifndef KS_COMBI_CODE_CONVERT_C
	#include "KS_COMBI_Code_Convert.c"
#endif


/*******************************************************************************/
/**[ 전역 변수 ]***************************************************************/
//한글 입력상태
#define COMBI_HAN_MODE	1
#define KS_HAN_MODE		2
//영문 입력상태
#define ENG_MODE_		0
int gHanMode=0;
int gHanMode_n=3;

//화면의 1줄에 들어가는 문자수
int ScreenMaxX=80;
/*******************************************************************************/

/*******************************************************************************/
//구조체를 초기화 합니다.
void HAN_INPUT_KEY_BUFFER_Clear(HAN_INPUT_KEY_BUFFER *p)
{
	if(p->buf!=NULL)
	{
		p->p=p->buf;
		*(p->p)='\0';
	}
	p->cho=0;
	p->cho2=0;
	p->jung=0;
	p->jong=0;
	p->jong2=0;
	p->status=0;
}
/*--------------------------------------------------------------------------*/
//구조체에 메모리를 할당합니다.
BOOL HAN_INPUT_KEY_BUFFER_Create(HAN_INPUT_KEY_BUFFER *p, int size)
{
	memset(p, 0, sizeof(HAN_INPUT_KEY_BUFFER));
	p->size=size;
	//문자열이 저장될 공간 확보
	////p->buf=(char*)calloc(0, p->size);<<===에러발생:calloc()보다 malloc()사용권장
	p->buf=(char*)malloc(p->size+1);
	if(p->buf==NULL)
		return FALSE;
	
	p->p=p->buf;
	*(p->p)='\0';
	return TRUE;
}
/*--------------------------------------------------------------------------*/
//구조체를 삭제 합니다.
void HAN_INPUT_KEY_BUFFER_Delete(HAN_INPUT_KEY_BUFFER *p)
{
	free(p->buf);
	(p->size)=0;
	(p->p)=p->buf;
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
//현재 입력된 값들을 완성형으로 변환 시켜 화면에 자소단위로 출력
void DEBUG_View_COMBI_code_to_KS_code(HCODE cho, HCODE cho2, HCODE jung, HCODE jong, HCODE jong2)
{
	typedef struct _jasoCombi_KS
	{
		HCODE combi;
		char *ks;
	}JASO_COMBI_KS;

	//초성
	JASO_COMBI_KS T_cho[]={
		{(HCODE)0x8841, "ㄱ"},{(HCODE)0x8c41, "ㄲ"},{(HCODE)0x9041, "ㄴ"},
		{(HCODE)0x9441, "ㄷ"},{(HCODE)0x9841, "ㄸ"},{(HCODE)0x9c41, "ㄹ"},
		{(HCODE)0xa041, "ㅁ"},{(HCODE)0xa441, "ㅂ"},{(HCODE)0xa841, "ㅃ"},
		{(HCODE)0xac41, "ㅅ"},{(HCODE)0xb041, "ㅆ"},{(HCODE)0xb441, "ㅇ"},
		{(HCODE)0xb841, "ㅈ"},{(HCODE)0xbc41, "ㅉ"},{(HCODE)0xc041, "ㅊ"},
		{(HCODE)0xc441, "ㅋ"},{(HCODE)0xc841, "ㅌ"},{(HCODE)0xcc41, "ㅍ"},
		{(HCODE)0xd041, "ㅎ"},{0, ""}};
	//중성
	JASO_COMBI_KS T_jung[]={
		{(HCODE)0x8461, "ㅏ"},{(HCODE)0x8481, "ㅐ"},{(HCODE)0x84a1, "ㅑ"},
		{(HCODE)0x84c1, "ㅒ"},{(HCODE)0x84e1, "ㅓ"},{(HCODE)0x8541, "ㅔ"},
		{(HCODE)0x8561, "ㅕ"},{(HCODE)0x8581, "ㅖ"},{(HCODE)0x85a1, "ㅗ"},
		{(HCODE)0x85c1, "ㅘ"},{(HCODE)0x85e1, "ㅙ"},{(HCODE)0x8641, "ㅚ"},
		{(HCODE)0x8661, "ㅛ"},{(HCODE)0x8681, "ㅜ"},{(HCODE)0x86a1, "ㅝ"},
		{(HCODE)0x86c1, "ㅞ"},{(HCODE)0x86e1, "ㅟ"},{(HCODE)0x8741, "ㅠ"},
		{(HCODE)0x8761, "ㅡ"},{(HCODE)0x8781, "ㅢ"},{(HCODE)0x87a1, "ㅣ"},
		{0, ""}};
	//종성
	JASO_COMBI_KS T_jong[]={
		{(HCODE)0x8442, "ㄱ"},{(HCODE)0x8443, "ㄲ"},{(HCODE)0x8444, "ㄳ"},
		{(HCODE)0x8445, "ㄴ"},{(HCODE)0x8446, "ㄵ"},{(HCODE)0x8447, "ㄶ"},
		{(HCODE)0x8448, "ㄷ"},{(HCODE)0x8449, "ㄹ"},{(HCODE)0x844a, "ㄺ"},
		{(HCODE)0x844b, "ㄻ"},{(HCODE)0x844c, "ㄼ"},{(HCODE)0x844d, "ㄽ"},
		{(HCODE)0x844e, "ㄾ"},{(HCODE)0x844f, "ㄿ"},{(HCODE)0x8450, "ㅀ"},
		{(HCODE)0x8451, "ㅁ"},{(HCODE)0x8453, "ㅂ"},{(HCODE)0x8454, "ㅄ"},
		{(HCODE)0x8455, "ㅅ"},{(HCODE)0x8456, "ㅆ"},{(HCODE)0x8457, "ㅇ"},
		{(HCODE)0x8458, "ㅈ"},{(HCODE)0x8459, "ㅊ"},{(HCODE)0x845a, "ㅋ"},
		{(HCODE)0x845b, "ㅌ"},{(HCODE)0x845c, "ㅍ"},{(HCODE)0x845d, "ㅎ"},
		{0, ""}};

	register int i;
	
	//초성 첫번째 출력
	i=0;
	while(T_cho[i].combi!=0)
	{
		if(T_cho[i].combi==cho)
		{
			printf("%s", T_cho[i].ks);
			break;
		}
		i++;
	}
	//초성 두번째 출력
	i=0;
	while(T_cho[i].combi!=0)
	{
		if(T_cho[i].combi==cho2)
		{
			printf("%s", T_cho[i].ks);
			break;
		}
		i++;
	}
	//중성 출력
	i=0;
	while(T_jung[i].combi!=0)
	{
		if(T_jung[i].combi==jung)
		{
			printf("%s", T_jung[i].ks);
			break;
		}
		i++;
	}
	//종성 첫번째 출력
	i=0;
	while(T_jong[i].combi!=0)
	{
		if(T_jong[i].combi==jong)
		{
			printf("%s", T_jong[i].ks);
			break;
		}
		i++;
	}
	//종성 두번째 출력
	i=0;
	while(T_jong[i].combi!=0)
	{
		if(T_jong[i].combi==jong2)
		{
			printf("%s", T_jong[i].ks);
			break;
		}
		i++;
	}
}
/*--------------------------------------------------------------------------*/
//초성자음코드+초성자음코드=초성자음코드
HCODE COMBI_MixJaumCho(HCODE jaum1, HCODE jaum2)
{
	//초성 : 쌍자음,부자음이 되는가?
	//(ㄱ(ㄲ,ㄺ), ㄷ(ㄸ), ㅁ(ㄻ), ㅂ(ㄼ,ㅃ), ㅅ(ㄳ,ㄽ, ㅄ, ㅆ),
	// ㅈ(ㄵ,ㅉ), ㅌ(ㄾ), ㅍ(ㄿ), ㅎ(ㄶ, ㅀ))
	//쌍자음(연타로 쌍자음을 입력할 수있다.)
	HCODE Table_COMBI_Han_jaumCho[][2]={
		{(HCODE)0x8841, (HCODE)0x8c41},	//ㄱ+ㄱ=ㄲ
		{(HCODE)0x9441, (HCODE)0x9841},	//ㄷ+ㄷ=ㄸ
		{(HCODE)0xa441, (HCODE)0xa841},	//ㅂ+ㅂ=ㅃ
		{(HCODE)0xac41, (HCODE)0xb041},	//ㅅ+ㅅ=ㅆ
		{(HCODE)0xb841, (HCODE)0xbc41},	//ㅈ+ㅈ=ㅉ
		{0,0}};
	register int i=0;

	while(Table_COMBI_Han_jaumCho[i][0]!=0)
	{
		if(Table_COMBI_Han_jaumCho[i][0]==jaum1)
		{
			if(Table_COMBI_Han_jaumCho[i][0]==jaum2)
			{
				return Table_COMBI_Han_jaumCho[i][1];
			}
		}
		i++;
	}
	return 0;
}				

/*--------------------------------------------------------------------------*/
//초성자음코드+초성자음코드=종성자음코드
HCODE COMBI_MixJaumJong(HCODE cho1, HCODE cho2)
{
	//종성 : 쌍자음,부자음이 되는가?
	//(ㄱ(ㄲ,ㄺ), ㄷ(ㄸ), ㅁ(ㄻ), ㅂ(ㄼ,ㅃ), ㅅ(ㄳ,ㄽ, ㅄ, ㅆ),
	// ㅈ(ㄵ,ㅉ), ㅌ(ㄾ), ㅍ(ㄿ), ㅎ(ㄶ, ㅀ))
	//연타로 쌍자음을 입력할 수있다
	HCODE Table_COMBI_Han_jaumJong[][3]={
		{(HCODE)0x8841, (HCODE)0x8841, (HCODE)0x8443},	//ㄱ+ㄱ=ㄲ
		{(HCODE)0x8841, (HCODE)0xac41, (HCODE)0x8444},	//ㄱ+ㅅ=ㄳ
		{(HCODE)0x9041, (HCODE)0xb841, (HCODE)0x8446},	//ㄴ+ㅈ=ㄵ
		{(HCODE)0x9041, (HCODE)0xd041, (HCODE)0x8447},	//ㄴ+ㅎ=ㄶ
		{(HCODE)0x9c41, (HCODE)0x8841, (HCODE)0x844a},	//ㄹ+ㄱ=ㄺ
		{(HCODE)0x9c41, (HCODE)0xa041, (HCODE)0x844b},	//ㄹ+ㅁ=ㄻ
		{(HCODE)0x9c41, (HCODE)0xa441, (HCODE)0x844c},	//ㄹ+ㅂ=ㄼ
		{(HCODE)0x9c41, (HCODE)0xac41, (HCODE)0x844d},	//ㄹ+ㅅ=ㄽ
		{(HCODE)0x9c41, (HCODE)0xc841, (HCODE)0x844e},	//ㄹ+ㅌ=ㄾ
		{(HCODE)0x9c41, (HCODE)0xcc41, (HCODE)0x844f},	//ㄹ+ㅍ=ㄿ
		{(HCODE)0x9c41, (HCODE)0xd041, (HCODE)0x8450},	//ㄹ+ㅎ=ㅀ
		{(HCODE)0xa441, (HCODE)0xac41, (HCODE)0x8454},	//ㅂ+ㅅ=ㅄ
		{(HCODE)0xac41, (HCODE)0xac41, (HCODE)0x8456},	//ㅅ+ㅅ=ㅆ
		{0,0,0}};
	register int i=0;

	while(Table_COMBI_Han_jaumJong[i][0]!=0)
	{
		if(Table_COMBI_Han_jaumJong[i][0]==cho1)
		{
			if(Table_COMBI_Han_jaumJong[i][1]==cho2)
			{
				return Table_COMBI_Han_jaumJong[i][2];
			}
		}
		i++;
	}
	return 0;
}				

/*--------------------------------------------------------------------------*/
//초성 코드를 종성 코드로 바꾼다.
HCODE COMBI_Han_Cho_to_Jong(HCODE cho)
{
	//실패시 0리턴
	//초성'ㄱ'(0x8841) => 종성'ㄱ'(0x8442)
	HCODE Table_COMBI_Han_ChoJong[][2]={
		{(HCODE)0x8841, (HCODE)0x8442},	//ㄱ
		{(HCODE)0x8c41, (HCODE)0x8443},	//ㄲ
		{(HCODE)0x9041, (HCODE)0x8445},	//ㄴ
		{(HCODE)0x9441, (HCODE)0x8448},	//ㄷ
		{(HCODE)0x9c41, (HCODE)0x8449},	//ㄹ
		{(HCODE)0xa041, (HCODE)0x8451},	//ㅁ
		{(HCODE)0xa441, (HCODE)0x8453},	//ㅂ
		{(HCODE)0xac41, (HCODE)0x8455},	//ㅅ
		{(HCODE)0xb041, (HCODE)0x8456},	//ㅆ
		{(HCODE)0xb441, (HCODE)0x8457},	//ㅇ
		{(HCODE)0xb841, (HCODE)0x8458},	//ㅈ
		{(HCODE)0xc041, (HCODE)0x8459},	//ㅊ
		{(HCODE)0xc441, (HCODE)0x845a},	//ㅋ
		{(HCODE)0xc841, (HCODE)0x845b},	//ㅌ
		{(HCODE)0xcc41, (HCODE)0x845c},	//ㅍ
		{(HCODE)0xd041, (HCODE)0x845d},	//ㅎ
		{0, 0}};
	register int i=0;

	while(Table_COMBI_Han_ChoJong[i][0]!=0)
	{
		if(Table_COMBI_Han_ChoJong[i][0]==cho)
		{
			return Table_COMBI_Han_ChoJong[i][1];
		}
		i++;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
//초성, 초성2, 중성, 종성, 종성2를 하나의 글자로 합친다.
HCODE COMBI_Han_cho_cho2_jung_jong_jong2_to_Hstr(HCODE cho, HCODE cho2, HCODE jung, HCODE jong, HCODE jong2)
{
	HCODE code;
	//초성 결합(ㄲㄸㅃㅆㅉ)
	if((cho!=0)&&(cho2!=0))
	{
		cho=COMBI_MixJaumCho(cho, cho2);
	}
	
	//종성 결합(ㄳㄵㄶㄺㄻㄼㄽㄾㄿㅀㅄㅆ)
	if((jong!=0)&&(jong2!=0))
	{
		//종성1(초성코드)+종성2(초성코드) => 결합(종성코드)
		jong=COMBI_MixJaumJong(jong, jong2);
	}
	else
	{
		//종성(초성코드) => 종성코드
		jong=COMBI_Han_Cho_to_Jong(jong);
	}

	//        1 xxxxx yyyyy zzzzz  => 1xxx xxyy yyyz zzzz
	// 0x8441=1 00001 00010 00001  => 1000 0100 0100 0001
	code=(HCODE)0x8000;
	if(cho!=0)
		code|=(cho & 0x7c00);
	else
		code|=0x0400;

	if(jung!=0)
		code|=(jung & 0x03e0);
	else
		code|=0x0040;

	if(jong!=0)
		code|=(jong & 0x001f);
	else
		code|=0x0001;

	return code;
}

/*--------------------------------------------------------------------------*/

//구조체에 저장된 내용을 화면에 출력합니다.(완성형 모드, 조합형 => 완성형 변환 루틴 필요)
BOOL HAN_INPUT_KEY_BUFFER_KS_Print(int x, int y, HAN_INPUT_KEY_BUFFER *p)
{
	char *q;
	HCODE code;
	char buf[1024];

	printf("\r");
	q=buf;
	*q='\0';
	
	//완성형으로 변환
	COMBI_Str_to_KS_Str(q, p->buf);
	while(*q!='\0')
	{
		putch(*q);
		q++;
		x++;
		if(x >= ScreenMaxX)
		{
			x=1;
			y++;
			printf("\n\r");
		}
	}

	//현재 입력되고 있는 내용도 출력합니다.
	//초성, 초성2, 중성, 종성, 종성2 합치기
	code=COMBI_Han_cho_cho2_jung_jong_jong2_to_Hstr(p->cho, p->cho2, p->jung, p->jong, p->jong2);
	//완성형으로 변환
	code=HCODE_COMBI_to_KS(code);
	putch(HCODE_LEFT_CHAR(code));
	putch(HCODE_RIGHT_CHAR(code));
	return TRUE;
}

/*--------------------------------------------------------------------------*/
//구조체에 저장된 내용을 문자열 버퍼로 출력합니다
void HAN_INPUT_KEY_BUFFER_COMBI_BufPrint(HAN_INPUT_KEY_BUFFER *p, char *strbuf)
{
	char *q;
	HCODE code;

	//gotoxy(x,y);
	q=p->buf;
	while(*q!='\0')
	{
		*strbuf=*q;
		strbuf++;
		q++;
	}
	//현재 입력되고 있는 내용도 출력합니다.
	//초성, 중성, 종성, 종성2 합치기
	code=COMBI_Han_cho_cho2_jung_jong_jong2_to_Hstr(p->cho, p->cho2, p->jung, p->jong, p->jong2);
	if((code&0xffff)!=0x8441)
	{
		*strbuf=HCODE_LEFT_CHAR(code);
		strbuf++;
		*strbuf=HCODE_RIGHT_CHAR(code);
		strbuf++;
	}
	*strbuf='\0';
}
/*--------------------------------------------------------------------------*/
//구조체에 저장된 내용을 화면에 출력합니다.(콘솔용)
void HAN_INPUT_KEY_BUFFER_COMBI_Print(int x, int y, HAN_INPUT_KEY_BUFFER *p)
{
	char *q;
	HCODE code;

	//gotoxy(x,y);
	q=p->buf;
	while(*q!='0')
	{
		putch(*q);
		q++;
		x++;
		if(x > ScreenMaxX)
		{
			x=1;
			y++;
		}
	}
	//현재 입력되고 있는 내용도 출력합니다.
	//초성, 중성, 종성, 종성2 합치기
	code=COMBI_Han_cho_cho2_jung_jong_jong2_to_Hstr(p->cho, p->cho2, p->jung, p->jong, p->jong2);
	putch(HCODE_LEFT_CHAR(code));
	putch(HCODE_RIGHT_CHAR(code));
}
/*--------------------------------------------------------------------------*/
//코드가 한글의 자음, 모음중 어느것인지 판단합니다.
//입력은 2벌식을 기준으로 합니다.(초성,종성)
//ㄱㄲㄳㄴㄵㄶㄷㄹㄺㄻㄼㄽㄾㄿㅀㅁㅂㅄㅅㅆㅇㅈㅊㅋㅌㅍㅎㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ
int COMBI_Han_Get_Type(HCODE code)
{
	HCODE Table_COMBI_Han_cho[]={
		(HCODE)0x8442, (HCODE)0x8443, (HCODE)0x8444, (HCODE)0x8445, (HCODE)0x8446, 
		(HCODE)0x8447, (HCODE)0x8448, (HCODE)0x8449, (HCODE)0x844a, (HCODE)0x844b,
		(HCODE)0x844c, (HCODE)0x844d, (HCODE)0x844e, (HCODE)0x844f, (HCODE)0x8450, 
		(HCODE)0x8451, (HCODE)0x8453, (HCODE)0x8454, (HCODE)0x8455, (HCODE)0x8456, 
		(HCODE)0x8457, (HCODE)0x8458, (HCODE)0x8459, (HCODE)0x845a, (HCODE)0x845b, 
		(HCODE)0x845c, (HCODE)0x845d, (HCODE)0x8841, (HCODE)0x8c41, (HCODE)0x9041, 
		(HCODE)0x9441, (HCODE)0x9841, (HCODE)0x9c41, (HCODE)0xa041, (HCODE)0xa441, 
		(HCODE)0xa841, (HCODE)0xac41, (HCODE)0xb041, (HCODE)0xb441, (HCODE)0xb841, 
		(HCODE)0xbc41, (HCODE)0xc041, (HCODE)0xc441, (HCODE)0xc841, (HCODE)0xcc41, 
		(HCODE)0xd041, 0};
	//ㅏㅐㅑㅒㅓㅔㅕㅖㅗㅘㅙㅚㅛㅜㅝㅞㅟㅠㅡㅢㅣ
	HCODE Table_COMBI_Han_jung[]={
		(HCODE)0x8461, (HCODE)0x8481, (HCODE)0x84a1, (HCODE)0x84c1, (HCODE)0x84e1,
		(HCODE)0x8541, (HCODE)0x8561, (HCODE)0x8581, (HCODE)0x85a1, (HCODE)0x85c1,
		(HCODE)0x85e1, (HCODE)0x8641, (HCODE)0x8661, (HCODE)0x8681, (HCODE)0x86a1, 
		(HCODE)0x86c1, (HCODE)0x86e1, (HCODE)0x8741, (HCODE)0x8761, (HCODE)0x8781, 
		(HCODE)0x87a1, 0};
	register int i;
	
	//여기서는 BackSpace도 판단합니다.
	//2048 = 0x0800
	if(code==0x0800)
		return HCODE_BACKSPACE;

	i=0;
	while(Table_COMBI_Han_cho[i]!=0)
	{
		if(code==Table_COMBI_Han_cho[i])
			return HCODE_JAUM;
		i++;
	}
	i=0;
	while(Table_COMBI_Han_jung[i]!=0)
	{
		if(code==Table_COMBI_Han_jung[i])
			return HCODE_MOUM;
		i++;
	}
	//자음도 모음도 백스페이스도 아님
	return 0;
}
/*--------------------------------------------------------------------------*/
/*
void DEBUG_HanPutch(char *str, HCODE cho, HCODE cho2, HCODE jung, HCODE jong, HCODE jong2)
{
	HCODE code;
	char *p=str;

	code=COMBI_Han_cho_cho2_jung_jong_jong2_to_Hstr(cho, cho2, jung, jong, jong2);
	*p=HCODE_LEFT_CHAR(code);	p++;
	*p=HCODE_RIGHT_CHAR(code);	p++;
	*p='\0';
	
	COMBI_Str_to_KS_Str(str, str);
}
*/
/*--------------------------------------------------------------------------*/
//초성, 초성2, 중성, 종성, 종성2를 하나의 글자로 합쳐 한글 입력 구조체에 넣는다.
BOOL COMBI_Han_putch(HAN_INPUT_KEY_BUFFER *p, HCODE cho, HCODE cho2, HCODE jung, HCODE jong, HCODE jong2)
{
	HCODE code;

	//버퍼가 충분하다면
	if(p->size > (int)(strlen(p->buf)+3))
	{
		//초성, 중성, 종성, 종성2 합치기
		code=COMBI_Han_cho_cho2_jung_jong_jong2_to_Hstr(cho, cho2, jung, jong, jong2);
		*(p->p)=HCODE_LEFT_CHAR(code);	p->p++;
		*(p->p)=HCODE_RIGHT_CHAR(code);	p->p++;
		*(p->p)='\0';
		return TRUE;
	}
	else
	{
		//버퍼가 부족합니다.
		printf("\n버퍼가 부족합니다.\n");
		return FALSE;
	}
}
/*--------------------------------------------------------------------------*/
//ascii코드를 한글 입력 구조체에 넣는다.
//상위 8비트만 데이터가 있다.
BOOL COMBI_Eng_putch(HAN_INPUT_KEY_BUFFER *p, HCODE ascii)
{
	//버퍼가 충분하다면
	if(p->size > (int)(strlen(p->buf)+3))
	{
		*(p->p)=HCODE_LEFT_CHAR(ascii);	p->p++;
		*(p->p)='\0';
		return TRUE;
	}
	else
	{
		//버퍼가 부족합니다.
		return FALSE;
	}
}
/*--------------------------------------------------------------------------*/
//버퍼에 저장된 마지막 문자를 삭제한다.
//한글이면 한글 1문자를 삭제한다.
void COMBI_Han_DelPreChar(HAN_INPUT_KEY_BUFFER *p)
{
	//*(p->p)=='0' 이다
	if(p->p > p->buf+1)
	{
		//2문자 이상 존재
		//한글인지 검사
	
		BOOL HanFlg = FALSE;
		char* Tp = p->buf;

		while (Tp <= p->p)
		{
			
			if ( ((*Tp) & 0x80) != 0 )
			{
				Tp =  Tp + 2;
				if (Tp == p->p)
				{
					HanFlg = TRUE;
					break;
				}										
			}
			else
			{
				Tp =  Tp + 1;
				if (Tp == p->p)
				{
					HanFlg = FALSE;
					break;
				}

			}
		}


		if(HanFlg)
		{
			//한글임
			//삭제(2 byte)
			p->p-=2;
			*(p->p)='\0';
		}
		else
		{
			//한글 아님
			//삭제
			p->p--;
			*(p->p)='\0';
		}


//		if((*(p->p-2) & 0x80)!=0)
//		{
//			//한글임
//			//삭제(2 byte)
//			p->p-=2;
//			*(p->p)='\0';
//		}
//		else
//		{
//			//한글 아님
//			//삭제
//			p->p--;
//			*(p->p)='\0';
//		}
	}
	else
	{
		//1문자 미만 존재 
		if(p->p == p->buf)
		{
			//저장된 문자가 없음
		}
		else
		{
			//1문자가 있음
			//삭제
			p->p--;
			*(p->p)='\0';
		}
	}
}
/*--------------------------------------------------------------------------*/
//부모음이 되는가?(ㅘ, ㅙ, ㅚ, ㅝ, ㅞ, ㅟ, ㅢ)
//주의 : 순서대로 와야함. 예) ㅗ+ㅏ(ㅇ),  ㅏ+ㅗ(X)
HCODE COMBI_MixDoubleVowel(HCODE jung1, HCODE jung2)
{
	HCODE Table_COMBI_Han_jung[][3]={
		{(HCODE)0x85a1, (HCODE)0x8461, (HCODE)0x85c1},	//ㅗ+ㅏ=ㅘ
		{(HCODE)0x85a1, (HCODE)0x8481, (HCODE)0x85e1},	//ㅗ+ㅐ=ㅙ
		{(HCODE)0x85a1, (HCODE)0x87a1, (HCODE)0x8641},	//ㅗ+ㅣ=ㅚ
		{(HCODE)0x8681, (HCODE)0x84e1, (HCODE)0x86a1},	//ㅜ+ㅓ=ㅝ
		{(HCODE)0x8681, (HCODE)0x8541, (HCODE)0x86c1},	//ㅜ+ㅔ=ㅞ
		{(HCODE)0x8681, (HCODE)0x87a1, (HCODE)0x86e1},	//ㅜ+ㅣ=ㅟ
		{(HCODE)0x8761, (HCODE)0x87a1, (HCODE)0x8781},	//ㅡ+ㅣ=ㅢ
		{0,0}};
	register int i=0;

	while(Table_COMBI_Han_jung[i][0]!=0)
	{
		if(Table_COMBI_Han_jung[i][0]==jung1)
		{
			if(Table_COMBI_Han_jung[i][1]==jung2)
			{
				//부모음 됨
				return Table_COMBI_Han_jung[i][2];
			}
		}
		i++;
	}
	return 0;
}			




void SUNNY_COMBI_Han_Input(HAN_INPUT_KEY_BUFFER *p, HCODE code)
{


	COMBI_Han_putch(p, p->cho, p->cho2, p->jung, p->jong, p->jong2);
	//현재 입력된코드 출력

	p->cho=0;
	p->cho2=0;
	p->jung=0;
	p->jong=0;
	p->jong2=0;
	p->status=0;	


	switch(p->status)
	{
		case 0:	//s0 : 처음 상태------------------------------------------------
			switch(COMBI_Han_Get_Type(code))
			{
			case HCODE_JAUM:	//자음:
				//자음이 입력됨
				p->cho=code;
				//다음 상태로 이동
				p->status=1;
				break;
			case HCODE_MOUM:	//모음:
				//처음 부터 모음이 입력됨
				//초성은 fill cdoe로 채우고 s2 : 초성과 중성이 입력된상태로 이동
				//fill code=00001  1 00001 00010 00001 = 0x8441
				p->cho=0;
				p->jung=code;
				//다음 상태로 이동
				p->status=2;
				break;
			case HCODE_BACKSPACE:	//BackSpace:
				//이전 문자 삭제
				COMBI_Han_DelPreChar(p);
				///p->cho=0;
				break;
			default:	//기타문자
				//현재 입력된코드 출력
				COMBI_Eng_putch(p, code);
				///p->cho=0;
				break;
			}
			break;

	}
}









/*--------------------------------------------------------------------------*/
//조합형 한글 입력
void COMBI_Han_Input(HAN_INPUT_KEY_BUFFER *p, HCODE code)
{
	HCODE temp_hcode;

	switch(p->status)
	{
		case 0:	//s0 : 처음 상태------------------------------------------------
			switch(COMBI_Han_Get_Type(code))
			{
			case HCODE_JAUM:	//자음:
				//자음이 입력됨
				p->cho=code;
				//다음 상태로 이동
				p->status=1;
				break;
			case HCODE_MOUM:	//모음:
				//처음 부터 모음이 입력됨
				//초성은 fill cdoe로 채우고 s2 : 초성과 중성이 입력된상태로 이동
				//fill code=00001  1 00001 00010 00001 = 0x8441
				p->cho=0;
				p->jung=code;
				//다음 상태로 이동
				p->status=2;
				break;
			case HCODE_BACKSPACE:	//BackSpace:
				//이전 문자 삭제
				COMBI_Han_DelPreChar(p);
				///p->cho=0;
				break;
			default:	//기타문자
				//현재 입력된코드 출력
				COMBI_Eng_putch(p, code);
				///p->cho=0;
				break;
			}
			break;
		case 1:	//s1 : 초성만 입력된상태--------------------------------------------------
			switch(COMBI_Han_Get_Type(code))
			{
			case HCODE_JAUM:	//자음:
				//자음이 입력됨
				//입력된 초성이 쌍자음,부자음이라면
				if(p->cho2!=0)
				{
					//현재 쌍자음, 부자음이 입력되어 있음
					//현재 쌍자음, 부자음을 출력하고 입력된 code를 초성으로 설정
					COMBI_Han_putch(p, p->cho, p->cho2, 0, 0, 0);
					p->cho=code;
					p->cho2=0;
					///status=1;
				}
				else
				{
					//초성 1개(기본자음)만 입력됨
					//쌍자음이 되는가?(ㄲ,ㄸ,ㅃㅆ,ㅉ)
					temp_hcode=COMBI_MixJaumCho(p->cho, code);
					if(temp_hcode!=0)
					{
						//부 자음됨
						p->cho2=code;
						///status=1;
					}
					else
					{
						//부자음(종성)이 되는가?(ㄺ,ㄻ,ㄼ,ㄳ,ㄽ,ㅄ,ㄵ,ㄾ,ㄿ,ㄶ,ㅀ)
						temp_hcode=COMBI_MixJaumJong(p->cho, code);
						if(temp_hcode!=0)
						{
							//부자음됨
							//현재 초성과 입력값을 종성으로 설정
							p->jong=p->cho;
							p->jong2=code;
							p->cho=0;
							///p->cho2=0;
							//다음상태로 이동: s3 : 초성, 중성, 종성이 입력된상태(글자완성안됨)
							p->status=3;
						}
						else
						{
							//부 자음 안됨
							//이전 입력된 자음은 출력하고 현재 입력값이 자음으로 설정
							COMBI_Han_putch(p, p->cho, p->cho2, 0, 0, 0);
							p->cho=code;
							p->cho2=0;
							///status=1;
						}
					}
				}
				break;
			case HCODE_MOUM:	//모음:
				//모음이 입력됨
				//현재 쌍자음, 부자음이 입력되어 있음
				p->jung=code;
				//다음 상태로 이동
				p->status=2;
				break;
			case HCODE_BACKSPACE:	//BackSpace:
				//이전 문자 삭제
				if(p->cho2!=0)
				{
					p->cho2=0;
					p->status = 1;	// sunny
				}
				else
				{
					p->cho=0;
					//이전 상태로 이동
					p->status=0;
				}

				break;
			default:	//기타문자
				//초성출력
				COMBI_Han_putch(p, p->cho, p->cho2, 0, 0, 0);
				//현재 입력된코드 출력
				COMBI_Eng_putch(p, code);
				//처음 상태로 이동
				p->cho=0;
				p->status=0;	
				break;
			}
			break;
		case 2:	//s2 : 초성과 중성이 입력된상태--------------------------------------
			switch(COMBI_Han_Get_Type(code))
			{
				case HCODE_JAUM:	//자음:
					//자음이 입력됨(종성)
					p->jong=code;
					//다음 상태로 이동
					p->status=3;
					break;
				case HCODE_MOUM:	//모음:
					//모음이 입력됨
					//부모음이 되는가?(ㅘ, ㅙ, ㅚ, ㅝ, ㅞ, ㅟ, ㅢ)
					temp_hcode=COMBI_MixDoubleVowel(p->jung, code);
					if(temp_hcode==FALSE)
					{
						//부모음이 안된다.
						//현재 값을 출력하고 s2 : 초성과 중성이 입력된상태로 이동
						COMBI_Han_putch(p, p->cho, p->cho2, p->jung, 0, 0);
						p->cho=0;
						p->jung=code;
						///status=2;
					}
					else
					{
						//부모음이 된다.
						//부모음으로 합친다.
						p->jung=temp_hcode;
						///status=2;
					}
					break;
				case HCODE_BACKSPACE:	//BackSpace:

					if (p->cho == 0) //초성이 없으면
					{
						//이전 문자(중성) 삭제
						p->jung=0;
						//이전 상태에서 초기화로
						p->status=0;
					}
					else
					{
						//이전 문자(중성) 삭제
						p->jung=0;
						//이전 상태(초성이 입력된 상태)로 이동
						p->status=1;
					}

//					//이전 문자(중성) 삭제
//					p->jung=0;
//					//이전 상태(초성이 입력된 상태)로 이동
//					p->status=1;



					break;
				default:	//기타문자
					//초성,중성을 출력하고 처음상태로 이동
					COMBI_Han_putch(p, p->cho, p->cho2, p->jung, 0, 0);
					//현재 입력된코드 출력
					COMBI_Eng_putch(p, code);
					//처음 상태로 이동
					p->cho=0;
					p->cho2=0;
					p->jung=0;
					p->status=0;	
					break;
			}
			break;
		case 3:	//s3 : 초성, 중성, 종성이 입력된상태(글자완성안됨)----------------------
			switch(COMBI_Han_Get_Type(code))
			{
			case HCODE_JAUM:	//자음:
				//자음이 입력됨
				//부자음이 입력되어있다면
				if(p->jong2!=0)
				{
					//부자음 출력, 현재 입력을 초성으로
					COMBI_Han_putch(p, p->cho, p->cho2, p->jung, p->jong, p->jong2);
					p->cho=code;
					p->cho2=0;
					p->jung=0;
					p->jong=0;
					p->jong2=0;
					//다음상태로 이동 : s1 : 초성만 입력된상태
					p->status=1;
				}
				else
				{
					//종성이 한개만 입력되어 있다.
					//받침글자(쌍자음,부자음)가 되는가?(ㄱㄲㄳㄴㄵㄶㄷㄹㄺㄻㄼㄽㄾㄿㅀㅁㅂㅄㅅㅆㅇㅈㅊㅋㅌㅍㅎ)
					temp_hcode=COMBI_MixJaumJong(p->jong, code);
					if(temp_hcode!=0)
					{
						//받침글자(쌍자음,부자음)가 됨
						//부자음으로 만듦
						p->jong2=code;
						///status=3;
					}
					else
					{
						//받침글자(쌍자음,부자음) 안됨 
						//현재 문자 출력후 입력된 자음을 초성에 입력
						COMBI_Han_putch(p, p->cho, p->cho2, p->jung, p->jong, 0);
						p->cho=code;
						p->cho2=0;
						p->jung=0;
						p->jong=0;
						p->jong2=0;
						//자음이 입력된 상태로 이동
						p->status=1;			
					}
				}
				break;
			case HCODE_MOUM:	//모음:
				//모음이 입력됨
				//종성이 1개 입력되어 있다면
				if(p->jong2==0)
				{
					//초성+중성(출력), 초성+중성으로 만든다.
					COMBI_Han_putch(p, p->cho, p->cho2, p->jung, 0, 0);	
					//초성, 중성이 입력된 상태로
					p->cho=p->jong;
					p->cho2=0;
					p->jung=code;
					p->jong=0;
					p->jong2=0;
					p->status=2;
				}
				else
				{
					//쌍자음,부자음임
					//초성+중성+종성 출력, 종성2=>초성+중성
					COMBI_Han_putch(p, p->cho, p->cho2, p->jung, p->jong, 0);
					//초성, 중성이 입력된 상태로
					p->cho=p->jong2;
					p->cho2=0;
					p->jung=code;
					p->jong=0;
					p->jong2=0;
					p->status=2;
				}
				break;
			case HCODE_BACKSPACE:	//BackSpace:
				//이전 문자 삭제
				//종성2가 있으면
				if(p->jong2!=0)
				{
					//종성2 삭제
					p->jong2=0;
					//초성, 중성, 종성이 입력된 상태
					///status=3;
				}
				else
				if (p->jung == 0)		//sunny 예외처리
				{
					if (p->cho == 0)
					{
						p->jong=0;
						p->status=0;
					}
					else
					{
						p->jong	=0;
						p->cho	=0;
						p->cho2 =0;

						p->status = 0;
					}


				}
				else
				{
					//종성 삭제
					p->jong=0;
					//초성, 중성이 입력된 상태
					p->status=2;
				}
				break;
			default:	//기타문자
				//초성,중성, 종성을 출력하고 처음상태로 이동
				COMBI_Han_putch(p, p->cho, p->cho2, p->jung, p->jong, p->jong2);
				//현재 입력된코드 출력
				COMBI_Eng_putch(p, code);
				//처음 상태로 이동
				p->cho=0;
				p->cho2=0;
				p->jung=0;
				p->jong=0;
				p->jong2=0;
				p->status=0;	
				break;
			}
			break;
	}
}
/*--------------------------------------------------------------------------*/
//아스키 입력을 한글 입력으로 변환
HCODE COMBI_Key_to_HCODE(char key, BOOL shiftkey)
{
	typedef struct _ascii_to_han
	{
		HCODE	han1;	//
		HCODE	han2;	//쉬프트키가 눌린상태
		char	type;	//자음(1), 모음(2)
	}KEY_BOARD_ASCII_TO_HAN;

	HCODE ret;
	KEY_BOARD_ASCII_TO_HAN keyboard_A_Z[]={
        {(HCODE)0xa041, 0, 1},				//ㅁ   	a
		{(HCODE)0x8741, 0, 2},				//ㅠ	b
        {(HCODE)0xc041, 0, 1},				//ㅊ	c
        {(HCODE)0xb441, 0, 1},				//ㅇ	d
        {(HCODE)0x9441, (HCODE)0x9841, 1},	//ㄷ,ㄸ	e
        {(HCODE)0x9c41, 0, 1},				//ㄹ	f
        {(HCODE)0xd041, 0, 1},				//ㅎ	g
        {(HCODE)0x85a1, 0, 2},				//ㅗ	h
        {(HCODE)0x84a1, 0, 2},				//ㅑ	i
        {(HCODE)0x84e1, 0, 2},				//ㅓ	j
        {(HCODE)0x8461, 0, 2},				//ㅏ	k
        {(HCODE)0x87a1, 0, 2},				//ㅣ	l
        {(HCODE)0x8761, 0, 2},				//ㅡ	m
        {(HCODE)0x8681, 0, 2},				//ㅜ	n
        {(HCODE)0x8481, (HCODE)0x84c1, 2},	//ㅐ,ㅒ	o
        {(HCODE)0x8541, (HCODE)0x8581, 2},	//ㅔ,ㅖ	p
        {(HCODE)0xa441, (HCODE)0xa841, 1},	//ㅂ,ㅃ	q
        {(HCODE)0x8841, (HCODE)0x8c41, 1},	//ㄱ,ㄲ	r
        {(HCODE)0x9041, 0, 1},				//ㄴ	s
        {(HCODE)0xac41, (HCODE)0xb041, 1},	//ㅅ,ㅆ	t
        {(HCODE)0x8561, 0, 2},				//ㅕ	u
        {(HCODE)0xcc41, 0, 1},				//ㅍ	v
        {(HCODE)0xb841, (HCODE)0xbc41, 1},	//ㅈ,ㅉ	w
        {(HCODE)0xc841, 0, 1},				//ㅌ	x
        {(HCODE)0x8661, 0, 2},				//ㅛ	y
        {(HCODE)0xc441, 0, 1},				//ㅋ	z
		{0,  0, 0}};
	static int flag_shift=0;
	static int shift_count=0;

	//키보드로부터 입력받은 키를 한글로 변환한다.
	if(gHanMode==COMBI_HAN_MODE)
	{
		//한글 입력 모드
		//소문자로 변환
		if('A' <= key && key <= 'Z')
		{
			if(shift_count > 2)
			{
				//현재 입력문자는 소문자로 입력한다.
				//쉬프트를 눌러 대문자를 입력했다.
				flag_shift=1;
			}
			else
			{
				//현재 입력모드는 대문자로 입력된다.
				flag_shift=0;
			}
			key=key+('a'-'A');
			shiftkey=TRUE;
		}
		if('a' <= key && key <= 'z')
		{
			if(shiftkey==TRUE)
			{
				//쉬프트 키가 눌린상태
				if(keyboard_A_Z[key-'a'].han2!=0)
					return keyboard_A_Z[key-'a'].han2;
				else
					return keyboard_A_Z[key-'a'].han1;
			}
			else
			{
				return keyboard_A_Z[key-'a'].han1;
			}
		}
		else
		{
			//영어문자가 아니다. 기호문자 입력
			//그대로 돌려준다.
			ret=(key<<8);
			return ret;
		}
	}
	else
	{
		//영문 입력모드
		//그대로 돌려준다.
		ret=(key<<8);
		return ret;
	}
}
/**********************************************************************/
#ifdef __DEBUG
void demo_HAN_INPUT_KEY_BUFFER_main()
//void main()
{
	HAN_INPUT_KEY_BUFFER key_buf;
	HCODE code;
	char key;
	BOOL exitwhile=FALSE;

	HAN_INPUT_KEY_BUFFER_Create(&key_buf, 100);
	//한글 입력 상태로 설정
	gHanMode=HAN_MODE;
	/*----------------------------*/
	while(exitwhile==FALSE)
	{
		key=_getch();
		switch(key)
		{
			case 0:
			case 0xe0:
			case -32:
					//화살표 키
				break;
			case 13:	//Enter
				//구조체를 초기화 합니다.
				HAN_INPUT_KEY_BUFFER_Clear(&key_buf);
				printf("\n");
				break;
			case 27:	//ESC
				exitwhile=TRUE;
				break;
			default:
				//입력받은 키를 한글 키로 변환
				code=COMBI_Key_to_HCODE(key, 0);
				COMBI_Han_Input(&key_buf, code);
				//HAN_INPUT_KEY_BUFFER_COMBI_Print(1, 1, &key_buf);
				HAN_INPUT_KEY_BUFFER_KS_Print(1, 1, &key_buf);
		}
	}
	/*----------------------------*/
	HAN_INPUT_KEY_BUFFER_Delete(&key_buf);
}
#endif