//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-08 00:15:35
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_SYSTEM_H__
#define __GLOBE_SYSTEM_H__
namespace glb {


//---------------------------------------------------------------------------
// System
//---------------------------------------------------------------------------
class GLB_EXPORT System
{
public :
  // windows command line processing
#ifdef GLB_PLATFORM_WINDOWS
  static void cmdLineToStringVec (StringAVec& vecArgs);
#else
  static void cmdLineToStringVec (StringAVec& vecArgs, int argc, char** argv);
#endif

  // process
  static uint getPID (void); // current process id

  // error
  static uint        lastError       (void);
  static const char* lastErrorString (void);
  static const char* getErrorString  (uint uiOsError);

  // sleep
  static void sleep (uint uiMilliseconds);

  // byte ordering
  static bool isBigEndian     (void);
  static void swapBytes       (uint16 valSource, byte* pOutBytes);
  static void swapBytes       (uint32 valSource, byte* pOutBytes);
  static void swapBytes       (uint64 valSource, byte* pOutBytes);
  static void swapBytes       (float32 valSource, byte* pOutBytes);
  static void swapBytes       (float64 valSource, byte* pOutBytes);
  static void swapBufferBytes (void* pBuffer, uint uiElemSize, uint uiElemCount);


private :
  static int detectEndianness (void);


private :
  static int ms_nIsBigEndian;
};


#include "System.inl.hpp"


} // namespace glb
#endif // #ifndef __GLOBE_SYSTEM_H__
