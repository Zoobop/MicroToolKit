#pragma once

#include "Core/Function.h"
#include "Core/Memory.h"

namespace mdt {

	template<typename _Type>
	class IContainer
	{
	public:
		virtual void ForEach(const Param<const _Type&>& _param) = 0;

		virtual constexpr inline size_t Capacity() const = 0;
		virtual constexpr _Type* Data() const = 0;
	};

}