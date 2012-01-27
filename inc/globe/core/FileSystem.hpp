//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-10 19:45:21
//
// $Id$
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
