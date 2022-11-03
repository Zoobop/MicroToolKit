#pragma once

#include "Core/Function.h"
#include "Core/Memory.h"

namespace mtk {

	template<typename _Type>
	class IExtendable
	{
	public:
		virtual void ForEach(const Action<const _Type&>& _param) = 0;
	};

}