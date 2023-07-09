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

		T Current;
		std::exception_ptr Exception;

		NODISCARD constexpr auto yield_value(const std::convertible_to<T> auto& value) noexcept
		{
			Current = value;
			return std::suspend_always();
		}

		NODISCARD constexpr auto yield_value(std::convertible_to<T> auto&& value) noexcept
		{
			Current = std::forward<T>(value);
			return std::suspend_always();
		}

		constexpr void return_void() noexcept
		{
		}

		constexpr void unhandled_exception() noexcept
		{
			Exception = std::current_exception();
		}

		// Accessors
		NODISCARD constexpr auto get_return_object() noexcept
		{
			return static_cast<TEnumerator>(Handle::from_promise(*this));
		}

		NODISCARD constexpr auto initial_suspend() noexcept { return std::suspend_always(); }
		NODISCARD constexpr auto final_suspend() noexcept { return std::suspend_always(); }
	};

	template <typename T>
	class Enumerator final
	{
	public:
		using promise_type = PromiseType<T, Enumerator>;
		using Handle = std::coroutine_handle<promise_type>;

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

		~Enumerator() noexcept
		{
			if (m_Handle)
				m_Handle.destroy();
		}

		// Utility
		NODISCARD constexpr T& Current() noexcept
		{
			TryFill();
			m_IsFull = false;
			return m_Handle.promise().Current;
		}

		NODISCARD constexpr bool HasNext() const noexcept { return !m_Handle.done(); }
		NODISCARD bool MoveNext() { return operator bool(); }

		// Operator Overloads
		explicit constexpr operator Handle() const noexcept { return m_Handle; }

		explicit operator bool()
		{
			TryFill();
			return !m_Handle.done();
		}

		constexpr T& operator=(const Enumerator&) noexcept = delete;
		constexpr T& operator=(Enumerator&&) noexcept = delete;

	private:
		Handle m_Handle;
		bool m_IsFull = false;

		void TryFill()
		{
			if (!m_IsFull)
			{
				m_Handle();
				if (m_Handle.promise().Exception)
					std::rethrow_exception(m_Handle.promise().Exception);

				m_IsFull = true;
			}
		}
	};

	template <typename T>
	class Iterator final
	{
	public:
		using Enumerator = Enumerator<T>;

		friend Enumerator;

		constexpr Iterator() noexcept = default;

		constexpr explicit Iterator(Enumerator&& enumerator) noexcept
			: m_Enumerator(std::move(enumerator))
		{
		}

		NODISCARD constexpr bool HasReachEnd() const noexcept
		{
			return !m_Enumerator.HasNext();
		}

		constexpr Iterator& operator++() noexcept
		{
			auto _ = m_Enumerator.MoveNext();
			return *this;
		}

		constexpr Iterator operator++(int) noexcept
		{
			Iterator iterator = *this;
			++*this;
			return *this;
		}

		constexpr Iterator& operator++() const noexcept
		{
			auto _ = m_Enumerator.MoveNext();
			return *this;
		}

		constexpr Iterator operator++(int) const noexcept
		{
			Iterator iterator = *this;
			++*this;
			return *this;
		}

		constexpr T* operator->() noexcept
		{
			return &m_Enumerator.Current();
		}

		constexpr T& operator*() noexcept
		{
			return m_Enumerator.Current();
		}

		NODISCARD constexpr bool operator==(const Iterator&) const noexcept
		{
			return HasReachEnd();
		}

		NODISCARD constexpr bool operator!=(const Iterator& other) const noexcept
		{
			return !(*this == other);
		}

	private:
		Enumerator m_Enumerator;
	};
}
