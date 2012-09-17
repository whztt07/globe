//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-04 20:59:57
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_VOLUMETEXTURE_H__
#define __GLOBE_VOLUMETEXTURE_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// VolumeTexture
//---------------------------------------------------------------------------
class GLB_EXPORT VolumeTexture : public SmartPointer<IDirect3DVolumeTexture9>
{
public :
  VolumeTexture (IDirect3DVolumeTexture9* pVolumeTexture=0);
};


#include "VolumeTexture.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_VOLUMETEXTURE_H__
