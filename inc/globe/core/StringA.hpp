//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-07 18:46:15
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_STRINGA_H__
#define __GLOBE_STRINGA_H__
namespace glb {


//---------------------------------------------------------------------------
// Types
//---------------------------------------------------------------------------
class StringA;

// vector<StringA>
typedef std::vector<StringA>        StringAVec;
typedef StringAVec::iterator        StringAVecIt;
typedef StringAVec::const_iterator  StringAVecCIt;

// list<StringA>
typedef std::list<StringA>           StringAList;
typedef StringAList::iterator        StringAListIt;
typedef StringAList::const_iterator  StringAListCIt;


//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------
static const uint   STRING_BASE_BUFFER_SIZE = 32 - sizeof(uint32) - sizeof(char*) - sizeof(uint32); // so the StringA object can be aligned on 16 bytes boundary, for comparison, sizeof(std::string) is 28 bytes on msvc7.1 so there's no major overhead
static const uint   STRING_GROW_SIZE        = 32;
static const uint32 STRING_HASH_SEED        = 0xDEADBEEF;


//---------------------------------------------------------------------------
// StringA
//---------------------------------------------------------------------------
class GLB_EXPORT StringA
{
public :
  enum BoolStringType
  {
    BOOLSTRINGTYPE_YESNO,      // representation : "yes"  / "no" (default)
    BOOLSTRINGTYPE_TRUEFALSE,  // representation : "true" / "false"
    BOOLSTRINGTYPE_ONOFF,      // representation : "on"   / "off"
    BOOLSTRINGTYPE_IO,         // representation : "0"    / "1"
  };


public :
  StringA (void);
  StringA (const StringA& rhs);
  StringA (const StringA& rhs, uint uiCount, uint uiStart=0);
#ifdef _STRING_
  StringA (const std::string& rhs);
  StringA (const std::string& rhs, uint uiCount, uint uiStart=0);
#endif
  StringA (const char* rhs);
  StringA (const char* rhs, uint uiCount, uint uiStart=0);
  explicit StringA (const bool   b);
  explicit StringA (const char   c);
  explicit StringA (const int32  i);
  explicit StringA (const uint32 ui);
  explicit StringA (const int64  i);
  explicit StringA (const uint64 ui);
  explicit StringA (const float  f);
  explicit StringA (const double d);
  virtual ~StringA (void);


  // StringA default behavior setup
  static void           setDefaultBoolStringType (BoolStringType eBoolStringType); // only influence Append(bool) and operator=(bool) methods. the extractBool() method will always recognize every supported representations.
  static BoolStringType getDefaultBoolStringType (void);


  // memory allocation
  virtual void grow    (uint uiDesiredSize, bool bKeepOldContent=true); // resize only if desired size is > current buffer size (do not forget : size = length + 1)
  virtual void resize  (uint uiDesiredSize, bool bKeepOldContent=true); // force resize, this will enventually cut your string (do not forget : size = length + 1)
  virtual void compact (void);                                          // fit buffer size to string length
  virtual void reset   (void);                                          // complete reset : raz string and free memory (use the clear() method if you just want make the string empty, which is faster)

  // length
  virtual void clear     (void);       // empty string but does not free memory (this is a fast alternative to reset())
  virtual bool isEmpty   (void) const; // check for an empty string
  virtual uint length    (void) const; // length of the string, this is an strlen() equivalent
  virtual uint maxLength (void) const; // max string length the buffer is currently capable to hold (so real buffer size is maxLength()+1)

  // direct access to the internal buffer
  // WARNING : use those methods only if you know what you are doing !
  virtual char* acquireBuffer (uint uiMinRequiredBufferSize, bool bKeepOldContent=true); // do not forget : size = length + 1
  virtual void  releaseBuffer (uint uiNewStringLength=0);

  // null-terminated c-string representation
  virtual const char* c_str                (void) const;
  virtual             operator const char* (void) const;
  virtual             operator const char* (void);

