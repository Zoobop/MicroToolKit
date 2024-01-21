#pragma once
#include "Core/Typedef.hpp"
#include "Core/Hash.hpp"
#include "Collections/Base/Enumerable.hpp"
#include "Utility/Node.hpp"
#include "Utility/Tuple.hpp"

namespace Micro
{
	template <typename T>
	using HashFunction = usize(*)(const T&);

	template <typename TKey>
	class SetBase : public Enumerable<TKey>
	{
	public:
		/*
		 *  ============================================================
		 *	|                          Aliases                         |
		 *  ============================================================
		 */


		using Node = HashNode<TKey>;


		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		constexpr SetBase() noexcept { Allocate(DefaultCapacity); }

		constexpr SetBase(const SetBase& hashTable) noexcept
			: m_LoadFactor(hashTable.m_LoadFactor)
		{
			Allocate(hashTable.m_Capacity);
			CopyFrom(hashTable);
		}

		constexpr SetBase(SetBase&& hashTable) noexcept
			: m_Data(std::move(hashTable.m_Data)), m_Size(hashTable.m_Size),
			m_Capacity(hashTable.m_Capacity),
			m_LoadFactor(hashTable.m_LoadFactor)
		{
			hashTable.m_Data = nullptr;
			hashTable.m_Size = 0;
			hashTable.m_Capacity = 0;
			hashTable.m_LoadFactor = 0;
		}

		constexpr SetBase(std::initializer_list<TKey>&& initializerList) noexcept
		{
			const usize length = initializerList.size();
			if (length == 0)
				return;

			Allocate(MAX(length, DefaultCapacity));

			for (auto& e : initializerList)
				Insert(std::move(const_cast<TKey&>(e)));
		}

		constexpr explicit SetBase(const usize capacity) noexcept { Allocate(capacity); }

		constexpr ~SetBase() noexcept override
		{
			Clear();

			delete[] m_Data.Data;

			m_Data = nullptr;
			m_Capacity = 0;
			m_LoadFactor = 0;
		}


		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		constexpr void Rehash(const f32 loadFactor) noexcept
		{
			if (loadFactor <= 0)
				return;

			if (m_LoadFactor - loadFactor < std::numeric_limits<f32>::epsilon())
				return;

			// Clamp value
			m_LoadFactor = MIN(loadFactor, 1.f);

			if (!IsWithinThreshold())
				Reallocate(m_Capacity * 2);
		}

		constexpr void Clear() noexcept
		{
			if (IsEmpty())
				return;

			for (usize i = 0; i < m_Capacity; i++)
			{
				Node* bucket = &m_Data[i];

				DeleteBucket(bucket);
			}

			m_Size = 0;
		}


		/*
		 *  ============================================================
		 *	|                         Accessors                        |
		 *  ============================================================
		 */


		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Size == 0 || m_Data == nullptr; }
		NODISCARD constexpr const Node* Data() const noexcept { return m_Data; }
		NODISCARD constexpr usize Size() const noexcept { return m_Size; }
		NODISCARD constexpr usize Capacity() const noexcept { return m_Capacity; }
		NODISCARD constexpr f32 LoadFactor() const noexcept { return m_LoadFactor; }

		/* Enumerators (Iterators) */

		NODISCARD Enumerator<TKey> GetEnumerator() noexcept override
		{
			for (usize i = 0; i < m_Capacity; i++)
			{
				Node* bucket = &m_Data[i];
				if (!bucket->IsValid())
					continue;

				Node* node = bucket;
				while (node && node->IsValid())
				{
					co_yield node->Value;
					node = node->Next;
				}
			}
		}

		NODISCARD Enumerator<TKey> GetEnumerator() const noexcept override
		{
			for (usize i = 0; i < m_Capacity; i++)
			{
				const Node* bucket = &m_Data[i];
				if (!bucket->IsValid())
					continue;

				Node* node = const_cast<Node*>(bucket);
				while (node && node->IsValid())
				{
					co_yield node->Value;
					node = node->Next;
				}
			}
		}


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		constexpr SetBase& operator=(const SetBase& other) noexcept
		{
			if (this == &other)
				return *this;

			if (!IsEmpty())
			{
				Clear();
				Reallocate(other.m_Capacity);
			}
			else
			{
				Allocate(other.m_Capacity);
			}

			CopyFrom(other);
			return *this;
		}

