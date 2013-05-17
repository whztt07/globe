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
// Created On : 2011-11-29 14:44:05
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline Vector3::Vector3 (void)
{
}

//---------------------------------------------------------------------------
inline Vector3::Vector3 (const D3DXVECTOR3& rhs)
: D3DXVECTOR3(rhs)
{
}

//---------------------------------------------------------------------------
inline Vector3::Vector3 (double x, double y, double z)
: D3DXVECTOR3(float(x), float(y), float(z))
{
}




//---------------------------------------------------------------------------
inline float Vector3::length (void) const
{
  return Math::sqrt<float>(
    x * x +
    y * y +
    z * z);
}

//---------------------------------------------------------------------------
inline float Vector3::squaredLength (void) const
{
  return
    x * x +
    y * y +
    z * z;
}

//---------------------------------------------------------------------------
inline float Vector3::dot (const Vector3& rhs) const
{
  return
    x * rhs.x +
    y * rhs.y +
    z * rhs.z;
}

//---------------------------------------------------------------------------
inline float Vector3::normalize (void)
{
  float len = this->length();

  if (len > GLOBE_MATH_ZEROTOLERANCE)
  {
    float invlen = 1.0f / len;
    x *= invlen;
    y *= invlen;
    z *= invlen;
  }
  else
  {
    len = 0.0f;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }

  return len;
}




//---------------------------------------------------------------------------
inline Vector3 Vector3::cross (const Vector3& rhs) const
{
  return Vector3(
    y * rhs.z - z * rhs.y,
    z * rhs.x - x * rhs.z,
    x * rhs.y - y * rhs.x);
}

//---------------------------------------------------------------------------
inline Vector3 Vector3::unitCross (const Vector3& rhs) const
{
  Vector3 vec(
    y * rhs.z - z * rhs.y,
    z * rhs.x - x * rhs.z,
    x * rhs.y - y * rhs.x);
  vec.normalize();
  return vec;
}
