//***************************************************************************
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
