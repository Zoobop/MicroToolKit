#pragma once

#define free_smem(x)		x = nullptr; delete x;
#define free_amem(x)		x = nullptr; delete[] x;

#include <functional>

namespace mdt {

	typedef std::function<void()> Void;
	
	template<typename ... Args>
	using Param = std::function<void(Args...)>;

	template<typename RType>
	using Func = std::function<RType()>;

	template<typename RType, typename ... Args>
	using Dynamic = std::function<RType(Args...)>;


}