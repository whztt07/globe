//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-04 19:59:05
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline Texture::Texture (IDirect3DTexture9* pTexture/*=0*/)
: SmartPointer<IDirect3DTexture9>(pTexture)
{
}

//---------------------------------------------------------------------------
inline uint Texture::getLevelWidth (uint uiLevel/*=0*/)
{
  GLB_ASSERT(this->getRef());
  return this->getLevelDesc(uiLevel).Width;
}

//---------------------------------------------------------------------------
inline uint Texture::getLevelHeight (uint uiLevel/*=0*/)
{
  GLB_ASSERT(this->getRef());
  return this->getLevelDesc(uiLevel).Height;
}

//---------------------------------------------------------------------------
inline D3DFORMAT Texture::getLevelFormat (uint uiLevel/*=0*/)
{
  GLB_ASSERT(this->getRef());
  return this->getLevelDesc(uiLevel).Format;
}
