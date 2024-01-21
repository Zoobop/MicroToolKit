#pragma once

#include "Collections/Base/Collection.hpp"
#include "Utility/Options/Optional.hpp"
#include "Utility/Options/Result.hpp"

namespace Micro
{
	/**
	 * \brief Represents a fixed-sized stack-allocated structure for manipulating and searching through a contiguous block of memory.
	 * \tparam T Type of elements in array
	 * \tparam TSize Stack-allocated size of the array
	 */
	template <typename T, usize TSize>
	class Array final : public StackCollection<T, TSize>
	{
	public:
		/*
		 *  ============================================================
		 *	|                          Aliases                         |
		 *  ============================================================
		 */


		using Base = StackCollection<T, TSize>;
		

		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		/**
		 * \brief Initializes a new instance of the Array class with an empty buffer.
		 */
		constexpr Array() noexcept = default;

		/**
		 * \brief Initializes a new instance of the Array class by copying the value of the Array argument.
		 * \param array Array to copy
		 */
		constexpr Array(const Array& array) noexcept : Base(array)
		{
		}

		/**
		 * \brief Initializes a new instance of the Array class by moving the value of the Array argument.
		 * \param array Array to move
		 */
		constexpr Array(Array&& array) noexcept : Base(std::move(array))
		{
		}

		/**
		 * \brief Initializes a new instance of the Array class by copying the value of the Stack-allocated collection argument.
		 * \param stackCollection Stack-allocated collection to copy
		 */
		constexpr Array(const Base& stackCollection) noexcept : Base(stackCollection)
		{
		}

		/**
		 * \brief Initializes a new instance of the Array class by copying the value of the Stack-allocated collection argument.
		 * \param stackCollection Stack-allocated collection to copy
		 */
		constexpr Array(Base&& stackCollection) noexcept : Base(std::move(stackCollection))
		{
		}

		/**
		 * \brief Initializes a new instance of the Array class by moving the elements from the initializer list.
		 * \param list Initializer list to move
		 */
		constexpr Array(std::initializer_list<T>&& list) noexcept : Base(std::move(list))
		{
		}

		/**
		 * \brief Initializes a new instance of the Array class by copying the elements of the Span argument.
		 * \param span Span to copy
		 */
		constexpr explicit Array(const Span<T>& span) noexcept : Base(span)
		{
		}

		/**
		 * \brief Initializes a new instance of the Array class by copying the elements of the default array argument.
		 * \param array Array to copy
		 */
		constexpr explicit Array(const T(&array)[TSize]) noexcept : Base(array)
		{
		}

		/**
		 * \brief Initializes a new instance of the Array class by copying the elements from the parameter pack.
		 * \param elements Elements to copy
		 */
		constexpr explicit Array(std::convertible_to<T> auto... elements) noexcept : Base(std::forward<T>(elements)...)
		{
		}

		/**
		 * \brief Default destructor that provides no implementation to free memory since it is bound to the stack.
		 */
		constexpr ~Array() noexcept override = default;


		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		/// <summary>
		/// Gets the value at the specified index, or returns an invalid result.
		/// </summary>
		/// <param name="index">Index to pull value</param>
		/// <returns>Value at index, or invalid result</returns>
		NODISCARD constexpr Result<T&> At(const usize index) noexcept
		{
			if (index >= TSize)
				return Result<T&>::CaptureError(IndexOutOfRangeError(index));

			return Result<T&>::Ok(Base::m_Data[index]);
		}
		
		/// <summary>
		/// Gets the value at the specified index, or returns an invalid result.
		/// </summary>
		/// <param name="index">Index to pull value</param>
		/// <returns>Value at index, or invalid result</returns>
		NODISCARD constexpr Result<const T&> At(const usize index) const noexcept
		{
			if (index >= TSize)
				return Result<const T&>::CaptureError(IndexOutOfRangeError(index));

			return Result<const T&>::Ok(Base::m_Data[index]);
		}

