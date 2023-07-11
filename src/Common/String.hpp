#pragma once

#include "Collections/Base/Sequence.hpp"
#include "Utility/Result.hpp"

namespace Micro
{
	template <typename T>
	concept CharSequence = requires(T string)
	{
		{ string.Data() } -> std::convertible_to<const char*>;
		{ string.Length() } -> std::convertible_to<size_t>;
		{ string.IsEmpty() } -> std::convertible_to<size_t>;
		{ string.Clear() } -> std::convertible_to<void>;
		{ string.operator[](0) } -> std::convertible_to<char>;
	};

	template <typename T>
	concept StdCharSequence = requires(T string)
	{
		{ string.data() } -> std::convertible_to<const char*>;
		{ string.size() } -> std::convertible_to<size_t>;
		{ string.empty() } -> std::convertible_to<bool>;
		{ string.clear() } -> std::convertible_to<void>;
	};

	class String final
	{
	public:
		// Constructors/Destructors
		constexpr String() noexcept = default;

		constexpr String(Null) noexcept
		{
		}

		String(const String& other)
		{
			Allocate(other.m_Size);

			StringCopy(other);
		}

		String(String&& other) noexcept
			: m_Data(other.m_Data), m_Size(other.m_Size)
		{
			other.m_Data = nullptr;
			other.m_Size = 0;

			m_Data[m_Size] = 0;
		}

		String(const CharSequence auto& string)
		{
			const size_t length = string.Length();
			if (length == 0)
				return;

			Allocate(length);
			StringCopy(string);
		}

		String(const StdCharSequence auto& string)
		{
			const size_t length = string.size();
			if (length == 0)
				return;

			Allocate(length);
			StringCopy(string);
		}

		String(const char character, const size_t count)
		{
			Allocate(count);

			for (size_t i = 0; i < count; i++)
				m_Data[i] = character;

			m_Data[m_Size] = 0;
		}

		String(const char* begin, const size_t count)
		{
			if (count == 0) return;

			Allocate(count);
			StringCopy(begin);
		}

		String(const char* begin, const char* end)
		{
			const size_t size = strlen(begin) - strlen(end) + 1;
			if (size == 0)
				return;

			m_Data = new char[size];
			for (auto iter = const_cast<char*>(begin); iter != end; ++iter)
				m_Data[m_Size++] = *iter;
			m_Data[m_Size] = 0;
		}

		String(const char* ptr)
		{
			const size_t length = strlen(ptr);
			if (length == 0)
				return;

			Allocate(length);
			StringCopy(ptr);
		}

		String(char* ptr)
		{
			const size_t length = strlen(ptr);
			if (length == 0)
				return;

			Allocate(length);
			StringCopy(ptr);
		}

		explicit String(const char character)
		{
			Allocate(1);
			m_Data[0] = character;
		}

		constexpr ~String() noexcept
		{
			delete[] m_Data;

			m_Data = nullptr;
			m_Size = 0;
		}

		// Accessors
		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Size == 0; }
		NODISCARD constexpr size_t Length() const noexcept { return m_Size; }
		NODISCARD constexpr const char* Data() const noexcept { return m_Data; }
		NODISCARD constexpr char* Data() noexcept { return m_Data; }

		// Utility
		String& Append(const String& string)
		{
			const size_t length = string.m_Size;
			if (length == 0) return *this;

			if (m_Data == nullptr)
			{
				Allocate(length);
				memcpy_s(m_Data, length + 1, string.m_Data, string.m_Size);
			}
			else
			{
				Reallocate(m_Size + length);
				strcat_s(m_Data, m_Size + 1, string.m_Data);
			}

			return *this;
		}

		String& Append(String&& string) noexcept
		{
			if (string.m_Size == 0) return *this;

			if (m_Data == nullptr)
			{
				m_Data = string.m_Data;
				m_Size = string.m_Size;
			}
			else
			{
				Reallocate(m_Size + string.m_Size);
				strcat_s(m_Data, m_Size + 1, string.m_Data);
			}

			string.m_Data = nullptr;
			string.m_Size = 0;
			return *this;
		}

		String& Append(const CharSequence auto& string)
		{
			const size_t length = string.Length();
			if (length == 0) return *this;

			if (m_Data == nullptr)
			{
				Allocate(length);
				StringCopy(string);
			}
			else
			{
				Reallocate(m_Size + length);
				strcat_s(m_Data, m_Size + 1, string.Data());
			}

			return *this;
		}

