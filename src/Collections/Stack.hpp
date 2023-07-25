#pragma once

#include "Collections/Base/Collection.hpp"
#include "Utility/Result.hpp"

namespace Micro
{
	template <typename T>
	class Stack final : public ContiguousCollection<T, Allocator<T>>
	{
	public:
		// Aliases
		using Base = ContiguousCollection<T, Allocator<T>>;
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

		void PushRange(const Collection<T>& container)
		{
			for (const auto& item : container)
				Push(item);
		}

		void PushRange(T firstElem, std::convertible_to<T> auto... elements)
		{
			for (auto values = {firstElem, static_cast<T>(std::move(elements))...}; auto&& item : values)
				Push(std::move(item));
		}

		NODISCARD RefResult<T> Pop()
		{
			if (!Base::IsEmpty())
			{
				auto& item = std::move(Base::m_Data[Base::m_Size - 1]);
				Base::m_Data[Base::m_Size - 1].~T();
				--Base::m_Size;
				return RefResult(std::move(item));
			}

			return RefResult<T>::Empty();
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

		NODISCARD RefResult<T> Peek() const
		{
			if (!Base::IsEmpty())
				return RefResult(Base::m_Data[Base::m_Size - 1]);
			return RefResult<T>::Empty();
		}

		NODISCARD RefResult<T> Front() const { return !Base::IsEmpty() ? RefResult(Base::m_Data[0]) : RefResult<T>::Empty(); }
		NODISCARD RefResult<T> Back() const
		{
			return !Base::IsEmpty() ? RefResult(Base::m_Data[Base::m_Size - 1]) : RefResult<T>::Empty();
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
