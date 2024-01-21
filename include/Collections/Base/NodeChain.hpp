#pragma once
#include "Core/Core.hpp"
#include "Core/Typedef.hpp"

namespace Micro
{
	template <typename T, template <typename> typename TNode>
	class NodeChain : public Enumerable<T>
	{
	public:
		/*
		 *  ============================================================
		 *	|                          Aliases                         |
		 *  ============================================================
		 */
		

		using Node = TNode<T>;
		using Allocator = typename Node::Allocator;


		/*
		 *  ============================================================
		 *	|                 Constructor/Destructors                  |
		 *  ============================================================
		 */
		constexpr NodeChain() noexcept = default;

		constexpr NodeChain(const NodeChain& other) noexcept
		{
			if (!IsNodeValid(other.m_Head))
				return;

			Allocate(other.m_Size);

			CopyFrom(other);
		}

		constexpr NodeChain(NodeChain&& other) noexcept
			: m_Head(other.m_Head), m_Tail(other.m_Tail), m_Size(other.m_Size)
		{
			other.m_Head = nullptr;
			other.m_Tail = nullptr;
			other.m_Size = 0;
		}

		constexpr NodeChain(std::initializer_list<T>&& initializerList) noexcept
		{
			const usize length = initializerList.size();
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

		constexpr explicit NodeChain(std::convertible_to<T> auto... elements) noexcept
		{
			// Get number of elements (arg count)
			const usize length = sizeof ...(elements);
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

		constexpr ~NodeChain() noexcept override
		{
			// Invalidate memory, then free
			Clear();
		}


		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		constexpr void Clear() noexcept
		{
			// Invalidate data
			Allocator::Dispose(m_Head);
			m_Head = nullptr;
			m_Tail = nullptr;
			m_Size = 0;
		}

		NODISCARD constexpr bool Contains(const T& value) const noexcept
		{
			auto node = m_Head;
			for (usize i = 0; i < m_Size; i++, node = node->Next)
				if (node->Value == value)
					return true;

			return false;
		}


		/*
		 *  ============================================================
		 *	|                        Accessors                         |
		 *  ============================================================
		 */


		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Size == 0; }
		NODISCARD constexpr const Node* Head() const noexcept { return m_Head; }
		NODISCARD constexpr const Node* Tail() const noexcept { return m_Tail; }
		NODISCARD constexpr usize Size() const noexcept { return m_Size; }

		/* Enumerators (Iterators) */

		NODISCARD Enumerator<T> GetEnumerator() noexcept override
		{
			auto node = m_Head;
			while (node)
			{
				auto& value = node->Value;
				co_yield value;

				node = node->Next;
			}
		}

		NODISCARD Enumerator<T> GetEnumerator() const noexcept override
		{
			auto node = const_cast<Node*>(m_Head);
			while (node)
			{
				const auto& value = node->Value;
				co_yield value;

				node = node->Next;
			}
		}


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		constexpr NodeChain& operator=(const NodeChain& other) noexcept
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

		constexpr NodeChain& operator=(NodeChain&& other) noexcept
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

		friend std::ostream& operator<<(std::ostream& stream, const NodeChain& current) noexcept
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
		/*
		 *  ============================================================
		 *	|                      Internal Helpers                    |
		 *  ============================================================
		 */


		constexpr void Allocate(const usize capacity) noexcept
		{
			m_Size = Allocator::Allocate(m_Head, capacity);
		}

		constexpr void Reallocate(const usize capacity) noexcept
		{
			m_Size = Allocator::Reallocate(m_Head, m_Tail, m_Size, capacity);
		}

		constexpr void CopyFrom(const NodeChain& other) noexcept
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

		
		/*
		 *  ============================================================
		 *	|                          Static                          |
		 *  ============================================================
		 */


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

		constexpr static void AssignNode(Node*& node, const T& value) noexcept
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

		constexpr static void AssignNode(Node*& node, T&& value) noexcept
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
		usize m_Size = 0;
	};
}
