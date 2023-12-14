#pragma once
#include <format>
#include <sstream>

#include "Collections/Base/Collection.hpp"
#include "Utility/Options/Result.hpp"

namespace Micro
{
	template <typename T>
	class Stack final : public HeapCollection<T>
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


		constexpr Stack() noexcept : Base()
		{
		}

		constexpr Stack(const Stack& stack) noexcept : Base(stack)
		{
		}

		constexpr Stack(Stack&& stack) noexcept : Base(std::move(stack))
		{
		}

		constexpr Stack(const Base& collection) : Base(collection)
		{
		}

		constexpr Stack(Base&& collection) noexcept : Base(std::move(collection))
		{
		}

		constexpr Stack(std::initializer_list<T>&& initializerList) noexcept : Base(std::move(initializerList))
		{
		}

		constexpr explicit Stack(const Span<T>& other) : Base(other)
		{
		}

		constexpr explicit Stack(std::convertible_to<T> auto... elements) noexcept : Base(
			std::forward<T>(static_cast<T>(elements))...)
		{
		}

		constexpr explicit Stack(const size_t capacity) : Base(capacity)
		{
		}

		constexpr ~Stack() noexcept override = default;


		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		constexpr void Push(const T& value) noexcept
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			Base::m_Data[Base::m_Size++] = value;
		}

		constexpr void Push(T&& value) noexcept
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			Base::m_Data[Base::m_Size++] = std::move(value);
		}

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

		constexpr void PushRange(const Base& collection) noexcept
		{
			if (collection.IsEmpty())
				return;

			const size_t size = collection.Size();
			const T* data = collection.Data();

			const size_t length = Base::m_Size + size;
			Base::HandleReallocation(length);

			for (size_t i = 0; i < size; i++)
				Push(data[i]);
		}

		constexpr void PushRange(Base&& collection) noexcept
		{
			if (collection.IsEmpty())
				return;

			const size_t size = collection.Size();
			T* data = const_cast<T*>(collection.Data());

			const size_t length = Base::m_Size + size;
			Base::HandleReallocation(length);

			for (size_t i = 0; i < size; i++)
				Push(std::move(data[i]));
		}

		/// <summary>
		/// Pushes all the elements from the given span to the stack by copy.
		/// </summary>
		/// <param name="span">Span to add to the stack</param>
		constexpr void PushRange(const Span<T>& span) noexcept
		{
			if (span.IsEmpty())
				return;

			const size_t size = span.Capacity();
			const T* data = span.Data();

			const size_t length = Base::m_Size + size;
			Base::HandleReallocation(length);

			for (size_t i = 0; i < size; i++)
				Push(data[i]);
		}

		constexpr void PushRange(std::convertible_to<T> auto... elements) noexcept
		{
			constexpr size_t argumentCount = sizeof ...(elements);
			static_assert(argumentCount != 0, "Cannot call 'PushRange' without any arguments!");

			const size_t length = Base::m_Size + argumentCount;
			Base::HandleReallocation(length);

			for (auto values = { static_cast<T>(std::move(elements))... }; auto&& item : values)
				Push(std::move(item));
		}

		NODISCARD constexpr Result<T> Pop() noexcept
		{
			if (!Base::IsEmpty())
			{
				auto item = std::move(Base::m_Data[Base::m_Size - 1]);
				Base::m_Data[Base::m_Size - 1].~T();
				--Base::m_Size;
				return Result<T>::Ok(std::move(item));
			}

			return Result<T>::CaptureError(InvalidOperationError("Cannot pop from empty Stack!"));
		}

		NODISCARD constexpr Result<T&> Peek() const noexcept
		{
			if (!Base::IsEmpty())
				return Result<T&>::Ok(Base::m_Data[Base::m_Size - 1]);

			return Result<T&>::CaptureError(InvalidOperationError("Cannot peek from empty Stack!"));
		}

		
		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		constexpr Stack& operator=(const Stack& stack) noexcept
		{
			// Validation
			if (this == &stack)
				return *this;

			const size_t capacity = stack.m_Capacity;
			if (capacity == 0)
				return *this;

			// Allocation
			if (!Base::IsEmpty())
				Base::Reallocate(capacity);
			else
				Base::Allocate(capacity);

			Base::m_Size = stack.m_Size;

			// Assignment
			Base::CopyFrom(stack);
			return *this;
		}

		constexpr Stack& operator=(Stack&& stack) noexcept
		{
			// Validation
			if (this == &stack)
				return *this;

			if (!Base::IsEmpty())
				Base::Clear();

			Base::m_Data = stack.m_Data;
			Base::m_Size = stack.m_Size;
			Base::m_Capacity = stack.m_Capacity;

			stack.m_Data = nullptr;
			stack.m_Size = 0;
			stack.m_Capacity = 0;

			return *this;
		}

	private:
		static constexpr size_t DefaultCapacity = 16;
	};
}

template <typename T>
struct std::formatter<Micro::Stack<T>> {
	constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const Micro::Stack<T>& obj, std::format_context& ctx) const {
		std::stringstream stream{};
		stream << obj;
        return std::format_to(ctx.out(), "{}", stream.str().c_str());
    }
};