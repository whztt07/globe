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
// Created On: 2012-09-17 16:46:32
//
//***************************************************************************

#ifndef __GLOBE_CONFIG_H__
#define __GLOBE_CONFIG_H__


// Inlcudes the VFS and Archive classes.
//
//#define GLOBE_VFS


// If enabled, the Archive class will be able create archive files, not only
// to read them.
// Requires GLOBE_VFS to be defined.
//
//#define GLOBE_ARCHIVE_CANCREATE


// Inlcudes all the math, renderer, engine and scenegraph classes.
// Requires GLOBE_VFS to be defined.
//
//#define GLOBE_GFX


#endif // #ifndef __GLOBE_CONFIG_H__
