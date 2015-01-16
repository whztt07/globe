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
// Created On: 2011-10-07 16:17:13
//
//***************************************************************************

#ifndef __GLOBE_TYPES_H__
#define __GLOBE_TYPES_H__


namespace glb
{
  typedef signed char   int8;
  typedef signed short  int16;
  typedef signed int    int32;

  typedef unsigned char   uint8;
  typedef unsigned short  uint16;
  typedef unsigned int    uint32;

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) // C99 detected : 64-bit types available
  typedef int64_t   int64;
  typedef uint64_t  uint64;
#elif defined(GLB_COMPILER_GCC)
  typedef signed long long    int64;
  typedef unsigned long long  uint64;
#elif defined(GLB_COMPILER_MSVC)
  typedef signed __int64    int64;
  typedef unsigned __int64  uint64;
#else
# error Unsupported compiler !
#endif

  typedef float   float32;
  typedef double  float64;

  typedef unsigned char  uchar;
  typedef unsigned int   uint;
  typedef unsigned long  ulong;

  typedef uint8  byte;

  typedef uint8   flags8;
  typedef uint16  flags16;
  typedef uint32  flags32;
  typedef uint64  flags64;

#if defined(GLB_COMPILER_GCC)
  typedef __INTPTR_TYPE__   int_ptr;
  typedef __UINTPTR_TYPE__  uint_ptr;
#elif defined(GLB_COMPILER_MSVC) && defined(_WIN64)
  typedef __int64           int_ptr;
  typedef unsigned __int64  uint_ptr;
#else
  typedef long           int_ptr;
  typedef unsigned long  uint_ptr;
#endif


} // namespace glb


typedef ::glb::int8     glb_int8;
typedef ::glb::int16    glb_int16;
typedef ::glb::int32    glb_int32;
typedef ::glb::uint8    glb_uint8;
typedef ::glb::uint16   glb_uint16;
typedef ::glb::uint32   glb_uint32;
typedef ::glb::int64    glb_int64;
typedef ::glb::uint64   glb_uint64;
typedef ::glb::float32  glb_float32;
typedef ::glb::float64  glb_float64;

typedef ::glb::uchar  glb_uchar;
typedef ::glb::uint   glb_uint;
typedef ::glb::ulong  glb_ulong;

typedef ::glb::byte  glb_byte;

typedef ::glb::flags8   glb_flags8;
typedef ::glb::flags16  glb_flags16;
typedef ::glb::flags32  glb_flags32;
typedef ::glb::flags64  glb_flags64;

typedef ::glb::int_ptr   glb_int_ptr;
typedef ::glb::uint_ptr  glb_uint_ptr;


GLB_STATIC_ASSERT(sizeof(glb::int8)     == 1);
GLB_STATIC_ASSERT(sizeof(glb::uint8)    == 1);
GLB_STATIC_ASSERT(sizeof(glb::int16)    == 2);
GLB_STATIC_ASSERT(sizeof(glb::uint16)   == 2);
GLB_STATIC_ASSERT(sizeof(glb::int32)    == 4);
GLB_STATIC_ASSERT(sizeof(glb::uint32)   == 4);
GLB_STATIC_ASSERT(sizeof(glb::int64)    == 8);
GLB_STATIC_ASSERT(sizeof(glb::uint64)   == 8);
GLB_STATIC_ASSERT(sizeof(glb::float32)  == 4);
GLB_STATIC_ASSERT(sizeof(glb::float64)  == 8);
GLB_STATIC_ASSERT(sizeof(glb::int_ptr)  == sizeof(void*));
GLB_STATIC_ASSERT(sizeof(glb::uint_ptr) == sizeof(void*));


#endif // #ifndef __GLOBE_TYPES_H__
