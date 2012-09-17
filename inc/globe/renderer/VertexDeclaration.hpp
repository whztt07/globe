//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-06 02:31:10
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_VERTEXDECLARATION_H__
#define __GLOBE_VERTEXDECLARATION_H__
#ifdef GLOBE_GFX
namespace glb {


//---------------------------------------------------------------------------
// VertexDeclaration
//---------------------------------------------------------------------------
class GLB_EXPORT VertexDeclaration : public SmartPointer<IDirect3DVertexDeclaration9>
{
public :
  VertexDeclaration (IDirect3DVertexDeclaration9* pVertexDeclaration=0);
};


#include "VertexDeclaration.inl.hpp"


} // namespace glb
#endif // #ifdef GLOBE_GFX
#endif // #ifndef __GLOBE_VERTEXDECLARATION_H__
