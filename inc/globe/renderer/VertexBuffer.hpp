//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-05 01:34:05
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_VERTEXBUFFER_H__
#define __GLOBE_VERTEXBUFFER_H__
namespace glb {


//---------------------------------------------------------------------------
// VertexBuffer
//---------------------------------------------------------------------------
class GLB_EXPORT VertexBuffer : public SmartPointer<IDirect3DVertexBuffer9>
{
public :
  VertexBuffer (IDirect3DVertexBuffer9* pBuffer=0);

  void* lock   (uint uiOffset, uint uiSize, DWORD dwFlags);
  void  unlock (void);
};


#include "VertexBuffer.inl.hpp"


} // namespace glb
#endif // #ifndef __GLOBE_VERTEXBUFFER_H__
