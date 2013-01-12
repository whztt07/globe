//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-07 18:46:25
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
//#define INTSIGNBITSET(i)     (((const unsigned long)(i)) >> 31)
#define INTSIGNBITNOTSET(i)  ((~((const unsigned long)(i))) >> 31)


//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------
static const char* c_apszBytesUnits[5] = { "B", "KB", "MB", "GB", "TB" };

// If you make changes to this structure, be sure to reflect your changes to
// the following methods :
// * StringA::strIsBool()
// * StringA::operator=(const bool)
// * StringA::append(const bool)
// And in some cases, you may need to check the StringA::BoolStringType
// enum...
static const struct c_aBooleanStrings_
{
  struct b_
  {
    uint8 uiLen;
    const char* psz;
  }
  b[2];
}
c_aBooleanStrings[4] =
{
  {{ { 2,"no"    }, { 3,"yes"  } }}, // BOOLSTRINGTYPE_YESNO
  {{ { 5,"false" }, { 4,"true" } }}, // BOOLSTRINGTYPE_TRUEFALSE
  {{ { 3,"off"   }, { 2,"on"   } }}, // BOOLSTRINGTYPE_ONOFF
  {{ { 1,"0"     }, { 1,"1"    } }}, // BOOLSTRINGTYPE_IO
};



//---------------------------------------------------------------------------
// Static Members
//---------------------------------------------------------------------------
StringA::BoolStringType StringA::ms_eBoolStringType = StringA::BOOLSTRINGTYPE_YESNO;




//---------------------------------------------------------------------------
// reset
//---------------------------------------------------------------------------
void StringA::reset (void)
{
  if (this->isBufferAlloc())
    free(m_pszData);

  this->init();
}




//---------------------------------------------------------------------------
// resize
//---------------------------------------------------------------------------
void StringA::resize (uint uiDesiredSize, bool bKeepOldContent/*=true*/)
{
  uint uiTemp;

  GLB_ASSERT(uiDesiredSize >= 1);

  // do we need to resize ?
  if (uiDesiredSize == m_uiAllocSize)
    return;

  // pad desired size to the nearest grow size boundary
  if (uiDesiredSize <= STRING_BASE_BUFFER_SIZE)
  {
    uiDesiredSize = STRING_BASE_BUFFER_SIZE;
  }
  else
  {
    uiTemp = uiDesiredSize % STRING_GROW_SIZE;
    if (uiTemp != 0)
      uiDesiredSize = uiDesiredSize + (STRING_GROW_SIZE - uiTemp);
  }
  GLB_ASSERT(uiDesiredSize >= 1);

  // shrink string length if needed
  if (!bKeepOldContent)
  {
    m_uiLength = 0;
  }
  else if (m_uiLength > (uiDesiredSize - 1))
  {
    m_uiLength = uiDesiredSize - 1;
    m_pszData[m_uiLength] = 0;
  }

  // (re)allocate buffer if needed
  if (this->isBufferAlloc())
  {
    if (uiDesiredSize <= STRING_BASE_BUFFER_SIZE)
    {
      if (bKeepOldContent)
      {
        m_pszData[m_uiLength] = 0;
        strcpy((char*)&m_szBaseBuffer, m_pszData); // do not use memcpy() here, this is not secure enough !
      }
      else
      {
        m_szBaseBuffer[0] = 0;
      }

      free(m_pszData);
      m_pszData = (char*)&m_szBaseBuffer;
    }
    else
    {
      m_pszData = (char*)realloc(m_pszData, uiDesiredSize);
      m_pszData[m_uiLength] = 0;
    }
  }
  else if (uiDesiredSize > STRING_BASE_BUFFER_SIZE)
  {
    m_pszData = (char*)malloc(uiDesiredSize);
    if (bKeepOldContent)
    {
      m_szBaseBuffer[m_uiLength] = 0;
      strcpy(m_pszData, (char*)&m_szBaseBuffer); // do not use memcpy() here, this is not secure enough !
    }
    else
    {
      m_pszData[m_uiLength] = 0;
    }
  }
  GLB_ASSERT(m_pszData[m_uiLength] == 0);
  GLB_ASSERT((uint)strlen(m_pszData) == m_uiLength);

  // update alloc size
  m_uiAllocSize = uiDesiredSize;
}




//---------------------------------------------------------------------------
// acquireBuffer
//---------------------------------------------------------------------------
char* StringA::acquireBuffer (uint uiMinRequiredBufferSize, bool bKeepOldContent/*=true*/)
{
  this->grow(uiMinRequiredBufferSize, bKeepOldContent);
  return m_pszData;
}

//---------------------------------------------------------------------------
// releaseBuffer
//---------------------------------------------------------------------------
void StringA::releaseBuffer (uint uiNewStringLength/*=0*/)
{
  if (uiNewStringLength <= 0)
  {
    m_uiLength = (uint)strlen(m_pszData);
  }
  else
  {
    m_uiLength = uiNewStringLength;
    GLB_ASSERT(m_pszData[m_uiLength] == 0);
  }

  GLB_ASSERT(m_uiLength < m_uiAllocSize);
  if (m_uiLength >= m_uiAllocSize)
  {
    m_uiLength = m_uiAllocSize - 1;
    m_pszData[m_uiLength] = 0;
  }
}




//---------------------------------------------------------------------------
// copy
//---------------------------------------------------------------------------
void StringA::copy (const StringA& strSource)
{
  GLB_ASSERT(&strSource != this);
  if (&strSource == this)
    return;

  this->grow(strSource.m_uiLength + 1, false);

  memcpy(m_pszData, strSource.m_pszData, strSource.m_uiLength);
  m_pszData[strSource.m_uiLength] = 0;
  m_uiLength = strSource.m_uiLength;
}

//---------------------------------------------------------------------------
// copy
//---------------------------------------------------------------------------
void StringA::copy (const char* pszSource)
{
  // handle null destination string
  if (!pszSource)
  {
    this->grow(1, false);
    m_uiLength   = 0;
    m_pszData[0] = 0;
    return;
  }

  // want to copy the same thing ?
  if (pszSource == m_pszData)
    return;

  // is it simply aliasing ?
  if ((pszSource >= m_pszData) && (pszSource <= (m_pszData + m_uiLength)))
  {
    uint Ix;

    GLB_ASSERT(strlen(pszSource) <= m_uiLength);

    for (Ix = 0; pszSource[Ix]; ++Ix)
      m_pszData[Ix] = pszSource[Ix];
    m_pszData[Ix] = 0;

    m_uiLength = Ix;
  }
  else
  {
    uint uiLen;

    // ensure we are big enough
    uiLen = (uint)strlen(pszSource);
    this->grow(uiLen + 1, false);

    // copy
    memcpy(m_pszData, pszSource, uiLen);
    m_pszData[uiLen] = 0;
    m_uiLength = uiLen;
  }
}

//---------------------------------------------------------------------------
// copyCount
//---------------------------------------------------------------------------
void StringA::copyCount (const char* pszSource, uint uiSourceLength, uint uiSourceStartOffset/*=0*/)
{
  // handle null destination string
  if (!pszSource)
  {
    this->grow(1, false);
    m_uiLength   = 0;
    m_pszData[0] = 0;
    return;
  }

  // adjust source string if needed
  if (uiSourceStartOffset >= 1)
  {
    GLB_ASSERT(uiSourceStartOffset < (uint)strlen(pszSource));
    pszSource += uiSourceStartOffset;
  }

  // want to copy the same thing ?
  if (pszSource == m_pszData)
  {
    if (uiSourceLength < m_uiLength)
    {
      m_uiLength = uiSourceLength;
      m_pszData[m_uiLength] = 0;
    }
    return;
  }

  // is it simply aliasing ?
  if ((pszSource >= m_pszData) && (pszSource <= (m_pszData + m_uiLength)))
  {
    uint Ix;

    GLB_ASSERT((uint)strlen(pszSource) <= m_uiLength);

    for (Ix = 0; pszSource[Ix] && (Ix < uiSourceLength); ++Ix)
      m_pszData[Ix] = pszSource[Ix];
    m_pszData[Ix] = 0;

    m_uiLength = Ix;
  }
  else
  {
    uint uiLen;

    // ensure we are big enough
    uiLen = (uint)strlen(pszSource);
    if (uiLen > uiSourceLength)
      uiLen = uiSourceLength;
    this->grow(uiLen + 1, false);

    // copy
    memcpy(m_pszData, pszSource, (size_t)uiLen);
    m_pszData[uiLen] = 0;
    m_uiLength = uiLen;
  }
}




//---------------------------------------------------------------------------
// operator=
//---------------------------------------------------------------------------
void StringA::operator= (const bool b)
{
  int  nBool = (int)b;
  uint uiLen = (uint)c_aBooleanStrings[ms_eBoolStringType].b[nBool].uiLen;

  this->grow(uiLen + 1, false);
  memcpy(m_pszData, c_aBooleanStrings[ms_eBoolStringType].b[nBool].psz, size_t(uiLen + 1));
  m_uiLength = uiLen;
}

//---------------------------------------------------------------------------
// operator=
//---------------------------------------------------------------------------
void StringA::operator= (const float f)
{
  char tmp[64];
  int  nLen;

  nLen = snprintf((char*)&tmp, sizeof(tmp), "%f", f);
  if (nLen < 0)
  {
    this->clear();
    return;
  }

	while ((nLen > 0) && (tmp[nLen-1] == '0'))
    tmp[--nLen] = 0;
	while ((nLen > 0) && (tmp[nLen-1] == '.'))
    tmp[--nLen] = 0;
  GLB_ASSERT((int)strlen((char*)&tmp) == nLen);

  this->grow((uint)nLen + 1, false);

  memcpy(m_pszData, (char*)&tmp, size_t(nLen + 1));
  m_uiLength = (uint)nLen;
}

//---------------------------------------------------------------------------
// operator=
//---------------------------------------------------------------------------
void StringA::operator= (const double d)
{
  char tmp[64];
  int  nLen;

  nLen = snprintf((char*)&tmp, sizeof(tmp), "%lf", d);
  if (nLen < 0)
  {
    this->clear();
    return;
  }

	while ((nLen > 0) && (tmp[nLen-1] == '0'))
    tmp[--nLen] = 0;
	while ((nLen > 0) && (tmp[nLen-1] == '.'))
    tmp[--nLen] = 0;
  GLB_ASSERT((int)strlen((char*)&tmp) == nLen);

  this->grow((uint)nLen + 1, false);

  memcpy(m_pszData, (char*)&tmp, size_t(nLen + 1));
  m_uiLength = (uint)nLen;
}




//---------------------------------------------------------------------------
// append
//---------------------------------------------------------------------------
void StringA::append (const StringA& str)
{
  uint uiFinalLen = m_uiLength + str.m_uiLength;

  this->grow(uiFinalLen + 1, true);

  memcpy((void*)(m_pszData + m_uiLength), str.m_pszData, str.m_uiLength);
  m_pszData[uiFinalLen] = 0;
  m_uiLength = uiFinalLen;
}

//---------------------------------------------------------------------------
// append
//---------------------------------------------------------------------------
void StringA::append (const StringA& str, uint uiLength)
{
  uint uiFinalLen;

  if (!uiLength)
    return;
  if (uiLength > str.m_uiLength)
    uiLength = str.m_uiLength;

  uiFinalLen = m_uiLength + uiLength;
  this->grow(uiFinalLen + 1, true);

  memcpy((void*)(m_pszData + m_uiLength), str.m_pszData, uiLength);
  m_pszData[uiFinalLen] = 0;
  m_uiLength = uiFinalLen;
}

//---------------------------------------------------------------------------
// append
//---------------------------------------------------------------------------
void StringA::append (const char* psz)
{
  uint uiSrcLen;
  uint uiFinalLen;

  GLB_ASSERT(psz);
  if (!psz)
    return;

  uiSrcLen   = (uint)strlen(psz);
  uiFinalLen = m_uiLength + uiSrcLen;

  this->grow(uiFinalLen + 1, true);

  memcpy((void*)(m_pszData + m_uiLength), psz, uiSrcLen);
  m_pszData[uiFinalLen] = 0;
  m_uiLength = uiFinalLen;
}

