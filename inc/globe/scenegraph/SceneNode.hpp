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
// Created On: 2011-12-08 03:10:45
//
//***************************************************************************

#ifndef __GLOBE_SCENENODE_H__
#define __GLOBE_SCENENODE_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// SceneNode
//---------------------------------------------------------------------------
class GLB_EXPORT SceneNode
{
public :
  typedef enum NodeType
  {
    NODE_ROOT,
    NODE_TRANSFORM,
    NODE_TARGET,
    NODE_CAMERA,
    NODE_LIGHT,
    NODE_DRAWABLE,
  };

  typedef std::list<SceneNode*>                Children;
  typedef Children::iterator                   ChildrenIt;
  typedef Children::const_iterator             ChildrenCIt;
  typedef VectorForeignIterator<Children>      ChildrenFIt;
  typedef VectorForeignConstIterator<Children> ChildrenCFIt;


public :
  SceneNode (NodeType eNodeType, const char* pszName=NULL);
  virtual ~SceneNode (void);

  void setName (const char* pszName);

  NodeType       getNodeType (void) const;
  const StringA& getName     (void) const;
  SceneNode*     getParent   (void) const;
  ChildrenFIt    getChildren (void);
  ChildrenCFIt   getChildren (void) const;
  bool           hasChild    (const StringA& strName) const;
  SceneNode*     findChild   (const StringA& strName) const;

  void attachChild (SceneNode* pChild);

  virtual Matrix4 getLocalTransform    (void);
  virtual Matrix4 getAbsoluteTransform (void);


private :
  SceneNode (void) { GLB_ASSERT(0); }


protected :
  NodeType   m_eNodeType;
  StringA    m_strName;
  SceneNode* m_pParent;
  Children   m_Children;
};


#include "SceneNode.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_SCENENODE_H__
