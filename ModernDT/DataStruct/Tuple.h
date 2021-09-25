#pragma once

#include <tuple>

namespace mdt {

	template<typename _Type1, typename _Type2>
	class Tuple
	{
	public:
		Tuple()
		{
		}

		Tuple(const _Type1& _first, const _Type2& _second)
			: m_First(_first), m_Second(_second)
		{
		}

		Tuple(const _Type1& _first, _Type2&& _second) noexcept
			: m_First(_first), m_Second(_second)
		{
		}

		Tuple(_Type1&& _first, const _Type2& _second) noexcept
			: m_First(_first), m_Second(_second)
		{
		}

		Tuple(_Type1&& _first, _Type2&& _second) noexcept
			: m_First(_first), m_Second(_second)
		{
		}

		Tuple(const Tuple<_Type1, _Type2>& _other)
			: m_First(_other.m_First), m_Second(_other.m_Second)
		{
		}

		Tuple(Tuple<_Type1, _Type2>&& _other) noexcept
			: m_First(_other.m_First), m_Second(_other.m_Second)
		{
		}

		~Tuple()
		{
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Tuple<_Type1, _Type2>& _tuple)
		{
			_stream << "{ " << _tuple.m_First << " : " << _tuple.m_Second << " }";
			return _stream;
		}

		void operator=(const Tuple<_Type1, _Type2>& _other)
		{
			m_First = _other.m_First;
			m_Second = _other.m_Second;
		}

		void operator=(Tuple<_Type1, _Type2>&& _other) noexcept
		{
			m_First = std::move(_other.m_First);
			m_Second = std::move(_other.m_Second);
		}

		const _Type1& First() const { return m_First; }
		const _Type2& Second() const { return m_Second; }
		_Type1& First() { return m_First; }
		_Type2& Second() { return m_Second; }

		inline void SetFirst(const _Type1& _first) { m_First = _first; }
		inline void SetSecond(const _Type2& _second) { m_Second = _second; }

	private:
		_Type1 m_First;
		_Type2 m_Second;
	};
	
	template<typename _Type>
	using Pair = Tuple<_Type, _Type>;
}