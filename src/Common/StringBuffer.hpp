#pragma once
#include "Common/String.hpp"

namespace Micro
{
	class StringBuffer final
	{
	public:
		// Constructors/Destructors
		constexpr StringBuffer() noexcept = default;
		constexpr StringBuffer(const StringBuffer&) noexcept = default;
		constexpr StringBuffer(StringBuffer&&) noexcept = default;

		constexpr StringBuffer(Null) noexcept
		{
		}

		StringBuffer(const char* ptr) noexcept
			: m_Ptr(ptr), m_Size(strlen(ptr))
		{
		}

		constexpr StringBuffer(const String& string) noexcept
			: m_Ptr(string.Data()), m_Size(string.Length())
		{
		}

		constexpr ~StringBuffer() noexcept = default;

		// Utility
		NODISCARD StringBuffer Slice(const size_t start) const
		{
			if (start >= m_Size)
				throw IndexOutOfRangeException(start);

			return {m_Ptr + start, m_Ptr + m_Size};
		}

		NODISCARD StringBuffer Slice(const size_t start, const size_t length) const
		{
			if (start + length > m_Size)
				throw IndexOutOfRangeException(start + length);

			return {m_Ptr + start, m_Ptr + start + length};
		}

		NODISCARD StringBuffer Trim(const char character = ' ') noexcept
		{
			size_t begin = 0;
			size_t end = 0;
			bool searchBegin = true;
			bool searchEnd = true;

			for (size_t i = 0; i < m_Size && searchBegin || searchEnd; ++i)
			{
				if (m_Ptr[i] != character && searchBegin)
				{
					searchBegin = false;
					begin = i;
				}

				if (m_Ptr[m_Size - i - 1] != character && searchEnd)
				{
					searchEnd = false;
					end = m_Size - i - 1;
				}
			}

			if (begin == 0 && end == m_Size - 1) return *this;
			if (searchBegin && searchEnd) return *this;
			if (searchBegin) return Slice(begin);
			if (searchEnd) return Slice(0, m_Size - end);
			return Slice(begin, m_Size - begin - end);
		}

		NODISCARD StringBuffer Trim(const std::same_as<char> auto ... characters) noexcept
		{
			constexpr size_t length = sizeof ...(characters);
			if (length == 0)
				return *this;

			const auto values = { characters... };
			const Sequence<char> set(values, length);

			size_t begin = 0;
			size_t end = 0;
			bool searchBegin = true;
			bool searchEnd = true;

			for (size_t i = 0; i < m_Size && searchBegin || searchEnd; ++i)
			{
				if (Contains(set, m_Ptr[i]) && searchBegin)
				{
					searchBegin = false;
					begin = i;
				}

				if (Contains(set, m_Ptr[m_Size - i - 1]) && searchEnd)
				{
					searchEnd = false;
					end = m_Size - i - 1;
				}
			}

			if (searchBegin && searchEnd) return *this;
			if (searchBegin) return Slice(begin);
			if (searchEnd) return Slice(0, ++end);
			return Slice(begin, ++end);
		}

		NODISCARD StringBuffer TrimStart(const char character = ' ') noexcept
		{
			for (size_t i = 0; i < m_Size; ++i)
				if (m_Ptr[i] != character)
					return Slice(i);

			return *this;
		}

		NODISCARD StringBuffer TrimStart(const std::same_as<char> auto... characters) noexcept
		{
			if (sizeof ...(characters) == 0)
				return *this;

			const auto values = { characters... };
			for (size_t i = 0; i < m_Size; ++i)
			{
				bool found = false;
				for (auto& c : values)
				{
					if (m_Ptr[i] == c)
					{
						found = true;
						break;
					}
				}

				if (!found)
					return Slice(i);
			}

			return *this;
		}

		NODISCARD StringBuffer TrimEnd(const char character = ' ') noexcept
		{
			size_t end = 0;
			bool searchEnd = true;

			for (size_t i = 0; i < m_Size || !searchEnd; ++i)
				if (m_Ptr[m_Size - i - 1] != character && searchEnd)
				{
					searchEnd = false;
					end = m_Size - i - 1;
				}

			if (searchEnd) return Slice(0, end);
			return *this;
		}

		NODISCARD StringBuffer TrimEnd(const std::same_as<char> auto... characters) noexcept
		{
			const size_t length = characters.size();
			if (length == 0) return *this;

			const auto values = { characters... };
			const Sequence<char> set(values, length);

			size_t end = 0;
			bool searchEnd = true;

			for (size_t i = 0; i < m_Size || !searchEnd; ++i)
				if (Micro::Contains(set, m_Ptr[m_Size - i - 1]) && searchEnd)
				{
					searchEnd = false;
					end = m_Size - i - 1;
				}

			if (searchEnd) return Slice(0, end);
			return *this;
		}

		NODISCARD constexpr bool Equals(const StringBuffer& other) const noexcept
		{
			const size_t length = MIN(other.m_Size, m_Size);
			for (size_t i = 0; i < length; i++)
				if (m_Ptr[i] != other[i])
					return false;

			return true;
		}

		template <size_t TSize>
		NODISCARD constexpr bool Equals(const char (&other)[TSize]) const noexcept
		{
			constexpr size_t length = TSize - 1;
			if (length != m_Size)
				return false;

			for (size_t i = 0; i < length; i++)
				if (m_Ptr[i] != other[i])
					return false;

			return true;
		}

		constexpr void Clear() noexcept
		{
			m_Ptr = nullptr;
			m_Size = 0;
		}

		// Accessors
		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Size == 0; }
		NODISCARD constexpr size_t Length() const noexcept { return m_Size; }
		NODISCARD constexpr const char* Data() const noexcept { return m_Ptr; }
		NODISCARD String ToString() const noexcept { return {m_Ptr, m_Ptr + m_Size}; }

		// Operator Overloads
		NODISCARD char operator[](const size_t index)
		{
			if (index >= m_Size)
				throw IndexOutOfRangeException(index);

			return m_Ptr[index];
		}

		NODISCARD char operator[](const size_t index) const
		{
			if (index >= m_Size)
				throw IndexOutOfRangeException(index);

			return m_Ptr[index];
		}

		StringBuffer& operator=(const StringBuffer& other) noexcept
		{
			m_Ptr = other.m_Ptr;
			m_Size = other.m_Size;

			return *this;
		}

		StringBuffer& operator=(StringBuffer&& other) noexcept
		{
			if (this == &other)
				return *this;

			m_Ptr = other.m_Ptr;
			m_Size = other.m_Size;

			other.Clear();

			return *this;
		}

		StringBuffer& operator=(const String& other)
		{
			m_Ptr = other.Data();
			m_Size = other.Length();
			return *this;
		}

		friend bool operator==(const StringBuffer& left, const StringBuffer& right)
		{
			return left.Equals(right);
		}

		friend bool operator!=(const StringBuffer& left, const StringBuffer& right)
		{
			return !(left == right);
		}

		friend std::ostream& operator<<(std::ostream& stream, const StringBuffer& current)
		{
			if (current.m_Size > 0)
				for (size_t i = 0; i < current.m_Size; i++)
					stream << current.m_Ptr[i];
			else
				stream << "";
			return stream;
		}

	private:
		StringBuffer(const char* begin, const char* end)
			: m_Ptr(begin), m_Size(strlen(begin) - strlen(end))
		{
		}

	private:
		const char* m_Ptr = nullptr;
		size_t m_Size = 0;
	};


	// Hash Function
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
