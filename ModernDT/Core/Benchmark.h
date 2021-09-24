#pragma once

#include <iostream>
#include <chrono>
#include <thread>

#ifdef _DEBUG
#define LOG(x)		std::cout << x << "\n";
#else
#define LOG(x)
#endif

using TIME = std::chrono::steady_clock::time_point;

namespace mdt {

	class Benchmark
	{
	public:
		static void Start(const char* _timerName = "");
		static void Stop();

	private:
		static const char* s_TimerName;
		static TIME s_Timestep;
		static TIME s_StartTime;
		static TIME s_StopTime;
		static bool s_Running;
	};
}