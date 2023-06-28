#pragma once

#include "Collections/Container.hpp"
#include "Utility/Result.hpp"

namespace mtk
{
	template <typename T>
	struct LinkedListNode final
	{
		T Value;
		LinkedListNode* Next = nullptr;
		MemStatus Status = MemStatus::Invalid;

		explicit constexpr LinkedListNode(const T& value) noexcept
			: Value(value), Status(MemStatus::Valid)
		{
		}

		explicit constexpr LinkedListNode(T&& value) noexcept
			: Value(std::move(value)), Status(MemStatus::Valid)
		{
		}

		NODISCARD constexpr bool IsValid() const { return Status == MemStatus::Valid; }
	};

	//template <typename T>
	//class LinkedList final : public SegmentedContainer<T, LinkedListNode<T>, Allocator<T>>
	//{
	//public:
	//	// Aliases
	//	using Base = SegmentedContainer<T, LinkedListNode<T>, Allocator<T>>;
	//	using Sequence = Sequence<T>;
	//	using Node = LinkedListNode<T>;

	//	friend Base;

	//	// Constructors/Destructors
	//	constexpr LinkedList() noexcept : Base()
	//	{
	//	}

	//	constexpr LinkedList(const LinkedList& other) : Base(other)
	//	{
	//	}

	//	constexpr LinkedList(LinkedList&& other) noexcept : Base(std::move(other))
	//	{
	//	}

	//	constexpr LinkedList(const Base& other) : Base(other)
	//	{
	//	}

	//	constexpr LinkedList(Base&& other) noexcept : Base(std::move(other))
	//	{
	//	}

	//	constexpr LinkedList(const Sequence& other) : Base(other)
	//	{
	//	}

	//	constexpr LinkedList(Sequence&& other) noexcept : Base(std::move(other))
	//	{
	//	}

	//	constexpr LinkedList(std::initializer_list<T>&& initializerList) noexcept : Base(std::move(initializerList))
	//	{
	//	}

	//	explicit constexpr LinkedList(std::convertible_to<T> auto... elements) noexcept : Base(
	//		std::forward<T>(static_cast<T>(elements))...)
	//	{
	//	}

	//	explicit LinkedList(const size_t capacity) : Base(capacity)
	//	{
	//	}

	//	constexpr ~LinkedList() noexcept override = default;

	//	// Utility
	//	void Push(const T& value)
	//	{
	//		if (!IsNodeValid(Base::m_Head))
	//		{
	//			Base::m_Head = Base::CreateNewNode(value);
	//			++Base::m_Size;
	//			return;
	//		}

	//		if (Base::m_Capacity <= Base::m_Size)
	//			Base::Reallocate(Base::m_Capacity * 2);

	//		auto node = Base::m_Head;
	//		while (!IsNodeValid(node->Next))
	//			node = node->Next;

	//		node = Base::CreateNewNode(value);
	//		++Base::m_Size;
	//	}

	//	void Push(T&& value)
	//	{
	//		if (!IsNodeValid(Base::m_Head))
	//		{
	//			Base::m_Head = Base::CreateNewNode(std::move(value));
	//			++Base::m_Size;
	//			return;
	//		}

	//		if (Base::m_Capacity <= Base::m_Size)
	//			Base::Reallocate(Base::m_Capacity * 2);

	//		auto node = Base::m_Head;
	//		while (node->Next == nullptr)
	//			node = node->Next;

	//		node = Base::CreateNewNode(std::move(value));
	//		++Base::m_Size;
	//	}

	//	//bool PushRange(const IDataHandler<T>& _container)
	//	//{
	//	//	for (const auto& item : _container.Data())
	//	//		Push(item);
	//	//	return true;
	//	//}

	//	//bool PushRange(std::initializer_list<T>&& _initList)
	//	//{
	//	//	for (const auto& item : _initList)
	//	//		Push(std::move(static_cast<T&&>(item)));
	//	//	return true;
	//	//}

	//	NODISCARD Result<T> Pop()
	//	{
	//		if (!Base::IsEmpty())
	//		{
	//			auto node = Base::m_Head;
	//			while (iter->_control == Ctrl::kFull)
	//			{
	//				node = iter;
	//				iter = iter->_next;
	//			}
	//			node->_control = Ctrl::kInvalid;

