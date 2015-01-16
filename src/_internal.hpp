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
// Created On: 2011-10-07 18:36:43
//
//***************************************************************************

#ifndef __GLOBE__INTERNAL_H__
#define __GLOBE__INTERNAL_H__


//---------------------------------------------------------------------------
// Branding :)
//---------------------------------------------------------------------------
#define GLB_BRAND_NAME            "Globe"
#define GLB_BRAND_VERSION_STRING  "1.0.0" // keep this value Version3() compatible !


//---------------------------------------------------------------------------
// Dependencies
//---------------------------------------------------------------------------
#if defined(_WIN32) || defined(_WIN64)
# ifndef STRICT
#   define STRICT
# endif
# ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
# endif
# ifndef NOMINMAX
#   define NOMINMAX
# endif
# if defined(DEBUG) || defined(_DEBUG) || defined(__DEBUG)
#   ifndef _CRTDBG_MAP_ALLOC
#     define _CRTDBG_MAP_ALLOC
#   endif
#   include <stdlib.h>
#   include <crtdbg.h>
# endif
# include <windows.h>
#endif

#include <globe/globe.hpp>

#ifdef GLOBE_VFS
# include "zlib/zlib.h"
#endif

#ifdef GLOBE_VFS
# include "minizip/zip.h"
# include "minizip/unzip.h"
#endif

#include "tinymt/tinymt32.hpp"
#include "tinymt/tinymt64.hpp"

#if defined(GLB_PLATFORM_WINDOWS)
# include <windows.h>
# include <shellapi.h>
# include <direct.h>
# include <fcntl.h>
# include <io.h>
# include <sys/timeb.h>
# include <sys/stat.h>
#elif defined(GLB_PLATFORM_LINUX)
# include <sys/stat.h>
# include <sys/types.h>
# include <fcntl.h>
#endif


#endif // #ifndef __GLOBE__INTERNAL_H__
