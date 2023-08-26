#pragma once
#include "Core/Core.hpp"
#include "Core/Memory/Memory.hpp"

namespace Micro::Internal
{
	// HashTable internal
	template <typename TNode>
	struct MetaData final
	{
		// Aliases
		using Node = TNode;
		using ValueType = typename Node::ValueType;
		using KeyType = typename Node::KeyType;
		using IteratorType = typename Node::IteratorType;

		// Fields
		MetaData* Next = nullptr;
		Node* BucketReference = nullptr;
		size_t Index = 0;
		MemStatus Status = MemStatus::Invalid;

		constexpr MetaData(Node* ptr, const size_t index) noexcept
			: Next(nullptr), BucketReference(ptr), Index(index), Status(MemStatus::Valid)
		{
		}

		constexpr void Invalidate() noexcept
		{
			Next = nullptr;
			BucketReference->Invalidate();
			BucketReference = nullptr;
			Index = 0;
			Status = MemStatus::Invalid;
		}

		NODISCARD constexpr bool IsValid() const { return Status == MemStatus::Valid; }
	};

	// HashTable internal
	template <typename T>
	struct HashNode final
	{
		T Value;
		HashNode* Next = nullptr;
		MemStatus Status = MemStatus::Invalid;

		NODISCARD constexpr bool IsValid() const noexcept { return Status == MemStatus::Valid; }
	};
}
