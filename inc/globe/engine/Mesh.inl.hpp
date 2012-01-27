//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-12-08 01:53:41
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline Drawable* Mesh::drawable (void)
{
  return static_cast<Drawable*>(this);
}

//---------------------------------------------------------------------------
inline void Mesh::draw (void)
{
  GLB_ASSERT(this->getRef());
  this->getRef()->DrawSubset(0);
}

//---------------------------------------------------------------------------
inline void Mesh::draw (uint uiSubset)
{
  GLB_ASSERT(this->getRef());
  this->getRef()->DrawSubset(uiSubset);
}
