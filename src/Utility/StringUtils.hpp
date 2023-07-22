#pragma once
#include "Collections/List.hpp"
#include "Common/StringBuffer.hpp"
#include "Common/StringBuilder.hpp"

namespace Micro
{
	NODISCARD constexpr bool IsNullOrEmpty(const CharSequence auto& string) noexcept
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

	template <StringConvertible T>
	NODISCARD constexpr String Join(const char character, const Sequence<T>& sequence) noexcept
	{
		constexpr size_t defaultSize = 64;

		StringBuilder builder(defaultSize);

		for (const auto& e : sequence)
			builder.Append(e.ToString()).Append(character);

		return builder.ToString();
	}

	template <StringConvertible T>
	NODISCARD constexpr String Join(const CharSequence auto& string, const Sequence<T>& sequence) noexcept
	{
		constexpr size_t defaultSize = 64;

		StringBuilder builder(defaultSize);

		for (const auto& e : sequence)
			builder.Append(e.ToString()).Append(string);

		return builder.ToString();
	}

	NODISCARD bool Contains(const CharSequence auto& string, const char character) noexcept
	{
		for (size_t i = 0; i < string.Length(); ++i)
			if (string[i] == character)
				return true;

		return false;
	}

	NODISCARD bool Contains(const CharSequence auto& string, const CharSequence auto& value) noexcept
	{
		const size_t length = value.Length();
		if (length == 0)
			return false;

		for (size_t i = 0, charIndex = 0; i < string.Length(); ++i)
			while (string[i] == value[charIndex])
			{
				++i;
				++charIndex;

				if (charIndex == length) 
					return true;
			}

		return false;
	}

	NODISCARD size_t Count(const CharSequence auto& string, const char character) noexcept
	{
		size_t count = 0;
		for (size_t i = 0; i < string.Length(); ++i)
			if (string[i] == character)
				count++;

		return count;
	}

	NODISCARD size_t Count(const CharSequence auto& string, const Predicate<char>& predicate) noexcept
	{
		size_t count = 0;
		for (size_t i = 0; i < string.Length(); ++i)
			if (predicate(string[i]))
				count++;

		return count;
	}

	NODISCARD Result<size_t> IndexOf(const CharSequence auto& string, const char character) noexcept
	{
		for (size_t i = 0; i < string.Length(); ++i)
			if (string[i] == character)
				return Result(i);

		return Result<size_t>::Empty();
	}

	NODISCARD Result<size_t> LastIndexOf(const CharSequence auto& string, const char character) noexcept
	{
		for (size_t i = string.Length(); i > 0; --i)
			if (string[i - 1] == character)
				return Result(i - 1);

		return Result<size_t>::Empty();
	}

	NODISCARD bool StartsWith(const CharSequence auto& string, const char character) noexcept
	{
		if (string.IsEmpty())
			return false;

		return string[0] == character;
	}

	NODISCARD bool StartsWith(const CharSequence auto& string, const CharSequence auto& value) noexcept
	{
		const size_t length = value.Length();
		if (string.IsEmpty() || string.Length() < length) 
			return false;

		for (size_t i = 0; i < length; ++i)
			if (string[i] != value[i])
				return false;

		return true;
	}

	NODISCARD bool EndsWith(const CharSequence auto& string, const char character) noexcept
	{
		if (string.IsEmpty())
			return false;

		return string[string.Length() - 1] == character;
	}

	NODISCARD bool EndsWith(const CharSequence auto& string, const CharSequence auto& value) noexcept
	{
		const size_t length = value.Length();
		if (string.IsEmpty() || string.Length() < length)
			return false;

		for (size_t i = 0; i < length; ++i)
		{
			const size_t index = string.Length() - i - 1;
			if (string[index] != value[index])
				return false;
		}

		return true;
	}

	NODISCARD List<String> Split(const CharSequence auto& string, const char delimiter = ' ') noexcept
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
