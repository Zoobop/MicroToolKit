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

		constexpr explicit StringBuilder(const CharSequence auto& string) noexcept
		{
			Allocate(MAX(string.Length(), DefaultCapacity));
			InternalCopy(string.Data(), string.Length());
		}

		constexpr explicit StringBuilder(const StdCharSequence auto& string) noexcept
		{
			Allocate(MAX(string.size(), DefaultCapacity));
			InternalCopy(string.data(), string.size());
		}

		constexpr explicit StringBuilder(const size_t size) noexcept
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
		NODISCARD constexpr char* ToCharArray() const noexcept { return m_Data; }
		NODISCARD constexpr String ToString() const noexcept { return {m_Data, m_Size}; }

		// Utility
		constexpr StringBuilder& Append(const CharSequence auto& string) noexcept
		{
			const size_t length = string.Length();
			if (length == 0) 
				return *this;

			if (m_Data == nullptr)
			{
				Allocate(MAX(length, DefaultCapacity));
				InternalCopy(string.Data(), length);
			}
			else
			{
				// Reallocate, if necessary
				const size_t newSize = m_Size + length;
				if (newSize > m_Capacity)
					Reallocate(m_Capacity + (m_Capacity / 2));

				// Set new data
				InternalConcat(m_Size, string.Data(), newSize);
			}

			m_Size += length;
			return *this;
		}

		constexpr StringBuilder& Append(const StdCharSequence auto& string) noexcept
		{
			const size_t length = string.size();
			if (length == 0)
				return *this;

			if (m_Data == nullptr)
			{
				Allocate(MAX(length, DefaultCapacity));
				InternalCopy(string.data(), length);
			}
			else
			{
				// Reallocate, if necessary
				const size_t newSize = m_Size + length;
				if (newSize > m_Capacity)
					Reallocate(m_Capacity + (m_Capacity / 2));

				// Set new data
				InternalConcat(m_Size, string.data(), newSize);
			}

			m_Size += length;
			return *this;
		}

		template <size_t TSize>
		constexpr StringBuilder& Append(const char (&ptr)[TSize]) noexcept
		{
			constexpr size_t length = TSize - 1;
			if constexpr (length == 0)
				return *this;

			if (m_Data == nullptr)
			{
				Allocate(MAX(length, DefaultCapacity));
				InternalCopy(ptr, length);
			}
			else
			{
				// Reallocate, if necessary
				const size_t newSize = m_Size + length;
				if (newSize > m_Capacity)
					Reallocate(m_Capacity + (m_Capacity / 2));

				// Set new data
				InternalConcat(m_Size, ptr, newSize);
			}

			m_Size += length;
			return *this;
		}

		constexpr StringBuilder& Append(const char* ptr, const size_t length) noexcept
		{
			if (length == 0) 
				return *this;

			if (m_Data == nullptr)
			{
				Allocate(MAX(length, DefaultCapacity));
				InternalCopy(ptr, length);
			}
			else
			{
				// Reallocate, if necessary
				const size_t newSize = m_Size + length;
				if (newSize > m_Capacity)
					Reallocate(m_Capacity + (m_Capacity / 2));

				// Set new data
				InternalConcat(m_Size, ptr, newSize);
			}

			m_Size += length;
			return *this;
		}

		constexpr StringBuilder& Append(const bool boolean) noexcept
		{
			return boolean ? Append("true") : Append("false");
		}

		constexpr StringBuilder& Append(const char character) noexcept
		{
			constexpr size_t length = 1;
			if (m_Data == nullptr)
			{
				Allocate(MAX(length, DefaultCapacity));
				InternalCopy(&character, length);
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

		constexpr StringBuilder& Append(const std::integral auto integer) noexcept
		{
			return Append(Micro::ToString(integer));
		}

		constexpr StringBuilder& Remove(const size_t start, const size_t length = 1) noexcept
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
		NODISCARD constexpr char& operator[](const size_t index)
		{
			if (index >= m_Size)
				throw IndexOutOfRangeException(index);

			return m_Data[index];
		}

		NODISCARD constexpr char& operator[](const size_t index) const
		{
			if (index >= m_Size)
				throw IndexOutOfRangeException(index);

			return m_Data[index];
		}

		constexpr StringBuilder& operator=(const StringBuilder& other)
		{
			if (this == &other)
				return *this;

			const size_t length = other.m_Size;
			if (!m_Data)
			{
				Allocate(length);
			}
			else
			{
				if (length != m_Size)
					Reallocate(length);
			}

			InternalCopy(other.m_Data, length);
			return *this;
		}

		constexpr StringBuilder& operator=(StringBuilder&& other) noexcept
		{
			if (this == &other)
				return *this;

			delete[] m_Data;

			m_Data = other.m_Data;
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;

			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;

			return *this;
		}

	private:
		constexpr void Allocate(const size_t capacity) noexcept
		{
			if (capacity == 0) return;

			const size_t length = capacity + 1;
			m_Data = new char[length];
			m_Data[capacity] = 0;
			m_Capacity = capacity;
		}

		constexpr void Reallocate(const size_t capacity) noexcept
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

		constexpr void InternalCopy(const char* ptr, const size_t size) const noexcept
		{
			for (size_t i = 0; i < size; i++)
				new(&m_Data[i]) char(ptr[i]);

			m_Data[m_Size] = 0;
		}

		constexpr void InternalConcat(const size_t startIndex, const char* ptr, const size_t length) const noexcept
		{
			for (size_t i = 0; i < length; i++)
				new(&m_Data[i + startIndex]) char(ptr[i]);
		}

	private:
		char* m_Data = nullptr;
		size_t m_Size = 0;
		size_t m_Capacity = 0;

		constexpr static size_t DefaultCapacity = 16;
	};
}