		String& Append(const StdCharSequence auto& string)
		{
			const size_t length = string.size();
			if (length == 0) return *this;

			if (m_Data == nullptr)
			{
				Allocate(length);
				StringCopy(string);
			}
			else
			{
				Reallocate(m_Size + length);
				strcat_s(m_Data, m_Size + 1, string.data());
			}

			return *this;
		}

		String& Append(char* string)
		{
			const size_t length = strlen(string);
			if (length == 0) return *this;

			if (m_Data == nullptr)
			{
				Allocate(length);
				StringCopy(string);
			}
			else
			{
				Reallocate(m_Size + length);
				strcat_s(m_Data, m_Size + 1, string);
			}

			return *this;
		}

		String& Append(const char* string)
		{
			const size_t length = strlen(string);
			if (length == 0) return *this;

			if (m_Data == nullptr)
			{
				Allocate(length);
				StringCopy(string);
			}
			else
			{
				Reallocate(m_Size + length);
				strcat_s(m_Data, m_Size + 1, string);
			}

			return *this;
		}

		String& Append(const char character)
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

		void Insert(const char character, const size_t index)
		{
			if (index >= m_Size)
				throw ArgumentOutOfRangeException(NAMEOF(index), index);

			m_Data[index] = character;
		}

		NODISCARD Result<size_t> IndexOf(const char character) const noexcept
		{
			for (size_t i = 0; i < m_Size; ++i)
				if (m_Data[i] == character)
					return Result(i);

			return Result<size_t>::Empty();
		}

		NODISCARD Result<size_t> IndexOf(const CharSequence auto& string, const size_t startIndex,
		                                 const size_t length) const noexcept
		{
			if (string.IsEmpty() || string.Length() > m_Size)
				return Result<size_t>::Empty();
			if (startIndex >= m_Size || length > m_Size - startIndex)
				return Result<size_t>::Empty();

			for (size_t i = 0; i < length; ++i)
				if (m_Data[i] == string[0])
				{
					if (i + string.Length() < m_Size)
					{
						if (memcmp(m_Data + i, string.Data(), string.Length()) == 0)
							return Result(i);

						continue;
					}

					break;
				}

			return Result<size_t>::Empty();
		}

		NODISCARD Result<size_t> LastIndexOf(const char character) const noexcept
		{
			for (size_t i = m_Size; i > 0; i--)
				if (m_Data[i-1] == character)
					return Result(i-1);

			return Result<size_t>::Empty();
		}

		NODISCARD Result<size_t> LastIndexOf(const CharSequence auto& string, const size_t startIndex, const size_t length) const noexcept
		{
			if (string.IsEmpty() || string.Length() > m_Size)
				return Result<size_t>::Empty();
			if (startIndex >= m_Size || length > m_Size - startIndex)
				return Result<size_t>::Empty();

			for (size_t i = m_Size; i > 0; i--)
				if (m_Data[i - 1] == string[0])
				{
					if (string.Length() - i < m_Size)
					{
						if (memcmp(m_Data + i, string.Data(), string.Length()) == 0)
							return Result(i);

						continue;
					}

					break;
				}

			return Result<size_t>::Empty();
		}

		NODISCARD String Replace(const char oldChar, const char newChar) const noexcept
		{
			if (IsEmpty()) 
				return *this;

			const auto string = new char[m_Size + 1];
			for (size_t i = 0; i < m_Size; ++i)
			{
				char character = m_Data[i];
				if (m_Data[i] == oldChar)
					character = newChar;

				string[i] = character;
			}

			string[m_Size] = 0;
			return string;
		}

		NODISCARD String Replace(const CharSequence auto& oldString, char newChar) const
		{
			const size_t length = oldString.Length();
			if (IsEmpty() || length > m_Size) 
				return *this;



			return *this;
		}

		NODISCARD String Replace(const CharSequence auto& oldString, const CharSequence auto& newString) const
		{
			return *this;
		}

		NODISCARD String Replace(const char oldChar, const CharSequence auto& newString) const
		{
			return *this;
		}

