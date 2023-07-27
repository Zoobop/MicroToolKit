#pragma once
#include "Common/String.hpp"

namespace Micro
{
	/**
	 * \brief Represents a way to manipulate a String's underlying buffer without any allocations or modifying the source buffer.
	 */
	class StringBuffer final
	{
	public:
		/*
		 *  ============================================================
		 *	|                 Constructors/Destructors                 |
		 *  ============================================================
		 */


		/**
		 * \brief Initializes a new instance of the StringBuffer class to have the underlying pointer reference be null.
		 */
		constexpr StringBuffer() noexcept = default;

		/**
		 * \brief Initializes a new instance of the StringBuffer class to have the underlying pointer reference be null.
		 *		  Acts as the default constructor.
		 */
		constexpr StringBuffer(Null) noexcept
		{
		}

		/**
		 * \brief Initializes a new instance of the StringBuffer class to have the underlying pointer reference be the
		 *		  same as the copied StringBuffer.
		 */
		constexpr StringBuffer(const StringBuffer&) noexcept = default;

		/**
		 * \brief Initializes a new instance of the StringBuffer class to have the underlying pointer reference be
		 *		  from the moved StringBuffer. This removes the pointer reference from the move StringBuffer.
		 */
		constexpr StringBuffer(StringBuffer&&) noexcept = default;

		/**
		 * \brief Initializes a new instance of the StringBuffer class to have the underlying pointer reference be
		 *		  the same as the String-like argument that is based on the CharSequence concept specifications.
		 * \param string 
		 */
		constexpr explicit StringBuffer(const CharSequence auto& string) noexcept
			: m_Data(string.Data()), m_Size(string.Length())
		{
		}

		/**
		 * \brief Initializes a new instance of the StringBuffer class to have the underlying pointer reference be
		 *		  the same as the String-like argument that is based on the StdCharSequence concept specifications.
		 * \param string 
		 */
		constexpr explicit StringBuffer(const StdCharSequence auto& string) noexcept
			: m_Data(string.data()), m_Size(string.size())
		{
		}

		/**
		 * \brief Initializes a new instance of the StringBuffer class to have the underlying pointer reference be
		 *		  the same as the raw string literal.
		 * \tparam TSize Represents the implicit capture of the raw string literal's size (including null termination)
		 * \param string Raw string literal
		 */
		template <size_t TSize>
		constexpr StringBuffer(const char (&string)[TSize]) noexcept
			: m_Data(string), m_Size(TSize - 1)
		{
		}

		/**
		 * \brief Initializes a new instance of the StringBuffer class to have the underlying pointer reference be
		 *		  the same as the char pointer.
		 * \param string Char pointer
		 * \param length Length of the char pointer
		 */
		constexpr StringBuffer(const char* string, const size_t length) noexcept
			: m_Data(string), m_Size(length)
		{
		}

		/**
		 * \brief Removes the pointer reference by setting it to null.
		 */
		constexpr ~StringBuffer() noexcept = default;


		/*
		 *  ============================================================
		 *	|                        Accessors                         |
		 *  ============================================================
		 */


		/// <summary>
		/// Represents if the string is empty or not.
		/// </summary>
		/// <returns>True, if the string is empty.</returns>
		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Size == 0 || m_Data == nullptr; }

		/// <summary>
		/// Represents a 64-bit unsigned integer as the length of the string.
		/// </summary>
		/// <returns>Length of type 'size_t'</returns>
		NODISCARD constexpr size_t Length() const noexcept { return m_Size; }

		/// <summary>
		/// Represents the underlying pointer reference to string data.
		/// </summary>
		/// <returns>Character array of type 'const char*'.</returns>
		NODISCARD constexpr const char* Data() const noexcept { return m_Data; }

		
		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		/// <summary>
		/// Gets the span of characters from the start index to the end of the string. Throws an 'IndexOutOfRangeException', if index is invalid.
		/// </summary>
		/// <param name="start">Index to start slice</param>
		/// <returns>New view of a StringBuffer with characters starting at the start index</returns>
		NODISCARD constexpr StringBuffer Slice(const size_t start) const
		{
			if (start >= m_Size)
				throw IndexOutOfRangeException(start);

			return {m_Data + start, m_Data + m_Size};
		}

