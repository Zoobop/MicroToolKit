#pragma once

#include "Basic.h"

namespace mdt {

	template<typename T>
	__interface IContainer
	{
		void ForEach(Param<const T&> _param);

		constexpr inline size_t Capacity() const;
		constexpr T* Data() const;
	};

}