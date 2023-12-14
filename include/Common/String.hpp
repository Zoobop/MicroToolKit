#pragma once
#include <format>

#include "Core/Hash.hpp"
#include "Collections/Base/Enumerable.hpp"
#include "Utility/Options/Optional.hpp"
#include "Internal/StringInternals.hpp"
#include "Span.hpp"

namespace Micro
{
	class String;
	using string = String;

	/*
	 *  ============================================================
	 *	|                        Concepts                          |
	 *  ============================================================
	 */

    
	/// <summary>
	/// Represents a string-like object within the MicroToolKit library.
	/// </summary>
	template <typename T>
	concept CharSequence = requires(T string)
	{
		{ string.Data() } -> std::convertible_to<const char*>;
		{ string.Length() } -> std::convertible_to<size_t>;
		{ string.IsEmpty() } -> std::convertible_to<size_t>;
		{ string.Clear() } -> std::convertible_to<void>;
		{ string.operator[](0) } -> std::convertible_to<char>;
	};

	/// <summary>
	/// Represents a string-like object within the standard library.
	/// </summary>
	template <typename T>
	concept StdCharSequence = requires(T string)
	{
		{ string.data() } -> std::convertible_to<const char*>;
		{ string.size() } -> std::convertible_to<size_t>;
		{ string.empty() } -> std::convertible_to<bool>;
		{ string.clear() } -> std::convertible_to<void>;
	};

	/// <summary>
	/// Represents an object with a defined 'ToString' function.
	/// </summary>
	template <typename T>
	concept StringConvertible = requires(T value)
	{
		{ value.ToString() } -> std::convertible_to<String>;
	};


	/**
	 * \brief Standard class used to hold and represent text as a sequence of UTF-8 code units.
	 */
	class String final : public Enumerable<char>
	{
	public:
		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		/**
		 * \brief Initializes a new instance of the String class to have the underlying char pointer be null.
		 */
		constexpr String() noexcept = default;

		/**
		 * \brief Initializes a new instance of the String class to have the underlying char pointer be null. (Acts the same as default constructor)
		 */
		constexpr String(Null) noexcept
		{
		}

		/**
		 * \brief Initializes a new instance of the String class by copying the value of the String argument.
		 * \param string String to copy
		 */
		constexpr String(const String& string) noexcept
		{
			Allocate(string.m_Size);

			InternalCopy(string.m_Data, string.m_Size);
		}

		/**
		 * \brief Initializes a new instance of the String class by moving the value of the String argument.
		 * \param string String to move
		 */
		constexpr String(String&& string) noexcept
			: m_Data(std::move(string.m_Data)), m_Size(string.m_Size)
		{
			string.m_Data = nullptr;
			string.m_Size = 0;
		}

		/**
		 * \brief Initializes a new instance of the String class by copying the value of the String-like argument that is based on the CharSequence concept specifications.
		 * \param string String-like object to copy
		 */
		constexpr explicit String(const CharSequence auto& string) noexcept
		{
			const size_t length = string.Length();
			if (length == 0)
				return;

			Allocate(length);
			InternalCopy(string.Data(), string.Length());
		}

		/**
		 * \brief Initializes a new instance of the String class by copying the value of the String-like argument that is based on the StdCharSequence concept specifications.
		 * \param string String-like object to move
		 */
		constexpr explicit String(const StdCharSequence auto& string) noexcept
		{
			const size_t length = string.size();
			if (length == 0)
				return;

			Allocate(length);
			InternalCopy(string.data(), string.size());
		}

		/**
		 * \brief Initializes a new instance of the String class by filling the underlying buffer with the character argument.
		 * \param character Character used to fill character buffer
		 * \param count Number of times to repeat character (also represents length of string)
		 */
		constexpr String(const char character, const size_t count) noexcept
		{
			Allocate(count);

			for (size_t i = 0; i < count; i++)
				m_Data[i] = character;

			m_Data[m_Size] = 0;
		}

		/**
		 * \brief Initializes a new instance of the String class by copying the value of the char pointer.
		 * \param string Char pointer used to copy from
		 * \param length Length of the char pointer
		 */
		constexpr String(const char* string, const size_t length) noexcept
		{
			if (length == 0 || !string) 
				return;

			Allocate(length);
			InternalCopy(string, length);
		}

		/**
		 * \brief Initializes a new instance of the String class by copying the value of the char pointer (raw string literal).
		 * \param string Raw string literal as a char pointer
		 */
		constexpr String(const char* string) noexcept
		{
			const size_t length = GetLength(string);
			if (length == 0)
				return;

			Allocate(length);
			InternalCopy(string, length);
		}

		/**
		 * \brief Initializes a new instance of the String class by copying the value of the char pointer (raw string literal).
		 * \tparam TSize Represents the implicit capture of the raw string literal's size (including null termination)
		 * \param string Raw string literal as a char pointer
		 */
		template <size_t TSize>
		constexpr String(const char (&string)[TSize]) noexcept
		{
			constexpr size_t length = TSize - 1;
			if (length == 0)
				return;

			Allocate(length);
			InternalCopy(string, length);
		}

		/**
		 * \brief Initializes a new instance of the String class by copying the value of the char.
		 * \param character Character to copy from
		 */
		constexpr explicit String(const char character) noexcept
		{
			Allocate(1);
			m_Data[0] = character;
		}

		/**
		 * \brief Initializes a new instance of the String class by copying the value of the first address of the char pointer and calculating the size using pointer arithmetic.
		 * \param begin Beginning of the char pointer buffer
		 * \param end End of the char pointer buffer
		 */
		constexpr String(const char* begin, const char* end) noexcept
		{
			const size_t size = end - begin;
			if (size == 0)
				return;

			Allocate(size);
			InternalCopy(begin, size);
		}

		/**
		 * \brief Frees the memory of the underlying char buffer and sets it to null.
		 */
		constexpr ~String() noexcept override
		{
			delete[] m_Data.Data;

			m_Data = nullptr;
			m_Size = 0;
		}


		/*
		 *  ============================================================
		 *	|                         Accessors                        |
		 *  ============================================================
		 */


		/// <summary>
		/// Represents if the string is empty or not.
		/// </summary>
		/// <returns>True, if the string is empty.</returns>
		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Size == 0 || !m_Data.IsValidMemory(); }

		/// <summary>
		/// Represents a 64-bit unsigned integer as the length of the string.
		/// </summary>
		/// <returns>Length of type 'size_t'</returns>
		NODISCARD constexpr size_t Length() const noexcept { return m_Size; }

		/// <summary>
		/// Represents the underlying char buffer (const version).
		/// </summary>
		/// <returns>Character array of type 'const char*'.</returns>
		NODISCARD constexpr const char* Data() const noexcept { return m_Data; }

		/// <summary>
		/// Represents the underlying char buffer (non-const version).
		/// </summary>
		/// <returns>Character array of type 'char*'.</returns>
		NODISCARD constexpr char* Data() noexcept { return m_Data; }


		/* Enumerators (Iterators) */

		/// <summary>
		/// Gets the Enumerator that enumerates over the characters in the string.
		/// </summary>
		/// <returns>Enumerator to enumerate over characters</returns>
		NODISCARD Enumerator<char> GetEnumerator() override
		{
			for (size_t i = 0; i < m_Size; i++)
			{
				auto& element = m_Data[i];
				co_yield element;
			}
		}

		/// <summary>
		/// Gets the Enumerator that enumerates over the characters in the string. (const version)
		/// </summary>
		/// <returns>Enumerator to enumerate over characters</returns>
		NODISCARD Enumerator<char> GetEnumerator() const override
		{
			for (size_t i = 0; i < m_Size; i++)
			{
				const auto& element = m_Data[i];
				co_yield element;
			}
		}


		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		/// <summary>
		/// Adds the String-like argument that is based on the CharSequence concept specifications to the end of the char buffer.
		/// If argument is empty, it will return with no allocations. (Mutates instance)
		/// </summary>
		/// <param name="string">String-like object to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr String& Append(const CharSequence auto& string) noexcept
		{
			const size_t length = string.Length();
			if (length == 0)
				return *this;

			if (m_Data == nullptr)
			{
				Allocate(length);
				InternalCopy(string.Data(), length);
			}
			else
			{
				const size_t startIndex = m_Size;
				Reallocate(m_Size + length);
				InternalConcat(startIndex, string.Data(), string.Length());
			}

			return *this;
		}
		
		/// <summary>
		/// Adds the String-like argument that is based on the StdCharSequence concept specifications to the end of the char buffer.
		/// If argument is empty, it will return with no allocations. (Mutates instance)
		/// </summary>
		/// <param name="string">String-like object to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr String& Append(const StdCharSequence auto& string) noexcept
		{
			const size_t length = string.size();
			if (length == 0)
				return *this;

			if (m_Data == nullptr)
			{
				Allocate(length);
				InternalCopy(string.data(), length);
			}
			else
			{
				const size_t startIndex = m_Size;
				Reallocate(m_Size + length);
				InternalConcat(startIndex, string.data(), string.size());
			}

			return *this;
		}

