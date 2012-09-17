//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-08 01:53:44
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
Mesh::Mesh (ID3DXMesh* pMesh/*=0*/)
: SmartPointer<ID3DXMesh>(pMesh)
{
}

//---------------------------------------------------------------------------
VertexBuffer Mesh::getVertexBuffer (void)
{
  IDirect3DVertexBuffer9* pVertexBuffer;

  GLB_ASSERT(this->getRef());
  GLB_DXTEST(this->getRef()->GetVertexBuffer(&pVertexBuffer));

  return VertexBuffer(pVertexBuffer);
}

//---------------------------------------------------------------------------
IndexBuffer Mesh::getIndexBuffer (void)
{
  IDirect3DIndexBuffer9* pIndexBuffer;

  GLB_ASSERT(this->getRef());
  GLB_DXTEST(this->getRef()->GetIndexBuffer(&pIndexBuffer));

  return IndexBuffer(pIndexBuffer);
}




//---------------------------------------------------------------------------
Mesh Mesh::createBox (float rWidth, float rHeight, float rDepth)
{
  ID3DXMesh* pMesh;

  GLB_ASSERT(g_pRenderer());

  GLB_DXTEST(D3DXCreateBox(
    g_Renderer().getDev(), rWidth, rHeight, rDepth, &pMesh, NULL));

  Mesh mesh;
  mesh.attachRef(pMesh);
  return mesh;
}

//---------------------------------------------------------------------------
Mesh Mesh::createCylinder (float rRadius1, float rRadius2, float rLength, uint uiSlices, uint uiStacks)
{
  ID3DXMesh* pMesh;

  GLB_ASSERT(g_pRenderer());

  GLB_DXTEST(D3DXCreateCylinder(
    g_Renderer().getDev(), rRadius1, rRadius2, rLength, uiSlices, uiStacks,
    &pMesh, NULL));

  Mesh mesh;
  mesh.attachRef(pMesh);
  return mesh;
}

//---------------------------------------------------------------------------
Mesh Mesh::createPolygon (float rLength, uint uiSides)
{
  ID3DXMesh* pMesh;

  GLB_ASSERT(g_pRenderer());

  GLB_DXTEST(D3DXCreatePolygon(
    g_Renderer().getDev(), rLength, uiSides, &pMesh, NULL));

  Mesh mesh;
  mesh.attachRef(pMesh);
  return mesh;
}

//---------------------------------------------------------------------------
Mesh Mesh::createSphere (float rRadius, uint uiSlices, uint uiStacks)
{
  ID3DXMesh* pMesh;

  GLB_ASSERT(g_pRenderer());

  GLB_DXTEST(D3DXCreateSphere(
    g_Renderer().getDev(), rRadius, uiSlices, uiStacks, &pMesh, NULL));

  Mesh mesh;
  mesh.attachRef(pMesh);
  return mesh;
}

//---------------------------------------------------------------------------
Mesh Mesh::createTeapot (void)
{
  ID3DXMesh* pMesh;

  GLB_ASSERT(g_pRenderer());

  GLB_DXTEST(D3DXCreateTeapot(g_Renderer().getDev(), &pMesh, NULL));

  Mesh mesh;
  mesh.attachRef(pMesh);
  return mesh;
}

//---------------------------------------------------------------------------
Mesh Mesh::createTorus (float rInnerRadius, float rOuterRadius, uint uiSides, uint uiRings)
{
  ID3DXMesh* pMesh;

  GLB_ASSERT(g_pRenderer());

  GLB_DXTEST(D3DXCreateTorus(
    g_Renderer().getDev(), rInnerRadius, rOuterRadius, uiSides, uiRings,
    &pMesh, NULL));

  Mesh mesh;
  mesh.attachRef(pMesh);
  return mesh;
}

//---------------------------------------------------------------------------
Mesh Mesh::loadX (const char* pszVirtPath)
{
  ID3DXMesh* pMesh;
  HRESULT hRes;

  GLB_ASSERT(g_pVFS());
  GLB_ASSERT(g_pRenderer());
  GLB_ASSERT(g_VFS().hasFile(pszVirtPath));

  if (g_VFS().isArchivedFile(pszVirtPath))
  {
    uint32    uiSize = 0;
    glb_byte* pData = g_VFS().acquireMemFile(pszVirtPath, &uiSize);

    hRes = D3DXLoadMeshFromXInMemory(
      pData, uiSize,
      0,
      g_Renderer().getDev(),
      NULL, NULL, NULL, NULL,
      &pMesh);
    g_VFS().releaseMemFile(pData);
  }
  else
  {
    StringA strFile(g_VFS().acquirePhysFile(pszVirtPath));

    hRes = D3DXLoadMeshFromX(
      strFile.c_str(),
      0,
      g_Renderer().getDev(),
      NULL, NULL, NULL, NULL,
      &pMesh);
    g_VFS().releasePhysFile(strFile);
  }

  if (FAILED(hRes))
    GLB_THROW(EXCODE_SYSTEM_API, "Failed to load X mesh \"%s\" ! Error %s : %s", pszVirtPath, DXGetErrorString9(hRes), DXGetErrorDescription9(hRes));

  Mesh mesh;
  mesh.attachRef(pMesh);
  return mesh;
}


} // namespace glb
#endif // #ifdef GLOBE_GFX
