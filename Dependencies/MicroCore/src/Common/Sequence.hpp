#pragma once
#include <ostream>

#include "Core/Core.hpp"
#include "Core/Function.hpp"
#include "Core/Memory.hpp"
#include "Core/Exceptions/Exception.hpp"

namespace mtk
{
	template <typename T>
	class Iterator
	{
	public:
		using ValueType = T;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;

	public:
		Iterator(PointerType ptr)
			: m_Ptr(ptr)
		{
		}

		Iterator& operator++()
		{
			++m_Ptr;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator iterator = *this;
			++*this;
			return *this;
		}

		Iterator& operator++() const
		{
			++m_Ptr;
			return *this;
		}

		Iterator operator++(int) const
		{
			Iterator iterator = *this;
			++*this;
			return *this;
		}

		Iterator& operator--()
		{
			--m_Ptr;
			return this;
		}

		Iterator operator--(int)
		{
			Iterator iterator = *this;
			--*this;
			return *this;
		}

		Iterator& operator--() const
		{
			--m_Ptr;
			return this;
		}

		Iterator operator--(int) const
		{
			Iterator iterator = *this;
			--*this;
			return *this;
		}

		ReferenceType operator[](size_t index)
		{
			return *(m_Ptr + index);
		}

		ReferenceType operator[](size_t index) const
		{
			return *(m_Ptr + index);
		}

		PointerType operator->()
		{
			return m_Ptr;
		}

		ReferenceType operator*()
		{
			return *m_Ptr;
		}

		bool operator==(const Iterator& other) const
		{
			return m_Ptr == other.m_Ptr;
		}

		bool operator!=(const Iterator& other) const
		{
			return !(*this == other);
		}

	private:
		PointerType m_Ptr = nullptr;
	};


	template <typename T>
	class Sequence : public Memory<T>
	{
	public:
		using Iterator = Iterator<T>;
		using ConstIterator = const Iterator;

		// Constructors/Destructors
		Sequence() = default;

		Sequence(const Sequence& other)
		{
			const size_t length = other.m_Capacity;
			if (length == 0) return;

			Allocate(length);

			for (size_t i = 0; i < m_Capacity; ++i)
				m_Data[i] = other.m_Data[i];
		}

		Sequence(Sequence&& other) noexcept
			: m_Data(other.m_Data), m_Capacity(other.m_Capacity)
		{
			other.m_Data = nullptr;
			other.m_Capacity = 0;
		}

		Sequence(const T* data, size_t size)
		{
			const size_t length = size;
			if (length == 0) return;

			Allocate(length);

			for (size_t i = 0; i < m_Capacity; ++i)
				m_Data[i] = data[i];
		}

		Sequence(std::initializer_list<T>&& data)
		{
			const size_t length = data.size();
			if (length == 0) return;

			Allocate(length);

			size_t index = 0;
			for (const T& elem : data)
				m_Data[index++] = std::move(const_cast<T&>(elem));
		}

		explicit Sequence(size_t size) { Allocate(size); }

		~Sequence() override { delete[] m_Data; }

		// Utility
		NODISCARD bool IsEmpty() const override { return m_Capacity == 0; }
		NODISCARD bool Contains(const T& item)
		{
			for (size_t i = 0; i < m_Capacity; i++)
			{
				if (m_Data[i] == item)
					return true;
			}
			return false;
		}

		NODISCARD void Clear()
		{
			delete[] m_Data;
			m_Data = nullptr;
			m_Capacity = 0;
		}

		NODISCARD constexpr Sequence& AsSequence() const { return *this; }

		// Accessors
		NODISCARD constexpr size_t Capacity() const override { return m_Capacity; }
		NODISCARD constexpr const T* Data() const override { return m_Data; }

		// Iterators
		NODISCARD constexpr Iterator begin() { return {m_Data}; }
		NODISCARD constexpr Iterator end() { return {m_Data + m_Capacity}; }
		NODISCARD constexpr ConstIterator begin() const { return {m_Data}; }
		NODISCARD constexpr ConstIterator end() const { return {m_Data + m_Capacity}; }

