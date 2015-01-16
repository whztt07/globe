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
// Created On: 2011-10-09 19:59:14
//
//***************************************************************************

#ifndef __GLOBE_EXCEPTION_H__
#define __GLOBE_EXCEPTION_H__
namespace glb {


//---------------------------------------------------------------------------
// Exception
//---------------------------------------------------------------------------
class GLB_EXPORT Exception : public ::std::exception
{
public :
  Exception (void);
  Exception (const char* pszMessage);
  Exception (const Exception& e);
  Exception (const ::std::exception& e);
  Exception (const char* pszFilePath, uint uiFileLine, uint uiExCode, const char* pszMessageFormat, ...);
  virtual ~Exception (void) throw();

  // operators
  Exception& operator= (const Exception& e);
  Exception& operator= (const ::std::exception& e);

  // description
  virtual const char* what           (void) const throw();
  virtual const char* getDescription (void) const throw(); // alias for what()

  // miscellaneous properties getters
  virtual const char* getFilePath (void) const;
  virtual uint        getFileLine (void) const;
  virtual uint        getCode     (void) const;

  // utilities
  virtual void    log                     (void) const;
  virtual StringA getFileName             (void) const;
  virtual StringA getCodeString           (void) const;
  virtual StringA getDescriptionMultiline (void) const;


  // static utils
  static bool        isUserExCode                (uint uiExCode);
  static bool        isStandardExCode            (ExceptionCode eExCode);
  static const char* standardExCodeToName        (ExceptionCode eExCode);
  static const char* standardExCodeToDescription (ExceptionCode eExCode);


protected :
  // description
  StringA m_strDescription;

  // miscellaneous properties
  const char* m_pszFilePath;
  uint        m_uiFileLine;
  uint        m_uiCode;
};


#include "Exception.inl.hpp"


} // namespace glb
#endif // #ifndef __GLOBE_EXCEPTION_H__
