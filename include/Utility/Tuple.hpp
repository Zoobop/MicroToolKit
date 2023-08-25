#pragma once

#include <ostream>

namespace Micro
{
	template <typename... T>
	struct Tuple;

	template <typename T>
	struct Tuple<T> final
	{
		T Component;

		constexpr Tuple(T component) noexcept
			: Component(std::move(component))
		{
		}

		friend std::ostream& operator<<(std::ostream& stream, const Tuple& tuple)
		{
			stream << '[' << tuple.Component << ']';
			return stream;
		}
	};

	template <typename T1, typename T2>
	struct Tuple<T1, T2> final
	{
		T1 Component1;
		T2 Component2;

		constexpr Tuple(T1 component1, T2 component2) noexcept
			: Component1(std::move(component1)), Component2(std::move(component2))
		{
		}

		friend std::ostream& operator<<(std::ostream& stream, const Tuple& tuple)
		{
			stream << '[' << tuple.Component1 << ", " << tuple.Component2 << ']';
			return stream;
		}
	};

	template <typename T1, typename T2, typename T3>
	struct Tuple<T1, T2, T3> final
	{
		T1 Component1;
		T2 Component2;
		T3 Component3;

		constexpr Tuple(T1 component1, T2 component2, T3 component3) noexcept
			: Component1(std::move(component1)), Component2(std::move(component2)), Component3(std::move(component3))
		{
		}

		friend std::ostream& operator<<(std::ostream& stream, const Tuple& tuple)
		{
			stream << '[' << tuple.Component1 << ", " << tuple.Component2 << ", " << tuple.Component3 << ']';
			return stream;
		}
	};

	template <typename T1, typename T2, typename T3, typename T4>
	struct Tuple<T1, T2, T3, T4> final
	{
		T1 Component1;
		T2 Component2;
		T3 Component3;
		T4 Component4;

		constexpr Tuple(T1 component1, T2 component2, T3 component3, T4 component4) noexcept
			: Component1(std::move(component1)), Component2(std::move(component2)), Component3(std::move(component3)),
			  Component4(std::move(component4))
		{
		}

		friend std::ostream& operator<<(std::ostream& stream, const Tuple& tuple)
		{
			stream << '[' << tuple.Component1 << ", " << tuple.Component2 << ", " << tuple.Component3 << ", " << tuple.
				Component4 << ']';
			return stream;
		}
	};

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	struct Tuple<T1, T2, T3, T4, T5> final
	{
		T1 Component1;
		T2 Component2;
		T3 Component3;
		T4 Component4;
		T5 Component5;

		constexpr Tuple(T1 component1, T2 component2, T3 component3, T4 component4, T5 component5) noexcept
			: Component1(std::move(component1)), Component2(std::move(component2)), Component3(std::move(component3)),
			  Component4(std::move(component4)), Component5(std::move(component5))
		{
		}

		friend std::ostream& operator<<(std::ostream& stream, const Tuple& tuple)
		{
			stream << '[' << tuple.Component1 << ", " << tuple.Component2 << ", " << tuple.Component3 << ", " << tuple.
				Component4 << ", " << tuple.Component5 << ']';
			return stream;
		}
	};

	template <typename... T>
	NODISCARD constexpr auto MakeTuple(T... values) noexcept
	{
		return Tuple<T...>{std::forward<T>(std::move(values))...};
	}
}
