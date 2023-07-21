#pragma once
#include "Enumerator.hpp"
#include "Core/Core.hpp"

namespace Micro
{
	template <typename T>
	class Enumerable
	{
	public:
		using Type = T;
		using Iterator = Iterator<T>;
		using ConstIterator = const Iterator;
		using Enumerator = Enumerator<T>;

		// Constructors/Destructor
		constexpr Enumerable() noexcept = default;
		constexpr Enumerable(const Enumerable&) noexcept = default;
		constexpr Enumerable(Enumerable&&) noexcept = default;
		constexpr virtual ~Enumerable() noexcept = default;

		// Enumerator
		NODISCARD virtual Enumerator GetEnumerator() { return {}; }
		NODISCARD virtual Enumerator GetEnumerator() const { return {}; }

		// Underlying Iterators
		NODISCARD constexpr Iterator begin() noexcept { return Iterator(GetEnumerator()); }
		NODISCARD constexpr Iterator end() noexcept { return Iterator(); }
		NODISCARD constexpr ConstIterator cbegin() const noexcept { return Iterator(GetEnumerator()); }
		NODISCARD constexpr ConstIterator cend() const noexcept { return Iterator(); }

		// Operator Overloads
		constexpr Enumerable& operator=(const Enumerable&) noexcept = default;
		constexpr Enumerable& operator=(Enumerable&&) noexcept = default;
	};
}
