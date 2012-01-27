//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-06 02:31:23
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_ANIMATION_H__
#define __GLOBE_ANIMATION_H__
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
#endif // #ifndef __GLOBE_ANIMATION_H__