//---------------------------------------------------------------------------
// append
//---------------------------------------------------------------------------
void StringA::append (const char* psz, uint uiLength)
{
  uint uiFinalLen;
  uint Ix;

  GLB_ASSERT(psz);
  if (!psz || !uiLength)
    return;

  uiFinalLen = m_uiLength + uiLength;

  this->grow(uiFinalLen + 1, true);

  for (Ix = 0; psz[Ix] && Ix < uiLength; ++Ix)
    m_pszData[m_uiLength + Ix] = psz[Ix];
  m_uiLength += Ix;

  m_pszData[m_uiLength] = 0;
  m_uiLength = uiFinalLen;
}

//---------------------------------------------------------------------------
// appendUrlEscape
//---------------------------------------------------------------------------
void StringA::appendUrlEscape (const char* psz)
{
  StringA strSrc(psz);
  StringA strDst;

  strSrc.urlEscape(strDst);
  *this += strDst;
}

//---------------------------------------------------------------------------
// append
//---------------------------------------------------------------------------
void StringA::append (const bool b)
{
  int nBool = (int)b;
  this->append(c_aBooleanStrings[ms_eBoolStringType].b[nBool].psz, (uint)c_aBooleanStrings[ms_eBoolStringType].b[nBool].uiLen);
}




//---------------------------------------------------------------------------
// insert
//---------------------------------------------------------------------------
void StringA::insert (const char c, uint uiOffset)
{
  uint Ix;

  if (uiOffset > m_uiLength)
    uiOffset = m_uiLength;

  this->grow(m_uiLength + 2, true);

  for (Ix = m_uiLength; Ix >= uiOffset; --Ix)
    m_pszData[Ix+1] = m_pszData[Ix];

  m_pszData[uiOffset] = c;
  ++m_uiLength;
}

//---------------------------------------------------------------------------
// insert
//---------------------------------------------------------------------------
void StringA::insert (const char* psz, uint uiOffset, uint uiLength/*=0*/)
{
  uint Ix;
  uint uiSrcLen;

  GLB_ASSERT(psz);
  if (!psz || !psz[0])
    return;

  // grow buffer
  uiSrcLen = (uint)strlen(psz);
  if ((uiLength == 0) || (uiLength > uiSrcLen))
    uiLength = uiSrcLen;
  this->grow(m_uiLength + uiLength + 1, true);

  // move tail to the right
  for (Ix = m_uiLength; Ix > uiOffset; --Ix)
    m_pszData[Ix+uiLength] = m_pszData[Ix];
  m_pszData[Ix+uiLength] = m_pszData[Ix];

  // fill-in the gap
  for (Ix = 0; Ix < uiLength; ++Ix)
    m_pszData[uiOffset+Ix] = psz[Ix];
  m_uiLength += uiLength;
}




//---------------------------------------------------------------------------
// erase
//---------------------------------------------------------------------------
void StringA::erase (int nOffset, uint uiLength/*=0*/)
{
  if (nOffset < 0)
    nOffset = (int)m_uiLength + nOffset;
  else if ((uint)nOffset >= m_uiLength)
    return;

  if (uiLength == 0)
  {
    uiLength = m_uiLength - (uint)nOffset;
  }
  else if (((uint)nOffset + uiLength) > m_uiLength)
  {
    m_pszData[nOffset] = 0;
    m_uiLength = (uint)nOffset;
    return;
  }
  GLB_ASSERT(((uint)nOffset + uiLength) <= m_uiLength);

  memmove(&m_pszData[nOffset], &m_pszData[(uint)nOffset + uiLength], m_uiLength - (uint)nOffset - uiLength + 1);
  m_uiLength -= uiLength;
  GLB_ASSERT(m_uiLength == (uint)strlen(m_pszData));
}




//---------------------------------------------------------------------------
// fill
//---------------------------------------------------------------------------
void StringA::fill (const char c, uint uiCount)
{
  GLB_ASSERT(uiCount);
  if (!uiCount)
    return;

  this->grow(uiCount + 1, false);

  memset(m_pszData, c, uiCount);
  m_pszData[uiCount] = 0;
  m_uiLength = uiCount;
}

//---------------------------------------------------------------------------
// fill
//---------------------------------------------------------------------------
void StringA::fill (const char* psz, uint uiCount)
{
  uint  uiSrcLen;
  uint  uiFinalLen;
  char* pDst;

  GLB_ASSERT(psz);
  GLB_ASSERT(uiCount);
  if (!psz || !uiCount)
    return;

  uiSrcLen   = (uint)strlen(psz);
  uiFinalLen = uiSrcLen * uiCount;

  this->grow(uiFinalLen + 1, false);

  pDst = m_pszData;
  while (uiCount--)
  {
    memcpy(pDst, psz, uiSrcLen);
    pDst += uiSrcLen;
  }
  m_pszData[uiFinalLen] = 0;

  m_uiLength = uiFinalLen;
}




//---------------------------------------------------------------------------
// pad
//---------------------------------------------------------------------------
void StringA::pad (const char c, uint uiDesiredLength)
{
  if (m_uiLength >= uiDesiredLength)
    return;

  this->grow(uiDesiredLength + 1, true);

  memset(m_pszData + m_uiLength, c, uiDesiredLength - m_uiLength);
  m_pszData[uiDesiredLength] = 0;
  m_uiLength = uiDesiredLength;
}

//---------------------------------------------------------------------------
// pad
//---------------------------------------------------------------------------
void StringA::pad (const char* psz, uint uiDesiredLength)
{
  uint  uiSrcLen;
  uint  uiLenToFill;
  char* pDst;

  GLB_ASSERT(psz);
  if (!psz)
    return;

  if (m_uiLength >= uiDesiredLength)
    return;

  this->grow(uiDesiredLength + 1, true);

  uiSrcLen    = (uint)strlen(psz);
  uiLenToFill = uiDesiredLength - m_uiLength;
  pDst        = m_pszData + m_uiLength;

  while (uiLenToFill >= uiSrcLen)
  {
    memcpy(pDst, psz, uiSrcLen);
    pDst        += uiSrcLen;
    uiLenToFill -= uiSrcLen;
  }
  GLB_ASSERT(uiLenToFill < uiSrcLen);
  memcpy(pDst, psz, uiLenToFill);

  m_pszData[uiDesiredLength] = 0;
  m_uiLength = uiDesiredLength;
}




//---------------------------------------------------------------------------
// mid
//---------------------------------------------------------------------------
StringA StringA::mid (uint uiOffset, uint uiLength) const
{
  uint    uiMyLen;
  StringA strResult;

  strResult.clear();

  uiMyLen = m_uiLength;
  if (!uiMyLen || (uiOffset >= uiMyLen))
    return NULL;

  if ((uiOffset + uiLength) >= uiMyLen)
    uiLength = uiMyLen - uiOffset;

  strResult.append((char*)&m_pszData[uiOffset], uiLength);

  return strResult;
}

//---------------------------------------------------------------------------
// mid
//---------------------------------------------------------------------------
const char* StringA::mid (uint uiOffset, uint uiLength, StringA& strOutResult) const
{
  uint uiMyLen;

  strOutResult.clear();

  uiMyLen = m_uiLength;
  if (!uiMyLen || (uiOffset >= uiMyLen))
    return NULL;

  if ((uiOffset + uiLength) >= uiMyLen)
    uiLength = uiMyLen - uiOffset;

  strOutResult.append((char*)&m_pszData[uiOffset], uiLength);

  return strOutResult;
}




//---------------------------------------------------------------------------
// substr
//---------------------------------------------------------------------------
const char* StringA::substr (StringA& strOutResult, int nOffset, int nLength/*=0*/) const
{
  strOutResult.clear();

  if (nOffset > 0)
  {
    if (nLength > 0)
    {
      return this->mid((uint)nOffset, (uint)nLength, strOutResult);
    }
    else if (nLength == 0)
    {
      return this->mid((uint)nOffset, m_uiLength, strOutResult);
    }
    else
    {
      nLength = nOffset + -nLength;
      if ((uint)nLength > m_uiLength)
        nLength = (int)m_uiLength;

      return this->mid((uint)nOffset, m_uiLength - (uint)nLength, strOutResult);
    }
  }
  else if (nOffset == 0)
  {
    if (nLength > 0)
    {
      return this->left((uint)nLength, strOutResult);
    }
    else if (nLength == 0)
    {
      strOutResult.copyCount(m_pszData, m_uiLength);
      return strOutResult;
    }
    else
    {
      nLength = -nLength;
      if ((uint)nLength > m_uiLength)
        nLength = (int)m_uiLength;

      return this->left(m_uiLength - (uint)nLength, strOutResult);
    }
  }
  else // if (nOffset < 0)
  {
    nOffset = -nOffset;
    if ((uint)nOffset >= m_uiLength)
      return NULL;

    if (nLength > 0)
    {
      return this->mid(m_uiLength - (uint)nOffset, (uint)nLength, strOutResult);
    }
    else if (nLength == 0)
    {
      return this->right((uint)nOffset, strOutResult);
    }
    else
    {
      nLength = nOffset + -nLength;
      if ((uint)nLength > m_uiLength)
        nLength = (int)m_uiLength;

      return this->mid(m_uiLength - (uint)nOffset, m_uiLength - (uint)nLength, strOutResult);
    }
  }
}




//---------------------------------------------------------------------------
// trimLeft
//---------------------------------------------------------------------------
void StringA::trimLeft (const char c)
{
  int   nLastFound = -1;
  char* p = m_pszData;

  while (*p && (*p == c))
  {
    ++nLastFound;
    ++p;
  }
  if (nLastFound < 0)
    return;  // none found

  memmove(&m_pszData[0], &m_pszData[nLastFound + 1], size_t(m_uiLength - nLastFound));
  m_uiLength -= (uint)nLastFound + 1;
}

//---------------------------------------------------------------------------
// trimLeft
//---------------------------------------------------------------------------
void StringA::trimLeft (const char* pszCharList)
{
  int   nLastFound = -1;
  char* pDst = m_pszData;
  char* pSrc;
  bool  bFound;

  while (*pDst)
  {
    bFound = false;
    for (pSrc = (char*)pszCharList; *pSrc; ++pSrc)
    {
      if (*pDst == *pSrc)
      {
        bFound = true;
        break;
      }
    }
    if (!bFound)
      break;

    ++nLastFound;
    ++pDst;
  }
  if (nLastFound < 0)
    return;

  memmove(&m_pszData[0], &m_pszData[nLastFound + 1], size_t(m_uiLength - nLastFound));
  m_uiLength -= (uint)nLastFound + 1;
}

//---------------------------------------------------------------------------
// trimRight
//---------------------------------------------------------------------------
void StringA::trimRight (const char c)
{
  char* p;

  if (m_uiLength <= 0)
    return;

  for (p = (m_pszData + m_uiLength - 1); (p >= m_pszData) && (*p == c); --p)
  {
    *p = 0;
    --m_uiLength;
  }
}

//---------------------------------------------------------------------------
// trimRight
//---------------------------------------------------------------------------
void StringA::trimRight (const char* pszCharList)
{
  char* p;
  char* pChar;

  if (m_uiLength <= 0)
    return;

  for (p = (m_pszData + m_uiLength - 1); (p >= m_pszData); --p)
  {
    for (pChar = (char*)pszCharList; *pChar; ++pChar)
    {
      if (*p == *pChar)
      {
        *p = 0;
        --m_uiLength;
        break;
      }
    }

    if (*p != 0)
      break;
  }
}




//---------------------------------------------------------------------------
// extractBool
//---------------------------------------------------------------------------
int StringA::extractBool (uint uiStartOffset/*=0*/) const
{
  char* pszOffset;
  int   nValue;

  if (uiStartOffset >= m_uiLength)
    return -1;
  pszOffset = m_pszData + uiStartOffset;

  StringA::strIsBool(pszOffset, false, &nValue);
  GLB_ASSERT((nValue >= -1) && (nValue <= 1));

  return nValue;
}

//---------------------------------------------------------------------------
// extractInt32
//---------------------------------------------------------------------------
int32 StringA::extractInt32 (uint uiStartOffset/*=0*/) const
{
  char* pszOffset;

  if (uiStartOffset >= m_uiLength)
    return -1;
  pszOffset = m_pszData + uiStartOffset;

  return atoi(pszOffset);
}

