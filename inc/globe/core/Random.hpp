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
