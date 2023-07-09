#pragma once

#include "Core/Function.h"
#include "Core/Memory.h"

namespace Micro {

	template<typename _Type>
	class IExtendable
	{
	public:
		virtual void ForEach(const Action<const _Type&>& _param) = 0;
	};

}