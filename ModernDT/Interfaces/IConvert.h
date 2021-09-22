#pragma once

#include "DataStruct/List.h"
#include "DataStruct/Stack.h"
#include "DataStruct/Queue.h"

namespace mdt {

	template<typename T>
	class IConvert
	{
	public:
		virtual List<T> ToList() = 0;
		virtual Stack<T> ToStack() = 0;
		virtual Queue<T> ToQueue() = 0;
	};

}