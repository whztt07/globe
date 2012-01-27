//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-06 02:31:26
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline void Animation::addTexture (Texture& tex)
{
  GLB_ASSERT(m_Textures.size() < (size_t)std::numeric_limits<int>::max()); // see getFrame*() methods to understand why
  m_Textures.push_back(tex);
}

//---------------------------------------------------------------------------
inline size_t Animation::getTextureCount (void) const
{
  return m_Textures.size();
}

//---------------------------------------------------------------------------
inline bool Animation::isEmpty (void) const
{
  return m_Textures.empty();
}

//---------------------------------------------------------------------------
inline Texture& Animation::getTexture (size_t uiIndex)
{
  GLB_ASSERT(uiIndex < m_Textures.size());
  return m_Textures[uiIndex];
}
