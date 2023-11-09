#pragma once

#include "Collections/Base/NodeChain.hpp"
#include "Utility/Node.hpp"

namespace Micro
{
	template <typename T>
	class LinkedList final : public NodeChain<T, DoubleNode>
	{
	public:
		// Aliases
		using Base = NodeChain<T, DoubleNode>;
		using Node = DoubleNode<T>;

		// Constructors/Destructors
		constexpr LinkedList() noexcept : Base()
		{
		}

		constexpr LinkedList(const LinkedList& other) : Base(other)
		{
		}

		constexpr LinkedList(LinkedList&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr LinkedList(const Base& other) : Base(other)
		{
		}

		constexpr LinkedList(Base&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr LinkedList(std::initializer_list<T>&& initializerList) noexcept : Base(std::move(initializerList))
		{
		}

		explicit constexpr LinkedList(std::convertible_to<T> auto... elements) noexcept : Base(
			std::forward<T>(static_cast<T>(elements))...)
		{
		}

		constexpr ~LinkedList() noexcept override = default;

		// Utility
		constexpr void PushBack(const T& value) noexcept
		{
			// Check head validity
			if (!Base::IsNodeValid(Base::m_Head))
			{
				Base::AssignNode(Base::m_Head, value);
				Base::m_Tail = Base::m_Head;
				++Base::m_Size;
				return;
			}

			// Get last node
			auto node = Base::m_Head;
			while (Base::IsNodeValid(node->Next))
				node = node->Next;

			// Add new node to back of chain
			Base::AssignNode(node->Next, value);
			auto newNode = node->Next;
			newNode->Prev = node;

			// Set new tail
			Base::m_Tail = newNode;

			++Base::m_Size;
		}

		constexpr void PushBack(T&& value) noexcept
		{
			// Check head validity
			if (!Base::IsNodeValid(Base::m_Head))
			{
				Base::AssignNode(Base::m_Head, std::move(value));
				Base::m_Tail = Base::m_Head;
				++Base::m_Size;
				return;
			}

			// Get last node
			auto node = Base::m_Head;
			while (Base::IsNodeValid(node->Next))
				node = node->Next;

			// Add new node to back of chain
			Base::AssignNode(node->Next, std::move(value));
			auto newNode = node->Next;
			newNode->Prev = node;

			// Set new tail
			Base::m_Tail = newNode;

			++Base::m_Size;
		}

		constexpr void PushFront(const T& value) noexcept
		{
			// Check head validity
			if (!Base::IsNodeValid(Base::m_Head))
			{
				Base::AssignNode(Base::m_Head, value);
				Base::m_Tail = Base::m_Head;
				++Base::m_Size;
				return;
			}

			// Add new node to front of chain
			auto node = Base::CreateEmptyNode();
			Base::AssignNode(node, std::move(Base::m_Head->Value));
			Base::AssignNode(Base::m_Head, value);

			node->Next = Base::m_Head->Next;
			node->Prev = Base::m_Head;

			Base::m_Head->Next = node;

			// Set tail, if second node
			if (Base::m_Size == 1)
				Base::m_Tail = node;

			++Base::m_Size;
		}

		constexpr void PushFront(T&& value) noexcept
		{
			// Check head validity
			if (!Base::IsNodeValid(Base::m_Head))
			{
				Base::AssignNode(Base::m_Head, std::move(value));
				Base::m_Tail = Base::m_Head;
				++Base::m_Size;
				return;
			}

			// Add new node to front of chain
			auto node = Base::CreateEmptyNode();
			Base::AssignNode(node, std::move(Base::m_Head->Value));
			Base::AssignNode(Base::m_Head, std::move(value));

			node->Next = Base::m_Head->Next;
			node->Prev = Base::m_Head;

			Base::m_Head->Next = node;

			// Set tail, if second node
			if (Base::m_Size == 1)
				Base::m_Tail = node;

			++Base::m_Size;
		}

		NODISCARD constexpr Result<T> PopBack() noexcept
		{
			if (!Base::IsEmpty())
			{
				// Get popped nodes value
				auto poppedNode = Base::m_Tail;
				T value = std::move(poppedNode->Value);

				// Remove references and free memory
				if (poppedNode == Base::m_Head)
				{
					poppedNode->Invalidate();
					delete poppedNode;

					Base::m_Head = nullptr;
					Base::m_Tail = nullptr;

					--Base::m_Size;
					return std::move(value);
				}

				auto prev = poppedNode->Prev;
				prev->Next = nullptr;

				poppedNode->Invalidate();
				delete poppedNode;

				Base::m_Tail = prev;

				--Base::m_Size;
				return std::move(value);
			}

			throw InvalidOperationError("Linked LinkedList is empty.");
		}

		NODISCARD constexpr Result<T> PopFront() noexcept
		{
			if (!Base::IsEmpty())
			{
				// Get popped nodes value
				auto poppedNode = Base::m_Head;
				T value = std::move(poppedNode->Value);

				// Remove references and free memory
				if (poppedNode == Base::m_Tail)
				{
					poppedNode->Invalidate();
					delete poppedNode;

					Base::m_Head = nullptr;
					Base::m_Tail = nullptr;

					--Base::m_Size;
					return std::move(value);
				}

				// Remove references and free memory
				auto next = poppedNode->Next;
				next->Prev = nullptr;

				poppedNode->Invalidate();
				delete poppedNode;

				Base::m_Head = next;
				--Base::m_Size;

				return std::move(value);
			}

			throw InvalidOperationError("Linked LinkedList is empty.");
		}

		constexpr bool Remove(const T& value) noexcept
		{
			// Check if empty
			if (Base::IsEmpty())
				return false;

			// Handle head removal
			if (!Base::IsNodeValid(Base::m_Head->Next))
			{
				delete Base::m_Head;

				Base::m_Head = nullptr;
				Base::m_Tail = nullptr;
				return true;
			}

			// Standard removal
			for (auto node = Base::m_Head; Base::IsNodeValid(node); node = node->Next)
			{
				if (node->Value != value)
					continue;

				// Cache references
				auto prev = node->Prev;
				auto next = node->Next;

				node->Invalidate();

				if (Base::m_Head == node)
					Base::m_Head = next;
				else if (Base::m_Tail == node)
					Base::m_Tail = prev;

				delete node;

				// Set pointers
				if (Base::IsNodeValid(prev))
					prev->Next = next;
				else if (Base::IsNodeValid(next))
					next->Prev = prev;

				--Base::m_Size;
				return true;
			}

			return false;
		}

		NODISCARD constexpr Result<T&> At(const usize index) noexcept
		{
			if (index >= Base::m_Size)
				throw IndexOutOfRangeError(index);

			auto node = Base::m_Head;
			for (usize i = 0; i < index; i++)
				node = node->Next;

			return node->Value;
		}

		NODISCARD constexpr Result<const T&> At(const usize index) const noexcept
		{
			if (index >= Base::m_Size)
				throw IndexOutOfRangeError(index);

			auto node = Base::m_Head;
			for (usize i = 0; i < index; i++)
				node = node->Next;

			return node->Value;
		}

		// Operator Overloads
		constexpr LinkedList& operator=(const LinkedList& other) noexcept
		{
			// Validation
			if (this == &other)
				return *this;

			if (other.IsEmpty())
				return *this;

			// Allocation
			if (!Base::IsEmpty())
				Base::Reallocate(other.m_Size);
			else
				Base::Allocate(other.m_Size);

			// Assignment
			Base::CopyFrom(other);
			return *this;
		}

		constexpr LinkedList& operator=(LinkedList&& other) noexcept
		{
			// Validation
			if (this == &other)
				return *this;

			if (other.IsEmpty())
				return *this;

			// Dispose
			Base::Clear();

			// Transfer
			Base::m_Head = other.m_Head;
			Base::m_Tail = other.m_Tail;
			Base::m_Size = other.m_Size;

			other.m_Head = nullptr;
			other.m_Tail = nullptr;
			other.m_Size = 0;

			return *this;
		}

	private:
		static constexpr usize c_DefaultCapacity = 4;
	};
}
