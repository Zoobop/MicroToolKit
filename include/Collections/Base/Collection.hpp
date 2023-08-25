#pragma once
#include <ostream>

#include "Core/Memory/Allocator.hpp"
#include "Collections/Base/Enumerable.hpp"
#include "Common/Span.hpp"

namespace Micro
{
    template <typename T>
	class HeapCollection : public Enumerable<T>
	{
	public:
		/*
		 *  ============================================================
		 *	|                          Aliases                         |
		 *  ============================================================
		 */


		using Memory = Memory<T>;
		using Allocator = Allocator<T>;
		using Span = Span<T>;

		
		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		constexpr HeapCollection() noexcept = default;

		constexpr HeapCollection(const HeapCollection& other) noexcept
		{
			if (other.IsEmpty())
				return;

			Allocate(other.m_Capacity);
			
			for (size_t i = 0; i < other.m_Size; i++)
				new(&m_Data[m_Size++]) T(other.m_Data[i]);
		}

		constexpr HeapCollection(HeapCollection&& other) noexcept
			: m_Data(std::move(other.m_Data)), m_Size(other.m_Size), m_Capacity(other.m_Capacity)
		{
			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;
		}

		constexpr HeapCollection(std::initializer_list<T>&& initializerList) noexcept
		{
			const size_t length = initializerList.size();
			if (length == 0)
				return;

			Allocate(length);

			for (auto& elem : initializerList)
				new(&m_Data[m_Size++]) T(std::move(const_cast<T&>(elem)));
		}

		constexpr explicit HeapCollection(const Span& sequence) noexcept
		{
			const size_t length = sequence.Capacity();
			if (length == 0)
				return;

			Allocate(length);

			for (size_t i = 0; i < length; i++)
				new(&m_Data[m_Size++]) T(sequence[i]);
		}

		constexpr explicit HeapCollection(std::convertible_to<T> auto... elements) noexcept
		{
			// Get number of elements (arg count)
			constexpr size_t length = sizeof ...(elements);

			Allocate(length);

			// Move values in data block
			for (auto values = {elements...}; auto&& elem : values)
				new(&m_Data[m_Size++]) T(std::move(elem));
		}

		constexpr explicit HeapCollection(const size_t capacity) noexcept { Allocate(capacity); }

		constexpr ~HeapCollection() noexcept override
		{
			// Invalidate memory, then free
			Allocator::ClearMemory(m_Data, m_Size);
			Allocator::Dispose(m_Data, m_Capacity);

			m_Data = nullptr;
			m_Size = 0;
			m_Capacity = 0;
		}

		
		/*
		 *  ============================================================
		 *	|                         Accessors                        |
		 *  ============================================================
		 */


		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Size == 0 || m_Data == nullptr; }
		NODISCARD constexpr const T* Data() const noexcept { return m_Data; }
		NODISCARD constexpr size_t Size() const noexcept { return m_Size; }
		NODISCARD constexpr size_t Capacity() const noexcept { return m_Capacity; }
		NODISCARD constexpr Span AsSpan() const noexcept { return {m_Data, m_Size}; }

		/* Enumerators (Iterators) */

		NODISCARD Enumerator<T> GetEnumerator() override
		{
			for (size_t i = 0; i < m_Size; i++)
			{
				auto& element = m_Data[i];
				co_yield element;
			}
		}

		NODISCARD Enumerator<T> GetEnumerator() const override
		{
			for (size_t i = 0; i < m_Size; i++)
			{
				const auto& element = m_Data[i];
				co_yield element;
			}
		}


		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		 /// <summary>
		 /// Test if each element within the given collection.
		 /// </summary>
		 /// <param name="collection">Collection to test equivalence</param>
		 /// <returns>True, if all elements match</returns>
		NODISCARD constexpr bool Equals(const HeapCollection& collection) const noexcept { return Micro::Equals(AsSpan(), collection.AsSpan()); }

