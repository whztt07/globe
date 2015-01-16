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
// Created On: 2011-10-07 18:18:44
//
//***************************************************************************

#ifndef __GLOBE_LOGGER_H__
#define __GLOBE_LOGGER_H__
namespace glb {


//---------------------------------------------------------------------------
// Logger
//---------------------------------------------------------------------------
class LoggerHandler;

class GLB_EXPORT Logger
{
public :
  enum Level
  {
    LLDEBUG,
    LLINFO,
    LLWARN,
    LLERROR,
    LLFATAL,
  };

  typedef void (*Handler)(Logger::Level eLevel, const char* pszFile, int iLine, const char* pszMessage);


public :
  static void setHandler  (Handler handler); // set handler to null to restore default behavior
  static void showNext    (bool bEnable);
  static bool getShowNext (void);

  static void print          (Level eLevel, const char* pszFile, int iLine, const char* pszFormat, ...);
  static void clear          (void);
  static void save           (const char* pszFile);
  static void saveIfNotEmpty (const char* pszFile);

  // utils
  static char levelToChar (Level eLevel);
  static void showMessage (Level eLevel, const char* pszFile, int iLine, const char* pszMessage);


private :
  static Handler         ms_Handler;
  static bool            ms_bShowNext;
  static std::ostrstream ms_Stream;
};


#include "Logger.inl.hpp"


} // namespace glb
#endif // #ifndef __GLOBE_LOGGER_H__
