//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-07 18:18:44
//
// $Id$
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
