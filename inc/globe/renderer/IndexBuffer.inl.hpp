//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-05 01:34:13
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline IndexBuffer::IndexBuffer (IDirect3DIndexBuffer9* pBuffer/*=0*/)
: SmartPointer<IDirect3DIndexBuffer9>(pBuffer)
{
}

//---------------------------------------------------------------------------
inline void IndexBuffer::unlock (void)
{
  GLB_ASSERT(this->getRef());
  this->getRef()->Unlock();
}
