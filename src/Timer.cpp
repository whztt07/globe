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
// Created On : 2011-10-07 21:26:57
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------
#if defined(GLB_PLATFORM_WINDOWS)
  const int32 c_nMaxLeapThreshold = 100;  // milliseconds
#endif


//---------------------------------------------------------------------------
// Local Variables
//---------------------------------------------------------------------------
#if defined(GLB_PLATFORM_WINDOWS)
  typedef /*DECLSPEC_IMPORT*/ DWORD (WINAPI* FNTIMEGETTIME)(void);

  static int           g_nPerfCounter   = -1; // this is a tribool : -1:"need to test if perf-counter is available" ; 0:"perf-counter is not available" ; 1:"perf-counter is available"
  static FNTIMEGETTIME g_pfnTimeGetTime = NULL;
#endif



//---------------------------------------------------------------------------
Timer::Timer (void)
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    m_dwTimerAffinityMask = 0;

    // lazy init
    if (g_nPerfCounter == -1)
    {
      // is it multi-core platform ?
      //SYSTEM_INFO sysinfo;
      //::GetSystemInfo(&sysinfo);
      //g_bMultiCore = (sysinfo.dwNumberOfProcessors > 1);

      // is performance counter available ?
      g_nPerfCounter = (::QueryPerformanceFrequency((LARGE_INTEGER*)&m_nFrequency) == FALSE) ? 0 : 1;

      // if performance counter is not available, fallback to timeGetTime().
      // but if performance counter *is* available, we still need an another
      // time strategy to find a workaround for the well-known Q274323
      // problem (potential counter leaps)...
      g_pfnTimeGetTime = (FNTIMEGETTIME)::GetProcAddress(::LoadLibraryA("winmm.dll"), "timeGetTime");

      // if timeGetTime() is not available, fallback to GetTickCount()
      if (!g_pfnTimeGetTime)
        g_pfnTimeGetTime = GetTickCount;
    }
  }
  #endif


  // setup "start" time
  this->reset();
}

//---------------------------------------------------------------------------
Timer::~Timer (void)
{
}

//---------------------------------------------------------------------------
void Timer::reset (void)
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    if (g_nPerfCounter > 0)
    {
      DWORD     dwProcessAffinityMask;
      DWORD     dwSystemAffinityMask;
      DWORD_PTR dwOldThreadAffinityMask;
      HANDLE    hThread;

      // get current process affinity with processor cores
      ::GetProcessAffinityMask(::GetCurrentProcess(), (PDWORD_PTR)&dwProcessAffinityMask, (PDWORD_PTR)&dwSystemAffinityMask);

      // lazy init for the timer affinity mask
      // this is a common trick to find the first core used by the current process
      if (!m_dwTimerAffinityMask)
      {
        m_dwTimerAffinityMask = 1;
        while (!(m_dwTimerAffinityMask & dwProcessAffinityMask))
          m_dwTimerAffinityMask <<= 1;
      }

      // setup our affinity to the desired processor core
      hThread = ::GetCurrentThread();
      dwOldThreadAffinityMask = ::SetThreadAffinityMask(hThread, m_dwTimerAffinityMask);

      // init timer by getting frequency and start time
      (void)::QueryPerformanceFrequency((LARGE_INTEGER*)&m_nFrequency);
      (void)::QueryPerformanceCounter((LARGE_INTEGER*)&m_nStart);
      m_dwStartTicks = g_pfnTimeGetTime();

      // reset affinity to be compliant with the calling environment
      (void)::SetThreadAffinityMask(hThread, dwOldThreadAffinityMask);

      // if frequency is not bound in a 32bit word, we'll get erroneous values
      // from Timer::Get*32() methods
      GLB_ASSERT(m_nFrequency <= (int64)std::numeric_limits<uint32>::max());

      // reset other members
      m_nLastPerfCountElapsed = 0;
    }
    else
    {
      GLB_ASSERT(g_pfnTimeGetTime);
      m_dwStartTicks = g_pfnTimeGetTime();
    }
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
    gettimeofday(&m_tvStart, NULL);
  }
  #endif
}



//---------------------------------------------------------------------------
#if defined(GLB_PLATFORM_WINDOWS)
GLB_FORCEINLINE void Timer::getPerformanceCount (int64& nOutPerfCount, uint32& uiOutTicksElapsed) const
{
  HANDLE    hThread = ::GetCurrentThread();
  DWORD_PTR dwOldThreadAffinityMask;

  GLB_ASSERT(g_nPerfCounter > 0);
  GLB_ASSERT(g_pfnTimeGetTime);

  dwOldThreadAffinityMask = ::SetThreadAffinityMask(hThread, m_dwTimerAffinityMask);

  (void)::QueryPerformanceCounter((LARGE_INTEGER*)&nOutPerfCount);
  uiOutTicksElapsed = g_pfnTimeGetTime();

  (void)::SetThreadAffinityMask(hThread, dwOldThreadAffinityMask);

  GLB_ASSERT(uiOutTicksElapsed > m_dwStartTicks);
  uiOutTicksElapsed -= m_dwStartTicks;
}
#endif

