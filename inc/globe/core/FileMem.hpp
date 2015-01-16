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
// Created On: 2011-10-10 20:37:48
//
//***************************************************************************

#ifndef __GLOBE_FILEMEM_H__
#define __GLOBE_FILEMEM_H__
namespace glb {


//---------------------------------------------------------------------------
// FileMem
//---------------------------------------------------------------------------
class GLB_EXPORT FileMem : public File
{
public :
  FileMem (void);
  FileMem (uint32 uiGrowBytes); // implicit call to openNew()
  FileMem (const char* pBuffer, uint32 uiBufferSize, uint32 uiInitialFileSize, uint32 uiGrowBytes=0); // implicit call to open()
  virtual ~FileMem (void);

  // open / close
  // * A FileMem buffer is always growable except in one case : when you
  //   provide your own buffer to the open() method. this is because the
  //   FileMem class will not be able to reallocate buffer if needed since
  //   there is no way to know, in standard c++, which alloc method you used
  //   (new[] or malloc, etc...).
  // * openNew() is a convenient shortcut for a open(0,0,0,uiGrowBytes) call
  //   to initialize an empty and automatically growable memory file.
  virtual void  open         (const char* pBuffer, uint32 uiBufferSize, uint32 uiInitialFileSize, uint32 uiGrowBytes=0);
  virtual void  openNew      (uint32 uiGrowBytes=1024);
  virtual void  openFromDisk (const char* pszFilePath, uint32 uiMaxLengthToReadFromDisk=0, int64 nOffset=0, uint32 uiGrowBytes=0);
  virtual void  close        (void);
  virtual bool  isOpen       (void) const;

  // read / write
  virtual uint32 read       (void* pBuffer, uint32 uiLength);
  virtual void   write      (const void* pBuffer, uint32 uiLength);
  virtual void   flush      (void);
  virtual bool   isGrowable (void) const;

  // seeking methods
  virtual uint64 seek        (SeekOrigin eOrigin, int64 nOffset);
  virtual uint64 getPosition (void) const;

  // file size
  virtual uint64 getLength (void) const;
  virtual void   setLength (uint64 uiNewLength);

  // direct access to the buffer
  // * use this only to *read* operations so you can get the amount of data
  //   with the getLength() method.
  // * remember the getLength() method does not return the capacity of the
  //   buffer but the data effectively written to this buffer !
  // * in any case, prefer to use the read/write methods or even consider to
  //   use the Streamer class. this method is only here to provide a way to
  //   get instant acces to the internal buffer so you can send it to an
  //   third party API if needed.
  virtual const byte* getDirectBuffer (void) const;


  // FileMem specific methods
  virtual void writeToDiskFile (const char* pszFilePath) const; // existing file will be truncated


protected :
  // buffer access
  virtual BufferAccessType getBufferAccessType      (void) const;
  virtual uint32           bufferAccess             (flags32 dwBufferAccessMode, uint32 uiLength=0, byte** ppBufferBegin=NULL, byte** ppBufferEnd=NULL);

  // memory file helpers
  virtual void growMemFile (uint32 uiNewSize);

  // memory helpers
  virtual byte* blockAlloc   (uint32 uiSize);
  virtual byte* blockRealloc (byte* pBlock, uint32 uiNewSize);
  virtual void  blockCopy    (byte* pTargetBlock, const byte* pSourceBlock, uint32 uiSize);
  virtual void  blockFree    (byte* pBlock);


protected :
  byte*  m_pBuffer;
  uint32 m_uiBufferSize;
  uint32 m_uiFileSize;
  uint32 m_uiGrowBytes;
  uint32 m_uiPosition;
  bool   m_bAutoDelete;
};


#include "FileMem.inl.hpp"


} // namespace glb
#endif // #ifndef __GLOBE_FILEMEM_H__