//---------------------------------------------------------------------------
// extractInt32FromHex
//---------------------------------------------------------------------------
int32 StringA::extractInt32FromHex (uint uiStartOffset/*=0*/, uint uiMaxHexDigitsToHandle/*=8*/, bool* pbOutErrorOccurred/*=NULL*/) const
{
  int32 nReturn = 0;
  char* p = m_pszData;
  uint8 n;
  bool  bFirstLoop = true;

  // reset state
  if (pbOutErrorOccurred)
    *pbOutErrorOccurred = false;

  // skip to the starting offset
  GLB_ASSERT(uiStartOffset < m_uiLength);
  if (uiStartOffset >= m_uiLength)
    goto __Error;
  p += uiStartOffset;

  // skip whitespaces
  while ((*p == ' ') || (*p == '\t'))
    ++p;

  // skip the '0x' prefix
  if ((p[0] == '0') && ((p[1] == 'x') || (p[1] == 'X')))
    p += 2;

  // proceed
  for ( ; *p && uiMaxHexDigitsToHandle; ++p, --uiMaxHexDigitsToHandle)
  {
    n = StringA::charHexToInt(*p);
    if (n == uint8(-1))
      goto __Error;

    if (!bFirstLoop)
      nReturn <<= 4;
    nReturn |= n;

    bFirstLoop = false;
  }

  return nReturn;

__Error :
  if (pbOutErrorOccurred)
    *pbOutErrorOccurred = true;
  return 0;
}

//---------------------------------------------------------------------------
// extractInt64
//---------------------------------------------------------------------------
int64 StringA::extractInt64 (uint uiStartOffset/*=0*/) const
{
  char* pszOffset;

  if (uiStartOffset >= m_uiLength)
    return -1;
  pszOffset = m_pszData + uiStartOffset;

#ifdef GLB_COMPILER_MSVC
  return _atoi64(pszOffset);
#else
  return atoll(pszOffset);
#endif
}

//---------------------------------------------------------------------------
// extractDouble
//---------------------------------------------------------------------------
double StringA::extractDouble (uint uiStartOffset/*=0*/) const
{
  char* pszOffset;

  if (uiStartOffset >= m_uiLength)
    return -1;
  pszOffset = m_pszData + uiStartOffset;

  return atof(pszOffset);
}




//---------------------------------------------------------------------------
// formatV
//---------------------------------------------------------------------------
void StringA::formatV (const char* pszFormat, va_list arglist)
{
  #if defined(GLB_COMPILER_MSVC) && (GLB_COMPILER_VERSION >= 1300)
  {
    int nLen;
    int nRes;

    nLen = _vscprintf(pszFormat, arglist);
    if (nLen < 0)
      return;

    this->grow((uint)nLen + 1, false);
    GLB_ASSERT(m_uiAllocSize >= (uint)nLen + 1);

    nRes = vsnprintf(m_pszData, (size_t)m_uiAllocSize, pszFormat, arglist);
    GLB_ASSERT(nRes == nLen);
    GLB_ASSERT(m_pszData[nRes] == 0);

    m_uiLength = (uint)nRes;
  }
  #elif defined(GLB_COMPILER_GCC) && ( (__GNUC__ > 2) || ((__GNUC__ == 2) && (__GNUC_MINOR__ >= 1)) )
  {
    // this vsnprintf() behavior requires requires glibc version >= 2.1
    // it seems to be also supported by the libc on macosx

    static char szTemp[2];
    int nLen;
    int nRes;

    nLen = vsnprintf((char*)&szTemp, 1, pszFormat, arglist);
    if (nLen < 0)
      return;

    this->grow((uint)nLen + 1, false);
    GLB_ASSERT(m_uiAllocSize >= (uint)nLen + 1);

    nRes = vsnprintf(m_pszData, (size_t)m_uiAllocSize, pszFormat, arglist);
    GLB_ASSERT(nRes == nLen);
    GLB_ASSERT(m_pszData[nRes] == 0);

    m_uiLength = (uint)nRes;
  }
  #else // *too* old compilers fall down here, seriously, you should upgrade :)
  {
    // this default implementation try not to allocate too much memory but
    // this implies an overhead since it has to try to vsprintf() until it
    // does not return an error...
    // note that for safety reasons, i've added a strong limit for the
    // maximum buffer size (c_uiMaxAllocSize).
    // note also that it is ok to modify the c_auiGrowSizes and
    // c_uiMaxAllocSize but please keep those values as much reasonable as
    // you can.  - jcl

    static const uint c_auiGrowSizes[]     = { 32, 64, 128, 256, 512, 1024, 2048 }; // arbitrary values
    static const int  c_nLastGrowSizeIndex = (sizeof(c_auiGrowSizes) / sizeof(uint)) - 1;
    static const uint c_uiMaxAllocSize     = 0x10000; // 64K bytes *formatting* should be enough for everyone, even for java style "coders"

    int  nRes;
    int  nGrowIndex = -1;
    uint uiGrowSize;
    uint uiAllocSize;

    nRes = vsnprintf(m_pszData, (size_t)m_uiAllocSize, pszFormat, arglist);
    while (nRes < 0)
    {
      // check max alloc size
      if (m_uiAllocSize >= c_uiMaxAllocSize)
      {
        uint uiLen;

        m_uiLength = m_uiAllocSize - 1;
        m_pszData[m_uiLength] = 0;

        // be sure length corresponds to the real length of string
        uiLen = (uint)strlen(m_pszData);
        GLB_ASSERT(uiLen <= m_uiLength);
        if (uiLen < m_uiLength)
        {
          m_uiLength = uiLen;
          m_pszData[m_uiLength] = 0;
        }

        return;
      }

      // increment grow size index
      if (nGrowIndex < c_nLastGrowSizeIndex)
        uiGrowSize = c_auiGrowSizes[++nGrowIndex];

      // compute alloc size
      uiAllocSize = m_uiAllocSize + uiGrowSize;
      if (uiAllocSize > c_uiMaxAllocSize)
        uiAllocSize = c_uiMaxAllocSize;

      // grow buffer
      this->grow(uiAllocSize, false);
      GLB_ASSERT(m_uiAllocSize >= uiAllocSize);

      // format
      nRes = vsnprintf(m_pszData, (size_t)m_uiAllocSize, pszFormat, arglist);
    }

    GLB_ASSERT(strlen(m_pszData) == (size_t)nRes);
    m_uiLength = (uint)nRes;
  }
  #endif
}




//---------------------------------------------------------------------------
// formatAppendBytesToHuman
//---------------------------------------------------------------------------
int StringA::formatAppendBytesToHuman (uint uiBytes, const char* pszPrefix/*=""*/, const char* pszSuffix/*=""*/)
{
  int   nUnit = 1;
  float rValue;

  while ((nUnit <= 4) && (uint(1 << (nUnit * 10)) < uiBytes))
    ++nUnit;
  --nUnit;

  rValue = float(uiBytes) / float(1 << (nUnit * 10));
  this->formatAppend("%s%.02f %s%s", pszPrefix?pszPrefix:"", rValue, c_apszBytesUnits[nUnit], pszSuffix?pszSuffix:"");

  return nUnit;
}

//---------------------------------------------------------------------------
// formatAppendBytesToHuman64
//---------------------------------------------------------------------------
int StringA::formatAppendBytesToHuman64 (uint64 uiBytes, const char* pszPrefix/*=""*/, const char* pszSuffix/*=""*/)
{
  int    nUnit = 1;
  double rValue;

  while ((nUnit <= 4) && (uint64(1 << (nUnit * 10)) < uiBytes))
    ++nUnit;
  --nUnit;

  rValue = double(uiBytes) / double(1 << (nUnit * 10));
  this->formatAppend("%s%.02lf %s%s", pszPrefix?pszPrefix:"", rValue, c_apszBytesUnits[nUnit], pszSuffix?pszSuffix:"");

  return nUnit;
}




//---------------------------------------------------------------------------
// urlEscape
//---------------------------------------------------------------------------
void StringA::urlEscape (StringA& strOutEscaped) const
{
  uint32 Ix;
  uchar* pSrc;
  uchar* pDst;
  uint32 uiRequiredLen = m_uiLength;

  pSrc = (uchar*)m_pszData;
  pDst = (uchar*)strOutEscaped.acquireBuffer(uiRequiredLen + 1, false);

  for (Ix = 0; *pSrc; ++pSrc)
  {
    // trick from libcurl :
    // portable character check (remember EBCDIC), do not use isalnum()
    // because its behavior is altered by the current locale.
    switch (*pSrc)
    {
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
      case 'a': case 'b': case 'c': case 'd': case 'e':
      case 'f': case 'g': case 'h': case 'i': case 'j':
      case 'k': case 'l': case 'm': case 'n': case 'o':
      case 'p': case 'q': case 'r': case 's': case 't':
      case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
      case 'A': case 'B': case 'C': case 'D': case 'E':
      case 'F': case 'G': case 'H': case 'I': case 'J':
      case 'K': case 'L': case 'M': case 'N': case 'O':
      case 'P': case 'Q': case 'R': case 'S': case 'T':
      case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
        // here, char is ascii, just copy it
        pDst[Ix++] = *pSrc;
        break;

      default :
        uiRequiredLen += 2;  // %XX

        // do we need to reallocate ?
        if ((uiRequiredLen + 1) > strOutEscaped.m_uiAllocSize)
        {
          pDst[Ix] = 0; // resize() method makes use of strcpy()
          strOutEscaped.releaseBuffer(Ix);
          uiRequiredLen += STRING_GROW_SIZE; // StringA already handles padded (re)allocates (see the resize() method) but be sure we do not reallocate to often
          pDst = (uchar*)strOutEscaped.acquireBuffer(uiRequiredLen + 1, true);
        }

        // convert character
        snprintf((char*)&pDst[Ix], 4, "%%%02X", *pSrc); // could be 3 on windows (so it will not append non-needed null char), but glibc requires 4
        GLB_ASSERT(strlen((char*)&pDst[Ix]) == 3);
        Ix += 3;
        break;
    }
  }

  pDst[Ix] = 0;
  strOutEscaped.releaseBuffer(Ix);
}

//---------------------------------------------------------------------------
// urlUnescape
//---------------------------------------------------------------------------
void StringA::urlUnescape (StringA& strOutUnescaped) const
{
  uint32 Ix;
  uchar* pSrc;
  uchar* pDst;
  char   szHex[4];

  pSrc = (uchar*)m_pszData;
  pDst = (uchar*)strOutUnescaped.acquireBuffer(m_uiLength + 1, false);

  for (Ix = 0; *pSrc; ++Ix)
  {
    if ((pSrc[0] == '%') && isxdigit(pSrc[1]) && isxdigit(pSrc[2]))
    {
      char* pDummy;

      szHex[0] = pSrc[1];
      szHex[1] = pSrc[2];
      szHex[2] = 0;

      pDst[Ix] = (uchar)strtol((char*)&szHex, &pDummy, 16); // we can safely force-cast, returned value will never exceed 255
      pSrc    += 3;
    }
    else
    {
      pDst[Ix] = *pSrc++;
    }
  }

  pDst[Ix] = 0;
  strOutUnescaped.releaseBuffer(Ix);
}

//---------------------------------------------------------------------------
// urlEscape
//---------------------------------------------------------------------------
void StringA::urlEscape (void)
{
  StringA strSrc(*this);
  strSrc.urlEscape(*this);
}

//---------------------------------------------------------------------------
// urlUnescape
//---------------------------------------------------------------------------
void StringA::urlUnescape (void)
{
  StringA strSrc(*this);
  strSrc.urlUnescape(*this);
}




//---------------------------------------------------------------------------
// ansiToUtf8
//---------------------------------------------------------------------------
void StringA::ansiToUtf8 (bool bComputeExactLengthFirst/*=false*/)
{
  uint    uiNewLength;
  uchar*  pSrc;
  StringA strSrc;
  uchar*  pDst;
  uchar   c;

  if (!m_pszData[0])
    return;

  // compute final length
  if (bComputeExactLengthFirst)
  {
    uiNewLength = m_uiLength;
    for (pSrc = (uchar*)m_pszData; *pSrc; ++pSrc)
    {
      if (*pSrc >= 128)
        ++uiNewLength;
    }
  }
  else
  {
    uiNewLength = m_uiLength << 1;
  }

  // copy source to swap
  strSrc = *this;

  // grow destination buffer
  this->grow(uiNewLength + 1, false);
  GLB_ASSERT(!m_pszData[0] && !m_uiLength);

  // convert
  pSrc = (uchar*)strSrc.m_pszData;
  pDst = (uchar*)m_pszData;
  while ((c = *pDst++))
  {
    if (c <= 127)
    {
      *pDst++ = c;
    }
    else
    {
      GLB_ASSERT((192 + (c / 64)) != 0);
      GLB_ASSERT((128 + (c % 64)) != 0);

      *pDst++ = 192 + (c / 64);
      *pDst++ = 128 + (c % 64);
    }
  }
  *pDst = 0;
  m_uiLength = uint((char*)pDst - m_pszData);
  GLB_ASSERT(m_uiLength == strlen(m_pszData));
}

