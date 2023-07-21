#pragma once
#include "Common/String.hpp"

namespace Micro
{
	class StringBuilder final
	{
	public:
		// Constructors/Destructors
		constexpr StringBuilder() noexcept = default;
		constexpr StringBuilder(const StringBuilder&) noexcept = default;
		constexpr StringBuilder(StringBuilder&&) noexcept = default;

		explicit StringBuilder(const size_t size) noexcept
		{
			Allocate(size);
		}

		constexpr ~StringBuilder() noexcept
		{
			delete[] m_Data;
			m_Size = 0;
			m_Capacity = 0;
		}

		// Accessors
		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Size == 0; }
		NODISCARD constexpr size_t Length() const noexcept { return m_Size; }
		NODISCARD constexpr const char* Data() const noexcept { return m_Data; }
		NODISCARD constexpr size_t Capacity() const noexcept { return m_Capacity; }
		NODISCARD char* ToCharArray() const noexcept { return m_Data; }
		NODISCARD String ToString() const noexcept { return {m_Data, m_Data + m_Size}; }

		// Utility
		StringBuilder& Append(const CharSequence auto& string) noexcept
		{
			const size_t length = string.Length();
			if (length == 0) return *this;

			if (m_Data == nullptr)
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

		StringBuilder& Append(const char* ptr) noexcept
		{
			const size_t length = strlen(ptr);
			if (length == 0) return *this;

			if (m_Data == nullptr)
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

		StringBuilder& Append(const bool boolean) noexcept
		{
			return Append(boolean ? "true" : "false");
		}

		StringBuilder& Append(const char character) noexcept
		{
			constexpr size_t length = 1;
			if (m_Data == nullptr)
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

		StringBuilder& Append(const std::integral auto integer) noexcept
		{
			return Append(Micro::ToString(integer));
		}

		StringBuilder& Remove(const size_t start, const size_t length = 1) noexcept
		{
			const size_t size = start + length;
			for (size_t i = start; i < size; i++)
				m_Data[i] = -1;

			// Shift items down
			ShiftLeft(m_Data, m_Size, start + 1, length);

			m_Size -= length;
			return *this;
		}

		constexpr void Clear() noexcept
		{
			//delete[] m_Data;
			m_Size = 0;
		}

		// Operator Overloads
		StringBuilder& operator=(const StringBuilder& other)
		{
			if (this == &other)
				return *this;

			Clear();

			Allocate(other.m_Capacity);
			memcpy_s(m_Data, m_Capacity + 1, other.m_Data, other.m_Capacity);
			return *this;
		}

		StringBuilder& operator=(StringBuilder&& other) noexcept
		{
			if (this == &other)
				return *this;

			Clear();

			m_Data = other.m_Data;
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;

			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;

			return *this;
		}

	private:
		void Allocate(const size_t capacity)
		{
			if (capacity == 0) return;

			const size_t length = capacity + 1;
			m_Data = new char[length];
			m_Data[capacity] = 0;
			m_Capacity = capacity;
		}

		void Reallocate(const size_t capacity)
		{
			if (capacity == 0) return;
			const size_t length = capacity + 1;

			// Reallocation
			if (m_Data != nullptr)
			{
				m_Data = static_cast<char*>(realloc(m_Data, length));
				m_Data[capacity] = 0;
				m_Capacity = capacity;
				return;
			}

			// Allocate initial memory
			Allocate(capacity);
		}

	private:
		char* m_Data = nullptr;
		size_t m_Size = 0;
		size_t m_Capacity = 0;

		constexpr static size_t DefaultCapacity = 16;
	};
}
