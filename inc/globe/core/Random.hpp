//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2012-09-17 16:56:10
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_RANDOM_H__
#define __GLOBE_RANDOM_H__
namespace glb {


//---------------------------------------------------------------------------
// Random
//---------------------------------------------------------------------------
class GLB_EXPORT Random
{
public :
  // init
  static void init   (uint32 uiSeed32=0, uint64 uiSeed64=0);
  static bool isInit (void);

  // randomness - seed
  static void setSeeds (uint32 uiSeed32, uint64 uiSeed64);

  // randomness - mersenne-twister (linear)
                        static uint32 getUint32    (void); //  0   <= r < 2^32
                        static uint32 getUint32    (uint32 uiMin, uint32 uiMax);
                        static uint64 getUint64    (void); //  0   <= r < 2^64
                        static uint64 getUint64    (uint64 uiMin, uint64 uiMax);
  template <typename T> static T      getUnit      (void); //  0.0 <= r < 1.0
  template <typename T> static T      getSymmetric (void); // -1.0 <= r < 1.0
  template <typename T> static T      getRange     (T rMin, T rMax); // min <= r < max
                        static void   getBytes     (glb_byte* pDestBuffer, uint uiSize);

  // randomness - gaussian (normaly distributed)
  template <typename T> static T getGaussian (void);

  // randomness - security
  // more expensive than the regular methods for the sake of security
  static void getBytesSecured (glb_byte* pDestBuffer, uint uiSize);


private :
  static uint32  rand32_uint32 (void);
  static float32 rand32_float  (void);
  static uint64  rand64_uint64 (void);
  static float64 rand64_double (void);

  static double boxMuller (double m, double s);


private :
  static bool ms_bInit;
};


#include "Random.inl.hpp"


} // namespace glb
#endif // #ifndef __GLOBE_RANDOM_H__
