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
// Created On : 2011-11-03 14:43:12
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_ARCHIVE_H__
#define __GLOBE_ARCHIVE_H__
#ifdef GLOBE_VFS
namespace glb {


// undefine this for security purpose when you don't need the archive
// creation part of code anymore
//#define GLOBE_ARCHIVE_CANCREATE


//---------------------------------------------------------------------------
// Archive
//---------------------------------------------------------------------------
class GLB_EXPORT Archive
{
public :
  enum
  {
    HEADER_MAGIC_SIZE     = 6,
    HEADER_XORKEY_MAXSIZE = 255,

    HEADER_VERSION_1 = 0x01, // uint8
    HEADER_VERSION   = HEADER_VERSION_1, // current version
  };

  typedef GLB_PACKED struct Header
  {
    glb_byte  aMagic[HEADER_MAGIC_SIZE];
    glb_uint8 cVersion;
    glb_uint8 cXorKeySize; // 0 if content is not xor'ed
    glb_byte  aXorKey[HEADER_XORKEY_MAXSIZE];
  };

  typedef struct Entry
  {
    StringA strName;
    uint32  uiSize;  // uncompressed size

    // used internally
    struct
    {
      ulong pos_in_zip_directory;
      ulong num_of_file;
    } unzPos; // unz_file_pos
  };

  typedef std::vector<Entry>      Entries;
  typedef Entries::iterator       EntriesIt;
  typedef Entries::const_iterator EntriesCIt;


public :
  Archive (void);
  ~Archive (void);

  void open   (const char* pszArchive);
  bool isOpen (void) const;
  void close  (void);

  const Entries& getEntriesList     (void) const;
  bool           hasEntry           (const char* pszName) const;
  uint32         getEntrySize       (const char* pszName) const;
  uint32         extractEntry       (const char* pszName, glb_byte* pOutBuffer, uint32 uiBufferSize) const;
  glb_byte*      extractEntry       (const char* pszName, uint32* puiOutSize=NULL) const; // caller should free() memory after use
  void           extractEntryToFile (const char* pszName, const char* pszDestFile) const;


  // static utils
  static bool isArchive      (const char* pszFile);
  static void extractZipFile (const char* pszArchive, const char* pszOutFile);
#ifdef GLOBE_ARCHIVE_CANCREATE
  static void create         (const char* pszArchive, StringAVec vecFiles, bool bRecursive, const glb_byte* aXorKey=NULL, uint uiXorKeySize=0);
  static void createFromZip  (const char* pszArchive, const char* pszZipFile, const glb_byte* pXorKey=NULL, uint8 cXorKeySize=0);
#endif


private :
  const Entry* getEntry (const char* pszName) const;

  // static utils
  static uint xorBuffer (glb_byte* pBuffer, uint32 uiBufferSize, const glb_byte* pKey, uint uiKeySize, uint uiKeyPos=0);

  // unzip io static callbacks
  static void*  onUnzOpenS      (void* pOpaque, const char* pszFile, int iMode);
  static ulong  onUnzReadS      (void* pOpaque, void* pStream, void* pBuffer, ulong uiSize);
  static ulong  onUnzWriteS     (void* pOpaque, void* pStream, const void* pBuffer, ulong uiSize);
  static long   onUnzTellS      (void* pOpaque, void* pStream);
  static long   onUnzSeekS      (void* pOpaque, void* pStream, ulong uiOffset, int iOrigin);
  static int    onUnzCloseS     (void* pOpaque, void* pStream);
  static int    onUnzTestErrorS (void* pOpaque, void* pStream);

  // unzip io callbacks
  ulong onUnzRead      (void* pBuffer, ulong uiSize);
  long  onUnzTell      (void);
  long  onUnzSeek      (ulong uiOffset, int iOrigin);
  int   onUnzClose     (void);
  int   onUnzTestError (void);


private :
  StringA m_strPath;

  // unzip context
  void* m_unzipFileFunc; // zlib_filefunc_def
  void* m_hUnzip;        // unzFile

  // unzip io context
  FileDisk m_Arch;
  Header   m_Header;
  uint     m_uiXorKeyPos;

  // entries
  Entries m_Entries;
};


#include "Archive.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_VFS
#endif // #ifndef __GLOBE_ARCHIVE_H__
