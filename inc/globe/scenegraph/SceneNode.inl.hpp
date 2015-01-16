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
// Created On: 2011-12-08 03:10:47
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
