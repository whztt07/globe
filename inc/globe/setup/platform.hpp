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
// Created On : 2011-10-07 16:17:09
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_PLATFORM_H__
#define __GLOBE_PLATFORM_H__


// compiler
//
// Known _MSC_VER values :
// * C Compiler 6.0      600
// * C/C++ Compiler 7.0  700
// * Visual C++ 1.0      800
// * Visual C++ 2.0      900
// * Visual C++ 2.x      900
// * Visual C++ 4.0      1000
// * Visual C++ 5.0      1100
// * Visual C++ 6.0      1200
// * Visual C++ 2002     1300
// * Visual C++ 2003     1310
// * Visual C++ 2005     1400
// * Visual C++ 2008     1500
// * Visual C++ 2010     1600
// * Visual C++ 2011     1700
#if defined(__GNUC__)
# ifndef __GNUC_PATCHLEVEL__
#   define __GNUC_PATCHLEVEL__  0
# endif
# define GLB_COMPILER_GCC
# define GLB_COMPILER_NAME     "GNU"
# define GLB_COMPILER_VERSION  (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)  // single version macro, format will be VVRRPP (Version, Revision, Patch)
# define GLB_FUNCTION          __func__  // __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
# define GLB_COMPILER_MSVC
# define GLB_COMPILER_NAME     "Microsoft"
# define GLB_COMPILER_VERSION  _MSC_VER
# define GLB_FUNCTION          __FUNCTION__
#else
# error Unknown compiler !
#endif


// platform
#if defined(_WIN32) || defined(_WIN64)
# define GLB_PLATFORM_NAME     "Windows"
# define GLB_PLATFORM_WINDOWS
#elif defined(linux) || defined(__linux) || defined(__linux__)
# define GLB_PLATFORM_NAME     "Linux"
# define GLB_PLATFORM_LINUX
#else
# error Unknown platform !
#endif


// build model
#if defined(GLB_COMPILER_GCC)
# if (defined(__WORDSIZE) && (__WORDSIZE == 64)) || defined(__arch64__) || defined(__LP64__) || defined(__x86_64__)
#   define GLB_MODEL_64
# else
#   define GLB_MODEL_32
# endif
#elif defined(GLB_COMPILER_MSVC)
# if defined(_M_X64)
#   define GLB_MODEL_64
# else
#   define GLB_MODEL_32
# endif
#else
# error Unknown platform !
#endif


#endif // #ifndef __GLOBE_PLATFORM_H__
