//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-29 20:26:13
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_VFS_H__
#define __GLOBE_VFS_H__
#ifdef GLOBE_VFS
namespace glb {


//---------------------------------------------------------------------------
// VFS
//---------------------------------------------------------------------------
class GLB_EXPORT VFS : public Singleton<VFS>
{
public :
  typedef struct File
  {
    StringA  strVirtPath;   // virtual path (case insensitive)
    StringA  strPhysPath;   // expanded (full) physical path of the file (or of the parent archive if file is archived)
    Archive* pArchive;      // if file is inside an archive
    StringA  strNameInArch; // how to address the entry in the parent archive
    uint     uiSize;        // uncompressed size

    glb_byte* pContent;    // if file has been acquired into memory
    StringA   strContent;  // if file has been acquired into the physical file system
  };

  typedef std::map<StringA,File> Files;    // key : virtual path
  typedef Files::iterator        FilesIt;
  typedef Files::const_iterator  FilesCIt;

  typedef std::vector<Archive*>    Archives;
  typedef Archives::iterator       ArchivesIt;
  typedef Archives::const_iterator ArchivesCIt;


public :
  VFS (void);
  ~VFS (void);

  // singleton stuff
  static VFS& getSingleton    (void);
  static VFS* getSingletonPtr (void);

  // directories
  void reset  (void); // free acquired files as well
  void addDir (const char* pszDirOrArchive); // don't forget to scanDirs() after

  // (re)scan directories
  // WARNING : this will cleanup the internal list of files and the acquired
  // files as well  !
  void scanDirs (bool bRecursive=true);

  // files (case insensitive search)
  bool       hasFile            (const char* pszVirtPath) const;
  bool       isArchivedFile     (const char* pszVirtPath) const;
  uint       getFileSize        (const char* pszVirtPath) const;
  StringAVec findFiles          (const char* pszVirtPathMask) const;
  StringA    extractPhysFiles   (const char* pszVirtPathMask); // extract multiple files to a physical temp dir (note : it is up to the caller to destroy the created temp dir)
  bool       hasAcquiredFiles   (void) const;
  uint       countAcquiredFiles (void) const;
  StringA    acquirePhysFile    (const char* pszVirtPath);
  glb_byte*  acquireMemFile     (const char* pszVirtPath, uint32* puiOutSize=NULL);
  void       releaseFile        (const char* pszVirtPath);
  void       releasePhysFile    (const StringA& strAcquiredFile);
  void       releaseMemFile     (glb_byte* pAcquiredFile);
  void       releaseAllFiles    (void);


private :
  void     scanDir     (StringA strDir, bool bRecursive, uint uiDirLevel, const StringA& strBaseVirtualPath);
  void     scanArchive (const char* pszArchive, const StringA& strBaseVirtualPath);
  FilesIt  findFile    (const char* pszVirtPath);
  FilesCIt findFile    (const char* pszVirtPath) const;
  void     releaseFile (File& f);
  void     resetFiles  (void);


private :
  StringAVec m_vecDirs;
  Files      m_Files;
  Archives   m_Archives; // opened archives
};


#include "VFS.inl.hpp"

static inline VFS& g_VFS()  { return VFS::getSingleton(); }
static inline VFS* g_pVFS() { return VFS::getSingletonPtr(); }


} // namespace glb
#endif // #ifdef GLOBE_VFS
#endif // #ifndef __GLOBE_VFS_H__
