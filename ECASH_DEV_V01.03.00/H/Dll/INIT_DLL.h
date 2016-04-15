#if !defined(__INIT_DLL__INCLUDED_)
#define __INIT_DLL__INCLUDED_


/**********************************************************
*
* NH initial DLL import header for the image converter (specialized for TIFF)
*
*	(Import library [INIT_DLL.lib] should be also linked with this header included)
*
*	Development environment: Visual C++ 6
*	Calling convention: __cdecl
*	Name decoration: C
*
* Programmed by
*
*	Euisun Choi
*
*	Fundamental Technology Team, Nautilus Hyosung / Jun. 2005
*
* updated Sep. 15, 2005
**********************************************************/

/* Default constants for TIFF conversion module */
#define DEFAULT_TIFF_COMPRESSION_MODE CCITT4
#define DEFAULT_TIFF_BINARIZAION_LEVEL ENHANCE_NORMAL


#ifdef __cplusplus
extern "C" {
#endif

/********************************************************
	Function Description:
	Load and initialize the TIFF conversion module

	***
	*** I/O params.
	*** 
	RETURN: TRUE, if success, FALSE otherwise

	@ module_path: Path name which the TIFF conversion module resides.
		(Please use absolute directory path name with back slash('\\') or NULL, not relative one)

		ex) "C:\\Program Files\\NH_TIFF_DLL"

		If NULL, it is assumed that the TIFF conversion module resides at the same place where this
		initial DLL and main App exist.

	NOTE:	Normally, this function may be called once at program startup.
				This function call should be always followed by 'close_tiff_converter()'
				in a pairwise manner.
********************************************************/
BOOL init_tiff_converter(LPCTSTR module_path /* = NULL */);


/********************************************************
	Function Description:
	Convert check image file (BMP) to TIFF (v6.0 spec, CCITT Group4 FAX)

	***
	*** TIFF compression mode:
	*** (TIFF v6.0 spec. supports various compression methods)
	***/
	enum TIFF_COMPRESSION{
		Default				 = 0,					// don't care !
		None				 = 1,					// no compression
		RLE						= 2,				// CCITT: Modified Huffman coding
		CCITT4				 = 4,					// CCITT Group 4 FAX (adopted by US Check 21 Acts.)
		PackBits			 = 32773,			// Macintosh Run Length Encoding
		LZW					 = 5,					// Lempel-Ziv-Welch Alg.
		JPEG				 = 7					// embedded JPEG
	}; /*

	***
	*** I/O params.
	*** 
	RETURN: TRUE, if success, FALSE otherwise

	@ src_filename: source image filename

	@ dst_filename: target TIFF filename
		(The converted image file can be stored at any place,
		where the destination folder exists or not.)

	@ tiff_compression: TIFF compression methods
		(Please use one of the above enumeration elements)
		(If CSU/CSM issued, absolutely CCITT4 :-)

	@ binarization_level: binarization (B/W) level
		(Please use one of the following enumeration elements) */

		enum BIN_JIN_MODE{
			AVG_THRESH = 0,			// average thresholding binarization
			ENHANCE_NORMAL,		// used in NH Jiro system
			ENHANCE_THICK,			// thicken document characters (may be slightly dark)
			ENHANCE_THICKER,		// more thicken ... ( ... darker)

			/* not available yet */
			ENHANCE_RESERVED1,		// reserved for future implementation
			ENHANCE_RESERVED2		// reserved ...
		}; /*

	// Comments on binarization:
	Basically, the CCITT Group4 FAX TIFF compression requires real B/W 1-bit packed image data.
	Therefore, in this case some binarization process should be performed before converting.

  // GENERAL IMAGE TYPE
  // Set @tiff_compression to default (don't care) and binarization_level = -1
  // If IMAGE TYPE is JPEG, set the @tiff_compression as JPEG QUALITY (90 recommeded <1-100>)

	NOTE: If it is needed to have an image file of different format from TIFF for any reason,
			just give the target image filename with the extension of that format. That's all !
			ex) *.jpg, *.gif, *.pcx, ...

			In this case, the option @tiff_compression will be ignored, and you may set the option
			@binarization_level = -1 to turn off the binarization process, if necessary.

			Supported type (input/output):
				BMP, JPG, GIF, TIFF, PNG, MNG, PNG, ICO, PCX, TGA,
				WMF, EMF, J2K, J2P, JBG, JPC, PGX, RAS, PNM
			
********************************************************/
BOOL NH_tiff_converter(LPCTSTR src_filename,
										LPCTSTR dst_filename,
										int tiff_compression /* = DEFAULT_TIFF_COMPRESSION_MODE */,
										int binarization_level /* = DEFAULT_TIFF_BINARIZAION_LEVEL */);

/********************************************************
	Function Description:
	Unload the TIFF conversion module

	NOTE: Normally, this function may be called once at the end of program.
 ********************************************************/
void close_tiff_converter();

#ifdef __cplusplus
}
#endif

#endif // !defined(__INIT_DLL__INCLUDED_)
