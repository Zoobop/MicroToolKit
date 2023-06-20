#pragma once
#include <ostream>

#include "Core/Memory.hpp"
#include "Collections/Container.hpp"
#include "Common/Sequence.hpp"
#include "Utility/Range.h"


namespace mtk
{
	template <typename T>
	class List final : public Container<T, Allocator<T>>
	{
	public:
		using Base = Container<T, Allocator<T>>;
		using Sequence = Sequence<T>;

		friend Base;

		// Constructors/Destructors
		constexpr List() noexcept : Base()
		{
		}

		constexpr List(const List& other) : Base(other)
		{
		}

		constexpr List(List&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr List(const Base& other) : Base(other)
		{
		}

		constexpr List(Base&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr List(const Sequence& other) : Base(other)
		{
		}

		constexpr List(Sequence&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr List(std::initializer_list<T>&& initializerList) noexcept : Base(std::move(initializerList))
		{
		}

		explicit constexpr List(std::convertible_to<T> auto... elements) noexcept : Base(
			std::forward<T>(static_cast<T>(elements))...)
		{
		}

		explicit List(const size_t capacity) : Base()
		{
			Base::Allocate(capacity);
		}

		~List() override = default;

		// Utility
		void Add(const T& value)
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(c_DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			new(&Base::m_Data[Base::m_Size++]) T(value);
		}

		void Add(T&& value)
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(c_DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			new(&Base::m_Data[Base::m_Size++]) T(std::move(value));
		}

		template <typename... Args>
		T& Emplace(Args&&... args) noexcept
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(c_DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			new(&Base::m_Data[Base::m_Size]) T(std::forward<Args>(args)...);
			return Base::m_Data[Base::m_Size++];
		}

		void AddRange(const Base& container)
		{
			if (container.IsEmpty())
				return;

			const size_t size = container.Size();
			const T* data = container.Data();
			for (size_t i = 0; i < size; i++)
				Add(data[i]);
		}

		void AddRange(Base&& container)
		{
			if (container.IsEmpty())
				return;

			const size_t size = container.Size();
			T* data = const_cast<T*>(container.Data());
			for (size_t i = 0; i < size; i++)
				Add(std::move(data[i]));
		}

		void AddRange(T firstElem, std::convertible_to<T> auto... elements)
		{
			for (auto values = {firstElem, static_cast<T>(std::move(elements))...}; auto&& item : values)
				Add(std::move(item));
		}

		void Insert(const size_t index, const T& value)
		{
			if (Base::m_Size <= index || index < 0)
				throw ArgumentOutOfRangeException(NAMEOF(index), index);

			if (Base::m_Capacity <= Base::m_Size + 1)
				Base::Reallocate(Base::m_Capacity * 2);

			ShiftRight(Base::m_Data.Data, Base::m_Size, index);

			new(&Base::m_Data[index]) T(value);
			++Base::m_Size;
		}

		void Insert(const size_t index, T&& value)
		{
			if (Base::m_Size <= index || index < 0)
				throw ArgumentOutOfRangeException(NAMEOF(index), index);

			if (Base::m_Capacity <= Base::m_Size + 1)
				Base::Reallocate(Base::m_Capacity * 2);

			ShiftRight(Base::m_Data.Data, Base::m_Size, index);

			new(&Base::m_Data[index]) T(std::move(value));
			++Base::m_Size;
		}

		void InsertRange(size_t startIndex, const Base& container)
		{
			// Check for valid sequence
			if (container.IsEmpty()) return;

			// Index validation
			if (Base::m_Size <= startIndex || startIndex < 0)
				throw ArgumentOutOfRangeException(NAMEOF(startIndex), startIndex);

			// Reallocate, if too small
			const size_t length = container.Capacity();
			if (Base::m_Capacity <= Base::m_Size + length)
				Base::Reallocate(MAX(Base::m_Capacity * 2, Base::m_Size + length));

			// Shift right to make space for new data
			ShiftRight(Base::m_Data, Base::m_Size, startIndex, length);

			// Assign data
			size_t index = startIndex;
			for (auto& elem : container)
				Base::m_Data[index++] = elem;

			// Increment size
			Base::m_Size += length;
		}

		void InsertRange(size_t startIndex, Base&& container)
		{
			// Check for valid sequence
			if (container.IsEmpty()) return;

			// Index validation
			if (Base::m_Size <= startIndex || startIndex < 0)
				throw ArgumentOutOfRangeException(NAMEOF(startIndex), startIndex);

			// Reallocate, if too small
			const size_t length = container.Capacity();
			if (Base::m_Capacity <= Base::m_Size + length)
				Base::Reallocate(MAX(Base::m_Capacity * 2, Base::m_Size + length));

			// Shift right to make space for new data
			ShiftRight(Base::m_Data, Base::m_Size, startIndex, length);

			// Assign data
			size_t index = startIndex;
			for (auto& elem : container)
				Base::m_Data[index++] = std::move(elem);

			// Increment size
			Base::m_Size += length;

			// Clear sequence
			container.Clear();
		}

		void InsertRange(const size_t startIndex, T firstElem, std::convertible_to<T> auto... elements)
		{
			// Index validation
			if (Base::m_Size <= startIndex || startIndex < 0)
				throw ArgumentOutOfRangeException(NAMEOF(startIndex), startIndex);

			// Reallocate, if too small
			const size_t length = 1 + sizeof ...(elements);
			if (Base::m_Capacity <= Base::m_Size + length)
				Base::Reallocate(MAX(Base::m_Capacity * 2, Base::m_Size + length));

			// Shift right to make space for new data
			ShiftRight(Base::m_Data, Base::m_Size, startIndex, length);

			// Assign first arg
			Base::m_Data[startIndex] = std::move(firstElem);

			// Assign parameter args
			size_t index = startIndex + 1;
			for (auto values = {static_cast<T>(elements)...}; auto& elem : values)
				Base::m_Data[index++] = std::move(elem);

			// Increment size
			Base::m_Size += length;
		}

		bool Remove(const T& value)
		{
			// Return if empty
			if (Base::IsEmpty())
				return false;

			// Get and return if invalid index
			const int32_t index = Base::IndexOf(*this, value);
			if (index == -1)
				return false;

			// Dispose
			RemoveAt(index);
			return true;
		}

		size_t RemoveAll(const Predicate<T>& predicate)
		{
			// TODO: Finish implementation
			// Find indexes of elements matching predicate
			size_t count = 0;
			for (size_t i = 0; i < Base::m_Size;)
			{
				if (predicate(Base::m_Data[i]))
				{
					count++;
					ShiftRight(Base::m_Data, Base::m_Size - count, i);
				}

				i++;
			}

			Base::m_Size -= count;
			return count;
		}

		void RemoveAt(const size_t index)
		{
			if (index >= Base::m_Size || index < 0)
				return;

			// Free memory
			Base::m_Data[index].~T();

			// Shift items down (+1 to shift invalid data to the end)
			ShiftLeft(Base::m_Data, Base::m_Size, index + 1);

			// Decrement size
			--Base::m_Size;
		}

		void RemoveRange(size_t _index, size_t _count)
		{
			// TODO: Finish implementation
		}

		NODISCARD bool Contains(const T& value) const
		{
			return mtk::Contains(*this, value);
		}

		NODISCARD bool Exists(const Predicate<T>& _predicate) const
		{
			for (size_t i = 0; i < Base::m_Size; i++)
			{
				if (_predicate(Base::m_Data[i])) return true;
			}

			return false;
		}

		NODISCARD T* Find(const Predicate<T>& _predicate) const
		{
			T item = nullptr;
			for (size_t i = 0; i < Base::m_Size; i++)
			{
				const T& elem = Base::m_Data[i];
				if (_predicate(elem))
				{
					item = elem;
					break;
				}
			}
			return item;
		}

		NODISCARD int32_t FindIndex(const Predicate<T>& _predicate) const
		{
			int32_t index = -1;
			for (size_t i = 0; i < Base::m_Size; i++)
			{
				const T& elem = Base::m_Data[i];
				if (_predicate(elem))
				{
					index = static_cast<int32_t>(i);
					break;
				}
			}
			return index;
		}

		NODISCARD T* FindLast(const Predicate<T>& _predicate) const
		{
			T item = nullptr;
			for (size_t i = 0; i < Base::m_Size; i++)
			{
				const T& elem = Base::m_Data[i];
				if (_predicate(elem))
				{
					item = elem;
				}
			}

			return item;
		}

		NODISCARD int32_t FindLastIndex(const Predicate<T>& _predicate) const
		{
			int32_t index = -1;
			for (size_t i = 0; i < Base::m_Size; i++)
			{
				const T& elem = Base::m_Data[i];
				if (_predicate(elem))
				{
					index = static_cast<int32_t>(i);
				}
			}
			return index;
		}

		NODISCARD List<T> FindAll(const Predicate<T>& _predicate) const
		{
			List<T> list(Base::m_Size);
			for (size_t i = 0; i < Base::m_Size; i++)
			{
				const T& elem = Base::m_Data[i];
				if (_predicate(elem))
				{
					list.Add(elem);
				}
			}
			return list;
		}

		// Accessors
		NODISCARD constexpr void Reserve(const size_t capacity) { Base::Reallocate(capacity); }

		// Operator Overloads
		operator Sequence() const { return {Base::m_Data, Base::m_Size}; }

		NODISCARD const T& operator[](const size_t index) const
		{
			if (index >= Base::m_Size || index < 0)
				throw IndexOutOfRangeException(index);

			return Base::m_Data[index];
		}

		NODISCARD Base operator[](Range range) const
		{
			if (range.end >= Base::m_Size || range.start >= range.end)
				throw ArgumentOutOfRangeException(NAMEOF(range));

			return {Base::m_Data + range.start, range.end - range.start};
		}

		List& operator=(const List& other)
		{
			// Validation
			if (this == &other)
				return *this;

			const size_t size = other.m_Capacity;
			if (size == 0) return *this;

			// Allocation
			if (!Base::IsEmpty())
				Reallocate(other.m_Capacity);
			else
				Allocate(other.m_Capacity);

			// Assignment
			Copy(other.m_Data, other.m_Size, Base::m_Data, Base::m_Capacity);
			return *this;
		}

		List& operator=(List&& other) noexcept
		{
			// Validation
			if (this == &other)
				return *this;

			const size_t size = other.m_Capacity;
			if (size == 0) return *this;

			// Allocation
			if (!Base::IsEmpty())
				Reallocate(other.m_Capacity);
			else
				Allocate(other.m_Capacity);

			// Assignment
			Move(other.m_Data, other.m_Size, Base::m_Data, Base::m_Capacity);
			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const List& current)
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

	private:
		static constexpr size_t c_DefaultCapacity = 4;
	};
}
