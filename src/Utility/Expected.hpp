#pragma once
#include "Core/Core.hpp"

namespace mtk
{
	template <typename TResult, typename TFallback>
	NODISCARD struct Expected
	{
		constexpr Expected(const TResult& result) noexcept
			: m_Result(&result), m_Fallback(nullptr)
		{
		}

		constexpr Expected(const TFallback& fallback) noexcept
			: m_Result(nullptr), m_Fallback(&fallback)
		{
		}

		NODISCARD constexpr bool IsValid() const { return m_Result != nullptr; }
		NODISCARD constexpr const TResult& Result() const { return *m_Result; }
		NODISCARD constexpr const TFallback& Fallback() const { return *m_Fallback; }

		constexpr operator TResult() const { return *m_Result; }
		constexpr operator TFallback() const { return *m_Fallback; }

		friend std::ostream& operator<<(std::ostream& stream, const Expected& expected)
		{
			if (expected.IsValid())
			{
				stream << expected.Result();
			}
			else
			{
				stream << expected.Fallback();
			}

			return stream;
		}

	private:
		const TResult* m_Result;
		const TFallback* m_Fallback;
	};
}
