//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-05 01:51:51
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
void* IndexBuffer::lock (uint uiOffset, uint uiSize, DWORD dwFlags)
{
  void* pData = 0;

  GLB_ASSERT(this->getRef());
  GLB_DXTEST(this->getRef()->Lock(uiOffset, uiSize, &pData, dwFlags));

  return pData;
}


} // namespace glb
#endif // #ifdef GLOBE_GFX
