#include "StringBuffer.h"

#include "Common/String.h"

namespace Micro
{
	const StringBuffer StringBuffer::Empty = {};

	constexpr StringBuffer::StringBuffer(std::nullptr_t) noexcept
	{
	}

	StringBuffer::StringBuffer(const char* ptr) noexcept
		: m_Ptr(ptr), m_Size(strlen(ptr))
	{
	}

	StringBuffer::StringBuffer(const String& string) noexcept
		: m_Ptr(string.Data()), m_Size(string.Length())
	{
	}

	StringBuffer::StringBuffer(const char* begin, const char* end)
		: m_Ptr(begin), m_Size(strlen(begin) - strlen(end))
	{
	}

	// Accessors
	NODISCARD const char* StringBuffer::Data() const { return m_Ptr; }
	NODISCARD size_t StringBuffer::Size() const { return m_Size; }

	NODISCARD StringBuffer StringBuffer::Slice(const size_t start) const
	{
		if (!m_Ptr || start >= m_Size)
			throw IndexOutOfRangeException(start);

		return {m_Ptr + start};
	}

	NODISCARD StringBuffer StringBuffer::Slice(const size_t start, const size_t length) const
	{
		if (!m_Ptr || start + length > m_Size)
			throw IndexOutOfRangeException(start + length);

		return {m_Ptr + start, m_Ptr + start + length};
	}

	bool StringBuffer::IsEmpty() const
	{
		return m_Ptr == nullptr || m_Size == 0;
	}

	void StringBuffer::Clear()
	{
		m_Ptr = nullptr;
		m_Size = 0;
	}

	bool StringBuffer::Equals(const StringBuffer& other) const
	{
		return m_Ptr == other.m_Ptr;
	}

	NODISCARD char StringBuffer::operator[](const size_t index)
	{
		if (index >= m_Size)
			throw IndexOutOfRangeException(index);

		return m_Ptr[index];
	}

	NODISCARD char StringBuffer::operator[](const size_t index) const
	{
		if (index >= m_Size)
			throw IndexOutOfRangeException(index);

		return m_Ptr[index];
	}

	StringBuffer& StringBuffer::operator=(const String& other)
	{
		m_Ptr = other.Data();
		m_Size = other.Length();
		return *this;
	}

	bool operator==(const StringBuffer& left, const StringBuffer& right)
	{
		return left.Equals(right);
	}

	bool operator!=(const StringBuffer& left, const StringBuffer& right)
	{
		return !(left == right);
	}

	std::ostream& operator<<(std::ostream& stream, const StringBuffer& current)
	{
		if (current.m_Size > 0)
		{
			for (size_t i = 0; i < current.m_Size; i++)
			{
				stream << current.m_Ptr[i];
			}
		}
		else
		{
			stream << "";
		}
		return stream;
	}
}
