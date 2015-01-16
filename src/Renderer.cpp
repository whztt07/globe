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
// Created On: 2011-11-28 22:25:53
//
//***************************************************************************

#include "_internal.hpp"
#ifdef GLOBE_GFX
namespace glb {

GLB_IMPLEMENT_SINGLETON(Renderer)


//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------
#define GLOBE_RENDERER_WNDCLASS  (GLB_BRAND_NAME "_RendererWnd")



//---------------------------------------------------------------------------
Renderer::Renderer (HINSTANCE hInstance)
: m_hInstance(hInstance)
, m_pD3D(0)
, m_pD3DDev(0)
, m_hWnd(0)
, m_bFullscreen(false)
{
  // check installed d3d and d3dx versions
  if (!D3DXCheckVersion(D3D_SDK_VERSION, D3DX_SDK_VERSION))
    GLB_THROW(EXCODE_SYSTEM_API, "Please download a newer version of the DirectX runtime !");

  // create the direct3d interface
  m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
  if (!m_pD3D)
    GLB_THROW(EXCODE_SYSTEM_API, "Direct3DCreate9() failed !");
}

//---------------------------------------------------------------------------
Renderer::~Renderer (void)
{
  this->uninit();
  m_pD3D->Release();;
}




//---------------------------------------------------------------------------
void Renderer::uninit (void)
{
  GLB_RELEASE(m_pD3DDev);

  if (m_hWnd)
  {
    DestroyWindow(m_hWnd);
    m_hWnd = 0;
    //UnregisterClass(GLOBE_RENDERER_WNDCLASS, m_hInstance);
  }

  if (m_bFullscreen)
  {
    ShowCursor(TRUE);
    m_bFullscreen = false;
  }
}

//---------------------------------------------------------------------------
void Renderer::init (const char* pszWndTitle, uint uiWidth, uint uiHeight, D3DFORMAT d3dFormat, bool bFullscreen, bool bVSync, uint uiAdapter/*=D3DADAPTER_DEFAULT*/, DWORD dwMinVertexShaderVersion/*=D3DVS_VERSION(0,0)*/)
{
  GLB_ASSERT(!this->isInit());

  m_bFullscreen = bFullscreen;

  // create window
  {
    WNDCLASSEX wc;
    RECT  rc      = { 0, 0, (LONG)uiWidth, (LONG)uiHeight };
    DWORD dwStyle =
      bFullscreen ?
      (WS_POPUP | WS_VISIBLE) : // | WS_CLIPCHILDREN | WS_CLIPSIBLINGS) :
      (WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX);// | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = DefWindowProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = m_hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = GLOBE_RENDERER_WNDCLASS;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    if (!RegisterClassEx(&wc))
      GLB_THROW(EXCODE_SYSTEM_API, "RegisterClassEx() failed ! Error %u : %s", System::lastError(), System::lastErrorString());

    AdjustWindowRect(&rc, dwStyle, FALSE);

    m_hWnd = CreateWindow(
      GLOBE_RENDERER_WNDCLASS,
      pszWndTitle,
      dwStyle,
      (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) >> 1,
      (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) >> 1,
      rc.right - rc.left,
      rc.bottom - rc.top,
      NULL, NULL, m_hInstance, NULL);
    if (!m_hWnd)
      GLB_THROW(EXCODE_SYSTEM_API, "CreateWindow() failed ! Error %u : %s", System::lastError(), System::lastErrorString());

    GetClientRect(m_hWnd, &rc);
    uiWidth  = uint(rc.right - rc.left);
    uiHeight = uint(rc.bottom - rc.top);
  }

  // create device
  {
    D3DCAPS9   d3dCaps;
    DWORD      dwTnL = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    D3DDEVTYPE d3dDevType = D3DDEVTYPE_HAL;
    D3DPRESENT_PARAMETERS d3dPP;

    GLB_DXTEST(
      m_pD3D->GetDeviceCaps(uiAdapter, D3DDEVTYPE_HAL, &d3dCaps),
      "Failed to get Direct3D device capabilities !");

    if ((d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
      (d3dCaps.VertexShaderVersion < dwMinVertexShaderVersion))
    {
      dwTnL = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
      GLB_LOGWARN("Insufficient GPU caps ! Downgrading from hardware to software vertex processing.");
    }

    for (uint i = 0; i < m_pD3D->GetAdapterCount(); ++i)
    {
      D3DADAPTER_IDENTIFIER9 d3dId;
      m_pD3D->GetAdapterIdentifier(i, 0, &d3dId);
      if (strstr((char*)&d3dId.Description, "PerfHUD"))
      {
        uiAdapter  = i;
        d3dDevType = D3DDEVTYPE_REF;
        GLB_LOGINFO("PerfHUD detected ! Video adapter choice overriden (PerfHUD : %u, \"%s\").", uiAdapter, (char*)&d3dId.Description);
        break;
      }
    }

    ZeroMemory(&d3dPP, sizeof(d3dPP));
    d3dPP.BackBufferWidth            = uiWidth;
    d3dPP.BackBufferHeight           = uiHeight;
    d3dPP.BackBufferFormat           = d3dFormat;
    d3dPP.BackBufferCount            = 1;
    d3dPP.MultiSampleType            = D3DMULTISAMPLE_NONE;
    d3dPP.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
    d3dPP.hDeviceWindow              = m_hWnd;
    d3dPP.Windowed                   = !bFullscreen;
  //d3dPP.EnableAutoDepthStencil     = TRUE;
  //d3dPP.AutoDepthStencilFormat     = Renderer::d3dGetDepthStencilFormat(m_pD3D, uiAdapter, d3dFormat);
  //d3dPP.Flags                      = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    d3dPP.PresentationInterval       = bVSync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dPP.FullScreen_RefreshRateInHz = !bFullscreen ? 0 : D3DPRESENT_RATE_DEFAULT;

    GLB_DXTEST(
      m_pD3D->CreateDevice(uiAdapter, d3dDevType, m_hWnd, dwTnL, &d3dPP, &m_pD3DDev),
      "Failed to create Direct3D device !");
  }

  ShowWindow(m_hWnd, TRUE);
  if (bFullscreen)
    while (ShowCursor(FALSE) > 0) { }

  // flush windows events queue
  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  GLB_DXTEST(m_pD3DDev->Clear(0, NULL, D3DCLEAR_TARGET, Color4::BLACK, 1.0f, 0));
  GLB_DXTEST(m_pD3DDev->Present(0, 0, 0, 0));
}

//---------------------------------------------------------------------------
D3DVIEWPORT9 Renderer::getViewport (void) const
{
  D3DVIEWPORT9 vp;

  GLB_ASSERT(this->isInit());
  GLB_DXTEST(m_pD3DDev->GetViewport(&vp));

  return vp;
}

//---------------------------------------------------------------------------
void Renderer::setViewport (const D3DVIEWPORT9* pViewport)
{
  GLB_ASSERT(this->isInit());
  GLB_DXTEST(m_pD3DDev->SetViewport(pViewport));
}

//---------------------------------------------------------------------------
D3DVIEWPORT9 Renderer::makeLetterboxViewport (float rDesiredAspectRatio) const
{
  GLB_ASSERT(this->isInit());

  D3DVIEWPORT9 vp;
  float rMonitorAspect;
  uint  uiBackBufferWidth;
  uint  uiBackBufferHeight;
  float rBackBufferAspect;

  // get monitor info
  {
    D3DDISPLAYMODE dm;

    GLB_DXTEST(m_pD3DDev->GetDisplayMode(0, &dm));
    rMonitorAspect = float(dm.Width) / float(dm.Height);

    //GLB_LOGDBG("Monitor resolution : %ux%u", dm.Width, dm.Height);
  }

  // get back buffer info
  {
    IDirect3DSurface9* pD3DSurf;
    D3DSURFACE_DESC    surfdesc;

    GLB_DXTEST(m_pD3DDev->GetRenderTarget(0, &pD3DSurf));
    GLB_DXTEST(pD3DSurf->GetDesc(&surfdesc));

    uiBackBufferWidth  = surfdesc.Width;
    uiBackBufferHeight = surfdesc.Height;
    rBackBufferAspect  = float(uiBackBufferWidth) / float(uiBackBufferWidth);

    //GLB_LOGDBG("Back-buffer resolution : %ux%u", surfdesc.Width, surfdesc.Height);
  }

	// setup resulting viewport
	{
    float rWidthRatio = 1.0f;
    float rHeightRatio =
      (rMonitorAspect / rDesiredAspectRatio) /
      (rDesiredAspectRatio / rBackBufferAspect);

  	if (rHeightRatio > 1.0f)
  	{
  		rWidthRatio  /= rHeightRatio;
  		rHeightRatio  = 1.0f;
  	}

  	vp.Width  = (uint)Math::round<float>(float(uiBackBufferWidth) * rWidthRatio);
  	vp.Height = (uint)Math::round<float>(float(uiBackBufferHeight) * rHeightRatio);
  	vp.X      = (uiBackBufferWidth - vp.Width) / 2;
  	vp.Y      = (uiBackBufferHeight - vp.Height) / 2;
  	vp.MinZ   = 0.0f;
  	vp.MaxZ   = 1.0f;

    //GLB_LOGDBG("Letterbox viewport : %u ; %u ; %u ; %u", vp.X, vp.Y, vp.Width, vp.Height);
  }

	return vp;
}




//---------------------------------------------------------------------------
Font Renderer::loadFont (const char* pszVirtPath, const char* pszFaceName, int iHeight, uint uiMipLevels, bool bAntialiased/*=false*/, bool bBold/*=false*/)
{
  ID3DXFont* pFont;
  glb_byte*  pFontData;
  uint       uiFontDataSize = 0;
  DWORD      dwInstalled = 0;

  GLB_ASSERT(this->isInit());
  GLB_ASSERT(g_pVFS());

  // load font into memory and install it
  pFontData = g_VFS().acquireMemFile(pszVirtPath, &uiFontDataSize);
  if (!AddFontMemResourceEx(pFontData, uiFontDataSize, NULL, &dwInstalled))
    GLB_THROW(EXCODE_SYSTEM_API, "Failed to install font from memory file \"%s\" ! Error %u : %s", pszVirtPath, System::lastError(), System::lastErrorString());

  // create the D3DX font object
  GLB_DXTEST(
    D3DXCreateFont(
      m_pD3DDev,
      iHeight, 0, FW_NORMAL,
      uiMipLevels, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
      (bAntialiased ? ANTIALIASED_QUALITY : NONANTIALIASED_QUALITY),
      DEFAULT_PITCH, pszFaceName, &pFont),
    "Failed to create D3DXFont with facename \"%s\" !", pszFaceName);

  // release memory file
  g_VFS().releaseMemFile(pFontData);

  Font font;
  font.attachRef(pFont);
  return font;
}

//---------------------------------------------------------------------------
void Renderer::printHud (Font& font, bool bTop, const char* pszFormat, ...)
{
  static StringA         str;
  static RECT            rc;
  static DWORD           dwFormat = DT_LEFT | DT_NOCLIP | (bTop ? DT_TOP : DT_BOTTOM);
  static D3DSURFACE_DESC d3dSurfDesc;

  va_list arglist;
  IDirect3DSurface9* pSurface;

  GLB_ASSERT(this->isInit());
  GLB_ASSERT(font.getRef());

  if (!pszFormat[0])
    return;
  va_start(arglist, pszFormat);
  str.formatV(pszFormat, arglist);
  va_end(arglist);

  // TODO : optimize this !
  GLB_DXTEST(m_pD3DDev->GetRenderTarget(0, &pSurface));
  GLB_DXTEST(pSurface->GetDesc(&d3dSurfDesc));
  rc.left   = 1;
  rc.top    = 1;
  rc.right  = d3dSurfDesc.Width + 1;
  rc.bottom = d3dSurfDesc.Height + 1;

  font->DrawText(NULL, str.c_str(), str.length(), &rc, dwFormat, D3DCOLOR_XRGB(0,0,0));
  --rc.left;
  --rc.top;
  --rc.right;
  --rc.bottom;
  font->DrawText(NULL, str.c_str(), str.length(), &rc, dwFormat, D3DCOLOR_XRGB(255,255,255));
}

//---------------------------------------------------------------------------
void Renderer::printHud (Font& font, int iX, int iY, D3DCOLOR d3dColor, DWORD dwFormat, const char* pszFormat, ...)
{
  static StringA         str;
  static RECT            rc;
  static D3DSURFACE_DESC d3dSurfDesc;

  va_list arglist;
  IDirect3DSurface9* pSurface;

  GLB_ASSERT(this->isInit());
  GLB_ASSERT(font.getRef());

  if (!pszFormat[0])
    return;
  va_start(arglist, pszFormat);
  str.formatV(pszFormat, arglist);
  va_end(arglist);

  // TODO : optimize this !
  GLB_DXTEST(m_pD3DDev->GetRenderTarget(0, &pSurface));
  GLB_DXTEST(pSurface->GetDesc(&d3dSurfDesc));
  rc.left   = (iX < 0) ? d3dSurfDesc.Width - uint(-iX) : iX;
  rc.top    = (iY < 0) ? d3dSurfDesc.Height - uint(-iY) : iY;
  rc.right  = d3dSurfDesc.Width;
  rc.bottom = d3dSurfDesc.Height;

  font->DrawText(NULL, str.c_str(), str.length(), &rc, dwFormat, d3dColor);
}




//---------------------------------------------------------------------------
Surface Renderer::createDepthStencilSurface (uint uiWidth, uint uiHeight, D3DFORMAT d3dFormat/*=D3DFMT_D24S8*/, bool bDiscard/*=true*/, D3DMULTISAMPLE_TYPE d3dMultisample/*=D3DMULTISAMPLE_NONE*/, DWORD dwMultisampleQuality/*=0*/)
{
  IDirect3DSurface9* pSurf;

  GLB_ASSERT(this->isInit());
  GLB_DXTEST(m_pD3DDev->CreateDepthStencilSurface(
    uiWidth, uiHeight, d3dFormat, d3dMultisample, dwMultisampleQuality,
    (bDiscard ? TRUE : FALSE), &pSurf, NULL));

  Surface surf;
  surf.attachRef(pSurf);
  return surf;
}

//---------------------------------------------------------------------------
Surface Renderer::getDepthStencilSurface (void)
{
  IDirect3DSurface9* pSurf;

  GLB_ASSERT(this->isInit());
  GLB_DXTEST(m_pD3DDev->GetDepthStencilSurface(&pSurf));

  Surface surf;
  surf.attachRef(pSurf);
  return surf;
}

//---------------------------------------------------------------------------
Surface Renderer::createRenderTarget (uint uiWidth, uint uiHeight, D3DFORMAT d3dFormat/*=D3DFMT_A8R8G8B8*/, bool bLockable/*=true*/, D3DMULTISAMPLE_TYPE d3dMultisample/*=D3DMULTISAMPLE_NONE*/, DWORD dwMultisampleQuality/*=0*/)
{
  IDirect3DSurface9* pSurf;

  GLB_ASSERT(this->isInit());
  GLB_DXTEST(m_pD3DDev->CreateRenderTarget(
    uiWidth, uiHeight, d3dFormat, d3dMultisample, dwMultisampleQuality,
    (bLockable ? TRUE : FALSE), &pSurf, NULL));

  Surface surf;
  surf.attachRef(pSurf);
  return surf;
}

//---------------------------------------------------------------------------
Surface Renderer::getRenderTarget (uint uiIndex/*=0*/)
{
  IDirect3DSurface9* pSurf;

  GLB_ASSERT(this->isInit());
  GLB_DXTEST(m_pD3DDev->GetRenderTarget(uiIndex, &pSurf));

  Surface surf;
  surf.attachRef(pSurf);
  return surf;
}




//---------------------------------------------------------------------------
Texture Renderer::loadTexture (const char* pszVirtPath)
{
  IDirect3DTexture9* pTex;

  GLB_ASSERT(this->isInit());
  GLB_ASSERT(g_pVFS());
  GLB_ASSERT(g_VFS().hasFile(pszVirtPath));

  if (g_VFS().isArchivedFile(pszVirtPath))
  {
    uint32    uiSize = 0;
    glb_byte* pData = g_VFS().acquireMemFile(pszVirtPath, &uiSize);

    GLB_DXTEST(
      D3DXCreateTextureFromFileInMemory(
        m_pD3DDev,
        pData, uiSize,
        &pTex),
      "Failed to create texture from \"%s\" !", pszVirtPath);
    g_VFS().releaseMemFile(pData);
  }
  else
  {
    StringA strFile(g_VFS().acquirePhysFile(pszVirtPath));

    GLB_DXTEST(
      D3DXCreateTextureFromFile(
        m_pD3DDev,
        strFile.c_str(),
        &pTex),
      "Failed to create texture from \"%s\" !", pszVirtPath);
    g_VFS().releasePhysFile(strFile);
  }

  Texture tex;
  tex.attachRef(pTex);
  return tex;
}

//---------------------------------------------------------------------------
CubeTexture Renderer::loadCubeTexture (const char* pszVirtPath)
{
  IDirect3DCubeTexture9* pTex;

  GLB_ASSERT(this->isInit());
  GLB_ASSERT(g_pVFS());
  GLB_ASSERT(g_VFS().hasFile(pszVirtPath));

  if (g_VFS().isArchivedFile(pszVirtPath))
  {
    uint32    uiSize = 0;
    glb_byte* pData = g_VFS().acquireMemFile(pszVirtPath, &uiSize);

    GLB_DXTEST(
      D3DXCreateCubeTextureFromFileInMemory(
        m_pD3DDev,
        pData, uiSize,
        &pTex),
      "Failed to create cube texture from \"%s\" !", pszVirtPath);
    g_VFS().releaseMemFile(pData);
  }
  else
  {
    StringA strFile(g_VFS().acquirePhysFile(pszVirtPath));

    GLB_DXTEST(
      D3DXCreateCubeTextureFromFile(
        m_pD3DDev,
        strFile.c_str(),
        &pTex),
      "Failed to create cube texture from \"%s\" !", pszVirtPath);
    g_VFS().releasePhysFile(strFile);
  }

  CubeTexture tex;
  tex.attachRef(pTex);
  return tex;
}

//---------------------------------------------------------------------------
VolumeTexture Renderer::loadVolumeTexture (const char* pszVirtPath)
{
  IDirect3DVolumeTexture9* pTex;

  GLB_ASSERT(this->isInit());
  GLB_ASSERT(g_pVFS());
  GLB_ASSERT(g_VFS().hasFile(pszVirtPath));

  if (g_VFS().isArchivedFile(pszVirtPath))
  {
    uint32    uiSize = 0;
    glb_byte* pData = g_VFS().acquireMemFile(pszVirtPath, &uiSize);

    GLB_DXTEST(
      D3DXCreateVolumeTextureFromFileInMemory(
        m_pD3DDev,
        pData, uiSize,
        &pTex),
      "Failed to create volume texture from \"%s\" !", pszVirtPath);
    g_VFS().releaseMemFile(pData);
  }
  else
  {
    StringA strFile(g_VFS().acquirePhysFile(pszVirtPath));

    GLB_DXTEST(
      D3DXCreateVolumeTextureFromFile(
        m_pD3DDev,
        strFile.c_str(),
        &pTex),
      "Failed to create volume texture from \"%s\" !", pszVirtPath);
    g_VFS().releasePhysFile(strFile);
  }

  VolumeTexture tex;
  tex.attachRef(pTex);
  return tex;
}

//---------------------------------------------------------------------------
Texture Renderer::createTexture (uint uiWidth, uint uiHeight, uint uiLevels, DWORD dwUsage, D3DFORMAT d3dFormat, D3DPOOL d3dPool/*=D3DPOOL_DEFAULT*/)
{
  IDirect3DTexture9* pTex;

  GLB_ASSERT(this->isInit());
  GLB_DXTEST(
    m_pD3DDev->CreateTexture(
      uiWidth, uiHeight, uiLevels, dwUsage, d3dFormat, d3dPool, &pTex, NULL),
    "Failed to create Direct3D texture !");

  Texture tex;
  tex.attachRef(pTex);
  return tex;
}

//---------------------------------------------------------------------------
VolumeTexture Renderer::createVolumeTexture (uint uiWidth, uint uiHeight, uint uiDepth, uint uiLevels, DWORD dwUsage, D3DFORMAT d3dFormat, D3DPOOL d3dPool/*=D3DPOOL_DEFAULT*/)
{
  IDirect3DVolumeTexture9* pTex;

  GLB_ASSERT(this->isInit());
  GLB_DXTEST(
    m_pD3DDev->CreateVolumeTexture(
      uiWidth, uiHeight, uiDepth, uiLevels, dwUsage, d3dFormat, d3dPool, &pTex, NULL),
    "Failed to create Direct3D volume texture !");

  VolumeTexture tex;
  tex.attachRef(pTex);
  return tex;
}




//---------------------------------------------------------------------------
VertexBuffer Renderer::createVertexBuffer (uint uiLength, DWORD dwUsage, DWORD dwFVF, D3DPOOL d3dPool/*=D3DPOOL_MANAGED*/)
{
  IDirect3DVertexBuffer9* pVtxBuf;

  GLB_ASSERT(this->isInit());
  GLB_DXTEST(
    m_pD3DDev->CreateVertexBuffer(
      uiLength, dwUsage, dwFVF, d3dPool, &pVtxBuf, NULL),
    "Failed to create Direct3D vertex buffer !");

  VertexBuffer vtxbuf;
  vtxbuf.attachRef(pVtxBuf);
  return vtxbuf;
}

//---------------------------------------------------------------------------
IndexBuffer Renderer::createIndexBuffer (uint uiLength, DWORD dwUsage, D3DFORMAT d3dFormat, D3DPOOL d3dPool/*=D3DPOOL_MANAGED*/)
{
  IDirect3DIndexBuffer9* pIdxBuf;

  GLB_ASSERT(this->isInit());
  GLB_DXTEST(
    m_pD3DDev->CreateIndexBuffer(
      uiLength, dwUsage, d3dFormat, d3dPool, &pIdxBuf, NULL),
    "Failed to create Direct3D index buffer !");

  IndexBuffer idxbuf;
  idxbuf.attachRef(pIdxBuf);
  return idxbuf;
}

//---------------------------------------------------------------------------
VertexDeclaration Renderer::createVertexDeclaration (const D3DVERTEXELEMENT9* pVtxElements)
{
  IDirect3DVertexDeclaration9* pVtxDecl;

  GLB_ASSERT(this->isInit());
  GLB_DXTEST(
    m_pD3DDev->CreateVertexDeclaration(pVtxElements, &pVtxDecl),
    "Failed to create Direct3D vertex declaration !");

  VertexDeclaration vtxdecl;
  vtxdecl.attachRef(pVtxDecl);
  return vtxdecl;
}




//---------------------------------------------------------------------------
void Renderer::directxResultTest (HRESULT hRes, const char* pszSrcFile/*=NULL*/, uint uiSrcLine/*=0*/, const char* pszMessageFormat/*=NULL*/, ...)
{
  if (FAILED(hRes))
  {
    StringA strMessage;
    va_list arglist;

    strMessage.format(
      "DirectX call failed (%s : %s) !",
      DXGetErrorString9(hRes),
      DXGetErrorDescription9(hRes));

    if (pszMessageFormat && pszMessageFormat[0])
    {
      strMessage += " Error : ";

      va_start(arglist, pszMessageFormat);
      strMessage.formatAppendV(pszMessageFormat, arglist);
      va_end(arglist);
    }

    throw Exception(pszSrcFile, uiSrcLine, (uint)EXCODE_SYSTEM_API, strMessage.c_str());
  }
}


} // namespace glb
#endif // #ifdef GLOBE_GFX
