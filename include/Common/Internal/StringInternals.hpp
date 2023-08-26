#pragma once
#include <concepts>

#include "Core/Core.hpp"
#include "Core/Typedef.hpp"
#include "Common/Span.hpp"
#include "Core/Memory/Memory.hpp"

namespace Micro::Internal
{
	template <typename TElem, typename UType>
	NODISCARD constexpr TElem* UIntegral_Internal(TElem* rNext, UType value) noexcept
	{
		// format value into buffer *ending at* rNext
		static_assert(std::is_unsigned_v<UType>, "value must be unsigned");

#ifdef _WIN64
		auto truncatedValue = value;
#else

		constexpr bool isBigUnsigned = sizeof(UType) > 4;
		if constexpr (isBigUnsigned) { // For 64-bit numbers, work in chunks to avoid 64-bit divisions.
			while (value > 0xFFFFFFFFU) {
				auto chunk = static_cast<unsigned long>(value % 1000000000);
				value /= 1000000000;

				for (int i = 0; i != 9; ++i) {
					*--rNext = static_cast<TElem>('0' + chunk % 10);
					chunk /= 10;
				}
			}
		}

		auto truncatedValue = static_cast<unsigned long>(value);
#endif

		do {
			*--rNext = static_cast<TElem>('0' + truncatedValue % 10);
			truncatedValue /= 10;
		} while (truncatedValue != 0);
		return rNext;
	}


#ifdef _WIN64
	NODISCARD inline Span<char> FloatToString_Internal(const char* fmt, const f64 floatingPoint) noexcept
	{
		const u64 length = _scprintf(fmt, floatingPoint);
		char* buffer = Alloc<char>(length + 1);

		auto _ = sprintf_s(buffer, length + 1, fmt, floatingPoint);
		return Span{ buffer, buffer + length };
	}
#else
	NODISCARD inline Span<char> FloatToString_Internal(const char* fmt, ...) noexcept
	{
		const u64 size = __gnu_cxx::__numeric_traits<f128>::__max_exponent10 + 20;
		char* buffer = static_cast<char*>(__builtin_alloca(sizeof(char) * size));

		__builtin_va_list args;
		__builtin_va_start(args, fmt);

		const u64 length = std::vsnprintf(buffer, size, fmt, args);

		__builtin_va_end(args);

		return Span{ buffer, buffer + length };
	}
#endif
}
