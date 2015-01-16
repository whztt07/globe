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
// Created On: 2011-12-16 03:01:47
//
//***************************************************************************

#ifndef __GLOBE_SCENECAMERA_H__
#define __GLOBE_SCENECAMERA_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// SceneCamera
//---------------------------------------------------------------------------
class GLB_EXPORT SceneCamera : public SceneNode
{
public :
  SceneCamera (const char* pszName=NULL);

  void setFOV    (float rDegrees);
  void setAspect (float rAspectRatio);
  void setZNear  (float rZNear);
  void setZFar   (float rZFar);

  const Matrix4& getProjection (void) const;


protected :
  float m_rFOV;    // degrees
  float m_rAspect;
  float m_rZNear;
  float m_rZFar;

  mutable Matrix4 m_mtxProj;
  mutable bool    m_bProjDirty;
};


#include "SceneCamera.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_SCENECAMERA_H__
