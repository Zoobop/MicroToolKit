#pragma once

#include "Collections/Container.hpp"
#include "Utility/Result.hpp"

namespace mtk
{
	template <typename T>
	class Stack final : public ContiguousContainer<T, Allocator<T>>
	{
	public:
		// Aliases
		using Base = ContiguousContainer<T, Allocator<T>>;
		using Sequence = Sequence<T>;

		friend Base;

		// Constructors/Destructors
		constexpr Stack() noexcept : Base()
		{
		}

		constexpr Stack(const Stack& other) : Base(other)
		{
		}

		constexpr Stack(Stack&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Stack(const Base& other) : Base(other)
		{
		}

		constexpr Stack(Base&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Stack(const Sequence& other) : Base(other)
		{
		}

		constexpr Stack(Sequence&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Stack(std::initializer_list<T>&& initializerList) noexcept : Base(std::move(initializerList))
		{
		}

		explicit constexpr Stack(std::convertible_to<T> auto... elements) noexcept : Base(
			std::forward<T>(static_cast<T>(elements))...)
		{
		}

		explicit Stack(const size_t capacity) : Base()
		{
			Reserve(capacity);
		}

		constexpr ~Stack() noexcept override = default;

		// Utility
		void Push(const T& value)
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(c_DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			Base::m_Data[Base::m_Size++] = value;
		}

		void Push(T&& value)
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(c_DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			Base::m_Data[Base::m_Size++] = std::move(value);
		}

		template <typename... Args>
		T& Emplace(Args&&... args)
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(c_DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			new(&Base::m_Data[Base::m_Size]) T(std::forward<Args>(args)...);
			return Base::m_Data[Base::m_Size++];
		}

		void PushRange(const Container<T>& container)
		{
			for (const auto& item : container)
				Push(item);
		}

		void PushRange(T firstElem, std::convertible_to<T> auto... elements)
		{
			for (auto values = {firstElem, static_cast<T>(std::move(elements))...}; auto&& item : values)
				Push(std::move(item));
		}

		NODISCARD Result<T> Pop()
		{
			if (!Base::IsEmpty())
			{
				auto& item = std::move(Base::m_Data[Base::m_Size - 1]);
				Base::m_Data[Base::m_Size - 1].~T();
				--Base::m_Size;
				return Result(std::move(item));
			}

			return Result<T>::Empty();
		}

		NODISCARD bool TryPop(T& out)
		{
			if (!Base::IsEmpty())
			{
				out = std::move(Base::m_Data[Base::m_Size - 1]);
				Base::m_Data[Base::m_Size - 1].~T();
				--Base::m_Size;
				return true;
			}
			return false;
		}

		NODISCARD Result<T> Peek() const
		{
			if (!Base::IsEmpty())
				return Result(Base::m_Data[Base::m_Size - 1]);
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
		Stack& operator=(const Stack& other)
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

		Stack& operator=(Stack&& other) noexcept
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