		/// <summary>
		/// Sets the element at the specific index with the given value.
		/// </summary>
		/// <param name="index"></param>
		/// <param name="value"></param>
		/// <returns>True, if set</returns>
		constexpr bool SetValue(const usize index, const T& value) noexcept
		{
			if (index >= TSize)
				return false;

			Base::m_Data[index] = value;
			return true;
		}
		
		/// <summary>
		/// Sets the element at the specific index with the given value.
		/// </summary>
		/// <param name="index"></param>
		/// <param name="value"></param>
		/// <returns>True, if set</returns>
		constexpr bool SetValue(const usize index, T&& value) noexcept
		{
			if (index >= TSize)
				return false;

			Base::m_Data[index] = std::move(value);
			return true;
		}

		/// <summary>
		/// Fills the entire Array with the given value.
		/// </summary>
		///	<param name="value">Value to fill Array</param>
		constexpr void Fill(const T& value) noexcept
		{
			for (usize i = 0; i < TSize; i++)
				Base::m_Data[i] = value;
		}

		/// <summary>
		/// Sets the elements in the Array to be the elements from the initializer list. If the initializer list
		/// has more elements than the Array length, only elements up to the Array length with be moved.
		/// </summary>
		///	<param name="initializerList">Initializer list to fill Array</param>
		constexpr void Set(std::initializer_list<T>&& initializerList) noexcept
		{
			const usize capacity = initializerList.size();
			if (capacity == 0)
				return;

			const usize length = MIN(capacity, TSize);
			const auto data = initializerList.begin();
			for (usize i = 0; i < length; i++)
				Base::m_Data[i] = std::move(const_cast<T&>(data[i]));
		}

		/// <summary>
		/// Sets the elements in the Array to be the elements from the span. If the span
		/// has more elements than the Array length, only elements up to the Array length with be moved.
		/// </summary>
		///	<param name="span">Span to fill Array</param>
		constexpr void Set(const Span<T>& span)
		{
			const usize size = MIN(span.Capacity(), TSize);
			const T* data = span.Data();
			for (usize i = 0; i < size; i++)
				Base::m_Data[i] = data[i];
		}

		/// <summary>
		/// Sets the elements in the Array to be the elements from the default array. If the default array
		/// has more elements than the Array length, only elements up to the Array length with be copied.
		/// </summary>
		///	<param name="array">Default array to fill Array</param>
		constexpr void Set(const T(&array)[TSize]) noexcept
		{
			constexpr usize size = TSize;
			for (usize i = 0; i < size; i++)
				Base::m_Data[i] = array[i];
		}

		/// <summary>
		/// Sets the elements in the Array to be the elements from the parameter pack. If the parameter pack
		/// has more elements than the Array length, only elements up to the Array length with be copied.
		/// </summary>
		///	<param name="elements">Parameter pack to fill Array</param>
		constexpr void Set(std::convertible_to<T> auto... elements) noexcept
		{
			usize index = 0;

			auto values = {elements...};
			for (auto& elem : values)
			{
				if (index == TSize)
					return;

				Base::m_Data[index++] = std::move(elem);
			}
		}

		/// <summary>
		/// Swaps the elements in the Array with the elements from the given Array.
		/// </summary>
		///	<param name="array">Array to swap with</param>
		constexpr void Swap(Array& array) noexcept
		{
			for (usize i = 0; i < TSize; i++)
			{
				auto temp = std::move(Base::m_Data[i]);
				Base::m_Data[i] = std::move(array.m_Data[i]);
				array.m_Data[i] = std::move(temp);
			}
		}

		/// <summary>
		/// Reverses the element order in the Array.
		/// </summary>
		constexpr void Reverse() noexcept
		{
			auto span = Base::AsSpan();
			constexpr usize length = TSize / 2;
			for (usize left = 0, right = TSize - 1; left < length; left++, --right)
				Micro::Swap(span, left, right);
		}

