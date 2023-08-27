#pragma once
#include "Collections/Base/Collection.hpp"
#include "Core/Errors/Error.hpp"
#include "Utility/Options/Result.hpp"

namespace Micro
{
	/**
	 * \brief Represents a dynamically-sized heap-allocating structure for manipulating and searching through a contiguous block of memory.
	 * \tparam T Type of elements in list
	 */
	template <typename T>
	class List final : public HeapCollection<T>
	{
	public:
		/*
		 *  ============================================================
		 *	|                          Aliases                         |
		 *  ============================================================
		 */


		using Base = HeapCollection<T>;

		
		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		/**
		 * \brief Initializes a new instance of the List class with an empty buffer.
		 */
		constexpr List() noexcept : Base()
		{
		}

		/**
		 * \brief Initializes a new instance of the List class by copying the value of the given List.
		 * \param list List to copy
		 */
		constexpr List(const List& list) noexcept : Base(list)
		{
		}

		/**
		 * \brief Initializes a new instance of the List class by moving the value of the given List.
		 * \param list List to move
		 */
		constexpr List(List&& list) noexcept : Base(std::move(list))
		{
		}

		/**
		 * \brief Initializes a new instance of the List class by copying the value of the given collection.
		 * \param collection Collection to copy
		 */
		constexpr List(const Base& collection) noexcept : Base(collection)
		{
		}

		/**
		 * \brief Initializes a new instance of the List class by moving the value of the given collection.
		 * \param collection Collection to move
		 */
		constexpr List(Base&& collection) noexcept : Base(std::move(collection))
		{
		}

		/**
		 * \brief Initializes a new instance of the List class by moving the value of the given initializer list.
		 * \param initializerList Initializer list to move
		 */
		constexpr List(std::initializer_list<T>&& initializerList) noexcept : Base(std::move(initializerList))
		{
		}

		/**
		 * \brief Initializes a new instance of the List class by copying the value of the given span.
		 * \param span Span to copy
		 */
		constexpr explicit List(const Span<T>& span) noexcept : Base(span)
		{
		}

		/**
		 * \brief Initializes a new instance of the List class by copying the given elements into the list.
		 * \param elements Elements to insert
		 */
		constexpr explicit List(std::convertible_to<T> auto... elements) noexcept : Base(
			std::forward<T>(static_cast<T>(elements))...)
		{
		}

		/**
		 * \brief Initializes a new instance of the List class with the given capacity.
		 * \param capacity Size to pre-allocate
		 */
		constexpr explicit List(const size_t capacity) noexcept : Base(capacity)
		{
		}

		/**
		 * \brief Frees the memory from the underlying buffer, then setting all values to their default state.
		 */
		constexpr ~List() noexcept override = default;

		
		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		/// <summary>
		/// Adds the given value to the list by copy.
		/// </summary>
		/// <param name="value">Value to add to list</param>
		constexpr void Add(const T& value) noexcept
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			Base::m_Data[Base::m_Size++] = value;
		}

