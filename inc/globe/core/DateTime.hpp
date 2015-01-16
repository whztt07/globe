//***************************************************************************
//
// Globe
// A small C++ library to allow fast prototyping of Direct3D animations.
//
// Copyright (C) 2012-2013 Jean-Charles Lefebvre <polyvertex@gmail.com>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions :
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
// Created On: 2011-10-14 18:16:35
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