		/// <summary>
		/// Tries to get the index of the first occurrence of the given value.
		/// </summary>
		/// <param name="value">Value to find</param>
		/// <returns>The first index of the value, or an invalid result</returns>
		NODISCARD constexpr Optional<usize> IndexOf(const T& value) const noexcept
		{
			for (usize i = 0; i < TSize; ++i)
				if (Base::m_Data[i] == value)
					return Optional<usize>(i);

			return Optional<usize>::Empty();
		}

		/// <summary>
		/// Tries to get the index of the last occurrence of the given value.
		/// </summary>
		/// <param name="value">Value to find</param>
		/// <returns>The last index of the value, or an invalid result</returns>
		NODISCARD constexpr Optional<usize> LastIndexOf(const T& value) const noexcept
		{
			for (usize i = 0; i < TSize; ++i)
			{
				const usize offset = TSize - 1 - i;
				if (Base::m_Data[offset] == value)
					return Optional<usize>(offset);
			}

			return Optional<usize>::Empty();
		}

		/// <summary>
		/// Searches for the given element with the Array by the predicate.
		/// </summary>
		/// <param name="predicate">Condition to search by</param>
		/// <returns>True, if found</returns>
		NODISCARD constexpr bool Exists(const Predicate<T>& predicate) const noexcept
		{
			for (usize i = 0; i < TSize; i++)
				if (predicate(Base::m_Data[i]))
					return true;
			return false;
		}

		/// <summary>
		/// Tests if all elements pass the given predicate.
		/// </summary>
		/// <param name="predicate">Condition to test elements with</param>
		/// <returns>True, if all elements pass condition</returns>
		NODISCARD constexpr bool TrueForAll(const Predicate<T>& predicate) const noexcept
		{
			for (usize i = 0; i < TSize; i++)
				if (!predicate(Base::m_Data[i]))
					return false;
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
		NODISCARD constexpr Result<T&> operator[](const usize index) noexcept { return At(index); }

		/// <summary>
		/// Gets the element at the given index, or an empty result if invalid. (const version)
		/// </summary>
		/// <param name="index">Index of element</param>
		/// <returns>Reference to the element at index, or empty result if invalid</returns>
		NODISCARD constexpr Result<const T&> operator[](const usize index) const noexcept { return At(index); }

		/// <summary>
		/// Copies the elements from the given Array.
		/// </summary>
		/// <param name="array">Array to copy</param>
		/// <returns>Reference of this instance</returns>
		constexpr Array& operator=(const Array& array) noexcept
		{
			if (this == &array)
				return *this;

			Base::CopyFrom(array);
			return *this;
		}

		/// <summary>
		/// Moves the elements from the given Array.
		/// </summary>
		/// <param name="array">Array to move</param>
		/// <returns>Reference of this instance</returns>
		constexpr Array& operator=(Array&& array) noexcept
		{
			if (this == &array)
				return *this;

			Base::MoveFrom(std::move(array));
			return *this;
		}

		/// <summary>
		/// Moves the elements from the given initializer list.
		/// </summary>
		/// <param name="initList">Initializer list to move</param>
		/// <returns>Reference of this instance</returns>
		constexpr Array& operator=(std::initializer_list<T>&& initList) noexcept
		{
			Set(std::move(initList));
			return *this;
		}
	};


	/*
	 *  ============================================================
	 *	|                    Global Functions                      |
	 *  ============================================================
	 */


	/// <summary>
	/// Hashes the Array to produce a unique hash code.
	/// </summary>
	/// <returns>Hash code as a 'usize'</returns>
	template <typename T, usize TSize>
	NODISCARD usize Hash(const Array<T, TSize>&) noexcept
	{
		return typeid(T).hash_code() + typeid(Array<T, TSize>).hash_code() + TSize;
	}

	/// <summary>
	/// Creates an Array using the given elements.
	/// </summary>
	/// <param name="elements">Elements to populate Array</param>
	/// <returns>New instance of an Array</returns>
	template <typename T>
	NODISCARD constexpr auto MakeArray(std::convertible_to<T> auto... elements) noexcept
	{
		constexpr usize length = sizeof ...(elements);
		return Array<T, length>(std::forward<T>(elements)...);
	}
}
