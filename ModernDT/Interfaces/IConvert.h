#pragma once

#include "Structures/List.h"
#include "Structures/Stack.h"
#include "Structures/Queue.h"

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