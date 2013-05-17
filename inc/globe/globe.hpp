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
// Created On : 2011-10-07 16:08:40
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_ENGINE_H__
#define __GLOBE_ENGINE_H__


// global configuration
#include "setup/config.hpp"

// debug mode ?
#if defined(DEBUG) || defined(_DEBUG) || defined(__DEBUG)
# ifndef GLB_DEBUG
#   define GLB_DEBUG
# endif
#endif

// standard c headers
#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <cerrno>
#include <cassert>
#include <cstring>
#include <ctime>
#include <cmath>

// standard c++ headers
#include <algorithm>
//#include <functional>
#include <limits>
#include <locale>
#include <string>
#include <vector>
//#include <deque>
#include <list>
//#include <set>
#include <map>
#include <iostream>
#include <strstream>

// dependencies headers
#ifdef GLOBE_GFX
# include <d3d9.h>
# include <d3dx9.h>
# include <dxerr9.h>
#endif
#ifdef min
# undef min
# undef max
#endif

// setup headers
#include "setup/platform.hpp"
#include "setup/macros.hpp"
#include "setup/types.hpp"

// core headers
#include "core/NonCopyable.hpp"
#include "core/Singleton.hpp"
#include "core/SmartPointer.hpp"
#include "core/StlHelpers.hpp"
#include "core/StringA.hpp"
//
#include "core/ExceptionCode.hpp"
#include "core/Exception.hpp"
#include "core/DateTime.hpp"
#include "core/System.hpp"
#include "core/Timer.hpp"
#include "core/Random.hpp"
#include "core/Version3.hpp"
//
#include "core/Logger.hpp"
#include "core/FileSystem.hpp"
#include "core/FileIterator.hpp"
#include "core/File.hpp"
#include "core/FileDisk.hpp"
#include "core/FileMem.hpp"
#include "core/ConfigFileRo.hpp"

// vfs headers
#ifdef GLOBE_VFS
# include "vfs/Archive.hpp"
# include "vfs/VFS.hpp"
#endif

// math headers
#ifdef GLOBE_GFX
# include "math/Math.hpp"
# include "math/Vector2.hpp"
# include "math/Vector3.hpp"
# include "math/Color4.hpp"
# include "math/Quaternion.hpp"
# include "math/Matrix4.hpp"
#endif

// renderer headers
#ifdef GLOBE_GFX
# include "renderer/Surface.hpp"
# include "renderer/Texture.hpp"
# include "renderer/Font.hpp"
# include "renderer/CubeTexture.hpp"
# include "renderer/VolumeTexture.hpp"
# include "renderer/VertexBuffer.hpp"
# include "renderer/IndexBuffer.hpp"
# include "renderer/VertexDeclaration.hpp"
# include "renderer/Renderer.hpp"
#endif

// engine headers
#ifdef GLOBE_GFX
# include "engine/Animation.hpp"
# include "engine/Drawable.hpp"
# include "engine/Effect.hpp"
# include "engine/Mesh.hpp"
#endif

// scenegraph headers
#ifdef GLOBE_GFX
# include "scenegraph/SceneNode.hpp"
# include "scenegraph/SceneCamera.hpp"
#endif


#endif // #ifndef __GLOBE_ENGINE_H__
