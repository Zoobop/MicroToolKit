#pragma once
#include <coroutine>

#include "Core/Core.hpp"

namespace Micro
{
	template <typename T, typename TEnumerator>
	struct EnumeratorType final
	{
		using Handle = std::coroutine_handle<EnumeratorType>;

		mutable T* Current = nullptr;

		NODISCARD constexpr auto yield_value(T& value) noexcept
		{
			Current = &value;
			return std::suspend_always();
		}

		NODISCARD constexpr auto yield_value(const T& value) const noexcept
		{
			Current = const_cast<T*>(&value);
			return std::suspend_always();
		}

		constexpr void return_void() const noexcept { }

		constexpr void unhandled_exception() const noexcept { }

		// Accessors
		NODISCARD constexpr auto get_return_object() noexcept
		{
			return static_cast<TEnumerator>(Handle::from_promise(*this));
		}

		NODISCARD constexpr auto initial_suspend() const noexcept { return std::suspend_always(); }
		NODISCARD constexpr auto final_suspend() const noexcept { return std::suspend_always(); }
	};


	template <typename T>
	class Enumerator final
	{
	public:
		/*
		 *  ============================================================
		 *	|                          Aliases                         |
		 *  ============================================================
		 */


		using promise_type = EnumeratorType<T, Enumerator>;
		using Handle = std::coroutine_handle<promise_type>;


		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		constexpr Enumerator() noexcept = default;

		constexpr Enumerator(const Enumerator& other) noexcept = delete;

		constexpr Enumerator(Enumerator&& other) noexcept
			: m_Handle(other.m_Handle)
		{
			other.m_Handle = nullptr;
		}

		constexpr explicit Enumerator(Handle handle) noexcept
			: m_Handle(handle)
		{
		}

		constexpr ~Enumerator() noexcept
		{
			if (m_Handle)
			{
				m_Handle.destroy();
			}
		}


		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		NODISCARD constexpr T& Current() noexcept { return *m_Handle.promise().Current; }
		NODISCARD constexpr const T& Current() const noexcept { return *m_Handle.promise().Current; }
		NODISCARD constexpr bool HasNext() const noexcept { return !m_Handle.done(); }
		NODISCARD constexpr bool MoveNext() const noexcept
		{
			if (HasNext())
			{
				m_Handle.resume();
			}

			return HasNext();
		}


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		constexpr explicit operator Handle() const noexcept { return m_Handle; }
		constexpr explicit operator bool() const noexcept { return HasNext(); }

		constexpr Enumerator& operator=(const Enumerator&) noexcept = delete;
		constexpr Enumerator& operator=(Enumerator&&) noexcept = delete;

	private:
		mutable Handle m_Handle;
	};

}