		NODISCARD String Substring(const size_t start) const noexcept
		{
			const size_t size = m_Size - start;
			if (size == 0 || size > m_Size) 
				return {};

			const auto string = new char[size + 1];
			memcpy_s(string, size + 1, m_Data + start, size + 1);
			string[size] = 0;
			return string;
		}

		NODISCARD String Substring(const size_t start, const size_t length) const noexcept
		{
			const size_t size = length;
			if (start >= m_Size || length > m_Size - start) return {};

			const auto string = new char[size + 1];
			memcpy_s(string, size + 1, m_Data + start, size + 1);
			string[size] = 0;
			return string;
		}

		NODISCARD bool Equals(const String& other) const noexcept
		{
			return strcmp(m_Data, other.m_Data) == 0;
		}

		NODISCARD bool Equals(const CharSequence auto& other) const noexcept
		{
			return strcmp(m_Data, other.Data()) == 0;
		}

		NODISCARD bool Equals(const StdCharSequence auto& other) const noexcept
		{
			return strcmp(m_Data, other.data()) == 0;
		}

		NODISCARD bool Equals(const char* other) const noexcept
		{
			return strcmp(m_Data, other) == 0;
		}

		NODISCARD bool Equals(const char other) const noexcept
		{
			return m_Data[0] == other;
		}

		NODISCARD bool Contains(const String& string) const noexcept
		{
			const size_t length = string.Length();
			if (length == 0) return false;

			for (size_t i = 0, charIndex = 0; i < m_Size; ++i)
				while (m_Data[i] == string[charIndex])
				{
					++i;
					++charIndex;

					if (charIndex == length) return true;
				}

			return false;
		}

		NODISCARD bool Contains(const CharSequence auto& string) const
		{
			const size_t length = string.Length();
			if (length == 0) return false;

			for (size_t i = 0, charIndex = 0; i < m_Size; ++i)
				while (m_Data[i] == string[charIndex])
				{
					++i;
					++charIndex;

					if (charIndex == length) return true;
				}

			return false;
		}

		NODISCARD bool Contains(const StdCharSequence auto& string) const
		{
			const size_t length = string.size();
			if (length == 0) return false;

			for (size_t i = 0, charIndex = 0; i < m_Size; ++i)
				while (m_Data[i] == string[charIndex])
				{
					++i;
					++charIndex;

					if (charIndex == length) return true;
				}

			return false;
		}

		NODISCARD bool Contains(const char* string) const noexcept
		{
			const size_t length = strlen(string);
			if (length == 0) return false;

			for (size_t i = 0, charIndex = 0; i < m_Size; ++i)
				while (m_Data[i] == string[charIndex])
				{
					++i;
					++charIndex;

					if (charIndex == length) return true;
				}

			return false;
		}

		NODISCARD bool Contains(const char character) const noexcept
		{
			for (size_t i = 0; i < m_Size; ++i)
				if (m_Data[i] == character) 
					return true;

			return false;
		}

