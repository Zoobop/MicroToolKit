#pragma once
#include "Core.hpp"

namespace mtk
{
	template <typename T>
	NODISCARD size_t Hash(const T& object) { return typeid(T).hash_code(); }
}
