#pragma once
#include <cstddef>
#include <typeinfo>
#include <concepts>

#include "Core.hpp"

namespace Micro
{
	template <typename T>
	NODISCARD size_t Hash(const T& object) noexcept { return typeid(T).hash_code(); }

	NODISCARD constexpr size_t Hash(const std::integral auto& integral) noexcept { return integral; }
	NODISCARD constexpr size_t Hash(const std::floating_point auto& flt) noexcept { return flt; }
}
