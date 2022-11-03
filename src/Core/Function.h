#pragma once

#include <functional>

namespace mtk {

#if 0
	
	template<typename>
	class Function;

	template<typename _RType, typename ... _Args>
	class Function<_RType(_Args...)>
	{
	public:
		Function() = default;
		Function(const Function& _func)
			: m_Ptr(_func.m_Ptr)
		{
		}

		Function(Function&& _func) noexcept
			: m_Ptr(std::move(_func.m_Ptr))
		{
		}

		~Function() = default;

		_RType operator()(_Args... _args)
		{
			return m_Ptr(std::forward<_Args>(std::move(_args))...);
		}

		_RType operator()(_Args... _args) const
		{
			return m_Ptr(std::forward<_Args>(std::move(_args))...);
		}

		Function& operator=(const Function& _func)
		{
			std::swap(m_Ptr, _func.m_Ptr);
			return *this;
		}

		Function& operator=(Function&& _func) noexcept
		{
			std::swap(m_Ptr, _func.m_Ptr);
			return *this;
		}
		
	private:
		_RType (*m_Ptr)(_Args...);
	};

#endif
	
	using VoidCall = std::function<void()>;
	
	template<typename ... _Args>
	using Action = std::function<void(_Args...)>;

	template<typename ... _Args>
	using Predicate = std::function<bool(_Args...)>;

	template<typename _RType, typename ... _Args>
	using Func = std::function<_RType(_Args...)>;
}