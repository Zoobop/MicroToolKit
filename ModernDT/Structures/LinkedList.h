#pragma once

#include "Interfaces/IExtendable.h"
#include "Interfaces/IConvert.h"
#include "Utility/SingleNode.h"

namespace mtk {

	template<typename _Type>
	class LinkedList : public IExtendable<_Type>, public IConvert<_Type>
	{
	public:
		using ValueType = _Type;
		using _Node = SingleNode<_Type>;
		using Iterator = ContainerIterator<LinkedList<_Type>>;

	public:
		friend class List<_Type>;
		friend class Queue<_Type>;
		friend class Stack<_Type>;

	public:
		LinkedList() 
		{
			ReAlloc(5);
		}

		LinkedList(const _Type& _value)
			: m_Head(new SingleNode<_Type>(_value)), m_Size(1), m_Capacity(2)
		{
			m_Head->_next = new _Node();
		}

		LinkedList(_Type&& _value)
			: m_Head(new SingleNode<_Type>(std::move(_value))), m_Size(1), m_Capacity(2) 
		{
			m_Head->_next = new _Node();
		}

		LinkedList(const LinkedList<_Type>& _other)
			: m_Head(_other.m_Head), m_Size(_other.m_Size), m_Capacity(_other.m_Capacity)
		{
		}

		LinkedList(LinkedList<_Type>&& _other)
			: m_Head(_other.m_Head), m_Size(_other.m_Size), m_Capacity(_other.m_Capacity)
		{
			_other.m_Head = nullptr;
			_other.m_Size = 0;
			_other.m_Capacity = 0;
		}

		~LinkedList()
		{
			CleanUp();
			Delete(m_Head, sizeof(SingleNode<_Type>));
		}

		// Utility
		bool Push(const _Type& _value)
		{
			if (m_Head->_control != Ctrl::kFull) {
				m_Head->_value = _value;
				m_Head->_control = Ctrl::kFull;
				m_Size = 1;
				return true;
			}
			
			if (m_Size >= m_Capacity)
				ReAlloc(m_Capacity * 2);

			_Node* node = m_Head;
			while (node->_control == Ctrl::kFull) {
				node = node->_next;
			}
			node->_value = _value;
			node->_control = Ctrl::kFull;

			node = nullptr;
			free_smem(node);

			m_Size++;
			return true;
		}

		bool Push(_Type&& _value)
		{
			if (m_Head->_control != Ctrl::kFull) {
				m_Head->_value = _value;
				m_Head->_control = Ctrl::kFull;
				m_Size = 1;
				return true;
			}

			if (m_Size >= m_Capacity)
				ReAlloc(m_Capacity * 2);

			_Node* node = m_Head;
			while (node->_control == Ctrl::kFull) {
				node = node->_next;
			}
			node->_value = _value;
			node->_control = Ctrl::kFull;

			node = nullptr;
			free_smem(node);

			m_Size++;
			return true;
		}

		bool PushRange(const IDataHandler<_Type>& _container)
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
				_Node* iter = m_Head;
				_Node* node = nullptr;
				while (iter->_control == Ctrl::kFull) {
					node = iter;
					iter = iter->_next;
				}
				node->_control = Ctrl::kInvalid;

