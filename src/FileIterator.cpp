//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-29 23:53:48
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
static inline void _FileIterator_ResetInfo (FileIterator::FileInfo& info)
{
  info.strName.clear();
  info.eType      = FileIterator::TYPE_UNKNOWN;
  info.uiSize     = 0;
  info.nModifTime = 0;
}

//---------------------------------------------------------------------------
static void _FileIterator_BuildFilePathIfNeeded (StringA& strFilePath, const StringA& strDir, const StringA& strName)
{
  if (strFilePath.isEmpty())
  {
    strFilePath.grow(strDir.length() + strName.length() + 2, false);
    strFilePath = strDir;
    strFilePath.pathAppend(strName);
  }
}

//---------------------------------------------------------------------------
#if defined(GLB_PLATFORM_WINDOWS)
static void _FileIterator_MapOsDataToFileInfo (FileIterator::FileInfo& info, const StringA& strDir, WIN32_FIND_DATAA& wfd, bool bFsInUtcTime)
{
  StringA strFilePath;

  // name
  info.strName = (char*)&wfd.cFileName;

  // type (always filled-in, this is costless here)
  info.eType =
    (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ?
    FileIterator::TYPE_DIRECTORY :
    FileIterator::TYPE_FILE;

  // reset other members first
  info.uiSize     = 0;
  info.nModifTime = 0;

  // size
  if ((info.eType != FileIterator::TYPE_DIRECTORY) && ((info.fQueryFlags & FileIterator::QUERY_SIZE) != 0))
  {
    _FileIterator_BuildFilePathIfNeeded(strFilePath, strDir, info.strName);
    info.uiSize = FileSystem::getSize(strFilePath);
  }

  // modification time
  if ((info.fQueryFlags & FileIterator::QUERY_MODIFTIME) != 0)
  {
    bFsInUtcTime; // shut up compiler for now...

    //if(bFsInUtcTime)
    {
      info.nModifTime = FileSystem::winUtcFileTimeToUnixTime((void*)&wfd.ftLastWriteTime);
    }
    /*else
    {
      FILETIME ft;

      //GLB_ASSERT(0); // THIS IS A TODO !!!

      // why this FileTimeToLocalFileTime() call ?
      // * because FindFirstFile() converts non-utc fs time into utc time
      //   *only* by taking the *current* dst setting into account, not the
      //   dst setting at saving time.
      // * so we have to revert the translation made by FindFirstFile() and
      //   doing it by ourselves.
      // * "microsoft always makes things easier for developers" (c)
      if (!::FileTimeToLocalFileTime(&wfd.ftLastWriteTime, &ft))
        info.nModifTime = 0;
      else
        info.nModifTime = FileSystem::Win_FileTimeToUnixUtcTime(ft, bFsInUtcTime);

      //
      // TODO
      // * Convert local time to utc time !
      //

      info.nModifTime = FileSystem::winUtcFileTimeToUnixTime(ft);
    }*/
  }
}

#else
static void _FileIterator_MapOsDataToFileInfo (FileIterator::FileInfo& info, const StringA& strDir, const char* pszFileName)
{
#if defined(GLB_PLATFORM_CYGWIN)
  #define STATSTRUCT  struct stat
  #define STATFUNC    ::stat
#else
  #define STATSTRUCT  struct stat64
  #define STATFUNC    ::stat64
#endif

  STATSTRUCT stats;

  // name
  info.strName = pszFileName;

  // reset other members first
  info.eType      = FileIterator::TYPE_UNKNOWN;
  info.uiSize     = 0;
  info.nModifTime = 0;

  // get file stats if needed
  if (info.fQueryFlags != 0)
  {
    StringA strFilePath;

    _FileIterator_BuildFilePathIfNeeded(strFilePath, strDir, info.strName);
    if (STATFUNC(strFilePath, &stats) != 0)
    {
      GLB_ASSERT(0); // error while calling stat() from FileIterator !
      return;
    }
  }

  // type
  if ((info.fQueryFlags & FileIterator::QUERY_TYPE) != 0)
  {
    if (S_ISDIR(stats.st_mode))
      info.eType = FileIterator::TYPE_DIRECTORY;
    else if (S_ISREG(stats.st_mode))
      info.eType = FileIterator::TYPE_FILE;
    else if (S_ISLNK(stats.st_mode))
      info.eType = FileIterator::TYPE_SYMLINK;
  }

  // size
  if ((info.fQueryFlags & FileIterator::QUERY_SIZE) != 0)
  {
    GLB_ASSERT(sizeof(info.uiSize) == sizeof(stats.st_size));
    info.uiSize = stats.st_size;
  }

  // modification time
  if ((info.fQueryFlags & FileIterator::QUERY_MODIFTIME) != 0)
  {
    GLB_ASSERT(sizeof(info.nModifTime) == sizeof(stats.st_mtime));
    info.nModifTime = stats.st_mtime;
  }

#undef STATSTRUCT
#undef STATFUNC
}

#endif




//---------------------------------------------------------------------------
FileIterator::FileIterator (void)
{
  #ifdef GLB_PLATFORM_WINDOWS
  {
    m_pHandle = INVALID_HANDLE_VALUE;
  }
  #else
  {
    m_pHandle = NULL;
  }
  #endif

  m_bEmpty = false;
}

//---------------------------------------------------------------------------
FileIterator::~FileIterator (void)
{
  this->close();
}

//---------------------------------------------------------------------------
void FileIterator::close (void)
{
  #ifdef GLB_PLATFORM_WINDOWS
  {
    if (m_pHandle != INVALID_HANDLE_VALUE)
    {
      ::FindClose(m_pHandle);
      m_pHandle = INVALID_HANDLE_VALUE;
    }
  }
  #else
  {
    if (!m_pHandle)
    {
      closedir((DIR*)m_pHandle);
      m_pHandle = NULL;
    }
  }
  #endif

  m_strDir.reset();
  m_bEmpty = false;
}

//---------------------------------------------------------------------------
bool FileIterator::openDir (const char* pszDirectory, FileIterator::FileInfo& infoFirst)
{
  this->close();

  GLB_ASSERT(pszDirectory);
  if (!pszDirectory)
    return false;
  GLB_ASSERT(pszDirectory[0]);
  if (!pszDirectory[0])
    return false;

  m_strDir = pszDirectory;
  while (m_strDir.pathHasTrailingSeparator())
    m_strDir.erase(-1);

  if (!FileSystem::isDirectory(m_strDir))
  {
    m_strDir.clear();
    return false;
  }

  if ((infoFirst.fQueryFlags & QUERY_DIRS_AND_FILES) == 0)
    infoFirst.fQueryFlags |= QUERY_DIRS_AND_FILES;

  if (!this->getNext(infoFirst))
  {
    m_bEmpty = true;
    return false;
  }

  return true;
}

//---------------------------------------------------------------------------
bool FileIterator::getNext (FileIterator::FileInfo& info)
{
  GLB_ASSERT(this->isOpen());
  if (!this->isOpen() || m_bEmpty)
    return false;

  info.fQueryFlags |= QUERY_TYPE;

  while (1)
  {
    if (!this->getNextImpl(info))
      return false;

    if (info.eType == TYPE_DIRECTORY && (info.fQueryFlags & QUERY_DIRS))
      return true;
    else if (info.eType != TYPE_DIRECTORY && (info.fQueryFlags & QUERY_FILES))
      return true;
  }

  return true;
}

//---------------------------------------------------------------------------
bool FileIterator::getNextImpl (FileIterator::FileInfo& info)
{
  bool bIsFirst = false;

  #ifdef GLB_PLATFORM_WINDOWS
  {
    WIN32_FIND_DATAA wfd;

    if (m_pHandle == INVALID_HANDLE_VALUE)
    {
      bIsFirst = true;

      m_strDir.pathAppend("*");

      m_pHandle = ::FindFirstFileA(m_strDir.c_str(), &wfd);

      if (m_strDir.right(1) == "*")
      {
        m_strDir.erase(-1);
        while (m_strDir.pathHasTrailingSeparator())
          m_strDir.erase(-1);
      }
    }
    else if (!::FindNextFileA(m_pHandle, &wfd))
    {
      _FileIterator_ResetInfo(info);
      return false;
    }

    if (m_pHandle == INVALID_HANDLE_VALUE)
    {
      GLB_LOGERR("Error while trying to iterate in \"%s\" ! Error %lu : %s", m_strDir.c_str(), System::lastError(), System::lastErrorString());
      _FileIterator_ResetInfo(info);
      return false;
    }
    else
    {
      // skip "." and ".." entries
      while (!strcmp((char*)&wfd.cFileName, ".") ||
        !strcmp((char*)&wfd.cFileName, "..") ||
        ((wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && !(info.fQueryFlags & FileIterator::QUERY_HIDDEN)))
      {
        if (!::FindNextFileA(m_pHandle, &wfd))
        {
          if (bIsFirst)
            m_bEmpty = true; // no valid entry found at all
          _FileIterator_ResetInfo(info);
          return false;
        }
      }

      // FindFirstFile() and FindNextFile() automatically convert non-utc
      // times into utc times
      _FileIterator_MapOsDataToFileInfo(info, m_strDir, wfd, true);
    }
  }
  #else
  {
    struct dirent* pDirEntry;

    if (!m_pHandle)
    {
      bIsFirst = true;

      m_pHandle = opendir(m_strDir.c_str());
      if (!m_pHandle)
      {
        GLB_LOGERROR("Error while trying to iterate in \"%s\" ! Error %lu : %s", m_strDir.c_str(), System::lastError(), System::lastErrorString());
        _FileIterator_ResetInfo(info);
        return false;
      }
    }

    GLB_ASSERT(m_pHandle);
    while ((pDirEntry = readdir((DIR*)m_pHandle)) != NULL)
    {
      if (strcmp((char*)&pDirEntry->d_name, ".") && strcmp((char*)&pDirEntry->d_name, ".."))
      {
        if (pDirEntry->d_name[0] != '.' || (info.fQueryFlags & FileIterator::QUERY_HIDDEN))
          break;
      }
    }

    if (!pDirEntry)
    {
      if (bIsFirst)
        m_bEmpty = true; // no valid entry found at all
      _FileIterator_ResetInfo(info);
      return false;
    }

    _FileIterator_MapOsDataToFileInfo(info, m_strDir, (char*)&pDirEntry->d_name);
  }
  #endif

  return true;
}

//---------------------------------------------------------------------------
bool FileIterator::seekToFirst (FileIterator::FileInfo& info)
{
  StringA strDir;

  GLB_ASSERT(this->isOpen());
  if(!this->isOpen())
    return false;

  // the problem :
  // * in the linux implementation, we could simply use the rewinddir()
  //   function to simply seek read pointer to start, but this kind of
  //   behavior is not allowed by the win32 api.
  // * so the only way to get same behavior between multiple platforms is to
  //   close the handle before opening a new one.
  strDir = m_strDir;
  return this->openDir(strDir, info);
}




//---------------------------------------------------------------------------
uint FileIterator::getFilesList (const char* pszDirectory, bool bRecursive, StringAVec& vecOutFiles, flags8 fQueryFilter/*=QUERY_DIRS_AND_FILES*/)
{
  FileIterator fit;
  FileInfo     fi;
  uint         uiCounter = 0;

  fi.fQueryFlags = QUERY_NAMEONLY | (fQueryFilter & QUERYMASK_FILTERS);
  if (bRecursive)
    fi.fQueryFlags |= QUERY_TYPE | QUERY_DIRS;

  if (fit.openDir(pszDirectory, fi))
  {
    do
    {
      if ((fi.eType == TYPE_DIRECTORY && (fQueryFilter & QUERY_DIRS)) ||
        (fi.eType == TYPE_FILE && (fQueryFilter & QUERY_FILES)))
      {
        vecOutFiles.push_back(fit.getDirectory() / fi.strName);
        ++uiCounter;
      }

      if (bRecursive && fi.eType == TYPE_DIRECTORY)
      {
        uiCounter += FileIterator::getFilesList(
          fit.getDirectory() / fi.strName,
          bRecursive,
          vecOutFiles,
          fQueryFilter);
      }
    }
    while (fit.getNext(fi));
  }

  return uiCounter;
}

//---------------------------------------------------------------------------
uint FileIterator::getFilesInfo (const char* pszDirectory, bool bRecursive, std::vector<FileIterator::FileInfo>& vecOutFilesInfo, flags8 fQueryFlags/*=QUERY_DIRS_AND_FILES*/)
{
  FileIterator fit;
  FileInfo     fi;
  uint         uiCounter = 0;

  fi.fQueryFlags = fQueryFlags;
  if (bRecursive)
    fi.fQueryFlags |= QUERY_TYPE | QUERY_DIRS;

  if (fit.openDir(pszDirectory, fi))
  {
    do
    {
      fi.strName = fit.getDirectory() / fi.strName;

      if ((fi.eType == TYPE_DIRECTORY && (fQueryFlags & QUERY_DIRS)) ||
        (fi.eType == TYPE_FILE && (fQueryFlags & QUERY_FILES)))
      {
        vecOutFilesInfo.push_back(fi);
        ++uiCounter;
      }

      if (bRecursive && fi.eType == TYPE_DIRECTORY)
      {
        uiCounter += FileIterator::getFilesInfo(
          fi.strName,
          bRecursive,
          vecOutFilesInfo,
          fQueryFlags);
      }
    }
    while (fit.getNext(fi));
  }

  return uiCounter;
}


} // namespace glb
