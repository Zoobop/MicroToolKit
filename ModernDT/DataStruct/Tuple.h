#pragma once

#include <tuple>

namespace mdt {

	template<typename T1, typename T2>
	class Tuple
	{
	public:
		using ValueType = Tuple<T1, T2>;

	public:
		Tuple()
		{
		}

		Tuple(const T1& _first, const T2& _second)
			: m_First(_first), m_Second(_second)
		{
		}

		~Tuple()
		{
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Tuple<T1, T2>& _tuple)
		{
			_stream << "{ " << _tuple.m_First << " : " << _tuple.m_Second << " }";
			return _stream;
		}

		void operator=(const Tuple<T1, T2>& _other)
		{
			m_First = _other.m_First;
			m_Second = _other.m_Second;
		}

		const T1& First() const { return m_First; }
		const T2& Second() const { return m_Second; }
		T1& First() { return m_First; }
		T2& Second() { return m_Second; }

		inline void SetFirst(const T1& _first) { m_First = _first; }
		inline void SetSecond(const T2& _second) { m_Second = _second; }

	private:
		T1 m_First;
		T2 m_Second;
	};
	
	template<typename T>
	using Pair = Tuple<T, T>;
}