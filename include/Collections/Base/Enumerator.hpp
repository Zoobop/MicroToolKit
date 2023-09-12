#pragma once
#include <coroutine>
#include <exception>

#include "Core/Core.hpp"

namespace Micro
{
	template <typename T, typename TEnumerator>
	struct PromiseType final
	{
		using Handle = std::coroutine_handle<PromiseType>;

		mutable T Current;
		mutable std::exception_ptr Exception;

		NODISCARD constexpr auto yield_value(const std::convertible_to<T> auto& value) const noexcept
		{
			Current = value;
			return std::suspend_always();
		}

		NODISCARD constexpr auto yield_value(std::convertible_to<T> auto&& value) const noexcept
		{
			Current = std::forward<T>(value);
			return std::suspend_always();
		}

		constexpr void return_void() const noexcept
		{
		}

		constexpr void unhandled_exception() const noexcept
		{
			Exception = std::current_exception();
		}

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


		using promise_type = PromiseType<T, Enumerator>;
		using Handle = std::coroutine_handle<promise_type>;


		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		constexpr Enumerator() noexcept = default;

		constexpr Enumerator(const Enumerator& other) noexcept
			: m_Handle(other.m_Handle)
		{
		}

		constexpr Enumerator(Enumerator&& other) noexcept
			: m_Handle(other.m_Handle)
		{
			other.m_Handle = nullptr;
		}

		constexpr explicit Enumerator(Handle handle) noexcept
			: m_Handle(handle)
		{
		}

		constexpr explicit Enumerator(const bool boolean) noexcept
			: m_Handle(nullptr), m_IsFull(boolean)
		{
		}

		constexpr ~Enumerator() noexcept
		{
			if (m_Handle)
				m_Handle.destroy();
		}

		
		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		NODISCARD constexpr T& Current() noexcept
		{
			TryFill();
			m_IsFull = false;
			return m_Handle.promise().Current;
		}

		NODISCARD constexpr const T& Current() const noexcept
		{
			TryFill();
			m_IsFull = false;
			return m_Handle.promise().Current;
		}

		NODISCARD constexpr bool HasNext() const noexcept { return !m_Handle.done(); }
		NODISCARD constexpr bool MoveNext() { return operator bool(); }
		NODISCARD constexpr bool MoveNext() const { return operator bool(); }

		
		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		constexpr explicit operator Handle() const noexcept { return m_Handle; }

		constexpr explicit operator bool()
		{
			TryFill();
			return !m_Handle.done();
		}

		constexpr explicit operator bool() const
		{
			TryFill();
			return !m_Handle.done();
		}

		constexpr T& operator=(const Enumerator&) noexcept = delete;
		constexpr T& operator=(Enumerator&&) noexcept = delete;

	private:
		/*
		 *  ============================================================
		 *	|                      Internal Helpers                    |
		 *  ============================================================
		 */


		constexpr void TryFill() const
		{
			if (!m_IsFull)
			{
				m_Handle();
				if (m_Handle.promise().Exception)
					std::rethrow_exception(m_Handle.promise().Exception);

				m_IsFull = true;
			}
		}

	private:
		mutable Handle m_Handle;
		mutable bool m_IsFull = false;
	};

	template <typename T>
	class CoroutineIterator final
	{
	public:
		constexpr CoroutineIterator() noexcept = default;

		constexpr explicit CoroutineIterator(Enumerator<T>&& enumerator) noexcept
			: m_Enumerator(std::move(enumerator))
		{
		}

		NODISCARD constexpr bool HasReachEnd() const noexcept
		{
			return !m_Enumerator.HasNext();
		}

		constexpr CoroutineIterator& operator++() noexcept
		{
			auto _ = m_Enumerator.MoveNext();
			return *this;
		}

		constexpr CoroutineIterator operator++(int) noexcept
		{
			CoroutineIterator iterator = *this;
			++*this;
			return iterator;
		}

		constexpr T* operator->() noexcept
		{
			return &m_Enumerator.Current();
		}

		constexpr T& operator*() noexcept
		{
			return m_Enumerator.Current();
		}

		NODISCARD constexpr bool operator==(const CoroutineIterator&) const noexcept
		{
			return HasReachEnd();
		}

		NODISCARD constexpr bool operator!=(const CoroutineIterator& other) const noexcept
		{
			return !(*this == other);
		}

	private:
		Enumerator<T> m_Enumerator;
	};

	template <typename T>
	class ConstCoroutineIterator final
	{
	public:
		constexpr ConstCoroutineIterator() noexcept = default;

		constexpr explicit ConstCoroutineIterator(Enumerator<T>&& enumerator) noexcept
			: m_Enumerator(std::move(enumerator))
		{
		}

		NODISCARD constexpr bool HasReachEnd() const noexcept { return !m_Enumerator.HasNext(); }

		constexpr const ConstCoroutineIterator& operator++() const noexcept
		{
			auto _ = m_Enumerator.MoveNext();
			return *this;
		}

		constexpr ConstCoroutineIterator operator++(int) const noexcept
		{
			ConstCoroutineIterator iterator = *this;
			++* this;
			return iterator;
		}

		constexpr const T* operator->() const noexcept { return &m_Enumerator.Current(); }

		constexpr const T& operator*() const noexcept { return m_Enumerator.Current(); }

		NODISCARD constexpr bool operator==(const ConstCoroutineIterator&) const noexcept { return HasReachEnd(); }

		NODISCARD constexpr bool operator!=(const ConstCoroutineIterator& other) const noexcept { return !(*this == other); }

	private:
		Enumerator<T> m_Enumerator;
	};
}
