//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-10 20:37:42
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_FILEDISK_H__
#define __GLOBE_FILEDISK_H__
namespace glb {


//---------------------------------------------------------------------------
// FileDisk
//---------------------------------------------------------------------------
class GLB_EXPORT FileDisk : public File
{
public :
  FileDisk (void);
  FileDisk (const char* pszFilePath, flags32 dwOpenFlags);
  virtual ~FileDisk (void);

  // open / close
  virtual void           open         (const char* pszFilePath, flags32 dwOpenFlags, uint32 dwUnused1=0, uint32 dwUnused2=0);
  virtual void           close        (void);
  virtual const StringA& getPath      (void) const;

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


protected :
  // buffer access
  virtual BufferAccessType getBufferAccessType      (void) const;
  virtual uint32           bufferAccess             (flags32 dwBufferAccessMode, uint32 uiLength=0, byte** ppBufferBegin=NULL, byte** ppBufferEnd=NULL);


protected :
  // path
  StringA m_strPath;

  // current buffer access status
  byte*  m_pMappedBuffer;
  uint32 m_uiMappedBufferLength;

  // handles
#if defined(GLB_PLATFORM_WINDOWS)
  void* m_hFile;
  void* m_hFileMap;
#else
  FILE* m_pFile;
#endif
};


#include "FileDisk.inl.hpp"


} // namespace glb
#endif // #ifndef __GLOBE_FILEDISK_H__
