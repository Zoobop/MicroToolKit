#pragma once

#include "Common/String.hpp"
#include "Common/StringBuilder.hpp"
#include "Common/Span.hpp"

namespace Micro
{
	NODISCARD constexpr string CollectionToString(const StringBuildable auto* data, const usize size)
	{
		StringBuilder builder(size);
		builder.Append('[');

		for (usize i = 0; i < size; i++)
		{
            builder.Append(data[i]);
			if (i != size - 1)
				builder.Append(", ");
		}

		builder.Append(']');
		return builder.ToString();
	}

	template <usize TSize>
	NODISCARD constexpr string CollectionToString(const StringBuildable auto (&data)[TSize])
	{
		StringBuilder builder(TSize);
		builder.Append('[');

		for (usize i = 0; i < TSize; i++)
		{
            builder.Append(data[i]);
			if (i != TSize - 1)
				builder.Append(", ");
		}

		builder.Append(']');
		return builder.ToString();
	}

    template <StringBuildable T>
	NODISCARD constexpr string SpanToString(const Span<T>& span)
	{
        const usize length = span.Capacity();
		StringBuilder builder(length);
		builder.Append('[');

		const T* data = span.Data();
		for (usize i = 0; i < length; i++)
		{
            builder.Append(data[i]);
			if (i != length - 1)
				builder.Append(", ");
		}

		builder.Append(']');
		return builder.ToString();
	}
}
