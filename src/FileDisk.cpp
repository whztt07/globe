//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-10 20:37:53
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
FileDisk::FileDisk (void)
: File()
{
  // properties
  m_pMappedBuffer        = NULL;
  m_uiMappedBufferLength = 0;

  // handles
#if defined(GLB_PLATFORM_WINDOWS)
  m_hFile    = INVALID_HANDLE_VALUE;
  m_hFileMap = NULL;
#else
  m_pFile = NULL;
#endif
}

//---------------------------------------------------------------------------
FileDisk::FileDisk (const char* pszFilePath, flags32 dwOpenFlags)
: File()
{
  // properties
  m_pMappedBuffer        = NULL;
  m_uiMappedBufferLength = 0;

  // handles
#if defined(GLB_PLATFORM_WINDOWS)
  m_hFile    = INVALID_HANDLE_VALUE;
  m_hFileMap = NULL;
#else
  m_pFile = NULL;
#endif

  // open
  this->open(pszFilePath, dwOpenFlags);
}

//---------------------------------------------------------------------------
FileDisk::~FileDisk (void)
{
  this->close();
}


//---------------------------------------------------------------------------
void FileDisk::close (void)
{
  // unmap file if needed
  if (this->isBufferRequested())
  {
    GLB_ASSERT(0);

    #if defined(GLB_PLATFORM_WINDOWS)
    {
      GLB_ASSERT(m_pMappedBuffer);
      if (m_pMappedBuffer)
        ::UnmapViewOfFile(m_pMappedBuffer);
      if (m_hFileMap)
      {
        ::CloseHandle(m_hFileMap);
        m_hFileMap = NULL;
      }
    }
    #else
    {
      GLB_ASSERT(m_pMappedBuffer && m_uiMappedBufferLength);
      if (m_pMappedBuffer && m_uiMappedBufferLength)
        munmap(m_pMappedBuffer, (size_t)m_uiMappedBufferLength);
    }
    #endif

    m_pMappedBuffer        = NULL;
    m_uiMappedBufferLength = 0;

    m_dwBufferAccessMode = BUFFERACCESSMODE_NONE;
  }

  // close file
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    if (m_hFile != INVALID_HANDLE_VALUE)
    {
      ::CloseHandle(m_hFile);
      m_hFile = INVALID_HANDLE_VALUE;
    }
  }
  #else
  {
    if (m_pFile)
    {
      ::fclose(m_pFile);
      m_pFile = NULL;
    }
  }
  #endif

  // clear path
  m_strPath.clear();

  // close parent
  File::close();
}

