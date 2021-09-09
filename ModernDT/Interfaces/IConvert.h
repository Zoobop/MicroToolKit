#pragma once

#include "DataStruct/List.h"
#include "DataStruct/Set.h"
#include "DataStruct/Stack.h"
#include "DataStruct/Queue.h"

namespace mdt {

	template<typename T>
	__interface IConvert
	{
		List<T> ToList();
		Set<T> ToSet();
		Stack<T> ToStack();
		Queue<T> ToQueue();
	};

}