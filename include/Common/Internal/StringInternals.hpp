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
	NODISCARD Span<char> FloatToString_Internal(const char* fmt, const std::floating_point auto floatingPoint) noexcept
	{
		const u64 length = _scprintf(fmt, floatingPoint);
		char* buffer = Alloc<char>(length + 1);

		auto _ = sprintf_s(buffer, length + 1, fmt, floatingPoint);
		return Span{ buffer, buffer + length };
	}
#else
	struct InternalBuffer final
	{
		char* Data;
		u64 Length;

		constexpr InternalBuffer(char* begin, const char* end) noexcept
			: Length(end - begin)
		{
			Data = Alloc<char>(Length + 1);
			Copy(begin, Length, Data, Length + 1);
			Data[Length] = 0;
		}
	};

	template <std::floating_point TFloat>
	NODISCARD Span<char> FloatToString_Internal(const char* fmt, const TFloat floatingPoint) noexcept
	{
		const u64 maxSize = __gnu_cxx::__numeric_traits<TFloat>::__max_exponent10 + 20;
		const auto buffer = __gnu_cxx::__to_xstring<InternalBuffer, char>(&std::vsnprintf, maxSize, fmt, floatingPoint);
		return Span{ buffer.Data, buffer.Length };
	}
#endif
}
