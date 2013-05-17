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
// Created On : 2011-10-14 18:18:07
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline DateTime::DateTime (void)
: m_Unix((time_t)-1)
{
}

//---------------------------------------------------------------------------
inline DateTime::DateTime (time_t unix)
{
  this->set(unix);
}

//---------------------------------------------------------------------------
inline DateTime::DateTime (tm* pTm)
{
  this->set(pTm);
}

//---------------------------------------------------------------------------
inline DateTime::DateTime (bool bUtc, int iYear, int iMonth, int iDay, int iHour/*=0*/, int iMin/*=0*/, int iSec/*=0*/, int iDST/*=-1*/)
{
  this->set(bUtc, iYear, iMonth, iDay, iHour, iMin, iSec, iDST);
}

//---------------------------------------------------------------------------
inline DateTime& DateTime::invalidate (void)
{
  m_Unix = (time_t)-1;
  return *this;
}

//---------------------------------------------------------------------------
inline bool DateTime::isValid (void) const
{
  return m_Unix != (time_t)-1;
}

//---------------------------------------------------------------------------
inline time_t DateTime::getUnix (void) const
{
  return m_Unix;
}

//---------------------------------------------------------------------------
inline bool DateTime::operator== (const DateTime& rhs) const
{
  return
    (this->isValid() && rhs.isValid()) ?
    (m_Unix == rhs.m_Unix) :
    false;
}

//---------------------------------------------------------------------------
inline bool DateTime::operator!= (const DateTime& rhs) const
{
  return !(*this == rhs);
}

//---------------------------------------------------------------------------
inline bool DateTime::operator< (const DateTime& rhs) const
{
  return
    (this->isValid() && rhs.isValid()) ?
    (m_Unix < rhs.m_Unix) :
    false;
}

//---------------------------------------------------------------------------
inline bool DateTime::operator> (const DateTime& rhs) const
{
  return
    (this->isValid() && rhs.isValid()) ?
    (m_Unix > rhs.m_Unix) :
    false;
}

//---------------------------------------------------------------------------
inline bool DateTime::operator<= (const DateTime& rhs) const
{
  return
    (this->isValid() && rhs.isValid()) ?
    (m_Unix <= rhs.m_Unix) :
    false;
}

//---------------------------------------------------------------------------
inline bool DateTime::operator>= (const DateTime& rhs) const
{
  return
    (this->isValid() && rhs.isValid()) ?
    (m_Unix >= rhs.m_Unix) :
    false;
}
