//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-07 18:41:01
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline void Logger::setHandler (Logger::Handler handler)
{
  ms_Handler = handler;
}

//---------------------------------------------------------------------------
inline void Logger::showNext (bool bEnable)
{
  ms_bShowNext = bEnable;
}

//---------------------------------------------------------------------------
inline bool Logger::getShowNext (void)
{
  return ms_bShowNext;
}

//---------------------------------------------------------------------------
inline void Logger::clear (void)
{
  ms_Stream.clear();
}
