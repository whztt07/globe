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
// Created On : 2011-11-29 14:43:24
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline Vector2::Vector2 (void)
{
}

//---------------------------------------------------------------------------
inline Vector2::Vector2 (const D3DXVECTOR2& rhs)
: D3DXVECTOR2(rhs)
{
}

//---------------------------------------------------------------------------
inline Vector2::Vector2 (double x, double y)
: D3DXVECTOR2(float(x), float(y))
{
}




//---------------------------------------------------------------------------
inline float Vector2::length (void) const
{
  return Math::sqrt<float>(x * x + y * y);
}

//---------------------------------------------------------------------------
inline float Vector2::squaredLength (void) const
{
  return x * x + y * y;
}

//---------------------------------------------------------------------------
inline float Vector2::dot (const Vector2& rhs) const
{
  return x * rhs.x + y * rhs.y;
}

//---------------------------------------------------------------------------
inline float Vector2::normalize (void)
{
  float len = this->length();

  if (len > GLOBE_MATH_ZEROTOLERANCE)
  {
    float invlen = 1.0f / len;
    x *= invlen;
    y *= invlen;
  }
  else
  {
    len = 0.0f;
    x = 0.0f;
    y = 0.0f;
  }

  return len;
}




//---------------------------------------------------------------------------
inline Vector2 Vector2::perp (void) const
{
  return Vector2(y, -x);
}

//---------------------------------------------------------------------------
inline Vector2 Vector2::unitPerp (void) const
{
  Vector2 vec(y, -x);
  vec.normalize();
  return vec;
}

//---------------------------------------------------------------------------
inline float Vector2::dotPerp (const Vector2& rhs) const
{
  return
    x * rhs.y -
    y * rhs.x;
}