//---------------------------------------------------------------------------
// utf8ToAnsi
//---------------------------------------------------------------------------
void StringA::utf8ToAnsi (const uchar* pSourceUtf8String)
{
  // http://www.cl.cam.ac.uk/~mgk25/unicode.html#utf-8

  uint   uiNewLength;
  uchar* pSrc;
  uchar* pDst;
  uchar  c;

  GLB_ASSERT(pSourceUtf8String && pSourceUtf8String[0]);
  if (!pSourceUtf8String)
    return;
  if (!pSourceUtf8String[0])
    return;

  // compute destination length
  pSrc        = (uchar*)pSourceUtf8String;
  uiNewLength = 0;
  while ((c = *pSrc++))
  {
    ++uiNewLength;

    if (c >= 0x80)
    {
      if ((c & 0xE0) == 0xC0)      // (c & 11100000b) == 11000000b : 2 bytes long, 11 bits of data
        ++pSrc;
      else if ((c & 0xF0) == 0xE0) // (c & 11110000b) == 11100000b : 3 bytes long, 16 bits of data
        pSrc += 2;
      else if ((c & 0xF8) == 0xF0) // (c & 11111000b) == 11110000b : 4 bytes long, 21 bits of data
        pSrc += 3;
      else if ((c & 0xFC) == 0xF8) // (c & 11111100b) == 11111000b : 5 bytes long, 26 bits of data
        pSrc += 4;
      else if ((c & 0xFE) == 0xFC) // (c & 11111110b) == 11111100b : 6 bytes long, 31 bits of data
        pSrc += 5;
    }
  }

  // grow destination buffer
  this->grow(uiNewLength + 1, false);

  // convert
  pSrc = (uchar*)pSourceUtf8String;
  pDst = (uchar*)m_pszData;
  while ((c = *pSrc++))
  {
    if (c >= 0x80)
    {
      if ((c & 0xE0) == 0xC0)      // (c & 11100000b) == 11000000b : 2 bytes long, 11 bits of data
      {
        uint16 uiWide = ((c & 0x1F) << 6) | (*pSrc++ & 0x3F);
        if (uiWide > 0xff)
          c = '?';
        else
          c = (uchar)uiWide;
      }
      else if ((c & 0xF0) == 0xE0) // (c & 11110000b) == 11100000b : 3 bytes long, 16 bits of data
      {
        c     = '?';
        pSrc += 2;
      }
      else if ((c & 0xF8) == 0xF0) // (c & 11111000b) == 11110000b : 4 bytes long, 21 bits of data
      {
        c     = '?';
        pSrc += 3;
      }
      else if ((c & 0xFC) == 0xF8) // (c & 11111100b) == 11111000b : 5 bytes long, 26 bits of data
      {
        c     = '?';
        pSrc += 4;
      }
      else if ((c & 0xFE) == 0xFC) // (c & 11111110b) == 11111100b : 6 bytes long, 31 bits of data
      {
        c     = '?';
        pSrc += 5;
      }
    }

    *pDst++ = c;
  }
  *pDst = 0;
  m_uiLength = uint(pDst - (uchar*)m_pszData);
  GLB_ASSERT(m_uiLength == (uint)strlen(m_pszData));
}




//---------------------------------------------------------------------------
// replace
//---------------------------------------------------------------------------
void StringA::replace (const char cOld, const char cNew, uint uiStartOffset/*=0*/)
{
  if (uiStartOffset >= m_uiLength)
    return;

  for (char* p = m_pszData + uiStartOffset; *p; ++p)
  {
    if (*p == cOld)
      *p = cNew;
  }
}

//---------------------------------------------------------------------------
// replaceI
//---------------------------------------------------------------------------
void StringA::replaceI (const char cOld, const char cNew, uint uiStartOffset/*=0*/)
{
  if (uiStartOffset >= m_uiLength)
    return;

  for (char* p = m_pszData + uiStartOffset; *p; ++p)
  {
    if (::toupper(*p) == ::toupper(cOld))
      *p = cNew;
  }
}

//---------------------------------------------------------------------------
// replace
//---------------------------------------------------------------------------
void StringA::replace (const char* pszOld, const char* pszNew, uint uiStartOffset/*=0*/)
{
  int  nOldLen;
  int  nNewLen;
  int  nSwaps;
  uint Ix;

  GLB_ASSERT(pszOld);
  if (!pszOld)
    return;
  GLB_ASSERT(pszNew);
  if (!pszNew)
    return;
  if (uiStartOffset >= m_uiLength)
    return;

  nOldLen = (int)strlen(pszOld);
  if (uiStartOffset > (m_uiLength - (uint)nOldLen))
    return;
  nNewLen = (int)strlen(pszNew);

  // compute the length of the future resulting string
  nSwaps = 0;
  for (Ix = uiStartOffset; Ix < m_uiLength; ++Ix)
  {
    if (StringA::strCompareCount((char*)&m_pszData[Ix], pszOld, (uint)nOldLen) == 0)
    {
      ++nSwaps;
      Ix += uint(nOldLen - 1);
    }
  }

  // do replace if needed
  if (nSwaps > 0)
  {
    StringA strOldData(*this, (m_uiLength-uiStartOffset), uiStartOffset);
    uint Jx;

    this->grow(m_uiLength + uint((nNewLen - nOldLen) * nSwaps) + 2, (uiStartOffset > 0));
    if (uiStartOffset > 0)
    {
      m_pszData[uiStartOffset] = 0;
      m_uiLength = uiStartOffset;
    }

    for (Ix = 0, Jx = uiStartOffset; Ix < strOldData.length(); ++Ix)
    {
      if (StringA::strCompareCount((char*)&strOldData[Ix], pszOld, (uint)nOldLen) == 0)
      {
        memcpy(m_pszData + Jx, pszNew, nNewLen);
        Ix += uint(nOldLen - 1);
        Jx += uint(nNewLen);
      }
      else
      {
        m_pszData[Jx++] = strOldData[Ix];
      }
    }

    m_pszData[Jx] = 0;
    GLB_ASSERT(Jx == (uint)strlen(m_pszData));
    m_uiLength = Jx;
  }
}

//---------------------------------------------------------------------------
// replaceI
//---------------------------------------------------------------------------
void StringA::replaceI (const char* pszOld, const char* pszNew, uint uiStartOffset/*=0*/)
{
  int  nOldLen;
  int  nNewLen;
  int  nSwaps;
  uint Ix;

  GLB_ASSERT(pszOld);
  if (!pszOld)
    return;
  GLB_ASSERT(pszNew);
  if (!pszNew)
    return;
  if (uiStartOffset >= m_uiLength)
    return;

  nOldLen = (int)strlen(pszOld);
  if (uiStartOffset > (m_uiLength - (uint)nOldLen))
    return;
  nNewLen = (int)strlen(pszNew);

  // compute the length of the future resulting string
  nSwaps = 0;
  for (Ix = uiStartOffset; Ix < m_uiLength; ++Ix)
  {
    if (StringA::strCompareCountI((char*)&m_pszData[Ix], pszOld, (uint)nOldLen) == 0)
    {
      ++nSwaps;
      Ix += uint(nOldLen - 1);
    }
  }

  // do replace if needed
  if (nSwaps > 0)
  {
    StringA strOldData(*this, (m_uiLength-uiStartOffset), uiStartOffset);
    uint Jx;

    this->grow(m_uiLength + uint((nNewLen - nOldLen) * nSwaps) + 2, (uiStartOffset > 0));
    if (uiStartOffset > 0)
    {
      m_pszData[uiStartOffset] = 0;
      m_uiLength = uiStartOffset;
    }

    for (Ix = 0, Jx = uiStartOffset; Ix < strOldData.length(); ++Ix)
    {
      if (StringA::strCompareCountI((char*)&strOldData[Ix], pszOld, (uint)nOldLen) == 0)
      {
        memcpy(m_pszData + Jx, pszNew, nNewLen);
        Ix += uint(nOldLen - 1);
        Jx += uint(nNewLen);
      }
      else
      {
        m_pszData[Jx++] = strOldData[Ix];
      }
    }

    m_pszData[Jx] = 0;
    GLB_ASSERT(Jx == (uint)strlen(m_pszData));
    m_uiLength = Jx;
  }
}




//---------------------------------------------------------------------------
// explode
//---------------------------------------------------------------------------
void StringA::explode (StringAVec& vecOutParts, const char* pszSeparator, uint uiMaxParts/*=0*/, bool bKeepEmptyParts/*=false*/) const
{
  int nStart = 0;
  int nPos;
  int nSeparatorLen;

  vecOutParts.clear();

  GLB_ASSERT(pszSeparator);
  if (!pszSeparator || !pszSeparator[0])
    return;

  if (m_uiLength <= 0)
    return;

  nSeparatorLen = (int)strlen(pszSeparator);

  do
  {
    nPos = this->find(pszSeparator, nStart);
    if ((nPos < 0) || ((uiMaxParts > 0) && ((uint)vecOutParts.size() == (uiMaxParts - 1))))
    {
      // copy the remaining part of the string
      vecOutParts.push_back(this->substr(nStart));
      return;
    }
    else if (nPos == nStart)
    {
      if (bKeepEmptyParts)
        vecOutParts.push_back(StringA());

      // go ahead and skip separator
      nStart = nPos + nSeparatorLen;
    }
    else
    {
      // copy up to delimiter
      vecOutParts.push_back(this->substr(nStart, nPos-nStart));
      nStart = nPos + nSeparatorLen;

      if (bKeepEmptyParts && ((uint)nStart >= m_uiLength))
        vecOutParts.push_back(StringA());
    }

    // seek up to the next wanted part of the haystack
    if (!bKeepEmptyParts)
    {
      while (StringA::strCompareCount((char*)&m_pszData[nStart], pszSeparator, (uint)nSeparatorLen) == 0)
      {
        nStart += nSeparatorLen;
        if ((uint)nStart >= m_uiLength)
          return;
      }
    }
  }
  while ((uint)nStart < m_uiLength);
}

//---------------------------------------------------------------------------
// explodeWithOneOf
//---------------------------------------------------------------------------
void StringA::explodeWithOneOf (StringAVec& vecOutParts, const char* pszSeparators, uint uiMaxParts/*=0*/, bool bKeepEmptyParts/*=false*/) const
{
  int nStart = 0;
  int nPos;
  int nSeparators;

  vecOutParts.clear();

  GLB_ASSERT(pszSeparators);
  if (!pszSeparators || !pszSeparators[0])
    return;

  if (m_uiLength <= 0)
    return;

  nSeparators = (int)strlen(pszSeparators);

  do
  {
    nPos = this->findFirstOf(pszSeparators, nStart, nSeparators);
    if ((nPos < 0) || ((uiMaxParts > 0) && ((uint)vecOutParts.size() == (uiMaxParts - 1))))
    {
      // copy the remaining part of the string
      vecOutParts.push_back(this->substr(nStart));
      return;
    }
    else if (nPos == nStart)
    {
      if (bKeepEmptyParts)
        vecOutParts.push_back(StringA());

      // go ahead and skip separator
      nStart = nPos + 1;
    }
    else
    {
      // copy up to delimiter
      vecOutParts.push_back(this->substr(nStart, nPos-nStart));
      nStart = nPos + 1;
    }

    // seek up to the next wanted part of the haystack
    if (!bKeepEmptyParts)
    {
      nStart = StringA::strFindFirstNotOf(m_pszData, pszSeparators, true, nStart, (int)m_uiLength, nSeparators);
      if (nStart < 0)
        return;
    }
  }
  while ((uint)nStart < m_uiLength);
}

