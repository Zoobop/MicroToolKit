#pragma once
#include "Memory.hpp"

namespace Micro
{
	template <typename T>
	class Allocator
	{
	public:
		using Memory = Memory<T>;

		constexpr Allocator() noexcept = default;
		constexpr Allocator(const Allocator&) noexcept = default;
		constexpr Allocator(Allocator&&) noexcept = default;
		constexpr virtual ~Allocator() noexcept = default;

		NODISCARD virtual size_t Allocate(Memory& data, const size_t currentCapacity, const size_t newCapacity)
		{
			// Don't allocate if same capacity
			if (currentCapacity == newCapacity)
				return currentCapacity;

			data = Alloc<T>(newCapacity);
			return newCapacity;
		}

		NODISCARD virtual size_t Reallocate(Memory& data, const size_t currentCapacity, const size_t newCapacity)
		{
			// Don't reallocate if same capacity
			if (currentCapacity == newCapacity)
				return currentCapacity;

			// Allocate (with constructor call)
			T* newBlock = Alloc<T>(newCapacity);

			// Move memory to new block
			for (size_t i = 0; i < currentCapacity; i++)
				newBlock[i] = std::move(data[i]);

			// Move memory to new block
			for (size_t i = 0; i < currentCapacity; i++)
				data[i].~T();

			// Free invalid memory
			Dispose(data, currentCapacity);

			data = newBlock;
			return newCapacity;
		}

		virtual void ClearMemory(Memory& data, const size_t capacity)
		{
			// Invalidate memory
			for (size_t i = 0; i < capacity; i++)
				data[i].~T();
		}

		virtual void Dispose(Memory& data, const size_t capacity)
		{
			Delete(data.Data, capacity);
		}

		constexpr Allocator& operator=(const Allocator&) noexcept = default;
		constexpr Allocator& operator=(Allocator&&) noexcept = default;
	};

	template <typename T, std::derived_from<Allocator<T>> TAllocator>
	class AllocatorProxy
	{
	public:
		using Memory = Memory<T>;

		static size_t Allocate(Memory& data, const size_t currentCapacity, const size_t newCapacity)
		{
			return s_Allocator.Allocate(data, currentCapacity, newCapacity);
		}

		static size_t Reallocate(Memory& data, const size_t currentCapacity, const size_t newCapacity)
		{
			return s_Allocator.Reallocate(data, currentCapacity, newCapacity);
		}

		static void ClearMemory(Memory& data, const size_t capacity)
		{
			s_Allocator.ClearMemory(data, capacity);
		}

		static void Dispose(Memory& data, const size_t capacity)
		{
			s_Allocator.Dispose(data, capacity);
		}

	private:
		inline static TAllocator s_Allocator = TAllocator{};
	};
}
