#pragma once

#include "Collections/Base/NodeChain.hpp"

namespace Micro
{
	template <typename T>
	class LinkedList final : public NodeChain<T, DoubleNode<T>>
	{
	public:
		// Aliases
		using Base = NodeChain<T, DoubleNode<T>>;
		using Node = DoubleNode<T>;

		friend Base;

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
		void PushBack(const T& value)
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

		void PushBack(T&& value)
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

		void PushFront(const T& value)
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

		void PushFront(T&& value)
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

		NODISCARD T PopBack()
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

		NODISCARD T PopFront()
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

		bool Remove(const T& value)
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

		NODISCARD T& At(const size_t index)
		{
			if (index >= Base::m_Size)
				throw IndexOutOfRangeError(index);

			auto node = Base::m_Head;
			for (size_t i = 0; i < index; i++)
				node = node->Next;

			return node->Value;
		}

		NODISCARD const T& At(const size_t index) const
		{
			if (index >= Base::m_Size)
				throw IndexOutOfRangeError(index);

			auto node = Base::m_Head;
			for (size_t i = 0; i < index; i++)
				node = node->Next;

			return node->Value;
		}

		// Operator Overloads
		LinkedList& operator=(const LinkedList& other)
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

		LinkedList& operator=(LinkedList&& other) noexcept
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
		static constexpr size_t c_DefaultCapacity = 4;
	};
}
