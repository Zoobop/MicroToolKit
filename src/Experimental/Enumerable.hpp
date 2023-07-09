#pragma once
#include "Enumerator.hpp"
#include "Core/Core.hpp"

namespace Micro
{
	template <typename T>
	class Enumerable
	{
	public:
		using Iterator = Iterator<T>;
		using ConstIterator = const Iterator;
		using Enumerator = Enumerator<T>;

		friend Enumerator;

		constexpr virtual ~Enumerable() noexcept = default;

		NODISCARD virtual Enumerator GetEnumerator() = 0;

		NODISCARD constexpr Iterator begin() noexcept { return Iterator(GetEnumerator()); }
		NODISCARD constexpr Iterator end() noexcept { return Iterator(); }
		NODISCARD constexpr ConstIterator cbegin() const noexcept { return begin(); }
		NODISCARD constexpr ConstIterator cend() const noexcept { return end(); }
	};
}
