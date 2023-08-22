#pragma once
#include <memory>
#include <ostream>

#include "Core/Core.hpp"
#include "Core/Typedef.hpp"

namespace Micro
{
	template <typename T>
	constexpr bool Copy(T* source, const size_t sourceSize, T* destination, const size_t destinationSize) noexcept
	{
		if (destinationSize < sourceSize)
			return false;

		for (size_t i = 0; i < sourceSize; i++)
			destination[i] = source[i];

		return true;
	}

	template <typename T>
	constexpr bool Move(T* source, const size_t sourceSize, T* destination, const size_t destinationSize) noexcept
	{
		if (destinationSize < sourceSize)
			return false;

		for (size_t i = 0; i < sourceSize; i++)
			destination[i] = std::move(source[i]);

		return true;
	}

	template <typename T>
	constexpr void ShiftRight(T* data, const size_t size, const size_t startIndex, const size_t amount = 1)
	{
		for (size_t i = size; i > startIndex; i--)
		{
			const size_t offset = (i - 1) + amount;
			auto temp = std::move(data[i - 1]);
			data[i - 1] = std::move(data[offset]);
			data[offset] = std::move(temp);
		}
	}

	template <typename T>
	constexpr void ShiftLeft(T* data, const size_t size, const size_t startIndex, const size_t amount = 1)
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

	NODISCARD constexpr size_t GetLength(const char* str) noexcept
	{
		size_t size = 0;
        if (str == nullptr)
            return size;

		while (str[size] != '\0')
			size++;
		return size;
	}


	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T, typename... Args>
	NODISCARD constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename... Args>
	NODISCARD constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	NODISCARD T* StackAlloc(const size_t size)
	{
		return static_cast<T*>(alloca(size * sizeof(T)));
	}

	template <typename T>
	NODISCARD T* Alloc(const size_t size)
	{
		return static_cast<T*>(operator new(size * sizeof(T)));
	}

	template <typename T>
	void Delete(T* block, const size_t size)
	{
		::operator delete(block, size * sizeof(T));
	}

	enum struct MemStatus : uint8_t { Valid = 0, Invalid = 1 };

	inline std::ostream& operator<<(std::ostream& stream, const MemStatus memStatus) noexcept
	{
		stream << (memStatus == MemStatus::Valid ? "Valid" : "Invalid");
		return stream;
	}

	template <typename T>
	struct Memory final
	{
		constexpr Memory() noexcept : Data(nullptr), Status(MemStatus::Valid)
		{
		}

		constexpr Memory(Null) noexcept : Data(nullptr), Status(MemStatus::Valid)
		{
		}

		constexpr Memory(const Memory&) noexcept = default;
		constexpr Memory(Memory&&) noexcept = default;

		constexpr Memory(T* data) noexcept : Data(data), Status(MemStatus::Valid)
		{
		}

		constexpr Memory(const T* data) noexcept : Data(const_cast<T*>(data)), Status(MemStatus::Valid)
		{
		}

		constexpr ~Memory() noexcept = default;

		constexpr void Invalidate() noexcept
		{
			Data = nullptr;
			Status = MemStatus::Invalid;
		}

		NODISCARD constexpr bool IsValidMemory() const noexcept
		{
			return !(Data == nullptr || Status != MemStatus::Valid);
		}

		constexpr Memory& operator=(const Memory& other) noexcept
		{
			if (this == &other)
				return *this;

			Data = other.Data;

			return *this;
		}

		constexpr Memory& operator=(Memory&& other) noexcept
		{
			if (this == &other)
				return *this;

			Data = other.Data;

			other.Invalidate();
			return *this;
		}

		NODISCARD constexpr operator T*() const noexcept { return Data; }
		NODISCARD constexpr T& operator[](const size_t index) { return Data[index]; }
		NODISCARD constexpr const T& operator[](const size_t index) const { return Data[index]; }

		T* Data;
		MemStatus Status;
	};
}
