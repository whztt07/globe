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
// Created On: 2011-11-28 21:24:14
//
//***************************************************************************


//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>::SmartPointer (void)
: m_pSmartObject(0)
{
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>::SmartPointer (const SmartPointer<T>& smartPtr)
{
  T* pObj = const_cast<T*>(smartPtr.getRef());

  if (pObj)
    pObj->AddRef();

  m_pSmartObject = pObj;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>::SmartPointer (T* pObject)
{
  if (pObject)
    pObject->AddRef();
  m_pSmartObject = pObject;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>::~SmartPointer (void)
{
  if (m_pSmartObject)
    m_pSmartObject->Release();
}

//---------------------------------------------------------------------------
template <class T>
inline void SmartPointer<T>::attachRef (T* pObject)
{
  if (m_pSmartObject)
    m_pSmartObject->Release();
  m_pSmartObject = pObject;
}

//---------------------------------------------------------------------------
template <class T>
inline void SmartPointer<T>::release (void)
{
  if (m_pSmartObject)
  {
    m_pSmartObject->Release();
    m_pSmartObject = 0;
  }
}

//---------------------------------------------------------------------------
template <class T>
inline T* SmartPointer<T>::getRef (void)
{
  return m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline const T* SmartPointer<T>::getRef (void) const
{
  return m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>::operator T* (void)
{
  return m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>::operator const T* (void) const
{
  return m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline T& SmartPointer<T>::operator* (void)
{
  return *m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline const T& SmartPointer<T>::operator* (void) const
{
  return *m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline T* SmartPointer<T>::operator-> (void)
{
  return m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline const T* SmartPointer<T>::operator-> (void) const
{
  return m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline T** SmartPointer<T>::operator& (void)
{
  return &m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline const T** SmartPointer<T>::operator& (void) const
{
  return (const T**)&m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>& SmartPointer<T>::operator= (T* pObject)
{
  if (m_pSmartObject != pObject)
  {
    if (m_pSmartObject)
      m_pSmartObject->Release();

    m_pSmartObject = pObject;

    if (m_pSmartObject)
      m_pSmartObject->AddRef();
  }

  return *this;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>& SmartPointer<T>::operator= (const SmartPointer<T>& smartPtr)
{
  T* pObject = const_cast<T*>(smartPtr.getRef());

  if (m_pSmartObject != pObject)
  {
    if (m_pSmartObject)
      m_pSmartObject->Release();

    m_pSmartObject = pObject;

    if (m_pSmartObject)
      m_pSmartObject->AddRef();
  }

  return *this;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>::operator const bool (void) const
{
  return m_pSmartObject != 0;
}

//---------------------------------------------------------------------------
template <class T>
inline bool SmartPointer<T>::operator! (void) const
{
  return m_pSmartObject == 0;
}

//---------------------------------------------------------------------------
template <class T>
inline bool SmartPointer<T>::operator== (const T* pObject) const
{
  return m_pSmartObject == pObject;
}

//---------------------------------------------------------------------------
template <class T>
inline bool SmartPointer<T>::operator== (const SmartPointer<T>& smartPtr) const
{
  return m_pSmartObject == smartPtr.getRef();
}

//---------------------------------------------------------------------------
template <class T>
inline bool SmartPointer<T>::operator!= (const T* pObject) const
{
  return m_pSmartObject != pObject;
}

//---------------------------------------------------------------------------
template <class T>
inline bool SmartPointer<T>::operator!= (const SmartPointer<T>& smartPtr) const
{
  return m_pSmartObject != smartPtr.getRef();
}

//---------------------------------------------------------------------------
template <class T>
inline bool SmartPointer<T>::operator< (const T* pObject) const
{
  return m_pSmartObject < pObject;
}

//---------------------------------------------------------------------------
template <class T>
inline bool SmartPointer<T>::operator< (const SmartPointer<T>& smartPtr) const
{
  return m_pSmartObject < smartPtr.getRef();
}




//---------------------------------------------------------------------------
inline SmartPointerObject::SmartPointerObject (uint uiInitialRefCount/*=1*/)
: m_uiRefCounter(uiInitialRefCount)
{
}

//---------------------------------------------------------------------------
inline SmartPointerObject::~SmartPointerObject (void)
{
  GLB_ASSERT(m_uiRefCounter == 0);
}

//---------------------------------------------------------------------------
inline void SmartPointerObject::AddRef (void)
{
  GLB_ASSERT(m_uiRefCounter < std::numeric_limits<uint>::max());
  ++m_uiRefCounter;
}

//---------------------------------------------------------------------------
inline void SmartPointerObject::Release (void)
{
  GLB_ASSERT(m_uiRefCounter > 0);
  if (m_uiRefCounter > 0)
  {
    if (--m_uiRefCounter == 0)
      delete this;
  }
}

//---------------------------------------------------------------------------
inline uint SmartPointerObject::RefCount (void) const
{
  GLB_ASSERT(m_uiRefCounter > 0);
  return m_uiRefCounter;
}