		/// <summary>
		/// Adds the raw string literal argument to the end of the char buffer.
		/// If argument is empty, it will return with no allocations. (Mutates instance)
		/// </summary>
		/// <typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to append</param>
		/// <returns>Reference of this instance</returns>
		template <size_t TSize>
		constexpr String& Append(const char (&string)[TSize]) noexcept
		{
			constexpr size_t length = TSize - 1;
			if (length == 0)
				return *this;

			if (m_Data == nullptr)
			{
				Allocate(length);
				InternalCopy(string, length);
			}
			else
			{
				const size_t startIndex = m_Size;
				Reallocate(m_Size + length);
				InternalConcat(startIndex, string, length);
			}

			return *this;
		}

		/// <summary>
		/// Adds the char pointer argument to the end of the char buffer.
		/// If argument is null, it will return with no allocations. (Mutates instance)
		/// </summary>
		/// <param name="string">Char pointer to append</param>
		/// <param name="length">Length of the char pointer</param>
		/// <returns>Reference of this instance</returns>
		constexpr String& Append(const char* string, const size_t length) noexcept
		{
			if (length == 0 || !string) 
				return *this;

			if (m_Data == nullptr)
			{
				Allocate(length);
				InternalCopy(string, length);
			}
			else
			{
				const size_t startIndex = m_Size;
				Reallocate(m_Size + length);
				InternalConcat(startIndex, string, length);
			}

			return *this;
		}

		/// <summary>
		/// Adds the character argument to the end of the char buffer. (Mutates instance)
		/// </summary>
		/// <param name="character">Character to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr String& Append(const char character) noexcept
		{
			if (m_Data == nullptr)
			{
				Allocate(1);
				m_Data[0] = character;
			}
			else
			{
				Reallocate(m_Size + 1);
				m_Data[m_Size - 1] = character;
			}

			return *this;
		}

		/// <summary>
		/// Replaces all occurrences of the String-like object that is based on the CharSequence concept specifications with the replacement string.
		/// </summary>
		/// <param name="string">String to search for</param>
		/// <param name="replacement">String used as the replacement</param>
		/// <returns>New instance of a String with the appropriate characters replaced</returns>
		NODISCARD constexpr String Replace(const CharSequence auto& string, const CharSequence auto& replacement) const noexcept
		{
			const size_t rightLength = string.Length();
			const size_t leftLength = replacement.Length();
			if (IsEmpty() || rightLength > m_Size)
				return *this;

			// Get number of replacements
			size_t occurrences = 0;
			size_t index = 0;
			while (true)
			{
				const auto result = IndexOf(string, index);
				if (!result.IsValid())
					break;

				index = result.Value() + rightLength;
				occurrences++;
			}

			if (occurrences == 0)
				return *this;

			const size_t removedSize = m_Size - (rightLength * occurrences);
			const size_t replacedSize = removedSize + (leftLength * occurrences);
			const auto data = new char[replacedSize + 1];

			auto result = IndexOf(string);
			size_t offsetIndex = 0;
			for (size_t i = 0; i < m_Size; i++)
			{
				const size_t startIndex = result.Value();
				if (i == startIndex && result.IsValid())
				{
					for (size_t j = 0; j < leftLength; ++j)
						new(&data[j + offsetIndex]) char(replacement[j]);

					// Skip by previous length
					i += rightLength - 1;
					offsetIndex += leftLength;
					result = IndexOf(string, i + 1);
					continue;
				}

				new(&data[offsetIndex++]) char(m_Data[i]);
			}

			data[replacedSize] = 0;
			return Create(data, replacedSize);
		}

		/// <summary>
		/// Replaces all occurrences of the String-like object that is based on the StdCharSequence concept specifications with the replacement string.
		/// </summary>
		/// <param name="string">String to search for</param>
		/// <param name="replacement">String used as the replacement</param>
		/// <returns>New instance of a String with the appropriate characters replaced</returns>
		NODISCARD constexpr String Replace(const StdCharSequence auto& string, const StdCharSequence auto& replacement) const noexcept
		{
			const size_t rightLength = string.size();
			const size_t leftLength = replacement.size();
			if (IsEmpty() || rightLength > m_Size)
				return *this;

			// Get number of replacements
			size_t occurrences = 0;
			size_t index = 0;
			while (true)
			{
				const auto result = IndexOf(string, index);
				if (!result.IsValid())
					break;

				index = result.Value() + rightLength;
				occurrences++;
			}

			if (occurrences == 0)
				return *this;

			const size_t removedSize = m_Size - (rightLength * occurrences);
			const size_t replacedSize = removedSize + (leftLength * occurrences);
			const auto data = new char[replacedSize + 1];

			auto result = IndexOf(string);
			size_t offsetIndex = 0;
			for (size_t i = 0; i < m_Size; i++)
			{
				const size_t startIndex = result.Value();
				if (i == startIndex && result.IsValid())
				{
					for (size_t j = 0; j < leftLength; ++j)
						new(&data[j + offsetIndex]) char(replacement[j]);

					// Skip by previous length
					i += rightLength - 1;
					offsetIndex += leftLength;
					result = IndexOf(string, i + 1);
					continue;
				}

				new(&data[offsetIndex++]) char(m_Data[i]);
			}

			data[replacedSize] = 0;
			return Create(data, replacedSize);
		}

		/// <summary>
		/// Replaces all occurrences of the raw string literal with the replacement string.
		/// </summary>
		///	<typeparam name="TRightSize">Represents the implicit capture of the left raw string literal's size (including null termination)</typeparam>
		///	<typeparam name="TLeftSize">Represents the implicit capture of the right raw string literal's size (including null termination)</typeparam>
		/// <param name="string">String to search for</param>
		/// <param name="replacement">String used as the replacement</param>
		/// <returns>New instance of a String with the appropriate characters replaced</returns>
		template <size_t TRightSize, size_t TLeftSize>
		NODISCARD constexpr String Replace(const char(&string)[TRightSize], const char(&replacement)[TLeftSize]) const noexcept
		{
			constexpr size_t rightLength = TRightSize - 1;
			constexpr size_t leftLength = TLeftSize - 1;
			if (IsEmpty() || rightLength > m_Size)
				return *this;

			// Get number of replacements
			size_t occurrences = 0;
			size_t index = 0;
			while (true)
			{
				const auto result = IndexOf(string, index);
				if (!result.IsValid())
					break;

				index = result.Value() + rightLength;
				occurrences++;
			} 

			if (occurrences == 0)
				return *this;

			const size_t removedSize = m_Size - (rightLength * occurrences);
			const size_t replacedSize = removedSize + (leftLength * occurrences);
			const auto data = new char[replacedSize + 1];

			auto result = IndexOf(string);
			size_t offsetIndex = 0;
			for (size_t i = 0; i < m_Size; i++)
			{
				const size_t startIndex = result.Value();
				if (i == startIndex && result.IsValid())
				{
					for (size_t j = 0; j < leftLength; ++j)
						new(&data[j + offsetIndex]) char(replacement[j]);

					// Skip by previous length
					i += rightLength - 1;
					offsetIndex += leftLength;
					result = IndexOf(string, i + 1);
					continue;
				}

				new(&data[offsetIndex++]) char(m_Data[i]);
			}

			data[replacedSize] = 0;
			return Create(data, replacedSize);
		}

		/// <summary>
		/// Replaces all occurrences of the character with the replacement character.
		/// </summary>
		/// <param name="character">Character to search for</param>
		/// <param name="replacement">Character used as the replacement</param>
		/// <returns></returns>
		NODISCARD constexpr String Replace(const char character, const char replacement) const noexcept
		{
			if (IsEmpty())
				return *this;

			const auto string = new char[m_Size + 1];
			for (size_t i = 0; i < m_Size; ++i)
			{
				const char current = m_Data[i];
				string[i] = current == character ? replacement : current;
			}

			string[m_Size] = 0;
			return Create(string, m_Size);
		}

		/// <summary>
		/// Gets the span of characters from the start index to the end of the string.
		/// </summary>
		/// <param name="start">Index to start substring</param>
		/// <returns>New instance of a String with characters starting at the start index, or an empty String if invalid</returns>
		NODISCARD constexpr String Substring(const size_t start) const noexcept
		{
			const size_t length = m_Size - start;
			if (IsEmpty() || length == 0 || start >= m_Size || length + start > m_Size)
				return Empty();

			return { &m_Data[start], length };
		}

		/// <summary>
		/// Gets the span of characters from the start index to 'length' characters passed.
		/// </summary>
		/// <param name="start">Index to start substring</param>
		/// <param name="length">Number of characters to grab passed the start index</param>
		/// <returns>New instance of a String with characters starting at the start index through length, or an empty String if invalid</returns>
		NODISCARD constexpr String Substring(const size_t start, const size_t length) const noexcept
		{
			if (IsEmpty() || start >= m_Size || length + start > m_Size) 
				return Empty();

			return { &m_Data[start], length };
		}

