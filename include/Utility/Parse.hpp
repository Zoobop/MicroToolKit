#pragma once
#include "Common/StringBuffer.hpp"

namespace Micro
{
	template <typename  T>
	NODISCARD T Parse(const StringBuffer& string) { return {}; }

	template <>
	NODISCARD constexpr bool Parse(const StringBuffer& string) { return string.Equals("true"); }

	template <>
	NODISCARD constexpr char Parse(const StringBuffer& string) { return string.IsEmpty() ? '\0' : string[0]; }

	template <>
	NODISCARD constexpr i8 Parse(const StringBuffer& string) { return string.IsEmpty() ? '\0' : string[0]; }

	template <>
	NODISCARD inline i16 Parse(const StringBuffer& string)
	{
		return static_cast<i16>(std::stoi({string.Data(), string.Length()}));
	}

	template <>
	NODISCARD inline i32 Parse(const StringBuffer& string)
	{
		return std::stoi({ string.Data(), string.Length() });
	}

	template <>
	NODISCARD inline i64 Parse(const StringBuffer& string)
	{
		return std::stoll({ string.Data(), string.Length() });
	}

	template <>
	NODISCARD inline f32 Parse(const StringBuffer& string)
	{
		return std::stof({ string.Data(), string.Length() });
	}

	template <>
	NODISCARD inline f64 Parse(const StringBuffer& string)
	{
		return std::stod({ string.Data(), string.Length() });
	}
}
