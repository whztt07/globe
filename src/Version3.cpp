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
// Created On : 2012-09-20 11:08:53
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
bool Version3::init (int32 iMajor, int32 iMinor, int32 iRevision)
{
  m_iMajor    = iMajor;
  m_iMinor    = iMinor;
  m_iRevision = iRevision;

  m_szStripped[0] = 0;
  m_szPadded[0]   = 0;

  if (!this->isValid())
  {
    this->reset();
    return false;
  }

  return true;
}

//---------------------------------------------------------------------------
bool Version3::init (int32 iCombined)
{
  m_szStripped[0] = 0;
  m_szPadded[0]   = 0;

  if ((iCombined < 0) || (iCombined > 9999999))
    goto __Error;

  // major
  if (iCombined >= 1000000)
  {
    m_iMajor   = iCombined / 1000000;
    iCombined %= 1000000;
  }
  else
  {
    m_iMajor = 0;
  }

  // minor
  if (iCombined >= 10000)
  {
    m_iMinor   = iCombined / 10000;
    iCombined %= 10000;
  }
  else
  {
    m_iMinor = 0;
  }

  // revision
  m_iRevision = iCombined;

  // check
  if (!this->isValid())
    goto __Error;

  return true;

__Error :
  this->reset();
  return false;
}

//---------------------------------------------------------------------------
bool Version3::init (const char* pszVersion)
{
  StringA str;

  if (!pszVersion)
    goto __Error;
  str = pszVersion;
  str.trimWhitespaces();
  if (str.isEmpty() || str.length() > 9)
    goto __Error;

  m_iMajor    = 0;
  m_iMinor    = 0;
  m_iRevision = 0;
  m_szStripped[0] = 0;
  m_szPadded[0]   = 0;

  if (str.isFullOfDigits())
  {
    m_iMajor = str.extractInt32();
    if (!this->isValid())
      goto __Error;
  }
  else if (str.findFirstOf('.') >= 0)
  {
    StringAVec vec;

    str.explode(vec, ".", 0, true);
    if (vec.size() < 1 || vec.size() > 3)
      goto __Error;

    for (uint i = 0; i < vec.size(); ++i)
    {
      if (!vec[i].isFullOfDigits())
        goto __Error;
      if (i == 0)
        m_iMajor = vec[i].extractInt32();
      else if (i == 1)
        m_iMinor = vec[i].extractInt32();
      else if (i == 2)
        m_iRevision = vec[i].extractInt32();
    }
  }
  else
  {
    goto __Error;
  }

  if (!this->isValid())
    goto __Error;

  return true;

__Error :
  this->reset();
  return false;
}

//---------------------------------------------------------------------------
void Version3::reset (void)
{
  m_iMajor    = -1;
  m_iMinor    = -1;
  m_iRevision = -1;

  m_szStripped[0] = 0;
  m_szPadded[0]   = 0;
}

//---------------------------------------------------------------------------
void Version3::buildStrings (void) const
{
  GLB_ASSERT(this->isValid());

  snprintf((char*)&m_szPadded, sizeof(m_szPadded)-1, "%d.%02d.%04d",
    m_iMajor, m_iMinor, m_iRevision);

  if (m_iRevision > 0)
  {
    snprintf((char*)&m_szStripped, sizeof(m_szStripped)-1, "%d.%d.%d",
      m_iMajor, m_iMinor, m_iRevision);
  }
  else if (m_iMinor > 0)
  {
    snprintf((char*)&m_szStripped, sizeof(m_szStripped)-1, "%d.%d",
      m_iMajor, m_iMinor);
  }
  else
  {
    snprintf ((char*)&m_szStripped, sizeof(m_szStripped)-1, "%d", m_iMajor);
  }
}


}
