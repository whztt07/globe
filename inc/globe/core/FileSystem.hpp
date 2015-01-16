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
// Created On: 2011-10-10 19:45:21
//
//***************************************************************************

#ifndef __GLOBE_FILESYSTEM_H__
#define __GLOBE_FILESYSTEM_H__
namespace glb {


//---------------------------------------------------------------------------
// FileSystem
//---------------------------------------------------------------------------
class GLB_EXPORT FileSystem
{
public :
  // file existency
  static bool doesExist (const char* pszPath);
  static bool remove    (const char* pszPath, bool bRecursive=false);

  // file type
  static bool isDirectory (const char* pszPath);

  // file attributes
  static uint64 getSize      (const char* pszPath);
  static time_t getModifTime (const char* pszPath); // returns unix time (utc)

  // directories specific
  static bool createDirectory (const char* pszPath, bool bRecursive=false);

  // files operation
  static bool copyFile (const char* pszFrom, const char* pszTo, bool bOverwrite=false); // doesn't keep attributes on non-windows platforms !
  static bool moveFile (const char* pszFrom, const char* pszTo, bool bOverwrite=false); // doesn't keep attributes on non-windows platforms !

  // temp files
  static StringA getTempDirectory    (void);
  static StringA createTempDirectory (const char* pszPrefix="");
  static StringA createTempFile      (const char* pszPrefix="", const char* pszExtension="");

  // extract file content to memory
  // * this method is just here for convenience, if you are searching for
  //   something more complete, try the FileMem::OpenFromDisk() method.
  static uint32 extractContent (const char* pszFilePath, byte* pBuffer, uint32 uiBufferSize);

  // windows specifics
#if defined(GLB_PLATFORM_WINDOWS)
  static int    winIsUtcFileSystem       (const char* pszPath);
  static time_t winUtcFileTimeToUnixTime (const void* pFileTimeStructUTC);
#endif
};


} // namespace glb
#endif // #ifndef __GLOBE_FILESYSTEM_H__
