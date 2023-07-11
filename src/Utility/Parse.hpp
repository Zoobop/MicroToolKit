#pragma once
#include "Common/String.hpp"

namespace Micro
{
	template <typename T>
	concept Parsable = requires(T object, const String& string)
	{
		{ T::Parse(string) } -> std::convertible_to<T>;
		{ T::TryParse(string, object) } -> std::convertible_to<bool>;
	};


	template <Parsable T>
	NODISCARD T Parse(const String& string) noexcept { return T::Parse(string); }

	template <Parsable T>
	NODISCARD bool TryParse(const String& string, T& object) noexcept { return T::TryParse(string, object); }
}
