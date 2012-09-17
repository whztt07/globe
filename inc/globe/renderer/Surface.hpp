//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-04 20:02:06
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_SURFACE_H__
#define __GLOBE_SURFACE_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// Surface
//---------------------------------------------------------------------------
class GLB_EXPORT Surface : public SmartPointer<IDirect3DSurface9>
{
public :
  Surface (IDirect3DSurface9* pSurface=0);

  // properties getters
  uint            getWidth  (void);
  uint            getHeight (void);
  D3DFORMAT       getFormat (void);
  D3DSURFACE_DESC getDesc   (void);
};


#include "Surface.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_SURFACE_H__
