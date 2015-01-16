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
// Created On: 2011-10-07 21:26:54
//
//***************************************************************************

#ifndef __GLOBE_TIMER_H__
#define __GLOBE_TIMER_H__
namespace glb {


//---------------------------------------------------------------------------
// Timer
//---------------------------------------------------------------------------
class GLB_EXPORT Timer
{
public :
  Timer (void);
  ~Timer (void);

  // reset
  // * reset() will setup the "start" time so every subsequent call to one
  //   of the get*() methods will return time passed from the "start" time
  //   offset.
  // * note that the Timer() constructor calls it for you so you'll want to
  //   call it only if you want to update the "start" time offset.
  void reset (void);

  // get elapsed time since ctor or last reset
  float32 getSecondsFloat32 (void) const;
  float64 getSecondsFloat64 (void) const;
  uint32  getMilliseconds32 (void) const;
  uint64  getMilliseconds64 (void) const;
  uint64  getMicroseconds   (void) const;


private :
#if defined(GLB_PLATFORM_WINDOWS)
  void getPerformanceCount             (int64& nOutPerfCount, uint32& uiOutTicksElapsed) const;
  void getCorrectPerformanceValue      (int64& nPerfCountElapsed) const;
  void getCorrectPerformanceValueMilli (uint32& uiPerfCountElapsedMilli) const;
#endif


private :
#if defined(GLB_PLATFORM_WINDOWS)
          int64    m_nFrequency;
  mutable int64    m_nStart;
          ulong    m_dwStartTicks;
          uint_ptr m_dwTimerAffinityMask;
  mutable int64    m_nLastPerfCountElapsed;
#elif defined(GLB_PLATFORM_LINUX)
  struct timeval m_tvStart;
#endif
};


} // namespace glb
#endif // #ifndef __GLOBE_STRINGA_H__
