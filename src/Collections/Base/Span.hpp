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
		 *	|                          Aliases                         |
		 *  ============================================================
		 */


		using Memory = Memory<T>;

		
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

		constexpr Span(const T* data, const size_t size) noexcept
			: m_Data(const_cast<T*>(data)), m_Capacity(size)
		{
		}

		constexpr Span(T* data, const size_t size) noexcept
			: m_Data(data), m_Capacity(size)
		{
		}

		template <size_t TSize>
		constexpr Span(const T (&data)[TSize]) noexcept
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


		NODISCARD constexpr size_t Capacity() const noexcept { return m_Capacity; }
		NODISCARD constexpr const T* Data() const noexcept { return m_Data; }
		NODISCARD constexpr T* Data() noexcept { return m_Data; }
		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Data == nullptr; }

		/* Enumerators (Iterators) */

		NODISCARD Enumerator<T> GetEnumerator() override
		{
			for (size_t i = 0; i < m_Capacity; i++)
			{
				auto& element = m_Data[i];
				co_yield element;
			}
		}

		NODISCARD Enumerator<T> GetEnumerator() const override
		{
			for (size_t i = 0; i < m_Capacity; i++)
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


		NODISCARD constexpr Span Slice(const size_t start) const noexcept
		{
			if (start >= m_Capacity)
				return Empty();

			return { m_Data + start, m_Data + m_Capacity };
		}

		NODISCARD constexpr Span Slice(const size_t start, const size_t length) const noexcept
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

			for (size_t i = 0; i < m_Capacity; i++)
			{
				if (m_Data[i] != span[i])
					return false;
			}

			return true;
		}

		NODISCARD constexpr bool Contains(const T& element) const noexcept
		{
			for (size_t i = 0; i < m_Capacity; i++)
				if (m_Data[i] == element)
					return true;
			return false;
		}

		NODISCARD constexpr Optional<size_t> IndexOf(const T& element) const noexcept
		{
			for (size_t i = 0; i < m_Capacity; ++i)
				if (m_Data[i] == element)
					return Optional<size_t>(i);

			return Optional<size_t>::Empty();
		}

		NODISCARD constexpr Optional<size_t> LastIndexOf(const T& element) const noexcept
		{
			for (size_t i = 0; i < m_Capacity; ++i)
			{
				const size_t offset = m_Capacity - 1 - i;
				if (m_Data[offset] == element)
					return Optional<size_t>(offset);
			}

			return Optional<size_t>::Empty();
		}

		NODISCARD constexpr bool Exists(const Predicate<T>& predicate) const noexcept
		{
			for (size_t i = 0; i < m_Capacity; ++i)
				if (predicate(m_Data[i]))
					return true;
			return false;
		}

		NODISCARD constexpr uint64_t CountBy(const Predicate<T>& predicate) const noexcept
		{
			uint64_t count = 0;
			for (size_t i = 0; i < m_Capacity; ++i)
				if (predicate(m_Data[i]))
					++count;
			return count;
		}

		constexpr void Reverse() noexcept
		{
			const size_t length = m_Capacity / 2;
			for (size_t i = 0; i < length; ++i)
			{
				const size_t swapIndex = m_Capacity - i - 1;
				const auto temp = std::move(m_Data[i]);
				m_Data[i] = m_Data[swapIndex];
				m_Data[swapIndex] = temp;
			}
		}

		constexpr bool Swap(const size_t index1, const size_t index2) noexcept
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
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		NODISCARD Optional<T&> operator[](const size_t index) noexcept
		{
			if (index >= m_Capacity)
				return Optional<T&>::Empty();

			return Optional<T&>(m_Data[index]);
		}

		NODISCARD Optional<const T&> operator[](const size_t index) const noexcept
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

		friend std::ostream& operator<<(std::ostream& stream, const Span& current) noexcept
		{
			stream << "[";
			for (size_t i = 0; i < current.m_Capacity; ++i)
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


		/// <summary>
		/// Creates an empty Span.
		/// </summary>
		/// <returns>New instance of an empty Span</returns>
		NODISCARD constexpr static Span Empty() noexcept { return {}; }

	private:
		Memory m_Data = nullptr;
		size_t m_Capacity = 0;
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
	/// <returns>Hash code as a 'size_t'</returns>
	template <typename T>
	NODISCARD size_t Hash(const Span<T>& object) noexcept
	{
		return typeid(T).hash_code() + typeid(Span<T>).hash_code() + object.Capacity();
	}

	template <typename T>
	NODISCARD constexpr bool Equals(const Span<T>& left, const Span<T>& right) noexcept { return left.Equals(right); }

	template <typename T>
	NODISCARD constexpr bool Contains(const Span<T>& span, const T& element) noexcept { return span.Contains(element); }

	template <typename T>
	NODISCARD constexpr Optional<size_t> IndexOf(const Span<T>& span, const T& element) noexcept { return span.IndexOf(element); }

	/// <summary>
	/// Tries to find the last index of the given element.
	/// </summary>
	/// <param name="span">Span to search through</param>
	/// <param name="element">Element to find</param>
	/// <returns>Last index of the element in the Span, or an invalid result if not found</returns>
	template <typename T>
	NODISCARD constexpr Optional<size_t> LastIndexOf(const Span<T>& span, const T& element) noexcept { return span.LastIndexOf(element); }

	template <typename T>
	NODISCARD constexpr bool Exists(const Span<T>& span, const Predicate<T>& predicate) noexcept { return span.Exists(predicate); }

	template <typename T>
	NODISCARD constexpr uint64_t CountBy(const Span<T>& span, const Predicate<T>& predicate) noexcept { return span.CountBy(predicate); }

	template <typename T>
	constexpr void Reverse(Span<T>& span) noexcept { span.Reverse(); }

	template <typename T>
	constexpr bool Swap(Span<T>& span, const size_t index1, const size_t index2) noexcept { return span.Swap(index1, index2); }
}
