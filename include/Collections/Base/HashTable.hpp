#pragma once
#include "Core/Hash.hpp"
#include "Collections/Base/Internal/HashTableInternal.hpp"
#include "Collections/Base/Enumerable.hpp"

namespace Micro
{
	template <typename T>
	class HashTableAllocator final
	{
	public:
		using Node = Internal::HashNode<T>;
		using MetaData = Internal::MetaData<Node>;
		using Memory = Memory<Node>;

		NODISCARD constexpr static size_t Allocate(Memory& data, const size_t currentCapacity, const size_t newCapacity) noexcept
		{
			// Don't allocate if same capacity
			if (currentCapacity == newCapacity)
				return currentCapacity;

			data = Alloc<Node>(newCapacity);
			return newCapacity;
		}

		NODISCARD constexpr static size_t Reallocate(Memory& data, MetaData*& metaData, const size_t currentCapacity,
		                                   const size_t newCapacity) noexcept
		{
			// Don't reallocate if same capacity
			if (currentCapacity == newCapacity)
				return currentCapacity;

			// Allocate (without constructor call)
			Node* newBlock = Alloc<Node>(newCapacity);
			MetaData* newMetaData = nullptr;

			// Re-hash using meta data
			auto metaNode = metaData;
			while (metaNode != nullptr)
			{
				auto valueNode = metaNode->BucketReference;
				while (valueNode != nullptr)
				{
					// Get new hash of value
					const size_t hash = Hash(valueNode->GetKey()) % newCapacity;

					// Check if bucket is valid
					auto bucket = &newBlock[hash];
					if (bucket->IsValid())
					{
						// Get to the end of the chain
						auto node = bucket;
						while (node->Next != nullptr)
							node = node->Next;

						// Create new node
						auto newNode = Alloc<Node>(1);
						new(newNode) Node(std::move(valueNode->Value));

						node->Next = newNode;

						// Update meta data with new pointer reference
						AddMetaData(newMetaData, newNode, hash);
					}
					else
					{
						// Move value into new block
						new(&newBlock[hash]) Node(std::move(valueNode->Value));

						// Update meta data with new pointer reference
						AddMetaData(newMetaData, &newBlock[hash], hash);
					}

					valueNode = valueNode->Next;
				}

				metaNode = metaNode->Next;
			}

			// Invalidate memory
			ClearMemory(metaData);

			// Free invalid memory
			Dispose(data, currentCapacity);

			data = newBlock;
			metaData = newMetaData;
			return newCapacity;
		}

		static void ClearMemory(MetaData* metaData) noexcept
		{
			// Clear meta data
			if (metaData == nullptr)
				return;

			auto metaDataIter = metaData;
			while (metaDataIter != nullptr)
			{
				// Delete children nodes
				DeleteNodeChildren(metaDataIter->BucketReference);

				// Free bucket head
				auto next = metaDataIter->Next;
				metaDataIter->BucketReference->Dispose();
				metaDataIter->Invalidate();

				metaDataIter = next;
			}
		}

		constexpr static void Dispose(Memory& data, const size_t capacity) noexcept
		{
			static_assert(capacity > 0);
			Delete(data.Data, capacity);
		}

		static void AddMetaData(MetaData*& metaData, Node* node, const size_t hash) noexcept
		{
			// Initial node
			if (metaData == nullptr)
			{
				metaData = Alloc<MetaData>(1);
				new(metaData) MetaData(node, hash);
				return;
			}

			// Subsequent nodes...
			auto metaDataNode = metaData;
			while (metaDataNode->Next != nullptr)
				metaDataNode = metaDataNode->Next;

			auto newNode = Alloc<MetaData>(1);
			new(newNode) MetaData(node, hash);

			metaDataNode->Next = newNode;
		}

		static void RemoveMetaData(MetaData*& metaData, Node* node) noexcept
		{
			// Initial node
			if (metaData == nullptr)
				return;

			// Search for existing node
			if (metaData->BucketReference != node)
				return;

			// Remove bucket
			auto head = metaData;
			metaData = metaData->Next;

			head->Invalidate();
			delete head;
		}

	private:
		constexpr static void DeleteNodeChildren(Node* head) noexcept
		{
			// Clear node chain
			auto node = head->Next;
			while (node != nullptr)
			{
				auto next = node->Next;

				node->Invalidate();
				delete node;

				node = next;
			}

			head->Next = nullptr;
		}
	};


