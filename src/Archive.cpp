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
// Created On : 2011-11-03 14:43:18
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
#ifdef GLOBE_VFS
namespace glb {


//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------
#ifdef GLB_DEBUG
//# define ARCHIVE_DEBUG_IO
#endif

static const glb_byte c_aMagicRef[Archive::HEADER_MAGIC_SIZE] =
{
  'G', 'L', 'B', 'A', 'R', 'C',
};

static const glb_byte c_aMasterXorKey[255] =
{
  0xB1, 0x97, 0xA7, 0x50, 0x3E, 0xC5, 0xBE, 0x84, 0x21, 0x1A, 0x5C, 0x12,
  0xC0, 0x3B, 0xCE, 0x60, 0xD2, 0x2B, 0x61, 0x47, 0x97, 0xF5, 0x9F, 0x5D,
  0x1A, 0xD8, 0xE9, 0x1F, 0x28, 0xB3, 0x72, 0x08, 0x4F, 0x5B, 0x3A, 0x01,
  0xBA, 0x9C, 0x44, 0x10, 0xBE, 0x38, 0xF1, 0xF7, 0xC1, 0x3E, 0x84, 0x31,
  0x5A, 0x86, 0x2B, 0x90, 0x08, 0x01, 0x31, 0x9A, 0x04, 0x15, 0x24, 0xB4,
  0x3D, 0x32, 0x6C, 0x5C, 0x0A, 0xB6, 0xA9, 0x7C, 0x8F, 0xC3, 0x0C, 0x28,
  0x15, 0xAD, 0x2E, 0x34, 0x64, 0xD9, 0x8B, 0xEC, 0x26, 0x80, 0x7F, 0x7C,
  0x9D, 0xAA, 0x23, 0x28, 0x88, 0x88, 0x94, 0x9E, 0x8D, 0x50, 0xE9, 0x4D,
  0xB2, 0x4D, 0xE7, 0xD1, 0x59, 0x4A, 0x57, 0x72, 0x48, 0xA2, 0x83, 0xE9,
  0x31, 0x1F, 0x61, 0x70, 0x28, 0x26, 0xAF, 0xAD, 0x9B, 0x1D, 0x67, 0xF2,
  0x63, 0x87, 0x82, 0x87, 0x20, 0x8F, 0x8B, 0x2B, 0x84, 0xAE, 0x45, 0x3F,
  0x00, 0x8A, 0x5F, 0x11, 0x22, 0x26, 0xC7, 0xBC, 0x64, 0xF7, 0x30, 0xD1,
  0xC1, 0x00, 0xA6, 0x74, 0xA3, 0x5A, 0xBA, 0x96, 0x77, 0xD3, 0x71, 0x2C,
  0x05, 0x40, 0x72, 0x7F, 0x35, 0x1E, 0x02, 0x14, 0x4C, 0x36, 0x79, 0xFC,
  0xE4, 0xA7, 0x32, 0x86, 0x93, 0x86, 0x0A, 0xEB, 0xF5, 0x71, 0xF3, 0xC0,
  0x4C, 0x3F, 0x39, 0x78, 0xD5, 0xC5, 0x8E, 0x22, 0x17, 0xD4, 0xBA, 0x59,
  0xDE, 0x72, 0x49, 0xFD, 0x92, 0x36, 0x8F, 0x8D, 0x90, 0x38, 0xFA, 0xBB,
  0x02, 0x03, 0x04, 0x5E, 0xE3, 0x05, 0x0E, 0x0C, 0x6D, 0xF3, 0xFD, 0x66,
  0xCE, 0x5F, 0x50, 0xA0, 0xF0, 0xEC, 0xC3, 0x38, 0xB6, 0xD2, 0x84, 0x63,
  0xC0, 0xED, 0x76, 0xD3, 0xED, 0xE3, 0x93, 0x66, 0x2B, 0x98, 0xAA, 0x7F,
  0x69, 0xA2, 0xD3, 0x22, 0x11, 0x60, 0xE6, 0x56, 0x39, 0xCE, 0xF6, 0x83,
  0xCF, 0x97, 0xF8,
};




//---------------------------------------------------------------------------
Archive::Archive (void)
{
  m_unzipFileFunc = new zlib_filefunc_def;
  ((zlib_filefunc_def*)m_unzipFileFunc)->zopen_file  = Archive::onUnzOpenS;
  ((zlib_filefunc_def*)m_unzipFileFunc)->zread_file  = Archive::onUnzReadS;
  ((zlib_filefunc_def*)m_unzipFileFunc)->zwrite_file = Archive::onUnzWriteS;
  ((zlib_filefunc_def*)m_unzipFileFunc)->ztell_file  = Archive::onUnzTellS;
  ((zlib_filefunc_def*)m_unzipFileFunc)->zseek_file  = Archive::onUnzSeekS;
  ((zlib_filefunc_def*)m_unzipFileFunc)->zclose_file = Archive::onUnzCloseS;
  ((zlib_filefunc_def*)m_unzipFileFunc)->zerror_file = Archive::onUnzTestErrorS;
  ((zlib_filefunc_def*)m_unzipFileFunc)->opaque      = this;

  m_hUnzip = NULL;

  memset(&m_Header, 0, sizeof(m_Header));
  m_uiXorKeyPos = 0;
}

//---------------------------------------------------------------------------
Archive::~Archive (void)
{
  this->close();
  delete m_unzipFileFunc;
}




//---------------------------------------------------------------------------
void Archive::close (void)
{
  if (m_hUnzip)
  {
    unzClose((unzFile)m_hUnzip);
    m_hUnzip = NULL;
  }

  GLB_ASSERT(!m_Arch.isOpen());
  memset(&m_Header, 0, sizeof(m_Header));
  m_uiXorKeyPos = 0;

  m_Entries.clear();
}

//---------------------------------------------------------------------------
void Archive::open (const char* pszArchive)
{
  this->close();

  // open zip archive
  m_strPath = pszArchive;
  m_hUnzip = unzOpen2(pszArchive, (zlib_filefunc_def*)m_unzipFileFunc);
  if (!m_hUnzip)
    GLB_THROW(EXCODE_UNKNOWN, "Failed to open archive %s !", pszArchive);

  // list entries
  {
    int iRes;
    char szName[1024];
    unz_file_info unzfi;
    Entry entry;

    iRes = unzGoToFirstFile((unzFile)m_hUnzip);
    if (iRes != UNZ_OK)
      GLB_THROW(EXCODE_UNKNOWN, "Could not find first entry of archive %s (code %i) !", pszArchive, iRes);

    do
    {
      iRes = unzGetCurrentFileInfo((unzFile)m_hUnzip, &unzfi, (char*)&szName, sizeof(szName), NULL, 0, NULL, 0);
      if (iRes != UNZ_OK)
        GLB_THROW(EXCODE_UNKNOWN, "Unable to read entry name from archive %s (code %i) !", pszArchive, iRes);

      entry.strName = (char*)&szName;
      entry.uiSize  = unzfi.uncompressed_size;

      iRes = unzGetFilePos((unzFile)m_hUnzip, (unz_file_pos*)&entry.unzPos);
      if (iRes != UNZ_OK)
        GLB_THROW(EXCODE_UNKNOWN, "Unable to get entry position from archive %s (code %i) !", pszArchive, iRes);

      m_Entries.push_back(entry);
    }
    while((iRes = unzGoToNextFile((unzFile)m_hUnzip)) == UNZ_OK);

    if (iRes != UNZ_END_OF_LIST_OF_FILE)
      GLB_THROW(EXCODE_UNKNOWN, "Failed to read next entry of archive %s (code %i) !", pszArchive, iRes);
  }
}

//---------------------------------------------------------------------------
bool Archive::hasEntry (const char* pszName) const
{
  GLB_ASSERT(this->isOpen());
  return this->getEntry(pszName) != NULL;
}

//---------------------------------------------------------------------------
uint32 Archive::getEntrySize (const char* pszName) const
{
  GLB_ASSERT(this->isOpen());
  const Entry* pEntry = this->getEntry(pszName);
  GLB_ASSERT(pEntry);
  return pEntry ? pEntry->uiSize : uint32(-1);
}

//---------------------------------------------------------------------------
uint32 Archive::extractEntry (const char* pszName, glb_byte* pOutBuffer, uint32 uiBufferSize) const
{
  int    iRes;
  uint32 uiToRead;
  uint32 uiTotalRead = 0;

  GLB_ASSERT(this->isOpen());
  GLB_ASSERT(pOutBuffer);
  GLB_ASSERT(uiBufferSize > 0);
  if (!uiBufferSize)
    return 0;

  const Entry* pEntry = this->getEntry(pszName);
  GLB_FORCEASSERT(pEntry);

  iRes = unzGoToFilePos((unzFile)m_hUnzip, (unz_file_pos*)&pEntry->unzPos);
  if (iRes != UNZ_OK)
    GLB_THROW(EXCODE_UNKNOWN, "Failed to seek to entry %s in archive %s (code %i) !", pszName, m_strPath.c_str(), iRes);

  iRes = unzOpenCurrentFile((unzFile)m_hUnzip);
  if (iRes != UNZ_OK)
    GLB_THROW(EXCODE_UNKNOWN, "Failed to open entry %s from archive %s (code %i) !", pszName, m_strPath.c_str(), iRes);

  do
  {
    // the return value type of unzReadCurrentFile() is int but a zip archive
    // can handle uint32 sized files so the following line allows us to avoid
    // overflow in case the entry size is bigger than max<int>()
    uiToRead = std::min<uint32>((uint32)std::numeric_limits<int>::max(), uiBufferSize);
    GLB_ASSERT(uiToRead <= uiBufferSize);

    iRes = unzReadCurrentFile((unzFile)m_hUnzip, pOutBuffer, uiToRead);
    if (iRes < 0)
      GLB_THROW(EXCODE_UNKNOWN, "Failed to read entry %s from archive %s (code %i) !", pszName, m_strPath.c_str(), iRes);

    uiTotalRead  += (uint32)iRes;
    uiBufferSize -= (uint32)iRes;
  }
  while (iRes > 0 && uiBufferSize > 0);

  iRes = unzCloseCurrentFile((unzFile)m_hUnzip);
  if (iRes != UNZ_OK)
    GLB_THROW(EXCODE_UNKNOWN, "Failed to close entry %s in archive %s (code %i) !", pszName, m_strPath.c_str(), iRes);

  return uiTotalRead;
}

//---------------------------------------------------------------------------
glb_byte* Archive::extractEntry (const char* pszName, uint32* puiOutSize/*=NULL*/) const
{
  glb_byte* pContent;

  GLB_ASSERT(this->isOpen());

  const Entry* pEntry = this->getEntry(pszName);
  GLB_FORCEASSERT(pEntry);

  pContent = (glb_byte*)malloc(pEntry->uiSize);
  if (Archive::extractEntry(pszName, pContent, pEntry->uiSize) != pEntry->uiSize)
    GLB_THROW(EXCODE_UNKNOWN, "Failed to extract entry %s from archive %s !", pszName, m_strPath.c_str());

  if (puiOutSize)
    *puiOutSize = pEntry->uiSize;

  return pContent;
}

//---------------------------------------------------------------------------
void Archive::extractEntryToFile (const char* pszName, const char* pszDestFile) const
{
  uint32    uiSize = 0;
  glb_byte* pBuffer = this->extractEntry(pszName, &uiSize);
  FileDisk  f;

  f.open(pszDestFile, File::FILE_FLAG_WRITE | File::FILE_FLAG_CREATE_ALWAYS);
  f.write(pBuffer, uiSize);
  f.close();

  free(pBuffer);
}




//---------------------------------------------------------------------------
bool Archive::isArchive (const char* pszFile)
{
  if (FileSystem::getSize(pszFile) > sizeof(Archive::Header))
  {
    FileDisk  f;
    glb_byte  aMagic[HEADER_MAGIC_SIZE];
    glb_uint8 cVersion;

    f.open(pszFile, File::FILE_FLAG_READ | File::FILE_FLAG_SHARE_READ);
    if (f.isOpen() &&
      f.read(&aMagic, sizeof(aMagic)) == sizeof(aMagic) &&
      f.read(&cVersion, sizeof(cVersion)) == sizeof(cVersion) &&
      !memcmp(&aMagic, &c_aMagicRef, sizeof(aMagic)) &&
      cVersion == HEADER_VERSION)
    {
      return true;
    }
  }

  return false;
}

//---------------------------------------------------------------------------
void Archive::extractZipFile (const char* pszArchive, const char* pszOutFile)
{
  FileDisk fin;
  FileDisk fout;
  Header   header;
  glb_byte aBuffer[8 * 1024];
  uint     uiSize;
  uint     uiKeyPos = 0;

  {
    Archive arch;
    arch.open(pszArchive);
    memcpy(&header, &arch.m_Header, sizeof(Header));
  }

  fout.open(pszOutFile, File::FILE_FLAG_WRITE | File::FILE_FLAG_CREATE_ALWAYS);
  fin.open(pszArchive, File::FILE_FLAG_READ | File::FILE_FLAG_SHARE_READ);
  fin.seek(File::FILE_SEEK_BEGIN, sizeof(Header));

  while ((uiSize = fin.read(&aBuffer, sizeof(aBuffer))) > 0)
  {
    if (header.cXorKeySize > 0)
      uiKeyPos = Archive::xorBuffer((glb_byte*)&aBuffer, uiSize, (glb_byte*)header.aXorKey, (uint)header.cXorKeySize, uiKeyPos);
    fout.write(&aBuffer, uiSize);
  }
}

//---------------------------------------------------------------------------
#ifdef GLOBE_ARCHIVE_CANCREATE
void Archive::create (const char* pszArchive, StringAVec vecFiles, bool bRecursive, const glb_byte* pXorKey/*=NULL*/, uint uiXorKeySize/*=0*/)
{
  typedef std::map<StringA,StringA> ArchFiles; // <name_in_arch,src_file>
  typedef ArchFiles::iterator       ArchFilesIt;

  StringA   strTempFile;
  ArchFiles mapArchFiles;

  if (FileSystem::doesExist(pszArchive))
    GLB_THROW(EXCODE_PARAM, "Archive file already exists : \"%s\" !", pszArchive);
  if (vecFiles.empty())
    GLB_THROW(EXCODE_PARAM, "No files to insert into archive file : \"%s\" !", pszArchive);

  strTempFile  = pszArchive;
  strTempFile += ".tmp";
  if (FileSystem::doesExist(strTempFile.c_str()))
    GLB_THROW(EXCODE_PARAM, "Temporary file already exists : \"%s\" !", strTempFile.c_str());

  // check the list of files, and populate directories if needed
  {
    StringAVec vecDirs;

    for (size_t i = 0; i < vecFiles.size(); ++i)
    {
      if (FileSystem::doesExist(vecFiles[i]))
      {
        if (FileSystem::isDirectory(vecFiles[i]))
        {
          vecDirs.push_back(vecFiles[i]);
        }
        else
        {
          mapArchFiles.insert(ArchFiles::value_type(
            vecFiles[i].pathExtractName(),
            vecFiles[i]));
        }
      }
    }

    for (size_t iDir = 0; iDir < vecDirs.size(); ++iDir)
    {
      vecFiles.clear();
      if (FileIterator::getFilesList(vecDirs[iDir], bRecursive, vecFiles, FileIterator::QUERY_FILES) > 0)
      {
        for (size_t iFile = 0; iFile < vecFiles.size(); ++iFile)
        {
          StringA strNameInArch;

          strNameInArch = vecFiles[iFile].substr((uint)vecDirs[iDir].length());
          while (StringA::charIsPathSeparator(strNameInArch[0]))
            strNameInArch.erase(0, 1);

          mapArchFiles.insert(ArchFiles::value_type(
            strNameInArch,
            vecFiles[iFile]));
        }
      }
    }

    //for (ArchFilesIt itF = mapArchFiles.begin(); itF != mapArchFiles.end(); ++itF)
    //  GLB_LOGDBG("Arch file : %s, %s", itF->first.c_str(), itF->second.c_str());
  }

  // create temporary zip archive
  {
    zipFile hZip = zipOpen(strTempFile, APPEND_STATUS_CREATE);
    if (!hZip)
      GLB_THROW(EXCODE_PARAM, "Cannot create ZIP archive \"%s\" !", strTempFile.c_str());

    for (ArchFilesIt itF = mapArchFiles.begin(); itF != mapArchFiles.end(); ++itF)
    {
      const StringA& strName = itF->first;
      const StringA& strFile = itF->second;

      uint64    uiSize64 = FileSystem::getSize(strFile);
      uint32    uiSize   = (uint32)uiSize64;
      glb_byte* pContent;
      int       iRes;

      if (uiSize64 > std::numeric_limits<uint32>::max())
        GLB_THROW(EXCODE_PARAM, "Input file is too big : \"%s\" !", strFile.c_str());

      {
        DateTime     dt(FileSystem::getModifTime(strFile));
        const tm*    pTm = dt.getTm(true);
        zip_fileinfo zipfi;

        GLB_FORCEASSERT((pTm->tm_year + 1900) >= 1980 && (pTm->tm_year + 1900) <= 2044);

        memset(&zipfi, 0, sizeof(zipfi));
        zipfi.tmz_date.tm_sec  = pTm->tm_sec;
        zipfi.tmz_date.tm_min  = pTm->tm_min;
        zipfi.tmz_date.tm_hour = pTm->tm_hour;
        zipfi.tmz_date.tm_mday = pTm->tm_mday;
        zipfi.tmz_date.tm_mon  = pTm->tm_mon;
        zipfi.tmz_date.tm_year = pTm->tm_year;

        iRes = zipOpenNewFileInZip(hZip, strName.c_str(), &zipfi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_BEST_COMPRESSION);
        if (iRes != ZIP_OK)
          GLB_THROW(EXCODE_UNKNOWN, "Failed to zipOpenNewFileInZip() (code %i) !", iRes);
      }

      pContent = (glb_byte*)malloc(uiSize);
      if (FileSystem::extractContent(strFile.c_str(), pContent, uiSize) != uiSize)
        GLB_THROW(EXCODE_UNKNOWN, "Failed to extract content of \"%s\" into memory !", strFile.c_str());

      iRes = zipWriteInFileInZip(hZip, pContent, uiSize);
      if (iRes != ZIP_OK)
        GLB_THROW(EXCODE_UNKNOWN, "Failed to zipWriteInFileInZip() (code %i) !", iRes);

      iRes = zipCloseFileInZip(hZip);
      if (iRes != ZIP_OK)
        GLB_THROW(EXCODE_UNKNOWN, "Failed to zipCloseFileInZip() (code %i) !", iRes);

      free(pContent);
    }

    zipClose(hZip, NULL);
  }

  // write the final file
  Archive::createFromZip(pszArchive, strTempFile.c_str(), pXorKey, uiXorKeySize);

  // delete the temporary zip archive
  FileSystem::remove(strTempFile.c_str());
}
#endif // #ifdef GLOBE_ARCHIVE_CANCREATE

//---------------------------------------------------------------------------
#ifdef GLOBE_ARCHIVE_CANCREATE
void Archive::createFromZip (const char* pszArchive, const char* pszZipFile, const glb_byte* pXorKey/*=NULL*/, uint8 cXorKeySize/*=0*/)
{
  if (FileSystem::doesExist(pszArchive))
    GLB_THROW(EXCODE_PARAM, "Archive file already exists : \"%s\" !", pszArchive);

  // check future size of the archive
  if (sizeof(Archive::Header) + FileSystem::getSize(pszZipFile) > std::numeric_limits<long>::max())
    GLB_THROW(EXCODE_PARAM, "Archive file will be too big \"%s\" !", pszArchive);

  // create output archive file
  FileDisk f(pszArchive, File::FILE_FLAG_WRITE | File::FILE_FLAG_CREATE_NEW);

  // prepare and write header
  {
    Header header;

    // setup header
    memcpy(&header.aMagic, &c_aMagicRef, HEADER_MAGIC_SIZE);
    header.cVersion    = HEADER_VERSION;
    header.cXorKeySize = cXorKeySize;
    Random::getBytesSecured((glb_byte*)&header.aXorKey, sizeof(header.aXorKey));
    if (cXorKeySize > 0)
      memcpy(&header.aXorKey, pXorKey, (size_t)cXorKeySize);

    // encrypt header
    Archive::xorBuffer(
      (glb_byte*)&header + HEADER_MAGIC_SIZE + 1,
      sizeof(header) - (HEADER_MAGIC_SIZE + 1),
      (glb_byte*)&c_aMasterXorKey,
      sizeof(c_aMasterXorKey));

    f.write(&header, sizeof(header));
  }

  // write zip file content into output archive
  {
    FileDisk infile(pszZipFile, File::FILE_FLAG_READ | File::FILE_FLAG_SHARE_READ);
    glb_byte aBuffer[8 * 1024];
    uint32   uiSize;
    uint     uiKeyPos = 0;

    while ((uiSize = infile.read(&aBuffer, sizeof(aBuffer))) > 0)
    {
      if (cXorKeySize > 0)
        uiKeyPos = Archive::xorBuffer((glb_byte*)&aBuffer, uiSize, pXorKey, (uint)cXorKeySize, uiKeyPos);
      f.write(&aBuffer, uiSize);
    }
  }
}
#endif // #ifdef GLOBE_ARCHIVE_CANCREATE




//---------------------------------------------------------------------------
const Archive::Entry* Archive::getEntry (const char* pszName) const
{
  for (EntriesCIt itE = m_Entries.begin(); itE != m_Entries.end(); ++itE)
  {
    const Entry& entry = *itE;
    if (entry.strName.equalsI(pszName))
      return &entry;
  }

  return NULL;
}




//---------------------------------------------------------------------------
uint Archive::xorBuffer (glb_byte* pBuffer, uint32 uiBufferSize, const glb_byte* pKey, uint uiKeySize, uint uiKeyPos/*=0*/)
{
  GLB_ASSERT(pBuffer);
  GLB_ASSERT(uiBufferSize);
  GLB_ASSERT(pKey);
  GLB_ASSERT(uiKeySize);
  //GLB_ASSERT(uiKeyPos < uiKeySize);

  for ( ; uiBufferSize-- ; ++pBuffer, ++uiKeyPos)
    *pBuffer = *pBuffer ^ pKey[uiKeyPos % uiKeySize];

  return uiKeyPos;
}




//---------------------------------------------------------------------------
void* Archive::onUnzOpenS (void* pOpaque, const char* pszFile, int iMode)
{
  Archive* pThis = (Archive*)pOpaque;

  GLB_ASSERT(!pThis->isOpen());
  GLB_ASSERT((iMode & ZLIB_FILEFUNC_MODE_READWRITEFILTER) == ZLIB_FILEFUNC_MODE_READ);
  if (!Archive::isArchive(pszFile))
    GLB_THROW(EXCODE_PARAM, "File is not a valid Archive \"%s\" !", pszFile);
  if (FileSystem::getSize(pszFile) > std::numeric_limits<long>::max())
    GLB_THROW(EXCODE_PARAM, "Archive file is too big \"%s\" !", pszFile);

  // open archive
  pThis->m_Arch.open(pszFile, File::FILE_FLAG_READ | File::FILE_FLAG_SHARE_READ);

  // read, check and decrypt header
  if (pThis->m_Arch.read(&pThis->m_Header, sizeof(pThis->m_Header)) != sizeof(pThis->m_Header))
    GLB_THROW("Failed to read archive \"%s\" !", pszFile);
  if (pThis->m_Header.cVersion != HEADER_VERSION)
    GLB_THROW(EXCODE_PARAM, "Archive file format version #%u is not supported \"%s\" !", (uint)pThis->m_Header.cVersion, pszFile);
  Archive::xorBuffer(
    (glb_byte*)&pThis->m_Header + HEADER_MAGIC_SIZE + 1,
    sizeof(pThis->m_Header) - (HEADER_MAGIC_SIZE + 1),
    (glb_byte*)&c_aMasterXorKey,
    sizeof(c_aMasterXorKey));

  // seek to the begining of the zip archive
  //pThis->m_Arch.seek(File::FILE_SEEK_BEGIN, sizeof(Archive::Header));

#ifdef ARCHIVE_DEBUG_IO
  GLB_LOGDBG("onUnzOpenS(%s) [xorkey:%u]", pszFile, (uint)pThis->m_Header.cXorKey);
#endif

  return (void*)1;
}

//---------------------------------------------------------------------------
ulong Archive::onUnzRead (void* pBuffer, ulong uiSize)
{
#ifdef ARCHIVE_DEBUG_IO
  GLB_LOGDBG("onUnzRead(%u)", uiSize);
#endif

  uiSize = (ulong)m_Arch.read(pBuffer, (uint64)uiSize);
  if (m_Header.cXorKeySize > 0)
    m_uiXorKeyPos = Archive::xorBuffer((glb_byte*)pBuffer, (uint32)uiSize, (glb_byte*)&m_Header.aXorKey, (uint)m_Header.cXorKeySize, m_uiXorKeyPos);

#ifdef ARCHIVE_DEBUG_IO
  //GLB_LOGDBG("onUnzRead() = 0x%02X", ((char*)pBuffer)[0]);
#endif

  return uiSize;
}

//---------------------------------------------------------------------------
long Archive::onUnzTell (void)
{
  long iRes = (long)m_Arch.getPosition(); // safe cast because we checked the file size before
  GLB_ASSERT(iRes >= 0);
  GLB_ASSERT(iRes >= sizeof(Archive::Header));
  iRes -= sizeof(Archive::Header);
  m_uiXorKeyPos = (uint)iRes;

#ifdef ARCHIVE_DEBUG_IO
  GLB_LOGDBG("onUnzTell() = %i", iRes + sizeof(Archive::Header));
#endif

  return iRes;
}

//---------------------------------------------------------------------------
long Archive::onUnzSeek (ulong uiOffset, int iOrigin)
{
  File::SeekOrigin eSeek;
  long iRes;

#ifdef ARCHIVE_DEBUG_IO
  GLB_LOGDBG("onUnzSeek(%u, %i)", uiOffset, iOrigin);
#endif

  switch (iOrigin)
  {
    case ZLIB_FILEFUNC_SEEK_CUR :
      eSeek = File::FILE_SEEK_CURRENT;
      break;
    case ZLIB_FILEFUNC_SEEK_END :
      eSeek = File::FILE_SEEK_END;
      break;
    case ZLIB_FILEFUNC_SEEK_SET :
      eSeek = File::FILE_SEEK_BEGIN;
      uiOffset += sizeof(Archive::Header);
      break;
    default :
      GLB_THROW(EXCODE_PARAM, "Unknown iOrigin parameter %i !", iOrigin);
  }

  iRes = (long)m_Arch.seek(eSeek, (int64)uiOffset); // safe cast (return value) because we checked the file size before
  GLB_ASSERT(iRes >= 0);
  GLB_ASSERT(iRes >= sizeof(Archive::Header));

  m_uiXorKeyPos = (uint)iRes - sizeof(Archive::Header);

#ifdef ARCHIVE_DEBUG_IO
  GLB_LOGDBG("onUnzSeek() = %i", iRes);
#endif

  return 0;
}

//---------------------------------------------------------------------------
int Archive::onUnzClose (void)
{
  GLB_ASSERT(m_Arch.isOpen());
  m_Arch.close();

#ifdef ARCHIVE_DEBUG_IO
  GLB_LOGDBG("onUnzClose()");
#endif

  return 0;
}

//---------------------------------------------------------------------------
int Archive::onUnzTestError (void)
{
#ifdef ARCHIVE_DEBUG_IO
  GLB_LOGDBG("onUnzTestError()");
#endif
  return 0;
}




//---------------------------------------------------------------------------
ulong Archive::onUnzReadS (void* pOpaque, void* pStream, void* pBuffer, ulong uiSize)
{
  return ((Archive*)pOpaque)->onUnzRead(pBuffer, uiSize);
}

//---------------------------------------------------------------------------
ulong Archive::onUnzWriteS (void* pOpaque, void* pStream, const void* pBuffer, ulong uiSize)
{
  GLB_ASSERT(0);
  return 0;
}

//---------------------------------------------------------------------------
long Archive::onUnzTellS (void* pOpaque, void* pStream)
{
  return ((Archive*)pOpaque)->onUnzTell();
}

//---------------------------------------------------------------------------
long Archive::onUnzSeekS (void* pOpaque, void* pStream, ulong uiOffset, int iOrigin)
{
  return ((Archive*)pOpaque)->onUnzSeek(uiOffset, iOrigin);
}

//---------------------------------------------------------------------------
int Archive::onUnzCloseS (void* pOpaque, void* pStream)
{
  return ((Archive*)pOpaque)->onUnzClose();
}

//---------------------------------------------------------------------------
int Archive::onUnzTestErrorS (void* pOpaque, void* pStream)
{
  return ((Archive*)pOpaque)->onUnzTestError();
}


} // namespace glb
#endif // #ifdef GLOBE_VFS
