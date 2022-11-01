#pragma once
#include "Interfaces/IStruct.h"
#include "Utility/DoubleNode.h"

namespace mtk {

	template<typename _Type>
	class BinaryTree : public IExtendable<_Type>, public IConvert<_Type>
	{
	public:
		using ValueType = _Type;
		using Iterator = ContainerIterator<BinaryTree<_Type>>;

	public:
		friend class List<_Type>;
		friend class Set<_Type>;
		friend class Stack<_Type>;
		friend class Queue<_Type>;

	public:
		BinaryTree() {}

		BinaryTree(const _Type& _root)
			: m_Root(new DoubleNode<_Type>(_root)), m_Size(1) {}

		~BinaryTree()
		{
			Clear();
		}

		// Utility
		bool Insert(const _Type& _value)
		{
			if (!m_Root) {
				m_Root = new DoubleNode<_Type>(_value);
				m_Size++;
				return true;
			}
			else {

				DoubleNode<_Type>* parent = nullptr;
				DoubleNode<_Type>* child = m_Root;

				while (child) {
					if (_value <= child->_value) {
						parent = child;
						child = child->_left;
					}
					else {
						parent = child;
						child = child->_right;
					}
				}

				if (_value < parent->_value) {
					parent->_left = new DoubleNode<_Type>(_value);
				}
				else {
					parent->_right = new DoubleNode<_Type>(_value);
				}

				parent = nullptr;
				child = nullptr;
				free_smem(parent);
				free_smem(child);

				m_Size++;
				return true;

			}
			return false;
		}

		bool InsertRange(const IExtendable<_Type>& _container)
		{
			for (const auto& item : _container.Data()) {
				Insert(item);
			}
			return true;
		}

		bool InsertRange(const std::initializer_list<_Type>& _initList)
		{
			for (const auto& item : _initList) {
				Insert(item);
			}
			return true;
		}


		bool Remove(const _Type& _value)
		{
			bool found = false;
			DoubleNode<_Type>* parent = nullptr;
			DoubleNode<_Type>* child = m_Root;

			while (child) {
				if (_value < child->_value) {
					parent = child;
					child = child->_left;
				}
				else if (_value > child->_value) {
					parent = child;
					child = child->_right;
				}
				else {
					found = true;
					break;
				}
			}

			if (found) {
				if (child->_left && child->_right) {
					if (!child->_left) {
						if (parent->_left == child)
							parent->_left = child->_right;
						else
							parent->_right = child->_right;
					}
					else if (child->_left) {
						if (parent->_left == child)
							parent->_left = child->_right;
						else
							parent->_right = child->_right;
					}
					else {
						DoubleNode<_Type>* parentLargest = child;
						DoubleNode<_Type>* largest = child->_left;
						while (largest) {
							parentLargest = largest;
							largest = largest->_right;
						}
						child->_value = largest->_value;

						if (parentLargest == child)
							child->_left = largest->_left;
						else
							parentLargest->_right = largest->_left;
					}
				}
			}

			parent = nullptr;
			child = nullptr;
			free_smem(parent);
			free_smem(child);

			m_Size--;
			return found;
		}

		bool RemoveAt(size_t _index)
		{
			return false;
		}

		bool Contains(const _Type& _value) const
		{
			DoubleNode<_Type>* parent = nullptr;
			DoubleNode<_Type>* child = m_Root;

			while (child) {
				if (child->_value == _value) {
					parent = nullptr;
					child = nullptr;
					free_smem(parent);
					free_smem(child);
					return true;
				}

				if (_value < child->_value) {
					parent = child;
					child = child->_left;
				}
				else if (_value > child->_value) {
					parent = child;
					child = child->_right;
				}
			}

			parent = nullptr;
			child = nullptr;
			free_smem(parent);
			free_smem(child);
			return false;
		}

		void Clear()
		{
			FreeNodes(m_Root);
		}

		// Iterators
		constexpr Iterator begin()
		{
			return Iterator(ToList().m_Data);
		}

		constexpr Iterator end()
		{
			return Iterator(ToList().m_Data + m_Size);
		}

		// Accessors
		constexpr inline _Type& Root() const { return m_Root->_value; }
		constexpr inline size_t Capacity() const override { return m_Size; }

		// IConvert
		virtual List<_Type> ToList() override
		{
			List<_Type> list(m_Size);
			ForEach([&](const _Type& _value)
				{
					list.Add(_value);
				});
			return list;
		}

		virtual Stack<_Type> ToStack() override
		{
			Stack<_Type> stack(m_Size);
			return stack;
		}

		virtual Queue<_Type> ToQueue() override
		{
			Queue<_Type> queue(m_Size);
			return queue;
		}

		// IExtendable
		virtual void ForEach(const Param<const _Type&>& _param) override
		{
			Traverse(m_Root, _param);
		}

		constexpr virtual _Type* Data() const override
		{
			m_Data.Clear();
			Collect(m_Root);
			return m_Data.Data();
		}

		// Operator Overloads
		friend std::ostream& operator<<(std::ostream& _stream, BinaryTree<_Type>& _current)
		{
			_stream << _current.ToList();
			return _stream;
		}

	private:
		void Traverse(const DoubleNode<_Type>* _current, const Param<const _Type&>& _param)
		{
			if (_current) {
				_param(_current->_value);
				Traverse(_current->_left, _param);
				Traverse(_current->_right, _param);
			}
		}

		void Collect(const DoubleNode<_Type>* _current) const
		{
			if (_current) {
				m_Data.Add(_current->_value);
				Collect(_current->_left);
				Collect(_current->_right);
			}
		}

		void FreeNodes(const DoubleNode<_Type>* _current)
		{
			if (_current) {
				FreeNodes(_current->_left);
				FreeNodes(_current->_right);
				_current = nullptr;
				free((void*)_current);
			}
		}

	private:
		DoubleNode<_Type>* m_Root = nullptr;
		size_t m_Size = 0;

		mutable List<_Type> m_Data;
	};
}