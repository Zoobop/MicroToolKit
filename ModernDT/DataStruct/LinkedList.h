#pragma once

#include "Container.h"

namespace mdt {

	template<typename T>
	class LinkedList
	{
	private:
		template<typename T>
		struct Node
		{
			T _value;
			Node<T>* _next = nullptr;
			Node<T>* _previous = nullptr;

			Node(const T& _value)
				: _value(_value) {}

			~Node()
			{
				delete _next;
				delete _previous;
			}
		};

	public:
		using ValueType = T;
		using Iterator = ContainerIterator<LinkedList<T>>;

	public:
		LinkedList() {}

		LinkedList(const T& _value)
			: m_Head(new Node<T>(_value)), m_Size(1) {}



	private:
		Node<T>* m_Head = nullptr;
		size_t m_Size = 0;
	};

}