		// Operator Overloads
		T& operator[](const size_t index)
		{
			if (index >= m_Capacity || index < 0)
				throw Exception("The index of '{}' is out of range.", index);

			return m_Data[index];
		}

		const T& operator[](const size_t index) const
		{
			if (index >= m_Capacity || index < 0)
				throw Exception("The index of '{}' is out of range.", index);

			return m_Data[index];
		}

		Sequence& operator=(const Sequence& other)
		{
			if (this == &other)
				return *this;

			const size_t length = other.m_Capacity;
			if (m_Data == nullptr)
			{
				Allocate(length);
				memcpy_s(m_Data, length, other.m_Data, length);
			}
			else
			{
				Reallocate(length);
				memcpy_s(m_Data, length, other.m_Data, length);
			}

			return *this;
		}

		Sequence& operator=(Sequence&& other) noexcept
		{
			if (this == &other)
				return *this;

			m_Data = other.m_Data;
			m_Capacity = other.m_Capacity;

			other.m_Data = nullptr;
			other.m_Capacity = 0;

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const Sequence& current)
		{
			if (current.m_Capacity > 0)
			{
				stream << "[";
				for (size_t i = 0; i < current.m_Capacity; ++i)
				{
					stream << current.m_Data[i];

					if (i + 1 < current.m_Capacity)
						stream << ", ";
				}
				stream << "]";
			}
			else
			{
				stream << "";
			}
			return stream;
		}

	protected:
		virtual void Allocate(size_t size)
		{
			m_Data = new T[size];
			m_Capacity = size;
		}

		virtual void Reallocate(size_t size)
		{
			if (size != m_Capacity)
			{
				delete[] m_Data;
				Allocate(size);
			}
			else
			{
				for (size_t i = 0; i < m_Capacity; i++)
					m_Data[i].~T();
			}
		}

	protected:
		T* m_Data = nullptr;
		size_t m_Capacity = 0;
	};


	template <typename T>
	NODISCARD int32_t IndexOf(const Sequence<T>& sequence, const T& item)
	{
		const size_t size = sequence.m_Capacity;
		for (size_t i = 0; i < size; ++i)
		{
			if (sequence.m_Data[i] == item)
				return i;
		}

		return -1;
	}

	template <typename T>
	NODISCARD int32_t CountBy(const Sequence<T>& sequence, Predicate<T> predicate)
	{
		int32_t count = 0;
		for (const T& elem : sequence)
		{
			if (predicate(elem))
				++count;
		}
		return count;
	}

	template <typename T>
	NODISCARD bool Contains(const Sequence<T>& sequence, const T& item)
	{
		for (const T& elem : sequence)
		{
			if (elem == item)
				return true;
		}
		return false;
	}

	template <typename T>
	NODISCARD bool Contains(const Sequence<T>& sequence, const Predicate<T>& predicate)
	{
		for (const T& elem : sequence)
		{
			if (predicate(elem))
				return true;
		}
		return false;
	}

	template <typename T>
	void Reverse(Sequence<T>& sequence)
	{
		const size_t length = sequence.m_Capacity / 2;
		for (size_t i = 0; i < length; ++i)
		{
			const size_t swapIndex = sequence.m_Capacity - i - 1;
			const T temp = std::move(sequence.m_Data[i]);
			sequence.m_Data[i] = sequence.m_Data[swapIndex];
			sequence.m_Data[swapIndex] = temp;
		}
	}

	template <typename T>
	void Swap(Sequence<T>& sequence, size_t index1, size_t index2)
	{
		if (index1 > sequence.m_Capacity || index2 > sequence.m_Capacity) return;

		const T temp = sequence.m_Data[index1];
		sequence.m_Data[index1] = sequence.m_Data[index2];
		sequence.m_Data[index2] = temp;
	}
}
