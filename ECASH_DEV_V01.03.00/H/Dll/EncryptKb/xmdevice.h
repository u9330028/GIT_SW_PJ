//
//	xmdevice.h
//		standard library for multiple devices
//		definition for device dependent functions
//
#ifndef	__XM_DEVICE_H__
#define __XM_DEVICE_H__

//	Device List
//	WIN32
//	WINCE
//	BREW
//	_DTWM		Intec device
//	PALM
//	HPLJ
//	DOS
//	POS_ARM		T-FINCE device (Hanaro - ARM)
//	MOBICOS		T-FINCE device (Mobicos - Mitsubishi)
//	CYBERNET	T-FINCE device (Cybernet - Hitachi)
//	WITHUS		T-FINCE device (Withus- Toshiba)
//	LINUX

//#define NO_GLOBAL
#define NO_INT64

#if	defined (_WIN32_WCE)
#define	WINCE
#endif


//	-----------------------------------------------------------------------------
//	BREW device
#if defined (AEE_SIMULATOR) || (!defined(AEE_SIMULATOR) && defined(DYNAMIC_APP))
#define	BREW

#include <AEEStdLib.h>
#include <AEEAppGen.h>
#include <AEEFile.h>		//	AEEFile Services

void * BREW_get_instance(AEECLSID cls_id);

//	end of BREW 
//	-----------------------------------------------------------------------------
//	-----------------------------------------------------------------------------
//	Intec device
#elif defined (_DTWM)
#include "bios.h"
#include "TWMLib.h"

// common definition

#define		NULL		cNULL

// string
#define		MEMSET		Xmemset
#define		MEMCPY		Xmemcpy
#define		MEMMOVE		Xmemcpy
#define		MEMCMP		Xmemcmp

#define		STRCPY		Xstrcpy
#define		STRCAT		Xstrcat

// memory
#define		MALLOC		Xmalloc
#define		FREE		Xfree
//	end of Intec 
//	-----------------------------------------------------------------------------
//	-----------------------------------------------------------------------------
//	Palm device
#elif defined (PALM)
#include <PalmOS.h>

#ifdef __GNUC__
#include "callback.h"
#endif

// string
#define		MEMSET(dest,ch,size)		MemSet((dest), (size), (ch))
#define		MEMCPY		MemMove
#define		MEMMOVE		MemMove
#define		MEMCMP		MemCmp

#define		STRCPY		StrCopy
#define		STRCAT		StrCat

// memory
#define		MALLOC		MemPtrNew
#define		FREE		MemPtrFree
//	-----------------------------------------------------------------------------
//      HP LaserJet
#elif   defined (HPLJ)         // HPLJ
#include <iof_stdio.h>
#include <iof_stdlib.h>
#include <iof_strings.h>

// string
#define         MEMSET          memset
#define         MEMCPY          memcpy
#define         MEMMOVE         memcpy
#define         MEMCMP          memcmp

#define         STRCPY          strcpy
#define         STRCAT          strcat

// memory
#define         MALLOC          malloc
#define         FREE            free

//	-----------------------------------------------------------------------------
//      MQX
#elif defined (MQX)
#include <mqx.h>
#include <bsp.h>

// string
#define         MEMSET						memset
#define         MEMCPY(dst, src, size)		_mem_copy(src, dst, size)
#define         MEMMOVE(dst, src, size)		_mem_copy(src, dst, size)
#define         MEMCMP						memcmp

#define         STRCPY						strcpy
#define         STRCAT						strcat

// memory
#define         MALLOC						_mem_alloc
#define         FREE						_mem_free

//	-----------------------------------------------------------------------------
//	Standard device
#else	// WIN32, WINCE, DOS, POS_ARM, CYBERNET, MOBICOS, LINUX, WITHUS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// string
#define		MEMSET		memset
#define		MEMCPY		memcpy
#define		MEMMOVE		memcpy
#define		MEMCMP		memcmp

#define		STRCPY		strcpy
#define		STRCAT		strcat

// memory
#define		MALLOC		malloc
#define		FREE		free
//	end of Standard 
//	-----------------------------------------------------------------------------
#endif


#if defined (WIN32) | defined (WINCE) | defined (BREW) | defined (DOS)
#define ENABLE_FILE
#endif

#if	defined (BREW)
#define	NO_GLOBAL
#define	NO_RAND_INIT
#endif

/*
#if defined (DOS) | defined (MOBICOS)
#define NO_INT64
#endif
*/

#if defined (MOBICOS)
#define CPU_16BIT
#pragma SECTION program prog_APP
#define NO_MALLOC
#endif

#if defined (CYBERNET)
#define CPU_16BIT
#define NO_MALLOC
#define	far
#define	near
int		GetTime(uint8 *str, int *tm);
#endif

#if defined (WITHUS)
#define CPU_16BIT
#define NO_MALLOC
#define	far
#define	near
#endif

#if	!defined (CPU_16BIT)
#define	far
#define	near
#define	USE_MALLOC
#endif

#if defined (WIN32) | defined (LINUX)
//#define ENABLE_RSA_PRI
#endif

#if	defined (PALM)
#undef	USE_MALLOC
#define NO_MALLOC
//#define ENABLE_RSA_PRI
#endif

#if defined (POS_ARM)
extern int curYear, curMonth, curDay;
#undef	USE_MALLOC
#define NO_MALLOC
#endif

#if defined (HPLJ)
#undef	USE_MALLOC
#define NO_MALLOC
#endif

#endif
