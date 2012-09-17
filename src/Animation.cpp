//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-06 02:31:28
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
Texture& Animation::getFrame (float rPos)
{
  int iIndex;

  GLB_ASSERT(!m_Textures.empty());

  iIndex = int(rPos * m_Textures.size());
  iIndex %= (int)m_Textures.size();

  return this->getTexture((size_t)iIndex);
}

//---------------------------------------------------------------------------
Texture& Animation::getFramePingPong (float rPos)
{
  int iIndex;

  GLB_ASSERT(!m_Textures.empty());

  iIndex = int(rPos * m_Textures.size());
  iIndex %= (int)m_Textures.size() * 2 - 1;
  if (iIndex >= int(m_Textures.size()))
    iIndex = m_Textures.size() - 1 - (iIndex - m_Textures.size());

  return this->getTexture((size_t)iIndex);
}




//---------------------------------------------------------------------------
Animation Animation::load (const char* pszVirtDir)
{
  Animation  anim;
  StringA    strVirtPathMask;
  StringAVec vecVirtPaths;

  GLB_ASSERT(g_pVFS());
  GLB_ASSERT(g_pRenderer());

  strVirtPathMask  = pszVirtDir;
  strVirtPathMask /= "*";
  vecVirtPaths = g_VFS().findFiles(strVirtPathMask);
  if (vecVirtPaths.empty())
    GLB_THROW(EXCODE_FILE_DOESNTEXIST, "Animation files not found in VFS with mask \"%s\" !", strVirtPathMask.c_str());
  std::sort(vecVirtPaths.begin(), vecVirtPaths.end());

  for (size_t i = 0; i < vecVirtPaths.size(); ++i)
  {
    Texture tex = g_Renderer().loadTexture(vecVirtPaths[i]);
    anim.addTexture(tex);
  }

  return anim;
}


} // namespace glb
#endif // #ifdef GLOBE_GFX