		constexpr SetBase& operator=(SetBase&& other) noexcept
		{
			if (this == &other)
				return *this;

			Clear();

			m_Data = other.m_Data;
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;
			m_LoadFactor = other.m_LoadFactor;

			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;
			other.m_LoadFactor = 0;

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const SetBase& hashTable) noexcept
		{
			stream << "[";

			const auto data = hashTable.Data();
			const usize capacity = hashTable.Capacity();
			const usize size = hashTable.Size();
			for (usize i = 0, count = 0; i < capacity && count < size; i++)
			{
				const auto bucket = &data[i];
				if (!bucket->IsValid())
					continue;

				stream << bucket->Value;

				if (++count < size)
					stream << ", ";

				auto node = bucket->Next;
				while (node && node->IsValid())
				{
					stream << node->Value;
					node = node->Next;

					if (++count < size)
						stream << ", ";
				}
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
			// Don't allocate if same capacity
			if (m_Capacity == capacity)
				return;

			m_Data = new Node[capacity];
			m_Capacity = capacity;
		}

		constexpr void Reallocate(const usize capacity) noexcept
		{
			// Don't reallocate if same capacity
			if (m_Capacity == capacity)
				return;

			// Allocate new block
			auto oldBlock = m_Data;
			const usize oldCapacity = m_Capacity;

			m_Data = new Node[capacity];
			m_Capacity = capacity;
			m_Size = 0;

			// Loop through all buckets and re-hash
			for (usize i = 0; i < oldCapacity; i++)
			{
				Node* bucket = &oldBlock[i];
				if (!bucket->IsValid())
					continue;

				// Insert bucket head value, then linked nodes
				Insert(std::move(bucket->Value));

				Node* node = bucket->Next;
				while (node && node->IsValid())
				{
					Insert(std::move(node->Value));
					node = node->Next;
				}
			}

			// Delete the old block
			for (usize i = 0; i < oldCapacity; i++)
			{
				Node* bucket = &oldBlock[i];
				if (!bucket->IsValid())
					continue;

				// Delete all in bucket
				DeleteBucket(bucket);
			}

			delete[] oldBlock;
		}

		constexpr void CopyFrom(const SetBase& other) noexcept
		{
			m_Size = 0;
			m_LoadFactor = other.m_LoadFactor;

			auto data = other.m_Data;
			for (usize i = 0; i < other.m_Capacity; i++)
			{
				Node* bucket = &data[i];
				if (!bucket->IsValid())
					continue;

				Insert(bucket->Value);

				Node* node = bucket->Next;
				while (node && node->IsValid())
				{
					Insert(node->Value);
					node = node->Next;
				}
			}

		}

		constexpr bool Insert(const TKey& key) noexcept
		{
			// Reallocate if necessary
			if (!IsWithinThreshold())
				Reallocate(m_Capacity * 2);

			// Get hash value
			const usize hash = Micro::Hash(key) % m_Capacity;

			// Check if bucket is valid
			auto bucket = &m_Data[hash];
			if (bucket->IsValid())
			{
				// Check bucket value for duplicate
				if (bucket->Value == key)
					return false;

				// Get to the end of the chain (check for existing value)
				auto node = bucket;
				while (node->Next)
				{
					if (node->Value == key)
						return false;

					node = node->Next;
				}

				// Create new node
				++m_Size;
				node->Next = new Node{ key };
				return true;
			}

			// Emplace value
			++m_Size;
			new(&m_Data[hash]) Node{ key };
			return true;
		}

		constexpr bool Insert(TKey&& key) noexcept
		{
			// Reallocate if necessary
			if (!IsWithinThreshold())
				Reallocate(m_Capacity * 2);

			// Get hash value
			const usize hash = Micro::Hash(key) % m_Capacity;

			// Check if bucket is valid
			auto bucket = &m_Data[hash];
			if (bucket->IsValid())
			{
				// Check bucket value for duplicate
				if (bucket->Value == key)
					return false;

				// Get to the end of the chain (check for existing value)
				auto node = bucket;
				while (node->Next != nullptr)
				{
					if (node->Value == key)
						return false;

					node = node->Next;
				}

				// Create new node
				++m_Size;
				node->Next = new Node{ std::move(key) };
				return true;
			}

			// Emplace value
			++m_Size;
			new(&m_Data[hash]) Node{ std::move(key) };
			return true;
		}

		constexpr bool Erase(const TKey& key) noexcept
		{
			// Get hash value
			const usize hash = Micro::Hash(key) % m_Capacity;

			// Check if bucket is valid
			auto bucket = &m_Data[hash];
			if (bucket->IsValid())
			{
				// Search for node with existing value
				if (bucket->Value == key)
				{
					// Dispose and shift nodes down (if applicable)
					if (bucket->Next != nullptr)
					{
						auto prev = bucket;
						auto node = prev->Next;
						while (node->Next != nullptr)
						{
							prev->Value = std::move(node->Value);

							prev = node;
							node = node->Next;
						}

						// Clean up tail references
						prev->Next = nullptr;
						delete node;
					}
					else
					{
						bucket->Dispose();
					}

					--m_Size;
					return true;
				}

				auto node = bucket;
				while (node->Next)
				{
					auto next = node->Next;
					if (next->Value == key)
					{
						// Move next pointer
						node->Next = next->Next;

						// Remove node
						next->Next = nullptr;
						delete next;

						--m_Size;
						return true;
					}

					node = next;
				}
			}

			return false;
		}

		NODISCARD constexpr bool IsWithinThreshold() const noexcept
		{
			return (static_cast<f64>(m_Size) / static_cast<f64>(m_Capacity)) < m_LoadFactor;
		}

	private:
		constexpr static void DeleteBucket(Node* head) noexcept
		{
			// Clear node chain
			auto node = head->Next;
			while (node != nullptr)
			{
				auto next = node->Next;

				node->Next = nullptr;
				delete node;

				node = next;
			}

			head->Dispose();
		}

	protected:
		Memory<Node> m_Data = nullptr;
		usize m_Size = 0;
		usize m_Capacity = 0;
		f32 m_LoadFactor = 0.9f;

	private:
		constexpr static usize DefaultCapacity = 25;
	};

