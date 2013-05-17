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
// Created On : 2011-10-19 19:10:35
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_CONFIGFILERO_H__
#define __GLOBE_CONFIGFILERO_H__
namespace glb {


//---------------------------------------------------------------------------
// ConfigFileRo
//---------------------------------------------------------------------------
class GLB_EXPORT ConfigFileRo
{
public :
  typedef std::map<StringA,StringA>  Settings;
  typedef Settings::iterator         SettingsIt;
  typedef Settings::const_iterator   SettingsCIt;


public :
  void load (const char* pszPath);

  uint    valuesCount (void) const;
  bool    hasValue    (const StringA& strName) const;
  StringA getValue    (const StringA& strName, const StringA& strDefaultValue="") const;


private :
  Settings m_Settings;
};


} // namespace glb
#endif // #ifndef __GLOBE_CONFIGFILERO_H__
