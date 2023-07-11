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

			return {m_Ptr + start};
		}

		NODISCARD StringBuffer Slice(const size_t start, const size_t length) const
		{
			if (start + length > m_Size)
				throw IndexOutOfRangeException(start + length);

			return {m_Ptr + start, m_Ptr + start + length};
		}

		NODISCARD constexpr bool Equals(const StringBuffer& other) const noexcept
		{
			return m_Ptr == other.m_Ptr;
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
		NODISCARD String ToString() const noexcept { return {m_Ptr, m_Size}; }

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
			if (this == &other)
				return *this;

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
}
