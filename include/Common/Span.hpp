#pragma once
#include <ostream>

#include "Core/Function.hpp"
#include "Core/Hash.hpp"
#include "Core/Memory/Memory.hpp"
#include "Collections/Base/Enumerable.hpp"
#include "Utility/Options/Optional.hpp"

namespace Micro
{
	template <typename T>
	class Span final : public Enumerable<T>
	{
	public:
		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		constexpr Span() noexcept = default;

		constexpr Span(const Span& other) noexcept
			: m_Data(other.m_Data), m_Capacity(other.m_Capacity)
		{
		}

		constexpr Span(Span&& other) noexcept
			: m_Data(other.m_Data), m_Capacity(other.m_Capacity)
		{
		}

		constexpr Span(const T* data, const usize size) noexcept
			: m_Data(const_cast<T*>(data)), m_Capacity(size)
		{
		}

		constexpr Span(T* data, const usize size) noexcept
			: m_Data(data), m_Capacity(size)
		{
		}

		template <usize TSize>
		constexpr explicit Span(const T (&data)[TSize]) noexcept
			: m_Data(data), m_Capacity(TSize)
		{
		}

		constexpr Span(const T* begin, const T* end) noexcept
			: m_Data(begin), m_Capacity(end - begin)
		{
		}

		constexpr ~Span() noexcept override = default;

		
		/*
		 *  ============================================================
		 *	|                         Accessors                        |
		 *  ============================================================
		 */


		NODISCARD constexpr usize Capacity() const noexcept { return m_Capacity; }
		NODISCARD constexpr const T* Data() const noexcept { return m_Data; }
		NODISCARD constexpr T* Data() noexcept { return m_Data; }
		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Data == nullptr; }

		/* Enumerators (Iterators) */

		NODISCARD Enumerator<T> GetEnumerator() noexcept override
		{
			for (usize i = 0; i < m_Capacity; i++)
			{
				auto& element = m_Data[i];
				co_yield element;
			}
		}