  // accessors to individual characters
  virtual char  operator[] (int8   nIndex) const;
  virtual char& operator[] (int8   nIndex);
  virtual char  operator[] (uint8  nIndex) const;
  virtual char& operator[] (uint8  nIndex);
  virtual char  operator[] (int16  nIndex) const;
  virtual char& operator[] (int16  nIndex);
  virtual char  operator[] (uint16 nIndex) const;
  virtual char& operator[] (uint16 nIndex);
  virtual char  operator[] (int32  nIndex) const;
  virtual char& operator[] (int32  nIndex);
  virtual char  operator[] (uint32 nIndex) const;
  virtual char& operator[] (uint32 nIndex);
  virtual char  operator[] (int64  nIndex) const;
  virtual char& operator[] (int64  nIndex);
  virtual char  operator[] (uint64 nIndex) const;
  virtual char& operator[] (uint64 nIndex);

  // copy
  virtual void copy      (const StringA& strSource);
#ifdef _STRING_
  virtual void copy      (const std::string& strSource);
#endif
  virtual void copy      (const char* pszSource);
  virtual void copyCount (const char* pszSource, uint uiSourceLength, uint uiSourceStartOffset=0);

  // copy operators
  virtual void operator= (const StringA& rhs);
#ifdef _STRING_
  virtual void operator= (const std::string& rhs);
#endif
  virtual void operator= (const char*  rhs);
  virtual void operator= (const bool   b);
  virtual void operator= (const char   c);
  virtual void operator= (const int16  i);
  virtual void operator= (const uint16 ui);
  virtual void operator= (const int32  i);
  virtual void operator= (const uint32 ui);
  virtual void operator= (const int64  i);
  virtual void operator= (const uint64 ui);
  virtual void operator= (const float  f);
  virtual void operator= (const double d);

  // appending
  virtual void append          (const StringA& str);
  virtual void append          (const StringA& str, uint uiLength);
#ifdef _STRING_
  virtual void append          (const std::string& str);
  virtual void append          (const std::string& str, uint uiLength);
#endif
  virtual void append          (const char*  psz);
  virtual void append          (const char*  psz, uint uiLength);
  virtual void appendUrlEscape (const char*  psz);
  virtual void append          (const bool   b);
  virtual void append          (const char   c);
  virtual void append          (const int16  i);
  virtual void append          (const uint16 ui);
  virtual void append          (const int32  i);
  virtual void append          (const uint32 ui);
  virtual void append          (const int64  i);
  virtual void append          (const uint64 ui);
  virtual void append          (const float  f);
  virtual void append          (const double d);

  // appending operators
  virtual StringA& operator+= (const StringA& rhs);
#ifdef _STRING_
  virtual StringA& operator+= (const std::string& rhs);
#endif
  virtual StringA& operator+= (const char*  rhs);
  virtual StringA& operator+= (const bool   b);
  virtual StringA& operator+= (const char   c);
  virtual StringA& operator+= (const int16  i);
  virtual StringA& operator+= (const uint16 ui);
  virtual StringA& operator+= (const int32  i);
  virtual StringA& operator+= (const uint32 ui);
  virtual StringA& operator+= (const int64  i);
  virtual StringA& operator+= (const uint64 ui);
  virtual StringA& operator+= (const float  f);
  virtual StringA& operator+= (const double d);
  virtual StringA& operator<< (const StringA& rhs);
#ifdef _STRING_
  virtual StringA& operator<< (const std::string& rhs);
#endif
  virtual StringA& operator<< (const char*  rhs);
  virtual StringA& operator<< (const bool   b);
  virtual StringA& operator<< (const char   c);
  virtual StringA& operator<< (const int16  i);
  virtual StringA& operator<< (const uint16 ui);
  virtual StringA& operator<< (const int32  i);
  virtual StringA& operator<< (const uint32 ui);
  virtual StringA& operator<< (const int64  i);
  virtual StringA& operator<< (const uint64 ui);
  virtual StringA& operator<< (const float  f);
  virtual StringA& operator<< (const double d);

