//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-10 20:35:30
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
File::File (void)
{
  // common properties
  m_dwOpenFlags = 0;

  // current buffer access status
  m_dwBufferAccessMode = BUFFERACCESSMODE_NONE;
}

//---------------------------------------------------------------------------
File::~File (void)
{
}


//---------------------------------------------------------------------------
void File::close (void)
{
  // common properties
  m_dwOpenFlags = 0;

  // current buffer access status
  m_dwBufferAccessMode = BUFFERACCESSMODE_NONE;
}

//---------------------------------------------------------------------------
bool File::validateBufferAccessMode (flags32 dwBufferAccessMode) const
{
  flags32 dwCommand        = dwBufferAccessMode   & BUFFERACCESSMODE_MASK_COMMAND;
//flags32 dwCurrentCommand = m_dwBufferAccessMode & BUFFERACCESSMODE_MASK_COMMAND;

  if (dwCommand == BUFFERACCESSMODE_COMMAND_READONLY)
  {
    if (this->isBufferRequested())
    {
      GLB_ASSERT(0);
      return false;
    }

    if ((m_dwOpenFlags & FILE_FLAG_READ) == 0)
    {
      GLB_ASSERT(0);
      return false;
    }
  }
  else if (dwCommand == BUFFERACCESSMODE_COMMAND_READWRITE)
  {
    if (this->isBufferRequested())
    {
      GLB_ASSERT(0);
      return false;
    }

    if ((m_dwOpenFlags & FILE_FLAG_WRITE) == 0)
    {
      GLB_ASSERT(0);
      return false;
    }
  }
  else if (dwCommand == BUFFERACCESSMODE_COMMAND_COMMIT)
  {
    if (!this->isBufferRequested())
    {
      GLB_ASSERT(0);
      return false;
    }
  }
  else
  {
    GLB_THROW(EXCODE_PARAM, "Unknown BufferAccessMode command 0x%X !", dwBufferAccessMode);
  }

  return true;
}


} // namespace glb
