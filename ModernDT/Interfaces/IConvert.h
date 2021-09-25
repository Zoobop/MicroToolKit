#pragma once

#include "DataStruct/List.h"
#include "DataStruct/Stack.h"
#include "DataStruct/Queue.h"

namespace mdt {

	template<typename _Type>
	class IConvert
	{
	public:
		virtual List<_Type> ToList() = 0;
		virtual Stack<_Type> ToStack() = 0;
		virtual Queue<_Type> ToQueue() = 0;
	};

}