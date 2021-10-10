#pragma once
#include <random>

namespace mdt {

	class Random
	{
	public:
		static double Rand(double _start, double _stop, bool _inclusive = true)
		{
			double _incluCount = (!_inclusive) ? 1.0 : 0.0;
			return _start + static_cast<double>(std::rand()) / (static_cast<double>(RAND_MAX / ((_stop + _incluCount) - _start)));
		}

		static float Rand(float _start, float _stop, bool _inclusive = true)
		{
			float _incluCount = (!_inclusive) ? 1.0f : 0.0f;
			return _start + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / ((_stop + _incluCount) - _start)));
		}

		static int8_t RandInt(int8_t _start, int8_t _stop, bool _inclusive = true)
		{
			int8_t _incluCount = (!_inclusive) ? (int8_t)1 : 0;
			return _start + static_cast<int8_t>(std::rand()) / (static_cast<int8_t>(RAND_MAX / ((_stop + _incluCount) - _start)));
		}

		static int16_t RandInt(int16_t _start, int16_t _stop, bool _inclusive = true)
		{
			int16_t _incluCount = (!_inclusive) ? (int16_t)1 : 0;
			return _start + static_cast<int16_t>(std::rand()) / (static_cast<int16_t>(RAND_MAX / ((_stop + _incluCount) - _start)));
		}

		static int32_t RandInt(int32_t _start, int32_t _stop, bool _inclusive = true)
		{
			int32_t _incluCount = (!_inclusive) ? (int32_t)1 : 0;
			return _start + static_cast<int32_t>(std::rand()) / (static_cast<int32_t>(RAND_MAX / ((_stop + _incluCount) - _start)));
		}

		static int64_t RandInt(int64_t _start, int64_t _stop, bool _inclusive = true)
		{
			int64_t _incluCount = (!_inclusive) ? (int64_t)1 : 0;
			return _start + static_cast<int64_t>(std::rand()) / (static_cast<int64_t>(RAND_MAX / ((_stop + _incluCount) - _start)));
		}

		static uint8_t RandInt(uint8_t _stop, bool _inclusive = true)
		{
			uint8_t _incluCount = (!_inclusive) ? (uint8_t)1 : 0;
			return static_cast<uint8_t>(std::rand()) / (static_cast<uint8_t>(RAND_MAX / (_stop + _incluCount)));
		}

		static uint16_t RandInt(uint16_t _stop, bool _inclusive = true)
		{
			uint16_t _incluCount = (!_inclusive) ? (uint16_t)1 : 0;
			return static_cast<uint16_t>(std::rand()) / (static_cast<uint16_t>(RAND_MAX / (_stop + _incluCount)));
		}

		static uint32_t RandInt(uint32_t _stop, bool _inclusive = true)
		{
			uint32_t _incluCount = (!_inclusive) ? (uint32_t)1 : 0;
			return static_cast<uint32_t>(std::rand()) / (static_cast<uint32_t>(RAND_MAX / (_stop + _incluCount)));
		}

		static uint64_t RandInt(uint64_t _stop, bool _inclusive = true)
		{
			uint64_t _incluCount = (!_inclusive) ? (uint64_t)1 : 0;
			return static_cast<uint64_t>(std::rand()) / (static_cast<uint64_t>(RAND_MAX / (_stop + _incluCount)));
		}

	private:
		Random() = delete;
	};

}