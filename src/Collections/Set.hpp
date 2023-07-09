#pragma once

#include "Base/Sequence.hpp"
#include "Collections/Base/HashTable.hpp"

namespace Micro
{
	template <Hashable T>
	struct SetNode final
	{
		// Aliases
		using KeyType = T;
		using ValueType = T;

		// Fields
		T Value;
		SetNode* Next = nullptr;
		MemStatus Status = MemStatus::Invalid;

		// Constructors
		constexpr SetNode() noexcept = default;

		constexpr SetNode(const SetNode& other) noexcept
			: Value(other.Value), Next(other.Next), Status(other.Status)
		{
		}

		constexpr SetNode(SetNode&& other) noexcept
			: Value(std::move(other.Value)), Next(other.Next), Status(other.Status)
		{
			other.Invalidate();
		}

		explicit constexpr SetNode(const T& value) noexcept
			: Value(value), Status(MemStatus::Valid)
		{
		}

		explicit constexpr SetNode(T&& value) noexcept
			: Value(std::move(value)), Status(MemStatus::Valid)
		{
		}

		constexpr ~SetNode() noexcept { Invalidate(); }

		// Utility
		constexpr void Invalidate() noexcept
		{
			Next = nullptr;
			Status = MemStatus::Invalid;
		}

		constexpr void Dispose() noexcept
		{
			Value.~T();
		}

		NODISCARD constexpr T& GetKey() noexcept { return Value; }
		NODISCARD constexpr T& GetValue() noexcept { return Value; }
		NODISCARD constexpr bool IsValid() const noexcept { return Status == MemStatus::Valid; }

		// Operator Overloads
		SetNode& operator=(const SetNode& other) noexcept
		{
			if (this == &other)
				return *this;

			Value = other.Value;
			Next = other.Next;
			Status = other.Status;

			return *this;
		}

		SetNode& operator=(SetNode&& other) noexcept
		{
			if (this == &other)
				return *this;

			Value = std::move(other.Value);
			Next = other.Next;
			Status = other.Status;

			return *this;
		}
	};

	template <Hashable T>
	class Set final : public HashTable<SetNode<T>>
	{
	public:
		// Aliases
		using Base = HashTable<SetNode<T>>;
		using Node = SetNode<T>;

		// Constructors/Destructor
		constexpr Set() noexcept : Base()
		{
		}

		constexpr Set(const Set& other) : Base(other)
		{
		}

		constexpr Set(Set&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Set(const Base& other) : Base(other)
		{
		}

		constexpr Set(Base&& other) noexcept : Base(std::move(other))
		{
		}

		Set(std::initializer_list<T>&& initializerList) noexcept : Base(std::move(initializerList))
		{
		}

		explicit Set(const size_t capacity) : Base(capacity)
		{
		}

		constexpr ~Set() noexcept override = default;

		// Utility
		bool Add(const T& value)
		{
			return Base::Insert(value);
		}

		bool Add(T&& value)
		{
			return Base::Insert(std::move(value));
		}

		bool Remove(const T& value)
		{
			// Get hash value
			const size_t hash = Hash(value) % Base::m_Capacity;
			++Base::m_Size;

			// Check if bucket is valid
			auto bucket = &Base::m_Data[hash];
			if (bucket->IsValid())
			{
				// Search for node with existing value
				auto node = bucket;
				while (node->Next != nullptr)
				{
					auto next = node->Next;
					if (next->Value == value)
					{
						// Move next pointer
						node->Next = next->Next;

						// Remove node
						next->Invalidate();

						delete next;

						--Base::m_Size;
						return true;
					}

					node = next;
				}
			}

			return false;
		}

		NODISCARD bool Overlaps(const Set& other) const
		{
			auto node = other.m_MetaData;
			while (node != nullptr)
			{
				if (Contains(node->Ptr->GetValue()))
					return true;

				node = node->Next;
			}
			return false;
		}

		NODISCARD void IntersectWith(const Set& other)
		{
			auto node = other.m_MetaData;
			while (node != nullptr)
			{
				if (!Contains(node->Ptr->GetValue()))
					continue;

				Remove(node->Ptr->GetValue());
				node = node->Next;
			}
		}

		NODISCARD void UnionWith(const Set& other)
		{
			auto node = other.m_MetaData;
			while (node != nullptr)
			{
				Add(node->Ptr->GetValue());
				node = node->Next;
			}
		}

		NODISCARD bool Contains(const T& value) const noexcept
		{
			auto bucket = &Base::m_Data[Hash(value) % Base::m_Capacity];
			while (bucket != nullptr)
			{
				if (bucket->Value == value)
					return true;

				bucket = bucket->Next;
			}

			return false;
		}

		// Operator Overloads
		Set& operator=(const Set& other)
		{
			if (this == &other)
				return *this;

			Base::Clear();

			Base::CopyFrom(other);

			return *this;
		}

		Set& operator=(Set&& other) noexcept
		{
			if (this == &other)
				return *this;

			Base::Clear();

			Base::m_Data = other.m_Data;
			Base::m_MetaData = other.m_MetaData;
			Base::m_Capacity = other.m_Capacity;
			Base::m_Size = other.m_Size;
			Base::m_LoadFactor = other.m_LoadFactor;

			other.m_Data = nullptr;
			other.m_MetaData = nullptr;
			other.m_Capacity = 0;
			other.m_Size = 0;
			other.m_LoadFactor = 0;
			return *this;
		}
	};
}
