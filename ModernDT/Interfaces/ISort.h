#pragma once

#include "Interfaces/IStruct.h"

namespace mdt {

	template<typename _Type>
	bool GreatorThan(const _Type& _first, const _Type& _second)
	{
		return _first > _second;
	}

	template<typename _Type>
	bool GreatorThanEqual(const _Type& _first, const _Type& _second)
	{
		return _first >= _second;
	}

	template<typename _Type>
	bool LessThan(const _Type& _first, const _Type& _second)
	{
		return _first < _second;
	}

	template<typename _Type>
	bool LessThanEqual(const _Type& _first, const _Type& _second)
	{
		return _first <= _second;
	}

	template<typename _Type>
	class ISort
	{
	public:
		virtual void Sort(const Dynamic<bool, const _Type&, const _Type&> _predicate = GreatorThan<_Type>) = 0;
		virtual void RSort(const Dynamic<bool, const _Type&, const _Type&> _predicate = GreatorThan<_Type>) = 0;
	};

}