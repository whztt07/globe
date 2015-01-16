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
// Created On: 2011-12-04 19:59:05
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
