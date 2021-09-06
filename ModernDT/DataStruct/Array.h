#pragma once

#include "Container.h"

namespace mdt {

	template<typename T, size_t S>
	class Array : public DataContainer<T>
	{
	public:
		using Iterator = ContainerIterator<DataContainer<T>>;

	public:
		Array()
		{
			Clear();
			__super::m_Size = S;
		}

		// Utility
		virtual bool Add(const T& _value) override
		{
			if (__super::m_Size < m_Capacity) {
				for (size_t i = 0; i < __super::m_Size; i++) {
					if (m_Data[i] == T()) {
						m_Data[i] = _value;
						__super::m_Size++;
						return true;
					}
				}
			}
			return false;
		}

		virtual bool Remove(const T& _value) override
		{
			if (__super::m_Size > 0) {
				for (size_t i = 0; i < __super::m_Size; i++) {
					if (m_Data[i] == _value) {
						m_Data[i] = T();
						__super::m_Size--;
						return true;
					}
				}
			}
			return false;
		}

		virtual bool RemoveAt(size_t _index) override
		{
			if (_index >= 0 && _index < m_Capacity) {
				return Remove(m_Data[_index]);
			}
			return false;
		}

		virtual bool Contains(const T& _value) const override
		{
			for (size_t i = 0; i < __super::m_Size; i++) {
				if (m_Data[i] == _value)
					return true;
			}
			return false;
		}

		virtual void Clear() override
		{
			for (size_t i = 0; i < m_Capacity; i++) {
				m_Data[i] = T();
			}
		}

		// Accessors

		// Iterator
		constexpr virtual Iterator begin() override
		{
			return Iterator(m_Data);
		}
		constexpr virtual Iterator end() override
		{
			return Iterator(m_Data + __super::m_Size);
		}

		// Operator Overloads
		T& operator[](size_t _index)
		{
			if (_index >= m_Capacity) {
				__debugbreak();
			}
			return m_Data[_index];
		}

		const T& operator[](size_t _index) const
		{
			if (_index >= m_Capacity) {
				__debugbreak();
			}
			return m_Data[_index];
		}

		void operator=(const Array<T, S>& _other)
		{
			m_Data = _other.m_Data;
			__super::m_Size = _other.m_Size;
			m_Capacity = _other.m_Capacity;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Array<T, S>& _current)
		{
			_stream << "[ ";
			for (size_t i = 0; i < _current.m_Capacity; i++) {
				_stream << _current.m_Data[i];
				if (i != _current.m_Capacity - 1)
					_stream << ", ";
			}

			_stream << " ]" << std::endl;
			return _stream;
		}

	private:
		T m_Data[S];

		size_t m_Capacity = S;
	};

}