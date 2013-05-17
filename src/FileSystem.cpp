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
// Created On : 2011-10-10 19:45:24
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
static bool _FileSystem_createDirectoryImpl (const char* pszPath)
{
  int nRes;

  #if defined(GLB_PLATFORM_WINDOWS)
  {
    nRes = _mkdir(pszPath);
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
    int nUMask;

    nUMask = umask(0);
    umask(nUMask);

    nRes = mkdir(pszPath, nUMask);
  }
  #else
  #error Unsupported platform !
  #endif

  if (nRes == -1)
  {
    switch (errno)
    {
      case EACCES :
        GLB_LOGERR("Permission denied to create diretory '%s' !", pszPath);
        return false;

      case EEXIST :
        if (FileSystem::isDirectory(pszPath))
        {
          return true;
        }
        else
        {
          GLB_LOGERR("Cannot create directory at '%s' because it already exist and it's a file !", pszPath);
          return false;
        }
        break;

      case ENOENT :
        GLB_LOGERR("Path not found ! Cannot create directory '%s'.", pszPath);
        return false;

      default :
        GLB_LOGERR("Failed to create directory '%s' !", System::lastErrorString());
        return false;
    }
  }

  return true;
}




//---------------------------------------------------------------------------
bool FileSystem::doesExist (const char* pszPath)
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    DWORD dwAttr = ::GetFileAttributesA(pszPath);
    if (dwAttr == INVALID_FILE_ATTRIBUTES) return false;
    return true;
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
    int nRes;
    struct stat stats;
    nRes = ::stat(pszPath, &stats);
    if (nRes != 0) return false;
    return true;
  }
  #else
  #error Unsupported platform !
  #endif
}

//---------------------------------------------------------------------------
bool FileSystem::remove (const char* pszPath, bool bRecursive/*=false*/)
{
  if (FileSystem::isDirectory(pszPath))
  {
    #if defined(GLB_PLATFORM_WINDOWS)
    {
      if (!bRecursive)
      {
        if (!RemoveDirectoryA(pszPath))
        {
          GLB_LOGWARN("Failed to remove directory \"%s\" non-recursively ! Error %u : %s", pszPath, System::lastError(), System::lastErrorString());
          return false;
        }
      }
      else
      {
        char szFrom[MAX_PATH + 1];
        int  iRes;
        SHFILEOPSTRUCTA fop;

        // msdn says that it's important to ensure the pFrom and pTo members
        // are double-null terminated...
        strcpy((char*)&szFrom, pszPath);
        szFrom[strlen((char*)&szFrom)] = '\0';

        ZeroMemory(&fop, sizeof(fop));
        fop.hwnd   = NULL;
        fop.wFunc  = FO_DELETE;
        fop.pFrom  = (char*)&szFrom;
        fop.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

        if ((iRes = SHFileOperation(&fop)) != 0)
        {
          GLB_LOGWARN("Failed to remove directory \"%s\" recursively ! Error %i (0x%X)", pszPath, iRes, iRes);
          return false;
        }
      }
    }
    #elif defined(GLB_PLATFORM_LINUX)
    {
      // TODO : use nftw() ?
      #error TODO
    }
    #else
    #error Unsupported platform !
    #endif
  }
  else
  {
    if (::remove(pszPath) != 0)
    {
      switch (errno)
      {
        case ENOENT :
          //GLB_LOGINFO("File \"%s\" does not exist.", pszPath);
          return true;

        case EACCES :
          GLB_LOGWARN("Access to file \"%s\" is denied !", pszPath);
          return false;

        default :
          GLB_LOGWARN("Unable to remove file \"%s\" (errno[%d]).", pszPath, errno);
          return false;
      }
    }
  }

  return true;
}

//---------------------------------------------------------------------------
bool FileSystem::isDirectory (const char* pszPath)
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    DWORD dwAttr;

    dwAttr = ::GetFileAttributesA(pszPath);
    if (dwAttr == INVALID_FILE_ATTRIBUTES) return false;

    return (dwAttr & FILE_ATTRIBUTE_DIRECTORY) ? true : false;
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
    int nRes;
    struct stat stats;

    nRes = ::stat(pszPath, &stats);
    if (nRes != 0) return false;

    return S_ISDIR(stats.st_mode) ? true : false;
  }
  #else
  #error Unsupported platform !
  #endif
}

