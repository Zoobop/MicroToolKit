#pragma once

#include "Utility/Tuple.hpp"
#include "Core/Memory/Memory.hpp"
#include "Collections/Base/HashTable.hpp"

namespace Micro
{
	template <typename TKey, typename TValue>
	struct MapNode final
	{
		// Aliases
		using KeyType = TKey;
		using ValueType = TValue;
		using IteratorType = Tuple<TKey, TValue>;
		using KeyValuePair = IteratorType;

		// Fields
		KeyValuePair Value;
		MapNode* Next = nullptr;
		MemStatus Status = MemStatus::Invalid;

		// Constructors
		constexpr MapNode() noexcept = default;

		constexpr MapNode(const MapNode& other) noexcept
			: Value(other.Value), Next(other.Next), Status(other.Status)
		{
		}

		constexpr MapNode(MapNode&& other) noexcept
			: Value(std::move(other.Value)), Next(other.Next), Status(other.Status)
		{
			other.Invalidate();
		}

		explicit constexpr MapNode(const KeyValuePair& value) noexcept
			: Value(value), Status(MemStatus::Valid)
		{
		}

		explicit constexpr MapNode(KeyValuePair&& value) noexcept
			: Value(std::move(value)), Status(MemStatus::Valid)
		{
		}

		constexpr ~MapNode() noexcept { Invalidate(); }

		// Utility
		constexpr void Invalidate() noexcept
		{
			Next = nullptr;
			Status = MemStatus::Invalid;
		}

		constexpr void Dispose() noexcept
		{
			Value.~KeyValuePair();
		}

		NODISCARD constexpr TKey& GetKey() noexcept { return Value.Component1; }
		NODISCARD constexpr TValue& GetValue() noexcept { return Value.Component2; }
		NODISCARD constexpr IteratorType& GetIteratorValue() noexcept { return Value; }
		NODISCARD constexpr bool IsValid() const noexcept { return Status == MemStatus::Valid; }

		// Operator Overloads
		MapNode& operator=(const MapNode& other) noexcept
		{
			if (this == &other)
				return *this;

			Value = other.Value;
			Next = other.Next;
			Status = other.Status;

			return *this;
		}

		MapNode& operator=(MapNode&& other) noexcept
		{
			if (this == &other)
				return *this;

			Value = std::move(other.Value);
			Next = other.Next;
			Status = other.Status;

			return *this;
		}
	};

	template <typename TKey, typename TValue>
	class Map final : public HashTable<Tuple<TKey, TValue>>
	{
	public:
		// Aliases
		using Node = MapNode<TKey, TValue>;
		using Base = HashTable<Node>;
		using KeyValuePair = Tuple<TKey, TValue>;

		// Constructors/Destructor
		constexpr Map() noexcept : Base()
		{
		}

		constexpr Map(const Map& other) : Base(other)
		{
		}

