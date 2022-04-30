#pragma once

#include "Utility/Tuple.h"
#include "Interfaces/IStruct.h"
#include "Interfaces/IHashTable.h"

namespace mtk {

	template<typename _KeyType, typename _Type>
	class Map : public IHashTable<_KeyType>
	{
	public:
		using Iterator = ContainerIterator<Map<_KeyType, _Type>>;
		using _Pair = Tuple<_KeyType, _Type>;
		using _HashNode = HashNode<_Pair>;
		using hashptr_t = _HashNode*;

	private:
		hashptr_t _NullNode = (_HashNode*)(0xcdcdcdcdcdcdcdcd);

	public:
		/*
		 * ============
		 * Constructors
		 * ============
		 */
		Map()
		{
			Map<_KeyType, _Type>::ReAlloc(50);
			__super::SetHash(Hash_Temp<_KeyType, newhash_t>);
		}

		Map(const Dynamic<newhash_t, const _KeyType&>& _hashFunc)
		{
			Map<_KeyType, _Type>::ReAlloc(50);
			__super::SetHash(_hashFunc);
		}

		Map(std::initializer_list<_Pair>&& _initList) noexcept
		{
			Map<_KeyType, _Type>::ReAlloc(50);
			__super::SetHash(Hash_Temp<_KeyType, newhash_t>);
			InsertRange(_initList);
		}

		Map(const size_t& _capacity, const float& _loadFactor)
		{
			__super::m_LoadFactor = _loadFactor;
			Map<_KeyType, _Type>::ReAlloc(_capacity);
			__super::SetHash(Hash_Temp<_Type, newhash_t>);
		}

		Map(const size_t& _capacity)
		{
			Map<_KeyType, _Type>::ReAlloc(_capacity);
			__super::SetHash(Hash_Temp<_Type, newhash_t>);
		}

		Map(const Map<_KeyType, _Type>& _other)
		{
			m_Data = _other.m_Data;
			__super::m_Capacity = _other.m_Capacity;
			__super::m_Size = _other.m_Size;
			__super::m_LoadFactor = _other.m_LoadFactor;
			__super::m_HashFunction = _other.m_HashFunction;
		}

		Map(Map<_KeyType, _Type>&& _other) noexcept
		{
			m_Data = _other.m_Data;
			__super::m_Capacity = _other.m_Capacity;
			__super::m_Size = _other.m_Size;
			__super::m_LoadFactor = _other.m_LoadFactor;
			__super::m_HashFunction = _other.m_HashFunction;

			_other.m_Data = nullptr;
			_other.m_Capacity = 0;
			_other.m_Size = 0;
			_other.m_LoadFactor = 0;
			_other.m_HashFunction = nullptr;
		}

		virtual ~Map()
		{
			Map<_KeyType, _Type>::CleanUp();
			Delete(m_Data, __super::m_Capacity);
		}

		/*
		 * ===============
		 * Utility Methods
		 * ===============
		 */
		
		/**
		 * \brief Inserts a key with a default value into the map.
		 * \param _key Map key
		 * \return True, if successful.
		 */
		bool Insert(const _KeyType& _key)
		{
			// Resize if current capacity is above load factor percentage
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			// Get the hash value from key
			newhash_t hash = __super::Hash(_key);
			_HashNode* node = &m_Data[hash];

			// Check if key already exists
			while (node && node != _NullNode) {
				if (node->_value._first == _key &&
					node->_control == Ctrl::kFull)
					return false;
				node = node->_next;
			}

			/*
			 * Linked insertion implementation
			 */
			node = &m_Data[hash];
			if (node != _NullNode && node->_control != Ctrl::kEmpty) {
				// Create new node to link to next hashed node
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				// Emplace node with key at an empty node within the hashed location
				new(&m_Data[hash]) _HashNode({ _key, _Type() });
				m_Data[hash]._next = newNode;

				// Free memory
				newNode = nullptr;
				delete newNode;
			}
			else {
				// Emplace node with key at the hashed location
				new(&m_Data[hash]) _HashNode({_key, _Type()});
			}

			// Free memory
			node = nullptr;
			delete node;

			// Increase size to reflect the insertion
			++__super::m_Size;
			return true;
		}

