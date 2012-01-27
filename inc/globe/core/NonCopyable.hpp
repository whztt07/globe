//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-08 00:58:45
//
// $Id$
//
//***************************************************************************

#ifndef __GLOBE_NONCOPYABLE_H__
#define __GLOBE_NONCOPYABLE_H__
namespace glb {


//---------------------------------------------------------------------------
// NonCopyable
//---------------------------------------------------------------------------
class GLB_EXPORT NonCopyable
{
protected :
  NonCopyable (void) { }
  ~NonCopyable (void) { }

private :
  NonCopyable (const NonCopyable& rhs);                  // no implementation
  const NonCopyable& operator= (const NonCopyable& rhs); // no implementation
};


} // namespace glb
#endif // #ifndef __GLOBE_NONCOPYABLE_H__