		constexpr Map(Map&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Map(const Base& other) : Base(other)
		{
		}

		constexpr Map(Base&& other) noexcept : Base(std::move(other))
		{
		}

		constexpr Map(std::initializer_list<KeyValuePair>&& initializerList) noexcept : Base(std::move(initializerList))
		{
		}

		constexpr explicit Map(const size_t capacity) : Base(capacity)
		{
		}

		constexpr ~Map() noexcept override = default;

		// Utility
		bool Add(const TKey& key, const TValue& value) noexcept
		{
			return Base::Insert({ key, value });
		}

		bool Add(TKey&& key, TValue&& value) noexcept
		{
			return Base::Insert({std::move(key), std::move(value)});
		}

		bool Add(const KeyValuePair& pair) noexcept
		{
			return Base::Insert(pair);
		}

		bool Add(KeyValuePair&& pair) noexcept
		{
			return Base::Insert(std::move(pair));
		}

		bool Remove(const TKey& key) noexcept
		{
			return Base::Erase(key);
		}

		NODISCARD const KeyValuePair& Find(const TKey& key) const
		{
			// Gets the hashed value of the key
			const size_t hash = Hash(key) % Base::m_Capacity;

			// Search bucket for matching key
			auto bucket = &Base::m_Data[hash];
			while (bucket != nullptr && bucket->IsValid())
			{
				if (bucket->Value.Component1 == key)
					return bucket->Value;

				bucket = bucket->Next;
			}

			// Throw exception if key is not found (key hashes to valid index but is not present)
			throw KeyNotFoundError("Key could not be found in the Map.", NAMEOF(key));
		}

		NODISCARD KeyValuePair& Find(const TKey& key)
		{
			// Gets the hashed value of the key
			const size_t hash = Hash(key) % Base::m_Capacity;

			// Search bucket for matching key
			auto bucket = &Base::m_Data[hash];
			while (bucket != nullptr && bucket->IsValid())
			{
				if (bucket->GetKey() == key)
					return bucket->Value;

				bucket = bucket->Next;
			}

			// Throw exception if key is not found (key hashes to valid index but is not present)
			throw KeyNotFoundError("Key could not be found in the Map.", NAMEOF(key));
		}

		NODISCARD Optional<KeyValuePair> Find(const Predicate<const TKey&> predicate) const noexcept
		{
			auto metaData = Base::m_MetaData;
			while (metaData != nullptr)
			{
				// Search bucket for matching key
				auto bucket = metaData->BucketReference;
				while (bucket != nullptr && bucket->IsValid())
				{
					if (predicate(bucket->Value.Component1))
						return RefResult(bucket->Value);

					bucket = bucket->Next;
				}
			}

			return Optional<KeyValuePair>::Empty();
		}

		NODISCARD bool ContainsKey(const TKey& key) const noexcept
		{
			// Get hash of bucket
			auto bucket = &Base::m_Data[Hash(key) % Base::m_Capacity];
			if (!bucket->IsValid())
				return false;

			// Loop through bucket for matching value
			while (bucket != nullptr)
			{
				if (bucket->Value.Component1 == key)
					return true;

				bucket = bucket->Next;
			}

			return false;
		}

		NODISCARD bool ContainsValue(const TValue& value) const noexcept
		{
			auto metaData = Base::m_MetaData;
			while (metaData != nullptr)
			{
				auto bucket = metaData->BucketReference;
				while (bucket != nullptr && bucket->IsValid())
				{
					if (bucket->GetValue() == value)
						return true;

					bucket = bucket->Next;
				}

				metaData = metaData->Next;
			}
			return false;
		}

		NODISCARD bool TryGetValue(const TKey& key, TValue& out) noexcept
		{
			// Get hash of bucket
			auto bucket = &Base::m_Data[Hash(key) % Base::m_Capacity];
			if (!bucket->IsValid())
				return false;

			// Loop through bucket for matching value
			while (bucket != nullptr)
			{
				if (bucket->GetKey() == key)
				{
					out = bucket->GetValue();
					return true;
				}

				bucket = bucket->Next;
			}

			return false;
		}

		// Operator Overloads
		NODISCARD const TValue& operator[](const TKey& key) const
		{
			const size_t hash = Hash(key) % Base::m_Capacity;
			const auto& pair = Base::m_Data[hash];

			if (!pair.IsValid())
				throw KeyNotFoundError("Key could not be found in the Map.", NAMEOF(key));

			return pair.Value.Component2;
		}

		NODISCARD TValue& operator[](const TKey& key)
		{
			const size_t hash = Hash(key) % Base::m_Capacity;
			auto& pair = Base::m_Data[hash];

			if (!pair.IsValid())
				throw KeyNotFoundError("Key could not be found in the Map.", NAMEOF(key));

			return pair.Value.Component2;
		}

		Map& operator=(const Map& other)
		{
			if (this == &other)
				return *this;

			Base::Clear();

			Base::CopyFrom(other);

			return *this;
		}

		Map& operator=(Map&& other) noexcept
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
