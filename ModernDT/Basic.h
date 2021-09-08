#pragma once

#define free_smem(x)		x = nullptr; delete x;
#define free_amem(x)		x = nullptr; delete[] x;


namespace mdt {

	typedef void(*Void)();
	
	template<typename ... Args>
	using Param = void(*)(Args...);

	template<typename RType>
	using Func = RType(*)();

	template<typename RType, typename ... Args>
	using Dynamic = RType(*)(Args...);


}