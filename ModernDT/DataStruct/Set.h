#pragma once

#include "Container.h"

namespace mdt {

	template<typename T>
	class Set : public DataContainer<T>
	{
	public:
		using Iterator = DataContainer<T>::Iterator;

	public:
		Set()
		{
			ReAlloc(2);
		}

		Set(size_t size)
			: m_Capacity(size)
		{
			ReAlloc(m_Capacity);
		}

		Set(const Set<T>& other)
		{
		}

		Set(Set<T>&& other)
		{
		}

		~Set()
		{
			delete[] m_Data;
		}

		// Override Methods
		virtual bool Add(const T& _value) override
		{
			if (!Contains(_value)) {
				if (__super::m_Size <= m_Capacity) {
					ReAlloc(m_Capacity + m_Capacity / 2);
				}

				m_Data[__super::m_Size] = _value;
				__super::m_Size++;
				return true;
			}
			return false;
		}

		virtual bool Remove(const T& _value) override
		{
			if (__super::m_Size > 0) {
				size_t currentIndex = 0;
				for (currentIndex; currentIndex < __super::m_Size; currentIndex++) {
					if (m_Data[currentIndex] == _value) {
						currentIndex++;
						break;
					}
				}

				for (currentIndex; currentIndex < __super::m_Size; currentIndex++) {
					m_Data[currentIndex - 1] = m_Data[currentIndex];
				}

				__super::m_Size--;
				return true;
			}
			return false;
		}

		virtual bool RemoveAt(size_t _index) override
		{
			if (_index > 0 && _index < __super::m_Size) {
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
			m_Data = nullptr;
			__super::m_Size = 0;
		}

		// Accessors
		constexpr inline size_t Capacity() const { return m_Capacity; }

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
		const T& operator[](size_t _index) const
		{
			if (_index >= m_Capacity) {
				__debugbreak();
			}
			return m_Data[_index];
		}

		T& operator[](size_t _index)
		{
			if (_index >= m_Capacity) {
				__debugbreak();
			}
			return m_Data[_index];
		}

		void operator=(const Set<T>& _other)
		{
			m_Data = _other.m_Data;
			__super::m_Size = _other.__super::m_Size;
			m_Capacity = _other.m_Capacity;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Set<T>& _current)
		{
			_stream << "[ ";
			for (size_t i = 0; i < _current.m_Size; i++) {
				_stream << _current.m_Data[i];
				if (i != _current.m_Size - 1)
					_stream << ", ";
			}

			_stream << " ]" << std::endl;
			return _stream;
		}

	private:
		void ReAlloc(size_t _capacity)
		{
			T* newBlock = new T[_capacity];

			if (_capacity < __super::m_Size)
				__super::m_Size = _capacity;

			for (size_t i = 0; i < __super::m_Size; i++) {
				newBlock[i] = m_Data[i];
			}

			delete[] m_Data;
			m_Data = newBlock;
			m_Capacity = _capacity;
		}

	private:
		T* m_Data = nullptr;

		size_t m_Capacity = 0;
	};


}