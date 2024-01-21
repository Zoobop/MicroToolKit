#pragma once
#include <typeinfo>
#include <concepts>

#include "Core.hpp"
#include "Typedef.hpp"

namespace Micro
{
	template <typename T>
	NODISCARD usize Hash(const T& object) noexcept { return typeid(T).hash_code(); }

	NODISCARD constexpr usize Hash(const std::integral auto& integral) noexcept { return integral; }
	NODISCARD constexpr usize Hash(const std::floating_point auto& flt) noexcept { return flt; }
}
