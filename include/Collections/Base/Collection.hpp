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
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		constexpr HeapCollection() noexcept = default;

		constexpr HeapCollection(const HeapCollection& other) noexcept
		{
			if (other.IsEmpty())
				return;

			Allocate(other.m_Capacity);
			
			for (usize i = 0; i < other.m_Size; i++)
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
			const usize length = initializerList.size();
			if (length == 0)
				return;

			Allocate(length);

			for (auto& elem : initializerList)
				new(&m_Data[m_Size++]) T(std::move(const_cast<T&>(elem)));
		}

		constexpr explicit HeapCollection(const Span<T>& sequence) noexcept
		{
			const usize length = sequence.Capacity();
			if (length == 0)
				return;

			Allocate(length);

			for (usize i = 0; i < length; i++)
				new(&m_Data[m_Size++]) T(sequence[i]);
		}

		constexpr explicit HeapCollection(std::convertible_to<T> auto... elements) noexcept
		{
			// Get number of elements (arg count)
			constexpr usize length = sizeof ...(elements);

			Allocate(length);

			// Move values in data block
			for (auto values = {elements...}; auto&& elem : values)
				new(&m_Data[m_Size++]) T(std::move(elem));
		}

		constexpr explicit HeapCollection(const usize capacity) noexcept { Allocate(capacity); }

		constexpr ~HeapCollection() noexcept override
		{
			// Invalidate memory, then free
			Allocator<T>::ClearMemory(m_Data, m_Size);
			Allocator<T>::Dispose(m_Data, m_Capacity);

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
		NODISCARD constexpr usize Size() const noexcept { return m_Size; }
		NODISCARD constexpr usize Capacity() const noexcept { return m_Capacity; }
		NODISCARD constexpr Span<T> AsSpan() const noexcept { return {m_Data, m_Size}; }

		/* Enumerators (Iterators) */

		NODISCARD Enumerator<T> GetEnumerator() noexcept override
		{
			for (usize i = 0; i < m_Size; i++)
			{
				auto& element = m_Data[i];
				co_yield element;
			}
		}

		NODISCARD Enumerator<T> GetEnumerator() const noexcept override
		{
			for (usize i = 0; i < m_Size; i++)
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
			Allocator<T>::ClearMemory(m_Data, m_Size);
			m_Size = 0;
		}


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		NODISCARD constexpr operator Span<T>() const noexcept { return AsSpan(); }

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

	protected:
		/*
		 *  ============================================================
		 *	|                      Internal Helpers                    |
		 *  ============================================================
		 */

		
		constexpr void Allocate(const usize capacity) noexcept
		{
			m_Capacity = Allocator<T>::Allocate(m_Data, m_Capacity, capacity);
		}

		constexpr void Reallocate(const usize capacity) noexcept
		{
			m_Capacity = Allocator<T>::Reallocate(m_Data, m_Capacity, capacity);
		}

		/// <summary>
		/// Determines the correct capacity to allocate or reallocate with. (Reallocates with
		///	'capacity + (capacity / 2)', but uses the expected capacity when above that calculated value) 
		/// </summary>
		/// <param name="expectedCapacity">Expected capacity to allocate with</param>
		constexpr void HandleReallocation(const usize expectedCapacity) noexcept
		{
			if (expectedCapacity > m_Capacity)
				Reallocate(MAX(m_Capacity + (m_Capacity / 2), expectedCapacity));
		}

		constexpr void CopyFrom(const HeapCollection& other) noexcept
		{
			for (usize i = 0; i < other.m_Size; i++)
				m_Data[i] = other.m_Data[i];
		}

		constexpr void MoveFrom(HeapCollection&& other) noexcept
		{
			for (usize i = 0; i < other.m_Size; i++)
				m_Data[i] = std::move(other.m_Data[i]);
		}

	protected:
		Memory<T> m_Data = nullptr;
		usize m_Size = 0;
		usize m_Capacity = 0;
	};


	template <typename T, usize TSize>
	class StackCollection : public Enumerable<T>
	{
	public:
		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		constexpr StackCollection() noexcept = default;

		constexpr StackCollection(const StackCollection& other) noexcept
		{
			for (usize i = 0; i < TSize; i++)
				new(&m_Data[i]) T(other.m_Data[i]);
		}

		constexpr StackCollection(StackCollection&& other) noexcept
		{
			for (usize i = 0; i < TSize; i++)
				new(&m_Data[i]) T(std::move(other.m_Data[i]));
		}

		constexpr StackCollection(std::initializer_list<T>&& initializerList) noexcept
		{
			const usize capacity = initializerList.size();
			if (capacity == 0)
				return;

			const usize length = MIN(capacity, TSize);
			const auto data = initializerList.begin();
			for (usize i = 0; i < length; i++)
				new(&m_Data[i]) T(std::move(const_cast<T&>(data[i])));
		}

		constexpr explicit StackCollection(const Span<T>& sequence) noexcept
		{
			const usize capacity = sequence.Capacity();
			if (capacity == 0)
				return;

			const usize length = MIN(capacity, TSize);
			for (usize i = 0; i < length; i++)
				new(&m_Data[i]) T(sequence[i]);
		}

		constexpr explicit StackCollection(std::convertible_to<T> auto... elements) noexcept
		{
			constexpr usize capacity = sizeof ...(elements);
			if (capacity == 0)
				return;

			constexpr usize length = MIN(capacity, TSize);
			const T values[] { std::forward<T>(elements)... };

			usize index = 0;
			for (auto&& e : values)
			{
				if (index == length)
					return;

				new(&m_Data[index++]) T(std::move(e));
			}
		}

		constexpr explicit StackCollection(const T(&array)[TSize]) noexcept
		{
			for (usize i = 0; i < TSize; i++)
				new(&m_Data[i]) T(array[i]);
		}

		constexpr ~StackCollection() noexcept override = default;


		/*
		 *  ============================================================
		 *	|                         Accessors                        |
		 *  ============================================================
		 */


		NODISCARD constexpr const T* Data() const noexcept { return m_Data; }
		NODISCARD constexpr usize Capacity() const noexcept { return TSize; }
		NODISCARD constexpr Span<T> AsSpan() const noexcept { return { m_Data, TSize}; }

		/* Enumerators (Iterators) */

		NODISCARD Enumerator<T> GetEnumerator() noexcept override
		{
			for (usize i = 0; i < TSize; i++)
			{
				auto& element = m_Data[i];
				co_yield element;
			}
		}

		NODISCARD Enumerator<T> GetEnumerator() const noexcept override
		{
			for (usize i = 0; i < TSize; i++)
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


		NODISCARD constexpr explicit operator Span<T>() const noexcept { return AsSpan(); }

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

	protected:
		/*
		 *  ============================================================
		 *	|                      Internal Helpers                    |
		 *  ============================================================
		 */


		constexpr void CopyFrom(const StackCollection& other) noexcept
		{
			for (usize i = 0; i < TSize; i++)
				m_Data[i] = other.m_Data[i];
		}

		constexpr void MoveFrom(StackCollection&& other) noexcept
		{
			for (usize i = 0; i < TSize; i++)
				m_Data[i] = std::move(other.m_Data[i]);
		}

	protected:
		T m_Data[TSize]{};
	};
}
