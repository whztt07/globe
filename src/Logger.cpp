//***************************************************************************
//
// Globe
// A small C++ library to allow fast prototyping of Direct3D animations.
//
// Copyright (C) 2012-2013 Jean-Charles Lefebvre <jcl [AT] jcl [DOT] io>
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
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-07 18:31:28
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
// Static Variables
//---------------------------------------------------------------------------
Logger::Handler Logger::ms_Handler   = NULL;
bool            Logger::ms_bShowNext = false;
std::ostrstream Logger::ms_Stream;


//---------------------------------------------------------------------------
char Logger::levelToChar (Level eLevel)
{
  switch (eLevel)
  {
    case LLDEBUG : return 'd';
    case LLINFO  : return 'i';
    case LLWARN  : return 'W';
    case LLERROR : return 'E';
    case LLFATAL : return 'X';
    default      : return '?';
  }
}

//---------------------------------------------------------------------------
void Logger::print (Level eLevel, const char* pszFile, int iLine, const char* pszFormat, ...)
{
  va_list arglist;
  StringA strMessage;

  // format message if needed
  if (pszFormat[0])
  {
    va_start(arglist, pszFormat);
    strMessage.formatV(pszFormat, arglist);
    va_end(arglist);
  }

  // output message
  if (ms_Handler)
  {
    ms_Handler(eLevel, pszFile, iLine, strMessage);
    ms_bShowNext = false;
  }
  else
  {
    StringA strLog;

#ifdef GLB_DEBUG
    if (pszFile && pszFile[0])
    {
      StringA strFileName(pszFile);
      strFileName.pathStripDirectory();
#ifdef GLB_PLATFORM_WINDOWS
      strLog.format("%s(%i): ", pszFile, iLine);
      OutputDebugStringA(strLog.c_str());
#endif
      strLog.format("%s(%i): ", strFileName.c_str(), iLine);
      ms_Stream << strLog.c_str();
    }
#endif

    strLog.format(
      "%s [%c] %s\n",
      DateTime::nowToString(false, true, true).c_str(),
      Logger::levelToChar(eLevel),
      strMessage.c_str());

#if defined(GLB_DEBUG) && defined(GLB_PLATFORM_WINDOWS)
    OutputDebugStringA(strLog.c_str());
#endif

    ms_Stream << strLog.c_str();

    if (ms_bShowNext)
    {
      Logger::showMessage(eLevel, pszFile, iLine, strMessage);
      ms_bShowNext = false;
    }
  }

}

//---------------------------------------------------------------------------
void Logger::save (const char* pszFile)
{
  FILE*   pFile;
  StringA strLog;

  pFile = fopen(pszFile, "w");
  if (!pFile)
    GLB_THROW(EXCODE_FILE_CREATE, "Failed to create log file \"%s\" !", pszFile);
  if (fwrite(ms_Stream.str(), 1, (size_t)ms_Stream.pcount(), pFile) != (size_t)ms_Stream.pcount())
    GLB_THROW(EXCODE_FILE_WRITING, "Failed to create complete log into file \"%s\" !", pszFile);
  fclose(pFile);
}

//---------------------------------------------------------------------------
void Logger::saveIfNotEmpty (const char* pszFile)
{
  if (ms_Stream.pcount() > 0)
    Logger::save(pszFile);
  else if (FileSystem::doesExist(pszFile))
    FileSystem::remove(pszFile);
}




//---------------------------------------------------------------------------
void Logger::showMessage (Level eLevel, const char* pszFile, int iLine, const char* pszMessage)
{
#ifdef GLB_PLATFORM_WINDOWS
  StringA strText;
  UINT    uiType;

  switch (eLevel)
  {
    case LLFATAL :
    case LLERROR :
      uiType = MB_ICONERROR;
      break;

    case LLWARN :
      uiType = MB_ICONEXCLAMATION;
      break;

    default :
      uiType = MB_ICONINFORMATION;
      break;
  }

  if (pszFile && pszFile[0])
  {
    StringA strFileName(pszFile);

    strFileName.pathStripDirectory();
    strText.format("%s at line %i :\n", strFileName.c_str(), iLine);
  }

  strText += pszMessage;

  MessageBox(NULL, strText.c_str(), GLB_BRAND_NAME " Application", uiType);
#endif
}


} // namespace glb