//---------------------------------------------------------------------------
void FileDisk::open (const char* pszFilePath, flags32 dwOpenFlags, uint32 dwUnused1/*=0*/, uint32 dwUnused2/*=0*/)
{
  GLB_ASSERT(pszFilePath);
  GLB_ASSERT(!this->isOpen()); // encourage good programming practice in debug mode :)

  // check open flags
  if (!(dwOpenFlags & FILE_FLAG_READWRITE))
    GLB_THROW(EXCODE_PARAM, "Open Flags must contain a Read/Write information (given flags : 0x%X) !", dwOpenFlags);

  // remove the truncate flag if we are not in write mode
  if (!(dwOpenFlags & FILE_FLAG_WRITE))
    dwOpenFlags &= ~FILE_FLAG_TRUNCATE;

  // close file first
  this->close();

  // open file
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    DWORD dwAccess;
    DWORD dwCreationDisposition;
    DWORD dwShareMode;

    // access mode
    dwAccess = 0;
    switch (dwOpenFlags & FILE_FLAG_READWRITE)
    {
      case FILE_FLAG_READ :
        dwAccess = GENERIC_READ;
        break;
      case FILE_FLAG_WRITE :
        dwAccess = GENERIC_WRITE;
        break;
      case FILE_FLAG_READWRITE :
        dwAccess = GENERIC_READ | GENERIC_WRITE;
        break;
      default :
        GLB_ASSERT(0);
        break;
    }

    // share mode
    dwShareMode = 0;
    if (dwOpenFlags & FILE_FLAG_SHARE_READWRITE)
      dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
    else if (dwOpenFlags & FILE_FLAG_SHARE_WRITE)
      dwShareMode = FILE_SHARE_WRITE;
    else if (dwOpenFlags & FILE_FLAG_SHARE_READ)
      dwShareMode = FILE_SHARE_READ;

    // creation disposition
    if ((dwOpenFlags & FILE_FLAG_CREATE_NEW) && (dwOpenFlags & FILE_FLAG_TRUNCATE))
      dwCreationDisposition = CREATE_ALWAYS;
    else if (dwOpenFlags & FILE_FLAG_CREATE_NEW)
      dwCreationDisposition = CREATE_NEW;
    else if (dwOpenFlags & FILE_FLAG_TRUNCATE)
      dwCreationDisposition = TRUNCATE_EXISTING;
    else
      dwCreationDisposition = OPEN_EXISTING;

    // open file
    m_hFile = ::CreateFileA(pszFilePath, dwAccess, 0, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
    if (m_hFile == INVALID_HANDLE_VALUE)
    {
      if (System::lastError() == ERROR_FILE_NOT_FOUND)
        GLB_THROW(EXCODE_FILE_DOESNTEXIST, "Unable to open file \"%s\" ! File does not exist.", pszFilePath);
      else
        GLB_THROW(EXCODE_FILE_OPEN, "Unable to open file \"%s\" ! Error %u : %s", pszFilePath, System::lastError(), System::lastErrorString());
    }
  }
  #else
  {
    char szMode[6]       = "\0";
    bool bFailIfNotExist = false;

    // open mode
    switch (dwOpenFlags & FILE_FLAG_READWRITE)
    {
      case FILE_FLAG_READ :
        if (dwOpenFlags & FILE_FLAG_CREATE_NEW)
        {
          strcpy((char*)&szMode, "a+");
        }
        else
        {
          strcpy((char*)&szMode, "r");
          bFailIfNotExist = false; // this is not a mistake : let the fopen() function to do that
        }
        break;

      case FILE_FLAG_WRITE :
        if ((dwOpenFlags & FILE_FLAG_CREATE_NEW) && (dwOpenFlags & FILE_FLAG_TRUNCATE))
        {
          strcpy((char*)&szMode, "w");
        }
        else if (dwOpenFlags & FILE_FLAG_CREATE_NEW)
        {
          strcpy((char*)&szMode, "a");
        }
        else if (dwOpenFlags & FILE_FLAG_TRUNCATE)
        {
          strcpy((char*)&szMode, "w");
          bFailIfNotExist = true;
        }
        else
        {
          strcpy((char*)&szMode, "a");
          bFailIfNotExist = true;
        }
        break;

      case FILE_FLAG_READWRITE :
        if ((dwOpenFlags & FILE_FLAG_CREATE_NEW) && (dwOpenFlags & FILE_FLAG_TRUNCATE))
        {
          strcpy((char*)&szMode, "w+");
        }
        else if (dwOpenFlags & FILE_FLAG_CREATE_NEW)
        {
          strcpy((char*)&szMode, "a+");
        }
        else if (dwOpenFlags & FILE_FLAG_TRUNCATE)
        {
          strcpy((char*)&szMode, "w+");
          bFailIfNotExist = true;
        }
        else
        {
          strcpy((char*)&szMode, "a+");
          bFailIfNotExist = true;
        }
        break;

      default :
        GLB_ASSERT(0);
        break;
    }

    // check if file does exist
    if (bFailIfNotExist && !FileSystem::DoesExist(pszFilePath))
      GLB_THROW(EXCODE_FILE_DOESNTEXIST, "File \"%s\" does not exist !", pszFilePath);

    // always open in binary mode
    strcat((char*)&szMode, "b");

    // open file
    m_pFile = fopen(pszFilePath, (char*)&szMode);
    if (!m_pFile)
    {
      if (errno == ENOENT)
        GLB_THROW(EXCODE_FILE_DOESNTEXIST, "Unable to open file \"%s\" ! File does not exist.", pszFilePath);
      else
        GLB_THROW(EXCODE_FILE_OPEN, "Unable to open file \"%s\" ! Error %u : %s", pszFilePath, System::lastError(), System::lastErrorString());
    }
  }
  #endif

  // setup common properties
  m_strPath     = pszFilePath;
  m_dwOpenFlags = dwOpenFlags;
}

