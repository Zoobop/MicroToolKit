#include "StringBuilder.h"

#include "String.h"

namespace mtk
{
	// Constants
	constexpr size_t DefaultCapacity = 16;

	// Constructors/Destructors
	StringBuilder::StringBuilder(size_t size) { Allocate(size); }

	// Accessors
	bool StringBuilder::IsEmpty() const { return m_Data == nullptr || m_Size == 0; }
	constexpr size_t StringBuilder::Size() const { return m_Size; }
	constexpr const char* StringBuilder::Data() const { return m_Data; }
	constexpr size_t StringBuilder::Capacity() const { return m_Capacity; }
	char* StringBuilder::ToCharArray() const { return m_Data; }
	String StringBuilder::ToString() const { return {m_Data, m_Size}; }

	// Utility
	void StringBuilder::SyncSize() { m_Size = strlen(m_Data); }

	StringBuilder& StringBuilder::Append(const String& string)
	{
		const size_t length = string.Length();
		if (length == 0) return *this;

		if (m_Data == nullptr || m_Size == 0)
		{
			Allocate(MAX(length, DefaultCapacity));
			memcpy_s(m_Data, m_Capacity + 1, string.Data(), length);
		}
		else
		{
			// Reallocate, if necessary
			const size_t newSize = m_Size + length;
			if (newSize > m_Capacity)
				Reallocate(m_Capacity + (m_Capacity / 2));

			// Set new data
			for (size_t i = m_Size; i < newSize; i++)
				m_Data[i] = string[i - m_Size];
		}

		m_Size += length;
		return *this;
	}

	StringBuilder& StringBuilder::Append(const char* ptr)
	{
		const size_t length = strlen(ptr);
		if (length == 0) return *this;

		if (m_Data == nullptr || m_Size == 0)
		{
			Allocate(MAX(length, DefaultCapacity));
			memcpy_s(m_Data, m_Capacity + 1, ptr, length);
		}
		else
		{
			// Reallocate, if necessary
			const size_t newSize = m_Size + length;
			if (newSize > m_Capacity)
				Reallocate(m_Capacity + (m_Capacity / 2));

			// Set new data
			for (size_t i = m_Size; i < newSize; i++)
				m_Data[i] = ptr[i - m_Size];
		}

		m_Size += length;
		return *this;
	}

	StringBuilder& StringBuilder::Append(bool boolean)
	{
		return Append(boolean ? "true" : "false");
	}

	StringBuilder& StringBuilder::Append(char character)
	{
		constexpr size_t length = 1;
		if (m_Data == nullptr || m_Size == 0)
		{
			Allocate(MAX(length, DefaultCapacity));
			memcpy_s(m_Data, m_Capacity + 1, &character, length);
		}
		else
		{
			// Reallocate, if necessary
			if (const size_t newSize = m_Size + length; newSize > m_Capacity)
				Reallocate(m_Capacity + (m_Capacity / 2));

			// Set new data
			m_Data[m_Size] = character;
		}

		m_Size += length;
		return *this;
	}

	StringBuilder& StringBuilder::Append(int32_t integer)
	{
		return Append(mtk::ToString(integer));
	}

	StringBuilder& StringBuilder::Append(int64_t integer)
	{
		return Append(mtk::ToString(integer));
	}

	void StringBuilder::Allocate(size_t capacity)
	{
		if (capacity == 0) return;

		const size_t length = capacity + 1;
		m_Data = new char[length];
		m_Data[capacity] = 0;
		m_Capacity = capacity;
	}

	void StringBuilder::Reallocate(size_t capacity)
	{
		if (capacity == 0) return;
		const size_t length = capacity + 1;

		// Reallocation
		if (m_Data != nullptr)
		{
			//const auto newBlock = new char[length];
			//memmove_s(newBlock, length, m_Data, m_Capacity);
			//delete[] m_Data;

			m_Data = static_cast<char*>(realloc(m_Data, length));
			m_Data[capacity] = 0;
			m_Capacity = capacity;
			return;
		}

		// Allocate initial memory
		Allocate(capacity);
	}
}
