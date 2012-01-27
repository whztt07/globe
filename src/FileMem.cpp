//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-10 20:37:56
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
FileMem::FileMem (void)
: File()
{
  m_pBuffer      = NULL;
  m_uiBufferSize = 0;
  m_uiFileSize   = 0;
  m_uiGrowBytes  = 0;
  m_uiPosition   = 0;
  m_bAutoDelete  = false;
}

//---------------------------------------------------------------------------
FileMem::FileMem (uint32 uiGrowBytes/*=1024*/)
: File()
{
  GLB_ASSERT(uiGrowBytes > 0);

  m_pBuffer      = NULL;
  m_uiBufferSize = 0;
  m_uiFileSize   = 0;
  m_uiGrowBytes  = 0;
  m_uiPosition   = 0;
  m_bAutoDelete  = false;

  this->openNew(uiGrowBytes);
}

//---------------------------------------------------------------------------
FileMem::FileMem (const char* pBuffer, uint32 uiBufferSize, uint32 uiInitialFileSize, uint32 uiGrowBytes/*=0*/)
: File()
{
  m_pBuffer      = NULL;
  m_uiBufferSize = 0;
  m_uiFileSize   = 0;
  m_uiGrowBytes  = uiGrowBytes;
  m_uiPosition   = 0;
  m_bAutoDelete  = false;

  this->open(pBuffer, uiBufferSize, uiInitialFileSize, uiGrowBytes);
}

//---------------------------------------------------------------------------
FileMem::~FileMem (void)
{
  this->close();
}


//---------------------------------------------------------------------------
void FileMem::close (void)
{
  GLB_ASSERT(m_uiFileSize <= m_uiBufferSize);

  if (m_pBuffer && m_bAutoDelete)
    this->blockFree(m_pBuffer);

  m_pBuffer      = NULL;
  m_uiBufferSize = 0;
  m_uiFileSize   = 0;
  m_uiGrowBytes  = 0;
  m_uiPosition   = 0;
  m_bAutoDelete  = false;

  File::close();
}

//---------------------------------------------------------------------------
void FileMem::open (const char* pBuffer, uint32 uiBufferSize, uint32 uiInitialFileSize, uint32 uiGrowBytes/*=0*/)
{
  if (pBuffer && (uiBufferSize == 0))
    GLB_THROW(EXCODE_PARAM, "Wrong buffer size !");
  if ((uiBufferSize == 0) && (uiGrowBytes == 0))
    GLB_THROW(EXCODE_PARAM, "Incorrect growing size !");
  if ((uiBufferSize != 0) && (uiGrowBytes != 0))
    GLB_THROW(EXCODE_PARAM, "Growing size must be null since a non-null buffer has been given !");
  if ((uiBufferSize != 0) && (uiInitialFileSize > uiBufferSize))
    GLB_THROW(EXCODE_PARAM, "Initial file size is bigger than buffer size !");

  GLB_ASSERT(m_pBuffer == NULL); // promote good programming practices :)
  this->close();

  m_pBuffer      = (byte*)pBuffer;
  m_uiBufferSize = uiBufferSize;
  m_uiFileSize   = (uiGrowBytes == 0) ? uiInitialFileSize : 0;
  m_uiGrowBytes  = uiGrowBytes;
  m_uiPosition   = 0;
  m_bAutoDelete  = false;

  // common properties
  m_dwOpenFlags = FILE_FLAG_READWRITE;
}

//---------------------------------------------------------------------------
void FileMem::openNew (uint32 uiGrowBytes/*=1024*/)
{
  GLB_ASSERT(uiGrowBytes > 0);
  this->open(NULL, 0, 0, uiGrowBytes);
}

