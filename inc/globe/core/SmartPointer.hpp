//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-11-28 21:24:12
//
// $Id$
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
