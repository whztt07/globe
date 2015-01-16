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
// Created On: 2011-11-28 21:24:12
//
//***************************************************************************

#ifndef __GLOBE_SMARTPOINTER_H__
#define __GLOBE_SMARTPOINTER_H__
namespace glb {


//---------------------------------------------------------------------------
// SmartPointer
//
// Note that the T class may be derived from the in-house SmartPointerObject
// class but is also compatible with the IUnknown class (Microsoft COM).
//---------------------------------------------------------------------------
template <class T>
class SmartPointer
{
public :
  SmartPointer (void);
  SmartPointer (const SmartPointer<T>& smartPtr);
  SmartPointer (T* pObject);
  ~SmartPointer (void);

  // explicit pointer setup (pObject will NOT be AddRef())
  void attachRef (T* pObject);
  void release   (void);

  // explicit pointer accessors
        T* getRef (void);
  const T* getRef (void) const;

  // implicit conversion
            operator T*       (void);
            operator const T* (void) const;
        T&  operator*         (void);
  const T&  operator*         (void) const;
        T*  operator->        (void);
  const T*  operator->        (void) const;
        T** operator&         (void);
  const T** operator&         (void) const;

  // assignment operators
  SmartPointer<T>& operator= (T* pObject);
  SmartPointer<T>& operator= (const SmartPointer<T>& smartptr);

  // comparison operators
  operator const bool (void) const;
  bool operator!  (void) const;
  bool operator== (const T* pObject) const;
  bool operator!= (const T* pObject) const;
  bool operator<  (const T* pObject) const;
  bool operator== (const SmartPointer<T>& smartPtr) const;
  bool operator!= (const SmartPointer<T>& smartPtr) const;
  bool operator<  (const SmartPointer<T>& smartPtr) const;

private :
  T* m_pSmartObject;
};


//---------------------------------------------------------------------------
// SmartPointerObject
//
// Note that the AddRef() and Release() methods are named according to the
// Microsoft's COM convention so the SmartPointer<> class may be used with
// classes derived from the COM's IUnknown class as well.
//---------------------------------------------------------------------------
class SmartPointerObject
{
protected :
  explicit SmartPointerObject (uint uiInitialRefCount=1);
  virtual ~SmartPointerObject (void);

public :
  void AddRef  (void);
  void Release (void);

  uint RefCount (void) const;

private :
  uint m_uiRefCounter;
};


//---------------------------------------------------------------------------
// Inline methods
//---------------------------------------------------------------------------
#include "SmartPointer.inl.hpp"


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
#define GlobeDeclareSmartPointerType(myclass) \
  typedef ::glb::SmartPointer<myclass> myclass##Ptr


} // namespace glb
#endif // #ifndef __GLOBE_SMARTPOINTER_H__
