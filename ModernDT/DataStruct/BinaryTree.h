#pragma once
#include "Container.h"
#include "Basic.h"

namespace mdt {

	template<typename T>
	class BinaryTree
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
			}
		};

	public:
		using ValueType = T;
		using Iterator = ContainerIterator<BinaryTree<T>>;

	public:
		friend List<T>;

	public:
		BinaryTree() {}

		BinaryTree(const T& _root)
			: m_Root(new Node<T>(_root)), m_Size(1) {}

		virtual ~BinaryTree()
		{
			delete m_Root;
		}

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
					if (_value < child->_value) {
						parent = child;
						child = child->_left;
					}
					else if (_value > child->_value) {
						parent = child;
						child = child->_right;
					}
					else {
						return false;
					}
				}

				if (_value < parent->_value) {
					parent->_left = new Node<T>(_value);
				}
				else {
					parent->_right = new Node<T>(_value);
				}

				m_Size++;
				return true;

			}
			return false;
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

			delete parent;
			delete child;

			m_Size--;
			return found;
		}

		bool RemoveAt(size_t _index)
		{
			return false;
		}

		T Find(const T& _value) const
		{

			return false;
		}

		void Clear()
		{
			
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
		constexpr inline Node<T>* Root() const { return m_Root; }
		constexpr inline size_t Size() const { return m_Size; }

		// Operator Overloads
		friend std::ostream& operator<<(std::ostream& _stream, BinaryTree<T>& _current)
		{
			_stream << _current.ToList();
			return _stream;
		}

		// Conversions
		List<T> ToList()
		{
			List<T> list(m_Size);
			return list;
		}

		void ForEach(Param<const T&> _param)
		{
			Traverse(m_Root, _param);
		}

	private:
		void Traverse(const Node<T>* _current, Param<const T&> _param)
		{
			if (_current) {
				_param(_current->_value);
				Traverse(_current->_left, _param);
				Traverse(_current->_right, _param);
			}
		}

		void Print(const T& _value)
		{
			std::cout << _value << std::endl;
		}

	private:
		Node<T>* m_Root = nullptr;
		size_t m_Size = 0;
	};
}