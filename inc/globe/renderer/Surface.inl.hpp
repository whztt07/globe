//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-04 20:02:11
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline Surface::Surface (IDirect3DSurface9* pSurface/*=0*/)
: SmartPointer<IDirect3DSurface9>(pSurface)
{
}

//---------------------------------------------------------------------------
inline uint Surface::getWidth (void)
{
  GLB_ASSERT(this->getRef());
  return this->getDesc().Width;
}

//---------------------------------------------------------------------------
inline uint Surface::getHeight (void)
{
  GLB_ASSERT(this->getRef());
  return this->getDesc().Height;
}

//---------------------------------------------------------------------------
inline D3DFORMAT Surface::getFormat (void)
{
  GLB_ASSERT(this->getRef());
  return this->getDesc().Format;
}
