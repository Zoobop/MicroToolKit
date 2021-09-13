#pragma once

#include "Core/Function.h"
#include "Core/Memory.h"

namespace mdt {

	template<typename T>
	class IContainer
	{
	public:
		virtual void ForEach(const Param<const T&>& _param) = 0;

		virtual constexpr inline size_t Capacity() const = 0;
		virtual constexpr T* Data() const = 0;
	};

}