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
// Created On: 2011-10-10 01:56:41
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
