//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-29 23:53:45
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline bool FileIterator::isOpen (void) const
{
  return !m_strDir.isEmpty();
}

//---------------------------------------------------------------------------
inline bool FileIterator::isEmpty (void) const
{
  return m_bEmpty;
}

//---------------------------------------------------------------------------
inline const StringA& FileIterator::getDirectory (void) const
{
  return m_strDir;
}
