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
// Created On: 2011-12-06 22:54:42
//
//***************************************************************************

#include "_internal.hpp"
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
Effect::Effect (void)
: SmartPointer<ID3DXEffect>()
, m_hWorld(0)
, m_hView(0)
, m_hProj(0)
, m_hWorldView(0)
, m_hWorldViewInv(0)
, m_hWorldViewProj(0)
, m_hWorldViewProjInv(0)
, m_hViewPos(0)
, m_hViewDir(0)
{
}

//---------------------------------------------------------------------------
D3DXHANDLE Effect::getParam (const char* pszParamName, D3DXHANDLE hParent/*=NULL*/, bool bThrowError/*=true*/)
{
  D3DXHANDLE hParam;

  GLB_ASSERT(this->getRef());

  hParam = this->getRef()->GetParameterByName(hParent, pszParamName);
  if (!hParam && bThrowError)
    GLB_THROW(EXCODE_PARAM, "Effect parameter not found by name \"%s\" !", pszParamName);

  return hParam;
}

//---------------------------------------------------------------------------
D3DXHANDLE Effect::getParamBySemantic (const char* pszParamSemantic, D3DXHANDLE hParent/*=NULL*/, bool bThrowError/*=true*/)
{
  D3DXHANDLE hParam;

  GLB_ASSERT(this->getRef());

  hParam = this->getRef()->GetParameterBySemantic(hParent, pszParamSemantic);
  if (!hParam && bThrowError)
    GLB_THROW(EXCODE_PARAM, "Effect parameter not found by semantic \"%s\" !", pszParamSemantic);

  return hParam;
}

//---------------------------------------------------------------------------
void Effect::setMatrices (const Matrix4& mtxWorld, const Matrix4& mtxView, const Matrix4& mtxProj)
{
  Matrix4 mtxWorldView;
  Matrix4 mtxWorldViewInv;
  Matrix4 mtxWorldViewProj;
  Matrix4 mtxWorldViewProjInv;

  GLB_ASSERT(this->getRef());

  if (m_hWorldView || m_hWorldViewInv || m_hViewPos || m_hViewDir)
    mtxWorldView = mtxWorld * mtxView;

  if (m_hWorldViewInv || m_hViewPos)
    mtxWorldViewInv = mtxWorldView.inverse();

  if (m_hWorldViewProj || m_hWorldViewProjInv)
    mtxWorldViewProj = mtxWorld * mtxView * mtxProj;

  if (m_hWorldViewProjInv)
    mtxWorldViewProjInv = mtxWorldViewProj.inverse();


  if (m_hWorld)
    this->setMatrix(m_hWorld, mtxWorld);
  if (m_hView)
    this->setMatrix(m_hView, mtxView);
  if (m_hProj)
    this->setMatrix(m_hProj, mtxProj);

  if (m_hWorldView)
    this->setMatrix(m_hWorldView, mtxWorldView);
  if (m_hWorldViewInv)
    this->setMatrix(m_hWorldViewInv, mtxWorldViewInv);

  if (m_hWorldViewProj)
    this->setMatrix(m_hWorldViewProj, mtxWorldViewProj);
  if (m_hWorldViewProjInv)
    this->setMatrix(m_hWorldViewProjInv, mtxWorldViewProjInv);

  if (m_hViewPos)
    this->setVector(m_hViewPos, mtxWorldViewInv.getTranslation());
  if (m_hViewDir)
    this->setVector(m_hViewDir, mtxWorldView.getZAxis());
}

//---------------------------------------------------------------------------
void Effect::draw (Drawable* pDrawable)
{
  ID3DXEffect* pEffect = this->getRef();
  UINT uiPasses = 0;

  GLB_ASSERT(pEffect);

  pEffect->Begin(&uiPasses, 0);
  for (UINT i = 0; i < uiPasses; ++i)
  {
    pEffect->BeginPass(i);
    pDrawable->draw();
    pEffect->EndPass();
  }
  pEffect->End();
}




//---------------------------------------------------------------------------
Effect Effect::load (const char* pszVirtPath)
{
  ID3DXEffect* pEffect;
  ID3DXBuffer* pErrBuf;
  StringA      strFile;
  DWORD        dwFlags = 0;
  HRESULT      hRes;

  GLB_ASSERT(g_pVFS());
  GLB_ASSERT(g_pRenderer());

#ifdef GLB_DEBUG
  dwFlags |= D3DXSHADER_DEBUG;
#else
  //dwFlags |= D3DXSHADER_SKIPVALIDATION;
#endif

  strFile = g_VFS().acquirePhysFile(pszVirtPath);
  hRes = D3DXCreateEffectFromFile(
    g_Renderer().getDev(), strFile.c_str(), NULL, NULL, 0, NULL, &pEffect,
    &pErrBuf);
  g_VFS().releasePhysFile(strFile);

  if (FAILED(hRes))
  {
    StringA strMessage;

    strMessage.format("Failed to load D3D effect \"%s\" (%s) !", pszVirtPath, DXGetErrorString9(hRes));
    if (pErrBuf)
      strMessage.formatAppend(" Compilation result :\n%s", (char*)pErrBuf->GetBufferPointer());

    GLB_THROW(EXCODE_SYSTEM_API, strMessage.c_str());
  }

  Effect effect;
  effect.attachRef(pEffect);
  effect.onEffectAttached();
  return effect;
}

//---------------------------------------------------------------------------
void Effect::onEffectAttached (void)
{
  m_hWorld            = this->getParamBySemantic("World", NULL, false);
  m_hWorldView        = this->getParamBySemantic("WorldView", NULL, false);
  m_hWorldViewInv     = this->getParamBySemantic("WorldViewInverse", NULL, false);
  m_hWorldViewProj    = this->getParamBySemantic("WorldViewProjection", NULL, false);
  m_hWorldViewProjInv = this->getParamBySemantic("WorldViewProjectionInverse", NULL, false);
  m_hView             = this->getParamBySemantic("View", NULL, false);
  m_hProj             = this->getParamBySemantic("Projection", NULL, false);

  m_hViewPos = this->getParamBySemantic("ViewPosition", NULL, false);
  m_hViewDir = this->getParamBySemantic("ViewDirection", NULL, false);
}


} // namespace glb
#endif // #ifdef GLOBE_GFX