		/// <summary>
		/// Gets the span of characters from the start index to 'length' characters passed. Throws an 'IndexOutOfRangeException', if index is invalid.
		/// </summary>
		/// <param name="start">Index to start slice</param>
		/// <param name="length">Number of characters to grab passed the start index</param>
		/// <returns>New view of a StringBuffer with characters starting at the start index through length</returns>
		NODISCARD constexpr StringBuffer Slice(const size_t start, const size_t length) const
		{
			if (start + length > m_Size)
				throw IndexOutOfRangeException(start + length);

			return {m_Data + start, m_Data + start + length};
		}

		/// <summary>
		/// Trims all whitespace characters from both ends of the string.
		/// </summary>
		/// <returns>New view of the trimmed StringBuffer</returns>
		NODISCARD constexpr StringBuffer Trim() const noexcept
		{
			constexpr char whitespace[6]{ '\n', '\t', '\r', '\b', ' ', '\0' };
			const Sequence<char> set(whitespace);

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

			return Slice(begin, end - begin);
		}

		/// <summary>
		/// Trims each occurrence of the character from both ends of the string.
		/// </summary>
		/// <param name="character">Character to trim from string</param>
		/// <returns>New view of the trimmed StringBuffer</returns>
		NODISCARD constexpr StringBuffer Trim(const char character) const noexcept
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

