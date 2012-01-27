//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-09 20:18:54
//
// $Id$
//
//***************************************************************************

#include "_internal.hpp"
namespace glb {


//---------------------------------------------------------------------------
// ExceptionCode descriptions
//---------------------------------------------------------------------------
static const struct EXCODESDESC_
{
  ExceptionCode eExCode;
  const char*   pszExCode;
  const char*   pszDescription;
}
EXCODESDESC[] =
{
  #define EXCODE_DEFINE(code,name,desc)  { name, #name, desc " (" #name ")" },
  #include <globe/core/ExceptionCodes.hpp>
  #undef EXCODE_DEFINE
};

static const int EXCODESDESC_NUM = sizeof(EXCODESDESC) / sizeof(EXCODESDESC[0]);


//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------
static const char* c_pszEmpty = "";



//---------------------------------------------------------------------------
Exception::Exception (void)
: ::std::exception()
, m_pszFilePath(c_pszEmpty)
, m_uiFileLine(0)
, m_uiCode(EXCODE_NONE)
{
}

//---------------------------------------------------------------------------
Exception::Exception (const char* pszMessage)
: ::std::exception() //::std::exception(pszMessage)
, m_pszFilePath(c_pszEmpty)
, m_uiFileLine(0)
, m_uiCode(EXCODE_NONE)
{
}

//---------------------------------------------------------------------------
Exception::Exception (const Exception& e)
: ::std::exception() //::std::exception(m_strDescription.c_str())
, m_strDescription(e.m_strDescription)
, m_pszFilePath(c_pszEmpty)
, m_uiFileLine(0)
, m_uiCode(EXCODE_NONE)
{
}

//---------------------------------------------------------------------------
Exception::Exception (const ::std::exception& e)
: ::std::exception() //::std::exception(m_strDescription.c_str())
, m_strDescription(e.what())
, m_pszFilePath(c_pszEmpty)
, m_uiFileLine(0)
, m_uiCode(EXCODE_STD_EXCEPTION)
{
}

//---------------------------------------------------------------------------
Exception::Exception (const char* pszFilePath, uint uiFileLine, uint uiExCode, const char* pszMessageFormat, ...)
: ::std::exception()
{
  // description
  if ((pszMessageFormat != NULL) && (pszMessageFormat[0] != 0))
  {
    va_list arglist;

    va_start(arglist, pszMessageFormat);
    m_strDescription.formatV(pszMessageFormat, arglist);
    va_end(arglist);
  }

  // miscellaneous properties
  m_pszFilePath = pszFilePath ? pszFilePath : c_pszEmpty;
  m_uiFileLine  = uiFileLine;
  m_uiCode      = uiExCode;
}

//---------------------------------------------------------------------------
Exception::~Exception (void) throw()
{
}



//---------------------------------------------------------------------------
Exception& Exception::operator= (const Exception& e)
{
  // description
  m_strDescription = e.m_strDescription;

  // miscellaneous properties
  m_pszFilePath = e.m_pszFilePath;
  m_uiFileLine  = e.m_uiFileLine;
  m_uiCode      = e.m_uiCode;

  return *this;
}

//---------------------------------------------------------------------------
Exception& Exception::operator= (const ::std::exception& e)
{
  // description
  m_strDescription = e.what();

  // miscellaneous properties
  m_pszFilePath = c_pszEmpty;
  m_uiFileLine  = 0;
  m_uiCode      = EXCODE_STD_EXCEPTION;

  return *this;
}



//---------------------------------------------------------------------------
void Exception::log (void) const
{
  StringA strMessage = "EXCEPTION : " + m_strDescription;

  if (Exception::isStandardExCode((ExceptionCode)m_uiCode))
  {
    strMessage += " (";
    strMessage += Exception::standardExCodeToName((ExceptionCode)m_uiCode);
    strMessage += ")";
  }
  else if (Exception::isUserExCode(m_uiCode))
  {
    strMessage.formatAppend(" (code %u)", m_uiCode);
  }

  Logger::print(Logger::LLFATAL, m_pszFilePath, m_uiFileLine, strMessage);
}

//---------------------------------------------------------------------------
StringA Exception::getFileName (void) const
{
  char* p;

  if (!m_pszFilePath[0])
    return StringA();

  p = (char*)(m_pszFilePath + strlen(m_pszFilePath));
  while ((p > (char*)m_pszFilePath) && (*p))
  {
    if ((*p == '/') || (*p == '\\'))
    {
      ++p;
      break;
    }
    --p;
  }

  return p;
}

//---------------------------------------------------------------------------
StringA Exception::getCodeString (void) const
{
  StringA strCode;

  if (Exception::isStandardExCode((ExceptionCode)m_uiCode))
    strCode = Exception::standardExCodeToName((ExceptionCode)m_uiCode);
  else
    strCode = m_uiCode;

  return strCode;
}

//---------------------------------------------------------------------------
StringA Exception::getDescriptionMultiline (void) const
{
  StringA strDesc;

  strDesc.format(
    "Exception caught !\n"
    "\n"
    "Source File\t%s\n"
    "Source Line\t%u\n"
    "Exception Code\t%s\n"
    "\n"
    "%s" //"%s\n"
    //"%s"
    //"Based on %s compiled on %s."
    , m_pszFilePath[0] ? this->getFileName().c_str() : "?"
    , m_uiFileLine
    , this->getCodeString().c_str()
    , m_strDescription.c_str()
    //, m_strDescription.IsEmpty() ? "" : "\n\n"
    //, Root::GetLibraryTitle().c_str(), Root::GetLibraryTimeStamp().c_str()
  );

  return strDesc;
}



//---------------------------------------------------------------------------
bool Exception::isStandardExCode (ExceptionCode eExCode)
{
  for (int Ix = 0; Ix < EXCODESDESC_NUM; ++Ix)
  {
    if (EXCODESDESC[Ix].eExCode == eExCode)
      return true;
  }

  return false;
}

//---------------------------------------------------------------------------
const char* Exception::standardExCodeToName (ExceptionCode eExCode)
{
  GLB_ASSERT(!Exception::isUserExCode(uint(eExCode)));
  GLB_ASSERT(eExCode != EXCODE_NONE);
  GLB_ASSERT(eExCode != EXCODE_USER_BASE);

  if ((eExCode == EXCODE_NONE) || (eExCode == EXCODE_USER_BASE) || Exception::isUserExCode(uint(eExCode)))
    return c_pszEmpty;

  for (int Ix = 0; Ix < EXCODESDESC_NUM; ++Ix)
  {
    if (EXCODESDESC[Ix].eExCode == eExCode)
      return EXCODESDESC[Ix].pszExCode;
  }

  GLB_ASSERT(0);
  return c_pszEmpty;
}

//---------------------------------------------------------------------------
const char* Exception::standardExCodeToDescription (ExceptionCode eExCode)
{
  GLB_ASSERT(!Exception::isUserExCode(uint(eExCode)));
  GLB_ASSERT(eExCode != EXCODE_NONE);
  GLB_ASSERT(eExCode != EXCODE_USER_BASE);

  if ((eExCode == EXCODE_NONE) || (eExCode == EXCODE_USER_BASE) || Exception::isUserExCode(uint(eExCode)))
    return c_pszEmpty;

  for (int Ix = 0; Ix < EXCODESDESC_NUM; ++Ix)
  {
    if (EXCODESDESC[Ix].eExCode == eExCode)
      return EXCODESDESC[Ix].pszDescription;
  }

  GLB_ASSERT(0);
  return c_pszEmpty;
}


} // namespace glb
