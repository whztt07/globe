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
// Created On : 2011-11-29 14:43:22
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_VECTOR2_H__
#define __GLOBE_VECTOR2_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// Vector2
//---------------------------------------------------------------------------
class GLB_EXPORT Vector2 : public D3DXVECTOR2
{
public :
  Vector2 (void);
  Vector2 (const D3DXVECTOR2& rhs);
  Vector2 (double x, double y);

  // vector operations
  float length        (void) const;
  float squaredLength (void) const;
  float dot           (const Vector2& rhs) const;
  float normalize     (void);

  // perpendicular vector
  Vector2 perp     (void) const;               // returns (y,-x)
  Vector2 unitPerp (void) const;               // returns (y,-x)/sqrt(x*x+y*y)
  float   dotPerp  (const Vector2& rhs) const; // returns x*rhs.y - y*rhs.x


public :
  // special vectors
  static const Vector2 ZERO;
  static const Vector2 UNIT;
  static const Vector2 UNIT_X;
  static const Vector2 UNIT_Y;
};


#include "Vector2.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_VECTOR2_H__
