#pragma once
#include "Container.h"

namespace mdt {

	template<typename T>
	class BinaryTree : public IContainer<T>, public IConvert<T>
	{
	private:
		template<typename T>
		struct Node
		{
			T _value;
			Node<T>* _left = nullptr;
			Node<T>* _right = nullptr;

			Node(const T& _value)
				: _value(_value) {}

			~Node()
			{
				_left = nullptr;
				_right = nullptr;
				free_smem(_left);
				free_smem(_right);
			}
		};

	public:
		using ValueType = T;
		using Iterator = ContainerIterator<BinaryTree<T>>;

	public:
		friend List<T>;
		friend Set<T>;
		friend Stack<T>;
		friend Queue<T>;

	public:
		BinaryTree() {}

		BinaryTree(const T& _root)
			: m_Root(new Node<T>(_root)), m_Size(1) {}

		~BinaryTree()
		{
			Clear();
		}

		// Utility
		bool Insert(const T& _value)
		{
			if (!m_Root) {
				m_Root = new Node<T>(_value);
				m_Size++;
				return true;
			}
			else {

				Node<T>* parent = nullptr;
				Node<T>* child = m_Root;

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
					parent->_left = new Node<T>(_value);
				}
				else {
					parent->_right = new Node<T>(_value);
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

		bool InsertRange(const IContainer<T>& _container)
		{
			for (const auto& item : _container.Data()) {
				Insert(item);
			}
			return true;
		}

		bool InsertRange(const std::initializer_list<T>& _initList)
		{
			for (const auto& item : _initList) {
				Insert(item);
			}
			return true;
		}


		bool Remove(const T& _value)
		{
			bool found = false;
			Node<T>* parent = nullptr;
			Node<T>* child = m_Root;

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
						Node<T>* parentLargest = child;
						Node<T>* largest = child->_left;
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

		bool Contains(const T& _value) const
		{
			Node<T>* parent = nullptr;
			Node<T>* child = m_Root;

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
		constexpr inline T& Root() const { return m_Root->_value; }
		constexpr inline size_t Capacity() const override { return m_Size; }

		// IConvert
		virtual List<T> ToList() override
		{
			List<T> list(m_Size);
			ForEach([&](const T& _value)
				{
					list.Add(_value);
				});
			return list;
		}

		virtual Set<T> ToSet() override
		{
			Set<T> set(m_Size);
			return set;
		}

		virtual Stack<T> ToStack() override
		{
			Stack<T> stack(m_Size);
			return stack;
		}

		virtual Queue<T> ToQueue() override
		{
			Queue<T> queue(m_Size);
			return queue;
		}

		// IContainer
		virtual void ForEach(const Param<const T&>& _param) override
		{
			Traverse(m_Root, _param);
		}

		constexpr virtual T* Data() const override
		{
			m_Data.Clear();
			Collect(m_Root);
			return m_Data.Data();
		}

		// Operator Overloads
		friend std::ostream& operator<<(std::ostream& _stream, BinaryTree<T>& _current)
		{
			_stream << _current.ToList();
			return _stream;
		}

	private:
		void Traverse(const Node<T>* _current, const Param<const T&>& _param)
		{
			if (_current) {
				_param(_current->_value);
				Traverse(_current->_left, _param);
				Traverse(_current->_right, _param);
			}
		}

		void Collect(const Node<T>* _current) const
		{
			if (_current) {
				m_Data.Add(_current->_value);
				Collect(_current->_left);
				Collect(_current->_right);
			}
		}

		void FreeNodes(const Node<T>* _current)
		{
			if (_current) {
				FreeNodes(_current->_left);
				FreeNodes(_current->_right);
				_current = nullptr;
				free((void*)_current);
			}
		}

	private:
		Node<T>* m_Root = nullptr;
		size_t m_Size = 0;

		mutable List<T> m_Data;
	};
}