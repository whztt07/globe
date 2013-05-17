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
// Created On : 2011-11-30 03:12:23
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline Quaternion::Quaternion (void)
{
}

//---------------------------------------------------------------------------
inline Quaternion::Quaternion (float x, float y, float z, float w)
: D3DXQUATERNION(x, y, z, w)
{
}




//---------------------------------------------------------------------------
inline Quaternion Quaternion::slerp (const Quaternion& q2, float t) const
{
  Quaternion res;
  D3DXQuaternionSlerp(&res, this, &q2, t);
  return res;
}
