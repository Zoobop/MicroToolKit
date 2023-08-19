#pragma once
#include "Collections/Base/Span.hpp"
#include "Core/Function.hpp"

namespace Micro
{
	template <typename T>
	concept Comparable = requires(T left, T right)
	{
		{ operator>(left, right) } -> std::convertible_to<bool>;
		{ operator>=(left, right) } -> std::convertible_to<bool>;
		{ operator<(left, right) } -> std::convertible_to<bool>;
		{ operator<=(left, right) } -> std::convertible_to<bool>;
		{ operator==(left, right) } -> std::convertible_to<bool>;
		{ operator!=(left, right) } -> std::convertible_to<bool>;
	};

	template <Comparable T>
	NODISCARD constexpr bool GreaterThan(const T& left, const T& right) noexcept { return left > right; }

	template <Comparable T>
	NODISCARD constexpr bool GreaterThanEqual(const T& left, const T& right) noexcept { return left >= right; }

	template <Comparable T>
	NODISCARD constexpr bool LessThan(const T& left, const T& right) noexcept { return left < right; }

	template <Comparable T>
	NODISCARD constexpr bool LessThanEqual(const T& left, const T& right) noexcept { return left <= right; }

	template <Comparable T>
	constexpr void Sort(Span<T>& sequence, const Func<bool, T, T>& predicate = GreaterThan<T>) noexcept
	{
		auto data = sequence.Data();
		const size_t size = sequence.Size();

		for (size_t i = 0; i < size; i++)
		{
			for (size_t j = i + 1; j < size; j++)
			{
				if (!predicate(data[i], data[j]))
					continue;

				auto& temp = std::move(data[i]);
				data[i] = std::move(data[j]);
				data[j] = std::move(temp);
			}
		}
	}

	template <Comparable T>
	constexpr void ReverseSort(Span<T>& sequence, const Func<bool, T, T>& predicate = GreaterThan<T>) noexcept
	{
		auto data = sequence.Data();
		const size_t size = sequence.Size();

		for (int32_t i = size - 1; i > 0; --i)
		{
			for (int32_t j = i - 1; j >= 0; --j)
			{
				if (!predicate(data[i], data[j]))
					continue;

				auto& temp = std::move(data[i]);
				data[i] = std::move(data[j]);
				data[j] = std::move(temp);
			}
		}

	}
}
