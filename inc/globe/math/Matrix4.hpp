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
// Created On: 2011-11-30 03:39:05
//
//***************************************************************************

#ifndef __GLOBE_MATRIX4_H__
#define __GLOBE_MATRIX4_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// Matrix4
//---------------------------------------------------------------------------
class GLB_EXPORT Matrix4 : public D3DXMATRIX
{
public :
  Matrix4 (void);
  Matrix4 (const Matrix4& rhs);
  Matrix4 (const D3DXMATRIX& rhs);
  Matrix4 (float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44);

  void makeZero        (void);
  void makeIdentity    (void);
  void makeTranslation (const Vector3& vecTranslate);
  void makeRotation    (const Vector3& vecYawPitchRoll);
  void makeRotation    (const Quaternion& quatRot);
  void makeScaling     (const Vector3& vecScale);
  void makeProjection  (float fovDegrees, float aspect, float znear, float zfar);
  void makeLookAt      (const Vector3& vecEye, const Vector3& vecTarget, const Vector3& vecUp);
  void makeLookAt      (const Vector3& vecEye, const Vector3& vecTarget, float roll);

  Matrix4 inverse        (void) const;
  Vector3 getTranslation (void) const;
  Vector3 getXAxis       (void) const;
  Vector3 getYAxis       (void) const;
  Vector3 getZAxis       (void) const;

  Vector3 operator* (const Vector3& vec) const;

  // D3DXMATRIX
  Matrix4 operator* (const Matrix4& mtx) const;


  // static utils
  static Matrix4 createTranslation (const Vector3& vecTranslate);
  static Matrix4 createRotation    (const Vector3& vecYawPitchRoll);
  static Matrix4 createRotation    (const Quaternion& quatRot);
  static Matrix4 createScaling     (const Vector3& vecScale);
  static Matrix4 createProjection  (float fovDegrees, float aspect, float znear, float zfar);
  static Matrix4 createLookAt      (const Vector3& vecEye, const Vector3& vecTarget, const Vector3& vecUp);
  static Matrix4 createLookAt      (const Vector3& vecEye, const Vector3& vecTarget, float roll);


public :
  // special vectors
  static const Matrix4 ZERO;
  static const Matrix4 IDENTITY;
};


#include "Matrix4.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_MATRIX4_H__
