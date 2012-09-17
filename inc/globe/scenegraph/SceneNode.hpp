//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-08 03:10:45
//
// $Id$
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
