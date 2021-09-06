#pragma once

#include "Container.h"

namespace mdt {

	template<typename T>
	class Stack
	{
	public:
		using ValueType = T;
		using Iterator = ContainerIterator<Stack<T>>;

	public:
		Stack()
		{
			ReAlloc(2);
		}

		Stack(size_t size)
			: m_Capacity(size)
		{
			ReAlloc(m_Capacity);
		}

		Stack(const Stack<T>& other)
		{

		}

		Stack(Stack<T>&& other)
		{
		}

		~Stack()
		{
			delete[] m_Data;
		}

		bool Push(const T& _value)
		{
			if (m_Size <= m_Capacity) {
				ReAlloc(m_Capacity + m_Capacity / 2);
			}

			m_Data[m_Size] = _value;
			m_Size++;

			return true;
		}

		T& Pop()
		{
			T item = m_Data[m_Size - 1];
			m_Size--;
			return item;
		}

		T& Peek() const
		{
			return m_Data[m_Size - 1];
		}

		bool Contains(const T& _value) const
		{
			for (size_t i = 0; i < m_Size; i++) {
				if (m_Data[i] == _value)
					return true;
			}
			return false;
		}

		void Clear()
		{
			m_Data = nullptr;
			m_Size = 0;
		}

		// Accessors
		constexpr inline size_t Size() const { return m_Size; }
		constexpr inline size_t Capacity() const { return m_Capacity; }

		// Iterator
		constexpr virtual Iterator begin()
		{
			return Iterator(m_Data);
		}

		constexpr virtual Iterator end()
		{
			return Iterator(m_Data + m_Size);
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

		void operator=(const Stack<T>& _other)
		{
			m_Data = _other.m_Data;
			m_Size = _other.m_Size;
			m_Capacity = _other.m_Capacity;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Stack<T>& _current)
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

			if (_capacity < m_Size)
				m_Size = _capacity;

			for (size_t i = 0; i < m_Size; i++) {
				newBlock[i] = m_Data[i];
			}

			delete[] m_Data;
			m_Data = newBlock;
			m_Capacity = _capacity;
		}

	private:
		T* m_Data = nullptr;

		size_t m_Size = 0;
		size_t m_Capacity = 0;
	};

}