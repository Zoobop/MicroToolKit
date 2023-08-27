#pragma once

#include "Collections/Base/Collection.hpp"
#include "Utility/Options/Optional.hpp"

namespace Micro
{
	template <typename T>
	class Queue final : public HeapCollection<T>
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


		constexpr Queue() noexcept : Base()
		{
		}

		constexpr Queue(const Queue& other) noexcept : Base(other)
		{
		}

		constexpr Queue(Queue&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Queue(const Base& other) noexcept : Base(other)
		{
		}

		constexpr Queue(Base&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Queue(std::initializer_list<T>&& initializerList) noexcept : Base(std::move(initializerList))
		{
		}

		constexpr explicit Queue(const Span<T>& other) noexcept : Base(other)
		{
		}

		constexpr explicit Queue(std::convertible_to<T> auto... elements) noexcept : Base(
			std::forward<T>(static_cast<T>(elements))...)
		{
		}

		constexpr explicit Queue(const size_t capacity) noexcept : Base(capacity)
		{
		}

		constexpr ~Queue() noexcept override = default;

		
		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		constexpr void Enqueue(const T& value) noexcept
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			ShiftRight<T>(Base::m_Data, Base::m_Size, 0);
			Base::m_Data[0] = value;
			++Base::m_Size;
		}

		constexpr void Enqueue(T&& value) noexcept
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			ShiftRight<T>(Base::m_Data, Base::m_Size, 0);
			Base::m_Data[0] = std::move(value);
			++Base::m_Size;
		}

		template <typename... Args>
		constexpr T& Emplace(Args&&... args) noexcept
		{
			if (!Base::m_Data.IsValidMemory())
				Base::Allocate(DefaultCapacity);
			else if (Base::m_Capacity <= Base::m_Size)
				Base::Reallocate(Base::m_Capacity * 2);

			ShiftRight<T>(Base::m_Data, Base::m_Size, 0);
			new(&Base::m_Data[0]) T(std::forward<Args>(args)...);
			return Base::m_Data[0];
		}

		constexpr void EnqueueRange(const Base& collection) noexcept
		{
			if (collection.IsEmpty())
				return;

			const size_t size = collection.Size();
			const T* data = collection.Data();

			const size_t length = Base::m_Size + size;
			Base::HandleReallocation(length);

			for (size_t i = 0; i < size; i++)
				Enqueue(data[i]);
		}

		constexpr void EnqueueRange(Base&& collection) noexcept
		{
			if (collection.IsEmpty())
				return;

			const size_t size = collection.Size();
			T* data = const_cast<T*>(collection.Data());

			const size_t length = Base::m_Size + size;
			Base::HandleReallocation(length);

			for (size_t i = 0; i < size; i++)
				Enqueue(std::move(data[i]));
		}

		/// <summary>
		/// Enqueues all the elements from the given span to the Queue by copy.
		/// </summary>
		/// <param name="span">Span to add to the Queue</param>
		constexpr void EnqueueRange(const Span<T>& span) noexcept
		{
			if (span.IsEmpty())
				return;

			const size_t size = span.Capacity();
			const T* data = span.Data();

			const size_t length = Base::m_Size + size;
			Base::HandleReallocation(length);

			for (size_t i = 0; i < size; i++)
				Enqueue(data[i]);
		}

		constexpr void EnqueueRange(std::convertible_to<T> auto... elements) noexcept
		{
			constexpr size_t argumentCount = sizeof ...(elements);
			static_assert(argumentCount != 0, "Cannot call 'EnqueueRange' without any arguments!");

			const size_t length = Base::m_Size + argumentCount;
			Base::HandleReallocation(length);

			for (auto values = { static_cast<T>(std::move(elements))... }; auto && item : values)
				Enqueue(std::move(item));
		}

		NODISCARD constexpr Result<T> Dequeue() noexcept
		{
			if (!Base::IsEmpty())
			{
				auto item = std::move(Base::m_Data[0]);
				Base::m_Data[0].~T();

				ShiftLeft<T>(Base::m_Data, Base::m_Size, 1);
				--Base::m_Size;
				return Result<T>::Ok(std::move(item));
			}
			return Result<T>::Error(InvalidOperationError("Cannot dequeue from empty Queue!"));
		}

		NODISCARD constexpr Result<T&> Peek() const noexcept
		{
			if (!Base::IsEmpty())
				return Result<T&>::Ok(Base::m_Data[0]);

			return Result<T&>::Error(InvalidOperationError("Cannot peek from empty Queue!"));
		}


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		/// <summary>
		/// Copies the elements from the given Queue.
		/// </summary>
		/// <param name="queue">Queue to copy</param>
		/// <returns>Reference of this instance</returns>
		constexpr Queue& operator=(const Queue& queue) noexcept
		{
			// Validation
			if (this == &queue)
				return *this;

			const size_t capacity = queue.m_Capacity;
			if (capacity == 0)
				return *this;

			// Allocation
			if (!Base::IsEmpty())
				Base::Reallocate(capacity);
			else
				Base::Allocate(capacity);

			Base::m_Size = queue.m_Size;

			// Assignment
			Base::CopyFrom(queue);
			return *this;
		}

		/// <summary>
		/// Moves the elements from the given Queue.
		/// </summary>
		/// <param name="queue">Queue to move</param>
		/// <returns>Reference of this instance</returns>
		constexpr Queue& operator=(Queue&& queue) noexcept
		{
			// Validation
			if (this == &queue)
				return *this;

			if (!Base::IsEmpty())
				Base::Clear();

			Base::m_Data = queue.m_Data;
			Base::m_Size = queue.m_Size;
			Base::m_Capacity = queue.m_Capacity;

			queue.m_Data = nullptr;
			queue.m_Size = 0;
			queue.m_Capacity = 0;

			return *this;
		}

	private:
		static constexpr size_t DefaultCapacity = 16;
	};
}