  // appending operators (path)
  // shortcuts to the pathAppend() method
  virtual StringA& operator/= (const StringA& rhs);
  virtual StringA& operator/= (const char* rhs);
#ifdef _STRING_
  virtual StringA& operator/= (const std::string& rhs);
#endif

  // inserting
  virtual void insert (const char c, uint uiOffset);
  virtual void insert (const char* psz, uint uiOffset, uint uiLength=0);

  // erasing
  virtual void erase (int nOffset, uint uiLength=0);

  // filling
  // * current content will be *discarded* by fill() methods.
  // * use the pad() method if you only want to append some
  //   characters to reach a given final string length.
  virtual void fill (const char c, uint uiCount);
  virtual void fill (const char* psz, uint uiCount);

  // padding
  virtual void pad (const char c, uint uiDesiredLength);
  virtual void pad (const char* psz, uint uiDesiredLength);

  // case sensitive/insensitive comparison
  virtual int  compare       (const char* psz) const;
  virtual int  compareCount  (const char* psz, uint uiMaxCount) const;
  virtual int  compareI      (const char* psz) const;
  virtual int  compareCountI (const char* psz, uint uiMaxCount) const;
  virtual bool hasPrefix     (const char c, bool bCaseSensitive);
  virtual bool hasPrefix     (const char* psz, bool bCaseSensitive);
  virtual bool hasSuffix     (const char c, bool bCaseSensitive);
  virtual bool hasSuffix     (const char* psz, bool bCaseSensitive);
  virtual bool equals        (const char* psz) const;
  virtual bool equalsI       (const char* psz) const;

  // case manipulation
  virtual void toLower    (void);
  virtual void toUpper    (void);
  virtual void invertCase (void);

  // cutting
  virtual StringA     left  (uint uiLength) const;
  virtual const char* left  (uint uiLength, StringA& strOutResult) const;
  virtual StringA     right (uint uiLength) const;
  virtual const char* right (uint uiLength, StringA& strOutResult) const;
  virtual StringA     mid   (uint uiOffset, uint uiLength) const;
  virtual const char* mid   (uint uiOffset, uint uiLength, StringA& strOutResult) const;

  // a substr() emulation of the so-handy php function (check http://php.net/substr for more information)
  virtual StringA     substr (int nOffset, int nLength=0) const;
  virtual const char* substr (StringA& strOutResult, int nOffset, int nLength=0) const;

  // cleaning
  virtual void trimWhitespaces      (void);
  virtual void trim                 (const char c);
  virtual void trim                 (const char* pszCharList);
  virtual void trimLeft             (const char c);
  virtual void trimLeft             (const char* pszCharList);
  virtual void trimLeftWhitespaces  (void);
  virtual void trimRight            (const char c);
  virtual void trimRight            (const char* pszCharList);
  virtual void trimRightWhitespaces (void);

  // converting
  virtual bool   isBool              (bool bStrict=false, int* pnOutValue=NULL) const;
  virtual bool   isInt               (bool bStrict=false) const;
  virtual bool   isFloat             (bool bStrict=false) const;
  virtual bool   isFullOfDigits      (void) const;
  virtual int    extractBool         (uint uiStartOffset=0) const;
  virtual int32  extractInt32        (uint uiStartOffset=0) const;
  virtual int32  extractInt32FromHex (uint uiStartOffset=0, uint uiMaxHexDigitsToHandle=8, bool* pbOutErrorOccurred=NULL) const;
  virtual int64  extractInt64        (uint uiStartOffset=0) const;
  virtual float  extractFloat        (uint uiStartOffset=0) const;
  virtual double extractDouble       (uint uiStartOffset=0) const;

  // formatting
  virtual void format         (const char* pszFormat, ...);
  virtual void formatV        (const char* pszFormat, va_list arglist);
  virtual void formatAppend   (const char* pszFormat, ...);
  virtual void formatAppendV  (const char* pszFormat, va_list arglist);
  virtual void formatPrepend  (const char* pszFormat, ...);
  virtual void formatPrependV (const char* pszFormat, va_list arglist);

