#pragma once

#include <functional>

namespace Micro
{
#if 0

	template<typename TReturn, typename ... Args>
	class Function final
	{
	public:
		constexpr Function() noexcept = default;
		constexpr Function(const Function&) noexcept = default;
		constexpr Function(Function&&) noexcept = default;
		constexpr ~Function() noexcept = default;

		constexpr Function(TReturn(*lambda)(Args...)) noexcept
			: m_Ptr(lambda)
		{
		}

		TReturn operator()(Args... args)
		{
			return m_Ptr(std::forward<Args>(std::move(args))...);
		}

		TReturn operator()(Args... args) const
		{
			return m_Ptr(std::forward<Args>(std::move(args))...);
		}

		constexpr Function& operator=(const Function&) noexcept = default;
		constexpr Function& operator=(Function&&) noexcept = default;
		
	private:
		TReturn (*m_Ptr)(Args...);
	};

#endif

	using VoidCall = std::function<void()>;

	template <typename... Args>
	using Action = std::function<void(const Args&...)>;

	template <typename... Args>
	using Predicate = std::function<bool(const Args&...)>;

	template <typename TReturn, typename... Args>
	using Func = std::function<TReturn(const Args&...)>;
}
