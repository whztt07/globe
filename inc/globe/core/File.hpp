//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-10 20:35:24
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_FILE_H__
#define __GLOBE_FILE_H__
namespace glb {


//---------------------------------------------------------------------------
// File
//---------------------------------------------------------------------------
class GLB_EXPORT File : private NonCopyable
{
public :
  enum OpenFlags
  {
    // access mode
    FILE_FLAG_READ            = 0x00000001,
    FILE_FLAG_WRITE           = 0x00000002,
    FILE_FLAG_READWRITE       = 0x00000003,

    // open / create mode (default behavior is to throw an exception if file does not exist)
    FILE_FLAG_CREATE_NEW      = 0x00000010,  // create file only if it does *not* exist
    FILE_FLAG_TRUNCATE        = 0x00000020,  // only in write mode
    FILE_FLAG_CREATE_ALWAYS   = FILE_FLAG_CREATE_NEW | FILE_FLAG_TRUNCATE,

    // share access (default access is "exclusive")
    FILE_FLAG_SHARE_READ      = 0x00000100,
    FILE_FLAG_SHARE_WRITE     = 0x00000200,
    FILE_FLAG_SHARE_READWRITE = 0x00000300,

    // not used
    FILEOPENFLAGS_FORCE_DWORD = 0x7FFFFFFF,
  };

  enum SeekOrigin
  {
    FILE_SEEK_BEGIN   = SEEK_SET,  // from the beginning of the file
    FILE_SEEK_CURRENT = SEEK_CUR,  // from the current position in the file
    FILE_SEEK_END     = SEEK_END,  // from the end of the file
  };

  enum BufferAccessType
  {
    BUFFERACCESSTYPE_NONE,      // no direct access to the container's buffer
    BUFFERACCESSTYPE_DIRECT,    // direct access to the container, this access type allows dynamic and automatic growing in write mode
    BUFFERACCESSTYPE_MAPPED,    // buffer access is granted by using mmap() on linux or MapViewOfFile() on windows platforms
    BUFFERACCESSTYPE_BLOCKING,  // socket or anything similar which is blocking
  };

  enum BufferAccessMode
  {
    // command
    BUFFERACCESSMODE_COMMAND_READONLY  = 1,  // get "read only" access to the buffer
    BUFFERACCESSMODE_COMMAND_READWRITE = 2,  // get writable buffer
    BUFFERACCESSMODE_COMMAND_COMMIT    = 3,  // release buffer (commit changes after a read/write command)

    // command flags
    BUFFERACCESSMODE_FLAG_DONOTGROW = 0x01000000,  // only useful with the BUFFERACCESSMODE_COMMAND_READWRITE command

    // those values are used internally
    BUFFERACCESSMODE_NONE         = 0,
    BUFFERACCESSMODE_MASK_COMMAND = 0x00FFFFFF,
    BUFFERACCESSMODE_MASK_FLAGS   = 0xFF000000,
  };


public :
  File (void);
  virtual ~File (void);

  // open / close
  virtual void    open         (const char* pszFilePath, flags32 dwOpenFlags, uint32 dwUnused1=0, uint32 dwUnused2=0) = 0;
  virtual void    close        (void);
  virtual bool    isOpen       (void) const;
  virtual flags32 getOpenFlags (void) const;

  // read / write
  virtual uint32 read       (void* pBuffer, uint32 uiLength) = 0;
  virtual void   write      (const void* pBuffer, uint32 uiLength) = 0;
  virtual void   flush      (void) = 0;
  virtual bool   isReadable (void) const;
  virtual bool   isWritable (void) const;
  virtual bool   isGrowable (void) const = 0;

  // seeking methods
  virtual void   seekToBegin (void);
  virtual uint64 seekToEnd   (void);
  virtual uint64 seek        (SeekOrigin eOrigin, int64 nOffset) = 0;
  virtual uint64 getPosition (void) const = 0;

  // file size
  virtual uint64 getLength (void) const = 0;
  virtual void   setLength (uint64 uiNewLength) = 0;


protected :
  // buffer access
  virtual BufferAccessType getBufferAccessType      (void) const = 0;
  virtual uint32           bufferAccess             (flags32 dwBufferAccessMode, uint32 uiLength=0, byte** ppBufferBegin=NULL, byte** ppBufferEnd=NULL) = 0;
  virtual bool             isBufferRequested        (void) const;
  virtual bool             validateBufferAccessMode (flags32 dwBufferAccessMode) const;


protected :
  // common properties
  flags32 m_dwOpenFlags;

  // current buffer access status
  flags32 m_dwBufferAccessMode;
};


#include "File.inl.hpp"


} // namespace glb
#endif // #ifndef __GLOBE_FILE_H__
