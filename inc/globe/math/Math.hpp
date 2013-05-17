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
// Created On : 2011-11-06 11:56:20
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_MATH_H__
#define __GLOBE_MATH_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------
#define GLOBE_MATH_PI        3.1415926535897932384626433832795
#define GLOBE_MATH_TWOPI     (GLOBE_MATH_PI * 2.0)
#define GLOBE_MATH_HALFPI    (GLOBE_MATH_PI / 2.0)
#define GLOBE_MATH_THIRDPI   (GLOBE_MATH_PI / 3.0)
#define GLOBE_MATH_QUARTERPI (GLOBE_MATH_PI / 4.0)

#define GLOBE_MATH_DEG2RAD  (GLOBE_MATH_PI / 180.0)
#define GLOBE_MATH_RAD2DEG  (180.0 / GLOBE_MATH_PI)

#define GLOBE_MATH_ZEROTOLERANCE  (1e-5)


//---------------------------------------------------------------------------
// Math
//---------------------------------------------------------------------------
class GLB_EXPORT Math
{
public :
  // calculation
  template <typename T> static T abs   (T val);
  template <typename T> static T ceil  (T val);
  template <typename T> static T exp   (T val);
  template <typename T> static T floor (T val);
  template <typename T> static T frac  (T val);
  template <typename T> static T log   (T val);
  template <typename T> static T log10 (T val);
  template <typename T> static T mod   (T x, T y);
  template <typename T> static T pow   (T val, T exponent);
  template <typename T> static T round (T val);
  template <typename T> static T sq    (T val);
  template <typename T> static T sqrt  (T val);

  // trigonometry
  template <typename T> static T acos  (T val);
  template <typename T> static T asin  (T val);
  template <typename T> static T atan  (T val);
  template <typename T> static T atan2 (T a, T b);
  template <typename T> static T cos   (T val);
  template <typename T> static T sin   (T val);
  template <typename T> static T tan   (T val);
  template <typename T> static T tanh  (T val);
  //
  template <typename T> static T degrees (T radians);
  template <typename T> static T radians (T degrees);

  // utils
  template <typename T> static T min (T a, T b);
  template <typename T> static T min (T a, T b, T c);
  template <typename T> static T max (T a, T b);
  template <typename T> static T max (T a, T b, T c);
  //
  template <typename T> static bool equal (T a, T b, T tolerance);
  //
  template <typename T> static T bezierPoint (T a, T b, T c, T d, T t);
  template <typename T> static T clamp       (T val, T low, T high);
  template <typename T> static T lerp        (T val, T low, T high);
  template <typename T> static T smoothStep  (T val, T low, T high);
  template <typename T> static T map         (T val, T low1, T high1, T low2, T high2);
  template <typename T> static T norm        (T val, T low, T high);
};


#include "Math.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_MATH_H__