		/**
		 * \brief Inserts a key with a default value into the map.
		 * \param _key Map key (lvalue)
		 * \return True, if successful.
		 */
		bool Insert(_KeyType&& _key)
		{
			// Resize if current capacity is above load factor percentage
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			// Get the hash value from key
			newhash_t hash = __super::Hash(_key);
			_HashNode* node = &m_Data[hash];

			// Check if key already exists
			while (node && node != _NullNode) {
				if (node->_value._first == _key &&
					node->_control == Ctrl::kFull)
						return false;
				node = node->_next;
			}

			/*
			 * Linked insertion implementation (insert front)
			 */
			node = &m_Data[hash];
			if (node != _NullNode && node->_control == Ctrl::kFull) {
				// Create new node to link to next hashed node
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				// Emplace node with key at an empty node within the hashed location
				new(&m_Data[hash]) _HashNode({ std::move(_key), _Type() });
				m_Data[hash]._next = newNode;

				// Free memory
				newNode = nullptr;
				delete newNode;
			}
			else {
				// Emplace node with key at the hashed location
				new(&m_Data[hash]) _HashNode({ std::move(_key), _Type() });
			}

			// Free memory
			node = nullptr;
			delete node;

			// Increase size to reflect the insertion
			++__super::m_Size;
			return true;
		}

		/**
		 * \brief Inserts the key and value as a pair into the map.
		 * \param _key Map key
		 * \param _value Value of key
		 * \return True, if successful.
		 */
		bool Insert(const _KeyType& _key, const _Type& _value)
		{
			// Resize if current capacity is above load factor percentage
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			// Get the hash value from key
			newhash_t hash = __super::Hash(_key);
			_HashNode* node = &m_Data[hash];

			// Check if key already exists
			while (node && node != _NullNode) {
				if (node->_value._first == _key &&
					node->_control == Ctrl::kFull)
						return false;
				node = node->_next;
			}

			/*
			 * Linked insertion implementation (insert front)
			 */
			node = &m_Data[hash];
			if (node != _NullNode && node->_control == Ctrl::kFull) {
				// Create new node to link to next hashed node
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				// Emplace node with key at an empty node within the hashed location
				new(&m_Data[hash]) _HashNode({ _key, _value });
				m_Data[hash]._next = newNode;

				// Free memory
				newNode = nullptr;
				delete newNode;
			}
			else {
				// Emplace node with key at the hashed location
				new(&m_Data[hash]) _HashNode({ _key, _value });
			}

			// Free memory
			node = nullptr;
			delete node;

			// Increase size to reflect the insertion
			++__super::m_Size;
			return true;
		}

		/**
		 * \brief Inserts the key and value as a pair into the map.
		 * \param _key Map key (lvalue)
		 * \param _value Value of key
		 * \return True, if successful.
		 */
		bool Insert(_KeyType&& _key, const _Type& _value) noexcept
		{
			// Resize if current capacity is above load factor percentage
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			// Get the hash value from key
			newhash_t hash = __super::Hash(_key);
			_HashNode* node = &m_Data[hash];

			// Check if key already exists
			while (node && node != _NullNode) {
				if (node->_value._first == _key &&
					node->_control == Ctrl::kFull)
						return false;
				node = node->_next;
			}

			/*
			 * Linked insertion implementation (insert front)
			 */
			node = &m_Data[hash];
			if (node != _NullNode && node->_control == Ctrl::kFull) {
				// Create new node to link to next hashed node
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				// Emplace node with key at an empty node within the hashed location
				new(&m_Data[hash]) _HashNode({ std::move(_key), _value });
				m_Data[hash]._next = newNode;

				// Free memory
				newNode = nullptr;
				delete newNode;
			}
			else {
				// Emplace node with key at the hashed location
				new(&m_Data[hash]) _HashNode({ std::move(_key), _value });
			}

			// Free memory
			node = nullptr;
			delete node;

			// Increase size to reflect the insertion
			++__super::m_Size;
			return true;
		}

