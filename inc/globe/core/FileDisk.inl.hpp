//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-10 20:37:45
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline const StringA& FileDisk::getPath (void) const
{
  return m_strPath;
}

//---------------------------------------------------------------------------
inline bool FileDisk::isGrowable (void) const
{
  return this->isWritable();
}

//---------------------------------------------------------------------------
inline File::BufferAccessType FileDisk::getBufferAccessType (void) const
{
  return File::BUFFERACCESSTYPE_MAPPED;
}
