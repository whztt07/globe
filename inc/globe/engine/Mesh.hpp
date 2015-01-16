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
// Created On: 2011-12-08 01:53:38
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