//---------------------------------------------------------------------------
uint64 FileSystem::getSize (const char* pszPath)
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    typedef BOOL (WINAPI *PGETFILESIZEEX)(HANDLE, PLARGE_INTEGER);

    static PGETFILESIZEEX pfnGetFileSizeEx =
      (PGETFILESIZEEX)::GetProcAddress(::LoadLibraryA("kernel32.dll"), "GetFileSizeEx");

    HANDLE         hFile;
    ULARGE_INTEGER li;

    hFile = ::CreateFileA(pszPath, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
#ifdef GLB_DEBUG
      GLB_LOGERR("Unable to open file '%s' to get its size ! System error : %s", pszPath, System::lastErrorString());
#endif
      return 0;
    }

    if (!pfnGetFileSizeEx)
    {
      static bool bFirst = true;
      if (bFirst)
      {
        GLB_LOGINFO("Unable to get GetFileSizeEx() proc address from kernel32.dll ! Switching to GetFileSize()...");
        bFirst = false;
      }

      li.HighPart = 0;
      li.LowPart  = ::GetFileSize(hFile, &li.HighPart);
      if ((li.LowPart == INVALID_FILE_SIZE) && (::GetLastError() != NO_ERROR))
      {
#ifdef GLB_DEBUG
        GLB_LOGERR("Unable to get size of file '%s' ! System error : %s", pszPath, System::lastErrorString());
#endif
        ::CloseHandle(hFile);
        return 0;
      }
    }
    else
    {
      if (!pfnGetFileSizeEx(hFile, (PLARGE_INTEGER)&li))
      {
#ifdef GLB_DEBUG
        GLB_LOGERR("Unable to get size of file '%s' ! System error : %s", pszPath, System::lastErrorString());
#endif
        ::CloseHandle(hFile);
        return 0;
      }
    }

    ::CloseHandle(hFile);

    return static_cast<uint64>(li.QuadPart);
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
#if defined(GLB_PLATFORM_CYGWIN)
    #define STATSTRUCT  struct stat
    #define STATFUNC    ::stat
#else
    #define STATSTRUCT  struct stat64
    #define STATFUNC    ::stat64
#endif

    int nRes;
    STATSTRUCT stats;

    nRes = STATFUNC(pszPath, &stats);
    if (nRes != 0)
    {
#ifdef GLB_DEBUG
      GLB_LOGERR("Unable to get size of file '%s' ! System error : %s", pszPath, System::lastErrorString());
#endif
      return 0;
    }

    return stats.st_size;

#undef STATSTRUCT
#undef STATFUNC
  }
  #else
  #error Unsupported platform !
  #endif
}

//---------------------------------------------------------------------------
time_t FileSystem::getModifTime (const char* pszPath)
{
  GLB_ASSERT(pszPath);
  if (!pszPath)
    return 0;

  #if defined(GLB_PLATFORM_WINDOWS)
  {
    HANDLE           hFind;
    WIN32_FIND_DATAA wfd;

    // I used FindFirstFile() instead of GetFileTime() because it
    // automatically converts non-utc times into utc times.  - jcl
    hFind = ::FindFirstFileA(pszPath, &wfd);
    if (hFind == INVALID_HANDLE_VALUE)
    {
      GLB_LOGERR("Unable to get modification time of \"%s\" ! Error %lu : %s", pszPath, System::lastError(), System::lastErrorString());
      return 0;
    }
    ::FindClose(hFind);

    return FileSystem::winUtcFileTimeToUnixTime(&wfd.ftLastWriteTime);
  }
  #elif defined(GLB_PLATFORM_CYGWIN)
  {
    struct stat stats;

    if (::stat(pszPath, &stats) != 0)
    {
      GLB_LOGERR("Unable to get modification time of \"%s\" ! Error %lu : %s", pszPath, System::lastError(), System::lastErrorString());
      return 0;
    }

    return (time_t)stats.st_mtime;
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
    if (sizeof(time_t) == 64)
    {
      struct stat64 stats;
      if (::stat64(pszPath, &stats) != 0)
        goto __Error;
      return stats.st_mtime;
    }
    else if (sizeof(time_t) == 32)
    {
      struct stat stats;
      if (::stat(pszPath, &stats) != 0)
        goto __Error;
      return stats.st_mtime;
    }

    GLB_ASSERT(0 && "This should never happen !");
    return 0;

    __Error :
    GLB_LOGERR("Unable to get modification time of \"%s\" ! Error %lu : %s", pszPath, System::lastError(), System::lastErrorString());
    return 0;
  }
  #else
  #error Unsupported platform !
  #endif
}

