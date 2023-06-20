#pragma once

#include <memory>
#include "Exceptions/Exception.hpp"

#define INVALID_MEM_ADDRESS 0xcdcdcdcdcdcdcdcd

namespace mtk
{
	constexpr errno_t Success = 0;
	constexpr errno_t Error = -1;

	template <typename T>
	constexpr bool IsValidMemory(T* data)
	{
		return !(data == nullptr || data == reinterpret_cast<T*>(INVALID_MEM_ADDRESS));
	}

	template <typename T>
	errno_t Copy(T* source, const size_t sourceSize, T* destination, const size_t destinationSize)
	{
		if (destinationSize < sourceSize)
			return Error;

		for (size_t i = 0; i < sourceSize; i++)
			destination[i] = source[i];

		return Success;
	}

	template <typename T>
	errno_t Move(T* source, const size_t sourceSize, T* destination, const size_t destinationSize)
	{
		if (destinationSize < sourceSize)
			return Error;

		for (size_t i = 0; i < sourceSize; i++)
			destination[i] = std::move(source[i]);

		return Success;
	}

	template <typename T>
	void ShiftRight(T* data, const size_t size, const size_t startIndex, const size_t amount = 1)
	{
		for (size_t i = size - 1; i >= startIndex; i--)
		{
			const size_t offset = i + amount;
			auto temp = std::move(data[i]);
			data[i] = std::move(data[offset]);
			data[offset] = std::move(temp);
		}
	}

	template <typename T>
	void ShiftLeft(T* data, const size_t size, const size_t startIndex, const size_t amount = 1)
	{
		const size_t length = size - startIndex;
		for (size_t i = 0; i < length; i++)
		{
			const size_t offset = i + startIndex - amount;
			const size_t index = i + startIndex;
			auto temp = std::move(data[index]);
			data[index] = std::move(data[offset]);
			data[offset] = std::move(temp);
		}
	}


	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	T* Alloc(const size_t size)
	{
		return static_cast<T*>(operator new(size * sizeof(T)));
	}

	template <typename T>
	void Delete(T* block, const size_t size)
	{
		::operator delete(block, size * sizeof(T));
	}


	enum struct MemStatus : uint8_t { Valid = 0, Invalid = 1 };

	template <typename T>
	struct Memory final
	{
		constexpr Memory() noexcept : Data(nullptr), Status(MemStatus::Valid)
		{
		}

		constexpr Memory(std::nullptr_t) noexcept : Data(nullptr), Status(MemStatus::Valid)
		{
		}

		constexpr Memory(T* data) noexcept : Data(data), Status(MemStatus::Valid)
		{
		}

		constexpr void Invalidate() noexcept
		{
			Data = nullptr;
			Status = MemStatus::Invalid;
		}

		NODISCARD constexpr bool IsValidMemory() noexcept
		{
			return !(Data == nullptr || Status != MemStatus::Valid);
		}

		Memory& operator=(Memory&& other) noexcept
		{
			if (this == &other)
				return *this;

			Data = other.Data;

			other.Invalidate();
			return *this;
		}

		NODISCARD constexpr operator T*() const { return Data; }
		NODISCARD T& operator[](const size_t index) { return Data[index]; }
		NODISCARD const T& operator[](const size_t index) const { return Data[index]; }

		T* Data;
		MemStatus Status;
	};
}
