//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-11-29 14:44:05
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline Vector3::Vector3 (void)
{
}

//---------------------------------------------------------------------------
inline Vector3::Vector3 (const D3DXVECTOR3& rhs)
: D3DXVECTOR3(rhs)
{
}

//---------------------------------------------------------------------------
inline Vector3::Vector3 (double x, double y, double z)
: D3DXVECTOR3(float(x), float(y), float(z))
{
}




//---------------------------------------------------------------------------
inline float Vector3::length (void) const
{
  return Math::sqrt<float>(
    x * x +
    y * y +
    z * z);
}

//---------------------------------------------------------------------------
inline float Vector3::squaredLength (void) const
{
  return
    x * x +
    y * y +
    z * z;
}

//---------------------------------------------------------------------------
inline float Vector3::dot (const Vector3& rhs) const
{
  return
    x * rhs.x +
    y * rhs.y +
    z * rhs.z;
}

//---------------------------------------------------------------------------
inline float Vector3::normalize (void)
{
  float len = this->length();

  if (len > GLOBE_MATH_ZEROTOLERANCE)
  {
    float invlen = 1.0f / len;
    x *= invlen;
    y *= invlen;
    z *= invlen;
  }
  else
  {
    len = 0.0f;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }

  return len;
}




//---------------------------------------------------------------------------
inline Vector3 Vector3::cross (const Vector3& rhs) const
{
  return Vector3(
    y * rhs.z - z * rhs.y,
    z * rhs.x - x * rhs.z,
    x * rhs.y - y * rhs.x);
}

//---------------------------------------------------------------------------
inline Vector3 Vector3::unitCross (const Vector3& rhs) const
{
  Vector3 vec(
    y * rhs.z - z * rhs.y,
    z * rhs.x - x * rhs.z,
    x * rhs.y - y * rhs.x);
  vec.normalize();
  return vec;
}