//---------------------------------------------------------------------------
void FileMem::openFromDisk (const char* pszFilePath, uint32 uiMaxLengthToReadFromDisk/*=0*/, int64 nOffset/*=0*/, uint32 uiGrowBytes/*=0*/)
{
  FileDisk df;
  byte*    pBuffer;
  uint64   uiLength64;
  uint32   uiLength32;
  uint32   uiRes;

  GLB_ASSERT(nOffset >= 0);
  if (nOffset < 0)
    nOffset = 0;
  if (uiMaxLengthToReadFromDisk == 0)
    uiMaxLengthToReadFromDisk = std::numeric_limits<uint>::max();

  df.open(pszFilePath, FILE_FLAG_READ);
  df.seek(FILE_SEEK_BEGIN, nOffset);
  uiLength64 = df.getLength() - df.getPosition();
  if (uiLength64 > (uint64)uiMaxLengthToReadFromDisk)
    uiLength64 = (uint64)uiMaxLengthToReadFromDisk;
  GLB_ASSERT(uiLength64 <= std::numeric_limits<uint>::max());
  uiLength32 = (uint32)uiLength64; // here, it's safe to force cast to 32bit

  pBuffer = this->blockAlloc(uiLength32);
  uiRes   = df.read(pBuffer, uiLength32);
  GLB_ASSERT(uiRes == uiLength32);
  df.close();

  this->open((char*)pBuffer, uiLength32, uiRes);

  // do not forget this !
  m_bAutoDelete = true;

  // we can make this file growable since we allocated buffer ourself so
  // we know it is ok to reallocate buffer with our internal methods if
  // needed
  m_uiGrowBytes = uiGrowBytes;
}

//---------------------------------------------------------------------------
uint32 FileMem::read (void* pBuffer, uint32 uiLength)
{
  uint32 uiBytesToRead;

  // check state and parameters
  GLB_ASSERT(this->isReadable());
  GLB_FORCEASSERT(pBuffer != NULL);
  if (uiLength <= 0)
    return 0;
  if (m_uiPosition >= m_uiFileSize)
    return 0;

  // determine the number of bytes to read
  if ((m_uiPosition + uiLength) > m_uiFileSize)
    uiBytesToRead = m_uiFileSize - m_uiPosition;
  else
    uiBytesToRead = uiLength;

  // do read
  this->blockCopy((byte*)pBuffer, m_pBuffer + m_uiPosition, uiBytesToRead);
  m_uiPosition += uiBytesToRead;

  return uiBytesToRead;
}

//---------------------------------------------------------------------------
void FileMem::write (const void* pBuffer, uint32 uiLength)
{
  // check state and parameters
  GLB_ASSERT(this->isWritable());
  GLB_FORCEASSERT(m_pBuffer);
  GLB_ASSERT(uiLength > 0);
  if (uiLength <= 0)
    return;

  // do we need to grow file ?
  if ((m_uiPosition + uiLength) > m_uiBufferSize)
    this->growMemFile(m_uiPosition + uiLength);
  GLB_ASSERT((m_uiPosition + uiLength) <= m_uiBufferSize);

  // do write
  this->blockCopy(m_pBuffer + m_uiPosition, (byte*)pBuffer, uiLength);
  m_uiPosition += uiLength;

  // update file size if needed
  if (m_uiPosition > m_uiFileSize)
    m_uiFileSize = m_uiPosition;
}

//---------------------------------------------------------------------------
void FileMem::flush (void)
{
  GLB_ASSERT(this->isWritable());
  GLB_ASSERT(m_pBuffer);

  // nothing special to do here...
}