	template <typename T>
	class HashTable : public Enumerable<T>
	{
	public:
		/*
		 *  ============================================================
		 *	|                          Aliases                         |
		 *  ============================================================
		 */


		using Node = Internal::HashNode<T>;
		using MetaData = Internal::MetaData<Node>;
		using Memory = Memory<Node>;
		using Allocator = HashTableAllocator<T>;

		
		/*
		 *  ============================================================
		 *	|                  Constructors/Destructors                |
		 *  ============================================================
		 */


		constexpr HashTable() noexcept { Allocate(DefaultCapacity);	}

		constexpr HashTable(const HashTable& hashTable) noexcept
			: m_LoadFactor(hashTable.m_LoadFactor)
		{
			Allocate(hashTable.m_Capacity);
			CopyFrom(hashTable);
		}

		constexpr HashTable(HashTable&& hashTable) noexcept
			: m_Data(std::move(hashTable.m_Data)), m_MetaData(hashTable.m_MetaData), m_Size(hashTable.m_Size),
			  m_Capacity(hashTable.m_Capacity),
			  m_LoadFactor(hashTable.m_LoadFactor)
		{
			hashTable.m_Data = nullptr;
			hashTable.m_MetaData = nullptr;
			hashTable.m_Size = 0;
			hashTable.m_Capacity = 0;
			hashTable.m_LoadFactor = 0;
		}

		constexpr explicit HashTable(std::initializer_list<T>&& initializerList) noexcept
		{
			const size_t length = initializerList.size();
			if (length == 0)
				return;

			Allocate(MAX(length, DefaultCapacity));

			for (auto& e : initializerList)
				Insert(std::move(const_cast<T&>(e)));
		}

		constexpr explicit HashTable(const size_t capacity) noexcept { Allocate(capacity); }

		constexpr ~HashTable() noexcept override
		{
			Allocator::ClearMemory(m_MetaData);
			Allocator::Dispose(m_Data, m_Capacity);
			m_Data = nullptr;
			m_MetaData = nullptr;
			m_Size = 0;
			m_Capacity = 0;
			m_LoadFactor = 0;
		}


		/*
		 *  ============================================================
		 *	|                         Utility                          |
		 *  ============================================================
		 */


		constexpr void SetLoadFactor(const double_t loadFactor) noexcept { m_LoadFactor = loadFactor; }

		constexpr void Clear() noexcept
		{
			Allocator::ClearMemory(m_MetaData);
			m_MetaData = nullptr;
			m_Size = 0;
		}


		/*
		 *  ============================================================
		 *	|                         Accessors                        |
		 *  ============================================================
		 */


		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Size == 0; }
		NODISCARD constexpr const MetaData* Data() const noexcept { return m_MetaData; }
		NODISCARD constexpr size_t Size() const noexcept { return m_Size; }
		NODISCARD constexpr size_t Capacity() const noexcept { return m_Capacity; }
		NODISCARD constexpr double_t LoadFactor() const noexcept { return m_LoadFactor; }

		/* Enumerators (Iterators) */

		NODISCARD Enumerator<T> GetEnumerator() override
		{
			for (size_t i = 0; i < m_Size; i++)
			{
				auto& element = m_Data[i];
				co_yield element;
			}
		}

		NODISCARD Enumerator<T> GetEnumerator() const override
		{
			for (size_t i = 0; i < m_Size; i++)
			{
				const auto& element = m_Data[i];
				co_yield element;
			}
		}

		
		/*
		 *  ============================================================
		 *	|                    Operator Overloads                    |
		 *  ============================================================
		 */


		constexpr HashTable& operator=(const HashTable& other) noexcept
		{
			if (this == &other)
				return *this;

			if (!IsEmpty())
				Clear();

			CopyFrom(other);
			return *this;
		}

		constexpr HashTable& operator=(HashTable&& other) noexcept
		{
			if (this == &other)
				return *this;

			if (!IsEmpty())
				Clear();

			m_Data = other.m_Data;
			m_MetaData = other.m_MetaData;
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;
			m_LoadFactor = other.m_LoadFactor;

			other.m_Data = nullptr;
			other.m_MetaData = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;
			other.m_LoadFactor = 0;

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const HashTable& hashTable) noexcept
		{
			stream << "[";

			size_t counter = 0;
			auto metaDataNode = hashTable.m_MetaData;
			while (metaDataNode != nullptr)
			{
				auto node = metaDataNode->BucketReference;
				while (node != nullptr)
				{
					stream << node->Value;
					node = node->Next;
					counter++;

					if (counter < hashTable.m_Size)
						stream << ", ";
				}

				metaDataNode = metaDataNode->Next;
			}

			stream << "]";
			return stream;
		}

