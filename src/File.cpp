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
// Created On: 2011-10-10 20:35:30
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