		/**
		 * \brief Inserts the key and value as a pair into the map.
		 * \param _key Map key
		 * \param _value Value of key (lvalue)
		 * \return True, if successful.
		 */
		bool Insert(const _KeyType& _key, _Type&& _value) noexcept
		{
			// Resize if current capacity is above load factor percentage
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			// Get the hash value from key
			newhash_t hash = __super::Hash(_key);
			_HashNode* node = &m_Data[hash];

			// Check if key already exists
			while (node && node != _NullNode) {
				if (node->_value._first == _key &&
					node->_control == Ctrl::kFull)
						return false;
				node = node->_next;
			}

			/*
			 * Linked insertion implementation (insert front)
			 */
			node = &m_Data[hash];
			if (node != _NullNode && node->_control == Ctrl::kFull) {
				// Create new node to link to next hashed node
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				// Emplace node with key at an empty node within the hashed location
				new(&m_Data[hash]) _HashNode({ _key, std::move(_value) });
				m_Data[hash]._next = newNode;

				// Free memory
				newNode = nullptr;
				delete newNode;
			}
			else {
				// Emplace node with key at the hashed location
				new(&m_Data[hash]) _HashNode({ _key, std::move(_value) });
			}

			// Free memory
			node = nullptr;
			delete node;

			// Increase size to reflect the insertion
			++__super::m_Size;
			return true;
		}
		
		/**
		 * \brief Inserts the key and value as a pair into the map.
		 * \param _key Map key (lvalue)
		 * \param _value Value of key (lvalue)
		 * \return True, if successful.
		 */
		bool Insert(_KeyType&& _key, _Type&& _value) noexcept
		{
			// Resize if current capacity is above load factor percentage
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			// Get the hash value from key
			newhash_t hash = __super::Hash(_key);
			_HashNode* node = &m_Data[hash];

			// Check if key already exists
			while (node && node != _NullNode) {
				if (node->_value._first == _key &&
					node->_control == Ctrl::kFull)
						return false;
				node = node->_next;
			}

			/*
			 * Linked insertion implementation (insert front)
			 */
			node = &m_Data[hash];
			if (node != _NullNode && node->_control == Ctrl::kFull) {
				// Create new node to link to next hashed node
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				// Emplace node with key at an empty node within the hashed location
				new(&m_Data[hash]) _HashNode({ std::move(_key), std::move(_value) });
				m_Data[hash]._next = newNode;

				// Free memory
				newNode = nullptr;
				delete newNode;
			}
			else {
				// Emplace node with key at the hashed location
				new(&m_Data[hash]) _HashNode({ std::move(_key), std::move(_value) });
			}

			// Free memory
			node = nullptr;
			delete node;

			// Increase size to reflect the insertion
			++__super::m_Size;
			return true;
		}

		/**
		 * \brief Inserts the key/value pair into the map.
		 * \param _pair Key/value pair
		 * \return True, if successful.
		 */
		bool Insert(const _Pair& _pair)
		{
			// Resize if current capacity is above load factor percentage
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			// Get the hash value from key
			newhash_t hash = __super::Hash(_pair._first);
			_HashNode* node = &m_Data[hash];

			// Check if key already exists
			while (node && node != _NullNode) {
				if (node->_value._first == _pair._first &&
					node->_control == Ctrl::kFull)
						return false;
				node = node->_next;
			}

			/*
			 * Linked insertion implementation (insert front)
			 */
			node = &m_Data[hash];
			if (node != _NullNode && node->_control == Ctrl::kFull) {
				// Create new node to link to next hashed node
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				// Emplace node with key at an empty node within the hashed location
				new(&m_Data[hash]) _HashNode({ _pair });
				m_Data[hash]._next = newNode;

				// Free memory
				newNode = nullptr;
				delete newNode;
			}
			else {
				// Emplace node with key at the hashed location
				new(&m_Data[hash]) _HashNode({ _pair });
			}

			// Free memory
			node = nullptr;
			delete node;

			// Increase size to reflect the insertion
			++__super::m_Size;
			return true;
		}

		/**
		 * \brief Inserts the key/value pair into the map.
		 * \param _pair Key/value pair (lvalue)
		 * \return True, if successful.
		 */
		bool Insert(_Pair&& _pair) noexcept
		{
			// Resize if current capacity is above load factor percentage
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			// Get the hash value from key
			newhash_t hash = __super::Hash(_pair._first);
			_HashNode* node = &m_Data[hash];

			// Check if key already exists
			while (node && node != _NullNode) {
				if (node->_value._first == _pair._first &&
					node->_control == Ctrl::kFull)
						return false;
				node = node->_next;
			}

			/*
			 * Linked insertion implementation (insert front)
			 */
			node = &m_Data[hash];
			if (node != _NullNode && node->_control == Ctrl::kFull) {
				// Create new node to link to next hashed node
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				// Emplace node with key at an empty node within the hashed location
				new(&m_Data[hash]) _HashNode({ std::move(_pair) });
				m_Data[hash]._next = newNode;

				// Free memory
				newNode = nullptr;
				delete newNode;
			}
			else {
				// Emplace node with key at the hashed location
				new(&m_Data[hash]) _HashNode({ std::move(_pair) });
			}

			// Free memory
			node = nullptr;
			delete node;

			// Increase size to reflect the insertion
			++__super::m_Size;
			return true;
		}

