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
// Created On: 2011-12-16 03:01:49
//
//***************************************************************************


//---------------------------------------------------------------------------
inline SceneCamera::SceneCamera (const char* pszName/*=NULL*/)
: SceneNode(NODE_CAMERA, pszName)
, m_rFOV(60.0f)
, m_rAspect(float(4.0 / 3.0))
, m_rZNear(1.0f)
, m_rZFar(1000.0f)
, m_bProjDirty(true)
{
}




//---------------------------------------------------------------------------
inline void SceneCamera::setFOV (float rDegrees)
{
  m_rFOV = rDegrees;
  m_bProjDirty = true;
}

//---------------------------------------------------------------------------
inline void SceneCamera::setAspect (float rAspectRatio)
{
  m_rAspect = rAspectRatio;
  m_bProjDirty = true;
}

//---------------------------------------------------------------------------
inline void SceneCamera::setZNear (float rZNear)
{
  m_rZNear = rZNear;
  m_bProjDirty = true;
}

//---------------------------------------------------------------------------
inline void SceneCamera::setZFar (float rZFar)
{
  m_rZFar = rZFar;
  m_bProjDirty = true;
}




//---------------------------------------------------------------------------
inline const Matrix4& SceneCamera::getProjection (void) const
{
  if (m_bProjDirty)
  {
    m_mtxProj.makeProjection(m_rFOV, m_rAspect, m_rZNear, m_rZFar);
    m_bProjDirty = false;
  }

  return m_mtxProj;
}
