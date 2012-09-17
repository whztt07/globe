//***************************************************************************
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
