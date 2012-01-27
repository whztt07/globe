//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-08 00:42:54
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_STLHELPERS_H__
#define __GLOBE_STLHELPERS_H__
namespace glb {


//---------------------------------------------------------------------------
// VectorForeignIterator
//
// * A "foreign" iterator allows you to iterate through a given STL container
//   without the need to have any access to this container.
// * Note that by using this "foreign iterator", you are still dependent of
//   any changes made to the underlying container while iterating on it
//   (inserts, removes, etc...), since there are big chances that any
//   modification will unvalidate you iterator.
// * This also means instances of foreign iterator *must not* be stored, thus
//   they must be used at a given time only.
//---------------------------------------------------------------------------
template <class T>
class VectorForeignIterator
{
public :
  VectorForeignIterator (typename T::iterator itBegin, typename T::iterator itEnd);
  explicit VectorForeignIterator (T& vec);

  bool hasNext (void) const;

  typename T::value_type getNext         (void);
  typename T::value_type peekNext        (void);
  typename T::pointer    peekNextPointer (void);
  void                   seekNext        (void);

  VectorForeignIterator<T>& operator= (VectorForeignIterator<T>& rhs);

private :
  VectorForeignIterator (void) { GLB_ASSERT(0); }

private  :
  typename T::iterator m_itCurrent;
  typename T::iterator m_itEnd;
};


//---------------------------------------------------------------------------
// VectorForeignConstIterator
//---------------------------------------------------------------------------
template <class T>
class VectorForeignConstIterator
{
public :
  VectorForeignConstIterator (typename T::const_iterator itBegin, typename T::const_iterator itEnd);
  explicit VectorForeignConstIterator (const T& vec);

  bool hasNext (void) const;

  typename T::value_type    getNext         (void);
  typename T::value_type    peekNext        (void) const;
  typename T::const_pointer peekNextPointer (void) const;
  void                      seekNext        (void) const;

  VectorForeignConstIterator<T>& operator= (VectorForeignConstIterator<T>& rhs);

private :
  VectorForeignConstIterator (void) { GLB_ASSERT(0); }

private  :
  typename T::const_iterator m_itCurrent;
  typename T::const_iterator m_itEnd;
};


//---------------------------------------------------------------------------
// MapForeignIterator
//---------------------------------------------------------------------------
template <class T>
class MapForeignIterator
{
public :
  MapForeignIterator (typename T::iterator itBegin, typename T::iterator itEnd);
  explicit MapForeignIterator (T& m);

  bool hasNext (void) const;

  typename T::key_type    getNextKey   (void);
  typename T::mapped_type getNextValue (void);

  typename T::key_type     peekNextKey          (void);
  typename T::mapped_type  peekNextValue        (void);
  typename T::mapped_type* peekNextValuePointer (void);

  void seekNext (void);

  MapForeignIterator<T>& operator= (MapForeignIterator<T>& rhs);

private :
  MapForeignIterator (void) { GLB_ASSERT(0); }

private  :
  typename T::iterator m_itCurrent;
  typename T::iterator m_itEnd;
};


//---------------------------------------------------------------------------
// MapForeignConstIterator
//---------------------------------------------------------------------------
template <class T>
class MapForeignConstIterator
{
public :
  MapForeignConstIterator (typename T::const_iterator itBegin, typename T::const_iterator itEnd);
  explicit MapForeignConstIterator (const T& m);

  bool hasNext (void) const;

  typename T::key_type    getNextKey   (void);
  typename T::mapped_type getNextValue (void);

  typename T::key_type           peekNextKey          (void) const;
  typename T::mapped_type        peekNextValue        (void) const;
  const typename T::mapped_type* peekNextValuePointer (void) const;

  void seekNext (void) const;

  MapForeignConstIterator<T>& operator= (MapForeignConstIterator<T>& rhs);

private :
  MapForeignConstIterator (void) { GLB_ASSERT(0); }

private  :
  typename T::const_iterator m_itCurrent;
  typename T::const_iterator m_itEnd;
};


#include "StlHelpers.inl.hpp"


} // namespec glb
#endif // #ifndef __GLOBE_STLHELPERS_H__
