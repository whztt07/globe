//***************************************************************************
//
// Globe
// A small C++ library to allow fast prototyping of Direct3D animations.
//
// Copyright (C) 2012-2013 Jean-Charles Lefebvre <jcl [AT] jcl [DOT] io>
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
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-06 02:31:23
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_ANIMATION_H__
#define __GLOBE_ANIMATION_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// Animation
//---------------------------------------------------------------------------
class GLB_EXPORT Animation
{
  typedef std::vector<Texture>     Textures;
  typedef Textures::iterator       TexturesIt;
  typedef Textures::const_iterator TexturesCIt;


public :
  void   addTexture      (Texture& tex);
  size_t getTextureCount (void) const;
  bool   isEmpty         (void) const;

  Texture& getTexture       (size_t uiIndex);
  Texture& getFrame         (float rPos);
  Texture& getFramePingPong (float rPos);


  // static utils
  static Animation load (const char* pszVirtDir);


public :
  Textures m_Textures;
};


#include "Animation.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_ANIMATION_H__
