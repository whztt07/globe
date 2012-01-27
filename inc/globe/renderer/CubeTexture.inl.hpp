//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-04 20:59:48
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline CubeTexture::CubeTexture (IDirect3DCubeTexture9* pCubeTexture/*=0*/)
: SmartPointer<IDirect3DCubeTexture9>(pCubeTexture)
{
}
