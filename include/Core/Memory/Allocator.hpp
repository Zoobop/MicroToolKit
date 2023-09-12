#pragma once
#include "Memory.hpp"

namespace Micro
{
	template <typename T>
	class Allocator final
	{
	public:
		NODISCARD constexpr static size_t Allocate(Memory<T>& data, const size_t currentCapacity, const size_t newCapacity) noexcept
		{
			// Don't allocate if same capacity
			if (currentCapacity == newCapacity)
				return currentCapacity;

			data = Alloc<T>(newCapacity);
			return newCapacity;
		}

		NODISCARD constexpr static size_t Reallocate(Memory<T>& data, const size_t currentCapacity, const size_t newCapacity) noexcept
		{
			// Don't reallocate if same capacity
			if (currentCapacity == newCapacity)
				return currentCapacity;

			// Allocate (with constructor call)
			T* newBlock = Alloc<T>(newCapacity);

			// Move memory to new block and free old block
			for (size_t i = 0; i < currentCapacity; i++)
			{
				newBlock[i] = std::move(data[i]);
				data[i].~T();
			}

			// Free invalid memory
			Dispose(data, currentCapacity);

			data = newBlock;
			return newCapacity;
		}

		constexpr static void ClearMemory(Memory<T>& data, const size_t capacity) noexcept
		{
			// Invalidate memory
			for (size_t i = 0; i < capacity; i++)
				data[i].~T();
		}

		constexpr static void Dispose(Memory<T>& data, const size_t capacity) noexcept { Delete(data.Data, capacity); }
	};
}
