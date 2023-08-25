#pragma once
#include "Enumerator.hpp"
#include "Core/Core.hpp"

namespace Micro
{
	template <typename T>
	class Enumerable
	{
	public:
		/*
		 *  ============================================================
		 *	|                          Aliases                         |
		 *  ============================================================
		 */


		using Type = T;
		using Iterator = CoroutineIterator<T>;
		using ConstIterator = ConstCoroutineIterator<T>;
		using Enumerator = Enumerator<T>;

		
		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		constexpr Enumerable() noexcept = default;
		constexpr Enumerable(const Enumerable&) noexcept = default;
		constexpr Enumerable(Enumerable&&) noexcept = default;
		constexpr virtual ~Enumerable() noexcept = default;

		
		/*
		 *  ============================================================
		 *	|                         Accessors                        |
		 *  ============================================================
		 */


		NODISCARD virtual Enumerator GetEnumerator() { return {}; }
		NODISCARD virtual Enumerator GetEnumerator() const { return {}; }

		NODISCARD constexpr Iterator begin() noexcept { return Iterator(GetEnumerator()); }
		NODISCARD constexpr Iterator end() noexcept { return Iterator(); }
		NODISCARD constexpr ConstIterator begin() const noexcept { return ConstIterator(GetEnumerator()); }
		NODISCARD constexpr ConstIterator end() const noexcept { return ConstIterator(); }

		
		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		constexpr Enumerable& operator=(const Enumerable&) noexcept = default;
		constexpr Enumerable& operator=(Enumerable&&) noexcept = default;
	};
}
