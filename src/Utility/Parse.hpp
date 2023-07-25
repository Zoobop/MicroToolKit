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
	NODISCARD constexpr int8_t Parse(const StringBuffer& string) { return string.IsEmpty() ? '\0' : string[0]; }

	template <>
	NODISCARD inline int16_t Parse(const StringBuffer& string)
	{
		return (int16_t)std::stoi({ string.Data(), string.Length() });
	}

	template <>
	NODISCARD inline int32_t Parse(const StringBuffer& string)
	{
		return std::stoi({ string.Data(), string.Length() });
	}

	template <>
	NODISCARD inline int64_t Parse(const StringBuffer& string)
	{
		return std::stoll({ string.Data(), string.Length() });
	}

	template <>
	NODISCARD inline float_t Parse(const StringBuffer& string)
	{
		return std::stof({ string.Data(), string.Length() });
	}

	template <>
	NODISCARD inline double_t Parse(const StringBuffer& string)
	{
		return std::stod({ string.Data(), string.Length() });
	}
}
