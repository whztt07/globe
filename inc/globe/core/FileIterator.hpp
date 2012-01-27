//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-29 23:53:41
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_FILEITERATOR_H__
#define __GLOBE_FILEITERATOR_H__
namespace glb {


//---------------------------------------------------------------------------
// FileIterator
//---------------------------------------------------------------------------
class GLB_EXPORT FileIterator
{
public :
  enum FileType
  {
    TYPE_UNKNOWN,
    TYPE_FILE,
    TYPE_DIRECTORY,
    TYPE_SYMLINK,
  };

  enum QueryFlags
  {
    // files properties
    QUERY_NAMEONLY  = 0x00,
    QUERY_TYPE      = 0x01,
    QUERY_SIZE      = 0x02,
    QUERY_MODIFTIME = 0x04,
    QUERY_ALLPROPS  = QUERY_TYPE | QUERY_SIZE | QUERY_MODIFTIME,

    // filters (those flags are mutually exclusive)
    QUERY_FILES          = 0x10,
    QUERY_DIRS           = 0x20,
    QUERY_HIDDEN         = 0x40, // files and/or dirs
    QUERY_DIRS_AND_FILES = QUERY_FILES | QUERY_DIRS,

    // masks (used internally)
    QUERYMASK_FILTERS = 0xF0,
  };

  struct FileInfo
  {
    // query flags
    // * this value must be setup using flags from the QueryFlags enum
    //   *before* being sent to the openDir() or getNext() method.
    // * this value will never be reset by the openDir() or getNext() method
    //   so you'll only need to setup it the first time you use it.
    flags8 fQueryFlags;

    // file properties
    StringA  strName;     // file name (i.e.: "myfile.ext"), this field can be empty when getNext() reaches the end of dir
    FileType eType;       // file type, only if (fQueryFlags & QUERY_TYPE) != 0, *but* it's always setup on Windows because it's costless to get it
    uint64   uiSize;      // file size, only if (fQueryFlags & QUERY_SIZE) != 0
    time_t   nModifTime;  // modification unix time, only if (fQueryFlags & QUERY_MODIFTIME) != 0
  };


public :
  FileIterator (void);
  ~FileIterator (void);

  // open
  bool openDir (const char* pszDirectory, FileInfo& infoFirst);
  bool isOpen  (void) const;
  bool isEmpty (void) const;

  // iterator properties
  const StringA& getDirectory (void) const;

  // iterate
  bool getNext     (FileInfo& info);
  bool seekToFirst (FileInfo& info);

  // close
  void close (void);

  // static utils
  static uint getFilesList (const char* pszDirectory, bool bRecursive, StringAVec& vecOutFiles, flags8 fQueryFilter=QUERY_DIRS_AND_FILES);
  static uint getFilesInfo (const char* pszDirectory, bool bRecursive, std::vector<FileInfo>& vecOutFilesInfo, flags8 fQueryFlags=QUERY_DIRS_AND_FILES);


private :
  bool getNextImpl (FileInfo& info);


protected :
  StringA m_strDir;
  void*   m_pHandle;
  bool    m_bEmpty;
};


#include "FileIterator.inl.hpp"


} // namespace glb
#endif // #ifndef __GLOBE_FILEITERATOR_H__
