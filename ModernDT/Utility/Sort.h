#pragma once
#include "mtkpch.h"
#include "Interfaces/IDataHandler.h"

namespace mtk {

	template<typename _Type>
	bool GreaterThan(const _Type& _first, const _Type& _second)
	{
		return _first > _second;
	}

	template<typename _Type>
	bool GreaterThanEqual(const _Type& _first, const _Type& _second)
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
	void Sort(const IDataHandler<_Type>& _dataHandler, 
		const Dynamic<bool, const _Type&, const _Type&>& _predicate = GreaterThan<_Type>)
	{
		_Type* data = const_cast<_Type*>(_dataHandler.Data());
		size_t size = _dataHandler.Size();

		PROFILER();
		for (size_t i = 0; i < size; i++) {
			for (size_t j = i + 1; j < size; j++) {
				if (_predicate(data[i], data[j])) {
					_Type temp = std::move(data[i]);
					data[i] = std::move(data[j]);
					data[j] = std::move(temp);
				}
			}
		}
	}

	template<typename _Type>
	void RSort(const IDataHandler<_Type>& _dataHandler, 
		const Dynamic<bool, const _Type&, const _Type&>& _predicate = GreaterThan<_Type>)
	{
		_Type* data = const_cast<_Type*>(_dataHandler.Data());
		size_t size = _dataHandler.Size();

		PROFILER();
		for (int32_t i = size - 1; i > 0; i--) {
			for (int32_t j = i - 1; j >= 0; j--) {
				if (_predicate(data[i], data[j])) {
					_Type temp = std::move(data[i]);
					data[i] = std::move(data[j]);
					data[j] = std::move(temp);
				}
			}
		}
	}

}