		/// <summary>
		/// Adds the given value to the list by move.
		/// </summary>
		/// <param name="value">Value to add to list</param>
		constexpr void Add(T&& value) noexcept
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			Base::m_Data[Base::m_Size++] = std::move(value);
		}

		/// <summary>
		/// Uses the given arguments to create the value inside the list.
		/// </summary>
		/// <param name="args">Args to create value inside the list</param>
		///	<returns>Reference to the newly created value</returns>
		template <typename... Args>
		constexpr T& Emplace(Args&&... args) noexcept
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			new(&Base::m_Data[Base::m_Size]) T(std::forward<Args>(args)...);
			return Base::m_Data[Base::m_Size++];
		}

		/// <summary>
		/// Adds all the elements from the given collection to the list by copy.
		/// </summary>
		/// <param name="collection">Collection to add to the list</param>
		constexpr void AddRange(const Base& collection) noexcept
		{
			if (collection.IsEmpty())
				return;

			const size_t size = collection.Size();
			const T* data = collection.Data();

			const size_t length = Base::m_Size + size;
			Base::HandleReallocation(length);

			for (size_t i = 0; i < size; i++)
				Add(data[i]);
		}

		/// <summary>
		/// Adds all the elements from the given collection to the list by moves.
		/// </summary>
		/// <param name="collection">Collection to add to the list</param>
		constexpr void AddRange(Base&& collection) noexcept
		{
			if (collection.IsEmpty())
				return;

			const size_t size = collection.Size();
			T* data = const_cast<T*>(collection.Data());

			const size_t length = Base::m_Size + size;
			Base::HandleReallocation(length);

			for (size_t i = 0; i < size; i++)
				Add(std::move(data[i]));
		}

		/// <summary>
		/// Adds all the elements from the given span to the list by copy.
		/// </summary>
		/// <param name="span">Span to add to the list</param>
		constexpr void AddRange(const Span<T>& span) noexcept
		{
			if (span.IsEmpty())
				return;

			const size_t size = span.Capacity();
			const T* data = span.Data();

			const size_t length = Base::m_Size + size;
			Base::HandleReallocation(length);

			for (size_t i = 0; i < size; i++)
				Add(data[i]);
		}

		/// <summary>
		/// Adds all the elements to the list by copy.
		/// </summary>
		/// <param name="elements">Elements to add to the list</param>
		constexpr void AddRange(std::convertible_to<T> auto... elements) noexcept
		{
			constexpr size_t argumentCount = sizeof ...(elements);
			static_assert(argumentCount != 0, "Cannot call 'AddRange' without any arguments!");

			const size_t length = Base::m_Size + argumentCount;
			Base::HandleReallocation(length);

			for (auto values = {static_cast<T>(std::move(elements))...}; auto&& item : values)
				Add(std::move(item));
		}

		constexpr Result<bool> Insert(const size_t index, const T& value) noexcept
		{
			if (Base::m_Size <= index)
				return Result<bool>::Error(ArgumentOutOfRangeError{ NAMEOF(index), index });

			if (Base::m_Capacity <= Base::m_Size + 1)
				Base::Reallocate(Base::m_Capacity * 2);

			ShiftRight(Base::m_Data.Data, Base::m_Size, index);

			Base::m_Data[index] = value;
			++Base::m_Size;
			return Result<bool>::Ok(true);
		}

		constexpr Result<bool> Insert(const size_t index, T&& value) noexcept
		{
			if (Base::m_Size <= index)
				return Result<bool>::Error(ArgumentOutOfRangeError{ NAMEOF(index), index });

			if (Base::m_Capacity <= Base::m_Size + 1)
				Base::Reallocate(Base::m_Capacity * 2);

			ShiftRight(Base::m_Data.Data, Base::m_Size, index);

			Base::m_Data[index] = std::move(value);
			++Base::m_Size;
			return Result<bool>::Ok(true);
		}

		template <typename... Args>
		constexpr Result<T&> InsertEmplace(const size_t index, Args&& ... args) noexcept
		{
			if (Base::m_Size <= index)
				return Result<T&>::Error(ArgumentOutOfRangeError{ NAMEOF(index), index });

			if (Base::m_Capacity <= Base::m_Size + 1)
				Base::Reallocate(Base::m_Capacity * 2);

			ShiftRight(Base::m_Data.Data, Base::m_Size, index);

			new(&Base::m_Data[index]) T(std::forward<Args>(args)...);
			++Base::m_Size;
			return Result<T&>::Ok(Base::m_Data[index]);
		}

		constexpr Result<bool> InsertRange(const size_t startIndex, const Base& collection) noexcept
		{
			// Check for valid sequence
			if (collection.IsEmpty()) 
				return Result<bool>::Ok(false);

			// Index validation
			if (Base::m_Size <= startIndex)
				return Result<bool>::Error(ArgumentOutOfRangeError{ NAMEOF(startIndex), startIndex });

			// Reallocate, if too small
			const size_t length = collection.Capacity();
			const size_t reallocationSize = Base::m_Size + length;
			if (Base::m_Capacity <= reallocationSize)
				Base::HandleReallocation(reallocationSize);

			// Shift right to make space for new data
			ShiftRight(Base::m_Data.Data, Base::m_Size, startIndex, length);

			// Assign data
			size_t index = startIndex;
			for (auto& elem : collection)
				Base::m_Data[index++] = elem;

			// Increment size
			Base::m_Size += length;
			return Result<bool>::Ok(true);
		}

		constexpr Result<bool> InsertRange(const size_t startIndex, Base&& collection) noexcept
		{
			// Check for valid sequence
			if (collection.IsEmpty())
				return Result<bool>::Ok(false);

			// Index validation
			if (Base::m_Size <= startIndex)
				return Result<bool>::Error(ArgumentOutOfRangeError{ NAMEOF(startIndex), startIndex });

			// Reallocate, if too small
			const size_t length = collection.Capacity();
			const size_t reallocationSize = Base::m_Size + length;
			if (Base::m_Capacity <= reallocationSize)
				Base::HandleReallocation(reallocationSize);

			// Shift right to make space for new data
			ShiftRight(Base::m_Data.Data, Base::m_Size, startIndex, length);

			// Assign data
			size_t index = startIndex;
			for (auto& elem : collection)
				Base::m_Data[index++] = std::move(elem);

			// Increment size
			Base::m_Size += length;

			// Clear sequence
			collection.Clear();
			return Result<bool>::Ok(true);
		}

		constexpr Result<bool> InsertRange(const size_t startIndex, const Span<T>& span) noexcept
		{
			// Check for valid sequence
			if (span.IsEmpty())
				return Result<bool>::Ok(false);

			// Index validation
			if (Base::m_Size <= startIndex)
				return Result<bool>::Error(ArgumentOutOfRangeError{ NAMEOF(startIndex), startIndex });

			// Reallocate, if too small
			const size_t length = span.Capacity();
			const size_t reallocationSize = Base::m_Size + length;
			if (Base::m_Capacity <= reallocationSize)
				Base::HandleReallocation(reallocationSize);

			// Shift right to make space for new data
			ShiftRight(Base::m_Data.Data, Base::m_Size, startIndex, length);

			// Assign data
			size_t index = startIndex;
			for (auto& elem : span)
				Base::m_Data[index++] = elem;

			// Increment size
			Base::m_Size += length;
			return Result<bool>::Ok(true);
		}

		constexpr Result<bool> InsertRange(const size_t startIndex, std::convertible_to<T> auto... elements) noexcept
		{
			constexpr size_t length = sizeof ...(elements);
			static_assert(length != 0, "Cannot call 'InsertRange' without any arguments!");

			// Index validation
			if (Base::m_Size <= startIndex)
				return Result<bool>::Error(ArgumentOutOfRangeError{ NAMEOF(startIndex), startIndex });

			// Reallocate, if too small
			const size_t reallocationSize = Base::m_Size + length;
			if (Base::m_Capacity <= reallocationSize)
				Base::HandleReallocation(reallocationSize);

			// Shift right to make space for new data
			ShiftRight(Base::m_Data.Data, Base::m_Size, startIndex, length);

			// Assign parameter args
			size_t index = startIndex;
			for (auto values = {static_cast<T>(elements)...}; auto&& elem : values)
				Base::m_Data[index++] = std::move(elem);

			// Increment size
			Base::m_Size += length;
			return Result<bool>::Ok(true);
		}

		constexpr bool Remove(const T& value) noexcept
		{
			// Return if empty
			if (Base::IsEmpty())
				return false;

			// Get and return if invalid index
			const auto result = Micro::IndexOf(this->AsSpan(), value);
			if (!result.IsValid())
				return false;

			// Dispose
			return RemoveAt(result.Value()).IsValid();
		}

		constexpr Result<bool> RemoveAt(const size_t index) noexcept
		{
			if (index >= Base::m_Size)
				return Result<bool>::Error(ArgumentOutOfRangeError{ NAMEOF(index), index });

			// Free memory
			Base::m_Data[index].~T();

			// Shift items down (+1 to shift invalid data to the end)
			ShiftLeft<T>(Base::m_Data, Base::m_Size, index + 1);

			// Decrement size
			--Base::m_Size;
			return Result<bool>::Ok(true);
		}

		constexpr Result<bool> RemoveRange(const size_t index, const size_t count) noexcept
		{
			const size_t length = index + count;
			if (index >= Base::m_Size || length >= Base::m_Size)
				return Result<bool>::Error(ArgumentOutOfRangeError{ "Invalid range from {} to {}.", index, length });

			// Free memory
			for (size_t i = index; i < length; i++)
				Base::m_Data[i].~T();

			// Shift items down (to shift invalid data to the end)
			ShiftLeft<T>(Base::m_Data, Base::m_Size, length, count);

			// Decrement size
			Base::m_Size -= count;
			return Result<bool>::Ok(true);
		}

		constexpr size_t RemoveAll(const Predicate<T>& predicate) noexcept
		{
			// Find indexes of elements matching predicate
			size_t length = Base::m_Size;

			for (size_t i = 0; i < length;)
			{
				auto& value = Base::m_Data[i];
				if (predicate(value))
				{
					// Free memory
					value.~T();

					// Shift items down (+1 to shift invalid data to the end)
					ShiftLeft<T>(Base::m_Data, length--, i + 1);
					continue;
				}

				i++;
			}

			const size_t count = Base::m_Size - length;
			Base::m_Size -= count;
			return count;
		}

		NODISCARD constexpr Optional<size_t> IndexOf(const T& value) const noexcept { return Micro::IndexOf(this->AsSpan(), value); }

		NODISCARD constexpr Optional<size_t> LastIndexOf(const T& value) const noexcept { return Micro::LastIndexOf(this->AsSpan(), value); }

		NODISCARD constexpr Optional<size_t> FindIndex(const Predicate<T>& predicate) const noexcept
		{
			for (size_t i = 0; i < Base::m_Size; i++)
				if (predicate(Base::m_Data[i]))
					return Optional<size_t>(i);
			return Optional<size_t>::Empty();
		}

		NODISCARD constexpr Optional<size_t> FindLastIndex(const Predicate<T>& predicate) const noexcept
		{
			for (size_t i = Base::m_Size; i > 0; --i)
			{
				const T& elem = Base::m_Data[i - 1ULL];
				if (predicate(elem))
					return Optional<size_t>(i - 1ULL);
			}
			return Optional<size_t>::Empty();
		}

		NODISCARD constexpr Optional<T&> Find(const Predicate<T>& predicate) noexcept
		{
			for (size_t i = 0; i < Base::m_Size; i++)
			{
				const T& elem = Base::m_Data[i];
				if (predicate(elem))
					return Optional<T&>(elem);
			}

			return Optional<T&>::Empty();
		}

		NODISCARD constexpr Optional<T&> FindLast(const Predicate<T>& predicate) noexcept
		{
			for (size_t i = Base::m_Size; i > 0; --i)
			{
				const T& elem = Base::m_Data[i - 1ULL];
				if (predicate(elem))
					return Optional<T&>(elem);
			}

			return Optional<T&>::Empty();
		}

		NODISCARD constexpr List<Optional<T&>> FindAll(const Predicate<T>& predicate) noexcept
		{
			List<Optional<T&>> list(Base::m_Size);

			for (size_t i = 0; i < Base::m_Size; i++)
			{
				T& elem = Base::m_Data[i];
				if (predicate(elem))
					list.Emplace(elem);
			}

			return list;
		}

		NODISCARD constexpr Optional<const T&> Find(const Predicate<T>& predicate) const noexcept
		{
			for (size_t i = 0; i < Base::m_Size; i++)
			{
				const T& elem = Base::m_Data[i];
				if (predicate(elem))
					return Optional<const T&>(elem);
			}

			return Optional<const T&>::Empty();
		}

		NODISCARD constexpr Optional<const T&> FindLast(const Predicate<T>& predicate) const noexcept
		{
			for (size_t i = Base::m_Size; i > 0; --i)
			{
				const T& elem = Base::m_Data[i - 1ULL];
				if (predicate(elem))
					return Optional<const T&>(elem);
			}

			return Optional<const T&>::Empty();
		}

		NODISCARD constexpr List<Optional<const T&>> FindAll(const Predicate<T>& predicate) const noexcept
		{
			List<Optional<const T&>> list(Base::m_Size);

			for (size_t i = 0; i < Base::m_Size; i++)
			{
				const T& elem = Base::m_Data[i];
				if (predicate(elem))
					list.Emplace(elem);
			}

			return list;
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
		NODISCARD constexpr Result<T&> operator[](const size_t index) noexcept 
		{ 
			if (index >= Base::m_Size)
				return Result<T&>::Error(IndexOutOfRangeError(index));

			return Result<T&>::Ok(Base::m_Data[index]);
		}

		/// <summary>
		/// Gets the element at the given index, or an empty result if invalid. (const version)
		/// </summary>
		/// <param name="index">Index of element</param>
		/// <returns>Reference to the element at index, or empty result if invalid</returns>
		NODISCARD constexpr Result<const T&> operator[](const size_t index) const noexcept
		{
			if (index >= Base::m_Size)
				return Result<const T&>::Error(IndexOutOfRangeError(index));

			return Result<const T&>::Ok(Base::m_Data[index]);
		}

		/// <summary>
		/// Copies the elements from the given List.
		/// </summary>
		/// <param name="list">List to copy</param>
		/// <returns>Reference of this instance</returns>
		constexpr List& operator=(const List& list) noexcept
		{
			// Validation
			if (this == &list)
				return *this;

			const size_t capacity = list.m_Capacity;
			if (capacity == 0)
				return *this;

			// Allocation
			if (!Base::IsEmpty())
				Base::Reallocate(capacity);
			else
				Base::Allocate(capacity);

			Base::m_Size = list.m_Size;

			// Assignment
			Base::CopyFrom(list);
			return *this;
		}

		/// <summary>
		/// Moves the elements from the given List.
		/// </summary>
		/// <param name="list">List to move</param>
		/// <returns>Reference of this instance</returns>
		constexpr List& operator=(List&& list) noexcept
		{
			// Validation
			if (this == &list)
				return *this;

			if (!Base::IsEmpty())
				Base::Clear();

			Base::m_Data = list.m_Data;
			Base::m_Size = list.m_Size;
			Base::m_Capacity = list.m_Capacity;

			list.m_Data = nullptr;
			list.m_Size = 0;
			list.m_Capacity = 0;

			return *this;
		}

	private:
		static constexpr size_t DefaultCapacity = 16;
	};
}