//---------------------------------------------------------------------------
// split
//---------------------------------------------------------------------------
void StringA::split (const char* pszSeparator/*="\r\n"*/, uint uiChunkLen/*=76*/, uint uiStartOffset/*=0*/)
{
  uint uiSepLen;
  uint uiLen;

  GLB_ASSERT(pszSeparator && pszSeparator[0]);
  if (!pszSeparator)
    return;
  if (!pszSeparator[0])
    return;
  GLB_ASSERT(uiChunkLen >= 1);
  GLB_ASSERT(uiStartOffset >= 0);
  GLB_ASSERT(uiStartOffset < m_uiLength);

  uiSepLen = (uint)strlen(pszSeparator);

  // evaluate the grow size to gain some time while inserting separators
  uiLen = ((m_uiLength - uiStartOffset) / uiChunkLen) * uiSepLen;
  this->grow(m_uiLength + uiLen + 1, true);

  // split
  uiLen = 0;
  for (uint Ix = uiStartOffset; Ix < m_uiLength; )
  {
    if (uiLen < uiChunkLen)
    {
      ++Ix;
      ++uiLen;
    }
    else
    {
      GLB_ASSERT(uiLen == uiChunkLen);

      this->insert(pszSeparator, Ix);

      Ix += uiSepLen;
      uiLen = 0;
    }
  }
}




//---------------------------------------------------------------------------
// implode
//---------------------------------------------------------------------------
void StringA::implode (const StringAVec& vecParts, const char* pszGlue)
{
  static const char c_szEmptyGlue[] = "";

  StringAVecCIt itPart;
  StringAVecCIt itPartEnd = vecParts.end();
  uint uiGlueLen;
  uint uiFinalLen;

  if (vecParts.empty())
  {
    m_pszData[0] = 0;
    m_uiLength = 0;
    return;
  }

  GLB_ASSERT(pszGlue);
  if (!pszGlue)
    pszGlue = (char*)&c_szEmptyGlue;
  uiGlueLen = (uint)strlen(pszGlue);

  // compute final string length
  uiFinalLen = 0;
  for (itPart = vecParts.begin(); itPart != itPartEnd; ++itPart)
    uiFinalLen += (*itPart).length() + uiGlueLen;
  uiFinalLen -= uiGlueLen;

  // grow buffer
  this->grow(uiFinalLen + 1, false);

  // reset string
  if (m_uiLength > 0)
  {
    m_uiLength = 0;
    m_pszData[0] = 0;
  }
  GLB_ASSERT(m_uiLength == 0);
  GLB_ASSERT(m_pszData[0] == 0);

  // implode vector
  for (itPart = vecParts.begin() ; itPart != itPartEnd; ++itPart)
  {
    this->append(*itPart);
    if ((itPart + 1) != itPartEnd)
      this->append(pszGlue, uiGlueLen);
  }
}




//---------------------------------------------------------------------------
// pathAppend
//---------------------------------------------------------------------------
void StringA::pathAppend (const char* pszPathElement)
{
  uint uiTrailLen;

  GLB_ASSERT(pszPathElement);
  if (!pszPathElement || !pszPathElement[0])
    return;

  uiTrailLen = (uint)strlen(pszPathElement);

  if ((m_uiLength > 0) && !this->pathHasTrailingSeparator() && !StringA::charIsPathSeparator(pszPathElement[0]))
  {
    this->grow(m_uiLength + 1 + uiTrailLen + 1, true);

    m_pszData[m_uiLength++] = '/';
    memcpy(&m_pszData[m_uiLength], pszPathElement, uiTrailLen + 1);
    m_uiLength += uiTrailLen;

    GLB_ASSERT((uint)strlen(m_pszData) == m_uiLength);
  }
  else
  {
    this->grow(m_uiLength + uiTrailLen + 1, true);

    memcpy(&m_pszData[m_uiLength], pszPathElement, uiTrailLen + 1);
    m_uiLength += uiTrailLen;

    GLB_ASSERT((uint)strlen(m_pszData) == m_uiLength);
  }
}

//---------------------------------------------------------------------------
// pathExtractRoot
//---------------------------------------------------------------------------
void StringA::pathExtractRoot (StringA& strDest) const
{
  StringAVec vecPath;

  this->pathExplode(vecPath);
  if (vecPath.empty())
    strDest.clear();
  else
    strDest = vecPath[0];
}

//---------------------------------------------------------------------------
// pathExtractRoot
//---------------------------------------------------------------------------
StringA StringA::pathExtractRoot (void) const
{
  StringA strRes;
  this->pathExtractRoot(strRes);
  return strRes;
}

//---------------------------------------------------------------------------
// pathExtractDirectory
//
// I tried to be POSIX compliant (i.e.: have the same behavior than libc's
// dirname() function) :
//
// Examples :
// - '/usr/bin' returns '/usr'
// - '/usr/bin/' returns '/usr'
// - '/' returns '/'
// - '.' returns '.'
// - '..' returns '.'
// - 'c:/' returns 'c:/'
// - 'c:' returns 'c:'
//---------------------------------------------------------------------------
void StringA::pathExtractDirectory (StringA& strDest) const
{
  int Ix;

  strDest.clear();

  // handle special cases
  if (m_uiLength <= 0)
  {
    return;
  }
  else if (this->pathIsRoot())
  {
    strDest = m_pszData;
    return;
  }
  else if (m_pszData[0] == '.')
  {
    if ((m_uiLength == 1) || ((m_uiLength == 2) && (m_pszData[1] == '.')))
    {
      strDest = m_pszData;
      return;
    }
  }

  // skip trailing separator
  Ix = (int)m_uiLength - 1;
  while ((Ix > 0) && StringA::charIsPathSeparator(m_pszData[Ix]))
    --Ix;

  // seek to the next separator from the end
  for ( ; Ix > 0; --Ix)
  {
    if (StringA::charIsPathSeparator(m_pszData[Ix]))
    {
      this->left(Ix, strDest);
      return;
    }
  }
}

//---------------------------------------------------------------------------
// pathExtractDirectory
//---------------------------------------------------------------------------
StringA StringA::pathExtractDirectory (void) const
{
  StringA strRes;
  this->pathExtractDirectory(strRes);
  return strRes;
}

//---------------------------------------------------------------------------
// pathExtractName
//
// Examples :
// - '/usr/bin' returns 'bin'
// - '/usr/bin/' returns 'bin'
// - '/' returns ''
// - '///////' returns '/'
// - '.' returns ''
// - '..' returns ''
// - 'c:/' returns ''
// - 'c:' returns ''
//---------------------------------------------------------------------------
void StringA::pathExtractName (StringA& strDest) const
{
  int nStart;
  int nEnd;

  strDest.clear();

  if (m_uiLength <= 0)
    return;
  else if (this->pathIsRoot())
    return;

  // skip trailing separator
  nEnd = (int)m_uiLength - 1;
  while (StringA::charIsPathSeparator(m_pszData[nEnd]))
  {
    if (nEnd > 0)
    {
      --nEnd;
    }
    else if (nEnd == 0)
    {
      // like libc's basename(), if string is full of path separators, return a slash
      strDest = '/';
      return;
    }
  }

  if (nEnd == 0)
  {
    GLB_ASSERT(StringA::charIsPathSeparator(m_pszData[0]) == false); // this should never happen

    if (m_pszData[0] == '.')
      return;

    // here, string *is* the name
    strDest = m_pszData;
    return;
  }
  else if (nEnd == 1)
  {
    if ((m_pszData[0] == '.') && (m_pszData[1] == '.'))
      return;
  }

  for (nStart = nEnd - 1; nStart >= 0; --nStart)
  {
    if (StringA::charIsPathSeparator(m_pszData[nStart]))
    {
      GLB_ASSERT(nStart + 1 != nEnd);
      this->mid(uint(nStart + 1), uint(nEnd - nStart), strDest);
      return;
    }
  }
}

//---------------------------------------------------------------------------
// pathExtractName
//---------------------------------------------------------------------------
StringA StringA::pathExtractName (void) const
{
  StringA strRes;
  this->pathExtractName(strRes);
  return strRes;
}

//---------------------------------------------------------------------------
// pathExtractTitle
//
// Examples :
// - '/usr/bin' returns ''
// - '/usr/bin.ext' returns 'bin'
// - '/usr/bin/' returns ''
// - '/usr/bin.ext/' returns ''
// - '/' returns ''
// - '.' returns ''
// - '..' returns ''
// - 'c:/' returns ''
// - 'c:' returns ''
//---------------------------------------------------------------------------
void StringA::pathExtractTitle (StringA& strDest) const
{
  strDest.clear();

  if (m_uiLength <= 0)
    return;

  for (int Ix = (int)m_uiLength - 1; Ix > 0; --Ix)
  {
    if (StringA::charIsPathSeparator(m_pszData[Ix]))
    {
      int Jx = Ix + 1;

      while ((Jx < (int)m_uiLength) && (m_pszData[Jx] != '.'))
        ++Jx;

      this->mid(Ix+1, Jx-Ix-1, strDest);
      return;
    }
  }
}

//---------------------------------------------------------------------------
// pathExtractTitle
//---------------------------------------------------------------------------
StringA StringA::pathExtractTitle (void) const
{
  StringA strRes;
  this->pathExtractTitle(strRes);
  return strRes;
}

//---------------------------------------------------------------------------
// pathExtractExtension
//---------------------------------------------------------------------------
void StringA::pathExtractExtension (StringA& strDest) const
{
  strDest.clear();

  if (m_uiLength <= 0)
    return;

  for (int Ix = (int)m_uiLength - 1; Ix > 0; --Ix)
  {
    if (StringA::charIsPathSeparator(m_pszData[Ix]))
    {
      return;
    }
    else if (m_pszData[Ix] == '.')
    {
      this->right(m_uiLength-Ix-1, strDest);
      return;
    }
  }
}

//---------------------------------------------------------------------------
// pathExtractExtension
//---------------------------------------------------------------------------
StringA StringA::pathExtractExtension (void) const
{
  StringA strRes;
  this->pathExtractExtension(strRes);
  return strRes;
}

//---------------------------------------------------------------------------
// pathStripDirectory
//---------------------------------------------------------------------------
void StringA::pathStripDirectory (void)
{
  if (m_uiLength <= 0)
    return;

  for (int Ix = (int)m_uiLength - 1; Ix > 0; --Ix)
  {
    if (StringA::charIsPathSeparator(m_pszData[Ix]))
    {
      ++Ix;

      if ((uint)Ix >= m_uiLength)
      {
        m_pszData[0] = 0;
        m_uiLength = 0;
      }
      else
      {
        m_uiLength -= Ix;
        memmove(m_pszData, &m_pszData[Ix], m_uiLength + 1);

        GLB_ASSERT(m_pszData[m_uiLength] == 0);
        GLB_ASSERT((uint)strlen(m_pszData) == m_uiLength);
      }

      return;
    }
  }

  // here, path *is* a name
  // so there was nothing to do
}

//---------------------------------------------------------------------------
// pathStripName
//---------------------------------------------------------------------------
void StringA::pathStripName (void)
{
  if (m_uiLength <= 0)
    return;

  for (int Ix = (int)m_uiLength - 1; Ix > 0; --Ix)
  {
    if (StringA::charIsPathSeparator(m_pszData[Ix]))
    {
      m_uiLength = Ix;
      m_pszData[Ix] = 0;

      STRA_ASSERT((STRA_UINT)strlen(m_pszData) == m_uiLength);

      return;
    }
  }

  // here, string *is* a name
  m_pszData[0] = 0;
  m_uiLength = 0;
}

//---------------------------------------------------------------------------
// pathStripExtension
//---------------------------------------------------------------------------
void StringA::pathStripExtension (void)
{
  if (m_uiLength <= 0)
    return;

  for (int Ix = (int)m_uiLength - 1; Ix > 0; --Ix)
  {
    if (StringA::charIsPathSeparator(m_pszData[Ix]))
    {
      return;
    }
    else if (m_pszData[Ix] == '.')
    {
      m_pszData[Ix] = 0;
      m_uiLength = (uint)Ix;

      GLB_ASSERT(m_pszData[m_uiLength] == 0);
      GLB_ASSERT((uint)strlen(m_pszData) == m_uiLength);

      return;
    }
  }
}

