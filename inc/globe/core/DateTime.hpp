//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-14 18:16:35
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_DATETIME_H__
#define __GLOBE_DATETIME_H__
namespace glb {


//---------------------------------------------------------------------------
// DateTime
//---------------------------------------------------------------------------
class GLB_EXPORT DateTime
{
public :
  DateTime (void); // calls invalidate()
  DateTime (time_t unix);
  DateTime (tm* pTm);
  DateTime (bool bUtc, int iYear, int iMonth, int iDay, int iHour=0, int iMin=0, int iSec=0, int iDST=-1);

  DateTime& invalidate (void); // returns this
  DateTime& now        (void); // returns this
  void      set        (time_t unix);
  bool      set        (tm* pTm);
  bool      set        (bool bUtc, int iYear, int iMonth, int iDay, int iHour=0, int iMin=0, int iSec=0, int iDST=-1);
  bool      setIso8601 (const char* pszIso8601); // limited to the case "2011-01-01T12:34:56Z"

  bool      isValid          (void) const;
  time_t    getUnix          (void) const;
  time_t    getDiff          (const DateTime& rhs, bool bAbsolute) const;
  const tm* getTm            (bool bUtc) const; // NOT THREAD-SAFE
  StringA   format           (bool bUtc, const char* pszFmt) const; // NOT THREAD-SAFE, calls getTm() and strftime()
  StringA   getString        (bool bUtc, bool bDate=true, bool bTime=true) const;
  StringA   getIso8601String (void) const;

  bool operator== (const DateTime& rhs) const;
  bool operator!= (const DateTime& rhs) const;
  bool operator<  (const DateTime& rhs) const;
  bool operator>  (const DateTime& rhs) const;
  bool operator<= (const DateTime& rhs) const;
  bool operator>= (const DateTime& rhs) const;


  static DateTime getNow      (void);
  static StringA  nowToString (bool bUtc, bool bDate, bool bTime);


private :
  static time_t timegm (const tm* pTm);


public :
  time_t m_Unix;
};


#include "DateTime.inl.hpp"


} // namespace glb
#endif // #ifndef __GLOBE_DATETIME_H__
