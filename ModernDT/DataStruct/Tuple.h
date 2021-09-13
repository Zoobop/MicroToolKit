#pragma once

#include <tuple>

namespace mdt {

	template<typename _T1, typename _T2>
	class Tuple
	{
	public:
		Tuple()
		{
		}

		Tuple(const _T1& _first, const _T2& _second)
			: m_First(_first), m_Second(_second)
		{
		}

		Tuple(const Tuple<_T1, _T2>& _other)
			: m_First(_other.m_First), m_Second(_other.m_Second)
		{
		}

		Tuple(Tuple<_T1, _T2>&& _other) noexcept
			: m_First(std::move(_other.m_First)), m_Second(std::move(_other.m_Second))
		{
		}

		Tuple(_T1&& _first, _T2&& _second) noexcept
			: m_First(std::move(_first)), m_Second(std::move(_second))
		{
		}

		~Tuple()
		{
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Tuple<_T1, _T2>& _tuple)
		{
			_stream << "{ " << _tuple.m_First << " : " << _tuple.m_Second << " }";
			return _stream;
		}

		void operator=(const Tuple<_T1, _T2>& _other)
		{
			m_First = _other.m_First;
			m_Second = _other.m_Second;
		}

		void operator=(Tuple<_T1, _T2>&& _other) noexcept
		{
			m_First = std::move(_other.m_First);
			m_Second = std::move(_other.m_Second);
		}

		const _T1& First() const { return m_First; }
		const _T2& Second() const { return m_Second; }
		_T1& First() { return m_First; }
		_T2& Second() { return m_Second; }

		inline void SetFirst(const _T1& _first) { m_First = _first; }
		inline void SetSecond(const _T2& _second) { m_Second = _second; }

	private:
		_T1 m_First;
		_T2 m_Second;
	};
	
	template<typename _Type>
	using Pair = Tuple<_Type, _Type>;
}