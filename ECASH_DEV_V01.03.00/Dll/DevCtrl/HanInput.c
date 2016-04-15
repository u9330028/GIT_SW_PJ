/****************************************************************************

	�ѱ� �Է� ���丶Ÿ

	���� 1:30 01-05-28
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
/**[ ���� ���� ]***************************************************************/
//�ѱ� �Է»���
#define COMBI_HAN_MODE	1
#define KS_HAN_MODE		2
//���� �Է»���
#define ENG_MODE_		0
int gHanMode=0;
int gHanMode_n=3;

//ȭ���� 1�ٿ� ���� ���ڼ�
int ScreenMaxX=80;
/*******************************************************************************/

/*******************************************************************************/
//����ü�� �ʱ�ȭ �մϴ�.
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
//����ü�� �޸𸮸� �Ҵ��մϴ�.
BOOL HAN_INPUT_KEY_BUFFER_Create(HAN_INPUT_KEY_BUFFER *p, int size)
{
	memset(p, 0, sizeof(HAN_INPUT_KEY_BUFFER));
	p->size=size;
	//���ڿ��� ����� ���� Ȯ��
	////p->buf=(char*)calloc(0, p->size);<<===�����߻�:calloc()���� malloc()������
	p->buf=(char*)malloc(p->size+1);
	if(p->buf==NULL)
		return FALSE;
	
	p->p=p->buf;
	*(p->p)='\0';
	return TRUE;
}
/*--------------------------------------------------------------------------*/
//����ü�� ���� �մϴ�.
void HAN_INPUT_KEY_BUFFER_Delete(HAN_INPUT_KEY_BUFFER *p)
{
	free(p->buf);
	(p->size)=0;
	(p->p)=p->buf;
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
//���� �Էµ� ������ �ϼ������� ��ȯ ���� ȭ�鿡 �ڼҴ����� ���
void DEBUG_View_COMBI_code_to_KS_code(HCODE cho, HCODE cho2, HCODE jung, HCODE jong, HCODE jong2)
{
	typedef struct _jasoCombi_KS
	{
		HCODE combi;
		char *ks;
	}JASO_COMBI_KS;

	//�ʼ�
	JASO_COMBI_KS T_cho[]={
		{(HCODE)0x8841, "��"},{(HCODE)0x8c41, "��"},{(HCODE)0x9041, "��"},
		{(HCODE)0x9441, "��"},{(HCODE)0x9841, "��"},{(HCODE)0x9c41, "��"},
		{(HCODE)0xa041, "��"},{(HCODE)0xa441, "��"},{(HCODE)0xa841, "��"},
		{(HCODE)0xac41, "��"},{(HCODE)0xb041, "��"},{(HCODE)0xb441, "��"},
		{(HCODE)0xb841, "��"},{(HCODE)0xbc41, "��"},{(HCODE)0xc041, "��"},
		{(HCODE)0xc441, "��"},{(HCODE)0xc841, "��"},{(HCODE)0xcc41, "��"},
		{(HCODE)0xd041, "��"},{0, ""}};
	//�߼�
	JASO_COMBI_KS T_jung[]={
		{(HCODE)0x8461, "��"},{(HCODE)0x8481, "��"},{(HCODE)0x84a1, "��"},
		{(HCODE)0x84c1, "��"},{(HCODE)0x84e1, "��"},{(HCODE)0x8541, "��"},
		{(HCODE)0x8561, "��"},{(HCODE)0x8581, "��"},{(HCODE)0x85a1, "��"},
		{(HCODE)0x85c1, "��"},{(HCODE)0x85e1, "��"},{(HCODE)0x8641, "��"},
		{(HCODE)0x8661, "��"},{(HCODE)0x8681, "��"},{(HCODE)0x86a1, "��"},
		{(HCODE)0x86c1, "��"},{(HCODE)0x86e1, "��"},{(HCODE)0x8741, "��"},
		{(HCODE)0x8761, "��"},{(HCODE)0x8781, "��"},{(HCODE)0x87a1, "��"},
		{0, ""}};
	//����
	JASO_COMBI_KS T_jong[]={
		{(HCODE)0x8442, "��"},{(HCODE)0x8443, "��"},{(HCODE)0x8444, "��"},
		{(HCODE)0x8445, "��"},{(HCODE)0x8446, "��"},{(HCODE)0x8447, "��"},
		{(HCODE)0x8448, "��"},{(HCODE)0x8449, "��"},{(HCODE)0x844a, "��"},
		{(HCODE)0x844b, "��"},{(HCODE)0x844c, "��"},{(HCODE)0x844d, "��"},
		{(HCODE)0x844e, "��"},{(HCODE)0x844f, "��"},{(HCODE)0x8450, "��"},
		{(HCODE)0x8451, "��"},{(HCODE)0x8453, "��"},{(HCODE)0x8454, "��"},
		{(HCODE)0x8455, "��"},{(HCODE)0x8456, "��"},{(HCODE)0x8457, "��"},
		{(HCODE)0x8458, "��"},{(HCODE)0x8459, "��"},{(HCODE)0x845a, "��"},
		{(HCODE)0x845b, "��"},{(HCODE)0x845c, "��"},{(HCODE)0x845d, "��"},
		{0, ""}};

	register int i;
	
	//�ʼ� ù��° ���
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
	//�ʼ� �ι�° ���
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
	//�߼� ���
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
	//���� ù��° ���
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
	//���� �ι�° ���
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
//�ʼ������ڵ�+�ʼ������ڵ�=�ʼ������ڵ�
HCODE COMBI_MixJaumCho(HCODE jaum1, HCODE jaum2)
{
	//�ʼ� : ������,�������� �Ǵ°�?
	//(��(��,��), ��(��), ��(��), ��(��,��), ��(��,��, ��, ��),
	// ��(��,��), ��(��), ��(��), ��(��, ��))
	//������(��Ÿ�� �������� �Է��� ���ִ�.)
	HCODE Table_COMBI_Han_jaumCho[][2]={
		{(HCODE)0x8841, (HCODE)0x8c41},	//��+��=��
		{(HCODE)0x9441, (HCODE)0x9841},	//��+��=��
		{(HCODE)0xa441, (HCODE)0xa841},	//��+��=��
		{(HCODE)0xac41, (HCODE)0xb041},	//��+��=��
		{(HCODE)0xb841, (HCODE)0xbc41},	//��+��=��
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
//�ʼ������ڵ�+�ʼ������ڵ�=���������ڵ�
HCODE COMBI_MixJaumJong(HCODE cho1, HCODE cho2)
{
	//���� : ������,�������� �Ǵ°�?
	//(��(��,��), ��(��), ��(��), ��(��,��), ��(��,��, ��, ��),
	// ��(��,��), ��(��), ��(��), ��(��, ��))
	//��Ÿ�� �������� �Է��� ���ִ�
	HCODE Table_COMBI_Han_jaumJong[][3]={
		{(HCODE)0x8841, (HCODE)0x8841, (HCODE)0x8443},	//��+��=��
		{(HCODE)0x8841, (HCODE)0xac41, (HCODE)0x8444},	//��+��=��
		{(HCODE)0x9041, (HCODE)0xb841, (HCODE)0x8446},	//��+��=��
		{(HCODE)0x9041, (HCODE)0xd041, (HCODE)0x8447},	//��+��=��
		{(HCODE)0x9c41, (HCODE)0x8841, (HCODE)0x844a},	//��+��=��
		{(HCODE)0x9c41, (HCODE)0xa041, (HCODE)0x844b},	//��+��=��
		{(HCODE)0x9c41, (HCODE)0xa441, (HCODE)0x844c},	//��+��=��
		{(HCODE)0x9c41, (HCODE)0xac41, (HCODE)0x844d},	//��+��=��
		{(HCODE)0x9c41, (HCODE)0xc841, (HCODE)0x844e},	//��+��=��
		{(HCODE)0x9c41, (HCODE)0xcc41, (HCODE)0x844f},	//��+��=��
		{(HCODE)0x9c41, (HCODE)0xd041, (HCODE)0x8450},	//��+��=��
		{(HCODE)0xa441, (HCODE)0xac41, (HCODE)0x8454},	//��+��=��
		{(HCODE)0xac41, (HCODE)0xac41, (HCODE)0x8456},	//��+��=��
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
//�ʼ� �ڵ带 ���� �ڵ�� �ٲ۴�.
HCODE COMBI_Han_Cho_to_Jong(HCODE cho)
{
	//���н� 0����
	//�ʼ�'��'(0x8841) => ����'��'(0x8442)
	HCODE Table_COMBI_Han_ChoJong[][2]={
		{(HCODE)0x8841, (HCODE)0x8442},	//��
		{(HCODE)0x8c41, (HCODE)0x8443},	//��
		{(HCODE)0x9041, (HCODE)0x8445},	//��
		{(HCODE)0x9441, (HCODE)0x8448},	//��
		{(HCODE)0x9c41, (HCODE)0x8449},	//��
		{(HCODE)0xa041, (HCODE)0x8451},	//��
		{(HCODE)0xa441, (HCODE)0x8453},	//��
		{(HCODE)0xac41, (HCODE)0x8455},	//��
		{(HCODE)0xb041, (HCODE)0x8456},	//��
		{(HCODE)0xb441, (HCODE)0x8457},	//��
		{(HCODE)0xb841, (HCODE)0x8458},	//��
		{(HCODE)0xc041, (HCODE)0x8459},	//��
		{(HCODE)0xc441, (HCODE)0x845a},	//��
		{(HCODE)0xc841, (HCODE)0x845b},	//��
		{(HCODE)0xcc41, (HCODE)0x845c},	//��
		{(HCODE)0xd041, (HCODE)0x845d},	//��
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
//�ʼ�, �ʼ�2, �߼�, ����, ����2�� �ϳ��� ���ڷ� ��ģ��.
HCODE COMBI_Han_cho_cho2_jung_jong_jong2_to_Hstr(HCODE cho, HCODE cho2, HCODE jung, HCODE jong, HCODE jong2)
{
	HCODE code;
	//�ʼ� ����(����������)
	if((cho!=0)&&(cho2!=0))
	{
		cho=COMBI_MixJaumCho(cho, cho2);
	}
	
	//���� ����(������������������������)
	if((jong!=0)&&(jong2!=0))
	{
		//����1(�ʼ��ڵ�)+����2(�ʼ��ڵ�) => ����(�����ڵ�)
		jong=COMBI_MixJaumJong(jong, jong2);
	}
	else
	{
		//����(�ʼ��ڵ�) => �����ڵ�
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

//����ü�� ����� ������ ȭ�鿡 ����մϴ�.(�ϼ��� ���, ������ => �ϼ��� ��ȯ ��ƾ �ʿ�)
BOOL HAN_INPUT_KEY_BUFFER_KS_Print(int x, int y, HAN_INPUT_KEY_BUFFER *p)
{
	char *q;
	HCODE code;
	char buf[1024];

	printf("\r");
	q=buf;
	*q='\0';
	
	//�ϼ������� ��ȯ
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

	//���� �Էµǰ� �ִ� ���뵵 ����մϴ�.
	//�ʼ�, �ʼ�2, �߼�, ����, ����2 ��ġ��
	code=COMBI_Han_cho_cho2_jung_jong_jong2_to_Hstr(p->cho, p->cho2, p->jung, p->jong, p->jong2);
	//�ϼ������� ��ȯ
	code=HCODE_COMBI_to_KS(code);
	putch(HCODE_LEFT_CHAR(code));
	putch(HCODE_RIGHT_CHAR(code));
	return TRUE;
}

/*--------------------------------------------------------------------------*/
//����ü�� ����� ������ ���ڿ� ���۷� ����մϴ�
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
	//���� �Էµǰ� �ִ� ���뵵 ����մϴ�.
	//�ʼ�, �߼�, ����, ����2 ��ġ��
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
//����ü�� ����� ������ ȭ�鿡 ����մϴ�.(�ֿܼ�)
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
	//���� �Էµǰ� �ִ� ���뵵 ����մϴ�.
	//�ʼ�, �߼�, ����, ����2 ��ġ��
	code=COMBI_Han_cho_cho2_jung_jong_jong2_to_Hstr(p->cho, p->cho2, p->jung, p->jong, p->jong2);
	putch(HCODE_LEFT_CHAR(code));
	putch(HCODE_RIGHT_CHAR(code));
}
/*--------------------------------------------------------------------------*/
//�ڵ尡 �ѱ��� ����, ������ ��������� �Ǵ��մϴ�.
//�Է��� 2������ �������� �մϴ�.(�ʼ�,����)
//��������������������������������������������������������������������������������������������
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
	//�������¤äĤŤƤǤȤɤʤˤ̤ͤΤϤФѤҤ�
	HCODE Table_COMBI_Han_jung[]={
		(HCODE)0x8461, (HCODE)0x8481, (HCODE)0x84a1, (HCODE)0x84c1, (HCODE)0x84e1,
		(HCODE)0x8541, (HCODE)0x8561, (HCODE)0x8581, (HCODE)0x85a1, (HCODE)0x85c1,
		(HCODE)0x85e1, (HCODE)0x8641, (HCODE)0x8661, (HCODE)0x8681, (HCODE)0x86a1, 
		(HCODE)0x86c1, (HCODE)0x86e1, (HCODE)0x8741, (HCODE)0x8761, (HCODE)0x8781, 
		(HCODE)0x87a1, 0};
	register int i;
	
	//���⼭�� BackSpace�� �Ǵ��մϴ�.
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
	//������ ������ �齺���̽��� �ƴ�
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
//�ʼ�, �ʼ�2, �߼�, ����, ����2�� �ϳ��� ���ڷ� ���� �ѱ� �Է� ����ü�� �ִ´�.
BOOL COMBI_Han_putch(HAN_INPUT_KEY_BUFFER *p, HCODE cho, HCODE cho2, HCODE jung, HCODE jong, HCODE jong2)
{
	HCODE code;

	//���۰� ����ϴٸ�
	if(p->size > (int)(strlen(p->buf)+3))
	{
		//�ʼ�, �߼�, ����, ����2 ��ġ��
		code=COMBI_Han_cho_cho2_jung_jong_jong2_to_Hstr(cho, cho2, jung, jong, jong2);
		*(p->p)=HCODE_LEFT_CHAR(code);	p->p++;
		*(p->p)=HCODE_RIGHT_CHAR(code);	p->p++;
		*(p->p)='\0';
		return TRUE;
	}
	else
	{
		//���۰� �����մϴ�.
		printf("\n���۰� �����մϴ�.\n");
		return FALSE;
	}
}
/*--------------------------------------------------------------------------*/
//ascii�ڵ带 �ѱ� �Է� ����ü�� �ִ´�.
//���� 8��Ʈ�� �����Ͱ� �ִ�.
BOOL COMBI_Eng_putch(HAN_INPUT_KEY_BUFFER *p, HCODE ascii)
{
	//���۰� ����ϴٸ�
	if(p->size > (int)(strlen(p->buf)+3))
	{
		*(p->p)=HCODE_LEFT_CHAR(ascii);	p->p++;
		*(p->p)='\0';
		return TRUE;
	}
	else
	{
		//���۰� �����մϴ�.
		return FALSE;
	}
}
/*--------------------------------------------------------------------------*/
//���ۿ� ����� ������ ���ڸ� �����Ѵ�.
//�ѱ��̸� �ѱ� 1���ڸ� �����Ѵ�.
void COMBI_Han_DelPreChar(HAN_INPUT_KEY_BUFFER *p)
{
	//*(p->p)=='0' �̴�
	if(p->p > p->buf+1)
	{
		//2���� �̻� ����
		//�ѱ����� �˻�
	
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
			//�ѱ���
			//����(2 byte)
			p->p-=2;
			*(p->p)='\0';
		}
		else
		{
			//�ѱ� �ƴ�
			//����
			p->p--;
			*(p->p)='\0';
		}


//		if((*(p->p-2) & 0x80)!=0)
//		{
//			//�ѱ���
//			//����(2 byte)
//			p->p-=2;
//			*(p->p)='\0';
//		}
//		else
//		{
//			//�ѱ� �ƴ�
//			//����
//			p->p--;
//			*(p->p)='\0';
//		}
	}
	else
	{
		//1���� �̸� ���� 
		if(p->p == p->buf)
		{
			//����� ���ڰ� ����
		}
		else
		{
			//1���ڰ� ����
			//����
			p->p--;
			*(p->p)='\0';
		}
	}
}
/*--------------------------------------------------------------------------*/
//�θ����� �Ǵ°�?(��, ��, ��, ��, ��, ��, ��)
//���� : ������� �;���. ��) ��+��(��),  ��+��(X)
HCODE COMBI_MixDoubleVowel(HCODE jung1, HCODE jung2)
{
	HCODE Table_COMBI_Han_jung[][3]={
		{(HCODE)0x85a1, (HCODE)0x8461, (HCODE)0x85c1},	//��+��=��
		{(HCODE)0x85a1, (HCODE)0x8481, (HCODE)0x85e1},	//��+��=��
		{(HCODE)0x85a1, (HCODE)0x87a1, (HCODE)0x8641},	//��+��=��
		{(HCODE)0x8681, (HCODE)0x84e1, (HCODE)0x86a1},	//��+��=��
		{(HCODE)0x8681, (HCODE)0x8541, (HCODE)0x86c1},	//��+��=��
		{(HCODE)0x8681, (HCODE)0x87a1, (HCODE)0x86e1},	//��+��=��
		{(HCODE)0x8761, (HCODE)0x87a1, (HCODE)0x8781},	//��+��=��
		{0,0}};
	register int i=0;

	while(Table_COMBI_Han_jung[i][0]!=0)
	{
		if(Table_COMBI_Han_jung[i][0]==jung1)
		{
			if(Table_COMBI_Han_jung[i][1]==jung2)
			{
				//�θ��� ��
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
	//���� �Էµ��ڵ� ���

	p->cho=0;
	p->cho2=0;
	p->jung=0;
	p->jong=0;
	p->jong2=0;
	p->status=0;	


	switch(p->status)
	{
		case 0:	//s0 : ó�� ����------------------------------------------------
			switch(COMBI_Han_Get_Type(code))
			{
			case HCODE_JAUM:	//����:
				//������ �Էµ�
				p->cho=code;
				//���� ���·� �̵�
				p->status=1;
				break;
			case HCODE_MOUM:	//����:
				//ó�� ���� ������ �Էµ�
				//�ʼ��� fill cdoe�� ä��� s2 : �ʼ��� �߼��� �ԷµȻ��·� �̵�
				//fill code=00001  1 00001 00010 00001 = 0x8441
				p->cho=0;
				p->jung=code;
				//���� ���·� �̵�
				p->status=2;
				break;
			case HCODE_BACKSPACE:	//BackSpace:
				//���� ���� ����
				COMBI_Han_DelPreChar(p);
				///p->cho=0;
				break;
			default:	//��Ÿ����
				//���� �Էµ��ڵ� ���
				COMBI_Eng_putch(p, code);
				///p->cho=0;
				break;
			}
			break;

	}
}









/*--------------------------------------------------------------------------*/
//������ �ѱ� �Է�
void COMBI_Han_Input(HAN_INPUT_KEY_BUFFER *p, HCODE code)
{
	HCODE temp_hcode;

	switch(p->status)
	{
		case 0:	//s0 : ó�� ����------------------------------------------------
			switch(COMBI_Han_Get_Type(code))
			{
			case HCODE_JAUM:	//����:
				//������ �Էµ�
				p->cho=code;
				//���� ���·� �̵�
				p->status=1;
				break;
			case HCODE_MOUM:	//����:
				//ó�� ���� ������ �Էµ�
				//�ʼ��� fill cdoe�� ä��� s2 : �ʼ��� �߼��� �ԷµȻ��·� �̵�
				//fill code=00001  1 00001 00010 00001 = 0x8441
				p->cho=0;
				p->jung=code;
				//���� ���·� �̵�
				p->status=2;
				break;
			case HCODE_BACKSPACE:	//BackSpace:
				//���� ���� ����
				COMBI_Han_DelPreChar(p);
				///p->cho=0;
				break;
			default:	//��Ÿ����
				//���� �Էµ��ڵ� ���
				COMBI_Eng_putch(p, code);
				///p->cho=0;
				break;
			}
			break;
		case 1:	//s1 : �ʼ��� �ԷµȻ���--------------------------------------------------
			switch(COMBI_Han_Get_Type(code))
			{
			case HCODE_JAUM:	//����:
				//������ �Էµ�
				//�Էµ� �ʼ��� ������,�������̶��
				if(p->cho2!=0)
				{
					//���� ������, �������� �ԷµǾ� ����
					//���� ������, �������� ����ϰ� �Էµ� code�� �ʼ����� ����
					COMBI_Han_putch(p, p->cho, p->cho2, 0, 0, 0);
					p->cho=code;
					p->cho2=0;
					///status=1;
				}
				else
				{
					//�ʼ� 1��(�⺻����)�� �Էµ�
					//�������� �Ǵ°�?(��,��,����,��)
					temp_hcode=COMBI_MixJaumCho(p->cho, code);
					if(temp_hcode!=0)
					{
						//�� ������
						p->cho2=code;
						///status=1;
					}
					else
					{
						//������(����)�� �Ǵ°�?(��,��,��,��,��,��,��,��,��,��,��)
						temp_hcode=COMBI_MixJaumJong(p->cho, code);
						if(temp_hcode!=0)
						{
							//��������
							//���� �ʼ��� �Է°��� �������� ����
							p->jong=p->cho;
							p->jong2=code;
							p->cho=0;
							///p->cho2=0;
							//�������·� �̵�: s3 : �ʼ�, �߼�, ������ �ԷµȻ���(���ڿϼ��ȵ�)
							p->status=3;
						}
						else
						{
							//�� ���� �ȵ�
							//���� �Էµ� ������ ����ϰ� ���� �Է°��� �������� ����
							COMBI_Han_putch(p, p->cho, p->cho2, 0, 0, 0);
							p->cho=code;
							p->cho2=0;
							///status=1;
						}
					}
				}
				break;
			case HCODE_MOUM:	//����:
				//������ �Էµ�
				//���� ������, �������� �ԷµǾ� ����
				p->jung=code;
				//���� ���·� �̵�
				p->status=2;
				break;
			case HCODE_BACKSPACE:	//BackSpace:
				//���� ���� ����
				if(p->cho2!=0)
				{
					p->cho2=0;
					p->status = 1;	// sunny
				}
				else
				{
					p->cho=0;
					//���� ���·� �̵�
					p->status=0;
				}

				break;
			default:	//��Ÿ����
				//�ʼ����
				COMBI_Han_putch(p, p->cho, p->cho2, 0, 0, 0);
				//���� �Էµ��ڵ� ���
				COMBI_Eng_putch(p, code);
				//ó�� ���·� �̵�
				p->cho=0;
				p->status=0;	
				break;
			}
			break;
		case 2:	//s2 : �ʼ��� �߼��� �ԷµȻ���--------------------------------------
			switch(COMBI_Han_Get_Type(code))
			{
				case HCODE_JAUM:	//����:
					//������ �Էµ�(����)
					p->jong=code;
					//���� ���·� �̵�
					p->status=3;
					break;
				case HCODE_MOUM:	//����:
					//������ �Էµ�
					//�θ����� �Ǵ°�?(��, ��, ��, ��, ��, ��, ��)
					temp_hcode=COMBI_MixDoubleVowel(p->jung, code);
					if(temp_hcode==FALSE)
					{
						//�θ����� �ȵȴ�.
						//���� ���� ����ϰ� s2 : �ʼ��� �߼��� �ԷµȻ��·� �̵�
						COMBI_Han_putch(p, p->cho, p->cho2, p->jung, 0, 0);
						p->cho=0;
						p->jung=code;
						///status=2;
					}
					else
					{
						//�θ����� �ȴ�.
						//�θ������� ��ģ��.
						p->jung=temp_hcode;
						///status=2;
					}
					break;
				case HCODE_BACKSPACE:	//BackSpace:

					if (p->cho == 0) //�ʼ��� ������
					{
						//���� ����(�߼�) ����
						p->jung=0;
						//���� ���¿��� �ʱ�ȭ��
						p->status=0;
					}
					else
					{
						//���� ����(�߼�) ����
						p->jung=0;
						//���� ����(�ʼ��� �Էµ� ����)�� �̵�
						p->status=1;
					}

//					//���� ����(�߼�) ����
//					p->jung=0;
//					//���� ����(�ʼ��� �Էµ� ����)�� �̵�
//					p->status=1;



					break;
				default:	//��Ÿ����
					//�ʼ�,�߼��� ����ϰ� ó�����·� �̵�
					COMBI_Han_putch(p, p->cho, p->cho2, p->jung, 0, 0);
					//���� �Էµ��ڵ� ���
					COMBI_Eng_putch(p, code);
					//ó�� ���·� �̵�
					p->cho=0;
					p->cho2=0;
					p->jung=0;
					p->status=0;	
					break;
			}
			break;
		case 3:	//s3 : �ʼ�, �߼�, ������ �ԷµȻ���(���ڿϼ��ȵ�)----------------------
			switch(COMBI_Han_Get_Type(code))
			{
			case HCODE_JAUM:	//����:
				//������ �Էµ�
				//�������� �ԷµǾ��ִٸ�
				if(p->jong2!=0)
				{
					//������ ���, ���� �Է��� �ʼ�����
					COMBI_Han_putch(p, p->cho, p->cho2, p->jung, p->jong, p->jong2);
					p->cho=code;
					p->cho2=0;
					p->jung=0;
					p->jong=0;
					p->jong2=0;
					//�������·� �̵� : s1 : �ʼ��� �ԷµȻ���
					p->status=1;
				}
				else
				{
					//������ �Ѱ��� �ԷµǾ� �ִ�.
					//��ħ����(������,������)�� �Ǵ°�?(������������������������������������������������������)
					temp_hcode=COMBI_MixJaumJong(p->jong, code);
					if(temp_hcode!=0)
					{
						//��ħ����(������,������)�� ��
						//���������� ����
						p->jong2=code;
						///status=3;
					}
					else
					{
						//��ħ����(������,������) �ȵ� 
						//���� ���� ����� �Էµ� ������ �ʼ��� �Է�
						COMBI_Han_putch(p, p->cho, p->cho2, p->jung, p->jong, 0);
						p->cho=code;
						p->cho2=0;
						p->jung=0;
						p->jong=0;
						p->jong2=0;
						//������ �Էµ� ���·� �̵�
						p->status=1;			
					}
				}
				break;
			case HCODE_MOUM:	//����:
				//������ �Էµ�
				//������ 1�� �ԷµǾ� �ִٸ�
				if(p->jong2==0)
				{
					//�ʼ�+�߼�(���), �ʼ�+�߼����� �����.
					COMBI_Han_putch(p, p->cho, p->cho2, p->jung, 0, 0);	
					//�ʼ�, �߼��� �Էµ� ���·�
					p->cho=p->jong;
					p->cho2=0;
					p->jung=code;
					p->jong=0;
					p->jong2=0;
					p->status=2;
				}
				else
				{
					//������,��������
					//�ʼ�+�߼�+���� ���, ����2=>�ʼ�+�߼�
					COMBI_Han_putch(p, p->cho, p->cho2, p->jung, p->jong, 0);
					//�ʼ�, �߼��� �Էµ� ���·�
					p->cho=p->jong2;
					p->cho2=0;
					p->jung=code;
					p->jong=0;
					p->jong2=0;
					p->status=2;
				}
				break;
			case HCODE_BACKSPACE:	//BackSpace:
				//���� ���� ����
				//����2�� ������
				if(p->jong2!=0)
				{
					//����2 ����
					p->jong2=0;
					//�ʼ�, �߼�, ������ �Էµ� ����
					///status=3;
				}
				else
				if (p->jung == 0)		//sunny ����ó��
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
					//���� ����
					p->jong=0;
					//�ʼ�, �߼��� �Էµ� ����
					p->status=2;
				}
				break;
			default:	//��Ÿ����
				//�ʼ�,�߼�, ������ ����ϰ� ó�����·� �̵�
				COMBI_Han_putch(p, p->cho, p->cho2, p->jung, p->jong, p->jong2);
				//���� �Էµ��ڵ� ���
				COMBI_Eng_putch(p, code);
				//ó�� ���·� �̵�
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
//�ƽ�Ű �Է��� �ѱ� �Է����� ��ȯ
HCODE COMBI_Key_to_HCODE(char key, BOOL shiftkey)
{
	typedef struct _ascii_to_han
	{
		HCODE	han1;	//
		HCODE	han2;	//����ƮŰ�� ��������
		char	type;	//����(1), ����(2)
	}KEY_BOARD_ASCII_TO_HAN;

	HCODE ret;
	KEY_BOARD_ASCII_TO_HAN keyboard_A_Z[]={
        {(HCODE)0xa041, 0, 1},				//��   	a
		{(HCODE)0x8741, 0, 2},				//��	b
        {(HCODE)0xc041, 0, 1},				//��	c
        {(HCODE)0xb441, 0, 1},				//��	d
        {(HCODE)0x9441, (HCODE)0x9841, 1},	//��,��	e
        {(HCODE)0x9c41, 0, 1},				//��	f
        {(HCODE)0xd041, 0, 1},				//��	g
        {(HCODE)0x85a1, 0, 2},				//��	h
        {(HCODE)0x84a1, 0, 2},				//��	i
        {(HCODE)0x84e1, 0, 2},				//��	j
        {(HCODE)0x8461, 0, 2},				//��	k
        {(HCODE)0x87a1, 0, 2},				//��	l
        {(HCODE)0x8761, 0, 2},				//��	m
        {(HCODE)0x8681, 0, 2},				//��	n
        {(HCODE)0x8481, (HCODE)0x84c1, 2},	//��,��	o
        {(HCODE)0x8541, (HCODE)0x8581, 2},	//��,��	p
        {(HCODE)0xa441, (HCODE)0xa841, 1},	//��,��	q
        {(HCODE)0x8841, (HCODE)0x8c41, 1},	//��,��	r
        {(HCODE)0x9041, 0, 1},				//��	s
        {(HCODE)0xac41, (HCODE)0xb041, 1},	//��,��	t
        {(HCODE)0x8561, 0, 2},				//��	u
        {(HCODE)0xcc41, 0, 1},				//��	v
        {(HCODE)0xb841, (HCODE)0xbc41, 1},	//��,��	w
        {(HCODE)0xc841, 0, 1},				//��	x
        {(HCODE)0x8661, 0, 2},				//��	y
        {(HCODE)0xc441, 0, 1},				//��	z
		{0,  0, 0}};
	static int flag_shift=0;
	static int shift_count=0;

	//Ű����κ��� �Է¹��� Ű�� �ѱ۷� ��ȯ�Ѵ�.
	if(gHanMode==COMBI_HAN_MODE)
	{
		//�ѱ� �Է� ���
		//�ҹ��ڷ� ��ȯ
		if('A' <= key && key <= 'Z')
		{
			if(shift_count > 2)
			{
				//���� �Է¹��ڴ� �ҹ��ڷ� �Է��Ѵ�.
				//����Ʈ�� ���� �빮�ڸ� �Է��ߴ�.
				flag_shift=1;
			}
			else
			{
				//���� �Է¸��� �빮�ڷ� �Էµȴ�.
				flag_shift=0;
			}
			key=key+('a'-'A');
			shiftkey=TRUE;
		}
		if('a' <= key && key <= 'z')
		{
			if(shiftkey==TRUE)
			{
				//����Ʈ Ű�� ��������
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
			//����ڰ� �ƴϴ�. ��ȣ���� �Է�
			//�״�� �����ش�.
			ret=(key<<8);
			return ret;
		}
	}
	else
	{
		//���� �Է¸��
		//�״�� �����ش�.
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
	//�ѱ� �Է� ���·� ����
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
					//ȭ��ǥ Ű
				break;
			case 13:	//Enter
				//����ü�� �ʱ�ȭ �մϴ�.
				HAN_INPUT_KEY_BUFFER_Clear(&key_buf);
				printf("\n");
				break;
			case 27:	//ESC
				exitwhile=TRUE;
				break;
			default:
				//�Է¹��� Ű�� �ѱ� Ű�� ��ȯ
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