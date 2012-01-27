//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-09 19:59:17
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline const char* Exception::what (void) const throw()
{
  return m_strDescription.c_str();
}

//---------------------------------------------------------------------------
inline const char* Exception::getDescription (void) const throw()
{
  return m_strDescription.c_str();
}

//---------------------------------------------------------------------------
inline const char* Exception::getFilePath (void) const
{
  return m_pszFilePath;
}

//---------------------------------------------------------------------------
inline uint Exception::getFileLine (void) const
{
  return m_uiFileLine;
}

//---------------------------------------------------------------------------
inline uint Exception::getCode (void) const
{
  return m_uiCode;
}


//---------------------------------------------------------------------------
inline bool Exception::isUserExCode (uint uiExCode)
{
  return (uiExCode > uint(EXCODE_USER_BASE));
}
