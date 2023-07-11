#pragma once
#include "Core/Hash.hpp"
#include "Core/Memory/Memory.hpp"
#include "Collections/Base/Iterator.hpp"
#include "Utility/Tuple.hpp"

namespace Micro
{
	template <typename T>
	concept Hashable = requires(T object)
	{
		{ Micro::Hash<T>(object) };
	};

	template <typename TNode>
	concept HashNode = requires(TNode node)
	{
		{ node.GetKey() };
		{ node.GetValue() };
		{ node.Dispose() };
		{ node.IsValid() };
		{ node.Invalidate() };
		{ node.Next };
	};


	template <HashNode TNode>
	struct TableMetaData final
	{
		// Aliases
		using Node = TNode;
		using ValueType = typename Node::ValueType;
		using KeyType = typename Node::KeyType;

		// Fields
		TableMetaData* Next = nullptr;
		Node* BucketReference = nullptr;
		size_t Index = 0;
		MemStatus Status = MemStatus::Invalid;

		constexpr TableMetaData(Node* ptr, const size_t index) noexcept
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


	template <HashNode TNode, template <HashNode> typename TMetaData>
	class HashTableAllocator final
	{
	public:
		using Node = TNode;
		using MetaData = TMetaData<Node>;
		using Memory = Memory<Node>;

		constexpr HashTableAllocator() noexcept = default;
		constexpr HashTableAllocator(const HashTableAllocator&) noexcept = default;
		constexpr HashTableAllocator(HashTableAllocator&&) noexcept = default;
		constexpr ~HashTableAllocator() noexcept = default;

		constexpr HashTableAllocator& operator=(const HashTableAllocator&) noexcept = default;
		constexpr HashTableAllocator& operator=(HashTableAllocator&&) noexcept = default;

		NODISCARD static size_t Allocate(Memory& data, const size_t currentCapacity, const size_t newCapacity)
		{
			// Don't allocate if same capacity
			if (currentCapacity == newCapacity)
				return currentCapacity;

			data = Alloc<Node>(newCapacity);
			return newCapacity;
		}

		NODISCARD static size_t Reallocate(Memory& data, MetaData*& metaData, const size_t currentCapacity,
		                                   const size_t newCapacity)
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

		static void ClearMemory(MetaData* metaData)
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

