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
// Created On : 2012-09-17 16:56:16
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline bool Random::isInit (void)
{
  return ms_bInit;
}




//---------------------------------------------------------------------------
GLB_FORCEINLINE uint32 Random::getUint32 (void)
{
  GLB_ASSERT(ms_bInit);
  return Random::rand32_uint32();
}

//---------------------------------------------------------------------------
GLB_FORCEINLINE uint32 Random::getUint32 (uint32 uiMin, uint32 uiMax)
{
  GLB_ASSERT(ms_bInit);
  GLB_ASSERT(uiMax > uiMin);
  return Random::rand32_uint32() % (uiMax - uiMin + 1) + uiMin;
}

//---------------------------------------------------------------------------
GLB_FORCEINLINE uint64 Random::getUint64 (void)
{
  GLB_ASSERT(ms_bInit);
  return Random::rand64_uint64();
}

//---------------------------------------------------------------------------
GLB_FORCEINLINE uint64 Random::getUint64 (uint64 uiMin, uint64 uiMax)
{
  GLB_ASSERT(ms_bInit);
  GLB_ASSERT(uiMax > uiMin);
  return Random::rand64_uint64() % (uiMax - uiMin + 1) + uiMin;
}

//---------------------------------------------------------------------------
template<> GLB_FORCEINLINE float32 Random::getUnit (void)
{
  GLB_ASSERT(ms_bInit);
  return Random::rand32_float();
}

//---------------------------------------------------------------------------
template<> GLB_FORCEINLINE float64 Random::getUnit (void)
{
  GLB_ASSERT(ms_bInit);
  return Random::rand64_double();
}

//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Random::getUnit (void)
{
  GLB_ASSERT(ms_bInit);
  return (T)Random::rand64_double()
}

//---------------------------------------------------------------------------
template<> GLB_FORCEINLINE float32 Random::getSymmetric (void)
{
  GLB_ASSERT(ms_bInit);
  return 2.0f * Random::rand32_float() - 1.0f;
}

//---------------------------------------------------------------------------
template<> GLB_FORCEINLINE float64 Random::getSymmetric (void)
{
  GLB_ASSERT(ms_bInit);
  return 2.0 * Random::rand64_double() - 1.0;
}

//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Random::getSymmetric (void)
{
  GLB_ASSERT(ms_bInit);
  return T(2.0 * Random::rand64_double() - 1.0);
}

//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Random::getRange (T rMin, T rMax)
{
  GLB_ASSERT(ms_bInit);
  return (rMax - rMin) * Random::getUnit<T>() + rMin;
}
