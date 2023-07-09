#pragma once

#include <iostream>
#include <chrono>

namespace Micro
{
	struct Nanosecond
	{
		inline static const char* TimeAbbreviation = "ns";
	};

	struct Microsecond
	{
		inline static const char* TimeAbbreviation = "us";
	};

	struct Millisecond
	{
		inline static const char* TimeAbbreviation = "ms";
	};

	struct Second
	{
		inline static const char* TimeAbbreviation = "s";
	};

	struct Minute
	{
		inline static const char* TimeAbbreviation = "min";
	};

	struct Hour
	{
		inline static const char* TimeAbbreviation = "hr";
	};


	template <typename TFactor, typename TTimeUnit>
	class TimeHandle final
	{
	public:
		using Clock = std::chrono::high_resolution_clock;
		using Time = std::chrono::time_point<Clock>;

		// Constructors/Destructors
		TimeHandle() = delete;
		TimeHandle(const TimeHandle&) = delete;
		TimeHandle(TimeHandle&&) = delete;

		TimeHandle(const char* timerName = "Time")
			: m_TimerName(timerName), m_StartTime(Clock::now()),
			  m_TimeStamp(Clock::now())
		{
		}

		~TimeHandle() { Stop(); }

		// Utility
		void Stamp(const char* stampName = "")
		{
			const auto currentTime = Clock::now();
			const auto start = std::chrono::time_point_cast<TFactor>(m_TimeStamp).time_since_epoch().count();
			const auto end = std::chrono::time_point_cast<TFactor>(currentTime).time_since_epoch().count();
			const auto duration = end - start;
			m_TimeStamp = currentTime;

			if (stampName != "")
				std::cout << "Time Stamp: " << duration << " " << TTimeUnit::TimeAbbreviation << "\t(" << stampName <<
					")\n";
			else
				std::cout << "Time Stamp: " << duration << " " << TTimeUnit::TimeAbbreviation << "\n";
		}

		void Stop()
		{
			const auto stopTime = Clock::now();
			const auto start = std::chrono::time_point_cast<TFactor>(m_StartTime).time_since_epoch().count();
			const auto end = std::chrono::time_point_cast<TFactor>(stopTime).time_since_epoch().count();
			const auto duration = (end - start);

			if (m_TimerName != "")
				std::cout << "Time: " << duration << " " << TTimeUnit::TimeAbbreviation << "\t(" << m_TimerName <<
					")\n";
			else
				std::cout << "Time: " << duration << " " << TTimeUnit::TimeAbbreviation << "\n";
		}

		// Operator Overloads
		TimeHandle& operator=(const TimeHandle&) = delete;
		TimeHandle& operator=(TimeHandle&&) noexcept = delete;

	private:
		const char* m_TimerName;
		Time m_StartTime;
		Time m_TimeStamp;
	};

	using NanosecondTimer = TimeHandle<std::chrono::nanoseconds, Nanosecond>;
	using MicrosecondTimer = TimeHandle<std::chrono::microseconds, Microsecond>;
	using MillisecondTimer = TimeHandle<std::chrono::milliseconds, Millisecond>;
	using SecondTimer = TimeHandle<std::chrono::seconds, Second>;
	using MinuteTimer = TimeHandle<std::chrono::minutes, Minute>;
	using HourTimer = TimeHandle<std::chrono::hours, Hour>;
}

#define PROFILER()			::Micro::MicrosecondTimer timer(__FUNCTION__)
#define BENCHMARK()			::Micro::MillisecondTimer timer(__FUNCTION__)
#define EASYTIMER()			::Micro::SecondTimer timer(__FUNCTION__)

#define TEST_PROFILER(name)				::Micro::MicrosecondTimer timer(name)
#define TEST_BENCHMARK(name)			::Micro::MillisecondTimer timer(name)
#define TEST_EASYTIMER(name)			::Micro::SecondTimer timer(name)
