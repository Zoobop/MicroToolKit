#pragma once

#ifdef ENABLE_LOGGING
#define LOG(x)		std::cout << x << "\n"
#define PROMPT(x)	std::cout << x
#else
#define LOG(x)
#define PROMPT(x)
#endif