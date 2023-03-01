#pragma once

#include "Interfaces/IStruct.h"
#include "Interfaces/IDataHandler.h"
#include "Interfaces/IExtendable.h"

namespace mtk {

	template<typename _Type>
	class Stack : public IExtendable<_Type>, public IDataHandler<_Type>
	{
	public:
		using ValueType = _Type;
		using Iterator = ContainerIterator<Stack<_Type>>;

	public:
		Stack()
		{
			ReAlloc(2);
		}

		Stack(size_t _size)
			: m_Capacity(_size)
		{
			ReAlloc(m_Capacity);
		}

		Stack(std::initializer_list<_Type>&& _initList)
		{
			size_t size = 0;
			for (const auto& _item : _initList)
				size++;

			ReAlloc(size);
			for (const auto& _item : _initList)
				Push(std::move((_Type&&)_item));
		}

		Stack(const Stack<_Type>& _other)
			: m_Data(_other.m_Data), m_Size(_other.m_Size), m_Capacity(_other.m_Capacity)
		{
		}

		Stack(Stack<_Type>&& _other) noexcept
			: m_Capacity(_other.m_Capacity), m_Size(_other.m_Size)
		{
			m_Data = _other.m_Data;

			_other.m_Data = nullptr;
		}

		~Stack()
		{
			Clear();
			Delete(m_Data, m_Capacity * sizeof(_Type));
		}

		bool Push(const _Type& _value)
		{
			if (m_Size >= m_Capacity) {
				ReAlloc(m_Capacity + m_Capacity / 2);
			}

			m_Data[m_Size] = _value;
			m_Size++;

			return true;
		}

		bool Push(_Type&& _value)
		{
			if (m_Size >= m_Capacity) {
				ReAlloc(m_Capacity + m_Capacity / 2);
			}

			m_Data[m_Size] = std::move(_value);
			m_Size++;

			return true;
		}

		template<typename ... _Args>
		_Type& Emplace(_Args&&... args)
		{
			if (m_Size >= m_Capacity) {
				ReAlloc(m_Capacity + m_Capacity / 2);
			}

			new(&m_Data[m_Size]) _Type(std::forward<_Args>(args)...);
			return m_Data[m_Size++];
		}

		bool PushRange(const IExtendable<_Type>& _container)
		{
			for (const auto& item : _container.Data()) {
				Push(item);
			}
			return true;
		}

		bool PushRange(std::initializer_list<_Type>&& _initList)
		{
			for (const auto& item : _initList) {
				Push(std::move(item));
			}
			return true;
		}

		_Type Pop()
		{
			if (m_Size > 0) {
				_Type item = std::move(m_Data[m_Size - 1]);
				m_Data[m_Size - 1].~_Type();
				m_Size--;
				return item;
			}
			return {};
		}

		bool TryPop(_Type& _out)
		{
			if (m_Size > 0) {
				_out = std::move(m_Data[m_Size - 1]);
				m_Data[m_Size - 1].~_Type();
				m_Size--;
				return true;
			}
			return false;
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
		virtual void ForEach(const Action<const _Type&>& _param) override
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

		void operator=(const Stack<_Type>& _other)
		{
			m_Data = _other.m_Data;
			m_Size = _other.m_Size;
			m_Capacity = _other.m_Capacity;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Stack<_Type>& _current)
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