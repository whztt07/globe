//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-08 03:10:47
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline SceneNode::NodeType SceneNode::getNodeType (void) const
{
  return m_eNodeType;
}

//---------------------------------------------------------------------------
inline const StringA& SceneNode::getName (void) const
{
  return m_strName;
}

//---------------------------------------------------------------------------
inline SceneNode* SceneNode::getParent (void) const
{
  return m_pParent;
}

//---------------------------------------------------------------------------
inline SceneNode::ChildrenFIt SceneNode::getChildren (void)
{
  return ChildrenFIt(m_Children.begin(), m_Children.end());
}

//---------------------------------------------------------------------------
inline SceneNode::ChildrenCFIt SceneNode::getChildren (void) const
{
  return ChildrenCFIt(m_Children.begin(), m_Children.end());
}

//---------------------------------------------------------------------------
inline bool SceneNode::hasChild (const StringA& strName) const
{
  return this->findChild(strName) != 0;
}

//---------------------------------------------------------------------------
inline Matrix4 SceneNode::getLocalTransform (void)
{
  return Matrix4::IDENTITY;
}
