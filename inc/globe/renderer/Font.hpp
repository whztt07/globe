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
// Created On : 2011-12-10 23:55:52
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_FONT_H__
#define __GLOBE_FONT_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// Font
//---------------------------------------------------------------------------
class GLB_EXPORT Font : public SmartPointer<ID3DXFont>
{
public :
  Font (ID3DXFont* pFont=0);

  // properties
  D3DXFONT_DESC getDesc    (void);
  TEXTMETRIC    getMetrics (void);
};


#include "Font.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_FONT_H__
