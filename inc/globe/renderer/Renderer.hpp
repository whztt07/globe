//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-11-28 22:23:52
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_RENDERER_H__
#define __GLOBE_RENDERER_H__
namespace glb {


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
#ifdef GLB_DEBUG
# define GLB_DXTEST(hr, ...) ::glb::Renderer::directxResultTest(hr, __FILE__, __LINE__, ## __VA_ARGS__)
#else
# define GLB_DXTEST(hr, ...) ::glb::Renderer::directxResultTest(hr, NULL, 0, ## __VA_ARGS__)
#endif


//---------------------------------------------------------------------------
// Renderer
//---------------------------------------------------------------------------
class GLB_EXPORT Renderer : public Singleton<Renderer>
{
public :
  Renderer (HINSTANCE hInstance);
  ~Renderer (void);

  // singleton stuff
  static Renderer& getSingleton    (void);
  static Renderer* getSingletonPtr (void);

  // d3d interface (created by the constructor)
  IDirect3D9* getD3D (void);

  // d3d device
  void              init    (const char* pszWndTitle, uint uiWidth, uint uiHeight, D3DFORMAT d3dFormat, bool bFullscreen, bool bVSync, uint uiAdapter=D3DADAPTER_DEFAULT, DWORD dwMinVertexShaderVersion=D3DVS_VERSION(0,0));
  void              uninit  (void);
  bool              isInit  (void) const;
  IDirect3DDevice9* getDev  (void);
  HWND              getHWnd (void);

  // d3d device viewport
  D3DVIEWPORT9 getViewport           (void) const;
  void         setViewport           (const D3DVIEWPORT9* pViewport);
  D3DVIEWPORT9 makeLetterboxViewport (float rDesiredAspectRatio) const;

  // font
  Font loadFont (const char* pszVirtPath, const char* pszFaceName, int iHeight, uint uiMipLevels, bool bAntialiased=false, bool bBold=false);
  void printHud (Font& font, bool bTop, const char* pszFormat, ...);
  void printHud (Font& font, int iX, int iY, D3DCOLOR d3dColor, DWORD dwFormat, const char* pszFormat, ...);

  // surfaces
  Surface createDepthStencilSurface (uint uiWidth, uint uiHeight, D3DFORMAT d3dFormat=D3DFMT_D24S8, bool bDiscard=true, D3DMULTISAMPLE_TYPE d3dMultisample=D3DMULTISAMPLE_NONE, DWORD dwMultisampleQuality=0);
  void    setDepthStencilSurface    (Surface& surf);
  void    setDepthStencilSurface    (IDirect3DSurface9* pSurface);
  Surface getDepthStencilSurface    (void);
  Surface createRenderTarget        (uint uiWidth, uint uiHeight, D3DFORMAT d3dFormat=D3DFMT_A8R8G8B8, bool bLockable=true, D3DMULTISAMPLE_TYPE d3dMultisample=D3DMULTISAMPLE_NONE, DWORD dwMultisampleQuality=0);
  void    setRenderTarget           (Surface& surf, uint uiIndex=0);
  void    setRenderTarget           (IDirect3DSurface9* pSurface, uint uiIndex=0);
  Surface getRenderTarget           (uint uiIndex=0);

  // textures
  Texture       loadTexture         (const char* pszVirtPath);
  CubeTexture   loadCubeTexture     (const char* pszVirtPath);
  VolumeTexture loadVolumeTexture   (const char* pszVirtPath);
  Texture       createTexture       (uint uiWidth, uint uiHeight, uint uiLevels, DWORD dwUsage, D3DFORMAT d3dFormat, D3DPOOL d3dPool=D3DPOOL_DEFAULT);
  VolumeTexture createVolumeTexture (uint uiWidth, uint uiHeight, uint uiDepth, uint uiLevels, DWORD dwUsage, D3DFORMAT d3dFormat, D3DPOOL d3dPool=D3DPOOL_DEFAULT);

  // vertex buffers
  VertexBuffer      createVertexBuffer      (uint uiLength, DWORD dwUsage, DWORD dwFVF, D3DPOOL d3dPool=D3DPOOL_MANAGED);
  IndexBuffer       createIndexBuffer       (uint uiLength, DWORD dwUsage, D3DFORMAT d3dFormat, D3DPOOL d3dPool=D3DPOOL_MANAGED);
  VertexDeclaration createVertexDeclaration (const D3DVERTEXELEMENT9* pVtxElements);


  // static utils
  static void directxResultTest (HRESULT hRes, const char* pszSrcFile=NULL, uint uiSrcLine=0, const char* pszMessageFormat=NULL, ...);


private :
  HINSTANCE         m_hInstance;
  IDirect3D9*       m_pD3D;
  IDirect3DDevice9* m_pD3DDev;
  HWND              m_hWnd;
  bool              m_bFullscreen;
};


#include "Renderer.inl.hpp"

static inline Renderer& g_Renderer()  { return Renderer::getSingleton(); }
static inline Renderer* g_pRenderer() { return Renderer::getSingletonPtr(); }


} // namespace glb
#endif // #ifndef __GLOBE_RENDERER_H__
