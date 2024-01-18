#pragma once
#include <string>
#include <Common/Guid.hpp>
#include <Common/StringBuffer.hpp>

namespace Micro
{
	template <typename  T>
	NODISCARD T Parse(const StringBuffer& string) { return {}; }

	template <>
	NODISCARD constexpr bool Parse(const StringBuffer& string) { return string.Equals("true") || string.Equals('1'); }

	template <>
	NODISCARD constexpr char Parse(const StringBuffer& string) { return string.IsEmpty() ? '\0' : string.operator[](0).Value(); }

	template <>
	NODISCARD constexpr i8 Parse(const StringBuffer& string) { return string.IsEmpty() ? '\0' : string.operator[](0).Value(); }

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
	NODISCARD constexpr u8 Parse(const StringBuffer& string) { return string.IsEmpty() ? '\0' : static_cast<u8>(string.operator[](0).Value()); }

	template <>
	NODISCARD inline u16 Parse(const StringBuffer& string)
	{
		return static_cast<u16>(std::stoul({ string.Data(), string.Length() }));
	}

	template <>
	NODISCARD inline u32 Parse(const StringBuffer& string)
	{
		return std::stoul({ string.Data(), string.Length() });
	}

	template <>
	NODISCARD inline u64 Parse(const StringBuffer& string)
	{
		return std::stoull({ string.Data(), string.Length() });
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

	template <>
	NODISCARD inline Guid Parse(const StringBuffer& string)
	{
		constexpr usize length = 36;
		const StringBuffer guid = string.Slice(0, length);

		char data[length + 1]{ 0 };
		for (usize i = 0; i < length; i++)
		{
			data[i] = guid[i].Value();
		}

		return Guid{ data };
	}
}
