#pragma once
#include "Core/Core.hpp"

namespace mtk
{
	template <typename T>
	class Result final
	{
	public:
		// Constructors/Destructors
		constexpr Result() noexcept = default;

		explicit constexpr Result(const T& value) noexcept
			: m_Value(&value)
		{
		}

		// Accessors
		NODISCARD constexpr T& Value() const { return *m_Value; }
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
}
