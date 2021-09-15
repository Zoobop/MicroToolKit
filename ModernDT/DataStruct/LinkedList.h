#pragma once

#include "Container.h"
#include "Interfaces/IConvert.h"

namespace mdt {

	template<typename T>
	class LinkedList : public IContainer<T>, public IConvert<T>
	{
	private:
		template<typename T>
		struct Node
		{
			T _value;
			Node<T>* _next = nullptr;

			Node(const T& _value)
				: _value(_value) {}

			Node(T&& _value)
				: _value(std::move(_value)) {}

			~Node()
			{
				_next = nullptr;
				free_smem(_next);
			}
		};

	public:
		using ValueType = T;
		using Iterator = ContainerIterator<LinkedList<T>>;

	public:
		friend class List<T>;
		friend class Set<T>;
		friend class Queue<T>;
		friend class Stack<T>;

	public:
		LinkedList() {}

		LinkedList(const T& _value)
			: m_Head(new Node<T>(_value)), m_Size(1) {}

		LinkedList(const LinkedList<T>& _other)
			: m_Head(_other.m_Head), m_Size(_other.m_Size) {}

		LinkedList(T&& _value)
			: m_Head(new Node<T>(std::move(_value))), m_Size(1) {}

		LinkedList(LinkedList<T>&& _other)
			: m_Head(_other.m_Head), m_Size(_other.m_Size) 
		{
			_other.m_Head = nullptr;
		}

		~LinkedList()
		{
			Clear();
			Delete(m_Head, sizeof(Node<T>));
		}

		// Utility
		bool Push(const T& _value)
		{
			if (!m_Head) {
				m_Head = new Node<T>(_value);
				m_Size = 1;
				return true;
			}
			
			Node<T>* node = m_Head;
			Node<T>* prev = nullptr;
			while (node) {
				prev = node;
				node = node->_next;
			}
			
			node = new Node<T>(_value);
			prev->_next = node;

			node = nullptr;
			prev = nullptr;
			free_smem(node);
			free_smem(prev);

			m_Size++;
			return true;
		}

		bool Push(T&& _value)
		{
			if (!m_Head) {
				m_Head = new Node<T>(std::move((T&&)_value));
				m_Size = 1;
				return true;
			}

			Node<T>* node = m_Head;
			Node<T>* prev = nullptr;
			while (node) {
				prev = node;
				node = node->_next;
			}

			node = new Node<T>(std::move((T&&)_value));
			prev->_next = node;

			node = nullptr;
			prev = nullptr;
			free_smem(node);
			free_smem(prev);

			m_Size++;
			return true;
		}

		bool PushRange(const IContainer<T>& _container)
		{
			for (const auto& item : _container.Data()) {
				Push(item);
			}
			return true;
		}

		bool PushRange(std::initializer_list<T>&& _initList)
		{
			for (const auto& item : _initList) {
				Push(std::move((T&&)item));
			}
			return true;
		}

		T Pop()
		{
			if (m_Size > 0) {
				Node<T>* node = m_Head;
				Node<T>* prev = nullptr;
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
			return T();
		}

		bool Remove(const T& _value)
		{
			Node<T>* node = m_Head;
			Node<T>* prev = nullptr;
			while (node) {
				if (node->_value == _value) {
					Node<T>* next = node->_next;
					if (prev) prev->_next = next;
					return true;
				}
				prev = node;
				node = node->_next;
			}
			return false;
		}

		bool Remove(T&& _value)
		{
			Node<T>* node = m_Head;
			Node<T>* prev = nullptr;
			while (node) {
				if (node->_value == _value) {
					Node<T>* next = node->_next;
					if (prev) prev->_next = next;
					return true;
				}
				prev = node;
				node = node->_next;
			}
			return false;
		}

		bool Contains(const T& _value) const
		{
			for (Node<T>* node = m_Head; node != nullptr; node = node->_next) {
				if (node->_value == _value)
					return true;
			}
			return false;
		}

		bool Contains(T&& _value) const
		{
			for (Node<T>* node = m_Head; node != nullptr; node = node->_next) {
				if (node->_value == _value)
					return true;
			}
			return false;
		}

		void Clear()
		{
			Node<T>* node = m_Head;
			Node<T>* prev = nullptr;
			while (node) {
				if (!node->_next) {
					node->~Node();
					break;
				}
				prev = node;
				node = node->_next;
				prev->~Node();
			}

			m_Head = nullptr;
			m_Size = 0;

			m_Data.Clear();
		}

		// Accessors
		constexpr inline size_t Capacity() const override { return m_Size; }
		constexpr inline const T& Head() const { return m_Head->_value; }

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
		virtual List<T> ToList() override
		{
			List<T> list(m_Size);
			for (Node<T>* node = m_Head; node != nullptr; node = node->_next) {
				list.Add(node->_value);
			}
			return list;
		}

		virtual Set<T> ToSet() override
		{
			Set<T> set(m_Size);
			for (Node<T>* node = m_Head; node != nullptr; node = node->_next) {
				set.Add(node->_value);
			}
			return set;
		}

		virtual Stack<T> ToStack() override
		{
			Stack<T> stack(m_Size);
			for (Node<T>* node = m_Head; node != nullptr; node = node->_next) {
				stack.Push(node->_value);
			}
			return stack;
		}

		virtual Queue<T> ToQueue() override
		{
			Queue<T> queue(m_Size);
			for (Node<T>* node = m_Head; node != nullptr; node = node->_next) {
				queue.Enqueue(node->_value);
			}
			return queue;
		}

		// IContainer
		virtual void ForEach(const Param<const T&>& _param) override
		{
			for (Node<T>* node = m_Head; node != nullptr; node = node->_next) {
				_param(node->_value);
			}
		}

		constexpr virtual T* Data() const override
		{
			m_Data.Clear();
			Collect();
			return m_Data.Data();
		}

		// Operator Overloads
		friend std::ostream& operator<<(std::ostream& _stream, LinkedList<T>& _current)
		{
			_stream << _current.ToList();
			return _stream;
		}
	private:
		void Collect() const
		{
			for (Node<T>* node = m_Head; node != nullptr; node = node->_next) {
				m_Data.Add(node->_value);
			}
		}

	private:
		Node<T>* m_Head = nullptr;
		size_t m_Size = 0;

		mutable List<T> m_Data;
	};

}