		/// <summary>
		/// Searches for the given element within the collection.
		/// </summary>
		/// <param name="value">Value to find</param>
		/// <returns>True, if found</returns>
		NODISCARD constexpr bool Contains(const T& value) const noexcept { return Micro::Contains(AsSpan(), value); }

		constexpr void Clear() noexcept
		{
			// Invalidate data
			Allocator::ClearMemory(m_Data, m_Size);
			m_Size = 0;
		}


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		NODISCARD constexpr explicit operator Span() const noexcept { return AsSpan(); }

		constexpr HeapCollection& operator=(const HeapCollection& other) noexcept
		{
			// Validation
			if (this == &other)
				return *this;

			if (other.m_Capacity == 0)
				return *this;

			// Allocation
			if (m_Data.IsValidMemory())
				Reallocate(other.m_Capacity);
			else
				Allocate(other.m_Capacity);

			m_Size = other.m_Size;

			// Assignment
			CopyFrom(other);
			return *this;
		}

		constexpr HeapCollection& operator=(HeapCollection&& other) noexcept
		{
			if (this == &other)
				return *this;

			if (!IsEmpty())
				Clear();

			m_Data = other.m_Data;
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;

			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const HeapCollection& current) noexcept
		{
			stream << "[";
			for (size_t i = 0; i < current.m_Size; i++)
			{
				stream << current.m_Data[i];
				if (i != current.m_Size - 1)
					stream << ", ";
			}

			stream << "]";
			return stream;
		}

	protected:
		/*
		 *  ============================================================
		 *	|                      Internal Helpers                    |
		 *  ============================================================
		 */

		
		constexpr void Allocate(const size_t capacity) noexcept
		{
			m_Capacity = Allocator::Allocate(m_Data, m_Capacity, capacity);
		}

		constexpr void Reallocate(const size_t capacity) noexcept
		{
			m_Capacity = Allocator::Reallocate(m_Data, m_Capacity, capacity);
		}

		/// <summary>
		/// Determines the correct capacity to allocate or reallocate with. (Reallocates with
		///	'capacity + (capacity / 2)', but uses the expected capacity when above that calculated value) 
		/// </summary>
		/// <param name="expectedCapacity">Expected capacity to allocate with</param>
		constexpr void HandleReallocation(const size_t expectedCapacity) noexcept
		{
			if (expectedCapacity > m_Capacity)
				Reallocate(MAX(m_Capacity + (m_Capacity / 2), expectedCapacity));
		}

		constexpr void CopyFrom(const HeapCollection& other) noexcept
		{
			for (size_t i = 0; i < other.m_Size; i++)
				m_Data[i] = other.m_Data[i];
		}

		constexpr void MoveFrom(HeapCollection&& other) noexcept
		{
			for (size_t i = 0; i < other.m_Size; i++)
				m_Data[i] = std::move(other.m_Data[i]);
		}

