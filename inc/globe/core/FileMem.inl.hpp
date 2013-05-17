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