//---------------------------------------------------------------------------
uint32 FileDisk::read (void* pBuffer, uint32 uiLength)
{
  uint32 uiBytesRead = 0;

  GLB_ASSERT(this->isReadable());
  GLB_ASSERT(!this->isBufferRequested());

  GLB_ASSERT(pBuffer && uiLength);
  if (!uiLength)
    return 0;

  #if defined(GLB_PLATFORM_WINDOWS)
  {
    GLB_ASSERT(m_hFile != INVALID_HANDLE_VALUE);

    if (::ReadFile(m_hFile, pBuffer, uiLength, (DWORD*)&uiBytesRead, NULL) == 0)
      GLB_THROW(EXCODE_FILE_READING, "Error while reading file \"%s\" ! Error %u : %s", m_strPath.c_str(), System::lastError(), System::lastErrorString());
  }
  #else
  {
    GLB_ASSERT(m_pFile);

    uiBytesRead = (uint)fread(pBuffer, 1, (size_t)uiLength, m_pFile);
    if ((uiBytesRead == 0) && !feof(m_pFile))
      GLB_THROW(EXCODE_FILE_READING, "Error while reading file \"%s\" ! Error %u : %s", m_strPath.c_str(), System::lastError(), System::lastErrorString());
  }
  #endif

  return uiBytesRead;
}

//---------------------------------------------------------------------------
void FileDisk::write (const void* pBuffer, uint32 uiLength)
{
  GLB_ASSERT(this->isWritable());
  GLB_ASSERT(!this->isBufferRequested());

  GLB_ASSERT(pBuffer && uiLength);
  if (!uiLength)
    return;

  #if defined(GLB_PLATFORM_WINDOWS)
  {
    uint uiBytesWritten = 0;

    GLB_ASSERT(m_hFile != INVALID_HANDLE_VALUE);

    if (!::WriteFile(m_hFile, pBuffer, uiLength, (DWORD*)&uiBytesWritten, NULL))
      GLB_THROW(EXCODE_FILE_WRITING, "Error while writing to file \"%s\" ! Error %u : %s", m_strPath.c_str(), System::lastError(), System::lastErrorString());

    if (uiBytesWritten != uiLength)
      GLB_THROW(EXCODE_FILE_DISKFULL, "Error while writing to file \"%s\" !", m_strPath.c_str());
  }
  #else
  {
    GLB_ASSERT(m_pFile);

    if (fwrite(pBuffer, 1, (size_t)uiLength, m_pFile) != (size_t)uiLength)
      GLB_THROW(EXCODE_FILE_WRITING, "Error while writing to file \"%s\" ! Error %u : %s", m_strPath.c_str(), System::lastError(), System::lastErrorString());
  }
  #endif
}

//---------------------------------------------------------------------------
void FileDisk::flush (void)
{
  GLB_ASSERT(this->isWritable());
  GLB_ASSERT(!this->isBufferRequested());

  #if defined(GLB_PLATFORM_WINDOWS)
  {
    GLB_ASSERT(m_hFile != INVALID_HANDLE_VALUE);
    ::FlushFileBuffers(m_hFile);
  }
  #else
  {
    GLB_ASSERT(m_pFile);
    fflush(m_pFile);
  }
  #endif
}

