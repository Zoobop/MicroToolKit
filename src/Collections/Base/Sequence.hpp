#pragma once
#include <ostream>

#include "Core/Function.hpp"
#include "Core/Hash.hpp"
#include "Core/Memory/Memory.hpp"
#include "Collections/Base/Iterator.hpp"

namespace Micro
{
	template <typename T>
	class Sequence final
	{
	public:
		using Iterator = ContiguousIterator<T>;
		using ConstIterator = const Iterator;
		using Memory = Memory<T>;

		// Constructors/Destructors
		constexpr Sequence() noexcept = default;

		constexpr Sequence(const Sequence& other) noexcept
			: m_Data(other.m_Data), m_Capacity(other.m_Capacity)
		{
		}

		constexpr Sequence(Sequence&& other) noexcept
			: m_Data(other.m_Data), m_Capacity(other.m_Capacity)
		{
		}

		constexpr Sequence(const T* data, const size_t size) noexcept
			: m_Data(const_cast<T*>(data)), m_Capacity(size)
		{
		}

		constexpr Sequence(T* data, const size_t size) noexcept
			: m_Data(data), m_Capacity(size)
		{
		}

		constexpr Sequence(const T* begin, const T* end) noexcept
			: m_Data(begin), m_Capacity(end - begin)
		{
		}

		constexpr ~Sequence() noexcept = default;

		// Accessors
		NODISCARD constexpr size_t Capacity() const noexcept { return m_Capacity; }
		NODISCARD constexpr const T* Data() const noexcept { return m_Data; }
		NODISCARD constexpr T* Data() noexcept { return m_Data; }

		// Iterators
		NODISCARD constexpr Iterator begin() { return {m_Data.Data}; }
		NODISCARD constexpr Iterator end() { return {m_Data.Data + m_Capacity}; }
		NODISCARD constexpr ConstIterator begin() const { return {m_Data.Data}; }
		NODISCARD constexpr ConstIterator end() const { return {m_Data.Data + m_Capacity}; }

		// Operator Overloads
		NODISCARD T& operator[](const size_t index)
		{
			if (index >= m_Capacity)
				throw IndexOutOfRangeException(index);

			return m_Data[index];
		}

		NODISCARD const T& operator[](const size_t index) const
		{
			if (index >= m_Capacity)
				throw IndexOutOfRangeException(index);

			return m_Data[index];
		}

		Sequence& operator=(const Sequence& other) noexcept
		{
			if (this == &other)
				return *this;

			m_Data = other.m_Data;
			m_Capacity = other.m_Capacity;

			return *this;
		}

		Sequence& operator=(Sequence&& other) noexcept
		{
			if (this == &other)
				return *this;

			m_Data = std::move(other.m_Data);
			m_Capacity = other.m_Capacity;

			other.m_Data = nullptr;
			other.m_Capacity = 0;

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const Sequence& current)
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

	protected:
		Memory m_Data = nullptr;
		size_t m_Capacity = 0;
	};

	template <typename T>
	NODISCARD size_t Hash(const Sequence<T>& object) noexcept
	{
		return typeid(T).hash_code() + typeid(Sequence<T>).hash_code() + object.Capacity();
	}

	template <typename T>
	NODISCARD int32_t IndexOf(const Sequence<T>& sequence, const T& item)
	{
		const T* data = sequence.Data();
		const size_t capacity = sequence.Capacity();
		for (size_t i = 0; i < capacity; ++i)
			if (data[i] == item)
				return i;

		return -1;
	}

	template <typename T>
	NODISCARD int32_t CountBy(const Sequence<T>& sequence, const Predicate<const T&>& predicate)
	{
		int32_t count = 0;
		for (const T& elem : sequence)
			if (predicate(elem))
				++count;
		return count;
	}

	template <typename T>
	NODISCARD bool Contains(const Sequence<T>& container, const T& item)
	{
		for (const T& elem : container)
			if (elem == item)
				return true;
		return false;
	}

	template <typename T>
	NODISCARD bool Contains(const Sequence<T>& sequence, const Predicate<const T&>& predicate)
	{
		for (const T& elem : sequence)
			if (predicate(elem))
				return true;
		return false;
	}

	template <typename T>
	NODISCARD bool Exists(const Sequence<T>& sequence, const Predicate<const T&>& predicate)
	{
		const T* data = sequence.Data();
		const size_t capacity = sequence.Capacity();
		for (size_t i = 0; i < capacity; i++)
			if (predicate(data[i]))
				return true;

		return false;
	}

	template <typename T>
	void Reverse(Sequence<T>& sequence)
	{
		const size_t length = sequence.m_Capacity / 2;
		for (size_t i = 0; i < length; ++i)
		{
			const size_t swapIndex = sequence.m_Capacity - i - 1;
			const auto temp = std::move(sequence.m_Data[i]);
			sequence.m_Data[i] = sequence.m_Data[swapIndex];
			sequence.m_Data[swapIndex] = temp;
		}
	}

	template <typename T>
	void Swap(Sequence<T>& sequence, size_t index1, size_t index2)
	{
		if (index1 > sequence.m_Capacity || index2 > sequence.m_Capacity)
			return;

		const auto temp = std::move(sequence.m_Data[index1]);
		sequence.m_Data[index1] = std::move(sequence.m_Data[index2]);
		sequence.m_Data[index2] = std::move(temp);
	}
}
