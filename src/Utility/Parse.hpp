#pragma once
#include "Common/String.hpp"

namespace Micro
{
	//template <typename T>
	//concept Parsable = requires(T object, const CharSequence auto& string)
	//{
	//	{ T::Parse(string) } -> std::convertible_to<T>;
	//	{ T::TryParse(string, object) } -> std::convertible_to<bool>;
	//};


	//template <Parsable T>
	//NODISCARD T Parse(const CharSequence auto& string) noexcept { return T::Parse(string); }

	//template <Parsable T>
	//NODISCARD bool TryParse(const CharSequence auto& string, T& object) noexcept { return T::TryParse(string, object); }

	template <typename  T>
	NODISCARD T Parse(const StringBuffer& string) noexcept { return {}; }

	template <>
	NODISCARD inline bool Parse(const StringBuffer& string) noexcept
	{
		return string.Equals("true");
	}

	template <>
	NODISCARD inline char Parse(const StringBuffer& string) noexcept
	{
		return string[0];
	}

	template <>
	NODISCARD inline int8_t Parse(const StringBuffer& string) noexcept
	{
		return string[0];
	}

	template <>
	NODISCARD inline int16_t Parse(const StringBuffer& string) noexcept
	{
		return (int16_t)std::stoi({ string.Data(), string.Length() });
	}

	template <>
	NODISCARD inline int32_t Parse(const StringBuffer& string) noexcept
	{
		return std::stoi({ string.Data(), string.Length() });
	}

	template <>
	NODISCARD inline int64_t Parse(const StringBuffer& string) noexcept
	{
		return std::stoll({ string.Data(), string.Length() });
	}

	template <>
	NODISCARD inline float_t Parse(const StringBuffer& string) noexcept
	{
		return std::stof({ string.Data(), string.Length() });
	}

	template <>
	NODISCARD inline double_t Parse(const StringBuffer& string) noexcept
	{
		return std::stod({ string.Data(), string.Length() });
	}
}