//---------------------------------------------------------------------------
// pathExplode
//---------------------------------------------------------------------------
void StringA::pathExplode (StringAVec& vecPath) const
{
  char* pStart;
  char* pEnd;
  char* pszData;

  vecPath.clear();

  if (m_uiLength <= 0)
    return;

  if (this->pathIsWindowsNetwork())
  {
    // when path is like "//server/share/dir/file.ext"
    // we must return : "//server/share" "dir" "file.ext"

    StringA strTemp;
    int     nSepCount = 0;

    pStart = m_pszData + 2;
    pEnd   = pStart + 1;
    if (*pStart == 0)
      return;
    if (*pEnd == 0)
      return;

    for ( ; *pStart; ++pEnd)
    {
      if (StringA::charIsPathSeparator(*pEnd))
        ++nSepCount;
      if ((nSepCount >= 2) || (*pEnd == 0))
        break;
    }

    strTemp.copyCount(m_pszData, uint(pEnd - m_pszData));
    while (strTemp.pathStripTrailingSeparator()) { ; }
    vecPath.push_back(strTemp);

    if (*pEnd == 0)
      return;

    pszData = pEnd + 1;
  }
  else if (this->pathIsRoot())
  {
    // when path is root, do not explode it
    vecPath.push_back(m_pszData);
    return;
  }
  else if (StringA::charIsPathSeparator(m_pszData[0]))
  {
    GLB_ASSERT(StringA::charIsPathSeparator(m_pszData[1]) == false);
    vecPath.push_back(StringA(m_pszData[0]));

    pszData = m_pszData + 1;
  }
  else
  {
    pszData = m_pszData;
  }

  for (pStart = pszData, pEnd = pStart + 1; *pStart; ++pEnd)
  {
    if ((*pEnd == 0) || StringA::charIsPathSeparator(*pEnd))
    {
      StringA strTemp(pStart, uint(pEnd - pStart));

      if (!strTemp.isEmpty())
        vecPath.push_back(strTemp);

      if (*pEnd == 0)
        return;

      pStart = pEnd + 1;
    }
  }
}

//---------------------------------------------------------------------------
// pathCheckExtension
//---------------------------------------------------------------------------
bool StringA::pathCheckExtension (const char* pszExtension) const
{
  GLB_ASSERT(pszExtension);
  if (!pszExtension || !pszExtension[0])
    return false;

  if (m_uiLength <= 0)
    return false;

  for (int Ix = (int)m_uiLength - 1; Ix > 0; --Ix)
  {
    if (StringA::charIsPathSeparator(m_pszData[Ix]))
      return false;
    else if (m_pszData[Ix] == '.')
      return StringA::strCompareI((char*)&m_pszData[Ix+1], pszExtension) == 0;
  }

  return false;
}

//---------------------------------------------------------------------------
// pathChangeExtension
//---------------------------------------------------------------------------
void StringA::pathChangeExtension (const char* pszExtension)
{
  GLB_ASSERT(pszExtension);
  if (!pszExtension || !pszExtension[0])
    return;

  if (*pszExtension == '.')
    pszExtension++;

  if (m_uiLength <= 0)
    return;

  for (int Ix = (int)m_uiLength - 1; Ix > 0; --Ix)
  {
    if (StringA::charIsPathSeparator(m_pszData[Ix]))
    {
      this->append('.');
      this->append(pszExtension);

      return;
    }
    else if (m_pszData[Ix] == '.')
    {
      ++Ix;
      m_pszData[Ix] = 0;
      m_uiLength = (uint)Ix;

      this->append(pszExtension);

      return;
    }
  }
}

//---------------------------------------------------------------------------
// pathIsAbsolute
//---------------------------------------------------------------------------
bool StringA::pathIsAbsolute (void) const
{
  if (m_uiLength <= 0)
    return false;

  // validates "/" or "\\server" or "\\server\" or "\\server\share" or "\\server\share\"
  if (StringA::charIsPathSeparator(m_pszData[0]))
    return true;

  // validates "c:"
  if ((m_uiLength >= 2) && (m_pszData[1] == ':'))
  {
    if (((*m_pszData >= 'a') && (*m_pszData <= 'z')) || ((*m_pszData >= 'A') && (*m_pszData <= 'Z')))
      return true;
  }

  return false;
}

//---------------------------------------------------------------------------
// pathIsRoot
//---------------------------------------------------------------------------
bool StringA::pathIsRoot (void) const
{
  if (m_uiLength <= 0)
  {
    return false;
  }
  else if (m_uiLength == 1)
  {
    if (StringA::charIsPathSeparator(m_pszData[0]))
      return true;
  }
  else if (m_pszData[1] == ':') // m_uiLength is > 1
  {
    // validates "c:" or "c:/" or "c:\"
    if (((m_pszData[0] >= 'a') && (m_pszData[0] <= 'z')) || ((m_pszData[0] >= 'A') && (m_pszData[0] <= 'Z')))
    {
      if (m_uiLength == 2)
        return true;
      else if ((m_uiLength == 3) && (StringA::charIsPathSeparator(m_pszData[2])))
        return true;
    }
  }
  else if ((m_pszData[0] == '\\') && (m_pszData[1] == '\\')) // m_uiLength is > 1
  {
    StringAVec vecParts;

    // validates "\\server" or "\\server\" or "\\server\share" or "\\server\share\"
    this->explodeWithOneOf(vecParts, "/\\");
    if (vecParts.size() <= 2)
      return true;
  }

  return false;
}

//---------------------------------------------------------------------------
// pathExpand
//---------------------------------------------------------------------------
bool StringA::pathExpand (void)
{
  if (!this->pathIsAbsolute())
  {
    GLB_ASSERT(0 && "Path must be absolute ! pathExpand() is only here to resolve symlinks and expand '.' and '..' sequences.");
    return false;
  }

  #ifdef GLB_PLATFORM_WINDOWS
  {
    char  szBuffer[1024];
    DWORD dwLen;

    dwLen = ::GetFullPathNameA(m_pszData, sizeof(szBuffer), (char*)&szBuffer, 0);
    if (dwLen == 0)
    {
      __Error_GetFullPathName :
      GLB_LOGERR("Error while calling GetFullPathName(\"%s\") ! Error %u : %s", m_pszData, System::lastError(), System::lastErrorString());
      return false;
    }
    else if (dwLen >= sizeof(szBuffer))
    {
      // swap buffers' content
      if (sizeof(szBuffer) <= m_uiLength)
        return false;
      memcpy((char*)&szBuffer, m_pszData, m_uiLength + 1);

      this->grow(dwLen + 1, false);

      dwLen = ::GetFullPathNameA((char*)&szBuffer, m_uiAllocSize, m_pszData, 0);
      if (dwLen == 0)
      {
        GLB_ASSERT(0 && "Weird !");
        goto __Error_GetFullPathName;
      }

      GLB_ASSERT(dwLen < m_uiAllocSize);
      m_uiLength = dwLen;
      GLB_ASSERT(m_uiLength == (uint)strlen(m_pszData));
    }
    else
    {
      this->grow(dwLen + 1, false);

      memcpy(m_pszData, (char*)&szBuffer, dwLen + 1);
      m_uiLength = dwLen;
      GLB_ASSERT(m_uiLength == (uint)strlen(m_pszData));
    }
  }
  #else
  {
    char szBuffer[PATH_MAX];
    uint uiLen;

    if (!realpath(m_pszData, (char*)&szBuffer))
    {
      GLB_LOGERR("Error while trying to expand given path \"%s\" ! Error %u : %s", m_pszData, System::lastError(), System::lastErrorString());
      return false;
    }

    // since realpath() is buggy and *not* secure, and if by chance there
    // was not already a buffer overflow due to the realpath(), be sure there
    // is a nul-terminating character *inside* buffer.
    // if we find it, we are sure there was no problem, otherwise, be sure
    // the caller is advised about corrupted memory. this is the best we can
    // do here...
    // maybe there is an alternative to realpath() here but i don't know it,
    // please let me know :)  - jcl
    for (uiLen = 0; uiLen < sizeof(szBuffer); ++uiLen)
    {
      if (!szBuffer[uiLen])
        break;  // ok, we've found one inside buffer !
    }
    if (uiLen >= sizeof(szBuffer))
    {
      GLB_LOGERR("Error while trying to expand given path \"%s\" ! Your memory may be corrupted by a call to realpath().", m_pszData);
      return false;
    }

    this->grow(uiLen + 1, false);

    memcpy(m_pszData, (char*)&szBuffer, uiLen + 1);
    m_uiLength = uiLen;
  }
  #endif

  return true;
}

//---------------------------------------------------------------------------
// pathCompare
//---------------------------------------------------------------------------
int StringA::pathCompare (const char* pszPath, bool bCaseSensitive) const
{
  GLB_ASSERT(pszPath);
  return StringA::strComparePath(m_pszData, pszPath, bCaseSensitive);
}

//---------------------------------------------------------------------------
// pathIsInDirectory
//---------------------------------------------------------------------------
bool StringA::pathIsInDirectory (const char* pszDirPath, bool bCaseSensitive) const
{
  StringA    strParent;
  StringA    strChild;
  StringAVec vecParent;
  StringAVec vecChild;

  GLB_ASSERT(pszDirPath);

  strParent = pszDirPath;
  strChild  = *this;

  if (!strParent.pathExpand())
    return false;
  if (!strChild.pathExpand())
    return false;

  //GLB_ASSERT(FileSystem::isDirectory(strParent));

  strParent.pathExplode(vecParent);
  strChild.pathExplode(vecChild);

  // since we are sure both path were expanded correctly (even symlinks), if
  // parent path is longer than child path, it is obvious that path is not in
  // the given directory.
  if (vecParent.size() >= vecChild.size())
    return false;

  // compare beginning elements
  if (bCaseSensitive)
  {
    for (size_t Ix = 0; Ix < vecParent.size(); ++Ix)
    {
      if (vecParent[Ix].compare(vecChild[Ix]) != 0)
        return false;
    }
  }
  else
  {
    for (size_t Ix = 0; Ix < vecParent.size(); ++Ix)
    {
      if (vecParent[Ix].compareI(vecChild[Ix]) != 0)
        return false;
    }
  }

  return true;
}



//---------------------------------------------------------------------------
// StrCompare
//---------------------------------------------------------------------------
int StringA::strCompare (const char* psz1, const char* psz2)
{
  int c1, c2, d;

  do
  {
    c1 = *psz1++;
    c2 = *psz2++;

    d = c1 - c2;
    if (d)
      return (INTSIGNBITNOTSET(d) << 1) - 1;
  }
  while (c1);

  return 0;
}

//---------------------------------------------------------------------------
// StrCompareCount
//---------------------------------------------------------------------------
int StringA::strCompareCount (const char* psz1, const char* psz2, uint uiMaxCount)
{
  int c1, c2, d;

  do
  {
    if (!uiMaxCount--)
      return 0;

    c1 = *psz1++;
    c2 = *psz2++;

    d = c1 - c2;
    if (d)
      return (INTSIGNBITNOTSET(d) << 1) - 1;
  }
  while (c1);

  return 0;
}

//---------------------------------------------------------------------------
// StrCompareI
//---------------------------------------------------------------------------
int StringA::strCompareI (const char* psz1, const char* psz2)
{
  int c1, c2, d;

  do
  {
    c1 = *psz1++;
    c2 = *psz2++;

    d = c1 - c2;
    while (d)
    {
      if ((c1 >= 'A') && (c1 <= 'Z'))
      {
        d += ('a' - 'A');
        if (!d)
          break;
      }

      if ((c2 >= 'A') && (c2 <= 'Z'))
      {
        d -= ('a' - 'A');
        if (!d)
          break;
      }

      return (INTSIGNBITNOTSET(d) << 1) - 1;
    }
  }
  while (c1);

  return 0;
}

//---------------------------------------------------------------------------
// StrCompareCountI
//---------------------------------------------------------------------------
int StringA::strCompareCountI (const char* psz1, const char* psz2, uint uiMaxCount)
{
  int c1, c2, d;

  do
  {
    if (!uiMaxCount--)
      return 0;

    c1 = *psz1++;
    c2 = *psz2++;

    d = c1 - c2;
    while (d)
    {
      if ((c1 >= 'A') && (c1 <= 'Z'))
      {
        d += ('a' - 'A');
        if (!d)
          break;
      }

      if ((c2 >= 'A') && (c2 <= 'Z'))
      {
        d -= ('a' - 'A');
        if (!d)
          break;
      }

      return (INTSIGNBITNOTSET(d) << 1) - 1;
    }
  }
  while (c1);

  return 0;
}

