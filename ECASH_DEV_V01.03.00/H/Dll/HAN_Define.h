/*************************************************************

  한글 관련 정의 헤더파일입니다.

  2001.6.2
  j22000@netian.com

**************************************************************/
#define HAN_DEFINE_H
/**************************************************************/
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
/**************************************************************/
#ifndef INT8
	typedef char		INT8;
#endif
#ifndef	INT16
	typedef short	      INT16;
#endif
//#ifndef INT32
//	typedef long		INT32;
//#endif
#ifndef UINT8
	typedef unsigned char     UINT8;
#endif
#ifndef UINT16
	typedef unsigned short    UINT16;
#endif
//#ifndef UINT32
//	typedef unsigned long     UINT32;
//#endif
/*--------------------------------------------------*/
#ifndef BOOL
	#define BOOL	int
#endif
#ifndef TRUE
	#define TRUE	1
#endif
#ifndef FALSE
	#define FALSE	0
#endif

/**************************************************************/
#define HCODE			INT16
/**************************************************************/
#define STR2HCODE(l, h)	 ((((UINT16)(l)) << 8) | (((UINT16)(h)) & 0x00ff))
#define HCODE2STR(l, h, code)  l=((code) >> 8) & 0x00ff;  h=(code) & 0x00ff
/**************************************************************/