		/// <summary>
		/// Trims all whitespace characters from both ends of the string.
		/// </summary>
		/// <returns>New instance of the trimmed String</returns>
		NODISCARD constexpr String Trim() const noexcept
		{
			constexpr char whitespace[6]{ '\n', '\t', '\r', '\b', ' ', '\0'};
			const Span<char> set(whitespace);

			size_t begin = 0;
			size_t end = 0;
			const size_t length = m_Size / 2;
			for (size_t i = 0; i < length; ++i)
			{
				const size_t endIndex = m_Size - 1 - i;
				const bool foundBegin = Micro::Contains(set, m_Data[i]);
				const bool foundEnd = Micro::Contains(set, m_Data[endIndex]);

				if (foundBegin) begin = i + 1;
				if (foundEnd) end = endIndex;

				if (!(foundBegin || foundEnd))
					break;
			}

			return Substring(begin, end - begin);
		}

		/// <summary>
		/// Trims each occurrence of the character from both ends of the string.
		/// </summary>
		/// <param name="character">Character to trim from string</param>
		/// <returns>New instance of the trimmed String</returns>
		NODISCARD constexpr String Trim(const char character) const noexcept
		{
			size_t begin = 0;
			size_t end = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t endIndex = m_Size - 1 - i;
				const bool foundBegin = m_Data[i] == character;
				const bool foundEnd = m_Data[endIndex] == character;

				if (foundBegin) begin = i + 1;
				if (foundEnd) end = endIndex;

				if (!(foundBegin || foundEnd))
					break;
			}

