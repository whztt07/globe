//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-11-30 03:39:05
//
// $Id$
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