	protected:
		constexpr void Allocate(const size_t capacity) noexcept
		{
			m_Capacity = Allocator::Allocate(m_Data, m_Capacity, capacity);
		}

		constexpr void Reallocate(const size_t capacity) noexcept
		{
			m_Capacity = Allocator::Reallocate(m_Data, m_MetaData, m_Capacity, capacity);
		}

		constexpr void CopyFrom(const HashTable& other) noexcept
		{
			// TODO: Refactor
			auto data = other.m_Data;
			for (size_t i = 0; i < other.m_Capacity; i++)
			{
				if (data[i].IsValid())
				{
					m_Data[i] = data[i];
					
					// Update meta data
				}
			}

			//auto metaData = other.m_MetaData;
			//while (metaData != nullptr)
			//{
			//	auto node = metaData->BucketReference;
			//	while (node != nullptr)
			//	{
			//		Insert(node->Value);
			//		node = node->Next;
			//	}

			//	metaData = metaData->Next;
			//}
		}

		//bool Insert(const T& key)
		//{
		//	// Reallocate if necessary
		//	if (!IsWithinThreshold())
		//		Reallocate(m_Capacity * 2);

		//	// Get hash value
		//	const size_t hash = Hash(key) % m_Capacity;
		//	++m_Size;

		//	// Check if bucket is valid
		//	auto bucket = &m_Data[hash];
		//	if (bucket->IsValid())
		//	{
		//		// Get to the end of the chain (check for existing value)
		//		auto node = bucket;
		//		while (node->Next != nullptr)
		//		{
		//			if (node->GetKey() == key)
		//				return false;

		//			node = node->Next;
		//		}

		//		// Create new node
		//		auto newNode = Alloc<Node>(1);
		//		new(newNode) Node(key);

		//		node->Next = newNode;
		//		return true;
		//	}

		//	// Emplace value
		//	new(&m_Data[hash]) Node(key);
		//	AddMetaData(&m_Data[hash], hash);
		//	return true;
		//}

		//bool Insert(KeyType&& key)
		//{
		//	// Reallocate if necessary
		//	if (!IsWithinThreshold())
		//		Reallocate(m_Capacity * 2);

		//	// Get hash value
		//	const size_t hash = Hash(key) % m_Capacity;
		//	++m_Size;

		//	// Check if bucket is valid
		//	auto bucket = &m_Data[hash];
		//	if (bucket->IsValid())
		//	{
		//		// Get to the end of the chain (check for existing value)
		//		auto node = bucket;
		//		while (node->Next != nullptr)
		//		{
		//			if (node->GetKey() == key)
		//				return false;

		//			node = node->Next;
		//		}

		//		// Create new node
		//		auto newNode = Alloc<Node>(1);
		//		new(newNode) Node(std::move(key));

		//		node->Next = newNode;
		//		return true;
		//	}

		//	// Emplace value
		//	new(&m_Data[hash]) Node(std::move(key));
		//	AddMetaData(&m_Data[hash], hash);
		//	return true;
		//}

		//bool Insert(const KeyValuePair& tuple)
		//{
		//	// Reallocate if necessary
		//	if (!IsWithinThreshold())
		//		Reallocate(m_Capacity * 2);

		//	// Get hash value
		//	const size_t hash = Hash(tuple.Component1) % m_Capacity;
		//	++m_Size;

		//	// Check if bucket is valid
		//	auto bucket = &m_Data[hash];
		//	if (bucket->IsValid())
		//	{
		//		// Get to the end of the chain (check for existing value)
		//		auto node = bucket;
		//		while (node->Next != nullptr)
		//		{
		//			if (node->GetKey() == tuple.Component1)
		//				return false;

		//			node = node->Next;
		//		}

		//		// Create new node
		//		auto newNode = Alloc<Node>(1);
		//		new(newNode) Node(tuple);

		//		node->Next = newNode;
		//		return true;
		//	}

		//	// Emplace value
		//	new(&m_Data[hash]) Node(tuple);
		//	AddMetaData(&m_Data[hash], hash);
		//	return true;
		//}

		//bool Insert(KeyValuePair&& tuple)
		//{
		//	// Reallocate if necessary
		//	if (!IsWithinThreshold())
		//		Reallocate(m_Capacity * 2);

