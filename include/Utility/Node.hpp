#pragma once

#include "Core/Memory/Memory.hpp"

namespace Micro
{
	template <typename TNode>
	concept IsNode = requires(TNode node)
	{
		{ node.Value };
		{ node.Status };
		{ node.IsValid() };
	};

	template <typename T>
	struct SingleNode final
	{
		// Allocator
		class Allocator final
		{
		public:
			using Node = SingleNode;

			NODISCARD static size_t Allocate(Node*& head, const size_t newCapacity)
			{
				// Allocate head
				head = CreateEmptyNode();

				// Allocate remaining nodes
				auto node = head;
				const size_t length = newCapacity;
				for (size_t i = 1; i < length; i++, node = node->Next)
				{
					auto newNode = CreateEmptyNode();
					newNode->Prev = node;
					node->Next = newNode;
				}

				return newCapacity;
			}

			NODISCARD static size_t Reallocate(Node*& head, Node* tail, const size_t currentCapacity,
			                                   const size_t newCapacity)
			{
				// Only allocate if greater capacity
				if (currentCapacity >= newCapacity)
					return currentCapacity;

				// Check if head is valid
				if (!IsNodeValid(head))
					return Allocate(head, newCapacity);

				// Get tail and allocate new nodes
				auto node = tail;
				const size_t length = newCapacity - currentCapacity;
				for (size_t i = 0; i < length; i++, node = node->Next)
				{
					auto newNode = CreateEmptyNode();
					newNode->Prev = node;
					node->Next = newNode;
				}

				return newCapacity;
			}

			static void Dispose(Node* head)
			{
				auto node = head;
				while (IsNodeValid(node))
				{
					auto next = node->Next;
					node->Invalidate();
					delete node;

					node = next;
				}
			}

			NODISCARD constexpr static bool IsNodeValid(const Node* node) noexcept
			{
				return node != nullptr && node->IsValid();
			}

		private:
			NODISCARD constexpr static Node* CreateEmptyNode() noexcept
			{
				auto node = Alloc<Node>(1);
				node->Next = nullptr;
				node->Prev = nullptr;
				node->Status = MemStatus::Invalid;
				return node;
			}
		};

		// Fields
		T Value;
		SingleNode* Next = nullptr;
		MemStatus Status = MemStatus::Invalid;

		// Constructors
		constexpr SingleNode() noexcept = default;

		explicit constexpr SingleNode(const T& value) noexcept
			: Value(value), Status(MemStatus::Valid)
		{
		}

		explicit constexpr SingleNode(T&& value) noexcept
			: Value(std::move(value)), Status(MemStatus::Valid)
		{
		}

		// Utility
		constexpr void Invalidate() noexcept
		{
			Next = nullptr;
			Status = MemStatus::Invalid;
		}

		NODISCARD constexpr bool IsValid() const noexcept { return Status == MemStatus::Valid; }
	};

	template <typename T>
	struct DoubleNode final
	{
		// Allocator
		class Allocator final
		{
		public:
			using Node = DoubleNode;

			NODISCARD static size_t Allocate(Node*& head, const size_t newCapacity)
			{
				// Allocate head
				head = CreateEmptyNode();

				// Allocate remaining nodes
				auto node = head;
				const size_t length = newCapacity;
				for (size_t i = 1; i < length; i++, node = node->Next)
				{
					auto newNode = CreateEmptyNode();
					newNode->Prev = node;
					node->Next = newNode;
				}

				return newCapacity;
			}

			NODISCARD static size_t Reallocate(Node*& head, Node* tail, const size_t currentCapacity,
			                                   const size_t newCapacity)
			{
				// Only allocate if greater capacity
				if (currentCapacity >= newCapacity)
					return currentCapacity;

				// Check if head is valid
				if (!IsNodeValid(head))
					return Allocate(head, newCapacity);

				// Get tail and allocate new nodes
				auto node = tail;
				const size_t length = newCapacity - currentCapacity;
				for (size_t i = 0; i < length; i++, node = node->Next)
				{
					auto newNode = CreateEmptyNode();
					newNode->Prev = node;
					node->Next = newNode;
				}

				return newCapacity;
			}

			static void Dispose(Node* head)
			{
				auto node = head;
				while (IsNodeValid(node))
				{
					auto next = node->Next;
					node->Invalidate();
					delete node;

					node = next;
				}
			}

			NODISCARD constexpr static bool IsNodeValid(const Node* node) noexcept
			{
				return node != nullptr && node->IsValid();
			}

		private:
			NODISCARD constexpr static Node* CreateEmptyNode() noexcept
			{
				auto node = Alloc<Node>(1);
				node->Next = nullptr;
				node->Prev = nullptr;
				node->Status = MemStatus::Invalid;
				return node;
			}
		};

		// Fields
		T Value;
		DoubleNode* Next = nullptr;
		DoubleNode* Prev = nullptr;
		MemStatus Status = MemStatus::Invalid;

		// Constructors
		constexpr DoubleNode() noexcept = default;

		explicit constexpr DoubleNode(const T& value) noexcept
			: Value(value), Status(MemStatus::Valid)
		{
		}

		explicit constexpr DoubleNode(T&& value) noexcept
			: Value(std::move(value)), Status(MemStatus::Valid)
		{
		}

		// Utility
		constexpr void Invalidate() noexcept
		{
			Next = nullptr;
			Prev = nullptr;
			Status = MemStatus::Invalid;
		}

		NODISCARD constexpr bool IsValid() const noexcept { return Status == MemStatus::Valid; }
	};

	template <typename T>
	struct HashNode final
	{
		T Value;
		HashNode* Next = nullptr;
		MemStatus Status = MemStatus::Invalid;

		constexpr HashNode() noexcept = default;

		constexpr explicit HashNode(const T& value) noexcept
			: Value(value), Status(MemStatus::Valid)
		{
		}

		constexpr explicit HashNode(T&& value) noexcept
			: Value(std::move(value)), Status(MemStatus::Valid)
		{
		}

		NODISCARD constexpr bool IsValid() const noexcept { return Status == MemStatus::Valid; }

		NODISCARD constexpr void Dispose() noexcept
		{
			Value.~T();
			Next = nullptr;
			Status = MemStatus::Invalid;
		}
	};
}