//---------------------------------------------------------------------------
bool FileSystem::createDirectory (const char* pszPath, bool bRecursive/*=false*/)
{
  if (!bRecursive)
  {
    GLB_ASSERT(pszPath);
    return _FileSystem_createDirectoryImpl(pszPath);
  }
  else
  {
    StringA strPath(pszPath);

#if defined(GLB_PLATFORM_WINDOWS)
    typedef int (WINAPI *SHCREATEDIRECTORYA) (HWND, LPCSTR, SECURITY_ATTRIBUTES*);
    static SHCREATEDIRECTORYA pfnSHCreateDirectoryExA =
      (SHCREATEDIRECTORYA)::GetProcAddress(::LoadLibraryA("shell32.dll"), "SHCreateDirectoryExA");

    // MSDN says about SHCreateDirectoryExA() and CreateDirectory() functions
    // that given directory path should not exceed 248 characters *including*
    // the terminating null character (though there is the Unicode version
    // trick with the CreateDirectoryW() function).
    // Nothing is specified for the _mkdir() function so I guess it's OK,
    // otherwise we will be forced to make the _FileSystem_createDirectoryImpl()
    // function to call CreateDirectoryW() instead of _mkdir().  - jcl
    if (pfnSHCreateDirectoryExA && (strPath.length() < 248))
    {
      int nRes;

      strPath.pathSlashesToBackSlashes(); // required
      nRes = pfnSHCreateDirectoryExA(NULL, strPath.c_str(), NULL);
      if (nRes != ERROR_SUCCESS)
      {
        GLB_LOGERR("Could not create \"%s\" directory recursively ! Error %d.", pszPath, nRes);
        return false;
      }
    }
    else
#endif // #if defined(GLB_PLATFORM_WINDOWS)
    {
      StringAVec vecPath;

      strPath.pathExplode(vecPath);

      for (size_t Ix=0 ; Ix<vecPath.size() ; ++Ix)
      {
        // implode each part of the path with every previous parts
        if (Ix > 0)
        {
          vecPath[Ix].insert(vecPath[Ix-1].c_str(), 0);
          vecPath[Ix].insert('/', vecPath[Ix-1].length());

          GLB_ASSERT(!vecPath[Ix].pathHasTrailingSeparator());
        }

        if (FileSystem::doesExist(vecPath[Ix]))
        {
          if (!FileSystem::isDirectory(vecPath[Ix]))
          {
            GLB_LOGERR("Could not create \"%s\" directory recursively because \"%s\" is not a directory !", pszPath, vecPath[Ix].c_str());
            return false;
          }
        }
        else
        {
          if (Ix == 0)
          {
            if (vecPath[Ix].pathIsWindowsNetwork())
            {
              GLB_LOGERR("Cannot create a Windows Network share \"%s\" !", vecPath[Ix].c_str());
              return false;
            }
          }

          if (!_FileSystem_createDirectoryImpl(vecPath[Ix].c_str()))
            return false;
        }
      }
    }
  }

  return true;
}

