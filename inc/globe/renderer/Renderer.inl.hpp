//***************************************************************************
//
// Globe
// A small C++ library to allow fast prototyping of Direct3D animations.
//
// Copyright (C) 2012-2013 Jean-Charles Lefebvre <polyvertex@gmail.com>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions :
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
// Created On: 2011-11-28 22:23:54
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
