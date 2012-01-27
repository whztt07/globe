//***************************************************************************
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
