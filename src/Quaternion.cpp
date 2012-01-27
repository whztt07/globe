//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-11-30 03:12:27
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
const Quaternion Quaternion::ZERO     = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
const Quaternion Quaternion::IDENTITY = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);


//---------------------------------------------------------------------------
Quaternion::Quaternion (float angleX, float angleY, float angleZ)
: D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f)
{
  Quaternion rot;

  // TODO : test this ! should we reset rot first ?

  D3DXQuaternionRotationAxis(&rot, &Vector3::UNIT_X, angleX);
  *this *= rot;
  D3DXQuaternionRotationAxis(&rot, &Vector3::UNIT_Y, angleY);
  *this *= rot;
  D3DXQuaternionRotationAxis(&rot, &Vector3::UNIT_Z, angleZ);
  *this *= rot;
}


} // namespace glb
