//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-08 01:53:38
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_MESH_H__
#define __GLOBE_MESH_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// Mesh
//---------------------------------------------------------------------------
class GLB_EXPORT Mesh : public SmartPointer<ID3DXMesh>, public Drawable
{
public :
  Mesh (ID3DXMesh* pMesh=0);

  // properties
  VertexBuffer getVertexBuffer (void);
  IndexBuffer  getIndexBuffer  (void);

  // draw
  Drawable* drawable (void);
  void      draw     (void); // from Drawable, do a draw(0) call
  void      draw     (uint uiSubset);


  // static utils
  static Mesh createBox      (float rWidth, float rHeight, float rDepth);
  static Mesh createCylinder (float rRadius1, float rRadius2, float rLength, uint uiSlices, uint uiStacks);
  static Mesh createPolygon  (float rLength, uint uiSides);
  static Mesh createSphere   (float rRadius, uint uiSlices, uint uiStacks);
  static Mesh createTeapot   (void);
  static Mesh createTorus    (float rInnerRadius, float rOuterRadius, uint uiSides, uint uiRings);
  static Mesh loadX          (const char* pszVirtPath);
};


#include "Mesh.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_MESH_H__
