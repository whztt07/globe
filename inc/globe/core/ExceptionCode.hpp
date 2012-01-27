//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-09 20:12:53
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_EXCODE_H__
#define __GLOBE_EXCODE_H__
namespace glb {


//---------------------------------------------------------------------------
// ExceptionCode
//---------------------------------------------------------------------------
enum ExceptionCode
{
  #define EXCODE_DEFINE(code,name,desc)  name = code,
  #include "ExceptionCodes.hpp"
  #undef EXCODE_DEFINE

  GLBEXCEPTIONCODE_FORCE32 = 0x7FFFFFFF,
};


} // namespace glb
#endif // #ifndef __GLOBE_EXCODE_H__