				m_Size--;
				return node->_value;
			}
			return {};
		}

		bool Remove(const _Type& _value)
		{
			_Node* node = m_Head;
			while (node) {
				if (node->_value == _value) {
					node->_control = Ctrl::kInvalid;
					return true;
				}
				node = node->_next;
			}
			return false;
		}

		bool Remove(_Type&& _value)
		{
			_Node* node = m_Head;
			while (node) {
				if (node->_value == _value) {
					node->_control = Ctrl::kInvalid;
					return true;
				}
				node = node->_next;
			}
			return false;
		}

		bool Contains(const _Type& _value) const
		{
			for (_Node* node = m_Head; node != nullptr; node = node->_next) {
				if (node->_value == _value)
					return true;
			}
			return false;
		}

		bool Contains(_Type&& _value) const
		{
			for (_Node* node = m_Head; node != nullptr; node = node->_next) {
				if (node->_value == _value)
					return true;
			}
			return false;
		}

		void Reverse()
		{
			_Node* current = m_Head;
			_Node* prev = nullptr, *next = nullptr;

			while (current&& current->_control != Ctrl::kEmpty) {

				next = current->_next;

				current->_next = prev;
				prev = current;

				current = next;
			}
			m_Head = prev;
		}

		void Clear()
		{
			_Node* node = m_Head;
			while (node) {
				node->_control = Ctrl::kInvalid;
				node = node->_next;
			}

			m_Size = 0;
			m_Data.Clear();
		}

		// Accessors
		constexpr inline size_t Capacity() const { return m_Size; }
		constexpr inline size_t Size() const { return m_Size; }
		constexpr inline SingleNode<_Type>* Head() { return m_Head; }
		constexpr inline void Reserve(size_t _capacity) { ReAlloc(_capacity); }

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
			for (_Node* node = m_Head; node != nullptr; node = node->_next) {
				list.Add(node->_value);
			}
			return list;
		}

		virtual Stack<_Type> ToStack() override
		{
			Stack<_Type> stack(m_Size);
			for (_Node* node = m_Head; node != nullptr; node = node->_next) {
				stack.Push(node->_value);
			}
			return stack;
		}

		virtual Queue<_Type> ToQueue() override
		{
			Queue<_Type> queue(m_Size);
			for (_Node* node = m_Head; node != nullptr; node = node->_next) {
				queue.Enqueue(node->_value);
			}
			return queue;
		}

		// IExtendable
		virtual void ForEach(const Param<const _Type&>& _param) override
		{
			for (_Node* node = m_Head; node != nullptr; node = node->_next) {
				_param(node->_value);
			}
		}

		// Operator Overloads
		friend std::ostream& operator<<(std::ostream& _stream, LinkedList<_Type>& _current)
		{
			_stream << "[ ";
			auto node = _current.m_Head;
			for (auto i = 0; i < _current.m_Size && node; i++) {
				if (node->_control == Ctrl::kFull) {
					_stream << *node;
				}
				node = node->_next;
			}
			_stream << "]";
			return _stream;
		}
	private:
		void Collect() const
		{
			for (SingleNode<_Type>* node = m_Head; node != nullptr; node = node->_next) {
				m_Data.Add(node->_value);
			}
		}

		void CleanUp()
		{
			_Node* node = m_Head;
			_Node* prev = nullptr;
			while (node) {
				if (!node->_next) {
					node->~SingleNode();
					break;
				}
				prev = node;
				node = node->_next;
				prev->~SingleNode();
			}

			m_Size = 0;

			m_Data.Clear();
		}

		void ReAlloc(size_t _capacity)
		{
			if (m_Size < _capacity) {
				_Node* iter = m_Head;
				_Node* node = nullptr;
				while (iter) {
					node = iter;
					iter = iter->_next;
				}

				if (node == nullptr) {
					m_Head = new _Node();
					node = m_Head;
				}
				size_t difference = fabs(_capacity - m_Capacity);
				for (auto i = 0; i < difference; i++) {
					_Node* newNode = new _Node();
					node->_next = newNode;

					newNode = nullptr;
					delete newNode;

					node = node->_next;
				}

				iter = nullptr;
				node = nullptr;
				delete iter;
				delete node;
			}
			else {
				m_Size = _capacity;
				_Node* node = m_Head;

				for (auto i = 0; i <= _capacity; i++) {
					node = node->_next;
				}

				while (node) {
					node->_control = Ctrl::kEmpty;
					node = node->_next;
				}

				node = nullptr;
				delete node;
			}

			m_Capacity = _capacity;
		}

	private:
		_Node* m_Head = nullptr;
		size_t m_Capacity = 0;
		size_t m_Size = 0;

		mutable List<_Type> m_Data;
	};

}