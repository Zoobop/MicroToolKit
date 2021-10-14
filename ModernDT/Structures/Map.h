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
		hashptr_t _NullNode = (_HashNode*)0xcdcdcdcdcdcdcdcd;

	public:
		Map()
		{
			ReAlloc(50);
			__super::SetHash(Hash_Temp<_KeyType, newhash_t>);
		}

		Map(const Dynamic<newhash_t, const _KeyType&>& _hashFunc)
		{
			ReAlloc(50);
			__super::SetHash(_hashFunc);
		}

		Map(std::initializer_list<_Pair>&& _initList) noexcept
		{
			ReAlloc(50);
			__super::SetHash(Hash_Temp<_KeyType, newhash_t>);
			InsertRange(_initList);
		}

		Map(const size_t& _capacity, const float& _loadFactor)
		{
			__super::m_LoadFactor = _loadFactor;
			ReAlloc(_capacity);
			__super::SetHash(Hash_Temp<_Type, newhash_t>);
		}

		Map(const size_t& _capacity)
		{
			ReAlloc(_capacity);
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
			_other.m_HashFunction;
		}

		~Map()
		{
			CleanUp();
			Delete(m_Data, __super::m_Capacity);
		}

		// Utility Methods
		bool Insert(const _KeyType& _key)
		{
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			newhash_t hash = __super::Hash(_key);
			_HashNode* node = &m_Data[hash];

			while (node && node != _NullNode) {
				if (node->_value._first == _key &&
					node->_control == Ctrl::kFull)
					return false;
				node = node->_next;
			}

			node = &m_Data[hash];
			if (node != _NullNode && node->_control != Ctrl::kEmpty) {
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				new(&m_Data[hash]) _HashNode({ _key, _Type() });
				m_Data[hash]._next = newNode;

				newNode = nullptr;
				delete newNode;
			}
			else {
				new(&m_Data[hash]) _HashNode({_key, _Type()});
			}

			node = nullptr;
			delete node;

			__super::m_Size++;
			return true;
		}

		bool Insert(_KeyType&& _key)
		{
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			newhash_t hash = __super::Hash(_key);
			_HashNode* node = &m_Data[hash];

			while (node && node != _NullNode) {
				if (node->_value._first == _key &&
					node->_control == Ctrl::kFull)
					return false;
				node = node->_next;
			}

			node = &m_Data[hash];
			if (node != _NullNode && node->_control != Ctrl::kEmpty) {
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				new(&m_Data[hash]) _HashNode({ std::move(_key), _Type()});
				m_Data[hash]._next = newNode;

				newNode = nullptr;
				delete newNode;
			}
			else {
				new(&m_Data[hash]) _HashNode({ std::move(_key), _Type()});
			}

			node = nullptr;
			delete node;

			__super::m_Size++;
			return true;
		}

		bool Insert(const _KeyType& _key, const _Type& _value)
		{
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			newhash_t hash = __super::Hash(_key);
			_HashNode* node = &m_Data[hash];

			while (node && node != _NullNode) {
				if (node->_value._first == _key &&
					node->_control == Ctrl::kFull)
					return false;
				node = node->_next;
			}

			node = &m_Data[hash];
			if (node != _NullNode && node->_control != Ctrl::kEmpty) {
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				new(&m_Data[hash]) _HashNode({ _key, _value });
				m_Data[hash]._next = newNode;

				newNode = nullptr;
				delete newNode;
			}
			else {
				new(&m_Data[hash]) _HashNode({ _key, _value });
			}

			node = nullptr;
			delete node;

			__super::m_Size++;
			return true;
		}

		bool Insert(_KeyType&& _key, const _Type& _value) noexcept
		{
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			newhash_t hash = __super::Hash(_key);
			_HashNode* node = &m_Data[hash];

			while (node && node != _NullNode) {
				if (node->_value._first == _key &&
					node->_control == Ctrl::kFull)
					return false;
				node = node->_next;
			}

			node = &m_Data[hash];
			if (node != _NullNode && node->_control != Ctrl::kEmpty) {
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				new(&m_Data[hash]) _HashNode({ std::move(_key), _value});
				m_Data[hash]._next = newNode;

				newNode = nullptr;
				delete newNode;
			}
			else {
				new(&m_Data[hash]) _HashNode({ std::move(_key), _value });
			}

			node = nullptr;
			delete node;

			__super::m_Size++;
			return true;
		}

		bool Insert(const _KeyType& _key, _Type&& _value) noexcept
		{
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			newhash_t hash = __super::Hash(_key);
			_HashNode* node = &m_Data[hash];

			while (node && node != _NullNode) {
				if (node->_value._first == _key &&
					node->_control == Ctrl::kFull)
					return false;
				node = node->_next;
			}

			node = &m_Data[hash];
			if (node != _NullNode && node->_control != Ctrl::kEmpty) {
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				new(&m_Data[hash]) _HashNode({ _key, std::move(_value) });
				m_Data[hash]._next = newNode;

				newNode = nullptr;
				delete newNode;
			}
			else {
				new(&m_Data[hash]) _HashNode({ _key, std::move(_value) });
			}

			node = nullptr;
			delete node;

			__super::m_Size++;
			return true;
		}

		bool Insert(_KeyType&& _key, _Type&& _value) noexcept
		{
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			newhash_t hash = __super::Hash(_key);
			_HashNode* node = &m_Data[hash];

			while (node && node != _NullNode) {
				if (node->_value._first == _key &&
					node->_control == Ctrl::kFull)
					return false;
				node = node->_next;
			}

			node = &m_Data[hash];
			if (node != _NullNode && node->_control != Ctrl::kEmpty) {
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				new(&m_Data[hash]) _HashNode({ std::move(_key), std::move(_value) });
				m_Data[hash]._next = newNode;

				newNode = nullptr;
				delete newNode;
			}
			else {
				new(&m_Data[hash]) _HashNode({ std::move(_key), std::move(_value) });
			}

			node = nullptr;
			delete node;

			__super::m_Size++;
			return true;
		}

		bool Insert(const _Pair& _pair)
		{
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			newhash_t hash = __super::Hash(_pair._first);
			_HashNode* node = &m_Data[hash];

			while (node && node != _NullNode) {
				if (node->_value._first == _pair._first &&
					node->_control == Ctrl::kFull)
					return false;
				node = node->_next;
			}

			node = &m_Data[hash];
			if (node != _NullNode && node->_control != Ctrl::kEmpty) {
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				new(&m_Data[hash]) _HashNode({ _pair });
				m_Data[hash]._next = newNode;

				newNode = nullptr;
				delete newNode;
			}
			else {
				new(&m_Data[hash]) _HashNode({ _pair });
			}

			node = nullptr;
			delete node;

			__super::m_Size++;
			return true;
		}

		bool Insert(_Pair&& _pair) noexcept
		{
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			newhash_t hash = __super::Hash(_pair._first);
			_HashNode* node = &m_Data[hash];

			while (node && node != _NullNode) {
				if (node->_value._first == _pair._first &&
					node->_control == Ctrl::kFull)
					return false;
				node = node->_next;
			}

			node = &m_Data[hash];
			if (node != _NullNode && node->_control != Ctrl::kEmpty) {
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				new(&m_Data[hash]) _HashNode({ std::move(_pair) });
				m_Data[hash]._next = newNode;

				newNode = nullptr;
				delete newNode;
			}
			else {
				new(&m_Data[hash]) _HashNode({ std::move(_pair) });
			}

			node = nullptr;
			delete node;

			__super::m_Size++;
			return true;
		}

		bool InsertRange(const IDataHandler<_Pair>& _container)
		{
			if (_container.Data()) {
				size_t size = _container.Capacity();
				for (size_t i = 0; i < size; i++)
					Insert(_container.Data()[i]);
				return true;
			}
			return false;
		}

		bool InsertRange(std::initializer_list<_Pair>&& _initList) noexcept
		{
			if (_initList.size() > 0) {
				for (auto& item : _initList)
					Insert(std::move(item));
				return true;
			}
			return false;
		}

		bool InsertRange(const std::vector<_Pair>& _vector)
		{
			if (_vector.data()) {
				size_t size = _vector.size();
				for (size_t i = 0; i < size; i++)
					Insert(_vector.data()[i]);
				return true;
			}
			return false;
		}

		const _Pair& Find(const _KeyType& _key) const
		{
			newhash_t hash = __super::Hash(_key);
			if (m_Data[hash]._control != Ctrl::kFull) {
				__debugbreak();
			}
			return m_Data[hash]._value;
		}

		_Pair& Find(const _KeyType& _key)
		{
			newhash_t hash = __super::Hash(_key);
			if (m_Data[hash]._control != Ctrl::kFull) {
				__debugbreak();
			}
			return m_Data[hash]._value;
		}

		const _Type& At(const _KeyType& _key) const
		{
			newhash_t hash = __super::Hash(_key);
			if (m_Data[hash]._control != Ctrl::kFull) {
				__debugbreak();
			}
			return m_Data[hash]._value._second;
		}

		_Type& At(const _KeyType& _key)
		{
			newhash_t hash = __super::Hash(_key);
			if (m_Data[hash]._control != Ctrl::kFull) {
				__debugbreak();
			}
			return m_Data[hash]._value._second;
		}

		bool Erase(const _KeyType& _key)
		{
			newhash_t hash = __super::Hash(_key);
			_HashNode* node = &m_Data[hash];
			while (node && node != _NullNode) {
				if (node->_value._first == _key) {
					node->_control = Ctrl::kDeleted;
					__super::m_Size--;
					return true;
				}
				node = node->_next;
			}
			return false;
		}

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

		//void ForEach(const Param<const _Pair&>& _param)
		//{
		//	for (size_t i = 0; i < __super::m_Size; i++) {
		//		_param(m_Data[i]);
		//	}
		//}

		//void ForEach(const Param<const _Type&>& _param)
		//{
		//	for (size_t i = 0; i < __super::m_Size; i++) {
		//		_param(m_Data[i]._second);
		//	}
		//}

		// Accessors
		constexpr inline size_t Capacity() const { return __super::m_Capacity; }
		constexpr inline size_t Size() const { return __super::m_Size; }

		// Iterator
		constexpr Iterator begin()
		{
			return Iterator(m_Data);
		}

		constexpr Iterator end()
		{
			return Iterator(m_Data + __super::m_Size);
		}

		// Operator Overloads
		const _Type& operator[](const _KeyType& _key) const
		{
			newhash_t hash = __super::Hash(_key);
			if (m_Data[hash]._second._control != Ctrl::kFull) {
				__debugbreak();
			}
			return m_Data[hash]._second;
		}

		_Type& operator[](const _KeyType& _key)
		{
			newhash_t hash = __super::Hash(_key);
			if (m_Data[hash]._second._control != Ctrl::kFull) {
				__debugbreak();
			}
			return m_Data[hash]._second;
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
			_other.m_HashFunction;
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
		virtual void ReAlloc(size_t _capacity) override
		{
			_HashNode* newBlock = (_HashNode*)Alloc<_HashNode>(_capacity);

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

		virtual void CleanUp() override
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