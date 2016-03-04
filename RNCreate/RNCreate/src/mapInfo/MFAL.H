/*****************************************************************************
*       Copyright (c) 1989-2000, MAPINFO CORPORATION
*       All  rights reserved. 
*       Confidential Property of MAPINFO CORPORATION
*
* $Workfile: MFAL.H $ $Revision: 23 $ $Modtime: 4/12/02 2:28p $
*
* Description:
*   Map File Access Library public header
*
* $Log: /myst/MFAL/SRC/MFAL.H $
* 
* 23    4/12/02 2:40p Anveselo
* Added new ellipsoid PZ90 (requested by Russian partners). The number
* for this ellipsoid is 52. Tables, with this ellipsoid should have a
* version of 700.
* 
* 22    12/06/01 9:51a Anveselo
* Added support for new Double Stereograpic projection. Currently it
* supports only oblique case, might need to implement polar aspect later,
* when we get documentation. Also defined new datum - ATS77 (which uses
* ATS77 ellipsoid). Added new entries in PRJ file for Prince Edward
* Island and New Brunswick.
* 
* 20    10/18/01 4:01p Derek
* Added bit flag fields to MI_SYMBOLOGY to tell which fields are set
* 
* 19    10/11/01 5:29p Derek
* Changed MI_SYMBOLOGY structure to add support for FONT and CUSTOM
* Symbols.  Changed version number to 6510
* 
* 18    3/23/01 2:51p Anveselo
* Added support for new projection: Cassini-Soldner
* 
* 17    3/16/01 5:46p Ericb
* Added structure and define support for Collections and Multipoints
* 
* 16    3/15/01 2:50p Anveselo
* Added support for Lambert Azimuthal Equal-Area projection (Oblique
* aspect). Up to now we had only polar aspect of this projection. 
* 
* 13    00/04/30 21:47 Builder
* Added "#define ELLIPSOID_EVEREST_PAKISTAN 50", and "#define
* ELLIPSOID_ATS77 51".  Updated NUM_ELLIPSOIDS from 50 to 52.  Defines
* provided by Eric Bl.
* 
* 12    3/10/00 6:08p Ericb
* Removed an extra character before the  comment
* 
* 11    3/10/00 6:07p Ericb
* Added POLYCONIC to CSYSPARMS union and removed 16-bit APIs
* 
* 10    3/10/00 2:24p David
* Added support for Oblique Azimuthal Equidistant projection.
* 
* 8     9/24/99 12:54p Mark
* Merged in changes for the additional coordinate systems from Shelter
* 
* 7     5/18/99 10:54a Sandra
* Added new ellipsoid to Mfal
* Added a get row with bounds
* 
* 4     4/28/98 11:46a Paulc
* Fixed "string resource not found" error.  Added ability to open LINKED
* tables read-only
* 
* 3     4/21/98 9:34a Paulc
* Changed the CURRENT_API_VERSION 
* 
* 2     4/09/98 1:48p Paulc
* removed internal functions and moved them to MFALFunc.h
* 
* 1     98/03/06 12:07p Builder
* Initial sources for the Staten project based on midway BUILD3 patch 2
* sources (as of 1990306).
* 
*    Rev 4.1.1.2   05 Feb 1998 13:49:28   PAULC
* removed MFAL_CURRENT_VERSION and added CURRENT_API_VERSION 4520
* 
*    Rev 4.1.1.1   15 Jan 1998 15:36:06   PAULC
* Added function to translate a blob to an MIObject, added freeblob, numpoint
* Fixed Pragma for unix.  Added extended fonts and transparrent fill.
* 
*    Rev 4.1.1.0   15 Dec 1997 18:38:46   BUILDER
* No change.
* 
*    Rev 4.1   12 Dec 1997 17:22:04   BUILDER
* Update for liberty to MAINLINE merge.
* 
*    Rev 4.0.2.6   11 Oct 1997 16:12:04   PAULC
* See MFAL Final Merge list in the MapInfo Professional Products database in 
* 
*    Rev 4.0.2.5   25 Jul 1997 10:35:32   ERICB
* Changed number of points in MI_POLY to long. Changed pragma packs to be
* conditional to Win16 or Win32
* 
* 
*    Rev 4.0.2.4   18 Jul 1997 16:04:20   PAULC
* added new function MIGetDefaultBounds
* 
*    Rev 4.0.2.2   02 Jul 1997 14:46:42   DAVID
* Incorporate MFAL into MIMB build
*****************************************************************************/


#ifndef _MFAL_H
#define _MFAL_H

