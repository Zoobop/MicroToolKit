#pragma once
#include "Core/Core.hpp"
#include "Core/Typedef.hpp"
#include "Core/Memory/Memory.hpp"

namespace Micro
{
	template <typename T>
	struct Buffer final
	{
		T* Data;
		u64 Length;

		NODISCARD static constexpr Buffer Allocate(const u64 length) noexcept 
		{ 
			return Buffer{ .Data = Alloc<T>(length), .Length = length };
		}

		NODISCARD constexpr T& operator[](const u64 index) { return Data[index]; }
		NODISCARD constexpr const T& operator[](const u64 index) const { return Data[index]; }
	};

}