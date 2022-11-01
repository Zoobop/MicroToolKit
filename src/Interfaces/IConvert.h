#pragma once

#include "Common/List.h"
#include "Common/Queue.h"
#include "Common/Stack.h"

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