//---------------------------------------------------------------------------
#if defined(GLB_PLATFORM_WINDOWS)
GLB_FORCEINLINE void Timer::getCorrectPerformanceValue (int64& nPerfCountElapsed) const
{
  uint32 uiTicksElapsed;
  uint32 uiPerfCountElapsedMilli;
  int32  nLeapTest;

  // get performance counter value
  this->getPerformanceCount(nPerfCountElapsed, uiTicksElapsed);

  // prepare a workaround to compensate a potential performance counter leap
  nPerfCountElapsed       -= m_nStart;
  uiPerfCountElapsedMilli  = uint32(nPerfCountElapsed * 1000 / m_nFrequency);

  GLB_ASSERT((nPerfCountElapsed * 1000 / m_nFrequency) <= (LONGLONG)std::numeric_limits<uint32>::max());
  GLB_ASSERT(uiPerfCountElapsedMilli <= (uint32)std::numeric_limits<int32>::max());
  GLB_ASSERT(uiTicksElapsed          <= (uint32)std::numeric_limits<int32>::max());

  // if too great leap is detected, adjust start time and update state
  // accordingly before return
  nLeapTest = (int32)uiPerfCountElapsedMilli - (int32)uiTicksElapsed;
  if ((nLeapTest < -c_nMaxLeapThreshold) || (nLeapTest > c_nMaxLeapThreshold))
  {
    LONGLONG nUpdate = std::min<LONGLONG>(LONGLONG(nLeapTest * m_nFrequency / 1000), (nPerfCountElapsed - m_nLastPerfCountElapsed));

#ifdef GLB_DEBUG
    //GLB_LOGWARN("Performance Counter leap detected (%d ms) !", nLeapTest);
#endif

    m_nStart          += nUpdate;
    nPerfCountElapsed -= nUpdate;
  }

  // update state
  m_nLastPerfCountElapsed = nPerfCountElapsed;
}
#endif

//---------------------------------------------------------------------------
#if defined(GLB_PLATFORM_WINDOWS)
GLB_FORCEINLINE void Timer::getCorrectPerformanceValueMilli (uint32& uiPerfCountElapsedMilli) const
{
  int64  nPerfCountElapsed;
  uint32 uiTicksElapsed;
  int32  nLeapTest;

  // get performance counter value
  this->getPerformanceCount(nPerfCountElapsed, uiTicksElapsed);

  // prepare a workaround to compensate a potential performance counter leap
  nPerfCountElapsed       -= m_nStart;
  uiPerfCountElapsedMilli  = uint32(nPerfCountElapsed * 1000 / m_nFrequency);

  GLB_ASSERT((nPerfCountElapsed * 1000 / m_nFrequency) <= (LONGLONG)std::numeric_limits<uint32>::max());
  GLB_ASSERT(uiPerfCountElapsedMilli <= (uint32)std::numeric_limits<int32>::max());
  GLB_ASSERT(uiTicksElapsed          <= (uint32)std::numeric_limits<int32>::max());

  // if too great leap is detected, adjust start time and update state
  // accordingly before return
  nLeapTest = (int32)uiPerfCountElapsedMilli - (int32)uiTicksElapsed;
  if ((nLeapTest < -c_nMaxLeapThreshold) || (nLeapTest > c_nMaxLeapThreshold))
  {
    LONGLONG nUpdate = std::min<LONGLONG>(LONGLONG(nLeapTest * m_nFrequency / 1000), (nPerfCountElapsed - m_nLastPerfCountElapsed));

#ifdef GLB_DEBUG
    //GLB_LOGWARN("Performance Counter leap detected (%d ms) !", nLeapTest);
#endif

    m_nStart          += nUpdate;
    nPerfCountElapsed -= nUpdate;

    GLB_ASSERT((nPerfCountElapsed * 1000 / m_nFrequency) <= (LONGLONG)std::numeric_limits<uint32>::max());
    uiPerfCountElapsedMilli = uint32(nPerfCountElapsed * 1000 / m_nFrequency);
  }

  // update state
  m_nLastPerfCountElapsed = nPerfCountElapsed;
}
#endif



//---------------------------------------------------------------------------
float32 Timer::getSecondsFloat32 (void) const
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    if (g_nPerfCounter > 0)
    {
      int64 nPerfCountElapsed;
      this->getCorrectPerformanceValue(nPerfCountElapsed);
      return (float32)nPerfCountElapsed / (float32)m_nFrequency;
    }
    else
    {
      DWORD   dwTime;
      float32 rTime;

      dwTime = g_pfnTimeGetTime();
      GLB_ASSERT(dwTime > m_dwStartTicks);

      rTime  = float32(dwTime - m_dwStartTicks);
      rTime *= float32(0.001);

      return rTime;
    }
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
	  struct timeval tvNow;
    float32 rElapsed;

    gettimeofday(&tvNow, NULL);

    rElapsed  = float32(tvNow.tv_sec - m_tvStart.tv_sec);
    rElapsed += float32(0.000001) * float32(tvNow.tv_usec - m_tvStart.tv_usec);

    if (rElapsed < float32(0.0))
      rElapsed = float32(0.0);

    return rElapsed;
  }
  #endif
}