//---------------------------------------------------------------------------
bool FileSystem::copyFile (const char* pszFrom, const char* pszTo, bool bOverwrite/*=false*/)
{
  if (!bOverwrite && FileSystem::doesExist(pszTo))
    return false;

  #if defined(GLB_PLATFORM_WINDOWS)
  {
    char szFrom[MAX_PATH + 2];
    char szTo[MAX_PATH + 2];
    int  iRes;
    SHFILEOPSTRUCTA fop;

    // msdn says that it's important to ensure the pFrom and pTo members
    // are double-null terminated...
    strcpy((char*)&szFrom, pszFrom);
    szFrom[strlen((char*)&szFrom) + 1] = '\0';
    strcpy((char*)&szTo, pszTo);
    szTo[strlen((char*)&szTo) + 1] = '\0';

    ZeroMemory(&fop, sizeof(fop));
    fop.hwnd   = NULL;
    fop.wFunc  = FO_COPY;
    fop.pFrom  = (char*)&szFrom;
    fop.pTo    = (char*)&szTo;
    fop.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;

    if ((iRes = SHFileOperation(&fop)) != 0)
    {
      GLB_LOGWARN("Failed to copy \"%s\" to \"%s\" ! Error %i (0x%X)", pszFrom, pszTo, iRes, iRes);
      return false;
    }

    return true;
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
    // TODO : find a better way !

    FileDisk from;
    FileDisk to;
    glb_byte aBuffer[8 * 1024];
    uint     uiSize;

    from.open(pszFrom, File::FILE_FLAG_READ | File::FILE_FLAG_SHARE_READ);
    to.open(pszTo, File::FILE_FLAG_WRITE | (bOverwrite ? File::FILE_FLAG_CREATE_ALWAYS : File::FILE_FLAG_CREATE_NEW));

    while ((uiSize = from.read(&aBuffer, sizeof(aBuffer))) > 0)
      to.write(&aBuffer, uiSize);

    return true;
  }
  #else
  #error Unsupported platform !
  #endif
}

//---------------------------------------------------------------------------
bool FileSystem::moveFile (const char* pszFrom, const char* pszTo, bool bOverwrite/*=false*/)
{
  if (!bOverwrite && FileSystem::doesExist(pszTo))
    return false;

  #if defined(GLB_PLATFORM_WINDOWS)
  {
    char szFrom[MAX_PATH + 2];
    char szTo[MAX_PATH + 2];
    int  iRes;
    SHFILEOPSTRUCTA fop;

    // msdn says it's important to ensure that pFrom and pTo members are
    // correctly terminated by 2 null characters...
    strcpy((char*)&szFrom, pszFrom);
    szFrom[strlen((char*)&szFrom) + 1] = '\0';
    strcpy((char*)&szTo, pszTo);
    szTo[strlen((char*)&szTo) + 1] = '\0';

    ZeroMemory(&fop, sizeof(fop));
    fop.hwnd   = NULL;
    fop.wFunc  = FO_MOVE;
    fop.pFrom  = (char*)&szFrom;
    fop.pTo    = (char*)&szTo;
    fop.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;

    if ((iRes = SHFileOperation(&fop)) != 0)
    {
      GLB_LOGWARN("Failed to move \"%s\" to \"%s\" ! Error %i (0x%X)", pszFrom, pszTo, iRes, iRes);
      return false;
    }

    return true;
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
    // TODO : find a better way !
    bool bRes = FileSystem::copyFile(pszFrom, pszTo, true);
    if (bRes)
      FileSystem::remove(pszFrom);
    return bRes;
  }
  #else
  #error Unsupported platform !
  #endif
}