#ifdef __cplusplus 
extern "C"{
#endif

namespace MapInfo70 //namespace add by duliangliang
{
	
#ifndef WIN32 // add by changgy for ms
#define WIN32 1
#endif

#include <stdlib.h>

#if !(defined(unix) || defined(__unix__) || defined(USG))
	#pragma warning(disable:4103)
	#if defined (_WIN32) || defined (WIN32)
	 
	/* Pack to 8-byte boundaries */
	#pragma pack(push)
	#pragma pack(8)
#else 
	/* Pack to 1-byte boundaries */
	#pragma pack(push)
	#pragma pack(1)
#endif
#endif /* !(defined(unix) || defined(__unix__) || defined(USG)) */

#if defined(__HIGHC__)
  #pragma Align_members(1)
#endif

/****************************************************************************/
/* Data Types & Handy Constants                                             */
/****************************************************************************/

/* The #define MFAL_CURRENT_VERSION 300 has been changed to #define */
/* CURRENT_API_VERSION 4520*/
/* This was done for two reasons, 1) To prevent using an older application */
/* with this new DLL. Many updates have been made, requiring users of this */
/* api to recompile 2) Prevent users from passing MFAL_CURRENT_VERSION to */
/* MICreate Table. The api version has nothing to do with the table version.*/
#define CURRENT_API_VERSION 6510

#if defined(BOOLEAN)
#undef BOOLEAN
#endif
#define BOOLEAN short
typedef long     TABLEID, *pTABLEID;
typedef unsigned long MI_COLOR;

#if !defined(FALSE)
  #define FALSE    (0)
  #define TRUE     (!FALSE)
#endif

//#if WIN32 || defined(__HIGHC__)
  #define __huge
  #define __near
  #define __export
  #if defined(__HIGHC__)
    #define __far
    #define __pascal
  #endif
//#endif

#if defined(unix) || defined(__unix__) || defined(USG)
#define __huge
#define __far
#define __near
#define __pascal
#define __export
#define _halloc(x,y)  malloc(((long)x) * ((long)y))
#define _hfree free
#if !defined(min)
  #define min(x,y) ((x) < (y)) ? (x) : (y)
  #define max(x,y) ((x) > (y)) ? (x) : (y)
#endif
#endif

#define SZ_FONTNAME       32

/****************************************************************************/
/*  Definitions adjusted for platform compatibility by Erik Olson           */  
/*  12-19-93,                                                               */
/*  These definitions come from XVTX.H                                      */
/****************************************************************************/
#if !MI_MFAL
	#ifdef macintosh
	  #define SZ_DIRECTORY  224 /* 256(max) - 32(filenamelen) */
	  #define SZ_FILENAME   32
	  #define SZ_FILEROOT   (32-6)                /* filename - len(".Index")   */
 #elif defined(unix) || defined(__unix__) || defined(USG) 
	  #define SZ_DIRECTORY  224 /* 256(max) - 32(filenamelen) */
	  #define SZ_FILENAME   32
	  #define SZ_FILEROOT   (32-4)                /* filename - len(".XXX")   */
	#else
	  #if defined(WIN32)
	    #define SZ_DIRECTORY  255
	    #define SZ_FILENAME   255
	    #define SZ_FILEROOT   255
	  #else
	    #define SZ_DIRECTORY  131 /* 144(max) - 13(filenamelen) */
	    #define SZ_FILENAME   13
	    #define SZ_FILEROOT   (13-4)                /* filename - len(".XXX")     */
	  #endif
#endif

	#if defined(WIN32)
	  #define SZ_PATH       _MAX_PATH    /* MS C9 define */
	#else
	  #define SZ_PATH       (SZ_DIRECTORY + SZ_FILENAME)
	#endif
#endif

/****************************************************************************/
/* Data Type Defintions                                                     */
/****************************************************************************/

#undef DT_NONE  //±ÜÃâÓëmapabcµØÍ¼ÒýÇæ³åÍ»(add by duliang)
 
#if !defined(DT_NONE)

#define MODE_READ    1    /* shareable read only binary */
#define MODE_RDWR    2    /* read/write deny read, binary */

#define DT_NONE        0                      /* data types                 */
#define DT_CHAR        1
#define DT_DECIMAL     2
#define DT_INTEGER     3
#define DT_SMALLINT    4
#define DT_DATE        5
#define DT_LOGICAL     6
#define DT_FLOAT       8

typedef struct DPNT {
  double x;
  double y;
} DPNT, *pDPNT;

typedef struct DRECT {
  double x1;
  double y1;
  double x2;
  double y2;
} DRECT, *pDRECT;

typedef struct LPNT {
  long x, y;
} LPNT, __huge * pLPNT;

typedef struct LRECT {
  long x1, y1, x2, y2;
} LRECT, *pLRECT;

/****************************************************************************/
/* END of definitions from XVTX.H                                           */
/****************************************************************************/

typedef enum PAT_STYLE {
    PAT_HOLLOW = 1,
    PAT_SOLID,
    PAT_HORZ,
    PAT_VERT,
    PAT_FDIAG,
    PAT_BDIAG,
    PAT_CROSS,
    PAT_DIAGCROSS,
    /** other patterns don't have explicit enums **/
    PAT_MAX = 1024
} PAT_STYLE;

typedef enum PEN_STYLE {        /* pen style (must be P_SOLID) */
    P_SOLID = 0                 /* solid */
} PEN_STYLE;

#endif

#define ATTRIB_NAME_LEN            32
#define MAX_RECORDLENGTH           4000
#define MAX_CHAR_FIELD_WIDTH       254
#define MAX_DECIMAL_FIELD_WIDTH    20
#define MAX_CSYS_LENGTH            300
#if !defined(_H_MIOBJECT)

/****************************************************************************/
/* These definitions come from  MIOBJECT.H                                  */
/****************************************************************************/

#define T_JUSTIFY_LEFT    0
#define T_JUSTIFY_CENTER  1
#define T_JUSTIFY_RIGHT   2
#define T_JUSTIFY_FULL    3

#define T_SPACING_1       0
#define T_SPACING_1POINT5 1
#define T_SPACING_2       2

/****************************************************************************/
/* END of definitions from  MIOBJECT.H                                      */
/****************************************************************************/

#endif  

/****************************************************************************/
/* Indexed Search Defines                                                   */
/****************************************************************************/
#define MFAL_SEARCH_FIRST 1
#define MFAL_SEARCH_NEXT 2

#define MFAL_SEARCH_ERROR 0
#define MFAL_SEARCH_FOUND 1
#define MFAL_SEARCH_NOTFOUND 2
#define MFAL_SEARCH_INVALIDNEXT 3

/****************************************************************************/
/* Metadata Definitions                                                     */
/****************************************************************************/

#define MAX_METADATA_KEY   256
#define MAX_METADATA_VALUE 256


#ifndef _H_CPF
typedef struct CPF_HDR {
  double       LongitudeBase;
  double       LatitudeBase;
  double       LongitudeMultiplier;
  double       LatitudeMultiplier;
  short        CoordinateSize;
  short        KeyLength;
  short        PaddedKeyLength;
  long         MaximumBinaryKeyValue;
  char         KeyColumn[ATTRIB_NAME_LEN];
} CPF_HDR, *pCPF_HDR;
#endif

/****************************************************************************/
/* UNIT definitions                                                         */
/*                                                                          */
/* These definitions come from  MIEXTENS.H                                  */
/****************************************************************************/
#if !defined(UNIT_MILE)

#define UNIT_MILE             0
#define UNIT_KM               1
#define UNIT_INCH             2
#define UNIT_FOOT             3
#define UNIT_YARD             4
#define UNIT_MM               5
#define UNIT_CM               6
#define UNIT_M                7
#define UNIT_SURVEY_FOOT      8
#define UNIT_NAUTICAL_MILE    9
#define UNIT_TWIP             10
#define UNIT_POINT            11
#define UNIT_PICA             12
#define UNIT_DEGREE           13
#define UNIT_SQ_MILE          14
#define UNIT_SQ_KM            15
#define UNIT_SQ_INCH          16
#define UNIT_SQ_FOOT          17
#define UNIT_SQ_YARD          18
#define UNIT_SQ_MM            19
#define UNIT_SQ_CM            20
#define UNIT_SQ_M             21
#define UNIT_SQ_SURVEY_FOOT   22
#define UNIT_SQ_NAUTICAL_MILE 23
#define UNIT_SQ_TWIP          24
#define UNIT_SQ_POINT         25
#define UNIT_SQ_PICA          26
#define UNIT_SQ_DEGREE        27
#define UNIT_ACRE             28
#define UNIT_HECTARE          29
#define UNIT_LINK             30
#define UNIT_CHAIN            31
#define UNIT_ROD              32
#define UNIT_SQ_LINK          33
#define UNIT_SQ_CHAIN         34
#define UNIT_SQ_ROD           35
#define UNIT_PERCH            36
#define UNIT_ROOD             37

typedef short UNIT, *pUNIT;

/****************************************************************************/
/* END of definitions from  MIEXTENS.H                                      */
/****************************************************************************/

#endif
/****************************************************************************/
/* End of UNIT defintions                                                   */
/****************************************************************************/

/****************************************************************************/
/* ERROR CODE definitions                                                   */
/****************************************************************************/
#if !defined(_H_MIERRORS)

#define ERR_FPE_UNKNOWN           478
#define ERR_ACCESS_FILE           523
#define ERR_ACCESS_INDEX_FILE     388  /*"could not access index file"*/
#define ERR_ACCESSOBJECT          302
#define ERR_BADVERSIONWORKSPACE   330  /* Cannot open a later version wksp.  */
#define ERR_BAD_CHARSET          1283
#define ERR_BAD_COMMAND           599
#define ERR_BAD_DBTYPE            610
#define ERR_BAD_INDEX_FILE_VERSION 972
#define ERR_CANNOT_DELETE_ROW     882
#define ERR_CANNOT_FETCH_DELETED_ROW 1925
#define ERR_CANNOT_INDEX_EDITED_TABLE 718
#define ERR_CANNOT_WRITE_ROW      881
#define ERR_CANT_ACCESS_FILE      825
#define ERR_CANT_CREATE_TAB_DEF   754
#define ERR_CANT_OPERATE_ON_TABLE 683
#define ERR_CANT_PREPARE_DB       755
#define ERR_CANT_REMOVE_FILE      864
#define ERR_CANT_RENAME_EXISTING  859
#define ERR_COLUMN_NOT_INDEXED    873
#define ERR_COULDNT_CONVERT_DATA  854
#define ERR_CPF_BAD_MAGIC        1900
#define ERR_CPF_CANNOT_READ_VERSION  1901
#define ERR_CPF_ERR_READING_FILE 1902
#define ERR_CPF_ERR_WRITING_FILE 1903
#define ERR_CPF_INVALID_POINT    1904
#define ERR_CPF_INVALID_KEY_VALUE 1905
#define ERR_CREATE_INDEX          389  /*"could not create index file"*/
#define ERR_CREATE_NEW_TABLE      497
#define ERR_DATATYPE_MISMATCH     906
#define ERR_DBINIT_NEEDSPREPARE   530
#define ERR_EMPTY_OBJECT          894
#define ERR_EMPTY_TABLE_NAME      905
#define ERR_ERRINWORKSPACE        328
#define ERR_EXPR_NOT_IDENT        689
#define ERR_EXPR_NOT_STRING       694
#define ERR_FCN_ARG_RANGE         644
#define ERR_FIELDNAME_RESERVED    675
#define ERR_FIELDNOTFOUND         414  /*"Field %s not found."*/
#define ERR_FILEMGR_BADMODE       964
#define ERR_FILEMGR_EEXIST        369  /*"FILEMGR: error EEXIST."*/
#define ERR_FILEMGR_FILENOTFOUND  370  /*"FILEMGR: file %s not found"*/
#define ERR_FILEMGR_NOTOPEN       366  /* "FILEMGR: ERROR - file %s not open for access*/
#define ERR_FILEMGR_NOTREGISTERED 367  /* "FILEMGR: ERROR - file #%u not registered for access"*/
#define ERR_FILEMGR_OPENINGFILE   371  /*"FILEMGR: error %d opening file %s"*/
#define ERR_FILEMGR_TOOMANYFILES  368  /*"FILEMGR: Too many files open.  Could not open %s*/
#define ERR_FILE_ALREADY_OPEN     420
#define ERR_FILE_ALREADY_REGISTERED 432
#define ERR_FPE_OVERFLOW          471
#define ERR_INDEX_EXISTS          390  /*"Index for field %s already exists"*/
#define ERR_INTERNAL_ERROR        479
#define ERR_INVALID_CLAUSE        851
#define ERR_INVALID_FMT           674
#define ERR_INVALID_KEY_TYPE      508
#define ERR_INVALID_METADATA_KEY 1928
#define ERR_LINKS_IN_WHERE        401  /* Invalid WHERE clause */
#define ERR_MEM_ALLOC_FAIL        632
#define ERR_MEM_REALLOC_FAIL      633
#define ERR_MFAL_NOT_INITIALIZED 1929
#define ERR_MFAL_VERSION_MISMATCH 1930
#define ERR_MISSING_DBTYPE        714
#define ERR_MTFINTL_BADXLOCALE   8844
#define ERR_MTFINTL_DEFLOCALE    8843
#define ERR_MTFINTL_XLOCALEMOD   8845
#define ERR_NO_METADATA          1927
#define ERR_NOTENOUGHMEM          304  /* not enough memory to complete op   */
#define ERR_NOTVALIDWORKSPACE     329  /* File is not a valid workspace      */
#define ERR_NOT_VALID_WKS         522
#define ERR_NOT_VALID_XLS21       521
#define ERR_NO_MAP_EXISTS        1920
#define ERR_NULL_SELECTION        589
#define ERR_OBJECT_NOT_INSERTED   428
#define ERR_OPEN_INDEX            387  /*"could not create index"*/
#define ERR_OPEN_STRUCTURE_CHANGED 966
#define ERR_OPEN_TABLE            386  /*"could not open table"*/
#define ERR_PARSE                 615
#define ERR_PASSWORD_PROTECTED    876
#define ERR_PLINE_TOO_COMPLEX     899
#define ERR_READ_ONLY             891
#define ERR_READ_VERSION          503
#define ERR_REGION_TOO_COMPLEX    900
#define ERR_SYNTAXERROR           301
#define ERR_TABLE_ALREADY_OPEN    362  /* A table named %s is already open */
#define ERR_TABLE_IS_POINTFILE   1926
#define ERR_TABLE_IS_RASTER      1921
#define ERR_TABLE_IS_RELATIONAL  1922
#define ERR_TABLE_NO_COLS         321  /* no columns in a table              */
#define ERR_TABLE_NOT_RELATIONAL 1923
#define ERR_OPEN_TABLES_EXIST    1924
#define ERR_TOO_MANY_FIELDS       877
#define ERR_UNABLE_ACCESS_FILE    373  /*"Error:  unable to access %s file"*/
#define ERR_UNABLE_ACCESS_INDEX   501
#define ERR_UNABLE_OPEN_TABLE     505
#define ERR_XLSWKS_RANGENOTDEF    528
#define ERR_XLSWKS_RANGETOOBIG    529
#define ERR_XLSWKS_SPANSHEET      584
#define ERR_XLSWKS_STORINGFMTS    585
#define ERR_TABFILE_OUTDATED     1931
#define ERR_CANT_STAT            1932
#define ERR_TABLE_IS_SEAMLESS          1933
#define ERR_TABLE_TYPE_IS_UNSUPPORTED  1934
#define ERR_PENFILE_TOO_BIG            1935
#define ERR_TABLE_IS_LINKED            1936
#define ERR_TABLE_IS_QUERY             1937
#define ERR_TABLE_IS_ODBC              1938
#endif
/****************************************************************************/
/* End of ERROR CODE definitions                                            */
/****************************************************************************/

/****************************************************************************/
/* Character Set Definitions                                                */
/*                                                                          */
/* These definitions come from  SYSCHAR.H                                  */
/****************************************************************************/

#if !defined(CHARSET_NONE)

#define CHARSET_NONE           -1
#define CHARSET_NEUTRAL        0          /* Treat as if ASCII-7: never convert, etc. */

/* Unicode, vol.I, p. 467ff: Unicode Encoding to ISO 8859 Mappings */
#define CHARSET_ISO8859_1      1          /* ISO 8859-1 (Latin-1) */
#define CHARSET_ISO8859_2      2          /* ISO 8859-2 (Latin-2) */
#define CHARSET_ISO8859_3      3          /* ISO 8859-3 (Latin-3) */
#define CHARSET_ISO8859_4      4          /* ISO 8859-4 (Latin-4) */
#define CHARSET_ISO8859_5      5          /* ISO 8859-5 (English and Cyrillic-based) */
#define CHARSET_ISO8859_6      6          /* ISO 8859-6 (English and Arabic) */
#define CHARSET_ISO8859_7      7          /* ISO 8859-7 (English and Greek) */
#define CHARSET_ISO8859_8      8          /* ISO 8859-8 (English and Hebrew) */
#define CHARSET_ISO8859_9      9          /* ISO 8859-9 (Latin-5: Western Europe and Turkish) */

/* Unicode, vol.I, p. 519ff: Microsoft Windows Character Sets */
#define CHARSET_WLATIN1       10          /* Windows Latin-1 (Code Page 1252, a.k.a. "ANSI") */
#define CHARSET_WLATIN2       11          /* Windows Latin-2 (CP 1250) */
#define CHARSET_WARABIC       12          /* Windows Arabic (CP 1256) */
#define CHARSET_WCYRILLIC     13          /* Windows Cyrillic (CP 1251) */
#define CHARSET_WGREEK        14          /* Windows Greek (CP 1253) */
#define CHARSET_WHEBREW       15          /* Windows Hebrew (CP 1255) */
#define CHARSET_WTURKISH      16          /* Windows Turkish (CP 1254) */

/* Windows Far Eastern character sets */
#define CHARSET_WTCHINESE     17          /* Windows Big 5 ("Traditional": Taiwan, Hong Kong) */
#define CHARSET_WSCHINESE     18          /* Windows ?? ("Simplified": China) */
#define CHARSET_WJAPANESE     19          /* Windows Shift JIS X0208 (Japan) */
#define CHARSET_WKOREAN       20          /* Windows KS C5601 (Korea) */

/* Unicode, vol.I, p. 509ff: Unicode Encoding to Macintosh */
#define CHARSET_MROMAN        21          /* Mac Standard Roman */
#define CHARSET_MARABIC       22          /* Mac Arabic */
#define CHARSET_MGREEK        23          /* Mac Greek: ISO 8859-7 */
#define CHARSET_MHEBREW       24          /* Mac Hebrew: extension of ISO 8859-8 */

/* Other Macintosh character sets, including Far Eastern */
#define CHARSET_MCENTEURO     25          /* Mac Central European */
#define CHARSET_MCROATIAN     26          /* Mac Croatian */
#define CHARSET_MCYRILLIC     27          /* Mac Cyrillic */
#define CHARSET_MICELANDIC    28          /* Mac Icelandic */
#define CHARSET_MTHAI         29          /* Mac Thai: TIS 620-2529 */
#define CHARSET_MTURKISH      30          /* Mac Turkish */
#define CHARSET_MTCHINESE     31          /* Mac Big 5 ("Traditional": Taiwan, Hong Kong) */
#define CHARSET_MJAPANESE     32          /* Mac Shift JIS X0208 (Japan) */
#define CHARSET_MKOREAN       33          /* Mac KS C5601 (Korea) */

/* Unicode, vol.I, p. 536ff: Unicode to PC Code Page Mappings (Latin) */
#define CHARSET_CP437         34           /* IBM Code Page 437 ("extended ASCII") */
#define CHARSET_CP850         35           /* IBM CP 850 (Multilingual) */
#define CHARSET_CP852         36           /* IBM CP 852 (Eastern Europe) */
#define CHARSET_CP857         37           /* IBM CP 857 (Turkish) */
#define CHARSET_CP860         38           /* IBM CP 860 (Portugeuse) */
#define CHARSET_CP861         39           /* IBM CP 861 (Icelandic) */
#define CHARSET_CP863         40           /* IBM CP 863 (French Canada) */
#define CHARSET_CP865         41           /* IBM CP 865 (Norway) */

/* Unicode, vol.I, p. 546ff: Unicode to PC Code Page Mappings (Greek,Cyrillic,Arabic) */
#define CHARSET_CP855         42           /* IBM CP 855 (Cyrillic) */
#define CHARSET_CP864         43           /* IBM CP 864 (Arabic) */
#define CHARSET_CP869         44           /* IBM CP 869 (Modern Greek) */

/* Lotus proprietary character sets */
#define CHARSET_LICS          45           /* Lotus International: for older Lotus files */
#define CHARSET_LMBCS         46           /* Lotus MultiByte: for newer Lotus files */
#define CHARSET_LMBCS1        47           /* Lotus MultiByte group 1: newer Lotus files */
#define CHARSET_LMBCS2        48           /* Lotus MultiByte group 2: newer Lotus files */

/* Another Macintosh Far Eastern character set */
#define CHARSET_MSCHINESE     49           /* Macintosh ?? ("Simplified": China) */

/* UNIX Far Eastern character sets */
#define CHARSET_UTCHINESE     50           /* UNIX ?? ("Traditional": Taiwan, Hong Kong) */
#define CHARSET_USCHINESE     51           /* UNIX ?? ("Simplified": China) */
#define CHARSET_UJAPANESE     52           /* UNIX Packed EUC/JIS (Japan) */
#define CHARSET_UKOREAN       53           /* UNIX ?? (Korea) */

/* More Windows code pages (introduced by Windows 95) */

#define CHARSET_WTHAI         54           /* Windows Thai (CP 874) */
#define CHARSET_WBALTICRIM    55           /* Windows Baltic Rim (CP 1257) */
#define CHARSET_WVIETNAMESE   56           /* Windows Vietnamese (CP 1258) */

#define NUMBER_OF_CHARSETS    57           /* # character sets defined  */

/****************************************************************************/
/* END of definitions from  SYSCHAR.H                                      */
/****************************************************************************/

#endif  /* !defined(CHARSET_NONE) */

#define MAX_CHARSETNAME_LEN   40

/****************************************************************************/
/* End of Character Set Definitions                                         */
/****************************************************************************/
#ifndef XVT_INCL_TYPE     
typedef unsigned char UCHAR;
typedef unsigned int UINT;
#endif 
/****************************************************************************/
/* CSYS definitions                                                         */
/* These definitions come from CSYS.h                                       */
/****************************************************************************/
#define SRV_MAX_MI_TABLE_COL_WIDTH  MAX_CHAR_FIELDWIDTH /* 254 From Tabbase.h */

#if !defined(_H_CSYS)
/****************************************************************************/
/* Coefficients used for affine transformations (rubber-sheeting).          */
/****************************************************************************/
/* We can still use this with GetDatumInfo. */
#define DATUM_IGNORE      0                   /* don't do datum conversion  */
#define DATUM_NAD27       62 
#define DATUM_NAD83       74  
#define DATUM_WGS84       104 
#define DATUM_USER        999                 /* user-defined datum         */

typedef struct AFFINE_INFO {
  double Alpha[6];
  double Beta[6];
} AFFINE_INFO, *pAFFINE_INFO;

/****************************************************************************/
/* This list was copied from Defense Mapping Agency Technical Report        */
/* (DMA TR) 8350.2, "Department of Defense World Geodetic System 1984 --    */
/* Its Definition and Relationships with Local Geodetic Systems".           */
/*                                                                          */
/* Do NOT renumber the entries in this list!  Add new entries at the end.   */
/* The list must be in the same order as the ellipsoid table in CSYSDAT.C.  */
/****************************************************************************/
#define ELLIPSOID_GRS80              0
#define ELLIPSOID_WGS72              1
#define ELLIPSOID_AUSTRALIAN         2
#define ELLIPSOID_KRASSOVSKY         3
#define ELLIPSOID_INTERNATIONAL      4
#define ELLIPSOID_HAYFORD            5
#define ELLIPSOID_CLARKE80           6
#define ELLIPSOID_CLARKE66           7
#define ELLIPSOID_MOD_CLARKE66       8
#define ELLIPSOID_AIRY               9
#define ELLIPSOID_BESSEL            10
#define ELLIPSOID_EVEREST           11
#define ELLIPSOID_SPHERE            12
#define ELLIPSOID_MOD_AIRY          13
#define ELLIPSOID_MOD1_BESSEL       14
#define ELLIPSOID_MOD1_CLARKE80     15
#define ELLIPSOID_MOD2_CLARKE80     16
#define ELLIPSOID_MOD1_EVEREST      17
#define ELLIPSOID_FISCHER60         18
#define ELLIPSOID_MOD_FISCHER60     19
#define ELLIPSOID_FISCHER68         20
#define ELLIPSOID_GRS67             21
#define ELLIPSOID_HELMERT           22
#define ELLIPSOID_HOUGH             23
#define ELLIPSOID_SOUTH_AMERICAN    24
#define ELLIPSOID_WAR_OFFICE        25
#define ELLIPSOID_WGS60             26
#define ELLIPSOID_WGS66             27
#define ELLIPSOID_WGS84             28
#define ELLIPSOID_IGNORE            29
#define ELLIPSOID_CLARKE80_IGN      30
#define ELLIPSOID_IAG75             31
#define ELLIPSOID_MERIT83           32
#define ELLIPSOID_NEW_INTL67        33
#define ELLIPSOID_WALBECK           34
#define ELLIPSOID_MOD2_BESSEL       35
#define ELLIPSOID_CLARKE58          36
#define ELLIPSOID_MOD3_CLARKE80     37
#define ELLIPSOID_MOD4_CLARKE80     38
#define ELLIPSOID_MOD2_EVEREST      39
#define ELLIPSOID_MOD3_EVEREST      40
#define ELLIPSOID_INDONESIAN        41
#define ELLIPSOID_NWL_9D            42
#define ELLIPSOID_NWL_10D           43
#define ELLIPSOID_OSU86F            44
#define ELLIPSOID_OSU91A            45
#define ELLIPSOID_PLESSIS           46
#define ELLIPSOID_MOD4_EVEREST      48
#define ELLIPSOID_IRISH_WOFO        49
#define ELLIPSOID_EVEREST_PAKISTAN  50
#define ELLIPSOID_ATS77             51
#define ELLIPSOID_PZ90              52

#define NUM_ELLIPSOIDS              53


#if (defined(unix) || defined(__unix__) || defined(USG)) && defined(__osf__)
typedef unsigned char ELLIPSOID, *pELLIPSOID;
#else
typedef UCHAR ELLIPSOID, *pELLIPSOID;
#endif

typedef struct ELLIPSOID_INFO {
  double a;
  double e2;
  double f;
} ELLIPSOID_INFO, *pELLIPSOID_INFO;

/****************************************************************************/
/* We only list a few datums here, but a table in CSYSDAT.C defines over    */
/* 100 datums.  The datums below are the only ones we need to refer to in   */
/* our routines.  Do not change the numbers assigned to these datums.       */
/****************************************************************************/
#define DATUM_IGNORE       0                  /* don't do datum conversion  */
#define DATUM_3PARMS_FIRST 0                  /* first 3-param datum        */
#define DATUM_3PARMS_USER  999                /* user-defined 3-param datum */
#define DATUM_8PARMS_FIRST 1000               /* first 8-param datum        */
#define DATUM_8PARMS_USER  9999               /* user-defined 8-param datum */

typedef short DATUM, *pDATUM;

typedef struct DATUM_INFO {
  ELLIPSOID Ellipsoid;
  double ShiftX;                /* meters */
  double ShiftY;                /* meters */
  double ShiftZ;                /* meters */
  double RotateX;               /* arc-seconds */
  double RotateY;               /* arc-seconds */
  double RotateZ;               /* arc-seconds */
  double ScaleAdjust;           /* parts per million */
  double PrimeMeridian;         /* degrees east of Greenwich */
} DATUM_INFO, *pDATUM_INFO;

/****************************************************************************/
/* Do NOT renumber the entries in this list!  Add new entries at the end,   */
/* and increment NUM_CSYS_TYPES.                                            */
/****************************************************************************/
#define CSYS_UNUSED             -1
#define CSYS_NONEARTH           0
#define CSYS_LL                 1     /* Longitude / Latitude               */
#define CSYS_CEA                2     /* Cylindrical Equal-Area             */
#define CSYS_LAMBERT_CC         3     /* Lambert Conformal Conic            */
#define CSYS_LAMBERT_AEA        4     /* Lambert Azimuthal Equal-Area       */
#define CSYS_AZIMUTHAL          5     /* Azimuthal Equidistant              */
#define CSYS_CONIC              6     /* Equidistant Conic                  */
#define CSYS_HOTINE             7     /* Hotine Oblique Mercator            */
#define CSYS_TM                 8     /* Transverse Mercator                */
#define CSYS_ALBERS             9
#define CSYS_MERCATOR           10
#define CSYS_MILLER             11
#define CSYS_ROBINSON           12
#define CSYS_MOLLWEIDE          13
#define CSYS_ECKERT4            14
#define CSYS_ECKERT6            15
#define CSYS_SINUSOIDAL         16
#define CSYS_GALL               17
#define CSYS_NZMG               18    /* New Zealand Map Grid               */
#define CSYS_LAMBERT_BELGIUM72  19    /* Lambert CC modified for Belgium 72 */
#define CSYS_STEREOGRAPHIC      20
#define CSYS_TM_DENMARK_S34J    21    /* Trans Mercator modified for Denmark*/
#define CSYS_TM_DENMARK_S34S    22    /* Trans Mercator modified for Denmark*/
#define CSYS_TM_DENMARK_S45B    23    /* Trans Mercator modified for Denmark*/
#define CSYS_TM_FINLAND         24    /* Trans Mercator modified for Finland*/
#define CSYS_SWISS              25    /* Swiss Oblique Mercator             */
#define CSYS_MERC_STD_PARL      26    /* Mercator w/other standard parallel */
#define CSYS_POLYCONIC          27    /* "American" polyconic               */
#define CSYS_AZIMUTHAL_OBLIQUE  28    /* Azimuthal Equidistant, oblique aspect */
#define CSYS_LAMBERT_AEA_OBLIQUE 29   /* Lambert Azimuthal Equa-Area - oblique */
#define CSYS_CASSINI_SOLDNER    30    /* Cassin-Soldner                      */
#define CSYS_DBL_STEREOGRAPHIC  31    /* Double Stereographic                */

#define NUM_CSYS_TYPES          32    /* not including CSYS_UNUSED          */

typedef short CSYS_TYPE, *pCSYS_TYPE;

/****************************************************************************/
/* Parameters for the various types of projections.  CSYS_PARMS_COMMON      */
/* doesn't correspond to a particular projection -- instead, it represents  */
/* the fields that are common to most of the CSYS_PARMS_* structures.       */
/****************************************************************************/
#define CSYS_MAX_DOUBLE_PARMS  6

typedef struct CSYS_PARMS_COMMON {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  double     DoubleParms[CSYS_MAX_DOUBLE_PARMS];
} CSYS_PARMS_COMMON, *pCSYS_PARMS_COMMON;

typedef struct CSYS_PARMS_NONEARTH {
  BOOLEAN bIsotropic;                         /* used only for client CSYS  */
  char    SysQuadrant;                        /* used only for logical CSYS */
  UNIT    CoordUnits;
  char    UserQuadrant;
} CSYS_PARMS_NONEARTH, *pCSYS_PARMS_NONEARTH;

typedef struct CSYS_PARMS_LL {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits_unused;               /* unused -- for future use   */
  char       SysQuadrant;                     /* used only for logical CSYS */
  double     StdParallel;                     /* used only for client CSYS  */
} CSYS_PARMS_LL, *pCSYS_PARMS_LL;

typedef struct CSYS_PARMS_CEA {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  double     OriginLongitude;
  double     StdParallel;
} CSYS_PARMS_CEA, *pCSYS_PARMS_CEA;

typedef struct CSYS_PARMS_LAMBERT_CC {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  DPNT       Origin;
  double     StdParallel1;
  double     StdParallel2;
  double     FalseEasting;
  double     FalseNorthing;
} CSYS_PARMS_LAMBERT_CC, *pCSYS_PARMS_LAMBERT_CC;

typedef struct CSYS_PARMS_LAMBERT_AEA {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  DPNT       Origin;
  double     Range;
} CSYS_PARMS_LAMBERT_AEA, *pCSYS_PARMS_LAMBERT_AEA;

typedef struct CSYS_PARMS_AZIMUTHAL {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  DPNT       Origin;
  double     Range;
} CSYS_PARMS_AZIMUTHAL, *pCSYS_PARMS_AZIMUTHAL;

typedef struct CSYS_PARMS_CONIC {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  DPNT       Origin;
  double     StdParallel1;
  double     StdParallel2;
  double     FalseEasting;
  double     FalseNorthing;
} CSYS_PARMS_CONIC, *pCSYS_PARMS_CONIC;

typedef struct CSYS_PARMS_HOTINE {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  DPNT       Origin;
  double     Azimuth;
  double     ScaleFactor;
  double     FalseEasting;
  double     FalseNorthing;
} CSYS_PARMS_HOTINE, *pCSYS_PARMS_HOTINE;

typedef struct CSYS_PARMS_TM {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  DPNT       Origin;
  double     ScaleFactor;
  double     FalseEasting;
  double     FalseNorthing;
} CSYS_PARMS_TM, *pCSYS_PARMS_TM;

typedef struct CSYS_PARMS_ALBERS {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  DPNT       Origin;
  double     StdParallel1;
  double     StdParallel2;
  double     FalseEasting;
  double     FalseNorthing;
} CSYS_PARMS_ALBERS, *pCSYS_PARMS_ALBERS;

typedef struct CSYS_PARMS_MERCATOR {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  double     OriginLongitude;
} CSYS_PARMS_MERCATOR, *pCSYS_PARMS_MERCATOR;

typedef struct CSYS_PARMS_MILLER {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  double     OriginLongitude;
} CSYS_PARMS_MILLER, *pCSYS_PARMS_MILLER;

typedef struct CSYS_PARMS_ROBINSON {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  double     OriginLongitude;
} CSYS_PARMS_ROBINSON, *pCSYS_PARMS_ROBINSON;

typedef struct CSYS_PARMS_MOLLWEIDE {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  double     OriginLongitude;
} CSYS_PARMS_MOLLWEIDE, *pCSYS_PARMS_MOLLWEIDE;

typedef struct CSYS_PARMS_ECKERT4 {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  double     OriginLongitude;
} CSYS_PARMS_ECKERT4, *pCSYS_PARMS_ECKERT4;

typedef struct CSYS_PARMS_ECKERT6 {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  double     OriginLongitude;
} CSYS_PARMS_ECKERT6, *pCSYS_PARMS_ECKERT6;

typedef struct CSYS_PARMS_SINUSOIDAL {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  double     OriginLongitude;
} CSYS_PARMS_SINUSOIDAL, *pCSYS_PARMS_SINUSOIDAL;

typedef struct CSYS_PARMS_GALL {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  double     OriginLongitude;
} CSYS_PARMS_GALL, *pCSYS_PARMS_GALL;

typedef struct CSYS_PARMS_NZMG {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  DPNT       Origin;
  double     FalseEasting;
  double     FalseNorthing;
} CSYS_PARMS_NZMG, *pCSYS_PARMS_NZMG;

typedef struct CSYS_PARMS_STEREOGRAPHIC {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  DPNT       Origin;
  double     ScaleFactor;
  double     FalseEasting;
  double     FalseNorthing;
} CSYS_PARMS_STEREOGRAPHIC, *pCSYS_PARMS_STEREOGRAPHIC;

typedef struct CSYS_PARMS_SWISS {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  DPNT       Origin;
  double     FalseEasting;
  double     FalseNorthing;
} CSYS_PARMS_SWISS, *pCSYS_PARMS_SWISS;

typedef struct CSYS_PARMS_POLYCONIC {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  DPNT       Origin;
  double     FalseEasting;
  double     FalseNorthing;
} CSYS_PARMS_POLYCONIC, *pCSYS_PARMS_POLYCONIC;

typedef struct CSYS_PARMS_CASSINI {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  DPNT       Origin;
  double     FalseEasting;
  double     FalseNorthing;
} CSYS_PARMS_CASSINI, *pCSYS_PARMS_CASSINI;

typedef struct CSYS_PARMS_DBL_STEREOGRAPHIC {
  DATUM_INFO DatumInfo;
  UNIT       CoordUnits;
  DPNT       Origin;
  double     ScaleFactor;
  double     FalseEasting;
  double     FalseNorthing;
} CSYS_PARMS_DBL_STEREOGRAPHIC, *pCSYS_PARMS_DBL_STEREOGRAPHIC;

typedef struct CSYS_PARMS {
  union {
		CSYS_PARMS_COMMON        Common;
		CSYS_PARMS_NONEARTH      NonEarth;
		CSYS_PARMS_LL            LL;
		CSYS_PARMS_CEA           CEA;
		CSYS_PARMS_LAMBERT_CC    LambertCC;
		CSYS_PARMS_LAMBERT_AEA   LambertAEA;
		CSYS_PARMS_AZIMUTHAL     Azimuthal;
		CSYS_PARMS_CONIC         Conic;    
		CSYS_PARMS_HOTINE        Hotine;
		CSYS_PARMS_TM            TM;
		CSYS_PARMS_ALBERS        Albers;
		CSYS_PARMS_MERCATOR      Mercator;
		CSYS_PARMS_MILLER        Miller;
		CSYS_PARMS_ROBINSON      Robinson;
		CSYS_PARMS_MOLLWEIDE     Mollweide;
		CSYS_PARMS_ECKERT4       Eckert4;
		CSYS_PARMS_ECKERT6       Eckert6;
		CSYS_PARMS_SINUSOIDAL    Sinusoidal;
		CSYS_PARMS_GALL          Gall;
		CSYS_PARMS_NZMG          NZMG;
		CSYS_PARMS_STEREOGRAPHIC Stereographic;
		CSYS_PARMS_SWISS         Swiss;
		CSYS_PARMS_POLYCONIC     Polyconic;
		CSYS_PARMS_CASSINI       Cassini;
		CSYS_PARMS_DBL_STEREOGRAPHIC DblStereographic;
  }parms_union;
  AFFINE_INFO ProjAffineInfo;
  UINT        bProjAffine : 1;
  UNIT        UserCoordUnits;     /* used only if bProjAffine is TRUE */
} CSYS_PARMS, *pCSYS_PARMS;

#endif
/****************************************************************************/
/* END of definitions from CSYS.H                                           */
/****************************************************************************/

/****************************************************************************/
/* These are defines and structs for DBACCESS                               */
/*                                                                          */
/*  These definitions come from TABBASE.H                                   */
/****************************************************************************/

#define F_NONE     0  /* F_ stands for Format */
#define F_DBASE    1
#define F_LOTUS    2
#define F_ASCII    3
#define F_EXCEL    4
#define F_NATIVE   5
#define F_RASTER   6  /* table_type_raster */
#define F_SEAMLESS 7  /* table_type_seamless */
#define F_DAO      8  /* Access Table using DAO Jet Engine COM objects  */
#define F_QRY      9  /* Table is a saved query */
#define F_ODBC     10 /* Access Table using ODBC see dbodbc.h */
#define F_MEMTABLE 11

#define MFAL_NOOBJECT      200 
#define MFAL_POINT         201
#define MFAL_LINE          202
#define MFAL_ARC           203
#define MFAL_PLINE         204
#define MFAL_REGION        205
#define MFAL_TEXT          206
#define MFAL_RECT          207
#define MFAL_ROUNDRECT     208
#define MFAL_ELLIPSE       209
#define MFAL_MULTIPLINE    210         /* available in MI v3.0 and later     */
#define MFAL_FONT_POINT    211         /* available in MI v4.0 and later     */
#define MFAL_CUSTOM_POINT  212
#define MFAL_MULTIPOINT    213
#define MFAL_COLLECTION    214

/*
 * Some defines taken from miextens.h needed for custom symbols
 */
#define SZ_EFF_NAME   32

typedef struct MI_FONT_SYMBOL {
  short shape; 						   /* shape >= 31 spceifying character code */
  MI_COLOR color;			 				  /* override color */
  short pointsize;							  /* point size, like a font 1 - 48 */
  char  szFontName[SZ_FONTNAME];
  short style;                        /* Font style */
  double angle;                       /* rotation angle */
}	MI_FONT_SYMBOL, *pMI_FONT_SYMBOL;

typedef struct MI_CUSTOM_SYMBOL {
   char      szName[SZ_EFF_NAME];      /* custom symbol file name */
   MI_COLOR       color;                  /* override color                     */
   short       pointsize;              /* point size, like a font            */
   short       style;       /* The eff symbol style. */
} MI_CUSTOM_SYMBOL, *pMI_CUSTOM_SYMBOL;

typedef struct MI_SYMBOL {
  short code;
  MI_COLOR color;										/* override color */
  short pointsize;							  /* point size, like a font */
}	MI_SYMBOL, *pMI_SYMBOL;

typedef struct MI_CPEN {
  short width;
  PAT_STYLE pat;
  PEN_STYLE style;
  MI_COLOR color;
}	MI_CPEN, *pMI_CPEN;

typedef struct MI_XBRUSH {
  PAT_STYLE pat;
  MI_COLOR forecolor;
  MI_COLOR backcolor;
  UCHAR transparent;
}	MI_XBRUSH, *pMI_XBRUSH;

typedef struct MI_POLY {
  long points;
  DPNT __huge *pPnts;
} MI_POLY, *pMI_POLY;

typedef struct MI_ARC {                          /* ARC                        */
  DRECT     ellipseMBR;                       /* MBR                        */
  double    begangle;
  double    endangle;
  MI_CPEN      pen;                              /* which pen to use           */
} MI_ARC, *pMI_ARC;

typedef struct MI_ELLIPSE {                      /* ELLIPSE                    */
  DRECT     MBR;                              /* MBR                        */
  MI_CPEN       pen;                              /* which pen to use           */
  MI_XBRUSH     brush;                            /* which brush to use         */
} MI_ELLIPSE, *pMI_ELLIPSE;

typedef struct MI_LINE {                         /* LINE                       */
  DPNT      beg;                              /* coordinates                */
  DPNT      end;                              /* coordinates                */
  MI_CPEN   pen;                              /* which pen to use           */
} MI_LINE, *pMI_LINE;

typedef struct MI_PLINE {                        /* PLINE                      */
  long      points;                           /* number of points in pline  */
  unsigned  bSmooth : 1;
  MI_CPEN   pen;                              /* which pen to use           */
  DPNT      __huge *data;
} MI_PLINE, *pMI_PLINE;

typedef struct MI_POINT {                        /* POINT                      */
  DPNT pnt;                                      /* coordinates                */
  MI_SYMBOL symbol;
} MI_POINT, *pMI_POINT;

typedef struct MI_FONT_POINT {                        /* POINT                      */
  DPNT pnt;                                      /* coordinates                */
  MI_FONT_SYMBOL symbol;
} MI_FONT_POINT, *pMI_FONT_POINT;

typedef struct MI_CUSTOM_POINT {                        /* POINT                      */
  DPNT pnt;                                      /* coordinates                */
  MI_CUSTOM_SYMBOL symbol;
} MI_CUSTOM_POINT, *pMI_CUSTOM_POINT;


typedef struct MI_RECT {                         /* RECTANGLE                  */
  DRECT     MBR;                              /* MBR                        */
  MI_CPEN       pen;                              /* which pen to use           */
  MI_XBRUSH     brush;                            /* which brush to use         */
} MI_RECT, *pMI_RECT;

typedef struct MI_MULTIPLINE {                   /* MULTIPLINE                */
  short     plines;                              /* number of plines           */
  unsigned  bSmooth : 1;
  MI_CPEN       pen;                              /* which pen to use           */
  DPNT          centroid;                         /* polyline centroid        */
  pMI_POLY     data;
} MI_MULTIPLINE, *pMI_MULTIPLINE;

typedef struct MI_REGION {                       /* REGION                     */
  short         plines;                           /* number of plines           */
  MI_CPEN       pen;                              /* which pen to use           */
  MI_XBRUSH     brush;                            /* which brush to use         */
  DPNT          centroid;                         /* region centroid        */
  pMI_POLY      data;
} MI_REGION, *pMI_REGION;

typedef union SYMBOL_STYLE {
		MI_FONT_SYMBOL FontSymbol;
		MI_CUSTOM_SYMBOL CustomSymbol;
		MI_SYMBOL   VectorSymbol;
}SYMBOL_STYLE, *pSYMBOL_STYLE;

typedef struct MI_MULTIPOINT {
  unsigned char SymbolType;	/* MFAL_POINT, MFAL_FONT_POINT,  MFAL_CUSTOM_POINT */
	SYMBOL_STYLE Sym;
	DPNT centroid;
	MI_POLY data; /* always just one */
} MI_MULTIPOINT, *pMI_MULTIPOINT;

typedef struct MI_COLLECTION {
	MI_MULTIPOINT multipoint;
	MI_MULTIPLINE multipolyline;
	MI_REGION  	  region;
}MI_COLLECTION, *pMI_COLLECTION;

typedef struct MI_ROUNDRECT {                    /* ROUNDED RECTANGLE          */
  DRECT     MBR;                              /* MBR                        */
  double    oval_width;
  double    oval_height;
  MI_CPEN       pen;                              /* which pen to use           */
  MI_XBRUSH     brush;                            /* which brush to use         */
} MI_ROUNDRECT, *pMI_ROUNDRECT;

typedef struct MI_FONT_DSC {
  char name[SZ_FONTNAME];
} MI_FONT_DSC, *pMI_FONT_DSC;

typedef struct MI_FONT_STYLE {
        unsigned s_bold:1;
        unsigned s_italic:1;
        unsigned s_under:1;
        unsigned s_strikeout:1;
        unsigned s_outline:1;
        unsigned s_shadow:1;
        unsigned s_inverse:1;
    		unsigned s_blink:1;
	      unsigned:0;
} MI_FONT_STYLE;

typedef struct MI_FONT_EXTENDED_STYLE {
  BOOLEAN bHalo;
  BOOLEAN bAllCaps;
  BOOLEAN bDblSpace;
} MI_FONT_EXTENDED_STYLE, *pMI_FONT_EXTENDED_STYLE;

typedef struct MI_FNT {
  MI_FONT_STYLE style;
  MI_FONT_EXTENDED_STYLE extstyle;
  MI_FONT_DSC descrip;
  short       size;
  MI_COLOR    forecolor;
  MI_COLOR    backcolor;
  unsigned char opaque;
} MI_FNT, *pMI_FNT;

typedef struct MI_TEXT {                         /* TEXT                       */
  DRECT     textMBR;                          /* text MBR after rotation    */
  DPNT      line_end;                         /* endpt of annotating line   */
  MI_CPEN   pen;                              /* which pen to use 	*/
  double    angle;                            /* angle of text rotation     */
  unsigned  justify : 2;
  unsigned  spacing : 2;
  unsigned  linetype : 3;
  MI_FNT    font;                             /* place to store font        */
  char     *data;                             /* text */
} MI_TEXT, *pMI_TEXT;


typedef struct MI_OBJ {
  unsigned char  type;                        /* type of object             */
  union {
    MI_FONT_POINT    fontpoint;				 /* Font point                 */
    MI_CUSTOM_POINT  custompoint;      		 /* Custom point               */
    MI_ARC           arc;                    /* arc                        */      
    MI_ELLIPSE       ellipse;                /* ellipse/circle             */      
    MI_LINE          line;                   /* line                       */      
    MI_PLINE         pline;                  /* polyline                   */      
    MI_MULTIPLINE    multipline;             /* multi-polyline             */      
    MI_POINT         point;                  /* point                      */      
    MI_RECT          rect;                   /* rectangle                  */     
    MI_REGION        region;                 /* region                     */      
    MI_ROUNDRECT     roundrect;              /* rounded rectangle          */     
    MI_TEXT          text;                   /* text                       */
		MI_MULTIPOINT    multipoint;
		MI_COLLECTION		 collection;
    } obj;
} MI_OBJ, *pMI_OBJ;

typedef struct MI_SYMBOLOGY {
  unsigned bSymbol : 1;
  unsigned bLinepen : 1;
  unsigned bPen : 1;
  unsigned bBrush : 1;
  unsigned bFont : 1;
  unsigned char SymbolType;	/* MFAL_POINT, MFAL_FONT_POINT,  MFAL_CUSTOM_POINT */
  SYMBOL_STYLE symbol;
  MI_CPEN    linepen;
  MI_CPEN    pen;
  MI_XBRUSH  brush;
  MI_FNT   font;
} MI_SYMBOLOGY, *pMI_SYMBOLOGY;

typedef struct MI_ATTRDEF {                  /* attribute structure */
  char           name[ATTRIB_NAME_LEN];     /* name of attribute */                                                         
  char           type;                      /* 1=char, 2=decimal, 3=int... */
  unsigned char  width;                     /* if decimal or char type */
  char           decPlaces;                 /* if decimal type */
  BOOLEAN        bIndexed;                  /* TRUE if field is indexed     */
} MI_ATTRDEF, *pMI_ATTRDEF;

typedef struct MI_COORDSYS {
  CSYS_TYPE proj_type;         /* projection type */
  CSYS_PARMS parms;            /* will contain datum, units, doubles array */
  DRECT bounds;                /* bounding rectangle */
} MI_COORDSYS, *pMI_COORDSYS;

typedef struct VIEWTABINFO {
  char           filename[SZ_PATH];
  char           alias[ATTRIB_NAME_LEN];
} VIEWTABINFO, *pVIEWTABINFO;

typedef struct VIEWFIELDINFO {
  char           alias[ATTRIB_NAME_LEN];
  int            tablenum;
  char           fieldname[ATTRIB_NAME_LEN];
} VIEWFIELDINFO, *pVIEWFIELDINFO;

typedef struct VIEWJOININFO {
  int            fromtablenum;
  char           fromfield[ATTRIB_NAME_LEN];
  int            totablenum;
  char           tofield[ATTRIB_NAME_LEN];
} VIEWJOININFO, *pVIEWJOININFO;

typedef struct VIEWINFO {
  int            ntables;
  pVIEWTABINFO   tables;
  int            nfields;
  pVIEWFIELDINFO fields;
  pVIEWJOININFO  joins;
} VIEWINFO, *pVIEWINFO;

/****************************************************************************/
/* Map File Access Library functions                                        */
/****************************************************************************/


#if !defined(MFAL_API)
	#define MFAL_API __cdecl
#endif

int		MFAL_API	MICharacterSet(int, char *);
short	MFAL_API	MICloseTable(long tabid);
short	MFAL_API	MIConvertDatumInfo(pDATUM_INFO, pDATUM);
short	MFAL_API	MICreateIndex(TABLEID tabid, int col);
short	MFAL_API	MICreateMap(long tabid, struct MI_COORDSYS *pCoordSys);
long	MFAL_API	MICreateTable(char *pFilePath, struct MI_ATTRDEF *pAttrDef, int numFields, struct MI_COORDSYS *pCoordSys, int databaseformat);
short	MFAL_API	MIDeleteMetadata(TABLEID, char *);
short	MFAL_API	MIDropIndex(TABLEID tabid, int col);
short	MFAL_API	MIDropMap(long tabid);
short	MFAL_API	MIDropTable(TABLEID);
short	MFAL_API	MIEnableUserMessages(short);
short	MFAL_API	MIEnumerateMetadata(TABLEID, char *, void (__export MFAL_API *)(char *, char *));
short	MFAL_API	MIFetchRow(long tabid, long rowNo, char *pAttrData, struct MI_OBJ *pObj);
void	MFAL_API	MIFreeViewInfo(struct VIEWINFO **);
short	MFAL_API	MIGetAttributeDefs(long tabid, struct MI_ATTRDEF *pAttrDef);
short	MFAL_API	MIGetCoordSys(long tabid, struct MI_COORDSYS *pCoordSys);
short	MFAL_API	MIGetCoordsysString(pMI_COORDSYS, char *);
void	MFAL_API	MIGetDatumInfo(short datum, struct DATUM_INFO *pInfo);
void	MFAL_API	MIGetDefaultBounds(pMI_COORDSYS);
void	MFAL_API	MIGetEllipsoidInfo(unsigned char, struct ELLIPSOID_INFO *);
int		MFAL_API	MIGetErrorStatus();
short	MFAL_API	MIGetIndexes(long tabid, char *pTextData);
short	MFAL_API	MIGetMetadata(TABLEID, char *, char *, int);
short	MFAL_API	MIGetMFALVersion(int *, int *, int *);
short	MFAL_API	MIGetNumFields(long tabid, long *pNumFields);
short	MFAL_API	MIGetNumObjects(TABLEID tabid, long *pNumPoints, long *pNumLines, long *pNumAreas);
short	MFAL_API	MIGetNumRows(long tabid, long *pNumRows);
short	MFAL_API	MIGetTableFormat(TABLEID, int *, int *);
short	MFAL_API	MIGetTablePath(TABLEID, char *);
short	MFAL_API	MIHasUnsupportedData(TABLEID);
short	MFAL_API	MIIndexExists(TABLEID tabid, int col);
int		MFAL_API	MIIndexSearch(long, long, char *, short, long *);
short	MFAL_API	MIInit(int version);
short	MFAL_API	MIInsertRow(long tabid, char *pAttrData, struct MI_OBJ *pObj, long *pRowNo);
long	MFAL_API	MIOpenTable(char *pFilePath, int openMode);
int		MFAL_API	MIParseView(char *, struct VIEWINFO **);
long	MFAL_API	MIPrepareTable(char *pTableSpec,  char *pDataSpec, int databaseformat);
short	MFAL_API	MIRenameTable(TABLEID, char *);
int		MFAL_API	MISetCharacterSet(int);
void	MFAL_API	MISetMBQuoteConvention(BOOLEAN bFlag);
short	MFAL_API	MISetMetadata(TABLEID, char *, char *);
short	MFAL_API	MITerm();
short	MFAL_API	MIUpdateRow(long tabid, long rowNo, char *pAttrData, struct MI_OBJ *pObj);
short	MFAL_API	MIUseCsys(TABLEID, pMI_COORDSYS);
short	MFAL_API	MIWriteCoordSysClause(pMI_COORDSYS, char *);
short	MFAL_API	MIWriteMetadata(TABLEID);

}  //end namespace
/* Change packing back to default */
#if !(defined(unix) || defined(__unix__) || defined(USG))
	#pragma pack(pop)		  
	#pragma warning(default:4103)
#endif /* !(defined(unix) || defined(__unix__) || defined(USG)) */

#ifdef __cplusplus
}
#endif

#endif  /* ifndef _MFAL_H */
