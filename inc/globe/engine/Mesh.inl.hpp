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
// Created On : 2011-12-08 01:53:41
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline Drawable* Mesh::drawable (void)
{
  return static_cast<Drawable*>(this);
}

//---------------------------------------------------------------------------
inline void Mesh::draw (void)
{
  GLB_ASSERT(this->getRef());
  this->getRef()->DrawSubset(0);
}

//---------------------------------------------------------------------------
inline void Mesh::draw (uint uiSubset)
{
  GLB_ASSERT(this->getRef());
  this->getRef()->DrawSubset(uiSubset);
}
