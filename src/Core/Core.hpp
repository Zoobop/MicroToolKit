#pragma once

#define VALUE(x)		x
#define NAMEOF(x)	    #x
#define EVAL(x)			NAMEOF(##x)
#define MIN(a, b)       (((a) < (b)) ? (a) : (b))
#define MAX(a, b)       (((a) > (b)) ? (a) : (b))

#if defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
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

#define NODISCARD	[[nodiscard]]
#define NORETURN	[[noreturn]]