//---------------------------------------------------------------------------
StringA FileSystem::getTempDirectory (void)
{
  static StringA strSystemTempDir;

  if (!strSystemTempDir.isEmpty())
    return strSystemTempDir;


  #if defined(GLB_PLATFORM_WINDOWS)
  {
    char szTempDir[512];

    if (!::GetTempPathA(sizeof(szTempDir)-1, (char*)&szTempDir))
    {
      szTempDir[0] = '.';
      szTempDir[1] = 0;
    }
    else
    {
      // since the GetTempPath() function does not check if directory does
      // exist, create this directory by ourself if needed
      if (!FileSystem::doesExist((char*)&szTempDir))
        FileSystem::createDirectory((char*)&szTempDir);
    }

    strSystemTempDir = (char*)&szTempDir;
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
    const char* c_szEnvVars[] = { "TMP", "TMPDIR", "TEMP", NULL };

    for (int Ix=0 ; c_szEnvVars[Ix] ; ++Ix)
    {
      const char* pszDir = getenv(c_szEnvVars[Ix]);
      if (pszDir)
      {
        strSystemTempDir = pszDir;
        break;
      }
    }
    if (strSystemTempDir.isEmpty())
      strSystemTempDir = "/tmp";

    if (!FileSystem::doesExist(strSystemTempDir))
      FileSystem::createDirectory(strSystemTempDir);
  }
  #else
  #error Unsupported platform !
  #endif


  return strSystemTempDir;
}

//---------------------------------------------------------------------------
StringA FileSystem::createTempDirectory (const char* pszPrefix/*=""*/)
{
  StringA strDir = FileSystem::createTempFile(pszPrefix);

  FileSystem::remove(strDir);

  if (!FileSystem::createDirectory(strDir))
    GLB_THROW(EXCODE_FILE_CREATE, "Failed to create temporary directory \"%s\" !", strDir.c_str());

  return strDir;
}

