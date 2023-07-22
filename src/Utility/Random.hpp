#pragma once
#include <random>

#include "Core/Core.hpp"

namespace Micro
{
	class Random final
	{
	public:
		Random() = delete;

		NODISCARD static auto Rand(const std::floating_point auto start, const std::floating_point auto stop,
		                           const bool inclusive = true) noexcept
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			std::uniform_real_distribution dist(start, stop - inclusive);
			return dist(gen);
		}

		NODISCARD static auto RandInt(const std::signed_integral auto start, const std::signed_integral auto stop,
		                              const bool inclusive = true) noexcept
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			std::uniform_int_distribution dist(start, stop - inclusive);
			return dist(gen);
		}

		NODISCARD static auto RandInt(const std::unsigned_integral auto start, const std::unsigned_integral auto stop,
		                              const bool inclusive = true) noexcept
		{
			std::random_device rd;
			std::mt19937 gen(rd());

			std::uniform_int_distribution dist(start, stop - inclusive);
			return dist(gen);
		}
	};
}