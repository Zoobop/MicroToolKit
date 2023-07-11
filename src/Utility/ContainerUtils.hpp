#pragma once

#include "Common/String.hpp"
#include "Common/StringBuilder.hpp"
#include "Collections/Array.hpp"

namespace Micro
{
	template <typename T, size_t TSize>
	NODISCARD String ArrayToString(const Array<T, TSize>& array)
	{
		StringBuilder builder(TSize);

		builder.Append('[');

		const T* data = array.Data();
		for (size_t i = 0; i < TSize; i++)
		{
			try
			{
				builder.Append(data[i]);
			}
			catch (const Exception&)
			{
				builder.Append(&data[i]);
			}

			if (i != TSize - 1)
				builder.Append(", ");
		}

		builder.Append(']');
		return builder.ToString();
	}

	template <typename T>
	NODISCARD String ArrayToString(const T* data, const size_t size)
	{
		StringBuilder builder(size);

		builder.Append('[');

		for (size_t i = 0; i < size; i++)
		{
			try
			{
				builder.Append(data[i]);
			}
			catch (const Exception&)
			{
				builder.Append(&data[i]);
			}

			if (i != size - 1)
				builder.Append(", ");
		}

		builder.Append(']');
		return builder.ToString();
	}

	template <typename T, size_t TSize>
	NODISCARD String ArrayToString(const T (&data)[TSize])
	{
		StringBuilder builder(TSize);

		builder.Append('[');

		for (size_t i = 0; i < TSize; i++)
		{
			try
			{
				builder.Append(data[i]);
			}
			catch (const Exception&)
			{
				builder.Append(&data[i]);
			}

			if (i != TSize - 1)
				builder.Append(", ");
		}

		builder.Append(']');
		return builder.ToString();
	}
}
