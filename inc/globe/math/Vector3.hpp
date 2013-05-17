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
// Created On : 2011-11-29 14:44:03
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_VECTOR3_H__
#define __GLOBE_VECTOR3_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// Vector3
//---------------------------------------------------------------------------
class GLB_EXPORT Vector3 : public D3DXVECTOR3
{
public :
  Vector3 (void);
  Vector3 (const D3DXVECTOR3& rhs);
  Vector3 (double x, double y, double z);

  // vector operations
  float length        (void) const;
  float squaredLength (void) const;
  float dot           (const Vector3& rhs) const;
  float normalize     (void);

  // cross product
  inline Vector3 cross     (const Vector3& rhs) const;
  inline Vector3 unitCross (const Vector3& rhs) const;


public :
  // special vectors
  static const Vector3 ZERO;
  static const Vector3 UNIT;
  static const Vector3 UNIT_X;
  static const Vector3 UNIT_Y;
  static const Vector3 UNIT_Z;
};


#include "Vector3.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_VECTOR3_H__
