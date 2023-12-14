#pragma once
#include <ostream>

#include "Core/Core.hpp"
#include "Core/Errors/Error.hpp"

namespace Micro
{
	template <typename ...>
	class Result;


	template <typename T>
	class Result<T> final
	{
	private:
		/*
		 *  ============================================================
		 *	|                 Constructors/Destructors                 |
		 *  ============================================================
		 */


		constexpr explicit Result(const T& value) noexcept
			: m_Value(value), m_Error(), m_IsValid(true)
		{
		}

		constexpr explicit Result(T&& value) noexcept
			: m_Value(std::move(value)), m_Error(), m_IsValid(true)
		{
		}

		constexpr explicit Result(const Error& error) noexcept
			: m_Value(), m_Error(error), m_IsValid(false)
		{
		}

		constexpr explicit Result(Error&& error) noexcept
			: m_Value(), m_Error(std::move(error)), m_IsValid(false)
		{
		}

	public:
		/*
		 *  ============================================================
		 *	|                         Accessors                        |
		 *  ============================================================
		 */
		
		
		NODISCARD constexpr T& Value() noexcept { return m_Value; }
		NODISCARD constexpr const T& Value() const noexcept { return m_Value; }
		NODISCARD constexpr const Error& ErrorHandle() const noexcept { return m_Error; }
		NODISCARD constexpr bool IsValid() const noexcept { return m_IsValid; }

		
		/*
		 *  ============================================================
		 *	|                          Static                          |
		 *  ============================================================
		 */


		NODISCARD constexpr static Result Ok(const T& value) noexcept { return Result(value); }
		NODISCARD constexpr static Result Ok(T&& value) noexcept { return Result(std::move(value)); }
		NODISCARD constexpr static Result CaptureError(const Error& error) noexcept { return Result(error); }
		NODISCARD constexpr static Result CaptureError(Error&& error) noexcept { return Result(std::move(error)); }

		
		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		NODISCARD constexpr operator T() const noexcept { return m_Value; }
		NODISCARD constexpr operator Error() const noexcept { return m_Error; }

		friend std::ostream& operator<<(std::ostream& stream, const Result& result) noexcept
		{
			if (result.IsValid())
				stream << result.m_Value;
			else
				stream << "Error: " << result.m_Error.Message();
			return stream;
		}

	private:
		T m_Value;
		Error m_Error;
		bool m_IsValid;
	};


	template <typename T>
	class Result<T&> final
	{
	private:
		/*
		 *  ============================================================
		 *	|                 Constructors/Destructors                 |
		 *  ============================================================
		 */


		constexpr explicit Result(const T& value) noexcept
			: m_Value(&value), m_Error()
		{
		}

		constexpr explicit Result(const Error& error) noexcept
			: m_Value(nullptr), m_Error(error)
		{
		}

		constexpr explicit Result(Error&& error) noexcept
			: m_Value(nullptr), m_Error(std::move(error))
		{
		}

	public:
		/*
		 *  ============================================================
		 *	|                         Accessors                        |
		 *  ============================================================
		 */


		NODISCARD constexpr T& Value() { return *const_cast<T*>(m_Value); }
		NODISCARD constexpr const T& Value() const { return *m_Value; }
		NODISCARD constexpr const Error& ErrorHandle() const noexcept { return m_Error; }
		NODISCARD constexpr bool IsValid() const noexcept { return m_Value != nullptr; }

		
		/*
		 *  ============================================================
		 *	|                          Static                          |
		 *  ============================================================
		 */


		NODISCARD constexpr static Result Ok(const T& value) noexcept { return Result(value); }
		NODISCARD constexpr static Result CaptureError(const Error& error) noexcept { return Result(error); }
		NODISCARD constexpr static Result CaptureError(Error&& error) noexcept { return Result(std::move(error)); }


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		NODISCARD constexpr operator T() const { return *m_Value; }
		NODISCARD constexpr operator T&() { return *const_cast<T*>(m_Value); }
		NODISCARD constexpr operator T&() const { return *m_Value; }
		NODISCARD constexpr operator Error() const noexcept { return m_Error; }

		friend std::ostream& operator<<(std::ostream& stream, const Result& result) noexcept
		{
			if (result.IsValid())
				stream << *result.m_Value;
			else
				stream << "Error: " << result.m_Error.Message();
			return stream;
		}

	private:
		const T* m_Value;
		Error m_Error;
	};
}
