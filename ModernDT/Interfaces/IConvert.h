#pragma once

#include "DataStruct/List.h"
#include "DataStruct/Set.h"
#include "DataStruct/Stack.h"
#include "DataStruct/Queue.h"

namespace mdt {

	template<typename T>
	__interface IConvert
	{
		List<T> ToList() const;
		Set<T> ToSet() const;
		Stack<T> ToStack() const;
		Queue<T> ToQueue() const;
	};

}