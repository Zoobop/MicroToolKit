#pragma once

#ifdef ENABLE_PROFILING
#include "Core/Core.h"

#include <iostream>
#include <chrono>
#include <thread>

namespace mtk {

	struct nano { static const char* _abbr; }; const char* nano::_abbr = "ns";
	struct micro { static const char* _abbr; }; const char* micro::_abbr = "us";
	struct milli { static const char* _abbr; }; const char* milli::_abbr = "ms";
	struct second { static const char* _abbr; }; const char* second::_abbr = "s";
	struct minute { static const char* _abbr; }; const char* minute::_abbr = "min";
	struct hour { static const char* _abbr; }; const char* hour::_abbr = "hr";

	template<uint64_t _factor, typename _Abbr>
	class TimeHandle
	{
	public:
		using TIME = std::chrono::time_point<std::chrono::high_resolution_clock>;

	public:
		TimeHandle(const char* _timerName = "")
			: m_TimerName(_timerName)
		{
			m_StartTime = m_TimeStamp = std::chrono::high_resolution_clock::now();
		}

		void Stamp(const char* _stampName = "")
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_TimeStamp).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(currentTime).time_since_epoch().count();
			auto duration = end - start;
			m_TimeStamp = currentTime;

			if (_stampName != "")
				std::cout << "Time Stamp: " << duration << " " << _Abbr::_abbr << "\t(" << _stampName << ")\n";
			else
				std::cout << "Time Stamp: " << duration << " " << _Abbr::_abbr << "\n";
		}

		~TimeHandle()
		{
			Stop();
		}

	private:
		void Stop()
		{
			auto stopTime = std::chrono::high_resolution_clock::now();
			auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTime).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(stopTime).time_since_epoch().count();
			float duration = (end - start) * (1.0f / _factor);

			if (m_TimerName != "")
				std::cout << "Time: " << duration << " " << _Abbr::_abbr << "\t(" << m_TimerName << ")\n";
			else
				std::cout << "Time: " << duration << " " << _Abbr::_abbr << "\n";
		}

	private:
		const char* m_TimerName;
		TIME m_StartTime;
		TIME m_TimeStamp;
	};

	typedef mtk::TimeHandle<1, nano>				NanosecondTimer;
	typedef mtk::TimeHandle<1000, micro>			MicrosecondTimer;
	typedef mtk::TimeHandle<1000000, milli>			MillisecondTimer;
	typedef mtk::TimeHandle<1000000000, second>		SecondTimer;
	typedef mtk::TimeHandle<6000000000, minute>		MinuteTimer;
	typedef mtk::TimeHandle<36000000000, hour>		HourTimer;
}

#define PROFILER()			::mtk::MicrosecondTimer timer(__FUNCTION__)
#define BENCHMARK()			::mtk::MillisecondTimer timer(__FUNCTION__)
#define EASYTIMER()			::mtk::SecondTimer timer(__FUNCTION__)

#define TEST_PROFILER(name)				::mtk::MicrosecondTimer timer(name)
#define TEST_BENCHMARK(name)			::mtk::MillisecondTimer timer(name)
#define TEST_EASYTIMER(name)			::mtk::SecondTimer timer(name)
#else
#define PROFILER()
#define BENCHMARK()
#define EASYTIMER()
#endif