	protected:
		Memory m_Data = nullptr;
		size_t m_Size = 0;
		size_t m_Capacity = 0;
	};


	template <typename T, size_t TSize>
	class StackCollection : public Enumerable<T>
	{
	public:
		/*
		 *  ============================================================
		 *	|                          Aliases                         |
		 *  ============================================================
		 */


		using Span = Span<T>;


		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		constexpr StackCollection() noexcept = default;

		constexpr StackCollection(const StackCollection& other) noexcept
		{
			for (size_t i = 0; i < TSize; i++)
				new(&m_Data[i]) T(other.m_Data[i]);
		}

		constexpr StackCollection(StackCollection&& other) noexcept
		{
			for (size_t i = 0; i < TSize; i++)
				new(&m_Data[i]) T(std::move(other.m_Data[i]));
		}

		constexpr StackCollection(std::initializer_list<T>&& initializerList) noexcept
		{
			const size_t capacity = initializerList.size();
			if (capacity == 0)
				return;

			const size_t length = MIN(capacity, TSize);
			const auto data = initializerList.begin();
			for (size_t i = 0; i < length; i++)
				new(&m_Data[i]) T(std::move(const_cast<T&>(data[i])));
		}

		constexpr explicit StackCollection(const Span& sequence) noexcept
		{
			const size_t capacity = sequence.Capacity();
			if (capacity == 0)
				return;

			const size_t length = MIN(capacity, TSize);
			for (size_t i = 0; i < length; i++)
				new(&m_Data[i]) T(sequence[i]);
		}

		constexpr explicit StackCollection(std::convertible_to<T> auto... elements) noexcept
		{
			constexpr size_t capacity = sizeof ...(elements);
			if (capacity == 0)
				return;

			constexpr size_t length = MIN(capacity, TSize);
			const T values[] { std::forward<T>(elements)... };

			size_t index = 0;
			for (auto&& e : values)
			{
				if (index == length)
					return;

				new(&m_Data[index++]) T(std::move(e));
			}
		}

		constexpr explicit StackCollection(const T(&array)[TSize]) noexcept
		{
			for (size_t i = 0; i < TSize; i++)
				new(&m_Data[i]) T(array[i]);
		}

		constexpr ~StackCollection() noexcept override = default;


		/*
		 *  ============================================================
		 *	|                         Accessors                        |
		 *  ============================================================
		 */


		NODISCARD constexpr const T* Data() const noexcept { return m_Data; }
		NODISCARD constexpr size_t Capacity() const noexcept { return TSize; }
		NODISCARD constexpr Span AsSpan() const noexcept { return { m_Data, TSize}; }

		/* Enumerators (Iterators) */

		NODISCARD Enumerator<T> GetEnumerator() override
		{
			for (size_t i = 0; i < TSize; i++)
			{
				auto& element = m_Data[i];
				co_yield element;
			}
		}

		NODISCARD Enumerator<T> GetEnumerator() const override
		{
			for (size_t i = 0; i < TSize; i++)
			{
				const auto& element = m_Data[i];
				co_yield element;
			}
		}


		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		/// <summary>
		/// Test if each element within the given collection.
		/// </summary>
		/// <param name="collection">Collection to test equivalence</param>
		/// <returns>True, if all elements match</returns>
		NODISCARD constexpr bool Equals(const StackCollection& collection) const noexcept { return Micro::Equals(AsSpan(), collection.AsSpan()); }

		/// <summary>
		/// Searches for the given element within the collection.
		/// </summary>
		/// <param name="value">Value to find</param>
		/// <returns>True, if found</returns>
		NODISCARD constexpr bool Contains(const T& value) const noexcept { return Micro::Contains(AsSpan(), value); }


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		NODISCARD constexpr explicit operator Span() const noexcept { return AsSpan(); }

		constexpr StackCollection& operator=(const StackCollection& other) noexcept
		{
			// Validation
			if (this == &other)
				return *this;

			CopyFrom(other);
			return *this;
		}

		constexpr StackCollection& operator=(StackCollection&& other) noexcept
		{
			if (this == &other)
				return *this;

			MoveFrom(std::move(other));
			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const StackCollection& current) noexcept
		{
			stream << "[";
			for (size_t i = 0; i < TSize; i++)
			{
				stream << current.m_Data[i];
				if (i != TSize - 1)
					stream << ", ";
			}

			stream << "]";
			return stream;
		}

	protected:
		/*
		 *  ============================================================
		 *	|                      Internal Helpers                    |
		 *  ============================================================
		 */


		constexpr void CopyFrom(const StackCollection& other) noexcept
		{
			for (size_t i = 0; i < TSize; i++)
				m_Data[i] = other.m_Data[i];
		}

		constexpr void MoveFrom(StackCollection&& other) noexcept
		{
			for (size_t i = 0; i < TSize; i++)
				m_Data[i] = std::move(other.m_Data[i]);
		}

	protected:
		T m_Data[TSize]{};
	};
}
