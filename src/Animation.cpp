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
// Created On: 2011-12-06 02:31:28
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