		/**
		 * \brief Inserts all the key/value pairs in IDataHandler collection into the map.
		 * \param _container IDataHandler collection of key/value pairs
		 * \return True, if successful.
		 */
		bool InsertRange(const IDataHandler<_Pair>& _container)
		{
			if (_container.Data()) {
				const size_t size = _container.Capacity();
				for (size_t i = 0; i < size; i++)
				{
					// Insert all pairs
					Insert(_container.Data()[i]);
				}
				return true;
			}
			return false;
		}
		
		/**
		 * \brief Inserts all key/value pairs in initializer list into the map.
		 * \param _initList Initializer list of key/value pairs
		 * \return True, if successful.
		 */
		bool InsertRange(std::initializer_list<_Pair>&& _initList) noexcept
		{
			if (_initList.size() > 0) {
				for (auto& item : _initList)
					// Inserts all pairs
					Insert(std::move(item));
				return true;
			}
			return false;
		}

		/**
		 * \brief Inserts all key/value pairs in vector into the map.
		 * \param _vector Vector of key/value pairs
		 * \return True, if successful.
		 */
		bool InsertRange(const std::vector<_Pair>& _vector)
		{
			if (_vector.data()) {
				const size_t size = _vector.size();
				for (size_t i = 0; i < size; i++)
					// Inserts all pairs
					Insert(_vector.data()[i]);
				return true;
			}
			return false;
		}

		/**
		 * \brief Finds the key/value pair of the key.
		 * \param _key Key in the map
		 * \return Key/value pair of the key.
		 */
		[[nodiscard]] const _Pair& Find(const _KeyType& _key) const
		{
			// TODO: Refactor to search bucket values too
			newhash_t hash = __super::Hash(_key);
			if (m_Data[hash]._control != Ctrl::kFull) {
				DEBUG_BREAK();
			}
			return m_Data[hash]._value;
		}

		/**
		 * \brief Finds the key/value pair of the key.
		 * \param _key Key in the map
		 * \return Key/value pair of the key.
		 */
		_Pair& Find(const _KeyType& _key)
		{
			// TODO: Refactor to search bucket values too
			newhash_t hash = __super::Hash(_key);
			if (m_Data[hash]._control != Ctrl::kFull) {
				DEBUG_BREAK();
			}
			return m_Data[hash]._value;
		}

		/**
		 * \brief Gets the value of the associated key.
		 * \param _key Key in the map
		 * \return Value of the key.
		 */
		[[nodiscard]] const _Type& At(const _KeyType& _key) const
		{
			// TODO: Refactor to search bucket values too
			newhash_t hash = __super::Hash(_key);
			if (m_Data[hash]._control != Ctrl::kFull) {
				DEBUG_BREAK();
			}
			return m_Data[hash]._value._second;
		}

		/**
		 * \brief Gets the value of the associated key.
		 * \param _key Key in the map
		 * \return Value of the key.
		 */
		_Type& At(const _KeyType& _key)
		{
			// TODO: Refactor to search bucket values too
			newhash_t hash = __super::Hash(_key);
			if (m_Data[hash]._control != Ctrl::kFull) {
				DEBUG_BREAK();
			}
			return m_Data[hash]._value._second;
		}

		/**
		 * \brief Erases the value associated with the key. (Soft erase)
		 * \param _key Key in the map
		 * \return True, if successful.
		 */
		bool Erase(const _KeyType& _key)
		{
			newhash_t hash = __super::Hash(_key);
			_HashNode* node = &m_Data[hash];
			while (node && node != _NullNode) {
				if (node->_value._first == _key) {
					node->_control = Ctrl::kDeleted;
					--__super::m_Size;
					return true;
				}
				node = node->_next;
			}
			return false;
		}

		/**
		 * \brief Clears all of the entries in the map. (Soft erase)
		 */
		void Clear()
		{
			for (size_t i = 0; i < __super::m_Capacity; i++) {
				auto iter = &m_Data[i];
				while (iter && iter != _NullNode) {
					iter->_control = Ctrl::kEmpty;
					iter = iter->_next;
				}
			}

			__super::m_Size = 0;
		}