  // formatting - memory / bandwidth measurement
  virtual int formatBytesToHuman         (uint uiBytes);
  virtual int formatBytesToHuman64       (uint64 uiBytes);
  virtual int formatAppendBytesToHuman   (uint uiBytes, const char* pszPrefix="", const char* pszSuffix="");
  virtual int formatAppendBytesToHuman64 (uint64 uiBytes, const char* pszPrefix="", const char* pszSuffix="");

  // encoding - url
  virtual void urlEscape   (StringA& strOutEscaped) const;
  virtual void urlUnescape (StringA& strOutUnescaped) const;
  virtual void urlEscape   (void);  // only for convenience... slower than urlEscape(StringA) since it has to copy source string before encoding it
  virtual void urlUnescape (void);  // only for convenience... slower than urlUnescape(StringA) since it has to copy source string before decoding it

  // encoding - utf8
  // * utf8ToAnsi() is a very simplistic converting method since it matches
  //   only characters in the 8bit range [0,255], thus only characters in the
  //   ANSI table will be converted correctly, any other UTF8 character will
  //   be converted to a '?' character.
  // * The bComputeExactLengthFirst parameter for ansiToUtf8() lets you to
  //   choose the best way to perform the conversion : memory footprint
  //   vs. speed. Computing the exact destination length implies an overhead
  //   because a loop is first performed on every characaters.
  // * OTOH, if bComputeExactLengthFirst is FALSE, the method will simply
  //   make a "Length * 2" operation to be sure the converted data will fit
  //   entirely in the destination buffer in every cases.
  virtual void ansiToUtf8 (bool bComputeExactLengthFirst=false);
  virtual void utf8ToAnsi (const uchar* pSourceUtf8String);

  // finding
  virtual int findFirstOf     (char cNeedle,            int nStartOffset=0) const;
  virtual int findFirstOf     (const char* pszNeedles,  int nStartOffset=0, int nNeedlesLength=-1) const;
  virtual int findFirstOfI    (char cNeedle,            int nStartOffset=0) const;
  virtual int findFirstOfI    (const char* pszNeedles,  int nStartOffset=0, int nNeedlesLength=-1) const;
  virtual int findFirstNotOf  (char cNeedles,           int nStartOffset=0) const;
  virtual int findFirstNotOf  (const char* pszNeedless, int nStartOffset=0, int nNeedlessLength=-1) const;
  virtual int findFirstNotOfI (char cNeedles,           int nStartOffset=0) const;
  virtual int findFirstNotOfI (const char* pszNeedless, int nStartOffset=0, int nNeedlessLength=-1) const;
  virtual int findLastOf      (char cNeedle,            int nStartOffset=-1) const;
  virtual int findLastOf      (const char* pszNeedles,  int nStartOffset=-1, int nNeedlesLength=-1) const;
  virtual int findLastOfI     (char cNeedle,            int nStartOffset=-1) const;
  virtual int findLastOfI     (const char* pszNeedles,  int nStartOffset=-1, int nNeedlesLength=-1) const;
  virtual int findLastNotOf   (char cNeedless,          int nStartOffset=-1) const;
  virtual int findLastNotOf   (const char* pszNeedless, int nStartOffset=-1, int nNeedlessLength=-1) const;
  virtual int findLastNotOfI  (char cNeedless,          int nStartOffset=-1) const;
  virtual int findLastNotOfI  (const char* pszNeedless, int nStartOffset=-1, int nNeedlessLength=-1) const;
  virtual int find            (const char cNeedle,      int nStartOffset=0, int nEndOffset=-1) const;
  virtual int find            (const char* pszNeedle,   int nStartOffset=0, int nEndOffset=-1) const;
  virtual int findI           (const char cNeedle,      int nStartOffset=0, int nEndOffset=-1) const;
  virtual int findI           (const char* pszNeedle,   int nStartOffset=0, int nEndOffset=-1) const;

  // matching
  virtual bool match  (const char* pszPattern) const;
  virtual bool matchI (const char* pszPattern) const;

