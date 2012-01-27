//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-11-30 03:12:21
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_QUATERNION_H__
#define __GLOBE_QUATERNION_H__
namespace glb {


//---------------------------------------------------------------------------
// Quaternion
//---------------------------------------------------------------------------
class GLB_EXPORT Quaternion : public D3DXQUATERNION
{
public :
  Quaternion (void);
  Quaternion (float x, float y, float z, float w);
  Quaternion (float angleX, float angleY, float angleZ);

  Quaternion slerp (const Quaternion& q2, float t) const;


public :
  // special vectors
  static const Quaternion ZERO;
  static const Quaternion IDENTITY;
};


#include "Quaternion.inl.hpp"


} // namespace glb
#endif // #ifndef __GLOBE_QUATERNION_H__
