#pragma once

#include <tuple>

namespace mtk {

	template<typename _Type1, typename _Type2>
	struct Tuple
	{
		using _Tuple = Tuple<_Type1, _Type2>;

		_Type1 _first;
		_Type2 _second;

		Tuple()
		{
		}

		Tuple(const _Type1& _first, const _Type2& _second)
			: _first(_first), _second(_second)
		{
		}

		Tuple(const _Type1& _first, _Type2&& _second) noexcept
			: _first(_first), _second(_second)
		{
		}

		Tuple(_Type1&& _first, const _Type2& _second) noexcept
			: _first(_first), _second(_second)
		{
		}

		Tuple(_Type1&& _first, _Type2&& _second) noexcept
			: _first(_first), _second(_second)
		{
		}

		Tuple(const _Tuple& _other)
			: _first(_other._first), _second(_other._second)
		{
		}

		Tuple(_Tuple&& _other) noexcept
			: _first(_other._first), _second(_other._second)
		{
		}

		Tuple(const std::pair<_Type1, _Type2>& _pair)
			: _first(_pair.first), _second(_pair.second)
		{
		}

		Tuple(std::pair<_Type1, _Type2>&& _pair)
			: _first(_pair.first), _second(_pair.second)
		{
		}

		~Tuple()
		{
		}

		void operator=(const _Tuple& _other)
		{
			_first = _other._first;
			_second = _other._second;
		}

		void operator=(_Tuple&& _other) noexcept
		{
			_first = std::move(_other._first);
			_second = std::move(_other._second);
		}

		friend bool operator==(const _Tuple& _left, const _Tuple& _right)
		{
			return _left._first == _right._first && _left._second == _right._second;
		}

		friend bool operator!=(const _Tuple& _left, const _Tuple& _right)
		{
			return !(_left == _right);
		}

		friend bool operator>=(const _Tuple& _left, const _Tuple& _right)
		{
			return _left._first >= _right._first && _left._second >= _right._second;
		}

		friend bool operator<=(const _Tuple& _left, const _Tuple& _right)
		{
			return _left._first <= _right._first && _left._second <= _right._second;
		}

		friend bool operator>(const _Tuple& _left, const _Tuple& _right)
		{
			return _left._first > _right._first && _left._second > _right._second;
		}

		friend bool operator<(const _Tuple& _left, const _Tuple& _right)
		{
			return _left._first < _right._first && _left._second < _right._second;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const _Tuple& _tuple)
		{
			_stream << "{ " << _tuple._first << " : " << _tuple._second << " }";
			return _stream;
		}

	};
	
	template<typename _Type>
	using Pair = Tuple<_Type, _Type>;
}