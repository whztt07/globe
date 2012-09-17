//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-11-30 03:39:10
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
const Matrix4 Matrix4::ZERO = Matrix4(
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f);

const Matrix4 Matrix4::IDENTITY = Matrix4(
  1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f);


} // namespace glb
#endif // #ifdef GLOBE_GFX
