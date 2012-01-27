//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-11-06 11:56:20
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_MATH_H__
#define __GLOBE_MATH_H__
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
  // init
  static void init   (uint32 uiSeed32=0, uint64 uiSeed64=0);
  static bool isInit (void);

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

  // randomness - seed
  static void randSeeds (uint32 uiSeed32, uint64 uiSeed64);

  // randomness - mersenne-twister (linear)
                        static uint32 randUint32    (void); //  0   <= r < 2^32
                        static uint32 randUint32    (uint32 uiMin, uint32 uiMax);
                        static uint64 randUint64    (void); //  0   <= r < 2^64
                        static uint64 randUint64    (uint64 uiMin, uint64 uiMax);
  template <typename T> static T      randUnit      (void); //  0.0 <= r < 1.0
  template <typename T> static T      randSymmetric (void); // -1.0 <= r < 1.0
  template <typename T> static T      randRange     (T rMin, T rMax); // min <= r < max
                        static void   randBytes     (glb_byte* pDestBuffer, uint uiSize);

  // randomness - gaussian (normaly distributed)
  template <typename T> static T randGaussian (void);

  // randomness - security
  // more expensive than the regular methods for the sake of security
  static void randBytesSecured (glb_byte* pDestBuffer, uint uiSize);


private :
  static uint32  rand32_uint32 (void);
  static float32 rand32_float  (void);
  static uint64  rand64_uint64 (void);
  static float64 rand64_double (void);

  static double boxMuller (double m, double s);


private :
  static bool ms_bInit;
};


#include "Math.inl.hpp"


} // namespace glb
#endif // #ifndef __GLOBE_MATH_H__
