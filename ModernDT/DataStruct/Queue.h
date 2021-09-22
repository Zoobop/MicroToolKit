#pragma once

#include "Interfaces/IStruct.h"

namespace mdt {

	template<typename T>
	class Queue : public IContainer<T>
	{
	public:
		using ValueType = T;
		using Iterator = ContainerIterator<Queue<T>>;

	public:
		Queue()
		{
			ReAlloc(2);
		}

		Queue(size_t _size)
			: m_Capacity(_size)
		{
			ReAlloc(m_Capacity);
		}

		Queue(std::initializer_list<T>&& _initList)
		{
			size_t size = 0;
			for (const auto& _item : _initList)
				size++;

			ReAlloc(size);
			for (const auto& _item : _initList)
				Enqueue(std::move((T&&)_item));
		}

		Queue(const Queue<T>& _other)
			: m_Data(_other.m_Data), m_Size(_other.m_Size), m_Capacity(_other.m_Capacity)
		{
		}

		Queue(Queue<T>&& _other) noexcept
			: m_Capacity(_other.m_Capacity), m_Size(_other.m_Size)
		{
			m_Data = _other.m_Data;

			_other.m_Data = nullptr;
		}

		~Queue()
		{
			Clear();
			Delete(m_Data, m_Capacity * sizeof(T));
		}

		void Enqueue(const T& _value)
		{
			if (m_Size >= m_Capacity) {
				ReAlloc(m_Capacity + m_Capacity / 2);
			}

			m_Data[m_Size] = _value;
			m_Size++;
		}

		void Enqueue(T&& _value)
		{
			if (m_Size >= m_Capacity) {
				ReAlloc(m_Capacity + m_Capacity / 2);
			}

			m_Data[m_Size] = std::move(_value);
			m_Size++;
		}

		T Dequeue()
		{
			T item;
			if (m_Size > 0) {
				item = m_Data[0];
				for (size_t i = 1; i < m_Size; i++) {
					m_Data[i - 1] = std::move(m_Data[i]);
				}
				m_Data[m_Size--].~T();
			}
			return item;
		}

		T Peek() const
		{
			T item;
			if (m_Size > 0) {
				item = std::move(m_Data[m_Size - 1]);
				return item;
			}
			return item;
		}

		bool Contains(const T& _value) const
		{
			for (size_t i = 0; i < m_Size; i++) {
				if (m_Data[i] == _value)
					return true;
			}
			return false;
		}

		bool Contains(T&& _value) const
		{
			for (size_t i = 0; i < m_Size; i++) {
				if (m_Data[i] == _value)
					return true;
			}
			return false;
		}

		void Clear()
		{
			for (size_t i = 0; i < m_Size; i++)
				m_Data[i].~T();

			m_Size = 0;
		}

		// IContainer
		virtual void ForEach(const Param<const T&>& _param) override
		{
			for (size_t i = 0; i < m_Size; i++) {
				_param(m_Data[i]);
			}
		}

		constexpr virtual T* Data() const override { return m_Data; }

		// Accessors
		constexpr inline size_t Size() const { return m_Size; }
		constexpr inline size_t Capacity() const override { return m_Capacity; }

		// Iterator
		constexpr Iterator begin()
		{
			return Iterator(m_Data);
		}

		constexpr Iterator end()
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

		void operator=(const Queue<T>& _other)
		{
			m_Data = _other.m_Data;
			m_Size = _other.m_Size;
			m_Capacity = _other.m_Capacity;
		}

		void operator=(Queue<T>&& _other)
		{
			m_Data = _other.m_Data;
			m_Size = _other.m_Size;
			m_Capacity = _other.m_Capacity;
			_other.m_Data = nullptr;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Queue<T>& _current)
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
			T* newBlock = (T*)Alloc<T>(_capacity);

			if (_capacity < m_Size)
				m_Size = _capacity;

			for (size_t i = 0; i < m_Size; i++) {
				newBlock[i] = std::move(m_Data[i]);
			}

			for (size_t i = 0; i < m_Size; i++)
				m_Data[i].~T();

			Delete(m_Data, m_Capacity);
			m_Data = newBlock;
			m_Capacity = _capacity;
		}

	private:
		T* m_Data = nullptr;

		size_t m_Size = 0;
		size_t m_Capacity = 0;
	};

}