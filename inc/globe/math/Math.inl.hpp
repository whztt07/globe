//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-11-06 11:56:22
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Math::abs (T val)
{
  return ::abs(val); // (val < 0) ? -val : val;
}

//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Math::ceil (T val)
{
  return ::ceil(val);
}

//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Math::exp (T val)
{
  return ::exp(val);
}

//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Math::floor (T val)
{
  return ::floor(val);
}

//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Math::frac (T val)
{
  return val - ::floor(val);
}

//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Math::log (T val)
{
  return ::log(val);
}

//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Math::log10 (T val)
{
  return ::log10(val);
}

//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Math::mod (T a, T b)
{
  return ::fmod(a, b);
}

//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Math::pow (T val, T exponent)
{
  return ::pow(val, exponent);
}

//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Math::round (T val)
{
  return ::floor(val + T(0.5));
}

//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Math::sq (T val)
{
  return val * val;
}

//---------------------------------------------------------------------------
template <typename T> GLB_FORCEINLINE T Math::sqrt (T val)
{
  return ::sqrt(val);
}




//---------------------------------------------------------------------------
template <typename T> inline T Math::acos (T val)
{
  return ::acos(val);
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::asin (T val)
{
  return ::asin(val);
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::atan (T val)
{
  return ::atan(val);
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::atan2 (T a, T b)
{
  return ::atan2(a, b);
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::cos (T val)
{
  return ::cos(val);
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::sin (T val)
{
  return ::sin(val);
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::tan (T val)
{
  return ::tan(val);
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::tanh (T val)
{
  return ::tanh(val);
}




//---------------------------------------------------------------------------
template <typename T> inline T Math::degrees (T radians)
{
  return radians * T(GLOBE_MATH_RAD2DEG);
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::radians (T degrees)
{
  return degrees * T(GLOBE_MATH_DEG2RAD);
}




//---------------------------------------------------------------------------
template <typename T> inline T Math::min (T a, T b)
{
  return (a < b) ? a : b;
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::min (T a, T b, T c)
{
  return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::max (T a, T b)
{
  return (a > b) ? a : b;
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::max (T a, T b, T c)
{
  return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}




//---------------------------------------------------------------------------
template <typename T> inline bool Math::equal (T a, T b, T tolerance)
{
  return (b < (a + tolerance)) && (b > (a - tolerance));
}




//---------------------------------------------------------------------------
template <typename T> inline T Math::bezierPoint (T a, T b, T c, T d, T t)
{
  // Evaluates the Bezier at point t for points a, b, c, d. The parameter t
  // varies between 0 and 1, a and d are points on the curve, and b and c are
  // the control points. This can be done once with the x coordinates and a
  // second time with the y coordinates to get the location of a bezier curve
  // at t.
  //
  // Parameters :
  // * a coordinate of first point on the curve
  // * b coordinate of first control point
  // * c coordinate of second control point
  // * d coordinate of second point on the curve
  // * t value between 0 and 1

  T t1 = T(1.0) - t;
  return
    a * t1 * t1 * t1 +
    3 * b * t * t1 * t1 +
    3 * c * t * t * t1 +
    d * t * t * t;
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::clamp (T val, T low, T high)
{
  // Constrains a value to not exceed a maximum and minimum value.
  //
  // Parameters :
  // * val  the value to constrain
  // * low  minimum limit
  // * high maximum limit

  return (val < low) ? low : ((val > high) ? high : val);
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::lerp (T val, T low, T high)
{
  // Calculates a number between two numbers at a specific increment. The
  // val parameter is the amount to interpolate between the two values
  // where 0.0 equal to the first point, 0.1 is very near the first point,
  // 0.5 is half-way in between, etc. The lerp function is convenient for
  // creating motion along a straight path and for drawing dotted lines.
  //
  // Parameters :
  // * val  float between 0.0 and 1.0
  // * low  first value
  // * high second value

  //GLB_ASSERT(val >= T(0.0) && val <= T(1.0));
  return low + (high - low) * val;
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::smoothStep (T val, T low, T high)
{
  // Smoother than lerp()

  val = (val - low) / (high - low);
  T t = Math::clamp<T>(val, T(0), T(1));
  return t * t * (T(3) - T(2) * t);
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::map (T val, T low1, T high1, T low2, T high2)
{
  // Re-maps a number from one range to another.
  // Numbers outside the range are not clamped to 0 and 1, because
  // out-of-range values are often intentional and useful.
  //
  // Parameters :
  // * val   the incoming value to be converted
  // * low1  lower bound of the value's current range
  // * high1 upper bound of the value's current range
  // * low2  lower bound of the value's target range
  // * high2 upper bound of the value's target range

  // T rSpan1  = high1 - low1;
  // T rSpan2  = high2 - low2;
  // T rScaled = (val - low1) / rSpan1;
  //
  // return low2 + (rScaled * rSpan2);

  return low2 + (((val - low1) / (high1 - low1)) * (high2 - low2));
}

//---------------------------------------------------------------------------
template <typename T> inline T Math::norm (T val, T low, T high)
{
  return (val - low) / (high - low);
}
