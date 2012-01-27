//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-11-28 22:23:54
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline IDirect3D9* Renderer::getD3D (void)
{
  return m_pD3D;
}

//---------------------------------------------------------------------------
inline bool Renderer::isInit (void) const
{
  GLB_ASSERT((m_pD3DDev == 0) == (m_hWnd == 0));
  return m_pD3DDev != 0;
}

//---------------------------------------------------------------------------
inline IDirect3DDevice9* Renderer::getDev (void)
{
  GLB_ASSERT(this->isInit());
  return m_pD3DDev;
}

//---------------------------------------------------------------------------
inline HWND Renderer::getHWnd (void)
{
  GLB_ASSERT(this->isInit());
  return m_hWnd;
}




//---------------------------------------------------------------------------
inline void Renderer::setDepthStencilSurface (Surface& surf)
{
  GLB_ASSERT(this->isInit());
  GLB_ASSERT(surf.getRef());
  GLB_DXTEST(m_pD3DDev->SetDepthStencilSurface(surf.getRef()));
}

//---------------------------------------------------------------------------
inline void Renderer::setDepthStencilSurface (IDirect3DSurface9* pSurface)
{
  GLB_ASSERT(this->isInit());
  GLB_DXTEST(m_pD3DDev->SetDepthStencilSurface(pSurface));
}

//---------------------------------------------------------------------------
inline void Renderer::setRenderTarget (Surface& surf, uint uiIndex/*=0*/)
{
  GLB_ASSERT(this->isInit());
  GLB_ASSERT(surf.getRef());
  GLB_DXTEST(m_pD3DDev->SetRenderTarget(uiIndex, surf.getRef()));
}

//---------------------------------------------------------------------------
inline void Renderer::setRenderTarget (IDirect3DSurface9* pSurface, uint uiIndex/*=0*/)
{
  GLB_ASSERT(this->isInit());
  GLB_DXTEST(m_pD3DDev->SetRenderTarget(uiIndex, pSurface));
}
