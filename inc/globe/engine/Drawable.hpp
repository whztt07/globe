//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-07 00:06:31
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_DRAWABLE_H__
#define __GLOBE_DRAWABLE_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// Drawable
//---------------------------------------------------------------------------
class GLB_EXPORT Drawable
{
public :
  virtual void draw (void) = 0;
};


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_DRAWABLE_H__
