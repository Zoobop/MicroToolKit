#include "mdtpch.h"
#include "Benchmark.h"

using namespace std::literals::chrono_literals;

namespace mdt {

	const char* Benchmark::s_TimerName;
	TIME Benchmark::s_Timestamp;
	TIME Benchmark::s_StartTime;
	TIME Benchmark::s_StopTime;
	bool Benchmark::s_Running = false;

	void Benchmark::Start(const char* _timerName)
	{
		if (!s_Running) {
			s_TimerName = _timerName;
			s_Timestamp = s_StartTime = std::chrono::high_resolution_clock::now();
			s_Running = true;
		}
	}

	void Benchmark::Stamp(const char* _stampName)
	{
		if (s_Running) {
			auto currentTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> duration = currentTime - s_Timestamp;
			if (_stampName != "")
				std::cout << "Time Stamp: " << duration.count() * 1000.0f << "ms\t" << "(" << _stampName << ")" << "\n";
			else
				std::cout << "Time Stamp: " << duration.count() * 1000.0f << "ms\n";
		}
	}

	void Benchmark::Stop()
	{
		if (s_Running) {
			s_StopTime = std::chrono::high_resolution_clock::now();
			s_Running = false;
			std::chrono::duration<float> duration = s_StopTime - s_StartTime;
			if (s_TimerName != "")
				std::cout << "Time: " << duration.count() * 1000.0f << "ms\t" << "(" << s_TimerName << ")" << "\n";
			else
				std::cout << "Time: " << duration.count() * 1000.0f << "ms\n";
		}
	}

}