//---------------------------------------------------------------------------
float64 Timer::getSecondsFloat64 (void) const
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    if (g_nPerfCounter > 0)
    {
      int64 nPerfCountElapsed;
      this->getCorrectPerformanceValue(nPerfCountElapsed);
      return (float64)nPerfCountElapsed / (float64)m_nFrequency;
    }
    else
    {
      DWORD   dwTime;
      float64 rTime;

      dwTime = g_pfnTimeGetTime();
      GLB_ASSERT(dwTime > m_dwStartTicks);

      rTime  = float64(dwTime - m_dwStartTicks);
      rTime *= float64(0.001);

      return rTime;
    }
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
	  struct timeval tvNow;
    float64 rElapsed;

    gettimeofday(&tvNow, NULL);

    rElapsed  = float64(tvNow.tv_sec - m_tvStart.tv_sec);
    rElapsed += float64(0.000001) * float64(tvNow.tv_usec - m_tvStart.tv_usec);

    if (rElapsed < float64(0.0))
      rElapsed = float64(0.0);

    return rElapsed;
  }
  #endif
}

//---------------------------------------------------------------------------
uint32 Timer::getMilliseconds32 (void) const
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    if (g_nPerfCounter > 0)
    {
      uint32 uiPerfCountElapsedMilli;
      this->getCorrectPerformanceValueMilli(uiPerfCountElapsedMilli);
      return uiPerfCountElapsedMilli;
    }
    else
    {
      DWORD dwTime;

      dwTime = g_pfnTimeGetTime();
      GLB_ASSERT(dwTime > m_dwStartTicks);

      return (dwTime - m_dwStartTicks);
    }
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
	  struct timeval tvNow;
    uint32 uiElapsed;

    gettimeofday(&tvNow, NULL);

    uiElapsed  = uint32(tvNow.tv_sec - m_tvStart.tv_sec) * 1000;
    uiElapsed += uint32(tvNow.tv_usec - m_tvStart.tv_usec) / 1000;

    return uiElapsed;
  }
  #endif
}

//---------------------------------------------------------------------------
uint64 Timer::getMilliseconds64 (void) const
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    if (g_nPerfCounter > 0)
    {
      int64 nPerfCountElapsed;

      this->getCorrectPerformanceValue(nPerfCountElapsed);

      nPerfCountElapsed *= 1000;
      nPerfCountElapsed /= m_nFrequency;

      return nPerfCountElapsed;
    }
    else
    {
      DWORD dwTime;

      dwTime = g_pfnTimeGetTime();
      GLB_ASSERT(dwTime > m_dwStartTicks);

      return uint64(dwTime - m_dwStartTicks);
    }
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
	  struct timeval tvNow;
    int64 nElapsed;

    gettimeofday(&tvNow, NULL);

    nElapsed  = int64(tvNow.tv_sec - m_tvStart.tv_sec) * 1000;
    nElapsed += int64(tvNow.tv_usec - m_tvStart.tv_usec) / 1000;

    if (nElapsed < 0)
      nElapsed = 0;

    return (uint64)nElapsed;
  }
  #endif
}

//---------------------------------------------------------------------------
uint64 Timer::getMicroseconds (void) const
{
  #if defined(GLB_PLATFORM_WINDOWS)
  {
    if (g_nPerfCounter > 0)
    {
      int64 nPerfCountElapsed;

      this->getCorrectPerformanceValue(nPerfCountElapsed);

      // scale by 1000000 in order to get microsecond precision
      nPerfCountElapsed *= 1000000;
      nPerfCountElapsed /= m_nFrequency;

      return nPerfCountElapsed;
    }
    else
    {
      DWORD dwTime;

      dwTime = g_pfnTimeGetTime();
      GLB_ASSERT(dwTime > m_dwStartTicks);

      return uint64(dwTime - m_dwStartTicks) * 1000;
    }
  }
  #elif defined(GLB_PLATFORM_LINUX)
  {
    struct timeval tvNow;
    int64 nElapsed;

    gettimeofday(&tvNow, NULL);

    nElapsed  = int64(tvNow.tv_sec - m_tvStart.tv_sec) * 1000000;
    nElapsed += int64(tvNow.tv_usec - m_tvStart.tv_usec);

    if (nElapsed < 0)
      nElapsed = 0;

    return (uint64)nElapsed;
  }
  #endif
}


} // namespace glb
