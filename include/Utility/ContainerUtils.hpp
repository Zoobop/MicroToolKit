#pragma once

#include "Common/String.hpp"
#include "Common/StringBuilder.hpp"
#include "Common/Span.hpp"

namespace Micro
{
	NODISCARD constexpr String CollectionToString(const StringBuildable auto* data, const size_t size)
	{
		StringBuilder builder(size);
		builder.Append('[');

		for (size_t i = 0; i < size; i++)
		{
            builder.Append(data[i]);
			if (i != size - 1)
				builder.Append(", ");
		}

		builder.Append(']');
		return builder.ToString();
	}

	template <size_t TSize>
	NODISCARD constexpr String CollectionToString(const StringBuildable auto (&data)[TSize])
	{
		StringBuilder builder(TSize);
		builder.Append('[');

		for (size_t i = 0; i < TSize; i++)
		{
            builder.Append(data[i]);
			if (i != TSize - 1)
				builder.Append(", ");
		}

		builder.Append(']');
		return builder.ToString();
	}

    template <StringBuildable T>
	NODISCARD constexpr String SpanToString(const Span<T>& span)
	{
        const size_t length = span.Capacity();
		StringBuilder builder(length);
		builder.Append('[');

		const T* data = span.Data();
		for (size_t i = 0; i < length; i++)
		{
            builder.Append(data[i]);
			if (i != length - 1)
				builder.Append(", ");
		}

		builder.Append(']');
		return builder.ToString();
	}
}