  // replacing
  virtual void replace  (const char cOld, const char cNew, uint uiStartOffset=0);
  virtual void replaceI (const char cOld, const char cNew, uint uiStartOffset=0);
  virtual void replace  (const char* pszOld, const char* pszNew, uint uiStartOffset=0);
  virtual void replaceI (const char* pszOld, const char* pszNew, uint uiStartOffset=0);

  // splitting
  virtual void explode          (StringAVec& vecOutParts, const char* pszSeparator, uint uiMaxParts=0, bool bKeepEmptyParts=false) const;
  virtual void explodeWithOneOf (StringAVec& vecOutParts, const char* pszSeparators, uint uiMaxParts=0, bool bKeepEmptyParts=false) const;
  virtual void split            (const char* pszSeparator="\r\n", uint uiChunkLen=76, uint uiStartOffset=0);

  // concatenating
  virtual void implode (const StringAVec& vecParts, const char* pszGlue);

  // hashing
  virtual uint hash  (void) const;
  virtual uint hashI (void) const;

  // path string manipulation
  virtual void    pathAppend                 (const char* pszPathElement); // this is not a simple +=, it adds a trailing slash if needed before appending new path element
  virtual void    pathBackSlashesToSlashes   (void);
  virtual void    pathSlashesToBackSlashes   (void);
  virtual void    pathExtractRoot            (StringA& strDest) const;
  virtual StringA pathExtractRoot            (void) const;
  virtual void    pathExtractDirectory       (StringA& strDest) const;
  virtual StringA pathExtractDirectory       (void) const;
  virtual void    pathExtractName            (StringA& strDest) const;
  virtual StringA pathExtractName            (void) const;
  virtual void    pathExtractTitle           (StringA& strDest) const;
  virtual StringA pathExtractTitle           (void) const;
  virtual void    pathExtractExtension       (StringA& strDest) const;
  virtual StringA pathExtractExtension       (void) const;
  virtual void    pathStripDirectory         (void);
  virtual bool    pathStripTrailingSeparator (void);
  virtual void    pathStripName              (void);
  virtual void    pathStripExtension         (void);
  virtual bool    pathHasTrailingSeparator   (void) const;
  virtual void    pathExplode                (StringAVec& vecPath) const;
  virtual bool    pathCheckExtension         (const char* pszExtension) const; // case insensitive compare of the extension
  virtual void    pathChangeExtension        (const char* pszExtension);
  virtual bool    pathIsAbsolute             (void) const;
  virtual bool    pathIsRoot                 (void) const;
  virtual bool    pathIsWindowsNetwork       (void) const;  // i.e: it begins with "\\"
  virtual bool    pathExpand                 (void); // a realpath() implementation, path must be absolute ! this method is only used to expand '.' and '..' sequences and resolve symlinks
  virtual int     pathCompare                (const char* pszPath, bool bCaseSensitive) const; // note that '/' == '\\' here !
  virtual bool    pathIsInDirectory          (const char* pszDirPath, bool bCaseSensitive) const;



  // static utils - string comparison
  static int strCompare       (const char* psz1, const char* psz2);
  static int strCompareCount  (const char* psz1, const char* psz2, uint uiMaxCount);
  static int strCompareI      (const char* psz1, const char* psz2);
  static int strCompareCountI (const char* psz1, const char* psz2, uint uiMaxCount);
  static int strComparePath   (const char* pszPath1, const char* pszPath2, bool bCaseSensitive);  // warning : this method may be slow since it makes subsequent calls to pathExpand()

  // static utils - types of characters
  static char  charToLower              (int c);
  static char  charToUpper              (int c);
  static bool  charIsLower              (int c);
  static bool  charIsUpper              (int c);
  static bool  charIsAlpha              (int c);
  static bool  charIsNumeric            (int c);
  static bool  charIsAlNum              (int c);
  static bool  charIsPrintable          (int c);
  static bool  charIsSpace              (int c);
  static bool  charIsTab                (char c);
  static bool  charIsNewLine            (char c);
  static bool  charIsPathSeparator      (char c);
  static bool  charIsWindowsDriveLetter (char c);
  static bool  charIsBase64             (char c);
  static bool  charIsHex                (char c);
  static uint8 charHexToInt             (char c);
  static void  strToLower               (char* psz);
  static void  strToUpper               (char* psz);
  static void  strInvertCase            (char* psz);

