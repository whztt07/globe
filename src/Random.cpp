//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2012-09-17 16:56:49
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
bool Random::ms_bInit = false;

static tinymt32_t ms_tinymt32 = { { 0, 0, 0, 0 }, 0xe51b1ca3, 0xf720fdc9, 0xf8ebffff };
static tinymt64_t ms_tinymt64 = { { 0, 0 }, 0xe31e1c63, 0xef08fbc2, 0x7197ddf7dfffdfbcULL };



//---------------------------------------------------------------------------
void Random::init (uint32 uiSeed32/*=0*/, uint64 uiSeed64/*=0*/)
{
  if (!ms_bInit)
  {
    Random::setSeeds(uiSeed32, uiSeed64);
    ms_bInit = true;
  }
}




//---------------------------------------------------------------------------
void Random::setSeeds (uint32 uiSeed32, uint64 uiSeed64)
{
  tinymt32_init(&ms_tinymt32, uiSeed32);
  tinymt64_init(&ms_tinymt64, uiSeed64);
}

//---------------------------------------------------------------------------
uint32 Random::rand32_uint32 (void)
{
  GLB_ASSERT(ms_bInit);
  return tinymt32_generate_uint32(&ms_tinymt32);
}

//---------------------------------------------------------------------------
float32 Random::rand32_float (void)
{
  GLB_ASSERT(ms_bInit);
  return tinymt32_generate_float(&ms_tinymt32);
}

//---------------------------------------------------------------------------
uint64 Random::rand64_uint64 (void)
{
  GLB_ASSERT(ms_bInit);
  return tinymt64_generate_uint64(&ms_tinymt64);
}

//---------------------------------------------------------------------------
float64 Random::rand64_double (void)
{
  GLB_ASSERT(ms_bInit);
  return tinymt64_generate_double(&ms_tinymt64);
}

//---------------------------------------------------------------------------
void Random::getBytes (glb_byte* pDestBuffer, uint uiSize)
{
  GLB_ASSERT(ms_bInit);

  while (uiSize >= sizeof(uint64))
  {
    (*(uint64*)pDestBuffer) = tinymt64_generate_uint64(&ms_tinymt64);
    pDestBuffer += sizeof(uint64);
    uiSize      -= sizeof(uint64);
  }

  while (uiSize > 0)
  {
    *pDestBuffer = (glb_byte)tinymt32_generate_uint32(&ms_tinymt32);
    ++pDestBuffer;
    --uiSize;
  }
}

//---------------------------------------------------------------------------
void Random::getBytesSecured (glb_byte* pDestBuffer, uint uiSize)
{
  GLB_ASSERT(ms_bInit);

  // recommended in the mersenne-twister generator's FAQ since it is not
  // cryptographically secure
  const uint c_uiSecurityStep = 8;

  while (uiSize >= sizeof(uint64))
  {
    for (uint i = 0; i < c_uiSecurityStep; ++i)
      (void)tinymt64_generate_uint64(&ms_tinymt64);

    (*(uint64*)pDestBuffer) = tinymt64_generate_uint64(&ms_tinymt64);
    pDestBuffer += sizeof(uint64);
    uiSize      -= sizeof(uint64);
  }

  while (uiSize > 0)
  {
    for (uint i = 0; i < c_uiSecurityStep; ++i)
      (void)tinymt32_generate_uint32(&ms_tinymt32);

    *pDestBuffer = (glb_byte)tinymt32_generate_uint32(&ms_tinymt32);
    ++pDestBuffer;
    --uiSize;
  }
}

//---------------------------------------------------------------------------
template <typename T> T Random::getGaussian (void)
{
  GLB_ASSERT(ms_bInit);
  return (T)Random::boxMuller(0.0, 1.0);
}

//---------------------------------------------------------------------------
GLB_FORCEINLINE double Random::boxMuller (double m, double s)
{
  // Implements the Polar form of the Box-Muller Transformation
  //
  // (c) Copyright 1994, Everett F. Carter Jr.
  // Permission is granted by the author to use this software for any
  // application provided this copyright notice is preserved.

  // normal random variate generator
  // mean m, standard deviation s

  static double y2;
  static bool use_last = 0;

  double x1, x2, w, y1;

  if (use_last) // use value from previous call
  {
    y1 = y2;
    use_last = false;
  }
  else
  {
    do
    {
      x1 = 2.0 * Random::getUnit<double>() - 1.0;
      x2 = 2.0 * Random::getUnit<double>() - 1.0;
      w  = x1 * x1 + x2 * x2;
    }
    while (w >= 1.0);

    w  = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;
    y2 = x2 * w;
    use_last = true;
  }

  return (m + y1 * s);
}


} // namespace glb
