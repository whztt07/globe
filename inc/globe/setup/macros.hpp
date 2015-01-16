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
// Created On: 2011-10-07 16:17:05
//
//***************************************************************************

#ifndef __GLOBE_MACROS_H__
#define __GLOBE_MACROS_H__


// GLB_EXPORT
#if defined(GLB_PLATFORM_WINDOWS)
# if defined(GLB_COMPILER_MSVC) || defined(GLB_COMPILER_GCC)
#   if defined(GLOBE_DLL_BUILD)
#     define GLB_EXPORT  __declspec(dllexport)
#   elif defined(GLOBE_DLL_CLIENT_BUILD)
#     define GLB_EXPORT  __declspec(dllimport)
#   else
#     define GLB_EXPORT
#   endif
# else
#   error Compiler not supported yet !
# endif
#else
# define GLB_EXPORT
#endif
//
#if defined(GLB_COMPILER_MSVC) && (defined(GLB_DLL_BUILD) || defined(GLB_DLL_CLIENT_BUILD))
# pragma warning(disable : 4251) // non dll-interface class used as base for dll-interface class
# pragma warning(disable : 4275) // exported class X derivate from a non exported class Y
#endif

// use this to make your code uncompilable until you remove it
#define GLB_TODO  const char TODO[-1] = "This is a TODO !"

// static assert macro (i.e.: compile-time assert)
#if defined(GLB_COMPILER_MSVC) && (GLB_COMPILER_VERSION >= 1300)
# define GLB_STATIC_ASSERT(test) \
    typedef char GLB_CAT(globe_static_assert_,__COUNTER__)[(test) * 2 - 1]
#else
# define GLB_STATIC_ASSERT(test) \
    typedef char GLB_CAT(globe_static_assert_,__LINE__)[(test) * 2 - 1]
#endif

// macros for assertion
#ifdef GLB_DEBUG
# if defined(GLB_COMPILER_MSVC) && defined(_ASSERTE)
#   define GLB_ASSERT _ASSERTE // the _ASSERTE macro is defined in <crtdbg.h>
# else
#   define GLB_ASSERT assert
# endif
# define GLB_FORCEASSERT GLB_ASSERT
#else
# define GLB_ASSERT
# define GLB_FORCEASSERT(exp) do{ if(!(exp)) { throw ::glb::Exception(__FILE__, __LINE__, ::glb::EXCODE_ASSERT, #exp); } }while(0)
#endif

// count of elements into a specified array
#define GLB_ARRAY_COUNT(arr)  (sizeof(arr) / sizeof(arr[0]))

// free macros
#define GLB_FREE(block)       do{ if((block)) { ::free(block); (block)=NULL; } }while(0)
#define GLB_DELETE(obj)       do{ if((obj))   { delete   (obj);    (obj)=NULL; } }while(0)
#define GLB_DELETEARRAY(arr)  do{ if((arr))   { delete[] (arr);    (arr)=NULL; } }while(0)
#define GLB_RELEASE(obj)      do{ if((obj))   { (obj)->Release();  (obj)=NULL; } }while(0)

// util macros
#define GLB_VERBATIM(x)   x
#define GLB_STRINGIZE(x)  GLB_STRINGIZE__SUB(x)
#define GLB_CAT(a,b)      GLB_CAT__SUB(a,b)

// util macros subs (do not use directly)
#define GLB_STRINGIZE__SUB(x)  #x
#define GLB_CAT__SUB(a,b)      a##b

// compiler - break debugger
#if defined(GLB_DEBUG)
# if defined(GLB_COMPILER_GCC)
#   define GLB_DEBUG_BREAK  __asm ("int $3");
# elif defined(GLB_COMPILER_MSVC)
#   define GLB_DEBUG_BREAK  { if (::IsDebuggerPresent()) ::DebugBreak(); }
# else
#   error Unsupported compiler !
# endif
#else
# define GLB_DEBUG_BREAK
#endif

