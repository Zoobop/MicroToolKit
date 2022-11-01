#pragma once

#include <functional>

namespace mtk {

	using Void = std::function<void()>;
	
	template<typename ... _Args>
	using Param = std::function<void(_Args...)>;

	template<typename _RType>
	using Func = std::function<_RType()>;

	template<typename _RType, typename ... _Args>
	using Dynamic = std::function<_RType(_Args...)>;

}