			return Slice(begin, end - begin);
		}

		/// <summary>
		/// Trims all characters from both ends of the string.
		/// </summary>
		/// <param name="characters">Characters to trim from the string</param>
		/// <returns>New view of the trimmed StringBuffer</returns>
		NODISCARD constexpr StringBuffer Trim(std::convertible_to<char> auto... characters) const noexcept
		{
			constexpr size_t paramSize = sizeof ...(characters);
			if constexpr (paramSize == 0)
				return *this;

			const char values[paramSize + 1] = { characters..., '\0' };
			const Sequence<char> set(values);

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

			return Slice(begin, end - begin);
		}

		/// <summary>
		/// Trims all whitespace characters from the left end of the string.
		/// </summary>
		/// <returns>New view of the trimmed StringBuffer</returns>
		NODISCARD constexpr StringBuffer TrimStart() const noexcept
		{
			constexpr char whitespace[6]{ '\n', '\t', '\r', '\b', ' ', '\0' };
			const Sequence<char> set(whitespace);

			size_t begin = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				if (Micro::Contains(set, m_Data[i]))
					begin = i + 1;
				else
					break;
			}

			return Slice(begin);
		}

		/// <summary>
		/// Trims each occurrence of the character from the left end of the string.
		/// </summary>
		/// <param name="character">Character to trim from string</param>
		/// <returns>New view of the trimmed StringBuffer</returns>
		NODISCARD constexpr StringBuffer TrimStart(const char character) const noexcept
		{
			size_t begin = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				if (m_Data[i] == character)
					begin = i + 1;
				else
					break;
			}

			return Slice(begin);
		}

		/// <summary>
		/// Trims all characters from the left end of the string.
		/// </summary>
		/// <param name="characters">Characters to trim from the string</param>
		/// <returns>New view of the trimmed StringBuffer</returns>
		NODISCARD constexpr StringBuffer TrimStart(std::convertible_to<char> auto... characters) const noexcept
		{
			constexpr size_t length = sizeof ...(characters);
			if constexpr (length == 0)
				return *this;

			const char values[length + 1] = { characters..., '\0' };
			const Sequence<char> set(values);

			size_t begin = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				if (Micro::Contains(set, m_Data[i]))
					begin = i + 1;
				else
					break;
			}

			return Slice(begin);
		}

		/// <summary>
		/// Trims all whitespace characters from the right end of the string.
		/// </summary>
		/// <returns>New view of the trimmed StringBuffer</returns>
		NODISCARD constexpr StringBuffer TrimEnd() const noexcept
		{
			constexpr char whitespace[6]{ '\n', '\t', '\r', '\b', ' ', '\0' };
			const Sequence<char> set(whitespace);

			size_t end = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = m_Size - 1 - i;
				if (Micro::Contains(set, m_Data[offset]))
					end = offset;
				else
					break;
			}

			return Slice(0, end);
		}

		/// <summary>
		/// Trims each occurrence of the character from the right end of the string.
		/// </summary>
		/// <param name="character">Character to trim from string</param>
		/// <returns>New view of the trimmed StringBuffer</returns>
		NODISCARD constexpr StringBuffer TrimEnd(const char character) const noexcept
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

			return Slice(0, end);
		}

		/// <summary>
		/// Trims all characters from the right end of the string.
		/// </summary>
		/// <param name="characters">Characters to trim from the string</param>
		/// <returns>New view of the trimmed StringBuffer</returns>
		NODISCARD constexpr StringBuffer TrimEnd(std::convertible_to<char> auto... characters) const noexcept
		{
			constexpr size_t length = sizeof ...(characters);
			if constexpr (length == 0)
				return *this;

			const char values[length + 1] = { characters..., '\0' };
			const Sequence<char> set(values);

			size_t end = 0;
			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = m_Size - 1 - i;
				if (Micro::Contains(set, m_Data[offset]))
					end = m_Size - i - 1;
				else
					break;
			}

			return Slice(0, end);
		}

		/// <summary>
		/// Takes the current buffer view and creates a new String instance from it.
		/// </summary>
		/// <returns>New instance of a String using the character buffer view.</returns>
		NODISCARD constexpr String ToString() const noexcept { return { m_Data, m_Size }; }

		/**
		 * \brief Removes the pointer reference by setting it to null.
		 */
		constexpr void Clear() noexcept
		{
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
		NODISCARD constexpr bool Equals(const char(&string)[TSize]) const noexcept
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

			const StringBuffer wrapper(string, length);
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
		NODISCARD constexpr Result<size_t> IndexOf(const CharSequence auto& string) const noexcept
		{
			const size_t length = string.Length();
			if (IsEmpty() || length == 0 || length > m_Size)
				return Result<size_t>::Empty();

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
							return Result<size_t>(i);

						if (index >= m_Size)
							return Result<size_t>::Empty();
					}
				}
			}

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the index of the String-like object that is based on the StdCharSequence concept specifications.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <returns>Index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Result<size_t> IndexOf(const StdCharSequence auto& string) const noexcept
		{
			const size_t length = string.size();
			if (IsEmpty() || length == 0 || length > m_Size)
				return Result<size_t>::Empty();

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
							return Result<size_t>(i);

						if (index >= m_Size)
							return Result<size_t>::Empty();
					}
				}
			}

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the index of the raw string literal.
		/// </summary>
		/// <typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to find</param>
		/// <returns>Index of the start of the string, or an invalid result if not found</returns>
		template <size_t TSize>
		NODISCARD constexpr Result<size_t> IndexOf(const char(&string)[TSize]) const noexcept
		{
			constexpr size_t length = TSize - 1;
			if (IsEmpty() || length == 0 || length > m_Size)
				return Result<size_t>::Empty();

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
							return Result<size_t>(i);

						if (index >= m_Size)
							return Result<size_t>::Empty();
					}
				}
			}

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the index of the character.
		/// </summary>
		/// <param name="character">Character to find</param>
		/// <returns>Index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Result<size_t> IndexOf(const char character) const noexcept
		{
			for (size_t i = 0; i < m_Size; ++i)
				if (m_Data[i] == character)
					return Result<size_t>(i);

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the index of the String-like object that is based on the CharSequence concept specifications from the given start index.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <param name="startIndex">Index to start at</param>
		/// <returns>Index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Result<size_t> IndexOf(const CharSequence auto& string, const size_t startIndex) const noexcept
		{
			const size_t length = string.Length();
			if (IsEmpty() || length == 0 || length > m_Size || startIndex >= m_Size)
				return Result<size_t>::Empty();

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
							return Result<size_t>(i);

						if (index >= m_Size)
							return Result<size_t>::Empty();
					}
				}
			}

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the index of the String-like object that is based on the StdCharSequence concept specifications from the given start index.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <param name="startIndex">Index to start at</param>
		/// <returns>Index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Result<size_t> IndexOf(const StdCharSequence auto& string, const size_t startIndex) const noexcept
		{
			const size_t length = string.size();
			if (IsEmpty() || length == 0 || length > m_Size || startIndex >= m_Size)
				return Result<size_t>::Empty();

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
							return Result<size_t>(i);

						if (index >= m_Size)
							return Result<size_t>::Empty();
					}
				}
			}

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the index of the raw string literal from the given start index.
		/// </summary>
		/// <typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to find</param>
		/// <param name="startIndex">Index to start at</param>
		/// <returns>Index of the start of the string, or an invalid result if not found</returns>
		template <size_t TSize>
		NODISCARD constexpr Result<size_t> IndexOf(const char(&string)[TSize], const size_t startIndex) const noexcept
		{
			constexpr size_t length = TSize - 1;
			if (IsEmpty() || length == 0 || length > m_Size || startIndex >= m_Size)
				return Result<size_t>::Empty();

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
							return Result<size_t>(i);

						if (index >= m_Size)
							return Result<size_t>::Empty();
					}
				}
			}

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the index of the character at the given start index.
		/// </summary>
		/// <param name="character">Character to find</param>
		/// <param name="startIndex">Index to start at</param>
		/// <returns>Index of the start of the character, or an invalid result if not found</returns>
		NODISCARD constexpr Result<size_t> IndexOf(const char character, const size_t startIndex) const noexcept
		{
			if (startIndex >= m_Size)
				return Result<size_t>::Empty();

			for (size_t i = startIndex; i < m_Size; ++i)
				if (m_Data[i] == character)
					return Result<size_t>(i);

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the String-like object that is based on the CharSequence concept specifications.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <returns>Last index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Result<size_t> LastIndexOf(const CharSequence auto& string) const noexcept
		{
			const size_t length = string.Length();
			if (IsEmpty() || length == 0 || length > m_Size)
				return Result<size_t>::Empty();

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
							return Result<size_t>(index);

						if (index == 0)
							return Result<size_t>::Empty();

						--index;
						--count;
					}
				}
			}

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the String-like object that is based on the StdCharSequence concept specifications.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <returns>Last index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Result<size_t> LastIndexOf(const StdCharSequence auto& string) const noexcept
		{
			const size_t length = string.size();
			if (IsEmpty() || length == 0 || length > m_Size)
				return Result<size_t>::Empty();

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
							return Result<size_t>(index);

						if (index == 0)
							return Result<size_t>::Empty();

						--index;
						--count;
					}
				}
			}

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the raw string literal.
		/// </summary>
		/// <typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to find</param>
		/// <returns>Last index of the start of the string, or an invalid result if not found</returns>
		template <size_t TSize>
		NODISCARD constexpr Result<size_t> LastIndexOf(const char(&string)[TSize]) const noexcept
		{
			constexpr size_t length = TSize - 1;
			if (IsEmpty() || length == 0 || length > m_Size)
				return Result<size_t>::Empty();

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
							return Result<size_t>(index);

						if (index == 0)
							return Result<size_t>::Empty();

						--index;
						--count;
					}
				}
			}

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the character.
		/// </summary>
		/// <param name="character">Character to find</param>
		/// <returns>Last index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Result<size_t> LastIndexOf(const char character) const noexcept
		{
			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = m_Size - 1 - i;
				if (m_Data[offset] == character)
					return Result<size_t>(offset);
			}

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the String-like object that is based on the CharSequence concept specifications to the given end index.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <param name="endIndex">Index to end at</param>
		/// <returns>Last index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Result<size_t> LastIndexOf(const CharSequence auto& string, const size_t endIndex) const noexcept
		{
			const size_t length = string.Length();
			if (IsEmpty() || length == 0 || length > m_Size || endIndex >= m_Size)
				return Result<size_t>::Empty();

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
							return Result<size_t>(index);

						if (index == 0)
							return Result<size_t>::Empty();

						--index;
						--count;
					}
				}
			}

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the String-like object that is based on the StdCharSequence concept specifications to the given end index.
		/// </summary>
		/// <param name="string">String-like object to find</param>
		/// <param name="endIndex">Index to end at</param>
		/// <returns>Last index of the start of the string, or an invalid result if not found</returns>
		NODISCARD constexpr Result<size_t> LastIndexOf(const StdCharSequence auto& string, const size_t endIndex) const noexcept
		{
			const size_t length = string.size();
			if (IsEmpty() || length == 0 || length > m_Size || endIndex >= m_Size)
				return Result<size_t>::Empty();

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
							return Result<size_t>(index);

						if (index == 0)
							return Result<size_t>::Empty();

						--index;
						--count;
					}
				}
			}

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the raw string literal to the given end index.
		/// </summary>
		///	<typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to find</param>
		/// <param name="endIndex">Index to end at</param>
		/// <returns>Last index of the start of the string, or an invalid result if not found</returns>
		template <size_t TSize>
		NODISCARD constexpr Result<size_t> LastIndexOf(const char(&string)[TSize], const size_t endIndex) const noexcept
		{
			constexpr size_t length = TSize - 1;
			if (IsEmpty() || length == 0 || length > m_Size || endIndex >= m_Size)
				return Result<size_t>::Empty();

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
							return Result<size_t>(index);

						if (index == 0)
							return Result<size_t>::Empty();

						--index;
						--count;
					}
				}
			}

			return Result<size_t>::Empty();
		}

		/// <summary>
		/// Tries to find the last index of the character to the given end index.
		/// </summary>
		/// <param name="character">Character to find</param>
		/// <param name="endIndex">Index to end at</param>
		/// <returns>Last index of the character, or an invalid result if not found</returns>
		NODISCARD constexpr Result<size_t> LastIndexOf(const char character, const size_t endIndex) const noexcept
		{
			if (endIndex >= m_Size)
				return Result<size_t>::Empty();

			for (size_t i = 0; i < m_Size; ++i)
			{
				const size_t offset = endIndex - i;
				if (m_Data[offset] == character)
					return Result<size_t>(offset);
			}

			return Result<size_t>::Empty();
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
		NODISCARD constexpr bool EndsWith(const char(&string)[TSize]) const noexcept
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
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		/// <summary>
		/// Gets the character at the given index. Throws an 'IndexOutOfRangeException', if index is invalid.
		/// </summary>
		/// <param name="index">Index of element</param>
		/// <returns>Copy of character at index</returns>
		NODISCARD constexpr char operator[](const size_t index)
		{
			if (index >= m_Size)
				throw IndexOutOfRangeException(index);

			return m_Data[index];
		}

		/// <summary>
		/// Gets the character at the given index. Throws an 'IndexOutOfRangeException', if index is invalid. (const version)
		/// </summary>
		/// <param name="index">Index of element</param>
		/// <returns>Copy of character at index</returns>
		NODISCARD constexpr char operator[](const size_t index) const
		{
			if (index >= m_Size)
				throw IndexOutOfRangeException(index);

			return m_Data[index];
		}

		/// <summary>
		/// Copies the reference of the StringBuffer into this instance.
		/// </summary>
		/// <param name="string">StringBuffer to copy reference from</param>
		/// <returns>Reference to this instance</returns>
		constexpr StringBuffer& operator=(const StringBuffer& string) noexcept
		{
			if (this == &string)
				return *this;

			m_Data = string.m_Data;
			m_Size = string.m_Size;

			return *this;
		}

		/// <summary>
		/// Moves the reference of the StringBuffer into this instance.
		/// </summary>
		/// <param name="string">StringBuffer to move reference from</param>
		/// <returns>Reference to this instance</returns>
		constexpr StringBuffer& operator=(StringBuffer&& string) noexcept
		{
			if (this == &string)
				return *this;

			m_Data = string.m_Data;
			m_Size = string.m_Size;

			string.Clear();

			return *this;
		}

		/// <summary>
		/// Copies the reference of the String-like object that is based on the CharSequence concept specifications into this instance.
		/// </summary>
		/// <param name="string">String-like object to copy reference from</param>
		/// <returns>Reference to this instance</returns>
		constexpr StringBuffer& operator=(const CharSequence auto& string) noexcept
		{
			m_Data = string.Data();
			m_Size = string.Length();
			return *this;
		}

		/// <summary>
		/// Copies the reference of the String-like object that is based on the StdCharSequence concept specifications into this instance.
		/// </summary>
		/// <param name="string">String-like object to copy reference from</param>
		/// <returns>Reference to this instance</returns>
		constexpr StringBuffer& operator=(const StdCharSequence auto& string) noexcept
		{
			m_Data = string.data();
			m_Size = string.size();
			return *this;
		}

		/// <summary>
		/// Copies the reference of the raw string literal.
		/// </summary>
		/// <typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to copy reference from</param>
		/// <returns>Reference of this instance</returns>
		template <size_t TSize>
		constexpr StringBuffer& operator=(const char(&string)[TSize]) noexcept
		{
			constexpr size_t length = TSize - 1;
			if (length == 0)
				return *this;

			m_Data = string;
			m_Size = length;
			return *this;
		}

		/// <summary>
		/// Tests if the two buffer views are equal.
		/// </summary>
		/// <param name="left">StringBuffer to test against</param>
		/// <param name="right">StringBuffer to test against</param>
		/// <returns>True, if equal</returns>
		constexpr friend bool operator==(const StringBuffer& left, const StringBuffer& right) noexcept { return left.Equals(right); }

		/// <summary>
		/// Tests if the two buffer views are not equal.
		/// </summary>
		/// <param name="left">StringBuffer to test against</param>
		/// <param name="right">StringBuffer to test against</param>
		/// <returns>True, if not equal</returns>
		constexpr friend bool operator!=(const StringBuffer& left, const StringBuffer& right) noexcept { return !(left == right); }

		/// <summary>
		/// Prints the given StringBuffer out to the console.
		/// </summary>
		/// <param name="stream">Stream to print to the standard output</param>
		/// <param name="current">StringBuffer to print out</param>
		/// <returns>Reference of the stream</returns>
		friend std::ostream& operator<<(std::ostream& stream, const StringBuffer& current) noexcept
		{
			if (current.m_Size > 0)
				for (size_t i = 0; i < current.m_Size; i++)
					stream << current.m_Data[i];

			return stream;
		}

	private:
		/*
		 *  ============================================================
		 *	|                     Internal Helpers                     |
		 *  ============================================================
		 */


		/**
		 * \brief Creates a view of the character buffer using it's buffer bounds.
		 * \param begin Pointer to the beginning of the character buffer
		 * \param end Pointer to the end of the character buffer
		 */
		constexpr StringBuffer(const char* begin, const char* end) noexcept
			: m_Data(begin), m_Size(end - begin)
		{
		}

	private:
		const char* m_Data = nullptr;
		size_t m_Size = 0;
	};


	/*
	 *  ============================================================
	 *	|                      Global Functions                    |
	 *  ============================================================
	 */


	/// <summary>
	/// Hashes the StringBuffer to produce a unique hash code.
	/// </summary>
	/// <param name="object">StringBuffer to hash</param>
	/// <returns>Hash code as a 'size_t'</returns>
	template <>
	NODISCARD inline size_t Hash(const StringBuffer& object) noexcept
	{
		const size_t size = object.Length();
		size_t hash = 0;
		for (size_t i = 0; i < size; i++)
			hash += object[i];

		return typeid(StringBuffer).hash_code() + size + hash;
	}
}