  // static utils - converting
  static bool strIsBool         (const char* psz, bool bStrict=false, int* pnOutValue=NULL);
  static bool strIsInt          (const char* psz, bool bStrict=false);
  static bool strIsFloat        (const char* psz, bool bStrict=false);
  static bool strIsFullOfDigits (const char* psz);

  // static utils - finding
  static int strFindFirstOf    (const char* pszHaystack, const char cNeedle,      bool bCaseSensitive, int nStartOffset=0,  int nHaystackLength=-1);
  static int strFindFirstOf    (const char* pszHaystack, const char* pszNeedles,  bool bCaseSensitive, int nStartOffset=0,  int nHaystackLength=-1, int nNeedlesLength=-1);
  static int strFindFirstNotOf (const char* pszHaystack, const char cNeedless,    bool bCaseSensitive, int nStartOffset=0,  int nHaystackLength=-1);
  static int strFindFirstNotOf (const char* pszHaystack, const char* pszNeedless, bool bCaseSensitive, int nStartOffset=0,  int nHaystackLength=-1, int nNeedlessLength=-1);
  static int strFindLastOf     (const char* pszHaystack, const char cNeedle,      bool bCaseSensitive, int nStartOffset=-1, int nHaystackLength=-1);
  static int strFindLastOf     (const char* pszHaystack, const char* pszNeedles,  bool bCaseSensitive, int nStartOffset=-1, int nHaystackLength=-1, int nNeedlesLength=-1);
  static int strFindLastNotOf  (const char* pszHaystack, const char cNeedless,    bool bCaseSensitive, int nStartOffset=-1, int nHaystackLength=-1);
  static int strFindLastNotOf  (const char* pszHaystack, const char* pszNeedless, bool bCaseSensitive, int nStartOffset=-1, int nHaystackLength=-1, int nNeedlessLength=-1);
  static int strFind           (const char* pszHaystack, const char cNeedle,      bool bCaseSensitive, int nStartOffset=0,  int nEndOffset=-1, int nHaystackLength=-1);
  static int strFind           (const char* pszHaystack, const char* pszNeedle,   bool bCaseSensitive, int nStartOffset=0,  int nEndOffset=-1, int nHaystackLength=-1);

  // static utils - matching using wildcards
  static bool strMatch (const char* pszHaystack, const char* pszPattern, bool bCaseSensitive);

  // static utils - string hashing
  static uint32 strHash  (const char* psz);
  static uint32 strHash  (const char* pBlock, uint uiCount);
  static uint32 strHashI (const char* psz);
  static uint32 strHashI (const char* pBlock, uint uiCount);



  // appending - friends operators
  friend StringA operator+ (const StringA& lhs, const StringA& rhs);
  friend StringA operator+ (const StringA& lhs, const char* rhs);
  friend StringA operator+ (const char* lhs, const StringA& rhs);
  friend StringA operator+ (const StringA& lhs, const bool   b);
  friend StringA operator+ (const StringA& lhs, const char   c);
  friend StringA operator+ (const StringA& lhs, const int32  i);
  friend StringA operator+ (const StringA& lhs, const uint32 ui);
  friend StringA operator+ (const StringA& lhs, const int64  i);
  friend StringA operator+ (const StringA& lhs, const uint64 ui);
  friend StringA operator+ (const StringA& lhs, const float  f);
  friend StringA operator+ (const StringA& lhs, const double d);
#ifdef _STRING_
  friend StringA operator+ (const StringA& lhs, const std::string& rhs);
  friend StringA operator+ (const std::string& lhs, const StringA& rhs);
#endif

  // path appending - friends operators
  friend StringA operator/ (const StringA& lhs, const StringA& rhs);
  friend StringA operator/ (const StringA& lhs, const char* rhs);
  friend StringA operator/ (const char* lhs, const StringA& rhs);
#ifdef _STRING_
  friend StringA operator/ (const StringA& lhs, const std::string& rhs);
  friend StringA operator/ (const std::string& lhs, const StringA& rhs);
#endif

