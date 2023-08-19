#pragma once
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

		for (size_t i = 0; i < string.Length(); i++)
		{
			const char character = string[i];
			if (character != '\n' && character != '\t' && character != '\r' && character != '\b' && character != ' ')
				return false;
		}

		return true;
	}

	NODISCARD constexpr String Concat(const CharSequence auto& ... charSequences) noexcept
	{
		constexpr size_t paramSizes = sizeof ...(charSequences);
		static_assert(paramSizes > 0);

		constexpr size_t defaultSize = 32 * paramSizes;
		StringBuilder builder(defaultSize);

		auto params = {std::forward<decltype(charSequences)>(charSequences)...};
		for (const auto& e : params)
			builder.Append(e);

		return builder.ToString();
	}

	template <StringConvertible T>
	NODISCARD constexpr String Join(const char character, const Span<T>& sequence) noexcept
	{
		constexpr size_t defaultSize = 64;

		StringBuilder builder(defaultSize);

		for (const auto& e : sequence)
			builder.Append(e.ToString()).Append(character);

		return builder.ToString();
	}

	template <StringConvertible T, size_t TSize>
	NODISCARD constexpr String Join(const char (&string)[TSize], const Span<T>& sequence) noexcept
	{
		constexpr size_t defaultSize = 64;

		StringBuilder builder(defaultSize);

		for (const auto& e : sequence)
			builder.Append(e.ToString()).Append(string);

		return builder.ToString();
	}

	template <StringConvertible T>
	NODISCARD constexpr String Join(const CharSequence auto& string, const Span<T>& sequence) noexcept
	{
		constexpr size_t defaultSize = 64;

		StringBuilder builder(defaultSize);

		for (const auto& e : sequence)
			builder.Append(e.ToString()).Append(string);

		return builder.ToString();
	}

	NODISCARD constexpr size_t Count(const CharSequence auto& string, const char character) noexcept
	{
		size_t count = 0;
		for (size_t i = 0; i < string.Length(); ++i)
			if (string[i] == character)
				count++;

		return count;
	}

	NODISCARD constexpr size_t Count(const CharSequence auto& string, const Predicate<char>& predicate) noexcept
	{
		size_t count = 0;
		for (size_t i = 0; i < string.Length(); ++i)
			if (predicate(string[i]))
				count++;

		return count;
	}

	NODISCARD constexpr List<String> Split(const CharSequence auto& string, const char delimiter = ' ') noexcept
	{
		const size_t length = string.Length();
		List<String> list(length);

		StringBuffer buffer(string);
		size_t headIndex = 0;
		for (size_t i = 0; i < length; i++)
		{
			if (string[i] != delimiter)
				continue;

			const size_t stringLength = i - headIndex;
			list.Emplace(buffer.Slice(0, stringLength).ToString());

			buffer = buffer.Slice(stringLength + 1);
			headIndex = i + 1;
		}

		list.Emplace(buffer.ToString());
		return list;
	}
}