//---------------------------------------------------------------------------
uint64 FileMem::seek (SeekOrigin eOrigin, int64 nOffset)
{
  int64 nNewPosition;

  GLB_ASSERT(m_pBuffer);

  // determine future position in file
  switch (eOrigin)
  {
    case FILE_SEEK_BEGIN :
      nNewPosition = nOffset;
      break;
    case FILE_SEEK_CURRENT :
      nNewPosition = static_cast<int64>(m_uiPosition) + nOffset;
      break;
    case FILE_SEEK_END :
      GLB_FORCEASSERT((nOffset <= 0) && "Offset must be negative when seeking from FILE_SEEK_END !");
      nNewPosition = m_uiFileSize + nOffset;
      break;
    default :
      GLB_ASSERT(0); // incorrect SeekOrigin value
      return m_uiPosition;
      break;
  }
  GLB_FORCEASSERT(nNewPosition <= (int64)std::numeric_limits<uint>::max());

  // check future position value
  if (nNewPosition < 0)
    GLB_THROW(EXCODE_FILE_SEEKING, "Seeked before beginning of file !");
  else if (static_cast<uint32>(nNewPosition) > m_uiFileSize)
    this->growMemFile(static_cast<uint32>(nNewPosition));

  // update members
  m_uiPosition = static_cast<uint32>(nNewPosition);

  return m_uiPosition;
}

//---------------------------------------------------------------------------
void FileMem::setLength (uint64 uiNewLength)
{
  uint32 uiNewLength32;

  // check parameter
  GLB_FORCEASSERT(uiNewLength <= (uint64)std::numeric_limits<uint>::max());
  uiNewLength32 = static_cast<uint32>(uiNewLength);

  // grow file if needed
  if (uiNewLength32 > m_uiBufferSize)
    this->growMemFile(uiNewLength32);

  // adjust cursor if needed
  if (m_uiPosition > uiNewLength32)
    m_uiPosition = uiNewLength32;

  // update file size member
  m_uiFileSize = uiNewLength32;
}

//---------------------------------------------------------------------------
void FileMem::writeToDiskFile (const char* pszFilePath) const
{
  FileDisk fdisk;

  fdisk.open(pszFilePath, File::FILE_FLAG_WRITE | File::FILE_FLAG_CREATE_ALWAYS);
  fdisk.write((void*)m_pBuffer, m_uiFileSize);
  fdisk.close();
}

//---------------------------------------------------------------------------
uint32 FileMem::bufferAccess (flags32 dwBufferAccessMode, uint32 uiLength/*=0*/, byte** ppBufferBegin/*=NULL*/, byte** ppBufferEnd/*=NULL*/)
{
  flags32 dwCommand = dwBufferAccessMode & BUFFERACCESSMODE_MASK_COMMAND;

  GLB_ASSERT(this->isOpen());
  GLB_FORCEASSERT(this->validateBufferAccessMode(dwBufferAccessMode));


  // read request
  if (dwCommand == BUFFERACCESSMODE_COMMAND_READONLY)
  {
    GLB_ASSERT(ppBufferBegin != NULL);
    GLB_ASSERT(ppBufferEnd != NULL);

    if (uiLength == 0)
      uiLength = uint32(m_uiBufferSize - m_uiPosition);

    *ppBufferBegin = m_pBuffer + m_uiPosition;
    *ppBufferEnd   = m_pBuffer + std::min<uint32>(m_uiFileSize, (m_uiPosition + uiLength));

    // we are reading, so move the cursor now
    m_uiPosition += uint32((byte*)(*ppBufferEnd) - (byte*)(*ppBufferBegin));

    // update common properties
    m_dwBufferAccessMode = dwBufferAccessMode;

    return uint32( (byte*)(*ppBufferEnd) - (byte*)(*ppBufferBegin) );
  }

  // write request
  else if (dwCommand == BUFFERACCESSMODE_COMMAND_READWRITE)
  {
    GLB_ASSERT(ppBufferBegin != NULL);
    GLB_ASSERT(ppBufferEnd != NULL);

    // since we are in write mode, we can grow file if user wants to do so
    if (!(dwBufferAccessMode & BUFFERACCESSMODE_FLAG_DONOTGROW) && ((m_uiPosition + uiLength) > m_uiBufferSize))
      this->growMemFile(m_uiPosition + uiLength);

    *ppBufferBegin = m_pBuffer + m_uiPosition;
    *ppBufferEnd   = m_pBuffer + std::min<uint32>(m_uiBufferSize, (m_uiPosition + uiLength));

    // update common properties
    m_dwBufferAccessMode = dwBufferAccessMode;

    return uint32( (byte*)(*ppBufferEnd) - (byte*)(*ppBufferBegin) );
  }

  // commit request
  else if (dwCommand == BUFFERACCESSMODE_COMMAND_COMMIT)
  {
    GLB_ASSERT(ppBufferBegin == NULL);
    GLB_ASSERT(ppBufferEnd == NULL);
    GLB_ASSERT((m_uiPosition + uiLength) <= m_uiBufferSize);

    // seek only in write mode
    if ((m_dwBufferAccessMode & BUFFERACCESSMODE_MASK_COMMAND) == BUFFERACCESSMODE_COMMAND_READWRITE)
    {
      m_uiPosition += uiLength;
      if (m_uiPosition > m_uiFileSize)
        m_uiFileSize = m_uiPosition;

      GLB_ASSERT(m_uiFileSize <= m_uiBufferSize);
    }

    // update common properties
    m_dwBufferAccessMode = BUFFERACCESSMODE_NONE;

    return 0;
  }

  GLB_ASSERT(0); // should never reach this line
  return 0;   // shut up compiler
}



