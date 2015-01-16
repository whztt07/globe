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
// Created On: 2011-12-10 23:55:58
//
//***************************************************************************

#include "_internal.hpp"
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
D3DXFONT_DESC Font::getDesc (void)
{
  D3DXFONT_DESC desc;

  GLB_ASSERT(this->getRef());
  GLB_DXTEST(this->getRef()->GetDesc(&desc));

  return desc;
}

//---------------------------------------------------------------------------
TEXTMETRIC Font::getMetrics (void)
{
  TEXTMETRIC tm;

  GLB_ASSERT(this->getRef());
  GLB_DXTEST(this->getRef()->GetTextMetrics(&tm));

  return tm;
}


} // namespace glb
#endif // #ifdef GLOBE_GFX