		constexpr static void Dispose(Memory& data, const size_t capacity)
		{
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
		static void DeleteNodeChildren(Node* head)
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


	template <HashNode TNode>
	class HashTable
	{
	public:
		// Aliases
		using Node = TNode;
		using ValueType = typename Node::ValueType;
		using KeyType = typename Node::KeyType;
		using KeyValuePair = Tuple<KeyType, ValueType>;

		using Memory = Memory<Node>;
		using Allocator = HashTableAllocator<Node, TableMetaData>;
		using MetaData = TableMetaData<Node>;
		using Iterator = HashIterator<MetaData>;
		using ConstIterator = const Iterator;

		// Constructor/Destructor
		constexpr HashTable() noexcept
		{
			Allocate(DefaultCapacity);
		}

		HashTable(const HashTable& other)
			: m_LoadFactor(other.m_LoadFactor)
		{
			Allocate(other.m_Capacity);

			CopyFrom(other);
		}

		constexpr HashTable(HashTable&& other) noexcept
			: m_Data(std::move(other.m_Data)), m_MetaData(other.m_MetaData), m_Size(other.m_Size),
			  m_Capacity(other.m_Capacity),
			  m_LoadFactor(other.m_LoadFactor)
		{
			other.m_Data = nullptr;
			other.m_MetaData = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;
			other.m_LoadFactor = 0;
		}

		explicit HashTable(std::initializer_list<KeyType>&& initializerList) noexcept
		{
			const size_t length = initializerList.size();
			if (length == 0)
				return;

			Allocate(MAX(length, DefaultCapacity));

			for (auto& e : initializerList)
				Insert(std::move(const_cast<KeyType&>(e)));
		}

		explicit HashTable(std::initializer_list<KeyValuePair>&& initializerList) noexcept
		{
			const size_t length = initializerList.size();
			if (length == 0)
				return;

			Allocate(MAX(length, DefaultCapacity));

			for (auto& e : initializerList)
				Insert(std::move(const_cast<KeyValuePair&>(e)));
		}

		explicit HashTable(const size_t capacity) noexcept
		{
			Reallocate(capacity);
		}

		virtual ~HashTable()
		{
			Allocator::ClearMemory(m_MetaData);
			Allocator::Dispose(m_Data, m_Capacity);
			m_Data = nullptr;
			m_MetaData = nullptr;
			m_Size = 0;
			m_Capacity = 0;
			m_LoadFactor = 0;
		}

		// Utility
		NODISCARD constexpr bool IsEmpty() const noexcept { return m_Size == 0; }

		void Reserve(const size_t capacity)
		{
			Reallocate(capacity);
		}

		void Clear()
		{
			Allocator::ClearMemory(m_MetaData);
			m_MetaData = nullptr;
			m_Size = 0;
		}

		// Accessors
		NODISCARD constexpr const MetaData* Data() const noexcept { return m_MetaData; }
		NODISCARD constexpr size_t Size() const noexcept { return m_Size; }
		NODISCARD constexpr size_t Capacity() const noexcept { return m_Capacity; }
		NODISCARD constexpr double_t LoadFactor() const noexcept { return m_LoadFactor; }
		constexpr void SetLoadFactor(const double_t loadFactor) noexcept { m_LoadFactor = loadFactor; }

		// Iterators
		NODISCARD constexpr Iterator begin() { return Iterator(m_MetaData); }
		NODISCARD constexpr Iterator end() { return Iterator(nullptr); }
		NODISCARD constexpr ConstIterator cbegin() const { return begin(); }
		NODISCARD constexpr ConstIterator cend() const { return end(); }

		// Operator Overloads
		HashTable& operator=(const HashTable& other)
		{
			if (this == &other)
				return *this;

			Clear();

			CopyFrom(other);
			return *this;
		}

		HashTable& operator=(HashTable&& other) noexcept
		{
			if (this == &other)
				return *this;

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

		friend std::ostream& operator<<(std::ostream& stream, const HashTable& hashTable)
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
		void Allocate(const size_t capacity)
		{
			m_Capacity = Allocator::Allocate(m_Data, m_Capacity, capacity);
		}

		void Reallocate(const size_t capacity)
		{
			m_Capacity = Allocator::Reallocate(m_Data, m_MetaData, m_Capacity, capacity);
		}

		void CopyFrom(const HashTable& other)
		{
			auto metaData = other.m_MetaData;
			while (metaData != nullptr)
			{
				auto node = metaData->BucketReference;
				while (node != nullptr)
				{
					Insert(node->Value);
					node = node->Next;
				}

				metaData = metaData->Next;
			}
		}

		bool Insert(const KeyType& key)
		{
			// Reallocate if necessary
			if (!IsWithinThreshold())
				Reallocate(m_Capacity * 2);

			// Get hash value
			const size_t hash = Hash(key) % m_Capacity;
			++m_Size;

			// Check if bucket is valid
			auto bucket = &m_Data[hash];
			if (bucket->IsValid())
			{
				// Get to the end of the chain (check for existing value)
				auto node = bucket;
				while (node->Next != nullptr)
				{
					if (node->GetKey() == key)
						return false;

					node = node->Next;
				}

				// Create new node
				auto newNode = Alloc<Node>(1);
				new(newNode) Node(key);

				node->Next = newNode;
				return true;
			}

			// Emplace value
			new(&m_Data[hash]) Node(key);
			AddMetaData(&m_Data[hash], hash);
			return true;
		}

		bool Insert(KeyType&& key)
		{
			// Reallocate if necessary
			if (!IsWithinThreshold())
				Reallocate(m_Capacity * 2);

			// Get hash value
			const size_t hash = Hash(key) % m_Capacity;
			++m_Size;

			// Check if bucket is valid
			auto bucket = &m_Data[hash];
			if (bucket->IsValid())
			{
				// Get to the end of the chain (check for existing value)
				auto node = bucket;
				while (node->Next != nullptr)
				{
					if (node->GetKey() == key)
						return false;

					node = node->Next;
				}

				// Create new node
				auto newNode = Alloc<Node>(1);
				new(newNode) Node(std::move(key));

				node->Next = newNode;
				return true;
			}

			// Emplace value
			new(&m_Data[hash]) Node(std::move(key));
			AddMetaData(&m_Data[hash], hash);
			return true;
		}

		bool Insert(const KeyValuePair& tuple)
		{
			// Reallocate if necessary
			if (!IsWithinThreshold())
				Reallocate(m_Capacity * 2);

			// Get hash value
			const size_t hash = Hash(tuple.Component1) % m_Capacity;
			++m_Size;

			// Check if bucket is valid
			auto bucket = &m_Data[hash];
			if (bucket->IsValid())
			{
				// Get to the end of the chain (check for existing value)
				auto node = bucket;
				while (node->Next != nullptr)
				{
					if (node->GetKey() == tuple.Component1)
						return false;

					node = node->Next;
				}

				// Create new node
				auto newNode = Alloc<Node>(1);
				new(newNode) Node(tuple);

				node->Next = newNode;
				return true;
			}

			// Emplace value
			new(&m_Data[hash]) Node(tuple);
			AddMetaData(&m_Data[hash], hash);
			return true;
		}

		bool Insert(KeyValuePair&& tuple)
		{
			// Reallocate if necessary
			if (!IsWithinThreshold())
				Reallocate(m_Capacity * 2);

			// Get hash value
			const size_t hash = Hash(tuple.Component1) % m_Capacity;
			++m_Size;

			// Check if bucket is valid
			auto bucket = &m_Data[hash];
			if (bucket->IsValid())
			{
				// Get to the end of the chain (check for existing value)
				auto node = bucket;
				while (node->Next != nullptr)
				{
					if (node->GetKey() == tuple.Component1)
						return false;

					node = node->Next;
				}

				// Create new node
				auto newNode = Alloc<Node>(1);
				new(newNode) Node(std::move(tuple));

				node->Next = newNode;
				return true;
			}

			// Emplace value
			new(&m_Data[hash]) Node(std::move(tuple));
			AddMetaData(&m_Data[hash], hash);
			return true;
		}

		bool Erase(const KeyType& key)
		{
			// Get hash value
			const size_t hash = Hash(key) % m_Capacity;

			// Check if bucket is valid
			auto bucket = &m_Data[hash];
			if (bucket->IsValid())
			{
				// Search for node with existing value
				if (bucket->GetKey() == key)
				{
					// Dispose and shift nodes down (if applicable)
					bucket->Dispose();

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

					RemoveMetaData(bucket);

					--m_Size;
					return true;
				}

				auto node = bucket;
				while (node->Next != nullptr)
				{
					auto next = node->Next;
					if (next->GetKey() == key)
					{
						// Move next pointer
						node->Next = next->Next;

						// Remove node
						next->Invalidate();

						delete next;

						--m_Size;
						return true;
					}

					node = next;
				}
			}

			return false;
		}

		bool Erase(const KeyValuePair& tuple)
		{
			// Get hash value
			const size_t hash = Hash(tuple.Component1) % m_Capacity;

			// Check if bucket is valid
			auto bucket = &m_Data[hash];
			if (bucket->IsValid())
			{
				// Search for node with existing value
				if (bucket->GetKey() == tuple.Component1)
				{
					// Dispose and shift nodes down (if applicable)
					bucket->Dispose();

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

					RemoveMetaData(bucket);

					--m_Size;
					return true;
				}

				auto node = bucket;
				while (node->Next != nullptr)
				{
					auto next = node->Next;
					if (next->GetKey() == tuple.Component1)
					{
						// Move next pointer
						node->Next = next->Next;

						// Remove node
						next->Invalidate();

						delete next;

						--m_Size;
						return true;
					}

					node = next;
				}
			}

			return false;
		}

		NODISCARD constexpr bool IsWithinThreshold() const
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
		double_t m_LoadFactor = 0.75;

	private:
		constexpr static size_t DefaultCapacity = 16;
	};
}
