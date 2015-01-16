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
// Created On: 2011-10-10 21:10:17
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
