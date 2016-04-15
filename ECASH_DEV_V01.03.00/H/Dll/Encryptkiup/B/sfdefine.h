//
//	sf_define.h
//

#ifndef __SF_DEFINE_H__
#define __SF_DEFINE_H__

#if	defined (CPU_16BIT)
#define		uint8				unsigned char
#define		uint16				unsigned int
#define		uint32				unsigned long int
#define		int32				long int

typedef		unsigned char		SF_BOOL;				//  1 bit
typedef		unsigned char		SF_BYTE;				//  8 bits
typedef		unsigned long int	SF_WORD;				// 32 bits
#elif	defined (PALM) | defined (LINUX)
#define		uint8				unsigned char
#define		uint16				unsigned short
#define		uint32				unsigned long int
#define		int32				long int

typedef		unsigned char		SF_BOOL;				//  1 bit
typedef		unsigned char		SF_BYTE;				//  8 bits
typedef		unsigned long int	SF_WORD;				// 32 bits
#else	// default
#define		uint8				unsigned char
#define		uint16				unsigned short
#define		uint32				unsigned int
#define		int32				int

typedef		unsigned char		SF_BOOL;				//  1 bit
typedef		unsigned char		SF_BYTE;				//  8 bits
typedef		unsigned int		SF_WORD;				// 32 bits
#endif

#define		SF_ALGID_MD2					5
#define		SF_ALGID_MD5					7
#define		SF_ALGID_SHA1					8


#ifndef true
#define		true				1
#endif
#ifndef false
#define		false				0
#endif

#endif
