#pragma once
#include "Collections/Base/Collection.hpp"
#include "Utility/Result.hpp"

namespace Micro
{
	template <typename T>
	class List final : public ContiguousCollection<T>
	{
	public:
		// Aliases
		using Base = ContiguousCollection<T>;
		using Sequence = Sequence<T>;

		// Constructors/Destructors
		constexpr List() noexcept : Base()
		{
		}

		constexpr List(const List& other) noexcept : Base(other)
		{
		}

		constexpr List(List&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr List(const Base& other) noexcept : Base(other)
		{
		}

		constexpr List(Base&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr List(const Sequence& other) noexcept : Base(other)
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

		explicit List(const size_t capacity) noexcept : Base()
		{
			Reserve(capacity);
		}

		constexpr ~List() noexcept override = default;

		// Utility
		void Add(const T& value) noexcept
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			Base::m_Data[Base::m_Size++] = value;
		}

		void Add(T&& value) noexcept
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			Base::m_Data[Base::m_Size++] = std::move(value);
		}

		template <typename... Args>
		T& Emplace(Args&&... args) noexcept
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			new(&Base::m_Data[Base::m_Size]) T(std::forward<Args>(args)...);
			return Base::m_Data[Base::m_Size++];
		}

		void AddRange(const Base& container) noexcept
		{
			if (container.IsEmpty())
				return;

			const size_t size = container.Size();
			const T* data = container.Data();
			for (size_t i = 0; i < size; i++)
				Add(data[i]);
		}

		void AddRange(Base&& container) noexcept
		{
			if (container.IsEmpty())
				return;

			const size_t size = container.Size();
			T* data = const_cast<T*>(container.Data());
			for (size_t i = 0; i < size; i++)
				Add(std::move(data[i]));
		}

		void AddRange(T firstElem, std::convertible_to<T> auto... elements) noexcept
		{
			for (auto values = {firstElem, static_cast<T>(std::move(elements))...}; auto&& item : values)
				Add(std::move(item));
		}

		void Insert(const size_t index, const T& value)
		{
			if (Base::m_Size <= index)
				throw ArgumentOutOfRangeException(NAMEOF(index), index);

			if (Base::m_Capacity <= Base::m_Size + 1)
				Base::Reallocate(Base::m_Capacity * 2);

			ShiftRight(Base::m_Data.Data, Base::m_Size, index);

			Base::m_Data[index] = value;
			++Base::m_Size;
		}

		void Insert(const size_t index, T&& value)
		{
			if (Base::m_Size <= index)
				throw ArgumentOutOfRangeException(NAMEOF(index), index);

			if (Base::m_Capacity <= Base::m_Size + 1)
				Base::Reallocate(Base::m_Capacity * 2);

			ShiftRight(Base::m_Data.Data, Base::m_Size, index);

			Base::m_Data[index] = std::move(value);
			++Base::m_Size;
		}

		void InsertRange(const size_t startIndex, const Base& container)
		{
			// Check for valid sequence
			if (container.IsEmpty()) 
				return;

			// Index validation
			if (Base::m_Size <= startIndex)
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

		void InsertRange(const size_t startIndex, Base&& container)
		{
			// Check for valid sequence
			if (container.IsEmpty()) return;

			// Index validation
			if (Base::m_Size <= startIndex)
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
			if (Base::m_Size <= startIndex)
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
			const int32_t index = Micro::IndexOf(*this, value);
			if (index == -1)
				return false;

			// Dispose
			RemoveAt(index);
			return true;
		}

		void RemoveAt(const size_t index)
		{
			if (index >= Base::m_Size)
				throw ArgumentOutOfRangeException(NAMEOF(index), index);

			// Free memory
			Base::m_Data[index].~T();

			// Shift items down (+1 to shift invalid data to the end)
			ShiftLeft(Base::m_Data, Base::m_Size, index + 1);

			// Decrement size
			--Base::m_Size;
		}

		void RemoveRange(const size_t index, const size_t count)
		{
			// TODO: Finish implementation
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

		NODISCARD RefResult<T> Find(const Predicate<T>& predicate) const
		{
			for (size_t i = 0; i < Base::m_Size; i++)
			{
				const T& elem = Base::m_Data[i];
				if (predicate(elem))
					return RefResult(elem);
			}

			return RefResult<T>::Empty();
		}

		NODISCARD Result<size_t> FindIndex(const Predicate<T>& predicate) const
		{
			for (size_t i = 0; i < Base::m_Size; i++)
				if (predicate(Base::m_Data[i]))
					return Result<size_t>(i);
			return Result<size_t>::Empty();
		}

		NODISCARD RefResult<T> FindLast(const Predicate<T>& predicate) const
		{
			for (size_t i = Base::m_Size; i > 0; --i)
			{
				const T& elem = Base::m_Data[i - 1ULL];
				if (predicate(elem))
					return RefResult(elem);
			}

			return RefResult<T>::Empty();
		}

		NODISCARD Result<size_t> FindLastIndex(const Predicate<T>& predicate) const
		{
			for (size_t i = Base::m_Size; i > 0; --i)
			{
				const T& elem = Base::m_Data[i - 1ULL];
				if (predicate(elem))
					return Result<size_t>(i - 1ULL);
			}
			return Result<size_t>::Empty();
		}

		NODISCARD List FindAll(const Predicate<T>& predicate) const
		{
			List list(Base::m_Size);

			for (size_t i = 0; i < Base::m_Size; i++)
			{
				const T& elem = Base::m_Data[i];
				if (predicate(elem))
					list.Emplace(elem);
			}

			return list;
		}

		// Accessors
		NODISCARD constexpr void Reserve(const size_t capacity) noexcept { Base::Reallocate(capacity); }

		// Operator Overloads
		NODISCARD const T& operator[](const size_t index) const
		{
			if (index >= Base::m_Size)
				throw IndexOutOfRangeException(index);

			return Base::m_Data[index];
		}

		NODISCARD T& operator[](const size_t index)
		{
			if (index >= Base::m_Size)
				throw IndexOutOfRangeException(index);

			return Base::m_Data[index];
		}

		List& operator=(const List& other)
		{
			// Validation
			if (this == &other)
				return *this;

			const size_t size = other.m_Capacity;
			if (size == 0)
				return *this;

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

	private:
		static constexpr size_t DefaultCapacity = 4;
	};
}