//---------------------------------------------------------------------------
// StrComparePath
//---------------------------------------------------------------------------
int StringA::strComparePath (const char* pszPath1, const char* pszPath2, bool bCaseSensitive)
{
  GLB_ASSERT(pszPath1);
  GLB_ASSERT(pszPath2);

  StringA str1(pszPath1);
  StringA str2(pszPath2);

  str1.pathBackSlashesToSlashes();
  str2.pathBackSlashesToSlashes();

  if (bCaseSensitive)
    return StringA::strCompare(str1.m_pszData, str2.m_pszData);
  else
    return StringA::strCompareI(str1.m_pszData, str2.m_pszData);
}



//---------------------------------------------------------------------------
// StrIsBool
//---------------------------------------------------------------------------
bool StringA::strIsBool (const char* psz, bool bStrict/*=false*/, int* pnOutValue/*=NULL*/)
{
  // accepted boolean strings (case insensitive) :
  // 0, 1, false, true, no, yes, off, on

  char* p;
  bool  bValue;

  GLB_ASSERT(psz);
  p = (char*)psz;
  if (!*p)
    return false;

  // skip whitespaces
  if (!bStrict)
  {
    while ((*p == ' ') || (*p == '\t'))
      ++p;
  }

  // null string
  if (!*p)
  {
    goto __Invalid;
  }

  // 0
  else if (p[0] == '0')
  {
    bValue = false;
    ++p;
  }

  // 1
  else if (p[0] == '1')
  {
    bValue = true;
    ++p;
  }

  // false
  else if (p[0]=='f' || p[0]=='F')
  {
    if ((p[1]=='a'||p[1]=='A') && (p[2]=='l'||p[2]=='L') && (p[3]=='s'||p[3]=='S') && (p[4]=='e'||p[4]=='E'))
    {
      bValue = false;
      p += 5;
    }
    else
    {
      goto __Invalid;
    }
  }

  // no
  else if (p[0]=='n' || p[0]=='N')
  {
    if (p[1]=='o' || p[1]=='O')
    {
      bValue = false;
      p += 2;
    }
    else
    {
      goto __Invalid;
    }
  }

  // on, off
  else if (p[0]=='o' || p[0]=='O')
  {
    if (p[1]=='n' || p[1]=='N')
    {
      bValue = true;
      p += 2;
    }
    else if ((p[1]=='f' || p[1]=='F') && (p[2]=='f' || p[2]=='F'))
    {
      bValue = false;
      p += 3;
    }
    else
    {
      goto __Invalid;
    }
  }

  // true
  else if (p[0]=='t' || p[0]=='T')
  {
    if ((p[1]=='r' || p[1]=='R') && (p[2]=='u' || p[2]=='U') && (p[3]=='e' || p[3]=='E'))
    {
      bValue = true;
      p += 4;
    }
    else
    {
      goto __Invalid;
    }
  }

  // yes
  else if (p[0]=='y' || p[0]=='Y')
  {
    if ((p[1]=='e' || p[1]=='E') && (p[2]=='s' || p[2]=='S'))
    {
      bValue = true;
      p += 3;
    }
    else
    {
      goto __Invalid;
    }
  }

  // otherwise...
  else
  {
    goto __Invalid;
  }


  // boolean string must be followed by whitespaces or terminate
  if ( (bStrict && !*p)  ||  (!bStrict && (!*p || StringA::charIsSpace(*p))) )
  {
    // send back boolean value if needed
    if (pnOutValue)
      *pnOutValue = bValue ? 1 : 0;

    return true;
  }


__Invalid :
  if (pnOutValue)
    *pnOutValue = -1;
  return false;
}

//---------------------------------------------------------------------------
// StrIsInt
//---------------------------------------------------------------------------
bool StringA::strIsInt (const char* psz, bool bStrict/*=false*/)
{
  // format of an integer string : [whitespace] [sign] [digits] [whitespace|nl|\0]

  char* p;

  GLB_ASSERT(psz);
  p = (char*)psz;
  if (!*p)
    return false;

  // as this is done by atoi(), skip whitespaces first
  if (!bStrict)
  {
    while ((*p == ' ') || (*p == '\t'))
      ++p;
  }

  // handle first character which is a special case because of the eventual +/- sign
  if ((*p == '-') || (*p == '+'))
    ++p;

  // at least one or several digits
  if ((*p < '0') || (*p > '9'))
    return false;
  while ((*p >= '0') && (*p <= '9'))
    ++p;

  // [whitespace|\0]
  if ( (bStrict && !*p)  ||  (!bStrict && (!*p || StringA::charIsSpace(*p))) )
    return true;

  return false;
}

//---------------------------------------------------------------------------
// StrIsFloat
//---------------------------------------------------------------------------
bool StringA::strIsFloat (const char* psz, bool bStrict/*=false*/)
{
  // format of a floating point string :
  // [whitespace] [sign] [digits] [.digits] [ {d | D | e | E }[sign]digits] {whitespace|nl|\0}
  // "if no digits appear before the decimal point, at least one must appear after the decimal point."

  char* p;
  bool  bMantissaNeeded;

  GLB_ASSERT(psz);
  p = (char*)psz;
  if (!*p)
    return false;

  // [whitespace]
  // as this is done by atof(), skip whitespaces first
  if (!bStrict)
  {
    while ((*p == ' ') || (*p == '\t'))
      ++p;
  }

  // [sign]
  if ((*p == '-') || (*p == '+'))
    ++p;

  // [digits]
  if (*p == '.')
  {
    ++p;
    bMantissaNeeded = true;
    goto __Mantissa;
  }
  if ((*p < '0') || (*p > '9'))
    return false;
  bMantissaNeeded = false;
  while ((*p >= '0') && (*p <= '9'))
    ++p;
  if ( (bStrict && !*p)  ||  (!bStrict && (!*p || StringA::charIsSpace(*p))) )
    return true;

  // [.digits] - mantissa (required only if there was no main part)
  __Mantissa :
  if (bMantissaNeeded)
  {
    if ((*p < '0') || (*p > '9'))
      return false;
    ++p;
    while ((*p >= '0') && (*p <= '9'))
      ++p;
    if ( (bStrict && !*p)  ||  (!bStrict && (!*p || StringA::charIsSpace(*p))) )
      return true;
  }
  else if (*p == '.')
  {
    ++p;
    // digits are not required after a decimal point here
    while ((*p >= '0') && (*p <= '9'))
      ++p;
    if ( (bStrict && !*p)  ||  (!bStrict && (!*p || StringA::charIsSpace(*p))) )
      return true;
  }

  // [exponent]
  if ((*p != 'd') && (*p != 'D') && (*p != 'e') && (*p != 'E'))
    return false;
  ++p;
  if ((*p == '-') || (*p == '+'))
    ++p;
  if ((*p < '0') || (*p > '9'))
    return false;
  ++p;
  while ((*p >= '0') && (*p <= '9'))
    ++p;

  // [whitespace|\0]
  if ( (bStrict && !*p)  ||  (!bStrict && (!*p || StringA::charIsSpace(*p))) )
    return true;

  return false;
}

//---------------------------------------------------------------------------
// StrIsFullOfDigits
//---------------------------------------------------------------------------
bool StringA::strIsFullOfDigits (const char* psz)
{
  char* p;

  GLB_ASSERT(psz);
  p = (char*)psz;

  // there must be at least one digit
  if ((*p < '0') || (*p > '9'))
    return false;
  ++p;

  // skip other digits
  while ((*p >= '0') && (*p <= '9'))
    ++p;

  // after digits, null character is required
  if (!*p)
    return true;

  return false;
}




//---------------------------------------------------------------------------
// StrFindFirstOf
//---------------------------------------------------------------------------
int StringA::strFindFirstOf (const char* pszHaystack, const char cNeedle, bool bCaseSensitive, int nStartOffset/*=0*/, int nHaystackLength/*=-1*/)
{
  int nPos;
  int nEndOffset;

  if (!pszHaystack || !pszHaystack[0])
    return -1;

  if (nHaystackLength < 0)
    nHaystackLength = (int)strlen(pszHaystack);
  GLB_ASSERT(nHaystackLength <= (int)strlen(pszHaystack));

  if ((nStartOffset < 0) || (nStartOffset >= nHaystackLength))
    return -1;

  nEndOffset = nHaystackLength - 1;
  if (nEndOffset < nStartOffset)
    return -1;

  for (nPos = nStartOffset; nPos <= nEndOffset; ++nPos)
  {
    if (bCaseSensitive)
    {
      if (pszHaystack[nPos] == cNeedle)
        return nPos;
    }
    else
    {
      if (::toupper(pszHaystack[nPos]) == ::toupper(cNeedle))
        return nPos;
    }
  }

  return -1;
}

//---------------------------------------------------------------------------
// StrFindFirstOf
//---------------------------------------------------------------------------
int StringA::strFindFirstOf (const char* pszHaystack, const char* pszNeedles, bool bCaseSensitive, int nStartOffset/*=0*/, int nHaystackLength/*=-1*/, int nNeedlesLength/*=-1*/)
{
  int nPos;
  int nEndOffset;
  int Ix;

  if (!pszHaystack || !pszHaystack[0])
    return -1;
  if (!pszNeedles || !pszNeedles[0])
    return -1;

  if (nHaystackLength < 0)
    nHaystackLength = (int)strlen(pszHaystack);
  GLB_ASSERT(nHaystackLength <= (int)strlen(pszHaystack));

  if ((nStartOffset < 0) || (nStartOffset >= nHaystackLength))
    return -1;

  nEndOffset = nHaystackLength - 1;
  if (nEndOffset < nStartOffset)
    return -1;

  if (nNeedlesLength < 0)
    nNeedlesLength = (int)strlen(pszNeedles);

  for (nPos = nStartOffset; nPos <= nEndOffset; ++nPos)
  {
    for (Ix = 0; Ix < nNeedlesLength; ++Ix)
    {
      if (bCaseSensitive)
      {
        if (pszHaystack[nPos] == pszNeedles[Ix])
          return nPos;
      }
      else
      {
        if (::toupper(pszHaystack[nPos]) == ::toupper(pszNeedles[Ix]))
          return nPos;
      }
    }
  }

  return -1;
}

//---------------------------------------------------------------------------
// StrFindFirstNotOf
//---------------------------------------------------------------------------
int StringA::strFindFirstNotOf (const char* pszHaystack, const char cNeedless, bool bCaseSensitive, int nStartOffset/*=0*/, int nHaystackLength/*=-1*/)
{
  int nPos;
  int nEndOffset;

  if (!pszHaystack || !pszHaystack[0])
    return -1;

  if (nHaystackLength < 0)
    nHaystackLength = (int)strlen(pszHaystack);
  GLB_ASSERT(nHaystackLength <= (int)strlen(pszHaystack));

  if ((nStartOffset < 0) || (nStartOffset >= nHaystackLength))
    return -1;

  nEndOffset = nHaystackLength - 1;
  if (nEndOffset < nStartOffset)
    return -1;

  for (nPos = nStartOffset; nPos <= nEndOffset; ++nPos)
  {
    if (bCaseSensitive)
    {
      if (pszHaystack[nPos] != cNeedless)
        return nPos;
    }
    else
    {
      if (::toupper(pszHaystack[nPos]) != ::toupper(cNeedless))
        return nPos;
    }
  }

  return -1;
}

//---------------------------------------------------------------------------
// StrFindFirstNotOf
//---------------------------------------------------------------------------
int StringA::strFindFirstNotOf (const char* pszHaystack, const char* pszNeedless, bool bCaseSensitive, int nStartOffset/*=0*/, int nHaystackLength/*=-1*/, int nNeedlessLength/*=-1*/)
{
  int  nPos;
  int  nEndOffset;
  int  Ix;
  bool bFound;

  if (!pszHaystack || !pszHaystack[0])
    return -1;
  if (!pszNeedless || !pszNeedless[0])
    return -1;

  if (nHaystackLength < 0)
    nHaystackLength = (int)strlen(pszHaystack);
  GLB_ASSERT(nHaystackLength <= (int)strlen(pszHaystack));

  if ((nStartOffset < 0) || (nStartOffset >= nHaystackLength))
    return -1;

  nEndOffset = nHaystackLength - 1;
  if (nEndOffset < nStartOffset)
    return -1;

  if (nNeedlessLength < 0)
    nNeedlessLength = (int)strlen(pszNeedless);

  for (nPos = nStartOffset; nPos <= nEndOffset; ++nPos)
  {
    bFound = false;
    for (Ix = 0; Ix < nNeedlessLength; ++Ix)
    {
      if (bCaseSensitive)
      {
        if (pszHaystack[nPos] == pszNeedless[Ix])
        {
          bFound = true;
          break;
        }
      }
      else
      {
        if (::toupper(pszHaystack[nPos]) == ::toupper(pszNeedless[Ix]))
        {
          bFound = true;
          break;
        }
      }
    }

    if (!bFound)
      return nPos;
  }

  return -1;
}

