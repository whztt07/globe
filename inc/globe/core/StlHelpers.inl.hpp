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
// Created On: 2011-10-08 00:43:10
//
//***************************************************************************


//---------------------------------------------------------------------------
// VectorForeignIterator
//---------------------------------------------------------------------------
template <class T>
inline VectorForeignIterator<T>::VectorForeignIterator (typename T::iterator itBegin, typename T::iterator itEnd)
: m_itCurrent(itBegin)
, m_itEnd(itEnd)
{
}

template <class T>
inline VectorForeignIterator<T>::VectorForeignIterator (T& vec)
: m_itCurrent(vec.begin())
, m_itEnd(vec.end())
{
}

template <class T>
inline bool VectorForeignIterator<T>::hasNext (void) const
{
  return (m_itCurrent != m_itEnd);
}

template <class T>
inline typename T::value_type VectorForeignIterator<T>::getNext (void)
{
  return *m_itCurrent++;
}

template <class T>
inline typename T::value_type VectorForeignIterator<T>::peekNext (void)
{
  return *m_itCurrent;
}

template <class T>
inline typename T::pointer VectorForeignIterator<T>::peekNextPointer (void)
{
  return &(*m_itCurrent);
}

template <class T>
inline void VectorForeignIterator<T>::seekNext (void)
{
  return ++m_itCurrent;
}

template <class T>
inline VectorForeignIterator<T>& VectorForeignIterator<T>::operator= (VectorForeignIterator<T>& rhs)
{
  m_itCurrent = rhs.m_itCurrent;
  m_itEnd     = rhs.m_itEnd;

  return *this;
}


//---------------------------------------------------------------------------
// VectorForeignConstIterator
//---------------------------------------------------------------------------
template <class T>
inline VectorForeignConstIterator<T>::VectorForeignConstIterator (typename T::const_iterator itBegin, typename T::const_iterator itEnd)
: m_itCurrent(itBegin)
, m_itEnd(itEnd)
{
}

template <class T>
inline VectorForeignConstIterator<T>::VectorForeignConstIterator (const T& vec)
: m_itCurrent(vec.begin())
, m_itEnd(vec.end())
{
}

template <class T>
inline bool VectorForeignConstIterator<T>::hasNext (void) const
{
  return (m_itCurrent != m_itEnd);
}

template <class T>
inline typename T::value_type VectorForeignConstIterator<T>::getNext (void)
{
  return *m_itCurrent++;
}

template <class T>
inline typename T::value_type VectorForeignConstIterator<T>::peekNext (void) const
{
  return *m_itCurrent;
}

template <class T>
inline typename T::const_pointer VectorForeignConstIterator<T>::peekNextPointer (void) const
{
  return &(*m_itCurrent);
}

template <class T>
inline void VectorForeignConstIterator<T>::seekNext (void) const
{
  return ++m_itCurrent;
}

template <class T>
inline VectorForeignConstIterator<T>& VectorForeignConstIterator<T>::operator= (VectorForeignConstIterator<T>& rhs)
{
  m_itCurrent = rhs.m_itCurrent;
  m_itEnd     = rhs.m_itEnd;

  return *this;
}


//---------------------------------------------------------------------------
// MapForeignIterator
//---------------------------------------------------------------------------
template <class T>
inline MapForeignIterator<T>::MapForeignIterator (typename T::iterator itBegin, typename T::iterator itEnd)
: m_itCurrent(itBegin)
, m_itEnd(itEnd)
{
}

template <class T>
inline MapForeignIterator<T>::MapForeignIterator (T& m)
: m_itCurrent(m.begin())
, m_itEnd(m.end())
{
}

template <class T>
inline bool MapForeignIterator<T>::hasNext (void) const
{
  return (m_itCurrent != m_itEnd);
}

template <class T>
inline typename T::key_type MapForeignIterator<T>::getNextKey (void)
{
  return (m_itCurrent++)->first;
}

template <class T>
inline typename T::mapped_type MapForeignIterator<T>::getNextValue (void)
{
  return (m_itCurrent++)->second;
}

template <class T>
inline typename T::key_type MapForeignIterator<T>::peekNextKey (void)
{
  return m_itCurrent->first;
}

template <class T>
inline typename T::mapped_type MapForeignIterator<T>::peekNextValue (void)
{
  return m_itCurrent->second;
}

template <class T>
inline typename T::mapped_type* MapForeignIterator<T>::peekNextValuePointer (void)
{
  return &(m_itCurrent->second);
}

template <class T>
inline void MapForeignIterator<T>::seekNext (void)
{
  ++m_itCurrent;
}

template <class T>
inline MapForeignIterator<T>& MapForeignIterator<T>::operator= (MapForeignIterator<T>& rhs)
{
  m_itCurrent = rhs.m_itCurrent;
  m_itEnd     = rhs.m_itEnd;

  return *this;
}


//---------------------------------------------------------------------------
// MapForeignConstIterator
//---------------------------------------------------------------------------
template <class T>
inline MapForeignConstIterator<T>::MapForeignConstIterator (typename T::const_iterator itBegin, typename T::const_iterator itEnd)
: m_itCurrent(itBegin)
, m_itEnd(itEnd)
{
}

template <class T>
inline MapForeignConstIterator<T>::MapForeignConstIterator (const T& m)
: m_itCurrent(m.begin())
, m_itEnd(m.end())
{
}

template <class T>
inline bool MapForeignConstIterator<T>::hasNext (void) const
{
  return (m_itCurrent != m_itEnd);
}

template <class T>
inline typename T::key_type MapForeignConstIterator<T>::getNextKey (void)
{
  return (m_itCurrent++)->first;
}

template <class T>
inline typename T::mapped_type MapForeignConstIterator<T>::getNextValue (void)
{
  return (m_itCurrent++)->second;
}

template <class T>
inline typename T::key_type MapForeignConstIterator<T>::peekNextKey (void) const
{
  return m_itCurrent->first;
}

template <class T>
inline typename T::mapped_type MapForeignConstIterator<T>::peekNextValue (void) const
{
  return m_itCurrent->second;
}

template <class T>
inline const typename T::mapped_type* MapForeignConstIterator<T>::peekNextValuePointer (void) const
{
  return &(m_itCurrent->second);
}

template <class T>
inline void MapForeignConstIterator<T>::seekNext (void) const
{
  ++m_itCurrent;
}

template <class T>
inline MapForeignConstIterator<T>& MapForeignConstIterator<T>::operator= (MapForeignConstIterator<T>& rhs)
{
  m_itCurrent = rhs.m_itCurrent;
  m_itEnd     = rhs.m_itEnd;

  return *this;
}
