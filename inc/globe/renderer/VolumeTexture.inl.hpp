//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-04 20:59:59
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline VolumeTexture::VolumeTexture (IDirect3DVolumeTexture9* pVolumeTexture/*=0*/)
: SmartPointer<IDirect3DVolumeTexture9>(pVolumeTexture)
{
}
