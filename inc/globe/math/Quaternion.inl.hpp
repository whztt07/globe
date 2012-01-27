//***************************************************************************
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
