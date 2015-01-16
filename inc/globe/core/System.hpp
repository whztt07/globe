//***************************************************************************
//
// Globe
// A small C++ library to allow fast prototyping of Direct3D animations.
//
// Copyright (C) 2012-2013 Jean-Charles Lefebvre <polyvertex@gmail.com>
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
// Created On: 2011-10-08 00:15:35
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
