#pragma once

#include "MicroToolKit/src/Common/Span.hpp"
#include "Collections/Base/HashTable.hpp"

namespace Micro
{
	template <Hashable T>
	struct SetNode final
	{
		// Aliases
		using KeyType = T;
		using ValueType = T;
		using IteratorType = T;

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
		NODISCARD constexpr IteratorType& GetIteratorValue() noexcept { return Value; }
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
		using Node = SetNode<T>;
		using Base = HashTable<Node>;

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

		constexpr Set(std::initializer_list<T>&& initializerList) noexcept : Base(std::move(initializerList))
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
			return Base::Erase(value);
		}

		NODISCARD bool Overlaps(const Set& other) const
		{
			if (Base::IsEmpty())
				return false;

			auto metaData = other.m_MetaData;
			while (metaData != nullptr)
			{
				auto bucket = metaData->BucketReference;
				while (bucket != nullptr && bucket->IsValid())
				{
					if (Contains(bucket->Value))
						return true;

					bucket = bucket->Next;
				}

				metaData = metaData->Next;
			}
			return true;
		}

		NODISCARD bool SetEquals(const Set& other) const
		{
			if (Base::m_Size != other.m_Size)
				return false;

			auto metaData = other.m_MetaData;
			while (metaData != nullptr)
			{
				auto bucket = metaData->BucketReference;
				while (bucket != nullptr && bucket->IsValid())
				{
					if (!Contains(bucket->Value))
						return false;

					bucket = bucket->Next;
				}

				metaData = metaData->Next;
			}
			return true;
		}

		NODISCARD bool Contains(const T& value) const noexcept
		{
			// Get hash of bucket
			auto bucket = &Base::m_Data[Hash(value) % Base::m_Capacity];
			if (!bucket->IsValid())
				return false;

			// Loop through bucket for matching value
			while (bucket != nullptr)
			{
				if (bucket->Value == value)
					return true;

				bucket = bucket->Next;
			}

			return false;
		}

		void IntersectWith(const Set& other)
		{
			auto metaData = Base::m_MetaData;
			while (metaData != nullptr)
			{
				const auto next = metaData->Next;
				auto bucket = metaData->BucketReference;
				while (bucket != nullptr && bucket->IsValid())
				{
					const auto nextNode = bucket->Next;
					const auto& value = bucket->Value;
					if (!other.Contains(value))
						Remove(value);

					bucket = nextNode;
				}

				metaData = next;
			}
		}

		void UnionWith(const Set& other)
		{
			auto metaData = other.m_MetaData;
			while (metaData != nullptr)
			{
				auto bucket = metaData->BucketReference;
				while (bucket != nullptr && bucket->IsValid())
				{
					Add(bucket->Value);
					bucket = bucket->Next;
				}

				metaData = metaData->Next;
			}
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
