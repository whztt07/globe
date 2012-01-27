//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-06 02:31:12
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline VertexDeclaration::VertexDeclaration (IDirect3DVertexDeclaration9* pVertexDeclaration/*=0*/)
: SmartPointer<IDirect3DVertexDeclaration9>(pVertexDeclaration)
{
}