		/*
		 * =========
		 * Accessors
		 * =========
		 */
		
		/**
		 * \brief Gets the total capacity of the map.
		 * \return The total capacity of the map.
		 */
		[[nodiscard]] constexpr size_t Capacity() const { return __super::m_Capacity; }
		
		/**
		 * \brief Gets the number of entries in the map.
		 * \return The number of entries in the map.
		 */
		[[nodiscard]] constexpr size_t Size() const { return __super::m_Size; }

		/*
		 * =========
		 * Iterators
		 * =========
		 */
		constexpr Iterator begin()
		{
			return Iterator(m_Data);
		}

		constexpr Iterator end()
		{
			return Iterator(m_Data + __super::m_Capacity);
		}

		/*
		 * ==================
		 * Operator Overloads
		 * ==================
		 */
		const _Type& operator[](const _KeyType& _key) const
		{
			newhash_t hash = __super::Hash(_key);
			if (m_Data[hash]._control != Ctrl::kFull) {
				DEBUG_BREAK();
			}
			return m_Data[hash].value._second;
		}

		_Type& operator[](const _KeyType& _key)
		{
			newhash_t hash = __super::Hash(_key);
			if (m_Data[hash]._control != Ctrl::kFull) {
				DEBUG_BREAK();
			}
			return m_Data[hash]._value._second;
		}

		void operator=(const Map<_KeyType, _Type>& _other)
		{
			m_Data = _other.m_Data;
			__super::m_Capacity = _other.m_Capacity;
			__super::m_Size = _other.m_Size;
			__super::m_LoadFactor = _other.m_LoadFactor;
			__super::m_HashFunction = _other.m_HashFunction;
		}

		void operator=(Map<_KeyType, _Type>&& _other) noexcept
		{
			delete[] m_Data;

			m_Data = _other.m_Data;
			__super::m_Capacity = _other.m_Capacity;
			__super::m_Size = _other.m_Size;
			__super::m_LoadFactor = _other.m_LoadFactor;
			__super::m_HashFunction = _other.m_HashFunction;

			_other.m_Data = nullptr;
			_other.m_Capacity = 0;
			_other.m_Size = 0;
			_other.m_LoadFactor = 0;
			_other.m_HashFunction = nullptr;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Map<_KeyType, _Type>& _current)
		{
#if 0
			for (size_t i = 0; i < _current.m_Capacity; i++) {
				auto iter = &_current.m_Data[i];
				if (iter->_control != Ctrl::kFull) continue;
				while (iter && iter != _current._NullNode) {
					if (iter->_control == Ctrl::kFull)
						_stream << iter->_value._first << " : " << iter->_value._second << "\n";
					iter = iter->_next;
				}
			}
			return _stream;
#else
			for (size_t i = 0; i < _current.m_Capacity; i++) {
				auto iter = &_current.m_Data[i];
				if (iter->_control != Ctrl::kFull) continue;
				_stream << i << ": ";
				while (iter && iter != _current._NullNode) {
					if (iter->_control == Ctrl::kFull)
						_stream << iter->_value << " -> ";
					iter = iter->_next;
				}
				_stream << "\n";
			}
			return _stream;
#endif
		}
		
	private:
		void ReAlloc(size_t _capacity) override
		{
			auto* newBlock = (_HashNode*)Alloc<_HashNode>(_capacity);

			if (_capacity < __super::m_Size)
				__super::m_Size = _capacity;

			for (size_t i = 0; i < __super::m_Size; i++) {
				newBlock[i] = m_Data[i];
			}

			for (size_t i = 0; i < __super::m_Size; i++) {
				m_Data[i].~_HashNode();
			}

			Delete(m_Data, __super::m_Capacity);
			m_Data = newBlock;
			__super::m_Capacity = _capacity;
		}

		void CleanUp() override
		{
			for (auto i = 0; i < __super::m_Capacity; i++) {
				_HashNode* node = &m_Data[i];
				_HashNode* prev = nullptr;
				while (node && node != _NullNode) {
					if (!node->_next) {
						node->~_HashNode();
						break;
					}
					prev = node;
					node = node->_next;
					prev->~_HashNode();
				}
			}
			__super::m_Size = 0;
			__super::m_Capacity = 0;
		}

	private:
		_HashNode* m_Data = nullptr;
	};

}