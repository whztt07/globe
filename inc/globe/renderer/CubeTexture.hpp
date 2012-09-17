//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-04 20:59:46
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_CUBETEXTURE_H__
#define __GLOBE_CUBETEXTURE_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// CubeTexture
//---------------------------------------------------------------------------
class GLB_EXPORT CubeTexture : public SmartPointer<IDirect3DCubeTexture9>
{
public :
  CubeTexture (IDirect3DCubeTexture9* pCubeTexture=0);
};


#include "CubeTexture.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_CUBETEXTURE_H__
