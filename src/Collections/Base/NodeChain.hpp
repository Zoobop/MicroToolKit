#pragma once
#include "Iterator.hpp"
#include "Core/Core.hpp"

#include "Core/Errors/Error.hpp"

namespace Micro
{
	template <typename T, typename TNode>
	class NodeChain
	{
	public:
		// Aliases
		using Node = TNode;
		using Allocator = typename Node::Allocator;
		using Iterator = SegmentedIterator<T, Node>;
		using ConstIterator = const Iterator;

		// Constructors/Destructor
		constexpr NodeChain() noexcept = default;

		NodeChain(const NodeChain& other)
		{
			if (!IsNodeValid(other.m_Head))
				return;

			Allocate(other.m_Size);

			CopyFrom(other);
		}

		NodeChain(NodeChain&& other) noexcept
			: m_Head(other.m_Head), m_Tail(other.m_Tail), m_Size(other.m_Size)
		{
			other.m_Head = nullptr;
			other.m_Tail = nullptr;
			other.m_Size = 0;
		}

		NodeChain(std::initializer_list<T>&& initializerList) noexcept
		{
			const size_t length = initializerList.size();
			if (length == 0)
				return;

			// Move from
			m_Size = length;

			Allocate(length);

			// Move values into node chain
			auto node = m_Head;
			for (auto& elem : initializerList)
			{
				new(&node->Value) T(std::move(const_cast<T&>(elem)));
				node->Status = MemStatus::Valid;

				if (!IsNodeValid(node->Next))
					m_Tail = node;

				node = node->Next;
			}
		}

		explicit NodeChain(std::convertible_to<T> auto... elements) noexcept
		{
			// Get number of elements (arg count)
			const size_t length = sizeof ...(elements);
			m_Size = length;

			Allocate(length);

			// Move values into node chain
			auto node = m_Head;
			for (auto values = {elements...}; auto&& elem : values)
			{
				new(&node->Value) T(std::move(elem));
				node->Status = MemStatus::Valid;

				if (!IsNodeValid(node->Next))
					m_Tail = node;

				node = node->Next;
			}
		}

		virtual ~NodeChain()
		{
			// Invalidate memory, then free
			Clear();
		}

		// Utility
		virtual void Clear()
		{
			// Invalidate data
			Allocator::Dispose(m_Head);
			m_Head = nullptr;
			m_Tail = nullptr;
			m_Size = 0;
		}

		NODISCARD virtual bool Contains(const T& value) const
		{
			auto node = m_Head;
			for (size_t i = 0; i < m_Size; i++, node = node->Next)
				if (node->Value == value)
					return true;

			return false;
		}

		// Accessors
		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Size == 0; }
		NODISCARD constexpr const Node* Head() const noexcept { return m_Head; }
		NODISCARD constexpr const Node* Tail() const noexcept { return m_Tail; }
		NODISCARD constexpr size_t Size() const noexcept { return m_Size; }

		// Iterators
		NODISCARD constexpr Iterator begin() { return Iterator(m_Head); }
		NODISCARD constexpr Iterator end() { return Iterator(nullptr); }
		NODISCARD constexpr ConstIterator cbegin() const { return begin(); }
		NODISCARD constexpr ConstIterator cend() const { return end(); }

		NodeChain& operator=(const NodeChain& other)
		{
			// Validation
			if (this == &other)
				return *this;

			if (other.IsEmpty())
				return *this;

			// Allocation
			if (IsNodeValid(m_Head))
				Reallocate(other.m_Size);
			else
				Allocate(other.m_Size);

			// Assignment
			CopyFrom(other);

			return *this;
		}

		NodeChain& operator=(NodeChain&& other) noexcept
		{
			if (this == &other)
				return *this;

			m_Head = other.m_Head;
			m_Tail = other.m_Tail;
			m_Size = other.m_Size;

			other.m_Head = nullptr;
			other.m_Tail = nullptr;
			other.m_Size = 0;

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const NodeChain& current)
		{
			stream << "[";
			auto node = current.m_Head;
			while (IsNodeValid(node))
			{
				stream << node->Value;
				if (IsNodeValid(node->Next))
					stream << " -> ";

				node = node->Next;
			}

			stream << "]";
			return stream;
		}

	protected:
		void Allocate(const size_t capacity) noexcept
		{
			m_Size = Allocator::Allocate(m_Head, capacity);
		}

		void Reallocate(const size_t capacity) noexcept
		{
			m_Size = Allocator::Reallocate(m_Head, m_Tail, m_Size, capacity);
		}

		NODISCARD void CopyFrom(const NodeChain& other)
		{
			auto node = m_Head;
			for (auto iter = other.m_Head; IsNodeValid(iter); iter = iter->Next)
			{
				node->Value = iter->Value;

				if (!IsNodeValid(node->Next))
					m_Tail = node;

				node = node->Next;
			}
		}

		// Static
		NODISCARD constexpr static bool IsNodeValid(const Node* node) noexcept
		{
			return Allocator::IsNodeValid(node);
		}

		NODISCARD constexpr static Node* CreateEmptyNode() noexcept
		{
			auto node = Alloc<Node>(1);
			node->Status = MemStatus::Invalid;
			return node;
		}

		NODISCARD constexpr static void AssignNode(Node*& node, const T& value) noexcept
		{
			if (node == nullptr)
			{
				node = Alloc<Node>(1);
				new(node) Node(value);
				return;
			}

			new(&node->Value) T(value);
			node->Status = MemStatus::Valid;
		}

		NODISCARD constexpr static void AssignNode(Node*& node, T&& value) noexcept
		{
			if (node == nullptr)
			{
				node = Alloc<Node>(1);
				new(node) Node(std::move(value));
				return;
			}

			new(&node->Value) T(std::move(value));
			node->Status = MemStatus::Valid;
		}

	protected:
		Node* m_Head = nullptr;
		Node* m_Tail = nullptr;
		size_t m_Size = 0;
	};
}
