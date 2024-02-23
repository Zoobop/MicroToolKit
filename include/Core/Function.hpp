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

		constexpr Function(TReturn(*func)(Args...)) noexcept
			: m_Ptr(func)
		{
		}

		constexpr TReturn operator()(Args... args) noexcept
		{
			return m_Ptr(std::forward<Args>(args)...);
		}

		constexpr TReturn operator()(Args... args) const noexcept
		{
			return m_Ptr(std::forward<Args>(args)...);
		}

		constexpr Function& operator=(const Function&) noexcept = default;
		constexpr Function& operator=(Function&&) noexcept = default;
		
	private:
		TReturn (*m_Ptr)(Args...);
	};

#endif

	using Action = std::function<void()>;

	template <typename... Args>
	using Predicate = std::function<bool(const Args&...)>;

	template <typename TReturn, typename... Args>
	using Func = std::function<TReturn(const Args&...)>;
}