	//			m_Size--;
	//			return node->_value;
	//		}
	//		return Result<T>::Empty();
	//	}

	//	bool Remove(const T& _value)
	//	{
	//		_Node* node = m_Head;
	//		while (node)
	//		{
	//			if (node->_value == _value)
	//			{
	//				node->_control = Ctrl::kInvalid;
	//				return true;
	//			}
	//			node = node->_next;
	//		}
	//		return false;
	//	}

	//	bool Remove(T&& _value)
	//	{
	//		_Node* node = m_Head;
	//		while (node)
	//		{
	//			if (node->_value == _value)
	//			{
	//				node->_control = Ctrl::kInvalid;
	//				return true;
	//			}
	//			node = node->_next;
	//		}
	//		return false;
	//	}

	//	bool Contains(const T& _value) const
	//	{
	//		for (_Node* node = m_Head; node != nullptr; node = node->_next)
	//			if (node->_value == _value)
	//				return true;
	//		return false;
	//	}

	//	bool Contains(T&& _value) const
	//	{
	//		for (_Node* node = m_Head; node != nullptr; node = node->_next)
	//			if (node->_value == _value)
	//				return true;
	//		return false;
	//	}

	//	void Reverse()
	//	{
	//		_Node* current = m_Head;
	//		_Node *prev = nullptr, *next = nullptr;

	//		while (current && current->_control != Ctrl::kEmpty)
	//		{
	//			next = current->_next;

	//			current->_next = prev;
	//			prev = current;

	//			current = next;
	//		}
	//		m_Head = prev;
	//	}

	//	void Clear() override
	//	{
	//		_Node* node = m_Head;
	//		while (node)
	//		{
	//			node->_control = Ctrl::kInvalid;
	//			node = node->_next;
	//		}

	//		m_Size = 0;
	//		m_Data.Clear();
	//	}

	//	// Accessors
	//	constexpr Node* Head() { return Base::m_Head; }

	//private:
	//	//void Collect() const
	//	//{
	//	//	for (SingleNode<T>* node = m_Head; node != nullptr; node = node->_next)
	//	//		m_Data.Add(node->_value);
	//	//}

	//	//void CleanUp()
	//	//{
	//	//	_Node* node = m_Head;
	//	//	_Node* prev = nullptr;
	//	//	while (node)
	//	//	{
	//	//		if (!node->_next)
	//	//		{
	//	//			node->~SingleNode();
	//	//			break;
	//	//		}
	//	//		prev = node;
	//	//		node = node->_next;
	//	//		prev->~SingleNode();
	//	//	}

	//	//	m_Size = 0;

	//	//	m_Data.Clear();
	//	//}

	//	//void ReAlloc(size_t _capacity)
	//	//{
	//	//	if (m_Size < _capacity)
	//	//	{
	//	//		_Node* iter = m_Head;
	//	//		_Node* node = nullptr;
	//	//		while (iter)
	//	//		{
	//	//			node = iter;
	//	//			iter = iter->_next;
	//	//		}

	//	//		if (node == nullptr)
	//	//		{
	//	//			m_Head = new _Node();
	//	//			node = m_Head;
	//	//		}
	//	//		size_t difference = fabs(_capacity - m_Capacity);
	//	//		for (auto i = 0; i < difference; i++)
	//	//		{
	//	//			_Node* newNode = new _Node();
	//	//			node->_next = newNode;

	//	//			newNode = nullptr;
	//	//			delete newNode;

	//	//			node = node->_next;
	//	//		}

	//	//		iter = nullptr;
	//	//		node = nullptr;
	//	//		delete iter;
	//	//		delete node;
	//	//	}
	//	//	else
	//	//	{
	//	//		m_Size = _capacity;
	//	//		_Node* node = m_Head;

	//	//		for (auto i = 0; i <= _capacity; i++)
	//	//			node = node->_next;

	//	//		while (node)
	//	//		{
	//	//			node->_control = Ctrl::kEmpty;
	//	//			node = node->_next;
	//	//		}

	//	//		node = nullptr;
	//	//		delete node;
	//	//	}

	//	//	m_Capacity = _capacity;
	//	//}
	//};
}