		//	// Get hash value
		//	const size_t hash = Hash(tuple.Component1) % m_Capacity;
		//	++m_Size;

		//	// Check if bucket is valid
		//	auto bucket = &m_Data[hash];
		//	if (bucket->IsValid())
		//	{
		//		// Get to the end of the chain (check for existing value)
		//		auto node = bucket;
		//		while (node->Next != nullptr)
		//		{
		//			if (node->GetKey() == tuple.Component1)
		//				return false;

		//			node = node->Next;
		//		}

		//		// Create new node
		//		auto newNode = Alloc<Node>(1);
		//		new(newNode) Node(std::move(tuple));

		//		node->Next = newNode;
		//		return true;
		//	}

		//	// Emplace value
		//	new(&m_Data[hash]) Node(std::move(tuple));
		//	AddMetaData(&m_Data[hash], hash);
		//	return true;
		//}

		//bool Erase(const KeyType& key)
		//{
		//	// Get hash value
		//	const size_t hash = Hash(key) % m_Capacity;

		//	// Check if bucket is valid
		//	auto bucket = &m_Data[hash];
		//	if (bucket->IsValid())
		//	{
		//		// Search for node with existing value
		//		if (bucket->GetKey() == key)
		//		{
		//			// Dispose and shift nodes down (if applicable)
		//			bucket->Dispose();

		//			if (bucket->Next != nullptr)
		//			{
		//				auto prev = bucket;
		//				auto node = prev->Next;
		//				while (node->Next != nullptr)
		//				{
		//					prev->Value = std::move(node->Value);

		//					prev = node;
		//					node = node->Next;
		//				}

		//				// Clean up tail references
		//				prev->Next = nullptr;
		//				delete node;
		//			}

		//			RemoveMetaData(bucket);

		//			--m_Size;
		//			return true;
		//		}

		//		auto node = bucket;
		//		while (node->Next != nullptr)
		//		{
		//			auto next = node->Next;
		//			if (next->GetKey() == key)
		//			{
		//				// Move next pointer
		//				node->Next = next->Next;

		//				// Remove node
		//				next->Invalidate();

		//				delete next;

		//				--m_Size;
		//				return true;
		//			}

		//			node = next;
		//		}
		//	}

		//	return false;
		//}

		//bool Erase(const KeyValuePair& tuple)
		//{
		//	// Get hash value
		//	const size_t hash = Hash(tuple.Component1) % m_Capacity;

		//	// Check if bucket is valid
		//	auto bucket = &m_Data[hash];
		//	if (bucket->IsValid())
		//	{
		//		// Search for node with existing value
		//		if (bucket->GetKey() == tuple.Component1)
		//		{
		//			// Dispose and shift nodes down (if applicable)
		//			bucket->Dispose();

		//			if (bucket->Next != nullptr)
		//			{
		//				auto prev = bucket;
		//				auto node = prev->Next;
		//				while (node->Next != nullptr)
		//				{
		//					prev->Value = std::move(node->Value);

		//					prev = node;
		//					node = node->Next;
		//				}

		//				// Clean up tail references
		//				prev->Next = nullptr;
		//				delete node;
		//			}

		//			RemoveMetaData(bucket);

		//			--m_Size;
		//			return true;
		//		}

		//		auto node = bucket;
		//		while (node->Next != nullptr)
		//		{
		//			auto next = node->Next;
		//			if (next->GetKey() == tuple.Component1)
		//			{
		//				// Move next pointer
		//				node->Next = next->Next;

		//				// Remove node
		//				next->Invalidate();

		//				delete next;

		//				--m_Size;
		//				return true;
		//			}

		//			node = next;
		//		}
		//	}

		//	return false;
		//}

		NODISCARD constexpr bool IsWithinThreshold() const noexcept
		{
			return (static_cast<double_t>(m_Size) / static_cast<double_t>(m_Capacity)) < m_LoadFactor;
		}

		void AddMetaData(Node* node, const size_t hash)
		{
			Allocator::AddMetaData(m_MetaData, node, hash);
		}

		void RemoveMetaData(Node* node)
		{
			Allocator::RemoveMetaData(m_MetaData, node);
		}

	protected:
		Memory m_Data = nullptr;
		MetaData* m_MetaData = nullptr;
		size_t m_Size = 0;
		size_t m_Capacity = 0;
		double_t m_LoadFactor = 0.80;

	private:
		constexpr static size_t DefaultCapacity = 25;
	};
}
