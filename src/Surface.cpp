//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-04 20:02:14
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
D3DSURFACE_DESC Surface::getDesc (void)
{
  D3DSURFACE_DESC d3dSurfDesc;

  GLB_ASSERT(this->getRef());
  GLB_DXTEST(this->getRef()->GetDesc(&d3dSurfDesc));

  return d3dSurfDesc;
}


} // namespace glb
#endif // #ifdef GLOBE_GFX