//---------------------------------------------------------------------------
StringA FileSystem::createTempFile (const char* pszPrefix/*=""*/, const char* pszExtension/*=""*/)
{
  StringA strTempDir   = FileSystem::getTempDirectory();
  StringA strPrefix    = pszPrefix;
  StringA strExtension = pszExtension;

  strExtension.trimWhitespaces();


  #if defined(GLB_PLATFORM_WINDOWS)
  {
    char szTempFile[1024];
    int  nFile;

    if (strPrefix.isEmpty())
      strPrefix = GLB_BRAND_NAME;

    // create temp file name
    ::GetTempFileNameA(strTempDir.c_str(), strPrefix.c_str(), 1, (char*)&szTempFile);
    if (!strExtension.isEmpty())
    {
      // replace extension
      size_t nPos = strlen((char*)&szTempFile) - 1;
      while (szTempFile[nPos] != '.') nPos--;
      szTempFile[++nPos] = '\0';
      strcat((char*)&szTempFile, strExtension.c_str());
    }

    // touch file
    nFile = _open(
      (char*)&szTempFile,
      _O_CREAT | _O_TRUNC | _O_RDWR,
      _S_IREAD | _S_IWRITE);
    _close(nFile);

    return StringA((char*)&szTempFile);
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
    //
    // TODO
    // * Test THIS !!!!!!!!!!!
    //
    //#error TODO : Test THIS !!!!!!!!

    char szTempFile[1024];
    uint Ix = 0;
    int  nFile;

    do
    {
      snprintf((char*)&szTempFile, sizeof(szTempFile)-1,
        "%s/%s%05u.%s"
        , strTempDir.c_str()
        , (pszPrefix && pszPrefix[0]) ? pszPrefix : GLB_BRAND_NAME
        , ++Ix
        , (pszExtension && pszExtension[0]) ? pszExtension : "tmp" );
    }
    while (FileSystem::doesExist((char*)&szTempFile));

    // touch file
    nFile = open(
      (char*)&szTempFile,
      O_CREAT | O_TRUNC | O_RDWR,
      S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    close(nFile);

    return StringA((char*)&szTempFile);
  }
  #else
  #error Unsupported platform !
  #endif
}

//---------------------------------------------------------------------------
uint32 FileSystem::extractContent (const char* pszFilePath, byte* pBuffer, uint32 uiBufferSize)
{
  FILE*  pFile;
  size_t nRes;
  uint32 uiRead = 0;

  // check parameters
  GLB_ASSERT(pszFilePath);
  GLB_ASSERT(pszFilePath[0]);
  GLB_ASSERT(pBuffer);
  GLB_ASSERT(uiBufferSize);

  // open file
  pFile = fopen(pszFilePath, "rb");
  if (!pFile)
  {
    GLB_LOGERR("Unable to open file \"%s\" !", pszFilePath);
    return 0;
  }

  // read bytes
  while ((uiRead < uiBufferSize) && ((nRes = fread(pBuffer, 1, (uiBufferSize - uiRead), pFile)) > 0))
  {
    uiRead  += (uint32)nRes;
    pBuffer += nRes;
  }

  // close file
  fclose(pFile);

  return uiRead;
}

//---------------------------------------------------------------------------
#if defined(GLB_PLATFORM_WINDOWS)
int FileSystem::winIsUtcFileSystem (const char* pszPath)
{
  char szFsName[32];

  GLB_ASSERT(pszPath);
  if (!pszPath)
    return -1;
  GLB_ASSERT(pszPath[0]);
  if (!pszPath[0])
    return -1;

  StringA strPath(pszPath);
  StringA strRootPath;

  strPath.pathExpand();
  strPath.pathExtractRoot(strRootPath);
  if (strRootPath.isEmpty())
    return -1;

  if (!strRootPath.pathHasTrailingSeparator())
    strRootPath += '\\';

  if (!::GetVolumeInformationA(strRootPath.c_str(), NULL, 0, NULL, NULL, NULL, (char*)&szFsName, sizeof(szFsName)))
    return -1;

  return (!strcmp(szFsName, "NTFS") || !strcmp(szFsName, "HPFS") || !strcmp(szFsName, "OWFS"));
}
#endif

//---------------------------------------------------------------------------
#if defined(GLB_PLATFORM_WINDOWS)
time_t FileSystem::winUtcFileTimeToUnixTime (const void* pFileTimeStructUTC)
{
  const FILETIME& ftUTC = *((const FILETIME*)pFileTimeStructUTC);
  FILETIME      ftUnixEpoch;
  SYSTEMTIME    stUnixEpoch;
  LARGE_INTEGER liUnixEpoch;
  LARGE_INTEGER liUTC;

  // unix epoch is 1970-01-01 at midnight
  stUnixEpoch.wYear         = 1970;
  stUnixEpoch.wMonth        = 1;
  stUnixEpoch.wDayOfWeek    = 0;
  stUnixEpoch.wDay          = 1;
  stUnixEpoch.wHour         = 0;
  stUnixEpoch.wMinute       = 0;
  stUnixEpoch.wSecond       = 0;
  stUnixEpoch.wMilliseconds = 0;

  // get unix epoch in filetime format
  if (!::SystemTimeToFileTime(&stUnixEpoch, &ftUnixEpoch))
  {
    GLB_ASSERT(0); // this should never happen
    return 0;
  }

  // convert unix epoch filetime into a single int64
  // * msdn says : "It is not recommended that you add and subtract values
  //   from the FILETIME structure to obtain relative times. Instead, you
  //   should copy the low- and high-order parts of the file time to a
  //   LARGE_INTEGER structure, perform 64-bit arithmetic on the QuadPart
  //   member, and copy the LowPart and HighPart members into the FILETIME
  //   structure."
  liUnixEpoch.LowPart  = ftUnixEpoch.dwLowDateTime;
  liUnixEpoch.HighPart = ftUnixEpoch.dwHighDateTime;

  // align the given filetime to the unix epoch
  liUTC.LowPart   = ftUTC.dwLowDateTime;
  liUTC.HighPart  = ftUTC.dwHighDateTime;
  liUTC.QuadPart -= liUnixEpoch.QuadPart;
  liUTC.QuadPart /= 10000000; // 1 second = 10000000 * 100 nanoseconds

  // return diff
  if (sizeof(time_t) == sizeof(int32))
  {
    GLB_ASSERT(liUTC.HighPart == 0);
    return (time_t)liUTC.LowPart;
  }
  else if (sizeof(time_t) == sizeof(int64))
  {
    return (time_t)liUTC.QuadPart;
  }
  else
  {
    GLB_ASSERT(0 && "This should never happen !");
    return 0;
  }
}
#endif


} // namespace glb
