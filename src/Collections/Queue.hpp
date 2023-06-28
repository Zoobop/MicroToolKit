#pragma once

#include "Collections/Container.hpp"
#include "Utility/Result.hpp"

namespace mtk
{
	template <typename T>
	class Queue final : public ContiguousContainer<T, Allocator<T>>
	{
	public:
		// Aliases
		using Base = ContiguousContainer<T, Allocator<T>>;
		using Sequence = Sequence<T>;

		friend Base;

		// Constructors/Destructors
		constexpr Queue() noexcept : Base()
		{
		}

		constexpr Queue(const Queue& other) : Base(other)
		{
		}

		constexpr Queue(Queue&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Queue(const Base& other) : Base(other)
		{
		}

		constexpr Queue(Base&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Queue(const Sequence& other) : Base(other)
		{
		}

		constexpr Queue(Sequence&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Queue(std::initializer_list<T>&& initializerList) noexcept : Base(std::move(initializerList))
		{
		}

		explicit constexpr Queue(std::convertible_to<T> auto... elements) noexcept : Base(
			std::forward<T>(static_cast<T>(elements))...)
		{
		}

		explicit Queue(const size_t capacity) : Base()
		{
			Reserve(capacity);
		}

		constexpr ~Queue() noexcept override = default;

		// Utility
		void Enqueue(const T& value)
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(c_DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			ShiftRight<T>(Base::m_Data, Base::m_Size, 0);
			Base::m_Data[0] = value;
			++Base::m_Size;
		}

		void Enqueue(T&& value)
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(c_DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			ShiftRight<T>(Base::m_Data, Base::m_Size, 0);
			Base::m_Data[0] = std::move(value);
			++Base::m_Size;
		}

		NODISCARD Result<T> Dequeue()
		{
			if (!Base::IsEmpty())
			{
				T&& item = std::move(Base::m_Data[0]);
				Base::m_Data[0].~T();

				ShiftLeft<T>(Base::m_Data, Base::m_Size, 1);
				--Base::m_Size;
				return Result(item);
			}
			return Result<T>::Empty();
		}

		NODISCARD Result<T> Peek() const
		{
			if (!Base::IsEmpty())
				return Result(Base::m_Data[0]);
			return Result<T>::Empty();
		}

		NODISCARD Result<T> Front() const { return !Base::IsEmpty() ? Result(Base::m_Data[0]) : Result<T>::Empty(); }
		NODISCARD Result<T> Back() const
		{
			return !Base::IsEmpty() ? Result(Base::m_Data[Base::m_Size - 1]) : Result<T>::Empty();
		}

		// Accessors
		NODISCARD constexpr void Reserve(const size_t capacity) { Base::Reallocate(capacity); }

		// Operator Overloads
		Queue& operator=(const Queue& other)
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

		Queue& operator=(Queue&& other) noexcept
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
		static constexpr size_t c_DefaultCapacity = 4;
	};
}