  // case sensitive comparison - friends operators
  friend bool operator== (const StringA& lhs, const StringA& rhs);
  friend bool operator== (const StringA& lhs, const char* rhs);
  friend bool operator== (const char* lhs, const StringA& rhs);
  friend bool operator!= (const StringA& lhs, const StringA& rhs);
  friend bool operator!= (const StringA& lhs, const char* rhs);
  friend bool operator!= (const char* lhs, const StringA& rhs);
  friend bool operator<  (const StringA& lhs, const StringA& rhs);
  friend bool operator<  (const StringA& lhs, const char* rhs);
  friend bool operator<  (const char* lhs, const StringA& rhs);
  friend bool operator>  (const StringA& lhs, const StringA& rhs);
  friend bool operator>  (const StringA& lhs, const char* rhs);
  friend bool operator>  (const char* lhs, const StringA& rhs);
  friend bool operator<= (const StringA& lhs, const StringA& rhs);
  friend bool operator<= (const StringA& lhs, const char* rhs);
  friend bool operator<= (const char* lhs, const StringA& rhs);
  friend bool operator>= (const StringA& lhs, const StringA& rhs);
  friend bool operator>= (const StringA& lhs, const char* rhs);
  friend bool operator>= (const char* lhs, const StringA& rhs);
#ifdef _STRING_
  friend bool operator== (const StringA& lhs, const std::string& rhs);
  friend bool operator== (const std::string& lhs, const StringA& rhs);
  friend bool operator!= (const StringA& lhs, const std::string& rhs);
  friend bool operator!= (const std::string& lhs, const StringA& rhs);
  friend bool operator<  (const StringA& lhs, const std::string& rhs);
  friend bool operator<  (const std::string& lhs, const StringA& rhs);
  friend bool operator>  (const StringA& lhs, const std::string& rhs);
  friend bool operator>  (const std::string& lhs, const StringA& rhs);
  friend bool operator<= (const StringA& lhs, const std::string& rhs);
  friend bool operator<= (const std::string& lhs, const StringA& rhs);
  friend bool operator>= (const StringA& lhs, const std::string& rhs);
  friend bool operator>= (const std::string& lhs, const StringA& rhs);
#endif



protected :
  void init          (void);
  bool isBufferAlloc (void) const;



protected :
  uint32 m_uiLength;
  char*  m_pszData;
  uint32 m_uiAllocSize;
  char   m_szBaseBuffer[STRING_BASE_BUFFER_SIZE];


protected :
  static BoolStringType ms_eBoolStringType;
};


#include "StringA.inl.hpp"


} // namespace glb


//---------------------------------------------------------------------------
// hash<StringA>
//
// This specialization of the hash<> struct will allows us to use StringA as
// a key for the std::hash_* containers.
//---------------------------------------------------------------------------
/*namespace glb_hashmap_namespace
{
#if defined(STLPORT)
  #if !defined(_STLP_HASH_FUN_H)
    #error At least one of the STLport <hash_*> headers must be included !
  #endif
  template<> struct hash<::glb::StringA>
  {
    size_t operator() (const ::glb::StringA& str) const
    {
      // __stl_hash_string() can be found in "stlport/stl/_hash_fun.h"
      //return _STLP_PRIV __stl_hash_string(str.c_str());
      return str.Hash();
    }
  };

#elif defined(GLB_COMPILER_MSVC)
  template<> inline size_t hash_value (const ::glb::StringA& str)
  {
    //return hash_value(str.c_str());
    return str.Hash();
  }

#elif defined(GLB_COMPILER_GCC)
  template<> struct hash<glb::StringA>
  {
    size_t operator() (const ::glb::StringA& str) const
    {
      // __stl_hash_string() can be found in <ext/stl_hash_fun.h>
      //return __stl_hash_string(str.c_str());
      return str.Hash();
    }
  };

#else
#error Not implemented yet !
#endif
}*/


#endif // #ifndef __GLOBE_STRINGA_H__
