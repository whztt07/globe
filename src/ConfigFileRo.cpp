//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-19 19:10:37
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
void ConfigFileRo::load (const char* pszPath)
{
  StringAVec vecLines;

  // reset
  m_Settings.clear();

  // check file
  if (!FileSystem::doesExist(pszPath))
    GLB_THROWEXCODE(EXCODE_FILE_DOESNTEXIST);
  if (FileSystem::getSize(pszPath) > std::numeric_limits<uint32>::max())
    GLB_THROWEXCODE(EXCODE_MEMORY); // file is too big

  // extract file content
  {
    uint32  uiSize = (uint32)FileSystem::getSize(pszPath); // it is safe to cast-down here
    StringA strLines;
    byte*   pBuffer;

    pBuffer = (byte*)strLines.acquireBuffer(uiSize, false);
    FileSystem::extractContent(pszPath, pBuffer, uiSize);
    strLines.releaseBuffer();

    strLines.trimWhitespaces();
    strLines.explodeWithOneOf(vecLines, "\r\n");
  }

  // parse lines
  for (uint i = 0; i < vecLines.size(); ++i)
  {
    StringAVec vecL;

    // skip empty and comment lines
    vecLines[i].trimWhitespaces();
    if (vecLines[i].isEmpty() || vecLines[i][0] == '#')
      continue;

    // break key-value pair
    vecLines[i].explodeWithOneOf(vecL, "\t ", 2);
    if (vecL.size() >= 1)
    {
      if (vecL.size() < 2)
        vecL.push_back(StringA());
      if (m_Settings.find(vecL[0]) != m_Settings.end())
        GLB_LOGWARN("Duplicated setting name \"%s\" in config file \"%s\" !", vecL[0].c_str(), pszPath);

      m_Settings.insert(Settings::value_type(vecL[0], vecL[1]));
    }
  }
}

//---------------------------------------------------------------------------
bool ConfigFileRo::hasValue (const StringA& strName) const
{
  return m_Settings.find(strName) != m_Settings.end();
}

//---------------------------------------------------------------------------
StringA ConfigFileRo::getValue (const StringA& strName, const StringA& strDefaultValue/*=""*/) const
{
  SettingsCIt itSet = m_Settings.find(strName);
  return
    (itSet == m_Settings.end()) ?
    strDefaultValue :
    itSet->second;
}


} // namespace glb
