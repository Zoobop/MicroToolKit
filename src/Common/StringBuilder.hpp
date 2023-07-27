#pragma once
#include "Common/String.hpp"

namespace Micro
{
	/**
	 * \brief Represents a way to efficiently build a String using fewer allocations and direct mutations.
	 */
	class StringBuilder final
	{
	public:
		/*
		 *  ============================================================
		 *	|                 Constructors/Destructors                 |
		 *  ============================================================
		 */


		/**
		 * \brief Initializes a new instance of the StringBuilder class to have the underlying char pointer be null.
		 */
		constexpr StringBuilder() noexcept = default;

		/**
		 * \brief Initializes a new instance of the StringBuilder class using the given StringBuilder to be copied.
		 */
		constexpr StringBuilder(const StringBuilder&) noexcept = default;

		/**
		 * \brief Initializes a new instance of the StringBuilder class using the given StringBuilder to be moved.
		 */
		constexpr StringBuilder(StringBuilder&&) noexcept = default;

		/**
		 * \brief Initializes a new instance of the StringBuilder class by copying the value of the String-like argument
		 *		  that is based on the CharSequence concept specifications into the underlying buffer.
		 * \param string String-like object to copy
		 */
		constexpr explicit StringBuilder(const CharSequence auto& string) noexcept
		{
			Allocate(MAX(string.Length(), DefaultCapacity));
			InternalCopy(string.Data(), string.Length());
		}

		/**
		 * \brief Initializes a new instance of the StringBuilder class by copying the value of the String-like argument
		 *		  that is based on the StdCharSequence concept specifications into the underlying buffer.
		 * \param string String-like object to copy
		 */
		constexpr explicit StringBuilder(const StdCharSequence auto& string) noexcept
		{
			Allocate(MAX(string.size(), DefaultCapacity));
			InternalCopy(string.data(), string.size());
		}

		/**
		 * \brief Initializes a new instance of the StringBuilder class with the underlying buffer allocated to the
		 *		  given capacity.
		 * \param capacity Capacity to initialize the underlying buffer with
		 */
		constexpr explicit StringBuilder(const size_t capacity) noexcept { Allocate(capacity); }

		/**
		 * \brief Frees the memory of the underlying char buffer and sets it to null.
		 */
		constexpr ~StringBuilder() noexcept
		{
			delete[] m_Data;
			m_Data = nullptr;
			m_Size = 0;
			m_Capacity = 0;
		}

		
		/*
		 *  ============================================================
		 *	|                         Accessors                        |
		 *  ============================================================
		 */


		/// <summary>
		/// Represents if the StringBuilder is empty or not.
		/// </summary>
		/// <returns>True, if the StringBuilder is empty.</returns>
		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Size == 0 || m_Data == nullptr; }

		/// <summary>
		/// Represents a 64-bit unsigned integer as the length of the valid string.
		/// </summary>
		/// <returns>Length of type 'size_t'</returns>
		NODISCARD constexpr size_t Length() const noexcept { return m_Size; }

		/// <summary>
		/// Represents the underlying character buffer.
		/// </summary>
		/// <returns>Character array of type 'const char*'.</returns>
		NODISCARD constexpr const char* Data() const noexcept { return m_Data; }

		/// <summary>
		/// Represents a 64-bit unsigned integer as the capacity of the underlying buffer.
		/// </summary>
		/// <returns>Capacity of type 'size_t'</returns>
		NODISCARD constexpr size_t Capacity() const noexcept { return m_Capacity; }

		/// <summary>
		/// Represents the underlying character buffer. (const version)
		/// </summary>
		/// <returns>Character array of type 'char*'.</returns>
		NODISCARD constexpr char* ToCharArray() const noexcept { return m_Data; }


		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		/// <summary>
		/// Adds the String-like argument that is based on the CharSequence concept specifications to the end of the char buffer.
		/// If argument is empty, it will return with no allocations.
		/// </summary>
		/// <param name="string">String-like object to append</param>
		/// <returns>Reference of this instance</returns>
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
				HandleReallocation(newSize);

