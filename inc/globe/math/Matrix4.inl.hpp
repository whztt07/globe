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
// Created On: 2011-11-30 03:39:08
//
//***************************************************************************


//---------------------------------------------------------------------------
inline Matrix4::Matrix4 (void)
{
}

//---------------------------------------------------------------------------
inline Matrix4::Matrix4 (const Matrix4& rhs)
: D3DXMATRIX(rhs)
{
}

//---------------------------------------------------------------------------
inline Matrix4::Matrix4 (const D3DXMATRIX& rhs)
: D3DXMATRIX(rhs)
{
}

//---------------------------------------------------------------------------
inline Matrix4::Matrix4 (float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44)
: D3DXMATRIX(_11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44)
{
}




//---------------------------------------------------------------------------
inline void Matrix4::makeZero (void)
{
  *this = Matrix4::ZERO;
}

//---------------------------------------------------------------------------
inline void Matrix4::makeIdentity (void)
{
  *this = Matrix4::IDENTITY;
}

//---------------------------------------------------------------------------
inline void Matrix4::makeTranslation (const Vector3& vecTranslate)
{
  D3DXMatrixTranslation(
    this,
    vecTranslate.x,
    vecTranslate.y,
    vecTranslate.z);
}

//---------------------------------------------------------------------------
inline void Matrix4::makeRotation (const Vector3& vecYawPitchRoll)
{
  D3DXMatrixRotationYawPitchRoll(
    this,
    vecYawPitchRoll.x,
    vecYawPitchRoll.y,
    vecYawPitchRoll.z);
}

//---------------------------------------------------------------------------
inline void Matrix4::makeRotation (const Quaternion& quatRot)
{
  D3DXMatrixRotationQuaternion(this, &quatRot);
}

//---------------------------------------------------------------------------
inline void Matrix4::makeScaling (const Vector3& vecScale)
{
  D3DXMatrixScaling(
    this,
    vecScale.x,
    vecScale.y,
    vecScale.z);
}

//---------------------------------------------------------------------------
inline void Matrix4::makeProjection (float fovDegrees, float aspect, float znear, float zfar)
{
  D3DXMatrixPerspectiveFovLH(
    this,
    D3DXToRadian(fovDegrees),
    aspect,
    znear,
    zfar);
}

//---------------------------------------------------------------------------
inline void Matrix4::makeLookAt (const Vector3& vecEye, const Vector3& vecTarget, const Vector3& vecUp)
{
  D3DXMatrixLookAtLH(this, &vecEye, &vecTarget, &vecUp);
}

//---------------------------------------------------------------------------
inline void Matrix4::makeLookAt (const Vector3& vecEye, const Vector3& vecTarget, float roll)
{
  D3DXVECTOR3 vecUp(0.0f, 1.0f, 0.0f);
  D3DXMatrixLookAtLH(this, &vecEye, &vecTarget, &vecUp);
  *this *= Matrix4::createRotation(Vector3(0.0f, 0.0f, roll));
}




//---------------------------------------------------------------------------
inline Matrix4 Matrix4::inverse (void) const
{
  Matrix4 mtx;
  D3DXMatrixInverse(&mtx, NULL, this);
  return mtx;
}

//---------------------------------------------------------------------------
inline Vector3 Matrix4::getTranslation (void) const
{
  return Vector3(_41, _42, _43);
}

//---------------------------------------------------------------------------
inline Vector3 Matrix4::getXAxis (void) const
{
  return Vector3(_11, _21, _31);
}

//---------------------------------------------------------------------------
inline Vector3 Matrix4::getYAxis (void) const
{
  return Vector3(_12, _22, _32);
}

//---------------------------------------------------------------------------
inline Vector3 Matrix4::getZAxis (void) const
{
  return Vector3(_13, _23, _33);
}




//---------------------------------------------------------------------------
inline Vector3 Matrix4::operator* (const Vector3& vec) const
{
  D3DXVECTOR4 vecTmp;
  D3DXVec3Transform(&vecTmp, &vec, this);
  return Vector3(vecTmp.x, vecTmp.y, vecTmp.z);
}




//---------------------------------------------------------------------------
inline Matrix4 Matrix4::operator* (const Matrix4& mtx) const
{
  return (*(D3DXMATRIX*)this) * (*(D3DXMATRIX*)&mtx);
}




//---------------------------------------------------------------------------
inline Matrix4 Matrix4::createTranslation (const Vector3& vecTranslate)
{
  Matrix4 mtx;
  mtx.makeTranslation(vecTranslate);
  return mtx;
}

//---------------------------------------------------------------------------
inline Matrix4 Matrix4::createRotation (const Vector3& vecYawPitchRoll)
{
  Matrix4 mtx;
  mtx.makeRotation(vecYawPitchRoll);
  return mtx;
}

//---------------------------------------------------------------------------
inline Matrix4 Matrix4::createRotation (const Quaternion& quatRot)
{
  Matrix4 mtx;
  mtx.makeRotation(quatRot);
  return mtx;
}

//---------------------------------------------------------------------------
inline Matrix4 Matrix4::createScaling (const Vector3& vecScale)
{
  Matrix4 mtx;
  mtx.makeScaling(vecScale);
  return mtx;
}

//---------------------------------------------------------------------------
inline Matrix4 Matrix4::createProjection (float fovDegrees, float aspect, float znear, float zfar)
{
  Matrix4 mtx;
  mtx.makeProjection(fovDegrees, aspect, znear, zfar);
  return mtx;
}

//---------------------------------------------------------------------------
inline Matrix4 Matrix4::createLookAt (const Vector3& vecEye, const Vector3& vecTarget, const Vector3& vecUp)
{
  Matrix4 mtx;
  mtx.makeLookAt(vecEye, vecTarget, vecUp);
  return mtx;
}

//---------------------------------------------------------------------------
inline Matrix4 Matrix4::createLookAt (const Vector3& vecEye, const Vector3& vecTarget, float roll)
{
  Matrix4 mtx;
  mtx.makeLookAt(vecEye, vecTarget, roll);
  return mtx;
}
