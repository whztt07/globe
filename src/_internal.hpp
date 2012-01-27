//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-07 18:36:43
//
// $Id$
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

#include "zlib/zlib.h"

#include "minizip/zip.h"
#include "minizip/unzip.h"

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
