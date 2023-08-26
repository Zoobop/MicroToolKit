#pragma once
#include <concepts>

#include "Core/Core.hpp"

namespace Micro::Internal
{
	template <typename TElem, typename UType>
	NODISCARD constexpr TElem* UIntegral_Internal(TElem* rNext, UType value) noexcept
	{
		// format value into buffer *ending at* rNext
		static_assert(std::is_unsigned_v<UType>, "value must be unsigned");

#ifdef _WIN64
		auto truncatedValue = value;
#else // ^^^ _WIN64 ^^^ // vvv !_WIN64 vvv

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
#endif // _WIN64

		do {
			*--rNext = static_cast<TElem>('0' + truncatedValue % 10);
			truncatedValue /= 10;
		} while (truncatedValue != 0);
		return rNext;
	}
}
