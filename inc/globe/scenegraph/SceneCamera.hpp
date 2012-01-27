//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-16 03:01:47
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_SCENECAMERA_H__
#define __GLOBE_SCENECAMERA_H__
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
#endif // #ifndef __GLOBE_SCENECAMERA_H__
