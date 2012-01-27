//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-11-29 14:43:22
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_VECTOR2_H__
#define __GLOBE_VECTOR2_H__
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
#endif // #ifndef __GLOBE_VECTOR2_H__
