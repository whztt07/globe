//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-04 19:59:03
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_TEXTURE_H__
#define __GLOBE_TEXTURE_H__
namespace glb {


//---------------------------------------------------------------------------
// Texture
//---------------------------------------------------------------------------
class GLB_EXPORT Texture : public SmartPointer<IDirect3DTexture9>
{
public :
  Texture (IDirect3DTexture9* pTexture=0);

  // levels
  Surface         getLevel       (uint uiLevel=0);
  D3DSURFACE_DESC getLevelDesc   (uint uiLevel=0);
  uint            getLevelWidth  (uint uiLevel=0);
  uint            getLevelHeight (uint uiLevel=0);
  D3DFORMAT       getLevelFormat (uint uiLevel=0);
};


#include "Texture.inl.hpp"


} // namespace glb
#endif // #ifndef __GLOBE_TEXTURE_H__
