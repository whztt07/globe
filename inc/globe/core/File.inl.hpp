//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-10 20:35:27
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline bool File::isOpen (void) const
{
  return (m_dwOpenFlags != 0);
}

//---------------------------------------------------------------------------
inline flags32 File::getOpenFlags (void) const
{
  return m_dwOpenFlags;
}

//---------------------------------------------------------------------------
inline bool File::isReadable (void) const
{
  return (m_dwOpenFlags & File::FILE_FLAG_READ) != 0;
}

//---------------------------------------------------------------------------
inline bool File::isWritable (void) const
{
  return (m_dwOpenFlags & File::FILE_FLAG_WRITE) != 0;
}

//---------------------------------------------------------------------------
inline void File::seekToBegin (void)
{
  (void)this->seek(File::FILE_SEEK_BEGIN, 0LL);
}

//---------------------------------------------------------------------------
inline uint64 File::seekToEnd (void)
{
  return this->seek(File::FILE_SEEK_END, 0LL);
}

//---------------------------------------------------------------------------
inline bool File::isBufferRequested (void) const
{
  return (m_dwBufferAccessMode != File::BUFFERACCESSMODE_NONE);
}
