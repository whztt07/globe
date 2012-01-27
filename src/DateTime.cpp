//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-14 18:16:37
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
DateTime& DateTime::now (void)
{
  m_Unix = ::time(NULL);
  return *this;
}

//---------------------------------------------------------------------------
void DateTime::set (time_t unix)
{
  m_Unix = unix;
}

//---------------------------------------------------------------------------
bool DateTime::set (tm* pTm)
{
  GLB_ASSERT(pTm);
  m_Unix = mktime(pTm);
  return this->isValid();
}

//---------------------------------------------------------------------------
bool DateTime::set (bool bUtc, int iYear, int iMonth, int iDay, int iHour/*=0*/, int iMin/*=0*/, int iSec/*=0*/, int iDST/*=-1*/)
{
  tm tm_;

  GLB_ASSERT(iYear >= 1900);
  GLB_ASSERT(iMonth >= 1 && iMonth <= 12);
  GLB_ASSERT(iDay >= 1 && iDay <= 31);
  GLB_ASSERT(iHour >= 0 && iHour <= 23);
  GLB_ASSERT(iMin >= 0 && iMin <= 59);
  GLB_ASSERT(iSec >= 0 && iSec <= 59);

  tm_.tm_sec   = iSec;
  tm_.tm_min   = iMin;
  tm_.tm_hour  = iHour;
  tm_.tm_mday  = iDay;
  tm_.tm_mon   = iMonth - 1;
  tm_.tm_year  = iYear - 1900;
  tm_.tm_wday  = 0;    // ignored by mktime()
  tm_.tm_yday  = 0;    // ignored by mktime()
  tm_.tm_isdst = iDST;

  m_Unix = bUtc ? DateTime::timegm(&tm_) : mktime(&tm_);
  GLB_ASSERT(this->isValid());

  return this->isValid();
}

//---------------------------------------------------------------------------
bool DateTime::setIso8601 (const char* pszIso8601)
{
  // for now, we only handle the very simple UTC case :
  // "2011-01-01T12:34:56Z"

  int iRes, iYear, iMonth, iDay, iHour, iMin, iSec;

  GLB_ASSERT(pszIso8601);
  if (strlen(pszIso8601) != 20)
  {
    GLB_ASSERT(0);
    this->invalidate();
    return false;
  }

  iRes = sscanf(pszIso8601, "%4u-%2u-%2uT%2u:%2u:%2uZ",
    &iYear, &iMonth, &iDay, &iHour, &iMin, &iSec);
  if (iRes != 6)
  {
    GLB_ASSERT(0);
    this->invalidate();
    return false;
  }

  return this->set(true, iYear, iMonth, iDay, iHour, iMin, iSec);
}

//---------------------------------------------------------------------------
time_t DateTime::getDiff (const DateTime& rhs, bool bAbsolute) const
{
  if (!this->isValid() || !rhs.isValid())
  {
    GLB_ASSERT(0);
    return 0;
  }

  if (bAbsolute)
  {
    return
      (m_Unix >= rhs.m_Unix) ?
      (m_Unix - rhs.m_Unix) :
      (rhs.m_Unix - m_Unix);
  }
  else
  {
    return m_Unix - rhs.m_Unix;
  }
}

//---------------------------------------------------------------------------
const tm* DateTime::getTm (bool bUtc) const
{
  if (!this->isValid())
  {
    GLB_ASSERT(0);
    return NULL;
  }

  return bUtc ? gmtime(&m_Unix) : localtime(&m_Unix);
}

//---------------------------------------------------------------------------
StringA DateTime::format (bool bUtc, const char* pszFmt) const
{
  if (!this->isValid())
  {
    GLB_ASSERT(0);
    return StringA();
  }

  const tm* pTm = this->getTm(bUtc);
  const int iSizeStep = 64;
  const int iSizeMax  = 1024; // avoid infinite loop
  StringA   strRes;

  // TODO FIXME : strftime() may also return 0 in some localizations when
  // only "%p" is used for example !

  for (int iSize = iSizeStep; 1 ; iSize += iSizeStep)
  {
    char* pszBuffer = strRes.acquireBuffer(iSize, false);
    int nRes = strftime(pszBuffer, iSize, pszFmt, pTm);
    strRes.releaseBuffer();
    if (nRes > 0 || iSize >= iSizeMax)
      break;
  }

  return strRes;
}

//---------------------------------------------------------------------------
StringA DateTime::getString (bool bUtc, bool bDate/*=true*/, bool bTime/*=true*/) const
{
  StringA strRes;

  if (bDate)
    strRes += this->format(bUtc, "%Y-%m-%d");
  if (bTime)
  {
    if (!strRes.isEmpty())
      strRes += ' ';
    strRes += this->format(bUtc, "%H:%M:%S");
  }

  return strRes;
}

//---------------------------------------------------------------------------
StringA DateTime::getIso8601String (void) const
{
  return this->format(true, "%Y-%m-%dT%H:%M:%SZ");
}




//---------------------------------------------------------------------------
DateTime DateTime::getNow (void)
{
  return DateTime().now();
}

//---------------------------------------------------------------------------
StringA DateTime::nowToString (bool bUtc, bool bDate, bool bTime)
{
  return DateTime().now().getString(bUtc, bDate, bTime);
}




//---------------------------------------------------------------------------
time_t DateTime::timegm (const tm* pTm)
{
  //
  // * Hopefully portable implementation of timegm().
  // * Public domain, written by Pasi Eronen <pe@iki.fi>.
  // *
  // * Notes:
  // * - DOESN'T MODIFY THE STRUCT TM!
  // * - Epoch doesn't have to be at January 1, 1970.
  // * - Out-of-range fields for struct tm are not handled properly.
  //

  #define LEAP_YEAR(y) (((y)%4 == 0) && ((y)%100 != 0 || (y)%400 == 0))

  static struct tm epoch;
  static int epoch_initialized = 0;
  static int months[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  int y, days, m, secs;

  if (!epoch_initialized)
  {
    time_t zero = 0;
    struct tm *tmp = gmtime(&zero);
    epoch = *tmp;
    epoch_initialized = 1;
  }

  days = 0;
  if (pTm->tm_year < epoch.tm_year)
    return -1;
  for (y = epoch.tm_year; (y < pTm->tm_year); y++)
    days += LEAP_YEAR(y) ? 366 : 365;

  for (m = 0; (m < epoch.tm_mon); m++)
    days -= months[m];
  if (LEAP_YEAR(epoch.tm_year) && (epoch.tm_mon > 1))
    days--;
  for (m = 0; (m < pTm->tm_mon); m++)
    days += months[m];
  if (LEAP_YEAR(pTm->tm_year) && (pTm->tm_mon > 1))
    days++;

  days += (pTm->tm_mday - epoch.tm_mday);
  if (days < 0)
    return -1;

  secs =
    ((pTm->tm_hour - epoch.tm_hour) * 60 +
	  (pTm->tm_min - epoch.tm_min)) * 60 +
    (pTm->tm_sec - epoch.tm_sec);
  if ((days == 0) && (secs < 0))
    return -1;

  return days * 24 * 60 * 60 + secs;

  #undef LEAP_YEAR
}


} // namespace glb
