//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-04 19:59:07
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
Surface Texture::getLevel (uint uiLevel/*=0*/)
{
  IDirect3DSurface9* pSurf;

  GLB_ASSERT(this->getRef());
  GLB_DXTEST(this->getRef()->GetSurfaceLevel(uiLevel, &pSurf));
  pSurf->Release(); // GetSurfaceLevel() incremented the ref counter

  return Surface(pSurf);
}

//---------------------------------------------------------------------------
D3DSURFACE_DESC Texture::getLevelDesc (uint uiLevel/*=0*/)
{
  D3DSURFACE_DESC d3dSurfDesc;

  GLB_ASSERT(this->getRef());
  GLB_DXTEST(this->getRef()->GetLevelDesc(uiLevel, &d3dSurfDesc));

  return d3dSurfDesc;
}


} // namespace glb
