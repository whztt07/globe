//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-16 03:01:49
//
// $Id$
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