	template <typename TKey, typename TValue>
	struct KeyValuePair final
	{
		TKey Key;
		TValue Value;

		constexpr KeyValuePair() noexcept = default;

		constexpr KeyValuePair(const KeyValuePair& keyValuePair) noexcept
			: Key(keyValuePair.Key), Value(keyValuePair.Value)
		{
		}

		constexpr KeyValuePair(KeyValuePair&& keyValuePair) noexcept
			: Key(std::move(keyValuePair.Key)), Value(std::move(keyValuePair.Value))
		{
		}

		constexpr KeyValuePair(const TKey& key, const TValue& value) noexcept
			: Key(key), Value(value)
		{
		}

		constexpr KeyValuePair(const TKey& key, TValue&& value) noexcept
			: Key(key), Value(std::move(value))
		{
		}

		constexpr KeyValuePair(TKey&& key, const TValue& value) noexcept
			: Key(std::move(key)), Value(value)
		{
		}

		constexpr KeyValuePair(TKey&& key, TValue&& value) noexcept
			: Key(std::move(key)), Value(std::move(value))
		{
		}

		constexpr ~KeyValuePair() noexcept = default;

		constexpr KeyValuePair& operator=(const KeyValuePair&) noexcept = default;
		constexpr KeyValuePair& operator=(KeyValuePair&&) noexcept = default;

		friend std::ostream& operator<<(std::ostream& stream, const KeyValuePair& keyValuePair) noexcept
		{
			stream << "{ " << keyValuePair.Key << " : " << keyValuePair.Value << " }";
			return stream;
		}
	};

	template <typename TKey, typename TValue>
	class MapBase : public Enumerable<KeyValuePair<TKey, TValue>>
	{
	public:
		/*
		 *  ============================================================
		 *	|                          Aliases                         |
		 *  ============================================================
		 */


		using Pair = KeyValuePair<TKey, TValue>;
		using Node = HashNode<Pair>;


		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		constexpr MapBase() noexcept { Allocate(DefaultCapacity); }

		constexpr MapBase(const MapBase& hashTable) noexcept
			: m_LoadFactor(hashTable.m_LoadFactor)
		{
			Allocate(hashTable.m_Capacity);
			CopyFrom(hashTable);
		}

		constexpr MapBase(MapBase&& hashTable) noexcept
			: m_Data(std::move(hashTable.m_Data)), m_Size(hashTable.m_Size),
			m_Capacity(hashTable.m_Capacity),
			m_LoadFactor(hashTable.m_LoadFactor)
		{
			hashTable.m_Data = nullptr;
			hashTable.m_Size = 0;
			hashTable.m_Capacity = 0;
			hashTable.m_LoadFactor = 0;
		}

		constexpr MapBase(std::initializer_list<Pair>&& initializerList) noexcept
		{
			const usize length = initializerList.size();
			if (length == 0)
				return;

			Allocate(MAX(length, DefaultCapacity));

			for (auto& e : initializerList)
				Insert(std::move(const_cast<Pair&>(e)));
		}

