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
// Created On : 2011-12-06 22:54:39
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline void Effect::setFloat (D3DXHANDLE hParam, float rValue)
{
  GLB_ASSERT(this->getRef());
  EFFECT_DXTEST(this->getRef()->SetFloat(hParam, rValue));
}

//---------------------------------------------------------------------------
inline void Effect::setFloatArray (D3DXHANDLE hParam, float* prValues, uint uiValuesCount)
{
  GLB_ASSERT(this->getRef());
  EFFECT_DXTEST(this->getRef()->SetFloatArray(hParam, prValues, uiValuesCount));
}

//---------------------------------------------------------------------------
inline void Effect::setVector (D3DXHANDLE hParam, const Vector3& vec)
{
  GLB_ASSERT(this->getRef());
  D3DXVECTOR4 vec4(vec.x, vec.y, vec.z, 1.0f);
  EFFECT_DXTEST(this->getRef()->SetVector(hParam, &vec4));
}

//---------------------------------------------------------------------------
inline void Effect::setMatrix (D3DXHANDLE hParam, const Matrix4& mtx)
{
  GLB_ASSERT(this->getRef());
  EFFECT_DXTEST(this->getRef()->SetMatrix(hParam, &mtx));
}

//---------------------------------------------------------------------------
inline void Effect::setTexture (D3DXHANDLE hParam, const Texture& tex)
{
  GLB_ASSERT(this->getRef());
  EFFECT_DXTEST(this->getRef()->SetTexture(hParam, (IDirect3DTexture9*)tex.getRef()));
}

//---------------------------------------------------------------------------
inline void Effect::setTexture (D3DXHANDLE hParam, const CubeTexture& tex)
{
  GLB_ASSERT(this->getRef());
  EFFECT_DXTEST(this->getRef()->SetTexture(hParam, (IDirect3DCubeTexture9*)tex.getRef()));
}

//---------------------------------------------------------------------------
inline void Effect::setTexture (D3DXHANDLE hParam, const VolumeTexture& tex)
{
  GLB_ASSERT(this->getRef());
  EFFECT_DXTEST(this->getRef()->SetTexture(hParam, (IDirect3DVolumeTexture9*)tex.getRef()));
}
