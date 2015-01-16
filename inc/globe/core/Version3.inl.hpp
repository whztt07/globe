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
// Created On: 2012-09-20 11:08:51
//
//***************************************************************************


//---------------------------------------------------------------------------
inline Version3::Version3 (void)
{
  this->reset();
}

//---------------------------------------------------------------------------
inline Version3::Version3 (int32 iMajor, int32 iMinor, int32 iRevision)
{
  this->init(iMajor, iMinor, iRevision);
}

//---------------------------------------------------------------------------
inline Version3::Version3 (int32 iCombined)
{
  this->init(iCombined);
}

//---------------------------------------------------------------------------
inline Version3::Version3 (const char* pszVersion)
{
  this->init(pszVersion);
}

//---------------------------------------------------------------------------
inline Version3::Version3 (const Version3& rhs)
{
  *this = rhs;
}

//---------------------------------------------------------------------------
inline Version3::~Version3 (void)
{
}

//---------------------------------------------------------------------------
inline Version3& Version3::operator= (const Version3& rhs)
{
  m_iMajor    = rhs.m_iMajor;
  m_iMinor    = rhs.m_iMinor;
  m_iRevision = rhs.m_iRevision;

  strcpy((char*)&m_szStripped, (char*)&rhs.m_szStripped);
  strcpy((char*)&m_szPadded, (char*)&rhs.m_szPadded);

  return *this;
}

//---------------------------------------------------------------------------
inline bool Version3::isValid (void) const
{
  return
    (m_iMajor    >= 0) && (m_iMajor    <= 9) &&
    (m_iMinor    >= 0) && (m_iMinor    <= 99) &&
    (m_iRevision >= 0) && (m_iRevision <= 9999);
}

//---------------------------------------------------------------------------
inline int Version3::getMajor (void) const
{
  GLB_ASSERT(this->isValid());
  return m_iMajor;
}

//---------------------------------------------------------------------------
inline int Version3::getMinor (void) const
{
  GLB_ASSERT(this->isValid());
  return m_iMinor;
}

//---------------------------------------------------------------------------
inline int Version3::getRevision (void) const
{
  GLB_ASSERT(this->isValid());
  return m_iRevision;
}

//---------------------------------------------------------------------------
inline int Version3::getCombined (void) const
{
  GLB_ASSERT(this->isValid());
  return (1000000 * m_iMajor) + (10000 * m_iMinor) + m_iRevision;
}

//---------------------------------------------------------------------------
inline const char* Version3::getStrippedString (void) const
{
  GLB_ASSERT(this->isValid());
  if (!m_szStripped[0] && this->isValid())
    this->buildStrings();

  return (const char*)&m_szStripped;
}

//---------------------------------------------------------------------------
inline const char* Version3::getPaddedString (void) const
{
  GLB_ASSERT(this->isValid());
  if (!m_szPadded[0] && this->isValid())
    this->buildStrings();

  return (const char*)&m_szPadded;
}

//---------------------------------------------------------------------------
inline bool Version3::operator== (const Version3& rhs) const
{
  return this->getCombined() == rhs.getCombined();
}

//---------------------------------------------------------------------------
inline bool Version3::operator!= (const Version3& rhs) const
{
  return this->getCombined() != rhs.getCombined();
}

//---------------------------------------------------------------------------
inline bool Version3::operator< (const Version3& rhs) const
{
  return this->getCombined() < rhs.getCombined();
}

//---------------------------------------------------------------------------
inline bool Version3::operator> (const Version3& rhs) const
{
  return this->getCombined() > rhs.getCombined();
}

//---------------------------------------------------------------------------
inline bool Version3::operator<= (const Version3& rhs) const
{
  return this->getCombined() <= rhs.getCombined();
}

//---------------------------------------------------------------------------
inline bool Version3::operator>= (const Version3& rhs) const
{
  return this->getCombined() >= rhs.getCombined();
}
