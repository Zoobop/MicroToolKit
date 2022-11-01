#pragma once
#include <random>
#include "DebugUtils/Timer.h"

namespace mtk {

	class Random
	{
	public:
		static double Rand(double _start, double _stop, bool _inclusive = true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			double incluCount = (!_inclusive) ? 1.0 : 0.0;
			PROFILER();
			std::uniform_real_distribution<double> dist(_start, _stop - incluCount);
			return dist(gen);
		}

		static float Rand(float _start, float _stop, bool _inclusive = true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			float incluCount = (!_inclusive) ? 1.0f : 0.0f;
			PROFILER();
			std::uniform_real_distribution<float> dist(_start, _stop - incluCount);
			return dist(gen);
		}

		static int8_t RandInt(int8_t _start, int8_t _stop, bool _inclusive = true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			int8_t incluCount = (!_inclusive) ? 1 : 0;
			PROFILER();
			std::uniform_int<int8_t> dist(_start, _stop - incluCount);
			return dist(gen);
		}

		static int16_t RandInt(int16_t _start, int16_t _stop, bool _inclusive = true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			int16_t incluCount = (!_inclusive) ? 1 : 0;
			PROFILER();
			std::uniform_int_distribution<int16_t> dist(_start, _stop - incluCount);
			return dist(gen);
		}

		static int32_t RandInt(int32_t _start, int32_t _stop, bool _inclusive = true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			int32_t incluCount = (!_inclusive) ? 1 : 0;
			PROFILER();
			std::uniform_int_distribution<int32_t> dist(_start, _stop - incluCount);
			return dist(gen);
		}

		static int64_t RandInt(int64_t _start, int64_t _stop, bool _inclusive = true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			int64_t incluCount = (!_inclusive) ? 1 : 0;
			PROFILER();
			std::uniform_int_distribution<int64_t> dist(_start, _stop - incluCount);
			return dist(gen);
		}

		static int8_t RandInt(int8_t _stop, bool _inclusive = true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			int8_t incluCount = (!_inclusive) ? 1 : 0;
			PROFILER();
			std::uniform_int<int8_t> dist(0, _stop - incluCount);
			return dist(gen);
		}

		static int16_t RandInt(int16_t _stop, bool _inclusive = true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			int16_t incluCount = (!_inclusive) ? 1 : 0;
			PROFILER();
			std::uniform_int_distribution<int16_t> dist(0, _stop - incluCount);
			return dist(gen);
		}

		static int32_t RandInt(int32_t _stop, bool _inclusive = true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			int32_t incluCount = (!_inclusive) ? 1 : 0;
			PROFILER();
			std::uniform_int_distribution<int32_t> dist(0, _stop - incluCount);
			return dist(gen);
		}

		static int64_t RandInt(int64_t _stop, bool _inclusive = true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			int64_t incluCount = (!_inclusive) ? 1 : 0;
			PROFILER();
			std::uniform_int_distribution<int64_t> dist(0, _stop - incluCount);
			return dist(gen);
		}

		static uint8_t RandInt(uint8_t _stop, bool _inclusive = true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			uint8_t incluCount = (!_inclusive) ? 1 : 0;
			PROFILER();
			std::uniform_int<uint8_t> dist(0, _stop - incluCount);
			return dist(gen);
		}

		static uint16_t RandInt(uint16_t _stop, bool _inclusive = true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			uint16_t incluCount = (!_inclusive) ? 1 : 0;
			PROFILER();
			std::uniform_int_distribution<uint16_t> dist(0, _stop - incluCount);
			return dist(gen);
		}

		static uint32_t RandInt(uint32_t _stop, bool _inclusive = true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			uint32_t incluCount = (!_inclusive) ? 1 : 0;
			PROFILER();
			std::uniform_int_distribution<uint32_t> dist(0, _stop - incluCount);
			return dist(gen);
		}

		static uint64_t RandInt(uint64_t _stop, bool _inclusive = true)
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			uint64_t incluCount = (!_inclusive) ? 1 : 0;
			PROFILER();
			std::uniform_int_distribution<uint64_t> dist(0, _stop - incluCount);
			return dist(gen);
		}

	private:
		Random() = delete;
	};

}