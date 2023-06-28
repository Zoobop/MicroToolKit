#pragma once

#define VALUE(x)		x
#define NAMEOF(x)	    #x
#define EVAL(x)			NAMEOF(##x)
#define MIN(a, b)       (((a) < (b)) ? (a) : (b))
#define MAX(a, b)       (((a) > (b)) ? (a) : (b))

#if defined(_WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
// Windows Platform
#define DEBUG_BREAK()   __debugbreak()
#elif __APPLE__
// Apple Platform(s)
#define DEBUG_BREAK()
#elif __linux__
// Linux Platform
#define DEBUG_BREAK()
#elif __unix__
// Unix
#define DEBUG_BREAK()
#elif defined(_POSIX_VERSION)
// POSIX
#define DEBUG_BREAK()
#else
#error "Unknown compiler"
#endif

#define NODISCARD  [[nodiscard]]

#define I8_MIN    (-128)
#define I8_MAX    127
#define U8_MAX    0xff

#define I16_MIN    (-32768)
#define I16_MAX    32767
#define U16_MAX    0xffff

#define I32_MIN    (-2147483647 - 1)
#define I32_MAX    2147483647
#define U32_MAX    0xffffffff

#define I64_MAX    9223372036854775807i64
#define I64_MIN    (-9223372036854775807i64 - 1)
#define U64_MAX    0xffffffffffffffffui64

#define F32_MAX	   FLT_MAX
#define F32_MIN	   FLT_MIN

#define F64_MAX	   DBL_MAX
#define F64_MIN	   DBL_MIN