		constexpr explicit MapBase(const usize capacity) noexcept { Allocate(capacity); }

		constexpr ~MapBase() noexcept override
		{
			Clear();

			delete[] m_Data.Data;

			m_Data = nullptr;
			m_Capacity = 0;
			m_LoadFactor = 0;
		}


		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		constexpr void Rehash(const f32 loadFactor) noexcept
		{
			if (loadFactor <= 0)
				return;

			if (m_LoadFactor - loadFactor < std::numeric_limits<f32>::epsilon())
				return;

			// Clamp value
			m_LoadFactor = MIN(loadFactor, 1.f);

			if (!IsWithinThreshold())
				Reallocate(m_Capacity * 2);
		}

		constexpr void Clear() noexcept
		{
			if (IsEmpty())
				return;

			for (usize i = 0; i < m_Capacity; i++)
			{
				Node* bucket = &m_Data[i];

				DeleteBucket(bucket);
			}

			m_Size = 0;
		}


		/*
		 *  ============================================================
		 *	|                         Accessors                        |
		 *  ============================================================
		 */


		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Size == 0 || m_Data == nullptr; }
		NODISCARD constexpr const Node* Data() const noexcept { return m_Data; }
		NODISCARD constexpr usize Size() const noexcept { return m_Size; }
		NODISCARD constexpr usize Capacity() const noexcept { return m_Capacity; }
		NODISCARD constexpr f32 LoadFactor() const noexcept { return m_LoadFactor; }

		/* Enumerators (Iterators) */

		NODISCARD Enumerator<Pair> GetEnumerator() noexcept override
		{
			for (usize i = 0; i < m_Capacity; i++)
			{
				Node* bucket = &m_Data[i];
				if (!bucket->IsValid())
					continue;

				Node* node = bucket;
				while (node && node->IsValid())
				{
					co_yield node->Value;
					node = node->Next;
				}
			}
		}

		NODISCARD Enumerator<Pair> GetEnumerator() const noexcept override
		{
			for (usize i = 0; i < m_Capacity; i++)
			{
				const Node* bucket = &m_Data[i];
				if (!bucket->IsValid())
					continue;

				Node* node = const_cast<Node*>(bucket);
				while (node && node->IsValid())
				{
					co_yield node->Value;
					node = node->Next;
				}
			}
		}


		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		constexpr MapBase& operator=(const MapBase& other) noexcept
		{
			if (this == &other)
				return *this;

			if (!IsEmpty())
			{
				Clear();
				Reallocate(other.m_Capacity);
			}
			else
			{
				Allocate(other.m_Capacity);
			}

			CopyFrom(other);
			return *this;
		}