		NODISCARD bool StartsWith(const String& string) const noexcept
		{
			const size_t length = string.Length();
			if (IsEmpty() || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
				if (m_Data[i] != string[i])
					return false;

			return true;
		}

		NODISCARD bool StartsWith(const CharSequence auto& string) const noexcept
		{
			const size_t length = string.Length();
			if (IsEmpty() || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
				if (m_Data[i] != string[i])
					return false;

			return true;
		}

		NODISCARD bool StartsWith(const StdCharSequence auto& string) const noexcept
		{
			const size_t length = string.size();
			if (m_Size == 0 || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
				if (m_Data[i] != string[i])
					return false;

			return true;
		}

		NODISCARD bool StartsWith(const char* string) const noexcept
		{
			const size_t length = strlen(string);
			if (m_Size == 0 || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
				if (m_Data[i] != string[i])
					return false;

			return true;
		}

		NODISCARD bool StartsWith(const char character) const noexcept
		{
			if (m_Size == 0) 
				return false;

			return m_Data[0] == character;
		}

		NODISCARD bool EndsWith(const String& string) const noexcept
		{
			const size_t length = string.Length();
			if (m_Size == 0 || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
			{
				const size_t index = m_Size - i - 1;
				if (m_Data[index] != string[index])
					return false;
			}

			return true;
		}

		NODISCARD bool EndsWith(const CharSequence auto& string) const noexcept
		{
			const size_t length = string.Length();
			if (m_Size == 0 || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
			{
				const size_t index = m_Size - i - 1;
				if (m_Data[index] != string[index])
					return false;
			}

			return true;
		}

		NODISCARD bool EndsWith(const StdCharSequence auto& string) const noexcept
		{
			const size_t length = string.size();
			if (m_Size == 0 || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
			{
				const size_t index = m_Size - i - 1;
				if (m_Data[index] != string[index])
					return false;
			}

			return true;
		}

		NODISCARD bool EndsWith(const char* string) const noexcept
		{
			const size_t length = strlen(string);
			if (m_Size == 0 || m_Size < length) return false;

			for (size_t i = 0; i < length; ++i)
			{
				const size_t index = m_Size - i - 1;
				if (m_Data[index] != string[index])
					return false;
			}

			return true;
		}

		NODISCARD bool EndsWith(const char character) const noexcept
		{
			if (m_Size == 0)
				return false;

			return m_Data[m_Size - 1] == character;
		}

		NODISCARD String Trim(const char character) const noexcept
		{
			size_t begin = 0;
			size_t end = 0;

			bool searchBegin = true;
			bool searchEnd = true;

			for (size_t i = 0; i < m_Size && searchBegin || searchEnd; ++i)
			{
				if (m_Data[i] != character && searchBegin)
				{
					searchBegin = false;
					begin = i;
				}

				if (m_Data[m_Size - i - 1] != character && searchEnd)
				{
					searchEnd = false;
					end = m_Size - i - 1;
				}
			}

			if (begin == 0 && end == m_Size - 1) return *this;
			if (searchBegin && searchEnd) return *this;
			if (searchBegin) return Substring(begin);
			if (searchEnd) return Substring(0, m_Size - end);
			return Substring(begin, m_Size - begin - end);
		}

		NODISCARD String Trim(std::convertible_to<char> auto ... characters) const noexcept
		{
			const size_t length = sizeof ...(characters);
			if (length == 0) 
				return *this;

			const auto values = { characters ... };
			const Sequence<char> set(values, length);

			size_t begin = 0;
			size_t end = 0;

			bool searchBegin = true;
			bool searchEnd = true;

			for (size_t i = 0; i < m_Size && searchBegin || searchEnd; ++i)
			{
				if (Micro::Contains(set, m_Data[i]) && searchBegin)
				{
					searchBegin = false;
					begin = i;
				}

				if (Micro::Contains(set, m_Data[m_Size - i - 1]) && searchEnd)
				{
					searchEnd = false;
					end = m_Size - i - 1;
				}
			}

			if (searchBegin && searchEnd) return *this;
			if (searchBegin) return Substring(begin);
			if (searchEnd) return Substring(0, ++end);
			return Substring(begin, ++end);
		}

		NODISCARD String TrimStart(const char character) const noexcept
		{
			size_t begin = 0;
			bool searchBegin = true;

			for (size_t i = 0; i < m_Size || !searchBegin; ++i)
				if (m_Data[i] != character && searchBegin)
				{
					searchBegin = false;
					begin = i;
				}

			if (searchBegin) return Substring(begin);
			return *this;
		}

		NODISCARD String TrimStart(std::convertible_to<char> auto ... characters) const noexcept
		{
			const size_t length = characters.size();
			if (length == 0) return *this;

			const auto values = { characters ... };
			const Sequence<char> set(values, length);

			size_t begin = 0;
			bool searchBegin = true;

			for (size_t i = 0; i < m_Size; ++i)
				if (Micro::Contains(set, m_Data[i]) && searchBegin)
				{
					searchBegin = false;
					begin = i;
				}

			if (searchBegin) return Substring(begin);
			return *this;
		}

		NODISCARD String TrimEnd(const char character) const noexcept
		{
			size_t end = 0;
			bool searchEnd = true;

			for (size_t i = 0; i < m_Size || !searchEnd; ++i)
				if (m_Data[m_Size - i - 1] != character && searchEnd)
				{
					searchEnd = false;
					end = m_Size - i - 1;
				}

			if (searchEnd) return Substring(0, end);
			return *this;
		}

		NODISCARD String TrimEnd(std::convertible_to<char> auto ... characters) const noexcept
		{
			const size_t length = characters.size();
			if (length == 0) return *this;

			const auto values = { characters ... };
			const Sequence<char> set(values, length);

			size_t end = 0;
			bool searchEnd = true;

			for (size_t i = 0; i < m_Size || !searchEnd; ++i)
				if (Micro::Contains(set, m_Data[m_Size - i - 1]) && searchEnd)
				{
					searchEnd = false;
					end = m_Size - i - 1;
				}

			if (searchEnd) return Substring(0, end);
			return *this;
		}

		constexpr void Clear() noexcept
		{
			delete[] m_Data;
			m_Data = nullptr;
			m_Size = 0;
		}

		// Operator Overloads
		constexpr operator const char*() const noexcept { return m_Data; }

		constexpr operator std::string() const noexcept { return m_Data; }

		constexpr operator Sequence<char>() const noexcept { return {m_Data, m_Size}; }

		NODISCARD char operator[](const size_t index)
		{
			if (index >= m_Size)
				throw IndexOutOfRangeException(index);

			return m_Data[index];
		}

		NODISCARD char operator[](const size_t index) const
		{
			if (index >= m_Size)
				throw IndexOutOfRangeException(index);

			return m_Data[index];
		}

		String& operator=(const String& other) noexcept
		{
			if (this == &other)
				return *this;

			const size_t length = other.m_Size;
			if (!m_Data)
			{
				Allocate(length);
				StringCopy(other);
			}
			else
			{
				if (length != m_Size)
					Reallocate(length);

				StringCopy(other);
			}

			return *this;
		}

		String& operator=(String&& other) noexcept
		{
			m_Data = other.m_Data;
			m_Size = other.m_Size;

			other.m_Data = nullptr;
			other.m_Size = 0;

			return *this;
		}

		String& operator=(const CharSequence auto& other)
		{
			const size_t size = other.Length();
			if (size == 0) return *this;

			if (m_Data != nullptr)
				Reallocate(size);
			else
				Allocate(size);

			StringCopy(other);
			return *this;
		}

		String& operator=(const StdCharSequence auto& other)
		{
			const size_t size = other.size();
			if (size == 0) return *this;

			if (m_Data != nullptr)
				Reallocate(size);
			else
				Allocate(size);

			StringCopy(other);
			return *this;
		}

		String& operator=(const char* other)
		{
			const size_t size = strlen(other);
			if (size == 0) return *this;

			if (m_Data != nullptr)
				Reallocate(size);
			else
				Allocate(size);

			StringCopy(other);
			return *this;
		}

		String& operator=(char* other)
		{
			const size_t size = strlen(other);
			if (size == 0) return *this;

			if (m_Data != nullptr)
				Reallocate(size);
			else
				Allocate(size);

			StringCopy(other);
			return *this;
		}

		String& operator+=(const String& other) { return Append(other); }

		String& operator+=(String&& other) noexcept { return Append(std::move(other)); }

		String& operator+=(const CharSequence auto& other) { return Append(other); }

		String& operator+=(const StdCharSequence auto& other) { return Append(other); }

		String& operator+=(const char* other) { return Append(other); }

		String& operator+=(char* other) { return Append(other); }

		String& operator+=(const char other) { return Append(other); }

		friend String operator+(const String& left, const String& right)
		{
			const size_t leftSize = left.m_Size;
			const size_t rightSize = right.m_Size;

			if (leftSize + rightSize == 0) return {};
			if (leftSize == 0) return right;
			if (rightSize == 0) return left;

			const size_t size = leftSize + rightSize + 1;
			const auto concat = new char[size];
			memcpy_s(concat, size, left.m_Data, leftSize + 1);
			strcat_s(concat, size, right.m_Data);
			concat[size] = 0;
			return concat;
		}

		friend String operator+(const String& left, const char* right)
		{
			const size_t leftSize = left.m_Size;
			const size_t rightSize = strlen(right);

			if (leftSize + rightSize == 0) return {};
			if (leftSize == 0) return right;
			if (rightSize == 0) return left;

			const size_t size = leftSize + rightSize + 1;
			const auto concat = new char[size];
			memcpy_s(concat, size, left.m_Data, leftSize + 1);
			strcat_s(concat, size, right);
			concat[size] = 0;
			return concat;
		}

		friend String operator+(const char* left, const String& right)
		{
			const size_t leftSize = strlen(left);
			const size_t rightSize = right.m_Size;

			if (leftSize + rightSize == 0) return {};
			if (leftSize == 0) return right;
			if (rightSize == 0) return left;

			const size_t size = leftSize + rightSize + 1;
			const auto concat = new char[size];
			memcpy_s(concat, size, left, leftSize + 1);
			strcat_s(concat, size, right.m_Data);
			concat[size] = 0;
			return concat;
		}

		friend bool operator==(const String& left, const String& right) noexcept
		{
			return left.Equals(right);
		}

		friend bool operator!=(const String& left, const String& right) noexcept { return !(left == right); }

		friend std::ostream& operator<<(std::ostream& stream, const String& current)
		{
			if (current.m_Size > 0)
				stream << current.m_Data;
			else
				stream << "";
			return stream;
		}

	protected:
		void Allocate(const size_t capacity)
		{
			if (capacity == 0) return;

			const size_t length = capacity + 1;
			m_Data = new char[length];
			m_Size = capacity;
			m_Data[capacity] = 0;
		}

		void Reallocate(const size_t capacity)
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

		void StringCopy(const String& other) const noexcept
		{
			memcpy_s(m_Data, m_Size + 1, other.m_Data, other.m_Size);
			m_Data[m_Size] = 0;
		}

		void StringCopy(const CharSequence auto& other) const noexcept
		{
			memcpy_s(m_Data, m_Size + 1, other.Data(), other.Length());
			m_Data[m_Size] = 0;
		}

		void StringCopy(const StdCharSequence auto& other) const noexcept
		{
			memcpy_s(m_Data, m_Size + 1, other.data(), other.size());
			m_Data[m_Size] = 0;
		}

		void StringCopy(const char* ptr) const noexcept
		{
			memcpy_s(m_Data, m_Size + 1, ptr, strlen(ptr));
			m_Data[m_Size] = 0;
		}

	private:
		char* m_Data = nullptr;
		size_t m_Size = 0;
	};


	// Hash Function
	template <>
	NODISCARD inline size_t Hash(const String& object) noexcept
	{
		const size_t size = object.Length();
		size_t hash = 0;
		for (size_t i = 0; i < size; i++)
			hash += object[i];

		return typeid(String).hash_code() + size + hash;
	}

	template <typename TElem, typename T>
	NODISCARD String IntToString(T integral)
	{
		static_assert(std::is_integral_v<T>, "T must be integral");
		using UType = std::make_unsigned_t<T>;
		TElem buffer[21];
		TElem* const end = std::end(buffer);
		TElem* next = end;
		const auto uValue = static_cast<UType>(integral);
		if (integral < 0)
		{
			next = std::_UIntegral_to_buff(next, 0 - uValue);
			*--next = '-';
		}
		else
			next = std::_UIntegral_to_buff(next, uValue);

		return String(next, end);
	}

	template <typename TElem, typename T>
	NODISCARD String UIntToString(T integral)
	{
		static_assert(std::is_integral_v<T>, "T must be integral");
		static_assert(std::is_unsigned_v<T>, "T must be unsigned");
		TElem buffer[21];
		TElem* const end = std::end(buffer);
		TElem* const next = std::_UIntegral_to_buff(end, integral);
		return String(next, end);
	}

	NODISCARD inline String ToString(bool boolean) { return {boolean ? "true" : "false"}; }
	NODISCARD inline String ToString(char character) { return String(character); }
	NODISCARD inline String ToString(int integral) { return IntToString<char>(integral); }
	NODISCARD inline String ToString(unsigned int integral) { return UIntToString<char>(integral); }
	NODISCARD inline String ToString(long integral) { return IntToString<char>(integral); }
	NODISCARD inline String ToString(unsigned long integral) { return UIntToString<char>(integral); }
	NODISCARD inline String ToString(long long integral) { return IntToString<char>(integral); }
	NODISCARD inline String ToString(unsigned long long integral) { return UIntToString<char>(integral); }
	NODISCARD inline String ToString(double integral)
	{
		const auto size = static_cast<size_t>(_scprintf("%f", integral));
		String string('\0', size);
		auto _ = sprintf_s(string.Data(), size + 1, "%f", integral);
		return string;
	}

	NODISCARD inline String ToString(float integral) { return ToString(static_cast<double>(integral)); }
}
