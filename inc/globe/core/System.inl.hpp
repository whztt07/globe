//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-10 01:56:41
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline bool System::isBigEndian (void)
{
  if (ms_nIsBigEndian < 0)
    (void)System::detectEndianness();
  return ms_nIsBigEndian != 0;
}

//---------------------------------------------------------------------------
inline void System::swapBytes (uint16 valSource, byte* pDestBytes)
{
  struct { byte _[sizeof(uint16)]; } bytes;

  *(uint16*)&bytes = valSource;

  *pDestBytes++ = bytes._[1];
  *pDestBytes   = bytes._[0];
}

//---------------------------------------------------------------------------
inline void System::swapBytes (uint32 valSource, byte* pDestBytes)
{
  struct { byte _[sizeof(uint32)]; } bytes;

  *(uint32*)&bytes = valSource;

  *pDestBytes++ = bytes._[3];
  *pDestBytes++ = bytes._[2];
  *pDestBytes++ = bytes._[1];
  *pDestBytes   = bytes._[0];
}

//---------------------------------------------------------------------------
inline void System::swapBytes (uint64 valSource, byte* pDestBytes)
{
  struct { byte _[sizeof(uint64)]; } bytes;

  *(uint64*)&bytes = valSource;

  *pDestBytes++ = bytes._[7];
  *pDestBytes++ = bytes._[6];
  *pDestBytes++ = bytes._[5];
  *pDestBytes++ = bytes._[4];
  *pDestBytes++ = bytes._[3];
  *pDestBytes++ = bytes._[2];
  *pDestBytes++ = bytes._[1];
  *pDestBytes   = bytes._[0];
}

//---------------------------------------------------------------------------
inline void System::swapBytes (float32 valSource, byte* pDestBytes)
{
  System::swapBytes(*((uint32*)&valSource), pDestBytes);
}

//---------------------------------------------------------------------------
inline void System::swapBytes (float64 valSource, byte* pDestBytes)
{
  System::swapBytes(*((uint64*)&valSource), pDestBytes);
}
