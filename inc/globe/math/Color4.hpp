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
// Created On: 2011-12-17 23:16:42
//
//***************************************************************************

#ifndef __GLOBE_COLOR4_H__
#define __GLOBE_COLOR4_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// Color4
//---------------------------------------------------------------------------
class GLB_EXPORT Color4 : public D3DXCOLOR
{
public :
  Color4 (void);
  Color4 (const D3DXCOLOR& rhs);
  Color4 (float r, float g, float b, float a=1.0f);
  Color4 (double r, double g, double b, double a=1.0f);


public :
  // special colors
  static const Color4 ZERO;
  static const Color4 BLACK;
  static const Color4 WHITE;
  static const Color4 RED;
  static const Color4 GREEN;
  static const Color4 BLUE;
  static const Color4 YELLOW;
  static const Color4 PURPLE;
  static const Color4 CYAN;
};


#include "Color4.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_COLOR4_H__
