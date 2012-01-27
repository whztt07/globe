//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-07 21:26:54
//
// $Id$
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
