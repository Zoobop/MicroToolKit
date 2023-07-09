#pragma once
#include "Core.hpp"

namespace Micro
{
	template <typename T>
	NODISCARD size_t Hash(const T& object) noexcept { return typeid(T).hash_code(); }

	template <>
	NODISCARD inline size_t Hash(const int& object) noexcept { return object; }
}
