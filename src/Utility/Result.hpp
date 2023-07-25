#pragma once
#include "Core/Core.hpp"

#include <ostream>

namespace Micro
{
	template <typename T, bool TRef = false>
	class Result;

	template <typename T>
	class Result<T, true> final
	{
	public:
		// Constructors/Destructors
		constexpr Result() noexcept = default;

		constexpr explicit Result(const T& value) noexcept
			: m_Value(&value)
		{
		}

		// Accessors
		NODISCARD constexpr T& Value() { return *m_Value; }
		NODISCARD constexpr const T& Value() const { return *m_Value; }
		NODISCARD constexpr bool IsValid() const noexcept { return m_Value != nullptr; }

		// Static
		NODISCARD constexpr static Result Empty() { return Result{}; }

		// Operator overloads
		NODISCARD constexpr operator T() const { return *m_Value; }

		friend std::ostream& operator<<(std::ostream& stream, const Result& result)
		{
			if (result.IsValid())
				stream << *result.m_Value;
			else
				stream << "Invalid";
			return stream;
		}

	private:
		const T* m_Value = nullptr;
	};

	template <typename T>
	class Result<T, false> final
	{
	public:
		// Constructors/Destructors
		constexpr Result() noexcept = default;

		constexpr explicit Result(const T& value) noexcept
			: m_Value(value), m_IsValid(true)
		{
		}

		// Accessors
		NODISCARD constexpr T& Value() noexcept { return m_Value; }
		NODISCARD constexpr const T& Value() const noexcept { return m_Value; }
		NODISCARD constexpr bool IsValid() const noexcept { return m_IsValid; }

		// Static
		NODISCARD constexpr static Result Empty() noexcept { return {}; }

		// Operator overloads
		NODISCARD constexpr operator T() const noexcept { return m_Value; }

		constexpr friend std::ostream& operator<<(std::ostream& stream, const Result& result) noexcept
		{
			if (result.IsValid())
				stream << result.m_Value;
			else
				stream << "Invalid";
			return stream;
		}

	private:
		T m_Value{};
		bool m_IsValid = false;
	};

	template <typename T>
	using RefResult = Result<T, true>;
}
