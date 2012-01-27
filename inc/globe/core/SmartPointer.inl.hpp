//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-11-28 21:24:14
//
// $Id$
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
