#pragma once
#include <ostream>

#include "Core/Core.hpp"

namespace Micro
{
	template <typename ...>
	class Optional;


	template <typename T>
	class Optional<T> final
	{
	public:
		/*
		 *  ============================================================
		 *	|                 Constructors/Destructors                 |
		 *  ============================================================
		 */


		constexpr Optional() noexcept = default;

		constexpr explicit Optional(T value) noexcept
			: m_Value(std::move(value)), m_IsValid(true)
		{
		}

		
		/*
		 *  ============================================================
		 *	|                         Accessors                        |
		 *  ============================================================
		 */


		NODISCARD constexpr T& Value() noexcept { return m_Value; }
		NODISCARD constexpr const T& Value() const noexcept { return m_Value; }
		NODISCARD constexpr bool IsValid() const noexcept { return m_IsValid; }

		
		/*
		 *  ============================================================
		 *	|                          Static                          |
		 *  ============================================================
		 */


		NODISCARD constexpr static Optional Empty() noexcept { return Optional{}; }


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		NODISCARD constexpr operator T() const noexcept { return m_Value; }

		constexpr friend bool operator==(const Optional& left, const Optional& right) noexcept
		{
			return left.m_Value == right.m_Value;
		}

		constexpr friend bool operator!=(const Optional& left, const Optional& right) noexcept
		{
			return left.m_Value != right.m_Value;
		}

		friend std::ostream& operator<<(std::ostream& stream, const Optional& result) noexcept
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
	class Optional<T&> final
	{
	public:
		/*
		 *  ============================================================
		 *	|                 Constructors/Destructors                 |
		 *  ============================================================
		 */


		constexpr Optional() noexcept = default;

		constexpr explicit Optional(const T& value) noexcept
			: m_Value(&value)
		{
		}

		
		/*
		 *  ============================================================
		 *	|                         Accessors                        |
		 *  ============================================================
		 */


		NODISCARD constexpr T& Value() { return *const_cast<T*>(m_Value); }
		NODISCARD constexpr const T& Value() const { return *m_Value; }
		NODISCARD constexpr bool IsValid() const noexcept { return m_Value != nullptr; }

		
		/*
		 *  ============================================================
		 *	|                          Static                          |
		 *  ============================================================
		 */
		
		
		NODISCARD constexpr static Optional Empty() noexcept { return Optional{}; }

		
		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		NODISCARD constexpr operator T() const { return *m_Value; }

		constexpr friend bool operator==(const Optional& left, const Optional& right) noexcept
		{
			return left.m_Value == right.m_Value;
		}

		constexpr friend bool operator!=(const Optional& left, const Optional& right) noexcept
		{
			return left.m_Value != right.m_Value;
		}

		friend std::ostream& operator<<(std::ostream& stream, const Optional& result) noexcept
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