		NODISCARD Enumerator<T> GetEnumerator() const noexcept override
		{
			for (usize i = 0; i < m_Capacity; i++)
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


		NODISCARD constexpr Span Slice(const usize start) const noexcept
		{
			if (start >= m_Capacity)
				return Empty();

			return { m_Data + start, m_Data + m_Capacity };
		}

		NODISCARD constexpr Span Slice(const usize start, const usize length) const noexcept
		{
			if (start + length >= m_Capacity)
				return Empty();

			return { m_Data + start, m_Data + start + length };
		}

		/// <summary>
		/// Tests the equivalence between this instance and the span.
		/// </summary>
		/// <param name="span">Span to test against</param>
		/// <returns>True, if equal</returns>
		NODISCARD constexpr bool Equals(const Span& span) const noexcept
		{
			if (m_Capacity != span.Capacity())
				return false;

			for (usize i = 0; i < m_Capacity; i++)
			{
				if (m_Data[i] != span[i])
					return false;
			}

			return true;
		}

		NODISCARD constexpr bool Contains(const T& element) const noexcept
		{
			for (usize i = 0; i < m_Capacity; i++)
				if (m_Data[i] == element)
					return true;
			return false;
		}

		NODISCARD constexpr Optional<usize> IndexOf(const T& element) const noexcept
		{
			for (usize i = 0; i < m_Capacity; ++i)
				if (m_Data[i] == element)
					return Optional<usize>(i);

			return Optional<usize>::Empty();
		}

		NODISCARD constexpr Optional<usize> LastIndexOf(const T& element) const noexcept
		{
			for (usize i = 0; i < m_Capacity; ++i)
			{
				const usize offset = m_Capacity - 1 - i;
				if (m_Data[offset] == element)
					return Optional<usize>(offset);
			}

			return Optional<usize>::Empty();
		}

		NODISCARD constexpr bool Exists(const Predicate<T>& predicate) const noexcept
		{
			for (usize i = 0; i < m_Capacity; ++i)
				if (predicate(m_Data[i]))
					return true;
			return false;
		}

		NODISCARD constexpr usize Count(const Predicate<T>& predicate) const noexcept
		{
			usize count = 0;
			for (usize i = 0; i < m_Capacity; ++i)
				if (predicate(m_Data[i]))
					++count;
			return count;
		}

		constexpr void Reverse() noexcept
		{
			const usize length = m_Capacity / 2;
			for (usize i = 0; i < length; ++i)
			{
				const usize swapIndex = m_Capacity - i - 1;
				const auto temp = std::move(m_Data[i]);
				m_Data[i] = m_Data[swapIndex];
				m_Data[swapIndex] = temp;
			}
		}

		constexpr bool Swap(const usize index1, const usize index2) noexcept
		{
			if (index1 >= m_Capacity || index2 >= m_Capacity || index1 == index2)
				return false;

			auto temp = std::move(m_Data[index1]);
			m_Data[index1] = std::move(m_Data[index2]);
			m_Data[index2] = std::move(temp);
			return true;
		}


		/*
		 *  ============================================================
		 *	|                           LINQ                           |
		 *  ============================================================
		 */


		NODISCARD constexpr Optional<T&> First() noexcept { return operator[](0); }

		NODISCARD constexpr Optional<const T&> First() const noexcept { return operator[](0); }

		NODISCARD constexpr Optional<T&> Last() noexcept { return operator[](m_Capacity - 1); }

		NODISCARD constexpr Optional<const T&> Last() const noexcept { return operator[](m_Capacity - 1); }

		NODISCARD constexpr bool Any() const noexcept { return m_Data != nullptr && m_Capacity > 0; }

		NODISCARD constexpr bool Any(const Predicate<T>& predicate) const noexcept
		{
			for (usize i = 0; i < m_Capacity; ++i)
			{
				const auto& element = m_Data[i];
				if (predicate(element))
					return true;
			}

			return false;
		}

		NODISCARD constexpr bool All(const Predicate<T>& predicate) const noexcept
		{
			for (usize i = 0; i < m_Capacity; ++i)
			{
				const auto& element = m_Data[i];
				if (!predicate(element))
					return false;
			}

			return true;
		}


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		/// <summary>
		/// Gets the element at the given index, or an empty result if invalid.
		/// </summary>
		/// <param name="index">Index of element</param>
		/// <returns>Reference to the element at index, or empty result if invalid</returns>
		NODISCARD constexpr Optional<T&> operator[](const usize index) noexcept
		{
			if (index >= m_Capacity)
				return Optional<T&>::Empty();

			return Optional<T&>(m_Data[index]);
		}

		/// <summary>
		/// Gets the element at the given index, or an empty result if invalid. (const version)
		/// </summary>
		/// <param name="index">Index of element</param>
		/// <returns>Reference to the element at index, or empty result if invalid</returns>
		NODISCARD constexpr Optional<const T&> operator[](const usize index) const noexcept
		{
			if (index >= m_Capacity)
				return Optional<const T&>::Empty();

			return Optional<const T&>(m_Data[index]);
		}

		constexpr Span& operator=(const Span& other) noexcept
		{
			if (this == &other)
				return *this;

			m_Data = other.m_Data;
			m_Capacity = other.m_Capacity;

			return *this;
		}

		constexpr Span& operator=(Span&& other) noexcept
		{
			if (this == &other)
				return *this;

			m_Data = std::move(other.m_Data);
			m_Capacity = other.m_Capacity;

			other.m_Data = nullptr;
			other.m_Capacity = 0;

			return *this;
		}

        constexpr friend bool operator==(const Span<T>& left, const Span<T> right) noexcept { return left.Equals(right); }
        constexpr friend bool operator!=(const Span<T>& left, const Span<T> right) noexcept { return !(left == right); }

		friend std::ostream& operator<<(std::ostream& stream, const Span& current) noexcept
		{
			stream << "[";
			for (usize i = 0; i < current.m_Capacity; ++i)
			{
				stream << current.m_Data[i];

				if (i + 1 < current.m_Capacity)
					stream << ", ";
			}
			stream << "]";
			return stream;
		}


		/*
		 *  ============================================================
		 *	|                          Static                          |
		 *  ============================================================
		 */


		NODISCARD constexpr static Memory<T> TakeMemory(Span& span) noexcept
		{
			auto memory = span.m_Data;
			span.m_Data = nullptr;
			span.m_Capacity = 0;
			return memory;
		}

		/// <summary>
		/// Creates an empty Span.
		/// </summary>
		/// <returns>New instance of an empty Span</returns>
		NODISCARD constexpr static Span Empty() noexcept { return {}; }

	private:
		Memory<T> m_Data = nullptr;
		usize m_Capacity = 0;
	};


	/*
	 *  ============================================================
	 *	|                    Global Functions                      |
	 *  ============================================================
	 */


	/// <summary>
	/// Hashes the Span to produce a unique hash code.
	/// </summary>
	/// <param name="object">Span to hash</param>
	/// <returns>Hash code as a 'usize'</returns>
	template <typename T>
	NODISCARD usize Hash(const Span<T>& object) noexcept
	{
		return typeid(T).hash_code() + typeid(Span<T>).hash_code() + object.Capacity();
	}

	template <typename T>
	NODISCARD constexpr bool Equals(const Span<T>& left, const Span<T>& right) noexcept { return left.Equals(right); }

	template <typename T>
	NODISCARD constexpr bool Contains(const Span<T>& span, const T& element) noexcept { return span.Contains(element); }

	template <typename T>
	NODISCARD constexpr Optional<usize> IndexOf(const Span<T>& span, const T& element) noexcept { return span.IndexOf(element); }

	/// <summary>
	/// Tries to find the last index of the given element.
	/// </summary>
	/// <param name="span">Span to search through</param>
	/// <param name="element">Element to find</param>
	/// <returns>Last index of the element in the Span, or an invalid result if not found</returns>
	template <typename T>
	NODISCARD constexpr Optional<usize> LastIndexOf(const Span<T>& span, const T& element) noexcept { return span.LastIndexOf(element); }

	template <typename T>
	NODISCARD constexpr bool Exists(const Span<T>& span, const Predicate<T>& predicate) noexcept { return span.Exists(predicate); }

	template <typename T>
	NODISCARD constexpr usize Count(const Span<T>& span, const Predicate<T>& predicate) noexcept { return span.Count(predicate); }

	template <typename T>
	constexpr void Reverse(Span<T>& span) noexcept { span.Reverse(); }

	template <typename T>
	constexpr bool Swap(Span<T>& span, const usize index1, const usize index2) noexcept { return span.Swap(index1, index2); }
}