//---------------------------------------------------------------------------
void FileMem::growMemFile (uint32 uiNewSize)
{
  byte*  pFinalBuffer;
  uint32 uiFinalBufferSize;

  if (uiNewSize <= m_uiBufferSize)
    return;

  // determine the final buffer size according to the growing value
  GLB_ASSERT(m_uiGrowBytes > 0);
  if (m_uiGrowBytes <= 0)
  {
    GLB_THROW(EXCODE_FILE_WRITING, "This memory file is not growable !");
  }
  else if (m_uiGrowBytes == 1)
  {
    uiFinalBufferSize = uiNewSize;
  }
  else
  {
    uiFinalBufferSize = m_uiBufferSize;
    while (uiFinalBufferSize < uiNewSize)
      uiFinalBufferSize += m_uiGrowBytes;
  }

  // (re)allocate buffer
  if (m_pBuffer == NULL)
  {
    pFinalBuffer  = this->blockAlloc(uiFinalBufferSize);
    m_bAutoDelete = true;
  }
  else
  {
    pFinalBuffer = this->blockRealloc(m_pBuffer, uiFinalBufferSize);
  }

  // check allocated buffer
  if (!pFinalBuffer)
    GLB_THROWEXCODE(EXCODE_MEMORY);

  // update members
  m_pBuffer      = pFinalBuffer;
  m_uiBufferSize = uiFinalBufferSize;
}

//---------------------------------------------------------------------------
byte* FileMem::blockAlloc (uint32 uiSize)
{
  return (byte*)malloc(uiSize);
}

//---------------------------------------------------------------------------
byte* FileMem::blockRealloc (byte* pBlock, uint32 uiNewSize)
{
  GLB_ASSERT(uiNewSize > 0);
  return (byte*)realloc(pBlock, uiNewSize);
}

//---------------------------------------------------------------------------
#ifdef GLB_COMPILER_MSVC
#pragma intrinsic(memcpy)
#endif
void FileMem::blockCopy (byte* pTargetBlock, const byte* pSourceBlock, uint32 uiSize)
{
  GLB_ASSERT(pTargetBlock && pSourceBlock);
  GLB_ASSERT(uiSize > 0);

  (void)memcpy(pTargetBlock, pSourceBlock, uiSize);
}
#ifdef GLB_COMPILER_MSVC
#pragma function(memcpy)
#endif

//---------------------------------------------------------------------------
void FileMem::blockFree (byte* pBlock)
{
  GLB_ASSERT(pBlock);
  free(pBlock);
}


} // namespace glb
