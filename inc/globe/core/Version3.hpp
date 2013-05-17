//***************************************************************************
//
// Globe
// A small C++ library to allow fast prototyping of Direct3D animations.
//
// Copyright (C) 2012-2013 Jean-Charles Lefebvre <jcl [AT] jcl [DOT] io>
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
//
// Author     : Jean-Charles Lefebvre
// Created On : 2012-09-20 11:08:49
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_VERSION3_H__
#define __GLOBE_VERSION3_H__
namespace glb {


//---------------------------------------------------------------------------
// Version3
//
// A valid Version3 string must be formatted this way :
// "M.m.R" or "M.mm.RRRR"
//
// Where :
// * "M" is the major version number
// * "m" is the minor version number
// * "R" is the revision/build/whatever-you-want version number
//
// A valid Version3 has its components' values in :
// * 0 <= Major    <= 9
// * 0 <= minor    <= 99
// * 0 <= Revision <= 9999
//
// The "combined" value is an all-in-one representation of the 3 components
// of Version3 and is computed this way :
// (1000000 * Major) + (10000 * minor) + Revision
//---------------------------------------------------------------------------
class GLB_EXPORT Version3
{
public :
  Version3 (void);
  Version3 (int32 iMajor, int32 iMinor, int32 iRevision);
  Version3 (int32 iCombined);
  Version3 (const char* pszVersion);
  Version3 (const Version3& rhs);
  ~Version3 (void);

  bool init (int32 iMajor, int32 iMinor, int32 iRevision);
  bool init (int nCombined);
  bool init (const char* pszVersion);

  Version3& operator= (const Version3& rhs);

  bool isValid (void) const;

  bool operator== (const Version3& rhs) const;
  bool operator!= (const Version3& rhs) const;
  bool operator<  (const Version3& rhs) const;
  bool operator<= (const Version3& rhs) const;
  bool operator>  (const Version3& rhs) const;
  bool operator>= (const Version3& rhs) const;

  int         getMajor          (void) const;
  int         getMinor          (void) const;
  int         getRevision       (void) const;
  int         getCombined       (void) const;
  const char* getStrippedString (void) const; // "M.m.R" or "M.m" or "M"
  const char* getPaddedString   (void) const; // "M.mm.RRRR"


private :
  void reset        (void);
  void buildStrings (void) const;


private :
  int32 m_iCombined;

  int32 m_iMajor;
  int32 m_iMinor;
  int32 m_iRevision;

  mutable char m_szStripped[12];
  mutable char m_szPadded[12];
};


#include "Version3.inl.hpp"


}
#endif // #ifndef __GLOBE_VERSION3_H__
