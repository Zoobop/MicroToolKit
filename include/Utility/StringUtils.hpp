#pragma once
#include <ranges>

#include "Collections/List.hpp"
#include "Common/StringBuffer.hpp"
#include "Common/StringBuilder.hpp"

namespace Micro
{
	NODISCARD constexpr bool IsNullOrEmpty(const CharSequence auto& string) noexcept
	{
		return string.IsEmpty();
	}

	NODISCARD constexpr bool IsNullOrWhitespace(const CharSequence auto& string) noexcept
	{
		if (string.IsEmpty())
			return true;

		constexpr char whitespace[] = "\t\r\b\n \0";

		for (usize i = 0; i < string.Length(); i++)
		{
			const char character = string[i];
			if (!std::ranges::any_of(whitespace, [character](const char c) { return c == character; }))
				return false;
		}

		return true;
	}

	NODISCARD constexpr String Concat(const CharSequence auto& ... charSequences) noexcept
	{
		constexpr usize paramSizes = sizeof ...(charSequences);
		static_assert(paramSizes > 0);

		constexpr usize defaultSize = 32 * paramSizes;
		StringBuilder builder(defaultSize);

		auto params = {std::forward<decltype(charSequences)>(charSequences)...};
		for (const auto& e : params)
			builder.Append(e);

		return builder.ToString();
	}

	template <StringConvertible T>
	NODISCARD constexpr String Join(const char character, const Span<T>& sequence) noexcept
	{
		constexpr usize defaultSize = 64;

		StringBuilder builder(defaultSize);

		for (const auto& e : sequence)
			builder.Append(e.ToString()).Append(character);

		return builder.ToString();
	}

	template <StringConvertible T, usize TSize>
	NODISCARD constexpr String Join(const char (&string)[TSize], const Span<T>& sequence) noexcept
	{
		constexpr usize defaultSize = 64;

		StringBuilder builder(defaultSize);

		for (const auto& e : sequence)
			builder.Append(e.ToString()).Append(string);

		return builder.ToString();
	}

	template <StringConvertible T>
	NODISCARD constexpr String Join(const CharSequence auto& string, const Span<T>& sequence) noexcept
	{
		constexpr usize defaultSize = 64;

		StringBuilder builder(defaultSize);

		for (const auto& e : sequence)
			builder.Append(e.ToString()).Append(string);

		return builder.ToString();
	}

	NODISCARD constexpr usize Count(const CharSequence auto& string, const char character) noexcept
	{
		usize count = 0;
		for (usize i = 0; i < string.Length(); ++i)
			if (string[i] == character)
				count++;

		return count;
	}

	NODISCARD constexpr usize Count(const CharSequence auto& string, const Predicate<char>& predicate) noexcept
	{
		usize count = 0;
		for (usize i = 0; i < string.Length(); ++i)
			if (predicate(string[i]))
				count++;

		return count;
	}

	NODISCARD constexpr List<String> Split(const CharSequence auto& string, const char delimiter = ' ') noexcept
	{
		const usize length = string.Length();
		List<String> list(length);

		StringBuffer buffer(string);
		usize headIndex = 0;
		for (usize i = 0; i < length; i++)
		{
			if (string[i] != delimiter)
				continue;

			const usize stringLength = i - headIndex;
			list.Emplace(buffer.Slice(0, stringLength).Value().ToString());

			buffer = buffer.Slice(stringLength + 1);
			headIndex = i + 1;
		}

		list.Emplace(buffer.ToString());
		return list;
	}
}
