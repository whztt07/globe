//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-29 20:26:19
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
#ifdef GLOBE_VFS
namespace glb {

GLB_IMPLEMENT_SINGLETON(VFS)


//---------------------------------------------------------------------------
VFS::VFS (void)
{
}

//---------------------------------------------------------------------------
VFS::~VFS (void)
{
  this->reset();
}




//---------------------------------------------------------------------------
void VFS::reset (void)
{
  this->resetFiles();
  m_vecDirs.clear();
}

//---------------------------------------------------------------------------
void VFS::addDir (const char* pszDirOrArchive)
{
  StringA strDir(pszDirOrArchive);

  if (!FileSystem::doesExist(strDir))
  {
    strDir.pathStripTrailingSeparator();
    strDir.pathBackSlashesToSlashes();
  }
  else
  {
    if (!FileSystem::isDirectory(strDir) && !Archive::isArchive(strDir))
      GLB_THROW(EXCODE_PARAM, "Only directories or archives can be added to the VFS (file : %s) !", strDir.c_str());

    strDir.pathExpand();
    strDir.pathStripTrailingSeparator();
  }

  m_vecDirs.push_back(strDir);
}

//---------------------------------------------------------------------------
void VFS::scanDirs (bool bRecursive/*=true*/)
{
  this->resetFiles();

  for (size_t i = 0; i < m_vecDirs.size(); ++i)
  {
    if (FileSystem::doesExist(m_vecDirs[i]))
    {
      if (FileSystem::isDirectory(m_vecDirs[i]))
        this->scanDir(m_vecDirs[i], bRecursive, 0, "");
      else if (Archive::isArchive(m_vecDirs[i]))
        this->scanArchive(m_vecDirs[i], "");
    }
  }

  //for (FilesCIt itF = m_Files.begin(); itF != m_Files.end(); ++itF)
  //  GLB_LOGDBG("VFS File : %s", itF->first.c_str());
}

//---------------------------------------------------------------------------
void VFS::scanDir (StringA strDir, bool bRecursive, uint uiDirLevel, const StringA& strBaseVirtualPath)
{
  FileIterator fileIt;
  FileIterator::FileInfo fi;

  fi.fQueryFlags = FileIterator::QUERY_TYPE | FileIterator::QUERY_SIZE;
  if (!fileIt.openDir(strDir.c_str(), fi))
  {
    if (fileIt.isEmpty())
      return;
    GLB_THROW(EXCODE_PARAM, "Failed to scan directory \"%s\" !", strDir.c_str());
  }

  strDir.pathExpand();
  strDir.pathStripTrailingSeparator();
  strDir.pathBackSlashesToSlashes();

  do
  {
    StringA strFile(strDir / fi.strName);

    if (fi.eType == FileIterator::TYPE_DIRECTORY)
    {
      if (bRecursive)
      {
        this->scanDir(
          strFile,
          bRecursive,
          uiDirLevel + 1,
          strBaseVirtualPath / fi.strName);
      }
    }
    else if (fi.eType == FileIterator::TYPE_FILE)
    {
      if (Archive::isArchive(strFile))
      {
        this->scanArchive(strFile, strBaseVirtualPath);
      }
      else
      {
        VFS::File f;

        if (fi.uiSize > (uint64)std::numeric_limits<uint32>::max())
          GLB_THROW(EXCODE_UNKNOWN, "File \"%s\" is too big !", strFile.c_str());

        f.strVirtPath = strBaseVirtualPath / fi.strName;
        f.strPhysPath = strFile;
        f.pArchive    = NULL;
        f.uiSize      = (uint32)fi.uiSize;
        f.pContent    = NULL;

        f.strVirtPath.pathBackSlashesToSlashes();

#ifdef GLB_DEBUG
        if (this->findFile(f.strVirtPath) != m_Files.end())
          GLB_LOGDBG("File with the same virtual path already exists in VFS (%s) ! File entry will be replaced.", f.strVirtPath.c_str());
#endif
        m_Files.insert(Files::value_type(f.strVirtPath, f));
      }
    }
  }
  while (fileIt.getNext(fi));
}

//---------------------------------------------------------------------------
void VFS::scanArchive (const char* pszArchive, const StringA& strBaseVirtualPath)
{
  Archive* pArch = new Archive();

  pArch->open(pszArchive);
  const Archive::Entries& entries = pArch->getEntriesList();

  for (size_t i = 0; i < entries.size(); ++i)
  {
    VFS::File f;

    f.strVirtPath   = strBaseVirtualPath / entries[i].strName;
    f.strPhysPath   = pszArchive;
    f.pArchive      = pArch;
    f.strNameInArch = entries[i].strName;
    f.uiSize        = entries[i].uiSize;
    f.pContent      = NULL;

    f.strVirtPath.pathBackSlashesToSlashes();

#ifdef GLB_DEBUG
    if (this->findFile(f.strVirtPath) != m_Files.end())
      GLB_LOGDBG("File with the same virtual path already exists in VFS (%s) ! File entry will be replaced.", f.strVirtPath.c_str());
#endif
    m_Files.insert(Files::value_type(f.strVirtPath, f));
  }

  m_Archives.push_back(pArch);
}




//---------------------------------------------------------------------------
bool VFS::hasFile (const char* pszVirtPath) const
{
  FilesCIt itF = this->findFile(pszVirtPath);
  return itF != m_Files.end();
}

//---------------------------------------------------------------------------
bool VFS::isArchivedFile (const char* pszVirtPath) const
{
  FilesCIt itF = this->findFile(pszVirtPath);
  if (itF == m_Files.end())
    GLB_THROW(EXCODE_PARAM, "File not found in VFS : \"%s\" !", pszVirtPath);
  return itF->second.pArchive != NULL;
}

//---------------------------------------------------------------------------
uint VFS::getFileSize (const char* pszVirtPath) const
{
  FilesCIt itF = this->findFile(pszVirtPath);
  if (itF == m_Files.end())
    GLB_THROW(EXCODE_PARAM, "File not found in VFS : \"%s\" !", pszVirtPath);
  return itF->second.uiSize;
}

//---------------------------------------------------------------------------
StringAVec VFS::findFiles (const char* pszVirtPathMask) const
{
  StringAVec vecFound;

  for (FilesCIt itF = m_Files.begin(); itF != m_Files.end(); ++itF)
  {
    if (itF->first.matchI(pszVirtPathMask))
      vecFound.push_back(itF->first);
  }

  return vecFound;
}

//---------------------------------------------------------------------------
StringA VFS::extractPhysFiles (const char* pszVirtPathMask)
{
  StringA    strDir;
  StringAVec vecVirtPaths;

  vecVirtPaths = this->findFiles(pszVirtPathMask);
  if (vecVirtPaths.empty())
    return StringA();

  strDir = FileSystem::createTempDirectory();
  if (!FileSystem::doesExist(strDir))
    GLB_THROW(EXCODE_FILE_DOESNTEXIST, "Failed to create temporary directory !");

  for (size_t i = 0; i < vecVirtPaths.size(); ++i)
  {
    FilesIt itF = this->findFile(vecVirtPaths[i]);
    File&   f = itF->second;
    StringA strDest;

    strDest = strDir / vecVirtPaths[i];
    strDest.pathStripName();
    if (!FileSystem::createDirectory(strDest, true))
      goto __rollback;

    strDest = strDir / vecVirtPaths[i];

    if (!f.pArchive)
    {
      bool bWasAcquired = !f.strContent.isEmpty();

      if (!bWasAcquired)
      {
        (void)this->acquirePhysFile(vecVirtPaths[i]);
        FileSystem::moveFile(f.strContent, strDest);
        this->releasePhysFile(f.strContent);
      }
      else
      {
        FileSystem::copyFile(f.strContent, strDest);
      }
    }
    else
    {
      FileSystem::copyFile(f.strPhysPath, strDest);
    }
  }

  return strDir;


__rollback :
  if (!strDir.isEmpty())
    FileSystem::remove(strDir, true);
  return StringA();
}

//---------------------------------------------------------------------------
bool VFS::hasAcquiredFiles (void) const
{
  for (FilesCIt itF = m_Files.begin(); itF != m_Files.end(); ++itF)
  {
    if (itF->second.pContent || !itF->second.strContent.isEmpty())
      return true;
  }

  return false;
}

//---------------------------------------------------------------------------
uint VFS::countAcquiredFiles (void) const
{
  uint uiCount = 0;

  for (FilesCIt itF = m_Files.begin(); itF != m_Files.end(); ++itF)
  {
    if (itF->second.pContent || !itF->second.strContent.isEmpty())
      ++uiCount;
  }

  return uiCount;
}

//---------------------------------------------------------------------------
StringA VFS::acquirePhysFile (const char* pszVirtPath)
{
  FilesIt itF = this->findFile(pszVirtPath);
  if (itF == m_Files.end())
    GLB_THROW(EXCODE_PARAM, "File not found in VFS : \"%s\" !", pszVirtPath);

  File& file = itF->second;

  if (file.strContent.isEmpty())
  {
    if (!file.pArchive)
    {
      return file.strPhysPath;
    }
    else
    {
      // here, we want to create a temporary file inside the phys fs
      file.strContent = FileSystem::createTempFile("", file.strVirtPath.pathExtractExtension());
      file.pArchive->extractEntryToFile(file.strNameInArch, file.strContent);
    }
  }

  GLB_ASSERT(!file.strContent.isEmpty());
  return file.strContent;
}

//---------------------------------------------------------------------------
glb_byte* VFS::acquireMemFile (const char* pszVirtPath, uint32* puiOutSize/*=NULL*/)
{
  FilesIt itF = this->findFile(pszVirtPath);
  if (itF == m_Files.end())
    GLB_THROW(EXCODE_PARAM, "File not found in VFS : \"%s\" !", pszVirtPath);

  File& file = itF->second;

  if (!file.pContent)
  {
    if (file.pArchive)
    {
      file.pContent = file.pArchive->extractEntry(file.strNameInArch, NULL);
    }
    else
    {
      file.pContent = (glb_byte*)malloc(file.uiSize);
      if (FileSystem::extractContent(file.strPhysPath, file.pContent, file.uiSize) != file.uiSize)
        GLB_THROW(EXCODE_UNKNOWN, "Failed to read file %s !", file.strPhysPath.c_str());
    }
  }

  if (puiOutSize)
    *puiOutSize = file.uiSize;

  GLB_ASSERT(file.pContent);
  return file.pContent;
}

//---------------------------------------------------------------------------
void VFS::releaseFile (const char* pszVirtPath)
{
  FilesIt itF = this->findFile(pszVirtPath);
  GLB_ASSERT(itF != m_Files.end());
  if (itF != m_Files.end())
    this->releaseFile(itF->second);
}

//---------------------------------------------------------------------------
void VFS::releasePhysFile (const StringA& strAcquiredFile)
{
  for (FilesIt itF = m_Files.begin(); itF != m_Files.end(); ++itF)
  {
    if (itF->second.strContent == strAcquiredFile)
    {
      this->releaseFile(itF->second);
      return;
    }
  }
}

//---------------------------------------------------------------------------
void VFS::releaseMemFile (glb_byte* pAcquiredFile)
{
  for (FilesIt itF = m_Files.begin(); itF != m_Files.end(); ++itF)
  {
    if (itF->second.pContent == pAcquiredFile)
    {
      this->releaseFile(itF->second);
      return;
    }
  }
}

//---------------------------------------------------------------------------
void VFS::releaseAllFiles (void)
{
  for (FilesIt itF = m_Files.begin(); itF != m_Files.end(); ++itF)
    this->releaseFile(itF->second);
}




//---------------------------------------------------------------------------
VFS::FilesIt VFS::findFile (const char* pszVirtPath)
{
  StringA strFile(pszVirtPath);

  strFile.pathBackSlashesToSlashes();

  for (FilesIt itF = m_Files.begin(); itF != m_Files.end(); ++itF)
  {
    if (itF->first.equalsI(strFile.c_str()))
      return itF;
  }

  return m_Files.end();
}

//---------------------------------------------------------------------------
VFS::FilesCIt VFS::findFile (const char* pszVirtPath) const
{
  StringA strFile(pszVirtPath);

  strFile.pathBackSlashesToSlashes();

  for (FilesCIt itF = m_Files.begin(); itF != m_Files.end(); ++itF)
  {
    if (itF->first.equalsI(strFile.c_str()))
      return itF;
  }

  return m_Files.end();
}

//---------------------------------------------------------------------------
void VFS::releaseFile (VFS::File& file)
{
  if (file.pContent)
  {
    free(file.pContent);
    file.pContent = NULL;
  }

  if (!file.strContent.isEmpty())
  {
    if (FileSystem::doesExist(file.strContent) && !FileSystem::remove(file.strContent))
      GLB_LOGERR("Failed to delete temporary file \"%s\" !", file.strContent.c_str());
    file.strContent.reset();
  }
}

//---------------------------------------------------------------------------
void VFS::resetFiles (void)
{
  this->releaseAllFiles();

  for (size_t i = 0; i < m_Archives.size(); ++i)
    delete m_Archives[i];
  m_Archives.clear();

  m_Files.clear();
}


} // namespace glb
#endif // #ifdef GLOBE_VFS