			return Substring(begin, end - begin);
		}

		/// <summary>
		/// Trims all characters from both ends of the string.
		/// </summary>
		/// <param name="characters">Characters to trim from the string</param>
		/// <returns>New instance of the trimmed String</returns>
		NODISCARD constexpr String Trim(std::convertible_to<char> auto... characters) const noexcept
		{
			constexpr size_t paramSize = sizeof ...(characters);
			if constexpr (paramSize == 0)
				return *this;

			const char values[paramSize + 1] = {characters..., '\0'};
			const Span<char> set(values);

			size_t begin = 0;
			size_t end = 0;
			const size_t length = m_Size / 2;
			for (size_t i = 0; i < length; ++i)
			{
				const size_t endIndex = m_Size - 1 - i;
				const bool foundBegin = Micro::Contains(set, m_Data[i]);
				const bool foundEnd = Micro::Contains(set, m_Data[endIndex]);

				if (foundBegin) begin = i + 1;
				if (foundEnd) end = endIndex;

				if (!(foundBegin || foundEnd))
					break;
			}

			return Substring(begin, end - begin);
		}

		/// <summary>
		/// Trims all whitespace characters from the left end of the string.
		/// </summary>
		/// <returns>New instance of the trimmed String</returns>
		NODISCARD constexpr String TrimStart() const noexcept
		{
			constexpr char whitespace[6]{ '\n', '\t', '\r', '\b', ' ', '\0' };
			const Span<char> set(whitespace);

			size_t begin = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				if (Micro::Contains(set, m_Data[i]))
					begin = i + 1;
				else
					break;
			}

			return Substring(begin);
		}

		/// <summary>
		/// Trims each occurrence of the character from the left end of the string.
		/// </summary>
		/// <param name="character">Character to trim from string</param>
		/// <returns>New instance of the trimmed String</returns>
		NODISCARD constexpr String TrimStart(const char character) const noexcept
		{
			size_t begin = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				if (m_Data[i] == character)
					begin = i + 1;
				else
					break;
			}

			return Substring(begin);
		}

		/// <summary>
		/// Trims all characters from the left end of the string.
		/// </summary>
		/// <param name="characters">Characters to trim from the string</param>
		/// <returns>New instance of the trimmed String</returns>
		NODISCARD constexpr String TrimStart(std::convertible_to<char> auto... characters) const noexcept
		{
			constexpr size_t length = sizeof ...(characters);
			if constexpr (length == 0) 
				return *this;

			const char values[length + 1] = {characters..., '\0'};
			const Span<char> set(values);

			size_t begin = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				if (Micro::Contains(set, m_Data[i]))
					begin = i + 1;
				else
					break;
			}

			return Substring(begin);
		}

		/// <summary>
		/// Trims all whitespace characters from the right end of the string.
		/// </summary>
		/// <returns>New instance of the trimmed String</returns>
		NODISCARD constexpr String TrimEnd() const noexcept
		{
			constexpr char whitespace[6]{ '\n', '\t', '\r', '\b', ' ', '\0' };
			const Span<char> set(whitespace);

			size_t end = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = m_Size - 1 - i;
				if (Micro::Contains(set, m_Data[offset]))
					end = offset;
				else
					break;
			}

			return Substring(0, end);
		}

		/// <summary>
		/// Trims each occurrence of the character from the right end of the string.
		/// </summary>
		/// <param name="character">Character to trim from string</param>
		/// <returns>New instance of the trimmed String</returns>
		NODISCARD constexpr String TrimEnd(const char character) const noexcept
		{
			size_t end = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = m_Size - 1 - i;
				if (m_Data[offset] == character)
					end = offset;
				else
					break;
			}

			return Substring(0, end);
		}

		/// <summary>
		/// Trims all characters from the right end of the string.
		/// </summary>
		/// <param name="characters">Characters to trim from the string</param>
		/// <returns>New instance of the trimmed String</returns>
		NODISCARD constexpr String TrimEnd(std::convertible_to<char> auto... characters) const noexcept
		{
			constexpr size_t length = sizeof ...(characters);
			if constexpr (length == 0)
				return *this;

			const char values[length + 1] = { characters..., '\0' };
			const Span<char> set(values);

			size_t end = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = m_Size - 1 - i;
				if (Micro::Contains(set, m_Data[offset]))
					end = m_Size - i - 1;
				else
					break;
			}

			return Substring(0, end);
		}

		/// <summary>
		/// Converts all valid characters to their uppercase forms.
		/// </summary>
		/// <returns>New instance of a String with all valid characters as uppercase.</returns>
		NODISCARD constexpr String ToUpper() const noexcept
		{
			if (IsEmpty())
				return {};

			const auto data = new char[m_Size + 1];
			for (size_t i = 0; i < m_Size; i++)
			{
				char character = m_Data[i];
				if (character >= 'a' && character <= 'z')
					character -= 32;

				data[i] = character;
			}

			return Create(data, m_Size);
		}

		/// <summary>
		/// Converts all valid characters to their lowercase forms.
		/// </summary>
		/// <returns>New instance of a String with all valid characters as lowercase.</returns>
		NODISCARD constexpr String ToLower() const noexcept
		{
			if (IsEmpty())
				return {};

			const auto data = new char[m_Size + 1];
			for (size_t i = 0; i < m_Size; i++)
			{
				char character = m_Data[i];
				if (character >= 'A' && character <= 'Z')
					character += 32;

				data[i] = character;
			}

			return Create(data, m_Size);
		}

		/// <summary>
		/// Copies and returns an instance of this String.
		/// </summary>
		/// <returns>Copy of this instance.</returns>
		NODISCARD constexpr String ToString() const noexcept { return *this; }

		/**
		 * \brief Frees the memory of the underlying char buffer and sets it to null.
		 */
		constexpr void Clear() noexcept
		{
			delete[] m_Data.Data;
			m_Data = nullptr;
			m_Size = 0;
		}

		/// <summary>
		/// Tests the equivalence between this instance and other String-like object that is based on the CharSequence concept specifications.
		/// </summary>
		/// <param name="string">String-like object to test against</param>
		/// <returns>True, if equal</returns>
		NODISCARD constexpr bool Equals(const CharSequence auto& string) const noexcept
		{
			if (m_Size != string.Length())
				return false;

			for (size_t i = 0; i < m_Size; i++)
			{
				if (m_Data[i] != string[i])
					return false;
			}

			return true;
		}

		/// <summary>
		/// Tests the equivalence between this instance and other String-like object that is based on the StdCharSequence concept specifications.
		/// </summary>
		/// <param name="string">String-like object to test against</param>
		/// <returns>True, if equal</returns>
		NODISCARD constexpr bool Equals(const StdCharSequence auto& string) const noexcept
		{
			if (m_Size != string.size())
				return false;

			for (size_t i = 0; i < m_Size; i++)
			{
				if (m_Data[i] != string[i])
					return false;
			}

			return true;
		}

		/// <summary>
		/// Tests the equivalence between this instance and the raw string literal.
		/// </summary>
		///	<typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to test against</param>
		/// <returns>True, if equal</returns>
		template <size_t TSize>
		NODISCARD constexpr bool Equals(const char (&string)[TSize]) const noexcept
		{
			constexpr size_t length = TSize - 1;
			if (m_Size != length)
				return false;

			for (size_t i = 0; i < length; i++)
			{
				if (m_Data[i] != string[i])
					return false;
			}

			return true;
		}

		/// <summary>
		/// Tests the equivalence between this instance and the char pointer.
		/// </summary>
		/// <param name="string">Char pointer</param>
		/// <param name="length">Length of the pointer</param>
		/// <returns>True, if equal</returns>
		NODISCARD constexpr bool Equals(const char* string, const size_t length) const noexcept
		{
			if (m_Size != length)
				return false;

			for (size_t i = 0; i < length; i++)
			{
				if (m_Data[i] != string[i])
					return false;
			}

			return true;
		}

		/// <summary>
		/// Tests the equivalence between this instance and the character.
		/// </summary>
		///	<temp
		/// <param name="character">Character to test against</param>
		/// <returns>True, if equal</returns>
		NODISCARD constexpr bool Equals(const char character) const noexcept
		{
			return m_Data[0] == character;
		}

		/// <summary>
		/// Tests whether the string contains the String-like object that is based on the CharSequence concept specifications. 
		/// </summary>
		/// <param name="string">String-like object to test against</param>
		/// <returns>True, if is contained within the string</returns>
		NODISCARD constexpr bool Contains(const CharSequence auto& string) const noexcept
		{
			const size_t length = string.Length();
			if (length == 0) 
				return false;

			return IndexOf(string).IsValid();
		}

		/// <summary>
		/// Tests whether the string contains the String-like object that is based on the StdCharSequence concept specifications.
		/// </summary>
		/// <param name="string">String-like object to test against</param>
		/// <returns>True, if is contained within the string</returns>
		NODISCARD constexpr bool Contains(const StdCharSequence auto& string) const
		{
			const size_t length = string.size();
			if (length == 0)
				return false;

			return IndexOf(string).IsValid();
		}

		/// <summary>
		/// Tests whether the string contains the raw string literal.
		/// </summary>
		///	<typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to test against</param>
		/// <returns>True, if is contained within the string</returns>
		template <size_t TSize>
		NODISCARD constexpr bool Contains(const char(&string)[TSize]) const noexcept
		{
			const size_t length = TSize - 1;
			if (length == 0)
				return false;

			return IndexOf(string).IsValid();
		}

		/// <summary>
		/// Tests whether the string contains the raw string literal.
		/// </summary>
		/// <param name="string">Char pointer to test against</param>
		/// <param name="length">Length of char pointer</param>
		/// <returns>True, if is contained within the string</returns>
		NODISCARD constexpr bool Contains(const char* string, const size_t length) const noexcept
		{
			if (length == 0)
				return false;

			const String wrapper = Create(string, length);
			return IndexOf(wrapper).IsValid();
		}

		/// <summary>
		/// Tests whether the string contains the character.
		/// </summary>
		/// <param name="character">Character to test</param>
		/// <returns>True, if is contained within the string</returns>
		NODISCARD constexpr bool Contains(const char character) const noexcept
		{
			for (size_t i = 0; i < m_Size; ++i)
				if (m_Data[i] == character)
					return true;

			return false;
		}

		/// <summary>
		/// Tries to find the index of the String-like object that is based on the CharSequence concept specifications.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <returns>Index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Optional<size_t> IndexOf(const CharSequence auto& string) const noexcept
		{
			const size_t length = string.Length();
			if (IsEmpty() || length == 0 || length > m_Size)
				return Optional<size_t>::Empty();

			for (size_t i = 0; i < m_Size; ++i)
			{
				if (m_Data[i] == string[0])
				{
					size_t index = i;
					size_t count = 0;
					while (m_Data[index] == string[count])
					{
						index++;
						count++;

						if (count == length)
							return Optional<size_t>(i);

						if (index >= m_Size)
							return Optional<size_t>::Empty();
					}
				}
			}

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the index of the String-like object that is based on the StdCharSequence concept specifications.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <returns>Index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Optional<size_t> IndexOf(const StdCharSequence auto& string) const noexcept
		{
			const size_t length = string.size();
			if (IsEmpty() || length == 0 || length > m_Size)
				return Optional<size_t>::Empty();

			for (size_t i = 0; i < m_Size; ++i)
			{
				if (m_Data[i] == string[0])
				{
					size_t index = i;
					size_t count = 0;
					while (m_Data[index] == string[count])
					{
						index++;
						count++;

						if (count == length)
							return Optional<size_t>(i);

						if (index >= m_Size)
							return Optional<size_t>::Empty();
					}
				}
			}

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the index of the raw string literal.
		/// </summary>
		/// <typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to find</param>
		/// <returns>Index of the start of the string, or an invalid result if not found</returns>
		template <size_t TSize>
		NODISCARD constexpr Optional<size_t> IndexOf(const char(&string)[TSize]) const noexcept
		{
			constexpr size_t length = TSize - 1;
			if (IsEmpty() || length == 0 || length > m_Size)
				return Optional<size_t>::Empty();

			for (size_t i = 0; i < m_Size; ++i)
			{
				if (m_Data[i] == string[0])
				{
					size_t index = i;
					size_t count = 0;
					while (m_Data[index] == string[count])
					{
						index++;
						count++;

						if (count == length)
							return Optional<size_t>(i);

						if (index >= m_Size)
							return Optional<size_t>::Empty();
					}
				}
			}

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the index of the character.
		/// </summary>
		/// <param name="character">Character to find</param>
		/// <returns>Index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Optional<size_t> IndexOf(const char character) const noexcept
		{
			for (size_t i = 0; i < m_Size; ++i)
				if (m_Data[i] == character)
					return Optional<size_t>(i);

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the index of the String-like object that is based on the CharSequence concept specifications from the given start index.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <param name="startIndex">Index to start at</param>
		/// <returns>Index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Optional<size_t> IndexOf(const CharSequence auto& string, const size_t startIndex) const noexcept
		{
			const size_t length = string.Length();
			if (IsEmpty() || length == 0 || length > m_Size || startIndex >= m_Size)
				return Optional<size_t>::Empty();

			for (size_t i = startIndex; i < m_Size; ++i)
			{
				if (m_Data[i] == string[0])
				{
					size_t index = i;
					size_t count = 0;
					while (m_Data[index] == string[count])
					{
						index++;
						count++;

						if (count == length)
							return Optional<size_t>(i);

						if (index >= m_Size)
							return Optional<size_t>::Empty();
					}
				}
			}

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the index of the String-like object that is based on the StdCharSequence concept specifications from the given start index.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <param name="startIndex">Index to start at</param>
		/// <returns>Index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Optional<size_t> IndexOf(const StdCharSequence auto& string, const size_t startIndex) const noexcept
		{
			const size_t length = string.size();
			if (IsEmpty() || length == 0 || length > m_Size || startIndex >= m_Size)
				return Optional<size_t>::Empty();

			for (size_t i = startIndex; i < m_Size; ++i)
			{
				if (m_Data[i] == string[0])
				{
					size_t index = i;
					size_t count = 0;
					while (m_Data[index] == string[count])
					{
						index++;
						count++;

						if (count == length)
							return Optional<size_t>(i);

						if (index >= m_Size)
							return Optional<size_t>::Empty();
					}
				}
			}

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the index of the raw string literal from the given start index.
		/// </summary>
		/// <typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to find</param>
		/// <param name="startIndex">Index to start at</param>
		/// <returns>Index of the start of the string, or an invalid result if not found</returns>
		template <size_t TSize>
		NODISCARD constexpr Optional<size_t> IndexOf(const char(&string)[TSize], const size_t startIndex) const noexcept
		{
			constexpr size_t length = TSize - 1;
			if (IsEmpty() || length == 0 || length > m_Size || startIndex >= m_Size)
				return Optional<size_t>::Empty();

			for (size_t i = startIndex; i < m_Size; ++i)
			{
				if (m_Data[i] == string[0])
				{
					size_t index = i;
					size_t count = 0;
					while (m_Data[index] == string[count])
					{
						index++;
						count++;

						if (count == length)
							return Optional<size_t>(i);

						if (index >= m_Size)
							return Optional<size_t>::Empty();
					}
				}
			}

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the index of the character at the given start index.
		/// </summary>
		/// <param name="character">Character to find</param>
		/// <param name="startIndex">Index to start at</param>
		/// <returns>Index of the start of the character, or an invalid result if not found</returns>
		NODISCARD constexpr Optional<size_t> IndexOf(const char character, const size_t startIndex) const noexcept
		{
			if (startIndex >= m_Size)
				return Optional<size_t>::Empty();

			for (size_t i = startIndex; i < m_Size; ++i)
				if (m_Data[i] == character)
					return Optional<size_t>(i);

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the String-like object that is based on the CharSequence concept specifications.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <returns>Last index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Optional<size_t> LastIndexOf(const CharSequence auto& string) const noexcept
		{
			const size_t length = string.Length();
			if (IsEmpty() || length == 0 || length > m_Size)
				return Optional<size_t>::Empty();

			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = m_Size - 1 - i;
				if (m_Data[offset] == string[length - 1])
				{
					size_t index = offset;
					size_t count = length - 1;
					while (m_Data[index] == string[count])
					{
						if (count == 0)
							return Optional<size_t>(index);

						if (index == 0)
							return Optional<size_t>::Empty();

						--index;
						--count;
					}
				}
			}

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the String-like object that is based on the StdCharSequence concept specifications.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <returns>Last index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Optional<size_t> LastIndexOf(const StdCharSequence auto& string) const noexcept
		{
			const size_t length = string.size();
			if (IsEmpty() || length == 0 || length > m_Size)
				return Optional<size_t>::Empty();

			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = m_Size - 1 - i;
				if (m_Data[offset] == string[length - 1])
				{
					size_t index = offset;
					size_t count = length - 1;
					while (m_Data[index] == string[count])
					{
						if (count == 0)
							return Optional<size_t>(index);

						if (index == 0)
							return Optional<size_t>::Empty();

						--index;
						--count;
					}
				}
			}

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the raw string literal.
		/// </summary>
		/// <typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to find</param>
		/// <returns>Last index of the start of the string, or an invalid result if not found</returns>
		template <size_t TSize>
		NODISCARD constexpr Optional<size_t> LastIndexOf(const char(&string)[TSize]) const noexcept
		{
			constexpr size_t length = TSize - 1;
			if (IsEmpty() || length == 0 || length > m_Size)
				return Optional<size_t>::Empty();

			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = m_Size - 1 - i;
				if (m_Data[offset] == string[length - 1])
				{
					size_t index = offset;
					size_t count = length - 1;
					while (m_Data[index] == string[count])
					{
						if (count == 0)
							return Optional<size_t>(index);

						if (index == 0)
							return Optional<size_t>::Empty();

						--index;
						--count;
					}
				}
			}

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the character.
		/// </summary>
		/// <param name="character">Character to find</param>
		/// <returns>Last index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Optional<size_t> LastIndexOf(const char character) const noexcept
		{
			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = m_Size - 1 - i;
				if (m_Data[offset] == character)
					return Optional<size_t>(offset);
			}

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the String-like object that is based on the CharSequence concept specifications to the given end index.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <param name="endIndex">Index to end at</param>
		/// <returns>Last index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Optional<size_t> LastIndexOf(const CharSequence auto& string, const size_t endIndex) const noexcept
		{
			const size_t length = string.Length();
			if (IsEmpty() || length == 0 || length > m_Size || endIndex >= m_Size)
				return Optional<size_t>::Empty();

			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = endIndex - i;
				if (m_Data[offset] == string[length - 1])
				{
					size_t index = offset;
					size_t count = length - 1;
					while (m_Data[index] == string[count])
					{
						if (count == 0)
							return Optional<size_t>(index);

						if (index == 0)
							return Optional<size_t>::Empty();

						--index;
						--count;
					}
				}
			}

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the String-like object that is based on the StdCharSequence concept specifications to the given end index.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <param name="endIndex">Index to end at</param>
		/// <returns>Last index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Optional<size_t> LastIndexOf(const StdCharSequence auto& string, const size_t endIndex) const noexcept
		{
			const size_t length = string.size();
			if (IsEmpty() || length == 0 || length > m_Size || endIndex >= m_Size)
				return Optional<size_t>::Empty();

			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = endIndex - i;
				if (m_Data[offset] == string[length - 1])
				{
					size_t index = offset;
					size_t count = length - 1;
					while (m_Data[index] == string[count])
					{
						if (count == 0)
							return Optional<size_t>(index);

						if (index == 0)
							return Optional<size_t>::Empty();

						--index;
						--count;
					}
				}
			}

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the raw string literal to the given end index.
		/// </summary>
		///	<typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to find</param>
		/// <param name="endIndex">Index to end at</param>
		/// <returns>Last index of the start of the string, or an invalid result if not found</returns>
		template <size_t TSize>
		NODISCARD constexpr Optional<size_t> LastIndexOf(const char(&string)[TSize], const size_t endIndex) const noexcept
		{
			constexpr size_t length = TSize - 1;
			if (IsEmpty() || length == 0 || length > m_Size || endIndex >= m_Size)
				return Optional<size_t>::Empty();

			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = endIndex - i;
				if (m_Data[offset] == string[length - 1])
				{
					size_t index = offset;
					size_t count = length - 1;
					while (m_Data[index] == string[count])
					{
						if (count == 0)
							return Optional<size_t>(index);

						if (index == 0)
							return Optional<size_t>::Empty();

						--index;
						--count;
					}
				}
			}

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the character to the given end index.
		/// </summary>
		/// <param name="character">Character to find</param>
		/// <param name="endIndex">Index to end at</param>
		/// <returns>Last index of the character, or an invalid result if not found</returns>
		NODISCARD constexpr Optional<size_t> LastIndexOf(const char character, const size_t endIndex) const noexcept
		{
			if (endIndex >= m_Size)
				return Optional<size_t>::Empty();

			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = endIndex - i;
				if (m_Data[offset] == character)
					return Optional<size_t>(offset);
			}

			return Optional<size_t>::Empty();
		}

		/// <summary>
		/// Tests if the complete span of the given String-like object that is based on the CharSequence concept
		/// specifications is the start of the string.
		/// </summary>
		/// <param name="string">String to find</param>
		/// <returns>True, if starts with string</returns>
		NODISCARD constexpr bool StartsWith(const CharSequence auto& string) const noexcept
		{
			const size_t length = string.Length();
			if (IsEmpty() || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
				if (m_Data[i] != string[i])
					return false;

			return true;
		}

		/// <summary>
		/// Tests if the complete span of the given String-like object that is based on the StdCharSequence concept
		/// specifications is the start of the string.
		/// </summary>
		/// <param name="string">String to find</param>
		/// <returns>True, if starts with string</returns>
		NODISCARD constexpr bool StartsWith(const StdCharSequence auto& string) const noexcept
		{
			const size_t length = string.size();
			if (IsEmpty() || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
				if (m_Data[i] != string[i])
					return false;

			return true;
		}

		/// <summary>
		/// Tests if the complete span of the raw string literal is the start of the string.
		/// </summary>
		/// <typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">String to find</param>
		/// <returns>True, if starts with string</returns>
		template <size_t TSize>
		NODISCARD constexpr bool StartsWith(const char(&string)[TSize]) const noexcept
		{
			constexpr size_t length = TSize - 1;
			if (IsEmpty() || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
				if (m_Data[i] != string[i])
					return false;

			return true;
		}

		/// <summary>
		/// Tests if the complete span of the char pointer is the start of the string.
		/// </summary>
		/// <param name="string">Char pointer to find</param>
		/// <param name="length">Length of the char pointer</param>
		/// <returns>True, if starts with string</returns>
		NODISCARD constexpr bool StartsWith(const char* string, const size_t length) const noexcept
		{
			if (IsEmpty() || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
				if (m_Data[i] != string[i])
					return false;

			return true;
		}

		/// <summary>
		/// Tests if the character is the first element of the string.
		/// </summary>
		/// <param name="character">Character to find</param>
		/// <returns>True, if starts with string</returns>
		NODISCARD constexpr bool StartsWith(const char character) const noexcept
		{
			if (m_Size == 0)
				return false;

			return m_Data[0] == character;
		}

		/// <summary>
		/// Tests if the complete span of the given String-like object that is based on the CharSequence concept
		/// specifications is the end of the string.
		/// </summary>
		/// <param name="string">String to find</param>
		/// <returns>True, if ends with string</returns>
		NODISCARD constexpr bool EndsWith(const CharSequence auto& string) const noexcept
		{
			const size_t length = string.Length();
			if (m_Size == 0 || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
			{
				const size_t index = m_Size - i - 1;
				if (m_Data[index] != string[length - i - 1])
					return false;
			}

			return true;
		}

		/// <summary>
		/// Tests if the complete span of the given String-like object that is based on the StdCharSequence concept
		/// specifications is the end of the string.
		/// </summary>
		/// <param name="string">String to find</param>
		/// <returns>True, if ends with string</returns>
		NODISCARD constexpr bool EndsWith(const StdCharSequence auto& string) const noexcept
		{
			const size_t length = string.size();
			if (m_Size == 0 || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
			{
				const size_t index = m_Size - i - 1;
				if (m_Data[index] != string[length - i - 1])
					return false;
			}

			return true;
		}
		
		/// <summary>
		/// Tests if the complete span of the raw string literal is the end of the string.
		/// </summary>
		/// <typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">String to find</param>
		/// <returns>True, if ends with string</returns>
		template <size_t TSize>
		NODISCARD constexpr bool EndsWith(const char (&string)[TSize]) const noexcept
		{
			constexpr size_t length = TSize - 1;
			if (m_Size == 0 || m_Size < length)
				return false;

			for (size_t i = 0; i < length; ++i)
			{
				const size_t index = m_Size - i - 1;
				if (m_Data[index] != string[length - i - 1])
					return false;
			}

			return true;
		}

		/// <summary>
		/// Tests if the complete span of the char pointer is the end of the string.
		/// </summary>
		/// <param name="string">Char pointer to find</param>
		/// <param name="length">Length of the char pointer</param>
		/// <returns>True, if ends with string</returns>
		NODISCARD constexpr bool EndsWith(const char* string, const size_t length) const noexcept
		{
			if (m_Size == 0 || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
			{
				const size_t index = m_Size - i - 1;
				if (m_Data[index] != string[length - i - 1])
					return false;
			}

			return true;
		}

		/// <summary>
		/// Tests if the character is the last element of the string.
		/// </summary>
		/// <param name="character">Character to find</param>
		/// <returns>True, if ends with string</returns>
		NODISCARD constexpr bool EndsWith(const char character) const noexcept
		{
			if (m_Size == 0)
				return false;

			return m_Data[m_Size - 1] == character;
		}


		/*
		 *  ============================================================
		 *	|                          Static                          |
		 *  ============================================================
		 */


		/// <summary>
		/// Creates an empty String. (No heap allocations)
		/// </summary>
		/// <returns>New instance of an empty String</returns>
		NODISCARD constexpr static String Empty() noexcept { return {}; }

		/// <summary>
		/// Uses the char pointer and length to create a new string without another allocation.
		/// </summary>
		/// <param name="data">Char pointer</param>
		/// <param name="length">Length of char pointer</param>
		/// <returns>New instance of String with char pointer as the underlying buffer</returns>
		NODISCARD constexpr static String Create(const char* data, const size_t length) noexcept
		{
			String string;
			string.m_Data = const_cast<char*>(data);
			string.m_Size = length;
			string.m_Data[length] = 0;
			return string;
		}


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		/**
		 * \brief Implicit conversion to const char*
		 */
		constexpr operator const char*() const noexcept { return m_Data; }

		/**
		 * \brief Implicit conversion to std::string
		 */
		constexpr operator std::string() const noexcept { return m_Data.Data; }

		/**
		 * \brief Implicit conversion to Span<char>
		 */
		constexpr operator Span<char>() const noexcept { return { m_Data, m_Size }; }

		/// <summary>
		/// Gets the character at the given index, or an empty result if invalid.
		/// </summary>
		/// <param name="index">Index of element</param>
		/// <returns>Copy of character at index, or empty result if invalid</returns>
		NODISCARD constexpr Optional<char> operator[](const size_t index) noexcept
		{
			if (index >= m_Size)
				return Optional<char>::Empty();

			return Optional<char>(m_Data[index]);
		}

		/// <summary>
		/// Gets the character at the given index, or an empty result if invalid. (const version)
		/// </summary>
		/// <param name="index">Index of element</param>
		/// <returns>Copy of character at index, or empty result if invalid</returns>
		NODISCARD constexpr Optional<char> operator[](const size_t index) const noexcept
		{
			if (index >= m_Size)
				return Optional<char>::Empty();

			return Optional<char>(m_Data[index]);
		}

		/// <summary>
		/// Copies the data from the given string.
		/// </summary>
		/// <param name="string">String to copy</param>
		/// <returns>Reference of this instance</returns>
		constexpr String& operator=(const String& string) noexcept
		{
			if (this == &string)
				return *this;

			const size_t length = string.m_Size;
			if (!m_Data.IsValidMemory())
			{
				Allocate(length);
			}
			else
			{
				if (length != m_Size)
					Reallocate(length);
			}

			InternalCopy(string.m_Data, length);
			return *this;
		}

		/// <summary>
		/// Move the data from the given string.
		/// </summary>
		/// <param name="string">String to move</param>
		/// <returns>Reference of this instance</returns>
		constexpr String& operator=(String&& string) noexcept
		{
			if (this == &string)
				return *this;

			if (m_Data.IsValidMemory())
				Clear();

			m_Data = string.m_Data;
			m_Size = string.m_Size;

			string.m_Data = nullptr;
			string.m_Size = 0;

			return *this;
		}

		/// <summary>
		/// Copies the data from the given String-like object that is based on the CharSequence concept specifications.
		/// </summary>
		/// <param name="string">String-like object to copy</param>
		/// <returns>Reference of this instance</returns>
		constexpr String& operator=(const CharSequence auto& string) noexcept
		{
			if (this == &string)
				return *this;

			const size_t length = string.Size();
			if (!m_Data.IsValidMemory())
			{
				Allocate(length);
			}
			else
			{
				if (length != m_Size)
					Reallocate(length);
			}

			InternalCopy(string.Data(), length);
			return *this;
		}

		/// <summary>
		/// Copies the data from the given String-like object that is based on the StdCharSequence concept specifications.
		/// </summary>
		/// <param name="string">String-like object to copy</param>
		/// <returns>Reference of this instance</returns>
		constexpr String& operator=(const StdCharSequence auto& string) noexcept
		{
			const size_t length = string.size();
			if (!m_Data.IsValidMemory())
			{
				Allocate(length);
			}
			else
			{
				if (length != m_Size)
					Reallocate(length);
			}

			InternalCopy(string.data(), length);
			return *this;
		}

		/// <summary>
		/// Copies the data from the given raw string literal.
		/// </summary>
		/// <typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to copy</param>
		/// <returns>Reference of this instance</returns>
		template <size_t TSize>
		constexpr String& operator=(const char(&string)[TSize]) noexcept
		{
			constexpr size_t length = TSize - 1;
			if (length == 0)
				return *this;

			if (!m_Data.IsValidMemory())
			{
				Allocate(length);
			}
			else
			{
				if (length != m_Size)
					Reallocate(length);
			}

			InternalCopy(string, length);
			return *this;
		}

		/// <summary>
		/// Copies the data from the given character.
		/// </summary>
		/// <param name="character">Character to copy</param>
		/// <returns>Reference of this instance</returns>
		constexpr String& operator=(const char character) noexcept
		{
			if (!m_Data.IsValidMemory())
			{
				Allocate(1);
			}
			else
			{
				if (1 != m_Size)
					Reallocate(1);
			}

			m_Data[0] = character;
			m_Data[m_Size] = 0;
			return *this;
		}

		/// <summary>
		/// Appends the data from the given String to the end of the underlying buffer.
		/// </summary>
		/// <param name="string">String to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr String& operator+=(const String& string) noexcept { return Append(string); }

		/// <summary>
		/// Appends the data from the given String-like object that is based on the CharSequence concept specifications to the end of the underlying buffer.
		/// </summary>
		/// <param name="string">String-like object to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr String& operator+=(const CharSequence auto& string) noexcept { return Append(string); }

		/// <summary>
		/// Appends the data from the given String-like object that is based on the StdCharSequence concept specifications to the end of the underlying buffer.
		/// </summary>
		/// <param name="string">String-like object to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr String& operator+=(const StdCharSequence auto& string) noexcept { return Append(string); }

		/// <summary>
		/// Appends the data from the given raw string literal to the end of the underlying buffer.
		/// </summary>
		///	<typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to append</param>
		/// <returns>Reference of this instance</returns>
		template <size_t TSize>
		constexpr String& operator+=(const char (&string)[TSize]) noexcept { return Append(string, TSize); }

		/// <summary>
		/// Appends the given character to the end of the underlying buffer.
		/// </summary>
		/// <param name="character">Character to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr String& operator+=(const char character) noexcept { return Append(character); }

		/// <summary>
		/// Creates a new String from the concatenation of the two strings.
		/// </summary>
		/// <param name="left">String to concatenate</param>
		/// <param name="right">String to concatenate</param>
		/// <returns>New instance of the concatenated String</returns>
		constexpr friend String operator+(const String& left, const String& right) noexcept
		{
			const size_t leftSize = left.m_Size;
			const size_t rightSize = right.m_Size;

			if (leftSize + rightSize == 0) return {};
			if (leftSize == 0) return right;
			if (rightSize == 0) return left;

			const size_t size = leftSize + rightSize;
			const auto data = new char[size + 1];

			String newString = Create(data, size);
			newString.InternalCopy(left.m_Data, leftSize);
			newString.InternalConcat(leftSize, right.m_Data, rightSize);
			return newString;
		}

		/// <summary>
		/// Creates a new String from the concatenation of the two strings.
		/// </summary>
		/// <param name="left">String to concatenate</param>
		/// <param name="right">String to concatenate</param>
		/// <returns>New instance of the concatenated String</returns>
		constexpr friend String operator+(const String& left, const CharSequence auto& right) noexcept
		{
			const size_t leftSize = left.m_Size;
			const size_t rightSize = right.Length();

			if (leftSize + rightSize == 0) return {};
			if (leftSize == 0) return right;
			if (rightSize == 0) return left;

			const size_t size = leftSize + rightSize;
			const auto data = new char[size + 1];

			String newString = Create(data, size);
			newString.InternalCopy(left.m_Data, leftSize);
			newString.InternalConcat(leftSize, right.Data(), rightSize);
			return newString;
		}

		/// <summary>
		/// Creates a new String from the concatenation of the two strings.
		/// </summary>
		/// <param name="left">String to concatenate</param>
		/// <param name="right">String to concatenate</param>
		/// <returns>New instance of the concatenated String</returns>
		constexpr friend String operator+(const String& left, const StdCharSequence auto& right) noexcept
		{
			const size_t leftSize = left.m_Size;
			const size_t rightSize = right.size();

			if (leftSize + rightSize == 0) return {};
			if (leftSize == 0) return right;
			if (rightSize == 0) return left;

			const size_t size = leftSize + rightSize;
			const auto data = new char[size + 1];

			String newString = Create(data, size);
			newString.InternalCopy(left.m_Data, leftSize);
			newString.InternalConcat(leftSize, right.data(), rightSize);
			return newString;
		}

		/// <summary>
		/// Creates a new String from the concatenation of the two strings.
		/// </summary>
		/// <param name="left">String to concatenate</param>
		/// <param name="right">String to concatenate</param>
		/// <returns>New instance of the concatenated String</returns>
		template <size_t TSize>
		constexpr friend String operator+(const String& left, const char(&right)[TSize]) noexcept
		{
			const size_t leftSize = left.m_Size;
			const size_t rightSize = TSize - 1;

			if (leftSize + rightSize == 0) return {};
			if (leftSize == 0) return right;
			if (rightSize == 0) return left;

			const size_t size = leftSize + rightSize;
			const auto data = new char[size + 1];

			String newString = Create(data, size);
			newString.InternalCopy(left.m_Data, leftSize);
			newString.InternalConcat(leftSize, right, rightSize);
			return newString;
		}

		/// <summary>
		/// Creates a new String from the concatenation of the two strings.
		/// </summary>
		/// <param name="left">String to concatenate</param>
		/// <param name="character">Character to concatenate</param>
		/// <returns>New instance of the concatenated String</returns>
		constexpr friend String operator+(const String& left, const char character) noexcept
		{
			const size_t leftSize = left.m_Size;
			constexpr size_t rightSize = 1;

			if (leftSize == 0) 
				return String {character};

			const size_t size = leftSize + rightSize;
			const auto data = new char[size + 1];

			String newString = Create(data, size);
			newString.InternalCopy(left.m_Data, left.m_Size);
			newString.InternalConcat(leftSize, &character, rightSize);
			return newString;
		}

		/// <summary>
		/// Creates a new String from the concatenation of the two string.
		/// </summary>
		/// <param name="left">String to concatenate</param>
		/// <param name="right">String to concatenate</param>
		/// <returns>New instance of the concatenated String</returns>
		constexpr friend String operator+(const CharSequence auto& left, const String& right) noexcept
		{
			const size_t leftSize = left.Length();
			const size_t rightSize = right.m_Size;

			if (leftSize + rightSize == 0) return {};
			if (leftSize == 0) return right;
			if (rightSize == 0) return left;

			const size_t size = leftSize + rightSize;
			const auto data = new char[size + 1];

			String newString = Create(data, size);
			newString.InternalCopy(left.Data(), leftSize);
			newString.InternalConcat(leftSize, right.m_Data, rightSize);
			return newString;
		}

		/// <summary>
		/// Creates a new String from the concatenation of the two strings.
		/// </summary>
		/// <param name="left">String to concatenate</param>
		/// <param name="right">String to concatenate</param>
		/// <returns>New instance of the concatenated String</returns>
		constexpr friend String operator+(const StdCharSequence auto& left, const String& right) noexcept
		{
			const size_t leftSize = left.size();
			const size_t rightSize = right.m_Size;

			if (leftSize + rightSize == 0) return {};
			if (leftSize == 0) return right;
			if (rightSize == 0) return left;

			const size_t size = leftSize + rightSize;
			const auto data = new char[size + 1];

			String newString = Create(data, size);
			newString.InternalCopy(left.data(), leftSize);
			newString.InternalConcat(leftSize, right.m_Data, rightSize);
			return newString;
		}

		/// <summary>
		/// Creates a new String from the concatenation of the two strings.
		/// </summary>
		/// <param name="left">String to concatenate</param>
		/// <param name="right">String to concatenate</param>
		/// <returns>New instance of the concatenated String</returns>
		template <size_t TSize>
		constexpr friend String operator+(const char(&left)[TSize], const String& right) noexcept
		{
			constexpr size_t leftSize = TSize - 1;
			const size_t rightSize = right.m_Size;

			if (leftSize + rightSize == 0) return {};
			if (leftSize == 0) return right;
			if (rightSize == 0) return { left, leftSize };

			const size_t size = leftSize + rightSize;
			const auto data = new char[size + 1];

			String newString = Create(data, size);
			newString.InternalCopy(left, leftSize);
			newString.InternalConcat(leftSize, right.m_Data, rightSize);
			return newString;
		}

		/// <summary>
		/// Creates a new String from the concatenation of the two strings.
		/// </summary>
		/// <param name="character">Character to concatenate</param>
		/// <param name="right">String to concatenate</param>
		/// <returns>New instance of the concatenated String</returns>
		constexpr friend String operator+(const char character, const String& right) noexcept
		{
			constexpr size_t leftSize = 1;
			const size_t rightSize = right.m_Size;

			if (rightSize == 0) 
				return String{character};

			const size_t size = leftSize + rightSize;
			const auto data = new char[size + 1];

			String newString = Create(data, size);
			newString.InternalCopy(&character, leftSize);
			newString.InternalConcat(leftSize, right.m_Data, rightSize);
			return newString;
		}

		/// <summary>
		/// Tests if the two strings are equal.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if equal</returns>
		constexpr friend bool operator==(const String& left, const String& right) noexcept { return left.Equals(right); }

		/// <summary>
		/// Tests if the two strings are equal.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if equal</returns>
		constexpr friend bool operator==(const String& left, const CharSequence auto& right) noexcept { return left.Equals(right); }

		/// <summary>
		/// Tests if the two strings are equal.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if equal</returns>
		constexpr friend bool operator==(const String& left, const StdCharSequence auto& right) noexcept { return left.Equals(right); }

		/// <summary>
		/// Tests if the two strings are equal.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if equal</returns>
		template <size_t TSize>
		constexpr friend bool operator==(const String& left, const char(&right)[TSize]) noexcept { return left.Equals(right); }

		/// <summary>
		/// Tests if the string is equal to the character.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="character">Character to test against</param>
		/// <returns>True, if equal</returns>
		constexpr friend bool operator==(const String& left, const char character) noexcept { return left.Equals(character); }

		/// <summary>
		/// Tests if the two strings are equal.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if equal</returns>
		constexpr friend bool operator==(const CharSequence auto& left, const String& right) noexcept { return right.Equals(left); }

		/// <summary>
		/// Tests if the two strings are equal.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if equal</returns>
		constexpr friend bool operator==(const StdCharSequence auto& left, const String& right) noexcept { return right.Equals(left); }

		/// <summary>
		/// Tests if the two strings are equal.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if equal</returns>
		template <size_t TSize>
		constexpr friend bool operator==(const char(&left)[TSize], const String& right) noexcept { return right.Equals(left); }

		/// <summary>
		/// Tests if the character is equal to the string.
		/// </summary>
		/// <param name="character">Character to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if equal</returns>
		constexpr friend bool operator==(const char character, const String& right) noexcept { return right.Equals(character); }

		/// <summary>
		/// Tests if the two strings are not equal.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if not equal</returns>
		constexpr friend bool operator!=(const String& left, const String& right) noexcept { return !(left == right); }

		/// <summary>
		/// Tests if the two strings are not equal.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if not equal</returns>
		constexpr friend bool operator!=(const String& left, const CharSequence auto& right) noexcept { return !(left == right); }

		/// <summary>
		/// Tests if the two strings are not equal.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if not equal</returns>
		constexpr friend bool operator!=(const String& left, const StdCharSequence auto& right) noexcept { return !(left == right); }

		/// <summary>
		/// Tests if the two strings are not equal.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if not equal</returns>
		template <size_t TSize>
		constexpr friend bool operator!=(const String& left, const char (&right)[TSize]) noexcept { return !(left == right); }

		/// <summary>
		/// Tests if the string is not equal to the character.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="character">Character to test against</param>
		/// <returns>True, if not equal</returns>
		constexpr friend bool operator!=(const String& left, const char character) noexcept { return !(left == character); }

		/// <summary>
		/// Tests if the two strings are not equal.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if not equal</returns>
		constexpr friend bool operator!=(const CharSequence auto& left, const String& right) noexcept { return !(left == right); }

		/// <summary>
		/// Tests if the two strings are not equal.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if not equal</returns>
		constexpr friend bool operator!=(const StdCharSequence auto& left, const String& right) noexcept { return !(left == right); }

		/// <summary>
		/// Tests if the two strings are not equal.
		/// </summary>
		/// <param name="left">String to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if not equal</returns>
		template <size_t TSize>
		constexpr friend bool operator!=(const char (&left)[TSize], const String& right) noexcept { return !(left == right); }

		/// <summary>
		/// Tests if the character is not equal to the string.
		/// </summary>
		/// <param name="character">Character to test against</param>
		/// <param name="right">String to test against</param>
		/// <returns>True, if not equal</returns>
		constexpr friend bool operator!=(const char character, const String& right) noexcept { return !(character == right); }

		/// <summary>
		/// Prints the given string out to the console.
		/// </summary>
		/// <param name="stream">Stream to print to the standard output</param>
		/// <param name="current">String to print out</param>
		/// <returns>Reference of the stream</returns>
		constexpr friend std::ostream& operator<<(std::ostream& stream, const String& current) noexcept
		{
			if (current.m_Size > 0)
				stream << current.m_Data;
			return stream;
		}

	protected:
		/*
		 *  ============================================================
		 *	|                      Internal Helpers                    |
		 *  ============================================================
		 */


		/// <summary>
		/// Allocates a new block of memory with +1 capacity to account for null termination character.
		/// </summary>
		/// <param name="capacity">New capacity to allocate with</param>
		constexpr void Allocate(const size_t capacity) noexcept
		{
			if (capacity == 0) return;

			const size_t length = capacity + 1;
			m_Data = new char[length];
			m_Size = capacity;
			m_Data[capacity] = 0;
		}

		/// <summary>
		/// Reallocates a new block of memory or calls Allocate if the underlying buffer is null.
		/// </summary>
		/// <param name="capacity">New capacity to allocate with</param>
		constexpr void Reallocate(const size_t capacity) noexcept
		{
			if (capacity == 0) return;
			const size_t length = capacity + 1;

			// Reallocation
			if (m_Data != nullptr)
			{
				m_Data = static_cast<char*>(realloc(m_Data, length));
				m_Size = capacity;
				m_Data[m_Size] = 0;
				return;
			}

			// Allocate initial memory
			Allocate(capacity);
		}

		/// <summary>
		/// Copies the char pointer into the underlying buffer using the given length.
		/// </summary>
		/// <param name="ptr">Char pointer to copy</param>
		/// <param name="length">Length of char pointer</param>
		constexpr void InternalCopy(const char* ptr, const size_t length) noexcept
		{
			for (size_t i = 0; i < length; i++)
				new(&m_Data[i]) char(ptr[i]);
				
			m_Data[m_Size] = 0;
		}

		/// <summary>
		/// Copies the char pointer into the underlying buffer using the given length at a given starting index.
		/// </summary>
		/// <param name="startIndex">Index to start internal copy</param>
		/// <param name="ptr">Char pointer to copy</param>
		/// <param name="length">Length of char pointer</param>
		constexpr void InternalConcat(const size_t startIndex, const char* ptr, const size_t length) noexcept
		{
			for (size_t i = 0; i < length; i++)
				new(&m_Data[i + startIndex]) char(ptr[i]);
		}

	private:
		Memory<char> m_Data = nullptr;
		size_t m_Size = 0;
	};


	/*
	 *  ============================================================
	 *	|                    Global Functions                      |
	 *  ============================================================
	 */


	/// <summary>
	/// Hashes the String to produce a unique hash code.
	/// </summary>
	/// <param name="object">String to hash</param>
	/// <returns>Hash code as a 'size_t'</returns>
	template <>
	NODISCARD inline size_t Hash(const String& object) noexcept
	{
		const size_t size = object.Length();
		const auto data = object.Data();
		size_t hash = 0;
		for (size_t i = 0; i < size; i++)
			hash += data[i];

		return typeid(String).hash_code() + size + hash;
	}


	/// <summary>
	/// Converts a signed integer into a String.
	/// </summary>
	/// <param name="integral">Signed integer to convert</param>
	/// <returns>Signed integer as a String</returns>
	template <typename TElem, std::signed_integral T>
	NODISCARD constexpr String IntToString(T integral) noexcept
	{
		static_assert(std::is_integral_v<T>, "T must be integral");
		using UType = std::make_unsigned_t<T>;
		TElem buffer[21];
		TElem* const end = std::end(buffer);
		TElem* next = end;
		const auto uValue = static_cast<UType>(integral);
		if (integral < 0)
		{
			next = Internal::UIntegral_Internal(next, 0 - uValue);
			*--next = '-';
		}
		else
			next = Internal::UIntegral_Internal(next, uValue);

		return String(next, end);
	}

	/// <summary>
	/// Converts an unsigned integer into a String.
	/// </summary>
	/// <param name="integral">Unsigned integer to convert</param>
	/// <returns>Unsigned integer as a String</returns>
	template <typename TElem, std::unsigned_integral T>
	NODISCARD constexpr String UIntToString(T integral) noexcept
	{
		static_assert(std::is_integral_v<T>, "T must be integral");
		static_assert(std::is_unsigned_v<T>, "T must be unsigned");
		TElem buffer[21];
		TElem* const end = std::end(buffer);
		TElem* const next = Internal::UIntegral_Internal(end, integral);
		return String(next, end);
	}

	/// <summary>
	/// Converts a bool into a String.
	/// </summary>
	/// <param name="boolean">Bool to convert</param>
	/// <returns>Bool as a String</returns>
	NODISCARD constexpr String ToString(const bool boolean) noexcept { return boolean ? String{"true"} : String{"false"}; }

	/// <summary>
	/// Converts a character into a String.
	/// </summary>
	/// <param name="character">Character to convert</param>
	/// <returns>Character as a String</returns>
	NODISCARD constexpr String ToString(const char character) noexcept { return String(character); }

	/// <summary>
	/// Converts a signed integer into a String.
	/// </summary>
	/// <param name="integral">Signed integer to convert</param>
	/// <returns>Signed integer as a String</returns>
	NODISCARD constexpr String ToString(const int integral) noexcept { return IntToString<char>(integral); }

	/// <summary>
	/// Converts an unsigned integer into a String.
	/// </summary>
	/// <param name="integral">Unsigned integer to convert</param>
	/// <returns>Unsigned integer as a String</returns>
	NODISCARD constexpr String ToString(const unsigned int integral) noexcept { return UIntToString<char>(integral); }

	/// <summary>
	/// Converts an signed long into a String.
	/// </summary>
	/// <param name="integral">Signed long to convert</param>
	/// <returns>Signed long as a String</returns>
	NODISCARD constexpr String ToString(const long integral) noexcept { return IntToString<char>(integral); }

	/// <summary>
	/// Converts an unsigned long into a String.
	/// </summary>
	/// <param name="integral">Unsigned long to convert</param>
	/// <returns>Unsigned long as a String</returns>
	NODISCARD constexpr String ToString(const unsigned long integral) noexcept { return UIntToString<char>(integral); }

	/// <summary>
	/// Converts a long long into a String.
	/// </summary>
	/// <param name="integral">Long long to convert</param>
	/// <returns>Long long as a String</returns>
	NODISCARD constexpr String ToString(const long long integral) noexcept { return IntToString<char>(integral); }

	/// <summary>
	/// Converts an unsigned long long into a String.
	/// </summary>
	/// <param name="integral">Unsigned long long to convert</param>
	/// <returns>Unsigned long long as a String</returns>
	NODISCARD constexpr String ToString(const unsigned long long integral) noexcept { return UIntToString<char>(integral); }

	/// <summary>
	/// Converts a double into a String.
	/// </summary>
	/// <param name="floatingPoint">Double to convert</param>
	/// <returns>Double as a String</returns>
	NODISCARD inline String ToString(const f64 floatingPoint) noexcept
	{
		const Span<char> span = Internal::FloatToString_Internal<f64>("%Lf", floatingPoint);
		return String::Create(span.Data(), span.Capacity());
	}

	/// <summary>
	/// Converts a float into a String.
	/// </summary>
	/// <param name="floatingPoint">Float to convert</param>
	/// <returns>Float as a String</returns>
	NODISCARD inline String ToString(const f32 floatingPoint) noexcept 
	{ 
		const Span<char> span = Internal::FloatToString_Internal<f32>("%f", floatingPoint);
		return String::Create(span.Data(), span.Capacity());
	}
}


template <>
struct std::formatter<Micro::String> {
	constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const Micro::String& obj, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}", obj.Data());
    }
};