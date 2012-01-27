//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-10 20:37:50
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline bool FileMem::isOpen (void) const
{
  return (m_uiBufferSize != 0) || (m_uiGrowBytes != 0);
}

//---------------------------------------------------------------------------
inline bool FileMem::isGrowable (void) const
{
  return this->isWritable() && (m_uiGrowBytes > 0);
}

//---------------------------------------------------------------------------
inline uint64 FileMem::getPosition (void) const
{
  GLB_ASSERT(m_pBuffer);
  return m_uiPosition;
}

//---------------------------------------------------------------------------
inline uint64 FileMem::getLength (void) const
{
  return m_uiFileSize;
}

//---------------------------------------------------------------------------
inline const byte* FileMem::getDirectBuffer (void) const
{
  return m_pBuffer;
}

//---------------------------------------------------------------------------
inline File::BufferAccessType FileMem::getBufferAccessType (void) const
{
  return File::BUFFERACCESSTYPE_DIRECT;
}
