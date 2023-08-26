#pragma once
#include <chrono>

#include "IO/IOHandler.hpp"

namespace Micro
{
	struct Nanosecond final	{ constexpr static const char* TimeAbbreviation = "ns"; };

	struct Microsecond final { constexpr static const char* TimeAbbreviation = "us"; };

	struct Millisecond final { constexpr static const char* TimeAbbreviation = "ms"; };

	struct Second final { constexpr static const char* TimeAbbreviation = "s"; };

	struct Minute final { constexpr static const char* TimeAbbreviation = "min"; };

	struct Hour final { constexpr static const char* TimeAbbreviation = "hr"; };


	template <typename TFactor, typename TTimeUnit>
	class TimeHandle final
	{
	public:
		using Clock = std::chrono::high_resolution_clock;
		using Time = std::chrono::time_point<Clock>;

		// Constructors/Destructors
		constexpr TimeHandle() noexcept = delete;
		constexpr TimeHandle(const TimeHandle&) noexcept = delete;
		constexpr TimeHandle(TimeHandle&&) noexcept = delete;

		constexpr explicit TimeHandle(const char* timerName = "Timer") noexcept
			: m_TimerName(timerName), m_StartTime(Clock::now()),
			  m_TimeStamp(Clock::now())
		{
		}

		constexpr ~TimeHandle() noexcept { Stop(); }

		// Utility
		constexpr void Stamp(const char* stampName = nullptr) noexcept
		{
			const auto currentTime = Clock::now();
			const auto start = std::chrono::time_point_cast<TFactor>(m_TimeStamp).time_since_epoch().count();
			const auto end = std::chrono::time_point_cast<TFactor>(currentTime).time_since_epoch().count();
			const auto duration = end - start;
			m_TimeStamp = currentTime;

			if (stampName)
				IO::WriteLine("[{}] Time Stamp: {} {}\t({})", m_TimerName.Data(), duration, TTimeUnit::TimeAbbreviation, stampName);
			else
				IO::WriteLine("[{}] Time Stamp: {} {}", m_TimerName.Data(), duration, TTimeUnit::TimeAbbreviation);
		}

		constexpr void Stop() noexcept
		{
			const auto stopTime = Clock::now();
			const auto start = std::chrono::time_point_cast<TFactor>(m_StartTime).time_since_epoch().count();
			const auto end = std::chrono::time_point_cast<TFactor>(stopTime).time_since_epoch().count();
			const auto duration = (end - start);

			IO::WriteLine("Time: {} {}\t[{}]", duration, TTimeUnit::TimeAbbreviation, m_TimerName.Data());
		}

		// Operator Overloads
		constexpr TimeHandle& operator=(const TimeHandle&) noexcept = delete;
		constexpr TimeHandle& operator=(TimeHandle&&) noexcept = delete;

	private:
		string m_TimerName;
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

#define PROFILE()			::Micro::MicrosecondTimer timer(__FUNCTION__)
#define BENCHMARK()			::Micro::MillisecondTimer timer(__FUNCTION__)
