//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-05 01:34:11
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_INDEXBUFFER_H__
#define __GLOBE_INDEXBUFFER_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// IndexBuffer
//---------------------------------------------------------------------------
class GLB_EXPORT IndexBuffer : public SmartPointer<IDirect3DIndexBuffer9>
{
public :
  IndexBuffer (IDirect3DIndexBuffer9* pBuffer=0);

  void* lock   (uint uiOffset, uint uiSize, DWORD dwFlags);
  void  unlock (void);
};


#include "IndexBuffer.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_INDEXBUFFER_H__
