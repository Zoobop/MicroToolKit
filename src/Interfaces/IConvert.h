#pragma once

#include "Collections/List.h"
#include "Collections/Queue.h"
#include "Collections/Stack.h"

namespace mtk {

	template<typename _Type>
	class IConvert
	{
	public:
		virtual List<_Type> ToList() = 0;
		virtual Stack<_Type> ToStack() = 0;
		virtual Queue<_Type> ToQueue() = 0;
	};

}