//---------------------------------------------------------------------------
// StrFindLastOf
//---------------------------------------------------------------------------
int StringA::strFindLastOf (const char* pszHaystack, const char cNeedle, bool bCaseSensitive, int nStartOffset/*=-1*/, int nHaystackLength/*=-1*/)
{
  int nPos;
  int nEndOffset = 0;

  if (!pszHaystack || !pszHaystack[0])
    return -1;

  if (nHaystackLength < 0)
    nHaystackLength = (int)strlen(pszHaystack);
  GLB_ASSERT(nHaystackLength <= (int)strlen(pszHaystack));

  if (nStartOffset == -1)
    nStartOffset = nHaystackLength - 1;
  else if ((nStartOffset < 0) || (nStartOffset >= nHaystackLength))
    return -1;

  GLB_ASSERT(nEndOffset <= nStartOffset);

  for (nPos = nStartOffset; nPos >= nEndOffset; --nPos)
  {
    if (bCaseSensitive)
    {
      if (pszHaystack[nPos] == cNeedle)
        return nPos;
    }
    else
    {
      if (::toupper(pszHaystack[nPos]) == ::toupper(cNeedle))
        return nPos;
    }
  }

  return -1;
}

//---------------------------------------------------------------------------
// StrFindLastOf
//---------------------------------------------------------------------------
int StringA::strFindLastOf (const char* pszHaystack, const char* pszNeedles, bool bCaseSensitive, int nStartOffset/*=-1*/, int nHaystackLength/*=-1*/, int nNeedlesLength/*=-1*/)
{
  int nPos;
  int nEndOffset = 0;
  int Ix;

  if (!pszHaystack || !pszHaystack[0])
    return -1;
  if (!pszNeedles || !pszNeedles[0])
    return -1;

  if (nHaystackLength < 0)
    nHaystackLength = (int)strlen(pszHaystack);
  GLB_ASSERT(nHaystackLength <= (int)strlen(pszHaystack));

  if (nStartOffset == -1)
    nStartOffset = nHaystackLength - 1;
  else if ((nStartOffset < 0) || (nStartOffset >= nHaystackLength))
    return -1;

  GLB_ASSERT(nEndOffset <= nStartOffset);

  if (nNeedlesLength < 0)
    nNeedlesLength = (int)strlen(pszNeedles);

  for (nPos = nStartOffset; nPos >= nEndOffset; --nPos)
  {
    for (Ix = 0; Ix < nNeedlesLength; ++Ix)
    {
      if (bCaseSensitive)
      {
        if (pszHaystack[nPos] == pszNeedles[Ix])
          return nPos;
      }
      else
      {
        if (::toupper(pszHaystack[nPos]) == ::toupper(pszNeedles[Ix]))
          return nPos;
      }
    }
  }

  return -1;
}

//---------------------------------------------------------------------------
// StrFindLastNotOf
//---------------------------------------------------------------------------
int StringA::strFindLastNotOf (const char* pszHaystack, const char cNeedless, bool bCaseSensitive, int nStartOffset/*=-1*/, int nHaystackLength/*=-1*/)
{
  int nPos;
  int nEndOffset = 0;

  if (!pszHaystack || !pszHaystack[0])
    return -1;

  if (nHaystackLength < 0)
    nHaystackLength = (int)strlen(pszHaystack);
  GLB_ASSERT(nHaystackLength <= (int)strlen(pszHaystack));

  if (nStartOffset == -1)
    nStartOffset = nHaystackLength - 1;
  else if ((nStartOffset < 0) || (nStartOffset >= nHaystackLength))
    return -1;

  GLB_ASSERT(nEndOffset <= nStartOffset);

  for (nPos = nStartOffset; nPos >= nEndOffset; --nPos)
  {
    if (bCaseSensitive)
    {
      if (pszHaystack[nPos] != cNeedless)
        return nPos;
    }
    else
    {
      if (::toupper(pszHaystack[nPos]) != ::toupper(cNeedless))
        return nPos;
    }
  }

  return -1;
}

//---------------------------------------------------------------------------
// StrFindLastNotOf
//---------------------------------------------------------------------------
int StringA::strFindLastNotOf (const char* pszHaystack, const char* pszNeedless, bool bCaseSensitive, int nStartOffset/*=-1*/, int nHaystackLength/*=-1*/, int nNeedlessLength/*=-1*/)
{
  int  nPos;
  int  nEndOffset = 0;
  int  Ix;
  bool bFound;

  if (!pszHaystack || !pszHaystack[0])
    return -1;
  if (!pszNeedless || !pszNeedless[0])
    return -1;

  if (nHaystackLength < 0)
    nHaystackLength = (int)strlen(pszHaystack);
  GLB_ASSERT(nHaystackLength <= (int)strlen(pszHaystack));

  if (nStartOffset == -1)
    nStartOffset = nHaystackLength - 1;
  else if ((nStartOffset < 0) || (nStartOffset >= nHaystackLength))
    return -1;

  GLB_ASSERT(nEndOffset <= nStartOffset);

  if (nNeedlessLength < 0)
    nNeedlessLength = (int)strlen(pszNeedless);

  for (nPos = nStartOffset; nPos >= nEndOffset; --nPos)
  {
    bFound = false;
    for (Ix = 0; Ix < nNeedlessLength; ++Ix)
    {
      if (bCaseSensitive)
      {
        if (pszHaystack[nPos] == pszNeedless[Ix])
        {
          bFound = true;
          break;
        }
      }
      else
      {
        if (::toupper(pszHaystack[nPos]) == ::toupper(pszNeedless[Ix]))
        {
          bFound = true;
          break;
        }
      }
    }

    if (!bFound)
      return nPos;
  }

  return -1;
}




//---------------------------------------------------------------------------
// StrFind
//---------------------------------------------------------------------------
int StringA::strFind (const char* pszHaystack, const char cNeedle, bool bCaseSensitive, int nStartOffset/*=0*/, int nEndOffset/*=-1*/, int nHaystackLength/*=-1*/)
{
  int nPos;

  if (!pszHaystack || !pszHaystack[0])
    return -1;

  if (nHaystackLength < 0)
    nHaystackLength = (int)strlen(pszHaystack);
  GLB_ASSERT(nHaystackLength <= (int)strlen(pszHaystack));

  if ((nStartOffset < 0) || (nStartOffset >= nHaystackLength))
    return -1;

  if ((nEndOffset < 0) || (nEndOffset >= nHaystackLength))
    nEndOffset = nHaystackLength - 1;
  else if (nEndOffset < nStartOffset)
    return -1;

  for (nPos = nStartOffset; nPos <= nEndOffset; ++nPos)
  {
    if (bCaseSensitive)
    {
      if (pszHaystack[nPos] == cNeedle)
        return nPos;
    }
    else
    {
      if (::toupper(pszHaystack[nPos]) == ::toupper(cNeedle))
        return nPos;
    }
  }

  return -1;
}

//---------------------------------------------------------------------------
// StrFind
//---------------------------------------------------------------------------
int StringA::strFind (const char* pszHaystack, const char* pszNeedle, bool bCaseSensitive, int nStartOffset/*=0*/, int nEndOffset/*=-1*/, int nHaystackLength/*=-1*/)
{
  int nPos;
  int Ix;

  if (!pszHaystack || !pszHaystack[0])
    return -1;
  if (!pszNeedle || !pszNeedle[0])
    return -1;

  if (nHaystackLength < 0)
    nHaystackLength = (int)strlen(pszHaystack);
  GLB_ASSERT(nHaystackLength <= (int)strlen(pszHaystack));

  if ((nStartOffset < 0) || (nStartOffset >= nHaystackLength))
  {
    GLB_ASSERT(0);
    return -1;
  }

  if ((nEndOffset < 0) || (nEndOffset >= nHaystackLength))
    nEndOffset = nHaystackLength - 1;
  else if (nEndOffset < nStartOffset)
    return -1;

  for (nPos = nStartOffset; nPos <= nEndOffset; ++nPos)
  {
    if (bCaseSensitive)
    {
      for (Ix = 0; pszNeedle[Ix]; ++Ix)
      {
        if (pszHaystack[nPos+Ix] != pszNeedle[Ix])
          break;
      }
    }
    else
    {
      for (Ix = 0; pszNeedle[Ix]; ++Ix)
      {
        if (::toupper(pszHaystack[nPos+Ix]) != ::toupper(pszNeedle[Ix]))
          break;
      }
    }

    if (!pszNeedle[Ix])
      return nPos;
  }

  return -1;
}




//---------------------------------------------------------------------------
// StrMatch
//---------------------------------------------------------------------------
bool StringA::strMatch (const char* pszHaystack, const char* pszPattern, bool bCaseSensitive)
{
  StringA strTemp;
  int     Ix;
  int     nIndex;
  bool    bFound;

  while (*pszPattern)
  {
    if (*pszPattern == '*')
    {
      pszPattern++;
      strTemp.clear();

      if ((*pszPattern == '[') && (*(pszPattern+1) != '['))
      {
        GLB_LOGWARN("This kind of pattern \"*[...]\" will not work properly !");
        GLB_ASSERT(0);
      }

      for (Ix = 0; *pszPattern; Ix++)
      {
        if ((*pszPattern == '*') || (*pszPattern == '?') || ( (*pszPattern == '[') && (*(pszPattern+1) != '[') ) )
          break;

        strTemp += *pszPattern;
        if (*pszPattern == '[')
          pszPattern++;
        pszPattern++;
      }

      if (strTemp.length())
      {
        nIndex = StringA::strFind(pszHaystack, strTemp.c_str(), bCaseSensitive);
        if (nIndex < 0)
          return false;

        pszHaystack += (uint)nIndex + strTemp.length();
      }
    }
    else if (*pszPattern == '?')
    {
      pszPattern++;
      pszHaystack++;

      if (!*pszPattern && *pszHaystack)
        return false;
    }
    else if (*pszPattern == '[')
    {
      if (*(pszPattern+1) == '[')
      {
        if (*pszHaystack != '[')
          return false;

        pszPattern += 2;
        pszHaystack++;
      }
      else
      {
        pszPattern++;
        bFound = false;

        while (*pszPattern && !bFound)
        {
          if ((*pszPattern == ']') && (*(pszPattern+1) != ']'))
            break;

          if ((*(pszPattern+1) == '-') && *(pszPattern+2) && ( (*(pszPattern+2) != ']') || (*(pszPattern+3) == ']') ) )
          {
            if (bCaseSensitive)
            {
              if ((*pszHaystack >= *pszPattern) && (*pszHaystack <= *(pszPattern+2)))
                bFound = true;
            }
            else
            {
              if ((::toupper(*pszHaystack) >= ::toupper(*pszPattern)) && (::toupper(*pszHaystack) <= ::toupper(*(pszPattern+2))))
                bFound = true;
            }

            pszPattern += 3;
          }
          else
          {
            if (bCaseSensitive)
            {
              if (*pszPattern == *pszHaystack)
                bFound = true;
            }
            else
            {
              if (::toupper(*pszPattern) == ::toupper(*pszHaystack))
                bFound = true;
            }

            pszPattern++;
          }
        }

        if (!bFound)
          return false;

        while (*pszPattern)
        {
          if ((*pszPattern == ']') && (*(pszPattern+1) != ']'))
            break;

          pszPattern++;
        }

        pszPattern++;
        pszHaystack++;
      }
    }
    else
    {
      if (bCaseSensitive)
      {
        if (*pszPattern != *pszHaystack)
          return false;
      }
      else
      {
        if (::toupper(*pszPattern) != ::toupper(*pszHaystack))
          return false;
      }

      pszPattern++;
      pszHaystack++;
    }
  }

  return true;
}


} // namespace glb