// logging macros
#ifdef GLB_DEBUG
# define GLB_LOGDBG(msg, ...)  ::glb::Logger::print(::glb::Logger::LLDEBUG, __FILE__, __LINE__, msg, ## __VA_ARGS__)
#else
# define GLB_LOGDBG(msg, ...)
#endif
#define GLB_LOGINFO(msg, ...)   ::glb::Logger::print(::glb::Logger::LLINFO, __FILE__, __LINE__, msg, ## __VA_ARGS__)
#define GLB_LOGWARN(msg, ...)   ::glb::Logger::print(::glb::Logger::LLWARN, __FILE__, __LINE__, msg, ## __VA_ARGS__)
#define GLB_LOGERR(msg, ...)    ::glb::Logger::print(::glb::Logger::LLERROR, __FILE__, __LINE__, msg, ## __VA_ARGS__)
#define GLB_LOGFATAL(msg, ...)  ::glb::Logger::print(::glb::Logger::LLFATAL, __FILE__, __LINE__, msg, ## __VA_ARGS__)

// throw exceptions
#define GLB_THROW(excode, msg, ...)  throw ::glb::Exception(__FILE__, __LINE__, (uint)(excode), msg, ## __VA_ARGS__)
#define GLB_THROWEXCODE(excode)      throw ::glb::Exception(__FILE__, __LINE__, (uint)(excode), NULL)
#ifdef GLB_DEBUG
# define GLB_THROWSTR(str) throw (__FILE__ "(" GLB_STRINGIZE(__LINE__) "): " str)
#else
# define GLB_THROWSTR(str) throw (str)
#endif

// storage-class attributes
#if defined(GLB_COMPILER_GCC)
# define GLB_FORCEINLINE        __attribute__((always_inline))
# define GLB_ALIGN(x)           __attribute__((aligned(x)))
# define GLB_ALIGNED_STRUCT(x)  __attribute__((aligned(x))) struct
# define GLB_PACKED             __attribute__((packed))
# define GLB_PACKED_STRUCT      __attribute__((packed)) struct
# define GLB_DEPRECATED         __attribute__((__deprecated__))
# define GLB_FMTPRINTF(a,b)     __attribute__((format(printf,a,b)))
# define GLB_NORETURN           __attribute__((noreturn))
#elif defined(GLB_COMPILER_MSVC)
# define GLB_FORCEINLINE        __forceinline
# define GLB_ALIGN(x)           __declspec(align(x))
# define GLB_ALIGNED_STRUCT(x)  __declspec(align(x)) struct
# define GLB_PACKED             __declspec(align(1))
# define GLB_PACKED_STRUCT      __declspec(align(1)) struct
# define GLB_DEPRECATED         __declspec(deprecated)
# define GLB_FMTPRINTF(a,b)
# define GLB_NORETURN           __declspec(noreturn)
#else
# error Unsupported compiler !
#endif

// aliasing
#if defined(GLB_COMPILER_GCC)
# ifndef stricmp
#   define stricmp strcasecmp
# endif
#elif defined(GLB_COMPILER_MSVC)
# ifndef snprintf
#   define snprintf _snprintf
# endif
# ifndef vsnprintf
#   define vsnprintf _vsnprintf
# endif
#endif

// hack for <inttypes.h> replacement
//   TOFIX : make those macros *really* 64bit compliant at compile-time.
//   if sizeof(int)==sizeof(int64), those macros must reflect this.
#if defined(GLB_PLATFORM_WINDOWS)
# ifndef PRId64
#   define PRId64  "I64d"
# endif
# ifndef PRIi64
#   define PRIi64  "I64i"
# endif
# ifndef PRIu64
#   define PRIu64  "I64u"
# endif
#else
# ifndef PRId64
#   define PRId64  "lld"
# endif
# ifndef PRIi64
#   define PRIi64  "lli"
# endif
# ifndef PRIu64
#   define PRIu64  "llu"
# endif
#endif


#endif // #ifndef __GLOBE_MACROS_H__
