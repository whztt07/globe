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
// Created On: 2011-12-06 02:31:26
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
