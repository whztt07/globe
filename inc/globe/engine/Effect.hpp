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
// Created On : 2011-12-06 22:54:37
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_EFFECT_H__
#define __GLOBE_EFFECT_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
#ifdef GLB_DEBUG
# define EFFECT_DXTEST(call)  GLB_DXTEST(call)
#else
# define EFFECT_DXTEST(call)  call
#endif


//---------------------------------------------------------------------------
// Effect
//---------------------------------------------------------------------------
class GLB_EXPORT Effect : public SmartPointer<ID3DXEffect>
{
public :
  Effect (void);

  // parameters
  D3DXHANDLE getParam           (const char* pszParamName, D3DXHANDLE hParent=NULL, bool bThrowError=true);
  D3DXHANDLE getParamBySemantic (const char* pszParamSemantic, D3DXHANDLE hParent=NULL, bool bThrowError=true);
  void       setMatrices        (const Matrix4& mtxWorld, const Matrix4& mtxView, const Matrix4& mtxProj);
  void       setFloat           (D3DXHANDLE hParam, float rValue);
  void       setFloatArray      (D3DXHANDLE hParam, float* prValues, uint uiValuesCount);
  void       setVector          (D3DXHANDLE hParam, const Vector3& vec);
  void       setMatrix          (D3DXHANDLE hParam, const Matrix4& mtx);
  void       setTexture         (D3DXHANDLE hParam, const Texture& tex);
  void       setTexture         (D3DXHANDLE hParam, const CubeTexture& tex);
  void       setTexture         (D3DXHANDLE hParam, const VolumeTexture& tex);

  // draw
  void draw (Drawable* pDrawable);

  // static utils
  static Effect load (const char* pszVirtPath);


private :
  void onEffectAttached (void);


private :
  // standard semantics
  D3DXHANDLE m_hWorld;
  D3DXHANDLE m_hView;
  D3DXHANDLE m_hProj;
  D3DXHANDLE m_hWorldView;
  D3DXHANDLE m_hWorldViewInv;
  D3DXHANDLE m_hWorldViewProj;
  D3DXHANDLE m_hWorldViewProjInv;

  // miscellaneous semantics
  D3DXHANDLE m_hViewPos; // "ViewPosition"
  D3DXHANDLE m_hViewDir; // "ViewDirection"
};


#include "Effect.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_EFFECT_H__
