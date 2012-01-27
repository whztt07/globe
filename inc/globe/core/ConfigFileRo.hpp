//***************************************************************************
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