//---------------------------------------------------------------------------
uint64 FileDisk::seek (SeekOrigin eOrigin, int64 nOffset)
{
  GLB_ASSERT(!this->isBufferRequested());

  #if defined(GLB_PLATFORM_WINDOWS)
  {
    typedef BOOL (WINAPI *PSETFILEPOINTEREX)(HANDLE, LARGE_INTEGER, PLARGE_INTEGER, DWORD);

    static PSETFILEPOINTEREX pfnSetFilePointerEx =
      (PSETFILEPOINTEREX)::GetProcAddress(::LoadLibraryA("kernel32.dll"), "SetFilePointerEx");

    LARGE_INTEGER li;
    DWORD         dwMoveMethod;

    GLB_ASSERT(m_hFile != INVALID_HANDLE_VALUE);

    switch (eOrigin)
    {
      case File::FILE_SEEK_BEGIN :
        dwMoveMethod = FILE_BEGIN;
        break;
      case File::FILE_SEEK_CURRENT :
        dwMoveMethod = FILE_CURRENT;
        break;
      case File::FILE_SEEK_END :
        GLB_FORCEASSERT((nOffset <= 0) && "Offset must be negative when seeking from FILE_SEEK_END !");
        dwMoveMethod = FILE_END;
        break;
      default :
        GLB_THROWEXCODE(EXCODE_PARAM);
        break;
    }

    if (pfnSetFilePointerEx)
    {
      LARGE_INTEGER liDist;

      liDist.QuadPart = nOffset;
      if (!pfnSetFilePointerEx(m_hFile, liDist, &li, dwMoveMethod))
        goto __Error;
    }
    else
    {
      li.QuadPart = nOffset;
      li.LowPart  = ::SetFilePointer(m_hFile, li.LowPart, &li.HighPart, dwMoveMethod);
      if ((li.LowPart == INVALID_SET_FILE_POINTER) && (::GetLastError() != NO_ERROR))
        goto __Error;

      return li.QuadPart;
    }

    return li.QuadPart;

    __Error :
    GLB_THROW(EXCODE_FILE_SEEKING, "Error while seeking in file \"%s\" ! Error %u : %s", m_strPath.c_str(), System::lastError(), System::lastErrorString());
  }
  #else
  {
    GLB_ASSERT(m_pFile);
    if ((eOrigin == File::FILE_SEEK_END) && (nOffset > 0))
      GLB_THROW(EXCODE_PARAM, "Offset must be negative when seeking from FILE_SEEK_END !");

    if (fseeko(m_pFile, nOffset, (int)eOrigin) != 0)
      GLB_THROW(EXCODE_FILE_SEEKING, "Error while seeking in file \"%s\" ! Error %u : %s", m_strPath.c_str(), System::lastError(), System::lastErrorString());

    return ftello(m_pFile);
  }
  #endif
}

//---------------------------------------------------------------------------
uint64 FileDisk::getPosition (void) const
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    LARGE_INTEGER li;

    GLB_ASSERT(m_hFile != INVALID_HANDLE_VALUE);

    li.QuadPart = 0;
    li.LowPart  = ::SetFilePointer(m_hFile, li.LowPart, &li.HighPart, FILE_CURRENT);
    if ((li.LowPart == (DWORD)-1) && (::GetLastError() != NO_ERROR))
      GLB_THROW(EXCODE_SYSTEM_API, "Error while getting position in file \"%s\" ! Error %u : %s", m_strPath.c_str(), System::lastError(), System::lastErrorString());

    return li.QuadPart;
  }
  #else
  {
    off_t uiPos;

    GLB_ASSERT(m_pFile);

    uiPos = ftello(m_pFile);
    if (uiPos == (off_t)-1)
      GLB_THROW(EXCODE_SYSTEM_API, "Error while getting position in file \"%s\" ! Error %u : %s", m_strPath.c_str(), System::lastError(), System::lastErrorString());

    return uiPos;
  }
  #endif
}

//---------------------------------------------------------------------------
uint64 FileDisk::getLength (void) const
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    ULARGE_INTEGER uli;

    GLB_ASSERT(m_hFile != INVALID_HANDLE_VALUE);

    uli.HighPart = 0;
    uli.LowPart  = ::GetFileSize(m_hFile, &uli.HighPart);
    if ((uli.LowPart == (DWORD)-1) && (::GetLastError() != NO_ERROR))
      GLB_THROW(EXCODE_SYSTEM_API, "Error while getting size of open file \"%s\" ! Error %u : %s", m_strPath.c_str(), System::lastError(), System::lastErrorString());

    return uli.QuadPart;
  }
  #elif defined(GLB_PLATFORM_CYGWIN)
  {
    off_t  uiInitialPos;
    uint64 uiFileLength;

    GLB_ASSERT(m_pFile);

    uiInitialPos = ftello(m_pFile);
    if (uiInitialPos == (off_t)-1)
      goto __ThrowError;

    if (fseeko(m_pFile, 0, SEEK_END) != 0)
      goto __ThrowError;

    uiFileLength = (uint64)ftello(m_pFile);
    if (uiFileLength == (uint64)-1)
      goto __ThrowError;

    if (fseeko(m_pFile, uiInitialPos, SEEK_SET) != 0)
      goto __ThrowError;

    return uiFileLength;

    __ThrowError :
    GLB_THROW(EXCODE_SYSTEM_API, "Error while getting size of open file \"%s\" ! Error %u : %s", m_strPath.c_str(), System::lastError(), System::lastErrorString());
  }
  #else
  {
    struct stat64 stats;

    GLB_ASSERT(m_pFile);

    if (::fstat64(::fileno(m_pFile), &stats) != 0)
      GLB_THROW(EXCODE_SYSTEM_API, "Error while getting size of open file \"%s\" ! Error %u : %s", m_strPath.c_str(), System::lastError(), System::lastErrorString());

    return stats.st_size;
  }
  #endif
}

