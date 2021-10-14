#pragma once

#include <memory>

#define free_smem(x)		delete x
#define free_amem(x)		delete[] x

namespace mtk {

	template<typename _Type>
	using Scope = std::unique_ptr<_Type>;

	template<typename _Type, typename ... _Args>
	constexpr Scope<_Type> CreateScope(_Args&& ... args)
	{
		return std::make_unique<_Type>(std::forward<_Args>(args)...);
	}

	template<typename _Type>
	using Ref = std::shared_ptr<_Type>;

	template<typename _Type, typename ... _Args>
	constexpr Ref<_Type> CreateRef(_Args&& ... args)
	{
		return std::make_shared<_Type>(std::forward<_Args>(args)...);
	}

	template<typename _Type>
	void* Alloc(size_t _size)
	{
		return ::operator new(_size * sizeof(_Type));
	}

	template<typename _Type>
	void Delete(_Type* _block, size_t _size)
	{
		::operator delete(_block, _size * sizeof(_Type));
	}

}