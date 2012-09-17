//***************************************************************************
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
