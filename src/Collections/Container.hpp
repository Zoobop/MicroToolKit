#pragma once
#include <ostream>

#include "Core/Allocator.hpp"

namespace mtk
{
	template <typename T, typename TAllocator = Allocator<T>>
	class Container
	{
	public:
		using Memory = Memory<T>;
		using AllocatorProxy = AllocatorProxy<T, TAllocator>;
		using Iterator = Iterator<T>;
		using ConstIterator = const Iterator;

		Container() = default;

		Container(const Container& other)
		{
			if (other.m_Data == nullptr)
				return;

			Allocate(other.m_Capacity);

			auto result = Copy(m_Data, m_Capacity, other.m_Data, other.m_Capacity);
			if (result == Error)
				throw BadCopyException();
		}

		Container(Container&& other) noexcept
			: m_Data(other.m_Data), m_Size(other.m_Size), m_Capacity(other.m_Capacity)
		{
			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;
		}

		Container(std::initializer_list<T>&& initializerList) noexcept
		{
			const size_t length = initializerList.size();
			if (length == 0)
				return;

			Allocate(length);

			for (auto& elem : initializerList)
				m_Data[m_Size++] = std::move(const_cast<T&>(elem));
		}

		explicit Container(std::convertible_to<T> auto... elements) noexcept
		{
			// Get number of elements (arg count)
			const size_t length = sizeof ...(elements);

			Allocate(length);

			// Move values in data block
			for (auto values = {elements...}; auto&& elem : values)
				new(&m_Data[m_Size++]) T(std::move(elem));
		}

		virtual ~Container()
		{
			// Invalidate memory, then free
			AllocatorProxy::ClearMemory(m_Data, m_Size);
			AllocatorProxy::Dispose(m_Data, m_Capacity);
		}

		virtual void Clear()
		{
			// Invalidate data
			AllocatorProxy::ClearMemory(m_Data, m_Size);
			m_Size = 0;
		}

		NODISCARD bool Contains(const T& value)
		{
			for (size_t i = 0; i < m_Size; i++)
				if (m_Data[i] == value)
					return true;

			return false;
		}

		NODISCARD constexpr bool IsEmpty() const { return m_Size == 0; }
		NODISCARD constexpr const T* Data() const { return m_Data; }
		NODISCARD constexpr size_t Size() const { return m_Size; }
		NODISCARD constexpr size_t Capacity() const { return m_Capacity; }

		// Iterators
		NODISCARD constexpr Iterator begin() { return {m_Data}; }
		NODISCARD constexpr Iterator end() { return {m_Data + m_Size}; }
		NODISCARD constexpr ConstIterator begin() const { return {m_Data}; }
		NODISCARD constexpr ConstIterator end() const { return {m_Data + m_Size}; }

		NODISCARD Container& operator=(const Container& other)
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

			// Assignment
			auto result = Copy(m_Data, m_Capacity, other.m_Data, other.m_Capacity);
			if (result == Error)
				throw BadCopyException();

			return *this;
		}

		NODISCARD Container& operator=(Container&& other) noexcept
		{
			if (this == &other)
				return *this;

			m_Data = other.m_Data;
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;

			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const Container& current)
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
		void Allocate(const size_t capacity)
		{
			m_Capacity = AllocatorProxy::Allocate(m_Data, m_Capacity, capacity);
		}

		void Reallocate(const size_t capacity)
		{
			m_Capacity = AllocatorProxy::Reallocate(m_Data, m_Capacity, capacity);
		}

	protected:
		Memory m_Data = nullptr;
		size_t m_Size = 0;
		size_t m_Capacity = 0;
	};
}
