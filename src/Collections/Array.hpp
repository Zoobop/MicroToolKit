#pragma once

#include "Collections/Base/Container.hpp"

namespace Micro
{
	template <typename T, size_t TSize>
	class Array final : public FixedContainer<T, TSize>
	{
	public:
		// Aliases
		using Base = FixedContainer<T, TSize>;

		// Constructors/Destructors
		constexpr Array() noexcept = default;

		Array(const Array& other) noexcept : Base(other)
		{
		}

		Array(Array&& other) noexcept : Base(std::move(other))
		{
		}

		Array(std::initializer_list<T>&& data) noexcept : Base(std::move(data))
		{
		}

		explicit Array(std::convertible_to<T> auto... elements) noexcept : Base(std::forward<T>(elements)...)
		{
		}

		constexpr ~Array() noexcept override = default;

		// Utility
		void Initialize()
		{
			for (size_t i = 0; i < TSize; i++)
				new(&Base::m_Data[i]) T();
		}

		void Fill(const T& value)
		{
			for (size_t i = 0; i < TSize; i++)
				Base::m_Data[i] = value;
		}

		void Set(std::initializer_list<T>&& initList) noexcept
		{
			size_t index = 0;
			for (auto& elem : initList)
			{
				if (index == TSize)
					return;

				Base::m_Data[index++] = std::move(const_cast<T&&>(elem));
			}
		}

		void Set(std::convertible_to<T> auto... elements) noexcept
		{
			size_t index = 0;

			auto values = {elements...};
			for (auto& elem : values)
			{
				if (index == TSize)
					return;

				Base::m_Data[index++] = std::move(elem);
			}
		}

		void Set(const Sequence<T>& container)
		{
			const size_t size = MIN(container.Capacity(), TSize);
			const T* data = container.Data();
			for (size_t i = 0; i < size; i++)
				Base::m_Data[i] = data[i];
		}

		void Swap(Array& other) noexcept
		{
			for (size_t i = 0; i < TSize; i++)
			{
				auto temp = std::move(Base::m_Data[i]);
				Base::m_Data[i] = std::move(other.m_Data[i]);
				other.m_Data[i] = std::move(temp);
			}
		}

		NODISCARD bool Contains(const T& value) const
		{
			for (size_t i = 0; i < TSize; i++)
				if (Base::m_Data[i] == value)
					return true;
			return false;
		}

		NODISCARD T& operator[](const size_t index)
		{
			if (index >= TSize)
				throw IndexOutOfRangeException(index);

			return Base::m_Data[index];
		}

		NODISCARD const T& operator[](const size_t index) const
		{
			if (index >= TSize)
				throw IndexOutOfRangeException(index);

			return Base::m_Data[index];
		}

		Array& operator=(const Array& array)
		{
			if (this == &array)
				return *this;

			Base::CopyFrom(array);

			return *this;
		}

		Array& operator=(Array&& array) noexcept
		{
			if (this == &array)
				return *this;

			Base::MoveFrom(std::move(array));

			return *this;
		}

		Array& operator=(std::initializer_list<T>&& initList) noexcept
		{
			Set(std::move(initList));
			return *this;
		}
	};
}
