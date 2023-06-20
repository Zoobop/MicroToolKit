#include "StringBuffer.h"

#include "Common/String.h"

namespace mtk
{
	const StringBuffer StringBuffer::Empty = {};

	StringBuffer::StringBuffer()
		: c_PtrRef(nullptr), m_Size(0)
	{
	}

	StringBuffer::StringBuffer(const char* ref)
		: c_PtrRef(ref), m_Size(strlen(ref))
	{
	}

	StringBuffer::StringBuffer(const String& ref)
		: c_PtrRef(ref.Data()), m_Size(ref.Capacity())
	{
	}

	StringBuffer::StringBuffer(const char* startRef, const char* endRef)
		: c_PtrRef(startRef), m_Size(strlen(startRef) - strlen(endRef))
	{
	}

	// Accessors
	NODISCARD const char* StringBuffer::Data() const { return c_PtrRef; }
	NODISCARD size_t StringBuffer::Size() const { return m_Size; }

	NODISCARD StringBuffer StringBuffer::Slice(size_t start) const
	{
		if (!c_PtrRef || start >= m_Size)
		{
			DEBUG_BREAK();
		}
		return {c_PtrRef + start};
	}

	NODISCARD StringBuffer StringBuffer::Slice(size_t start, size_t length) const
	{
		if (!c_PtrRef || start + length > m_Size)
		{
			DEBUG_BREAK();
		}
		return {c_PtrRef + start, c_PtrRef + start + length};
	}

	bool StringBuffer::IsEmpty() const
	{
		return c_PtrRef == nullptr || m_Size == 0;
	}

	void StringBuffer::Clear()
	{
		c_PtrRef = nullptr;
		m_Size = 0;
	}

	bool StringBuffer::Equals(const StringBuffer& other) const
	{
		return c_PtrRef == other.c_PtrRef;
	}

	NODISCARD char StringBuffer::operator[](size_t index)
	{
		if (index >= m_Size)
		{
			DEBUG_BREAK();
		}
		return c_PtrRef[index];
	}

	NODISCARD char StringBuffer::operator[](size_t index) const
	{
		if (index >= m_Size)
		{
			DEBUG_BREAK();
		}
		return c_PtrRef[index];
	}

	StringBuffer& StringBuffer::operator=(const String& other)
	{
		c_PtrRef = other.Data();
		m_Size = other.Capacity();
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
				stream << current.c_PtrRef[i];
			}
		}
		else
		{
			stream << "";
		}
		return stream;
	}
}
