#pragma once

#ifdef ENABLE_LOGGING
#include <iostream>

namespace mtk {
	
	class Logger
	{
	public:
		template<typename _Cont>
		static void Log(const _Cont& _context, const char* _end = "")
		{
			std::cout << _context << _end;
		}

		template<typename ... _Args>
		static void LogFormat(const char* _context, _Args ... _args, const char* _end = "")
		{
			printf(_context, std::forward<_Args>(std::move(_args))...);
			printf(_end);
		}

	private:
		Logger() = delete;
	};

}

#define LOG(context)			::mtk::Logger::Log(context, "\n")
#define PROMPT(context)			::mtk::Logger::Log(context)
#define LOGF(context, ...)		printf(context, __VA_ARGS__); printf("\n")
#define PROMPTF(context, ...)	printf(context, __VA_ARGS__)

#else

#define LOG(x)
#define PROMPT(x)
#define LOGF(x)
#define PROMPTF(x)

#endif