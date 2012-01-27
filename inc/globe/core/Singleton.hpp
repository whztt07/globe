//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-10 21:10:17
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_SINGLETON_H__
#define __GLOBE_SINGLETON_H__
namespace glb {


//---------------------------------------------------------------------------
// Singleton
//
// Automatic Singleton Implementation by Scott Bilas.
// http://www.drizzle.com/~scottb/publish/gpgems1_singleton.htm
//
// CREDITS
// * Original code by Scott Bilas.
// * Modified to fit the needs of the Globe Engine.
//---------------------------------------------------------------------------
template <typename T>
class Singleton
{
protected :
  static T* ms_pSingleton;


public :
  Singleton (void)
  {
    GLB_ASSERT(!ms_pSingleton);
    ms_pSingleton = static_cast<T*>(this);
  }

  ~Singleton (void)
  {
    GLB_ASSERT(ms_pSingleton);
    ms_pSingleton = NULL;
  }

  static T& getSingleton (void)
  {
    GLB_ASSERT(ms_pSingleton);
    return *ms_pSingleton;
  }

  static T* getSingletonPtr (void)
  {
    return ms_pSingleton;
  }
};

template <typename T> T* Singleton<T>::ms_pSingleton = NULL;


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
#define GLB_IMPLEMENT_SINGLETON(classname) \
  classname& classname::getSingleton (void) \
  { \
    GLB_ASSERT(ms_pSingleton); \
    return *ms_pSingleton; \
  } \
  \
  classname* classname::getSingletonPtr (void) \
  { \
    return ms_pSingleton; \
  }


} // namespace glb
#endif // #ifndef __GLOBE_SINGLETON_H__
