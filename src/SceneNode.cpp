//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-08 03:10:50
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
SceneNode::SceneNode (SceneNode::NodeType eNodeType, const char* pszName/*=NULL*/)
: m_eNodeType(eNodeType)
, m_strName(pszName)
, m_pParent(0)
{
}

//---------------------------------------------------------------------------
SceneNode::~SceneNode (void)
{
}

//---------------------------------------------------------------------------
void SceneNode::setName (const char* pszName)
{
  m_strName = pszName;
}

//---------------------------------------------------------------------------
void SceneNode::attachChild (SceneNode* pChild)
{
  GLB_ASSERT(pChild->m_pParent == 0);

  m_Children.push_back(pChild);
  pChild->m_pParent = this;
}

//---------------------------------------------------------------------------
SceneNode* SceneNode::findChild (const StringA& strName) const
{
  ChildrenCIt itC;

  for (itC = m_Children.begin(); itC != m_Children.end(); ++itC)
  {
    if ((*itC)->m_strName == strName)
      return *itC;
  }

  for (itC = m_Children.begin(); itC != m_Children.end(); ++itC)
  {
    const SceneNode* pChild = (*itC)->findChild(strName);
    if (pChild)
      return const_cast<SceneNode*>(pChild);
  }

  return 0;
}

//---------------------------------------------------------------------------
Matrix4 SceneNode::getAbsoluteTransform (void)
{
  Matrix4 mtx = this->getLocalTransform();

#ifdef GLB_DEBUG
  SceneNode* pNode = this;
  while (pNode)
  {
    SceneNode* pParent = pNode->getParent();
    if (!pParent)
      GLB_FORCEASSERT(pNode->getNodeType() == NODE_ROOT);
    else
      mtx *= pParent->getLocalTransform();
    pNode = pParent;
  }
#else
  for (SceneNode* pNode = this->getParent(); pNode; pNode = pNode->getParent())
    mtx *= pNode->getLocalTransform();
#endif

  return mtx;
}


} // namespace glb
#endif // #ifdef GLOBE_GFX
