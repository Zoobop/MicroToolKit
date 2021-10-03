#pragma once

#include <tuple>

namespace mdt {

	template<typename _Type1, typename _Type2>
	struct Tuple
	{
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

		Tuple(const Tuple<_Type1, _Type2>& _other)
			: _first(_other._first), _second(_other._second)
		{
		}

		Tuple(Tuple<_Type1, _Type2>&& _other) noexcept
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

		friend std::ostream& operator<<(std::ostream& _stream, const Tuple<_Type1, _Type2>& _tuple)
		{
			_stream << "{ " << _tuple._first << " : " << _tuple._second << " }";
			return _stream;
		}

		void operator=(const Tuple<_Type1, _Type2>& _other)
		{
			_first = _other._first;
			_second = _other._second;
		}

		void operator=(Tuple<_Type1, _Type2>&& _other) noexcept
		{
			_first = std::move(_other._first);
			_second = std::move(_other._second);
		}
	};
	
	template<typename _Type>
	using Pair = Tuple<_Type, _Type>;
}