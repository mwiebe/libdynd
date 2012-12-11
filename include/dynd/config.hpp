//
// Copyright (C) 2011-12, Dynamic NDArray Developers
// BSD 2-Clause License, see LICENSE.txt
//

#ifndef _DYND__CONFIG_HPP_
#define _DYND__CONFIG_HPP_

#ifdef __clang__

//#  define DYND_RVALUE_REFS
//#  define DYND_INIT_LIST

// TODO: versions with constexpr
#  define DYND_CONSTEXPR constexpr

# define DYND_USE_STDINT

#elif defined(__GNUC__)

# define DYND_USE_STDINT

#if __GNUC__ > 4 || \
              (__GNUC__ == 4 && (__GNUC_MINOR__ >= 7))
// Use initializer lists on gcc >= 4.7
#  define DYND_INIT_LIST
// Use constexpr on gcc >= 4.7
#  define DYND_CONSTEXPR constexpr
// Use rvalue references on gcc >= 4.7
#  define DYND_RVALUE_REFS
#else
// Don't use constexpr on gcc < 4.7
#  define DYND_CONSTEXPR
#endif

#elif defined(_MSC_VER)

// If set, uses the FP status registers.
// On some compilers, there is no proper
// way to tell the compiler that these are
// important, and it reorders instructions
// so as to make them useless. On MSVC, there
// is #pragma fenv_access(on), which works.
#define DYND_USE_FPSTATUS

# if _MSC_VER >= 1600
// Use enable_if from std::tr1
#  define DYND_USE_TR1_ENABLE_IF
// Use rvalue refs
#  define DYND_RVALUE_REFS
#  define DYND_USE_STDINT
# else
#  define DYND_USE_BOOST_SHARED_PTR
# endif

// No DYND_CONSTEXPR yet, define it as nothing
#  define DYND_CONSTEXPR

#endif

// If RValue References are supported
#ifdef DYND_RVALUE_REFS
#  define DYND_MOVE(x) (std::move(x))
#else
#  define DYND_MOVE(x) (x)
#endif

// If Initializer Lists are supported
#ifdef DYND_INIT_LIST
#include <initializer_list>
#endif

// If being run from the CLING C++ interpreter
#ifdef DYND_CLING
// Don't use the memcpy function (it has inline assembly).

inline void DYND_MEMCPY(char *dst, const char *src, intptr_t count)
{
    char *cdst = (char *)dst;
    const char *csrc = (const char *)src;
    while (count--) {
        *cdst++ = *csrc++;
    }
}
#else
#include <cstring>
#define DYND_MEMCPY(dst, src, count) std::memcpy(dst, src, count)
#endif

#ifdef DYND_USE_TR1_ENABLE_IF
#include <type_traits>
namespace dynd {
    using std::tr1::enable_if;
}
#else
// enable_if is a small template, so we just replicate it here
namespace dynd {
	template<bool B, class T = void>
	struct enable_if {};
 
	template<class T>
	struct enable_if<true, T> { typedef T type; };
}
#endif

/////////////////////////////////////////
// Diagnostic configurations
//

/**
 * This preprocessor symbol enables or disables assertions
 * that pointers are aligned as they are supposed to be. This helps
 * test that alignment is being done correctly on platforms which
 * do not segfault on misaligned data.
 *
 * An exception is thrown if an improper unalignment is detected.
 *
 * See diagnostics.hpp for the macros which use this.
 */
#ifndef DYND_ALIGNMENT_ASSERTIONS
# define DYND_ALIGNMENT_ASSERTIONS 0
#endif

/**
 * This preprocessor symbol enables or disables tracing
 * of individual builtin dtype assignment operations.
 *
 * See diagnostics.hpp for the macros which use this.
 */
#ifndef DYND_ASSIGNMENT_TRACING
# define DYND_ASSIGNMENT_TRACING 0
#endif


/**
 * Preprocessor macro for marking variables unused, and suppressing
 * warnings for them.
 */
#define DYND_UNUSED(x)

#ifdef DYND_USE_STDINT
#include <stdint.h>
#else
typedef signed char      int8_t;
typedef short            int16_t;
typedef int              int32_t;
typedef __int64          int64_t;
typedef ptrdiff_t        intptr_t;
typedef unsigned char    uint8_t;
typedef unsigned short   uint16_t;
typedef unsigned int     uint32_t;
typedef unsigned __int64 uint64_t;
typedef size_t           uintptr_t;
#endif

#endif // _DYND__CONFIG_HPP_