		constexpr MapBase& operator=(MapBase&& other) noexcept
		{
			if (this == &other)
				return *this;

			Clear();

			m_Data = other.m_Data;
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;
			m_LoadFactor = other.m_LoadFactor;

			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;
			other.m_LoadFactor = 0;

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const MapBase& hashTable) noexcept
		{
			stream << "[";

			const auto data = hashTable.Data();
			const usize capacity = hashTable.Capacity();
			const usize size = hashTable.Size();
			for (usize i = 0, count = 0; i < capacity && count < size; i++)
			{
				const auto bucket = &data[i];
				if (!bucket->IsValid())
					continue;

				stream << bucket->Value;

				if (++count < size)
					stream << ", ";

				auto node = bucket->Next;
				while (node && node->IsValid())
				{
					stream << node->Value;
					node = node->Next;

					if (++count < size)
						stream << ", ";
				}
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
			// Don't allocate if same capacity
			if (m_Capacity == capacity)
				return;

			m_Data = new Node[capacity];
			m_Capacity = capacity;
		}

		constexpr void Reallocate(const usize capacity) noexcept
		{
			// Don't reallocate if same capacity
			if (m_Capacity == capacity)
				return;

			// Allocate new block
			auto oldBlock = m_Data;
			const usize oldCapacity = m_Capacity;

			m_Data = new Node[capacity];
			m_Capacity = capacity;
			m_Size = 0;

			// Loop through all buckets and re-hash
			for (usize i = 0; i < oldCapacity; i++)
			{
				Node* bucket = &oldBlock[i];
				if (!bucket->IsValid())
					continue;

				// Insert bucket head value, then linked nodes
				Insert(std::move(bucket->Value));

				Node* node = bucket->Next;
				while (node && node->IsValid())
				{
					Insert(std::move(node->Value));
					node = node->Next;
				}
			}

			// Delete the old block
			for (usize i = 0; i < oldCapacity; i++)
			{
				Node* bucket = &oldBlock[i];
				if (!bucket->IsValid())
					continue;

				// Delete all in bucket
				DeleteBucket(bucket);
			}

			delete[] oldBlock;
		}

		constexpr void CopyFrom(const MapBase& other) noexcept
		{
			m_Size = 0;
			m_LoadFactor = other.m_LoadFactor;

			auto data = other.m_Data;
			for (usize i = 0; i < other.m_Capacity; i++)
			{
				Node* bucket = &data[i];
				if (!bucket->IsValid())
					continue;

				Insert(bucket->Value);

				Node* node = bucket->Next;
				while (node && node->IsValid())
				{
					Insert(node->Value);
					node = node->Next;
				}
			}

		}

		constexpr bool Insert(const Pair& keyValuePair) noexcept
		{
			// Reallocate if necessary
			if (!IsWithinThreshold())
				Reallocate(m_Capacity * 2);

			// Get hash value
			const usize hash = Micro::Hash(keyValuePair.Key) % m_Capacity;

			// Check if bucket is valid
			auto bucket = &m_Data[hash];
			if (bucket->IsValid())
			{
				// Check bucket value for duplicate
				if (bucket->Value.Key == keyValuePair.Key)
					return false;

				// Get to the end of the chain (check for existing value)
				auto node = bucket;
				while (node->Next != nullptr)
				{
					if (node->Value.Value == keyValuePair.Value)
						return false;

					node = node->Next;
				}

				// Create new node
				++m_Size;
				node->Next = new Node{ keyValuePair };
				return true;
			}

			// Emplace value
			++m_Size;
			new(&m_Data[hash]) Node{ keyValuePair };
			return true;
		}

		constexpr bool Insert(Pair&& keyValuePair) noexcept
		{
			// Reallocate if necessary
			if (!IsWithinThreshold())
				Reallocate(m_Capacity * 2);

			// Get hash value
			const usize hash = Micro::Hash(keyValuePair.Key) % m_Capacity;

			// Check if bucket is valid
			auto bucket = &m_Data[hash];
			if (bucket->IsValid())
			{
				// Check bucket value for duplicate
				if (bucket->Value.Key == keyValuePair.Key)
					return false;

				// Get to the end of the chain (check for existing value)
				auto node = bucket;
				while (node->Next != nullptr)
				{
					if (node->Value.Value == keyValuePair.Value)
						return false;

					node = node->Next;
				}

				// Create new node
				++m_Size;
				node->Next = new Node{ std::move(keyValuePair) };
				return true;
			}

			// Emplace value
			++m_Size;
			new(&m_Data[hash]) Node{ std::move(keyValuePair) };
			return true;
		}

		constexpr bool Erase(const TKey& key) noexcept
		{
			// Get hash value
			const usize hash = Micro::Hash(key) % m_Capacity;

			// Check if bucket is valid
			auto bucket = &m_Data[hash];
			if (bucket->IsValid())
			{
				// Search for node with existing value
				if (bucket->Value.Key == key)
				{
					// Dispose and shift nodes down (if applicable)
					if (bucket->Next)
					{
						auto prev = bucket;
						auto node = prev->Next;
						while (node->Next != nullptr)
						{
							prev->Value = std::move(node->Value);

							prev = node;
							node = node->Next;
						}

						// Clean up tail references
						prev->Next = nullptr;
						delete node;
					}
					else
					{
						bucket->Dispose();
					}

					--m_Size;
					return true;
				}

				auto node = bucket;
				while (node->Next)
				{
					auto next = node->Next;
					if (next->Value.Key == key)
					{
						// Move next pointer
						node->Next = next->Next;

						// Remove node
						next->Next = nullptr;
						delete next;

						--m_Size;
						return true;
					}

					node = next;
				}
			}

			return false;
		}

		NODISCARD constexpr bool IsWithinThreshold() const noexcept
		{
			return (static_cast<f64>(m_Size) / static_cast<f64>(m_Capacity)) < m_LoadFactor;
		}

	private:
		constexpr static void DeleteBucket(Node* head) noexcept
		{
			// Clear node chain
			auto node = head->Next;
			while (node != nullptr)
			{
				auto next = node->Next;

				node->Next = nullptr;
				delete node;

				node = next;
			}

			head->Dispose();
		}

	protected:
		Memory<Node> m_Data = nullptr;
		usize m_Size = 0;
		usize m_Capacity = 0;
		f32 m_LoadFactor = 0.9f;

	private:
		constexpr static usize DefaultCapacity = 25;
	};
}
