//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-08 00:19:36
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
// Static Members
//---------------------------------------------------------------------------
int System::ms_nIsBigEndian = -1;


//---------------------------------------------------------------------------
uint System::getPID (void)
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    return ::GetCurrentProcessId();
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
    return (uint)getpid();
  }
  #else
  #error Platform not supported !
  #endif
}

//---------------------------------------------------------------------------
uint System::lastError (void)
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    return ::GetLastError();
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
    return errno;
  }
  #else
  #error Unsupported platform !
  #endif
}

//---------------------------------------------------------------------------
const char* System::lastErrorString (void)
{
  return System::getErrorString(System::lastError());
}

//---------------------------------------------------------------------------
const char* System::getErrorString (uint uiOsError)
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    // This code is not thread safe, but it would be useless to lock it with
    // a mutex here because we return a reference to the local static
    // buffer (szError).

    static char szError[1024];

    DWORD dwRes = ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK, 0, uiOsError, 0, (char*)&szError, sizeof(szError), 0);
    if (dwRes == 0)
    {
      szError[0] = 0;
      GLB_LOGERR("Error while calling FormatMessage() ! Error %u.", ::GetLastError());
    }
    else
    {
      GLB_ASSERT(dwRes == (DWORD)strlen((char*)&szError));

      if ((szError[dwRes-2] == '\n') || (szError[dwRes-2] == '\r'))
        szError[dwRes-2] = '\0';
      else if ((szError[dwRes-1] == '\n') || (szError[dwRes-1] == '\r'))
        szError[dwRes-1] = '\0';
    }

    return (const char*)&szError;
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
    return strerror(uiOsError);
  }
  #else
  #error Unsupported platform !
  #endif
}

//---------------------------------------------------------------------------
void System::sleep (uint uiMilliseconds)
{
#if defined(GLB_PLATFORM_WINDOWS)
  ::Sleep(uiMilliseconds);
#elif defined(GLB_PLATFORM_LINUX)
  ::usleep(uiMilliseconds * 1000);
#else
# error Platform not supported !
#endif
}

//---------------------------------------------------------------------------
void System::swapBufferBytes (void* pInOutBuffer, uint uiElemSize, uint uiElemCount)
{
  register uchar *p, *q;

  p = (uchar*)pInOutBuffer;

  if (uiElemSize == 2)
  {
    q = p + 1;
    while (uiElemCount--)
    {
      *p ^= *q;
      *q ^= *p;
      *p ^= *q;
      p  += 2;
      q  += 2;
    }
    return;
  }

  while (uiElemCount--)
  {
    q = p + uiElemSize - 1;
    while (p < q)
    {
      *p ^= *q;
      *q ^= *p;
      *p ^= *q;
      ++p;
      --q;
    }
    p += uiElemSize >> 1;
  }
}

//---------------------------------------------------------------------------
int System::detectEndianness (void)
{
  // detect endianness
  GLB_ASSERT(ms_nIsBigEndian < 0);
  if (ms_nIsBigEndian < 0)
  {
    byte swaptest[2] = { 1, 0 };
    ms_nIsBigEndian  = (*(int16*)swaptest == 1) ? 0 : 1;
  }
  GLB_ASSERT((ms_nIsBigEndian == 0) || (ms_nIsBigEndian == 1));

  return ms_nIsBigEndian;
}


} // namespace glb