//---------------------------------------------------------------------------
void FileDisk::setLength (uint64 uiNewLength)
{
  GLB_ASSERT(this->isWritable());
  GLB_ASSERT(!this->isBufferRequested());

  #if defined(GLB_PLATFORM_WINDOWS)
  {
    GLB_ASSERT(m_hFile != INVALID_HANDLE_VALUE);

    this->seek(File::FILE_SEEK_BEGIN, uiNewLength);
    if (!::SetEndOfFile(m_hFile))
      GLB_THROW(EXCODE_SYSTEM_API, "Error while setting size of file \"%s\" to %"PRIu64" bytes ! Error %u : %s", m_strPath.c_str(), uiNewLength, System::lastError(), System::lastErrorString());
  }
  #else
  {
    GLB_ASSERT(m_pFile);

    if (fseeko(m_pFile, uiNewLength, SEEK_SET) != 0)
      GLB_THROW(EXCODE_SYSTEM_API, "Error while setting size of file \"%s\" to %"PRIu64" bytes ! Error %u : %s", m_strPath.c_str(), uiNewLength, System::lastError(), System::lastErrorString());

    // seek to begin to imitate the windows' implementation
    this->seekToBegin();
  }
  #endif
}

//---------------------------------------------------------------------------
uint32 FileDisk::bufferAccess (flags32 dwBufferAccessMode, uint32 uiLength/*=0*/, byte** ppBufferBegin/*=NULL*/, byte** ppBufferEnd/*=NULL*/)
{
  flags32 dwCommand = dwBufferAccessMode & BUFFERACCESSMODE_MASK_COMMAND;

  GLB_ASSERT(this->isOpen());
  GLB_FORCEASSERT(this->validateBufferAccessMode(dwBufferAccessMode));


  #if defined(GLB_PLATFORM_WINDOWS)
  {
    GLB_ASSERT(m_hFile != INVALID_HANDLE_VALUE);

    // buffer request
    if ((dwCommand == BUFFERACCESSMODE_COMMAND_READONLY) || (dwCommand == BUFFERACCESSMODE_COMMAND_READWRITE))
    {
      DWORD          dwPageAccess;
      DWORD          dwMapAccess;
      ULARGE_INTEGER uiOffset;
      uint64         uiFileLength;

      GLB_FORCEASSERT(ppBufferBegin && ppBufferEnd && "Invalid parameters !");

      switch (m_dwOpenFlags & FILE_FLAG_READWRITE)
      {
        case FILE_FLAG_READ :
          dwPageAccess = PAGE_READONLY;
          dwMapAccess  = FILE_MAP_READ;
          break;
        case FILE_FLAG_WRITE :
        case FILE_FLAG_READWRITE :
          dwPageAccess = PAGE_READWRITE;
          dwMapAccess  = FILE_MAP_WRITE;  // means read/write
          break;
      }

      // determine offset
      uiOffset.QuadPart = this->getPosition();

      // reduce requested length if ot goes beyond the current file boundaries
      uiFileLength = this->getLength();
      if ((uiOffset.QuadPart + static_cast<uint64>(uiLength)) > uiFileLength)
        uiLength = static_cast<uint32>(uiFileLength - uiOffset.QuadPart);

      // create file mapping
      m_hFileMap = ::CreateFileMapping(m_hFile, NULL, dwPageAccess, 0, uiLength, NULL);
      if (!m_hFileMap)
        goto __ThrowError;

      // map file
      m_pMappedBuffer = static_cast<byte*>(::MapViewOfFile(m_hFileMap, dwMapAccess, uiOffset.HighPart, uiOffset.LowPart, (SIZE_T)uiLength));
      if (!m_pMappedBuffer)
        goto __ThrowError;

      // update properties
      m_dwBufferAccessMode   = dwBufferAccessMode;
      m_uiMappedBufferLength = uiLength;

      // result values
      *ppBufferBegin = m_pMappedBuffer;
      *ppBufferEnd   = m_pMappedBuffer + uiLength;

      return uiLength;
    }

    // release request
    else if (dwCommand == BUFFERACCESSMODE_COMMAND_COMMIT)
    {
      ::UnmapViewOfFile(m_pMappedBuffer);
      m_pMappedBuffer        = NULL;
      m_uiMappedBufferLength = 0;

      if (m_hFileMap != NULL)
      {
        ::CloseHandle(m_hFileMap);
        m_hFileMap = NULL;
      }

      // seek
      GLB_ASSERT((this->getPosition() + static_cast<uint64>(uiLength)) <= this->getLength());
      this->seek(File::FILE_SEEK_CURRENT, static_cast<int64>(uiLength));

      // update properties
      m_dwBufferAccessMode = BUFFERACCESSMODE_NONE;

      return 0;
    }

  }
  #else
  {
    GLB_ASSERT(m_pFile);

    // buffer request
    if ((dwCommand == BUFFERACCESSMODE_COMMAND_READONLY) || (dwCommand == BUFFERACCESSMODE_COMMAND_READWRITE))
    {
      int    nProt = 0;
      off_t  uiOffset;
      uint64 uiFileLength;

      GLB_FORCEASSERT(ppBufferBegin && ppBufferEnd && "Invalid parameters !");

      switch (m_dwOpenFlags & FILE_FLAG_READWRITE)
      {
        case FILE_FLAG_READ :
          nProt = PROT_READ;
          break;
        case FILE_FLAG_WRITE :
          nProt = PROT_WRITE;
          break;
        case FILE_FLAG_READWRITE :
          nProt = PROT_READ | PROT_WRITE;
          break;
        default :
          GLB_ASSERT(0);
          break;
      }

      uiOffset     = this->getPosition();
      uiFileLength = this->getLength();

      // reduce requested length if ot goes beyond the current file boundaries
      if ((uiOffset + static_cast<uint64>(uiLength)) > uiFileLength)
        uiLength = static_cast<uint32>(uiFileLength - uiOffset);

      // map
      m_pMappedBuffer = static_cast<byte*>(mmap(NULL, (size_t)uiLength, nProt, MAP_SHARED, fileno(m_pFile), uiOffset));
      if (m_pMappedBuffer == MAP_FAILED)
        goto __ThrowError;

      // update properties
      m_dwBufferAccessMode   = dwBufferAccessMode;
      m_uiMappedBufferLength = uiLength;

      // result values
      *ppBufferBegin = m_pMappedBuffer;
      *ppBufferEnd   = m_pMappedBuffer + uiLength;

      return uiLength;
    }

    // release request
    else if (dwCommand == BUFFERACCESSMODE_COMMAND_COMMIT)
    {
      GLB_ASSERT(m_pMappedBuffer && m_uiMappedBufferLength);
      if (m_pMappedBuffer && m_uiMappedBufferLength)
        munmap(m_pMappedBuffer, (size_t)m_uiMappedBufferLength);

      // update properties
      m_pMappedBuffer        = NULL;
      m_uiMappedBufferLength = 0;

      // update common properties
      m_dwBufferAccessMode = BUFFERACCESSMODE_NONE;

      return 0;
    }
  }
  #endif


__ThrowError :
  GLB_THROW(EXCODE_FILE_MAPPING, "Error while mapping file \"%s\" into memory ! Error %u : %s", m_strPath.c_str(), System::lastError(), System::lastErrorString());
}


} // namespace glb
