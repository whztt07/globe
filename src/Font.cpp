//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-10 23:55:58
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
D3DXFONT_DESC Font::getDesc (void)
{
  D3DXFONT_DESC desc;

  GLB_ASSERT(this->getRef());
  GLB_DXTEST(this->getRef()->GetDesc(&desc));

  return desc;
}

//---------------------------------------------------------------------------
TEXTMETRIC Font::getMetrics (void)
{
  TEXTMETRIC tm;

  GLB_ASSERT(this->getRef());
  GLB_DXTEST(this->getRef()->GetTextMetrics(&tm));

  return tm;
}


} // namespace glb