				// Set new data
				InternalConcat(m_Size, string.Data(), length);
			}

			m_Size += length;
			return *this;
		}

		/// <summary>
		/// Adds the String-like argument that is based on the StdCharSequence concept specifications to the end of the char buffer.
		/// If argument is empty, it will return with no allocations.
		/// </summary>
		/// <param name="string">String-like object to append</param>
		/// <returns>Reference of this instance</returns>
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
				HandleReallocation(newSize);

				// Set new data
				InternalConcat(m_Size, string.data(), length);
			}

			m_Size += length;
			return *this;
		}

		/// <summary>
		/// Adds the raw string literal argument to the end of the char buffer.
		/// If argument is empty, it will return with no allocations.
		/// </summary>
		/// <typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to append</param>
		/// <returns>Reference of this instance</returns>
		template <size_t TSize>
		constexpr StringBuilder& Append(const char (&string)[TSize]) noexcept
		{
			constexpr size_t length = TSize - 1;
			if constexpr (length == 0)
				return *this;

			if (m_Data == nullptr)
			{
				Allocate(MAX(length, DefaultCapacity));
				InternalCopy(string, length);
			}
			else
			{
				// Reallocate, if necessary
				const size_t newSize = m_Size + length;
				HandleReallocation(newSize);

				// Set new data
				InternalConcat(m_Size, string, length);
			}

			m_Size += length;
			return *this;
		}

		/// <summary>
		/// Adds the char pointer argument to the end of the char buffer.
		/// If argument is null, it will return with no allocations.
		/// </summary>
		/// <param name="string">Char pointer to append</param>
		/// <param name="length">Length of the char pointer</param>
		/// <returns>Reference of this instance</returns>
		constexpr StringBuilder& Append(const char* string, const size_t length) noexcept
		{
			if (length == 0) 
				return *this;

			if (m_Data == nullptr)
			{
				Allocate(MAX(length, DefaultCapacity));
				InternalCopy(string, length);
			}
			else
			{
				// Reallocate, if necessary
				const size_t newSize = m_Size + length;
				HandleReallocation(newSize);

				// Set new data
				InternalConcat(m_Size, string, length);
			}

			m_Size += length;
			return *this;
		}

		/// <summary>
		/// Adds the bool argument to the end of the char buffer.
		/// </summary>
		/// <param name="boolean">Bool to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr StringBuilder& Append(const bool boolean) noexcept
		{
			return boolean ? Append("true") : Append("false");
		}

		/// <summary>
		/// Adds the character argument to the end of the char buffer.
		/// </summary>
		/// <param name="character">Character to append</param>
		/// <returns>Reference of this instance</returns>
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
				const size_t newSize = m_Size + length;
				HandleReallocation(newSize);

				// Set new data
				m_Data[m_Size] = character;
			}

			m_Size += length;
			return *this;
		}

		/// <summary>
		/// Adds the integer argument to the end of the char buffer.
		/// </summary>
		/// <param name="integer">Integer to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr StringBuilder& Append(const std::integral auto integer) noexcept { return Append(Micro::ToString(integer)); }

		/// <summary>
		/// Adds the floating-point argument to the end of the char buffer.
		/// </summary>
		/// <param name="floatingPoint">Floating-point value to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr StringBuilder& Append(const std::floating_point auto floatingPoint) noexcept { return Append(Micro::ToString(floatingPoint)); }

		/// <summary>
		/// Adds a new line character to the end of the char buffer.
		/// </summary>
		/// <returns>Reference of this instance</returns>
		constexpr StringBuilder& AppendLine() noexcept
		{
			constexpr size_t length = 1;
			if (m_Data == nullptr)
			{
				Allocate(MAX(length, DefaultCapacity));
				InternalCopy("\n", length);
			}
			else
			{
				// Reallocate, if necessary
				const size_t newSize = m_Size + length;
				HandleReallocation(newSize);

				// Set new data
				InternalConcat(m_Size, "\n", length);
			}

			m_Size += length;
			return *this;
		}

		/// <summary>
		/// Adds the String-like argument that is based on the CharSequence concept specifications to the
		/// end of the char buffer with a new line. If argument is empty, it will return with no allocations.
		/// </summary>
		/// <param name="string">String-like object to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr StringBuilder& AppendLine(const CharSequence auto& string) noexcept
		{
			const size_t length = string.Length() + 1;
			if (length == 0)
				return *this;

			if (m_Data == nullptr)
			{
				Allocate(MAX(length, DefaultCapacity));
				InternalCopy(string.Data(), length - 1);
			}
			else
			{
				// Reallocate, if necessary
				const size_t newSize = m_Size + length;
				HandleReallocation(newSize);

				// Set new data
				InternalConcat(m_Size, string.Data(), length - 1);
			}

			InternalConcat(m_Size + length - 1, "\n", 1);
			m_Size += length;
			return *this;
		}

		/// <summary>
		/// Adds the String-like argument that is based on the StdCharSequence concept specifications to the
		/// end of the char buffer with a new line. If argument is empty, it will return with no allocations.
		/// </summary>
		/// <param name="string">String-like object to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr StringBuilder& AppendLine(const StdCharSequence auto& string) noexcept
		{
			const size_t length = string.size() + 1;
			if (length == 0)
				return *this;

			if (m_Data == nullptr)
			{
				Allocate(MAX(length, DefaultCapacity));
				InternalCopy(string.data(), length - 1);
			}
			else
			{
				// Reallocate, if necessary
				const size_t newSize = m_Size + length;
				HandleReallocation(newSize);

				// Set new data
				InternalConcat(m_Size, string.data(), length - 1);
			}

			InternalConcat(m_Size + length - 1, "\n", 1);
			m_Size += length;
			return *this;
		}

		/// <summary>
		/// Adds the raw string literal argument to the end of the char buffer with a new line.
		/// If argument is empty, it will return with no allocations.
		/// </summary>
		/// <typeparam name="TSize">Represents the implicit capture of the raw string literal's size (including null termination)</typeparam>
		/// <param name="string">Raw string literal to append</param>
		/// <returns>Reference of this instance</returns>
		template <size_t TSize>
		constexpr StringBuilder& AppendLine(const char(&string)[TSize]) noexcept
		{
			constexpr size_t length = TSize;
			if constexpr (length == 0)
				return *this;

			if (m_Data == nullptr)
			{
				Allocate(MAX(length, DefaultCapacity));
				InternalCopy(string, length - 1);
			}
			else
			{
				// Reallocate, if necessary
				const size_t newSize = m_Size + length;
				HandleReallocation(newSize);

				// Set new data
				InternalConcat(m_Size, string, length - 1);
			}

			InternalConcat(m_Size + length - 1, "\n", 1);
			m_Size += length;
			return *this;
		}

		/// <summary>
		/// Adds the char pointer argument to the end of the char buffer with a new line.
		/// If argument is null, it will return with no allocations.
		/// </summary>
		/// <param name="string">Char pointer to append</param>
		/// <param name="length">Length of the char pointer</param>
		/// <returns>Reference of this instance</returns>
		constexpr StringBuilder& AppendLine(const char* string, const size_t length) noexcept
		{
			if (length == 0)
				return *this;

			if (m_Data == nullptr)
			{
				Allocate(MAX(length + 1, DefaultCapacity));
				InternalCopy(string, length);
			}
			else
			{
				// Reallocate, if necessary
				const size_t newSize = m_Size + length + 1;
				HandleReallocation(newSize);

				// Set new data
				InternalConcat(m_Size, string, length);
			}

			InternalConcat(m_Size + length - 1, "\n", 1);
			m_Size += length;
			return *this;
		}

		/// <summary>
		/// Adds the bool argument to the end of the char buffer with a new line.
		/// </summary>
		/// <param name="boolean">Bool to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr StringBuilder& AppendLine(const bool boolean) noexcept
		{
			return boolean ? Append("true\n") : Append("false\n");
		}

		/// <summary>
		/// Adds the character argument to the end of the char buffer with a new line.
		/// </summary>
		/// <param name="character">Character to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr StringBuilder& AppendLine(const char character) noexcept
		{
			constexpr size_t length = 2;
			if (m_Data == nullptr)
			{
				Allocate(MAX(length, DefaultCapacity));
				InternalCopy(&character, length - 1);
			}
			else
			{
				// Reallocate, if necessary
				const size_t newSize = m_Size + length - 1;
				HandleReallocation(newSize);

				// Set new data
				m_Data[m_Size] = character;
				m_Data[m_Size + 1] = '\n';
			}

			m_Size += length;
			return *this;
		}

		/// <summary>
		/// Adds the integer argument to the end of the char buffer with a new line.
		/// </summary>
		/// <param name="integer">Integer to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr StringBuilder& AppendLine(const std::integral auto integer) noexcept { return Append(Micro::ToString(integer)).AppendLine(); }

		/// <summary>
		/// Adds the floating-point argument to the end of the char buffer with a new line.
		/// </summary>
		/// <param name="floatingPoint">Floating-point value to append</param>
		/// <returns>Reference of this instance</returns>
		constexpr StringBuilder& AppendLine(const std::floating_point auto floatingPoint) noexcept { return Append(Micro::ToString(floatingPoint)).AppendLine(); }

		/// <summary>
		/// Removes elements from the starting index through the length and shifts down
		/// the remaining data to fill the empty space within the buffer.
		/// </summary>
		/// <param name="start">Index to start the removal</param>
		/// <param name="length">Number of elements to remove</param>
		/// <returns>Reference of this instance</returns>
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

		/// <summary>
		/// Takes the current buffer and creates a new String instance from it.
		/// </summary>
		/// <returns>New instance of a String using the character buffer.</returns>
		NODISCARD constexpr String ToString() const noexcept { return { m_Data, m_Size }; }

		/**
		 * \brief Soft clears the builder by setting the size to zero to avoid reallocation.
		 */
		constexpr void Clear() noexcept { m_Size = 0; }


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		/// <summary>
		/// Gets the character at the given index. Throws an 'IndexOutOfRangeException', if index is invalid.
		/// </summary>
		/// <param name="index">Index of element</param>
		/// <returns>Reference of character at index</returns>
		NODISCARD constexpr char& operator[](const size_t index)
		{
			if (index >= m_Size)
				throw IndexOutOfRangeException(index);

			return m_Data[index];
		}

		/// <summary>
		/// Gets the character at the given index. Throws an 'IndexOutOfRangeException', if index is invalid. (const version)
		/// </summary>
		/// <param name="index">Index of element</param>
		/// <returns>Reference of character at index</returns>
		NODISCARD constexpr const char& operator[](const size_t index) const
		{
			if (index >= m_Size)
				throw IndexOutOfRangeException(index);

			return m_Data[index];
		}

		/// <summary>
		/// Copies the reference of the StringBuilder into this instance.
		/// </summary>
		/// <param name="builder">StringBuilder to copy reference from</param>
		/// <returns>Reference to this instance</returns>
		constexpr StringBuilder& operator=(const StringBuilder& builder)
		{
			if (this == &builder)
				return *this;

			const size_t length = builder.m_Size;
			if (!m_Data)
			{
				Allocate(length);
			}
			else
			{
				if (length != m_Size)
					Reallocate(length);
			}

			InternalCopy(builder.m_Data, length);
			return *this;
		}

		/// <summary>
		/// Moves the reference of the StringBuilder into this instance.
		/// </summary>
		/// <param name="builder">StringBuilder to move reference from</param>
		/// <returns>Reference to this instance</returns>
		constexpr StringBuilder& operator=(StringBuilder&& builder) noexcept
		{
			if (this == &builder)
				return *this;

			delete[] m_Data;

			m_Data = builder.m_Data;
			m_Size = builder.m_Size;
			m_Capacity = builder.m_Capacity;

			builder.m_Data = nullptr;
			builder.m_Size = 0;
			builder.m_Capacity = 0;

			return *this;
		}

		/// <summary>
		/// Prints the given StringBuilder out to the console.
		/// </summary>
		/// <param name="stream">Stream to print to the standard output</param>
		/// <param name="current">StringBuilder to print out</param>
		/// <returns>Reference of the stream</returns>
		friend std::ostream& operator<<(std::ostream& stream, const StringBuilder& current) noexcept
		{
			if (current.m_Size > 0)
			{
				stream << "[";
				for (size_t i = 0; i < current.m_Size; i++)
					stream << current.m_Data[i];
				stream << "]";
			}

			return stream;
		}

	private:
		/*
		 *  ============================================================
		 *	|                     Internal Helpers                     |
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
			m_Data[capacity] = 0;
			m_Capacity = capacity;
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
				m_Data[capacity] = 0;
				m_Capacity = capacity;
				return;
			}

			// Allocate initial memory
			Allocate(capacity);
		}

		/// <summary>
		/// Determines the correct capacity to allocate or reallocate with. (Reallocates with
		///	'capacity + (capacity / 2)', but uses the expected capacity when above that calculated value) 
		/// </summary>
		/// <param name="expectedCapacity">Expected capacity to allocate with</param>
		constexpr void HandleReallocation(const size_t expectedCapacity)
		{
			if (expectedCapacity > m_Capacity)
				Reallocate(MAX(m_Capacity + (m_Capacity / 2), expectedCapacity));
		}

		/// <summary>
		/// Copies the char pointer into the underlying buffer using the given length.
		/// </summary>
		/// <param name="ptr">Char pointer to copy</param>
		/// <param name="length">Length of char pointer</param>
		constexpr void InternalCopy(const char* ptr, const size_t length) const noexcept
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
		constexpr void InternalConcat(const size_t startIndex, const char* ptr, const size_t length) const noexcept
		{
			for (size_t i = 0; i < length; i++)
				new(&m_Data[i + startIndex]) char(ptr[i]);
		}

	private:
		char* m_Data = nullptr;
		size_t m_Size = 0;
		size_t m_Capacity = 0;

		constexpr static size_t DefaultCapacity = 64;
	};


	/*
	 *  ============================================================
	 *	|                      Global Functions                    |
	 *  ============================================================
	 */


	/// <summary>
	/// Hashes the StringBuilder to produce a unique hash code.
	/// </summary>
	/// <param name="object">StringBuilder to hash</param>
	/// <returns>Hash code as a 'size_t'</returns>
	template <>
	NODISCARD inline size_t Hash(const StringBuilder& object) noexcept
	{
		const size_t size = object.Length();
		size_t hash = 0;
		for (size_t i = 0; i < size; i++)
			hash += object[i];

		return typeid(StringBuilder).hash_code() + size + hash;
	}
}
