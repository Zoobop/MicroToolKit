#pragma once

#include "Interfaces/IStruct.h"
#include "Interfaces/IDataHandler.h"

namespace mtk {

	template<typename _Type>
	class Queue : public IExtendable<_Type>, public IDataHandler<_Type>
	{
	public:
		using ValueType = _Type;
		using Iterator = ContainerIterator<Queue<_Type>>;

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

		Queue(std::initializer_list<_Type>&& _initList)
		{
			size_t size = 0;
			for (const auto& _item : _initList)
				size++;

			ReAlloc(size);
			for (const auto& _item : _initList)
				Enqueue(std::move((_Type&&)_item));
		}

		Queue(const Queue<_Type>& _other)
			: m_Data(_other.m_Data), m_Size(_other.m_Size), m_Capacity(_other.m_Capacity)
		{
		}

		Queue(Queue<_Type>&& _other) noexcept
			: m_Capacity(_other.m_Capacity), m_Size(_other.m_Size)
		{
			m_Data = _other.m_Data;

			_other.m_Data = nullptr;
		}

		~Queue()
		{
			Clear();
			Delete(m_Data, m_Capacity * sizeof(_Type));
		}

		void Enqueue(const _Type& _value)
		{
			if (m_Size >= m_Capacity) {
				ReAlloc(m_Capacity + m_Capacity / 2);
			}

			m_Data[m_Size] = _value;
			m_Size++;
		}

		void Enqueue(_Type&& _value)
		{
			if (m_Size >= m_Capacity) {
				ReAlloc(m_Capacity + m_Capacity / 2);
			}

			m_Data[m_Size] = std::move(_value);
			m_Size++;
		}

		_Type Dequeue()
		{
			if (m_Size > 0) {
				_Type item = m_Data[0];
				for (size_t i = 1; i < m_Size; i++) {
					m_Data[i - 1] = std::move(m_Data[i]);
				}
				m_Data[m_Size--].~_Type();
				return item;
			}
			return {};
		}

		_Type Peek() const
		{
			if (m_Size > 0) {
				_Type item = std::move(m_Data[m_Size - 1]);
				return item;
			}
			return {};
		}

		bool Contains(const _Type& _value) const
		{
			for (size_t i = 0; i < m_Size; i++) {
				if (m_Data[i] == _value)
					return true;
			}
			return false;
		}

		bool Contains(_Type&& _value) const
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
				m_Data[i].~_Type();

			m_Size = 0;
		}

		// IExtendable
		virtual void ForEach(const Param<const _Type&>& _param) override
		{
			for (size_t i = 0; i < m_Size; i++) {
				_param(m_Data[i]);
			}
		}

		constexpr virtual const _Type* Data() const override { return m_Data; }
		constexpr virtual _Type* Data() override { return m_Data; }

		// Accessors
		constexpr inline size_t Size() const override { return m_Size; }
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
		const _Type& operator[](size_t _index) const
		{
			if (_index >= m_Capacity) {
				DEBUG_BREAK();
			}
			return m_Data[_index];
		}

		void operator=(const Queue<_Type>& _other)
		{
			m_Data = _other.m_Data;
			m_Size = _other.m_Size;
			m_Capacity = _other.m_Capacity;
		}

		void operator=(Queue<_Type>&& _other)
		{
			m_Data = _other.m_Data;
			m_Size = _other.m_Size;
			m_Capacity = _other.m_Capacity;
			_other.m_Data = nullptr;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Queue<_Type>& _current)
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
			_Type* newBlock = (_Type*)Alloc<_Type>(_capacity);

			if (_capacity < m_Size)
				m_Size = _capacity;

			for (size_t i = 0; i < m_Size; i++) {
				newBlock[i] = std::move(m_Data[i]);
			}

			for (size_t i = 0; i < m_Size; i++)
				m_Data[i].~_Type();

			Delete(m_Data, m_Capacity);
			m_Data = newBlock;
			m_Capacity = _capacity;
		}

	private:
		_Type* m_Data = nullptr;

		size_t m_Size = 0;
		size_t m_Capacity = 0;
	};

}