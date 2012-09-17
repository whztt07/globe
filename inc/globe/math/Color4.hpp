//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-17 23:16:42
//
// $Id$
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
