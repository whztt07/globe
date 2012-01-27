//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-05 01:34:07
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline VertexBuffer::VertexBuffer (IDirect3DVertexBuffer9* pBuffer/*=0*/)
: SmartPointer<IDirect3DVertexBuffer9>(pBuffer)
{
}

//---------------------------------------------------------------------------
inline void VertexBuffer::unlock (void)
{
  GLB_ASSERT(this->getRef());
  this->getRef()->Unlock();
}
