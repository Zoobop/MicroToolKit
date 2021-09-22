#pragma once

#include "Interfaces/IStruct.h"
#include "Interfaces/IConvert.h"
#include "Utility/SingleNode.h"

namespace mdt {

	template<typename _Type>
	class LinkedList : public IContainer<_Type>, public IConvert<_Type>
	{
	public:
		using ValueType = _Type;
		using Iterator = ContainerIterator<LinkedList<_Type>>;

	public:
		friend class List<_Type>;
		friend class Queue<_Type>;
		friend class Stack<_Type>;

	public:
		LinkedList() {}

		LinkedList(const _Type& _value)
			: m_Head(new SingleNode<_Type>(_value)), m_Size(1) {}

		LinkedList(const LinkedList<_Type>& _other)
			: m_Head(_other.m_Head), m_Size(_other.m_Size) {}

		LinkedList(_Type&& _value)
			: m_Head(new SingleNode<_Type>(std::move(_value))), m_Size(1) {}

		LinkedList(LinkedList<_Type>&& _other)
			: m_Head(_other.m_Head), m_Size(_other.m_Size) 
		{
			_other.m_Head = nullptr;
		}

		~LinkedList()
		{
			Clear();
			Delete(m_Head, sizeof(SingleNode<_Type>));
		}

		// Utility
		bool Push(const _Type& _value)
		{
			if (!m_Head) {
				m_Head = new SingleNode<_Type>(_value);
				m_Size = 1;
				return true;
			}
			
			SingleNode<_Type>* node = m_Head;
			SingleNode<_Type>* prev = nullptr;
			while (node) {
				prev = node;
				node = node->_next;
			}
			
			node = new SingleNode<_Type>(_value);
			prev->_next = node;

			node = nullptr;
			prev = nullptr;
			free_smem(node);
			free_smem(prev);

			m_Size++;
			return true;
		}

		bool Push(_Type&& _value)
		{
			if (!m_Head) {
				m_Head = new SingleNode<_Type>(std::move((_Type&&)_value));
				m_Size = 1;
				return true;
			}

			SingleNode<_Type>* node = m_Head;
			SingleNode<_Type>* prev = nullptr;
			while (node) {
				prev = node;
				node = node->_next;
			}

			node = new SingleNode<_Type>(std::move((_Type&&)_value));
			prev->_next = node;

			node = nullptr;
			prev = nullptr;
			free_smem(node);
			free_smem(prev);

			m_Size++;
			return true;
		}

		bool PushRange(const IContainer<_Type>& _container)
		{
			for (const auto& item : _container.Data()) {
				Push(item);
			}
			return true;
		}

		bool PushRange(std::initializer_list<_Type>&& _initList)
		{
			for (const auto& item : _initList) {
				Push(std::move((_Type&&)item));
			}
			return true;
		}

		_Type Pop()
		{
			if (m_Size > 0) {
				SingleNode<_Type>* node = m_Head;
				SingleNode<_Type>* prev = nullptr;
				while (node) {
					if (!node->_next) {
						break;
					}
					prev = node;
					node = node->_next;
				}
				prev->_next = nullptr;

				m_Size--;
				return node->_value;
			}
			return _Type();
		}

		bool Remove(const _Type& _value)
		{
			SingleNode<_Type>* node = m_Head;
			SingleNode<_Type>* prev = nullptr;
			while (node) {
				if (node->_value == _value) {
					SingleNode<_Type>* next = node->_next;
					if (prev) prev->_next = next;
					return true;
				}
				prev = node;
				node = node->_next;
			}
			return false;
		}

		bool Remove(_Type&& _value)
		{
			SingleNode<_Type>* node = m_Head;
			SingleNode<_Type>* prev = nullptr;
			while (node) {
				if (node->_value == _value) {
					SingleNode<_Type>* next = node->_next;
					if (prev) prev->_next = next;
					return true;
				}
				prev = node;
				node = node->_next;
			}
			return false;
		}

		bool Contains(const _Type& _value) const
		{
			for (SingleNode<_Type>* node = m_Head; node != nullptr; node = node->_next) {
				if (node->_value == _value)
					return true;
			}
			return false;
		}

		bool Contains(_Type&& _value) const
		{
			for (SingleNode<_Type>* node = m_Head; node != nullptr; node = node->_next) {
				if (node->_value == _value)
					return true;
			}
			return false;
		}

		void Clear()
		{
			SingleNode<_Type>* node = m_Head;
			SingleNode<_Type>* prev = nullptr;
			while (node) {
				if (!node->_next) {
					node->~SingleNode();
					break;
				}
				prev = node;
				node = node->_next;
				prev->~SingleNode();
			}

			m_Head = nullptr;
			m_Size = 0;

			m_Data.Clear();
		}

		// Accessors
		constexpr inline size_t Capacity() const override { return m_Size; }
		constexpr inline const _Type& Head() const { return m_Head->_value; }
		constexpr inline _Type& Head() { return m_Head->_value; }

		// Iterators
		constexpr Iterator begin()
		{
			return Iterator(ToList().m_Data);
		}

		constexpr Iterator end()
		{
			return Iterator(ToList().m_Data + m_Size);
		}

		// IConvert
		virtual List<_Type> ToList() override
		{
			List<_Type> list(m_Size);
			for (SingleNode<_Type>* node = m_Head; node != nullptr; node = node->_next) {
				list.Add(node->_value);
			}
			return list;
		}

		virtual Stack<_Type> ToStack() override
		{
			Stack<_Type> stack(m_Size);
			for (SingleNode<_Type>* node = m_Head; node != nullptr; node = node->_next) {
				stack.Push(node->_value);
			}
			return stack;
		}

		virtual Queue<_Type> ToQueue() override
		{
			Queue<_Type> queue(m_Size);
			for (SingleNode<_Type>* node = m_Head; node != nullptr; node = node->_next) {
				queue.Enqueue(node->_value);
			}
			return queue;
		}

		// IContainer
		virtual void ForEach(const Param<const _Type&>& _param) override
		{
			for (SingleNode<_Type>* node = m_Head; node != nullptr; node = node->_next) {
				_param(node->_value);
			}
		}

		constexpr virtual _Type* Data() const override
		{
			m_Data.Clear();
			Collect();
			return m_Data.Data();
		}

		// Operator Overloads
		friend std::ostream& operator<<(std::ostream& _stream, LinkedList<_Type>& _current)
		{
			_stream << _current.ToList();
			return _stream;
		}
	private:
		void Collect() const
		{
			for (SingleNode<_Type>* node = m_Head; node != nullptr; node = node->_next) {
				m_Data.Add(node->_value);
			}
		}

	private:
		SingleNode<_Type>* m_Head = nullptr;
		size_t m_Size = 0;

		mutable List<_Type> m_Data;
	};

}