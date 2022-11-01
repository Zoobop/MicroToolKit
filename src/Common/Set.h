#pragma once

#include "Interfaces/IStruct.h"
#include "Interfaces/IHashTable.h"

namespace mtk {

	template<typename _Type>
	class Set : public IHashTable<_Type, newhash_t>
	{
	public:
		using Iterator = ContainerIterator<Set<_Type>>;
		using _KeyType = newhash_t;
		using _HashNode = HashNode<_Type>;
		using hashptr_t = _HashNode*;

	private:
		hashptr_t _NullNode = (hashptr_t)0xcdcdcdcdcdcdcdcd;

	public:
		Set() 
		{
			ReAlloc(50);
			__super::SetHash(Hash_Temp<_Type, _KeyType>);
		}

		Set(const size_t& _capacity, const float& _loadFactor)
		{
			ReAlloc(_capacity);
			__super::SetHash(Hash_Temp<_Type, _KeyType>);
		}

		Set(const Dynamic<_KeyType, const _Type&>& _hashFunc)
		{
			ReAlloc(50);
			__super::SetHash(_hashFunc);
		}

		Set(const size_t& _capacity) 
		{
			ReAlloc(_capacity);
			__super::SetHash(Hash_Temp<_Type, _KeyType>);
		}

		Set(std::initializer_list<_Type>&& _initList)
		{
			__super::SetHash(Hash_Temp<_Type, _KeyType>);

			size_t size = 0;
			for (const auto& _item : _initList)
				size++;

			ReAlloc(size);
			InsertRange(_initList);
		}

		Set(const Set<_Type>& _other)
		{
			m_Data = _other.m_Data;
			__super::m_Capacity = _other.m_Capacity;
			__super::m_Size = _other.m_Size;
			__super::m_LoadFactor = _other.m_LoadFactor;
			__super::m_HashFunction = _other.m_HashFunction;
		}

		Set(Set<_Type>&& _other) noexcept
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

		~Set()
		{
			CleanUp();
			Delete(m_Data, __super::m_Capacity);
		}

		// Utility Methods
		bool Insert(const _Type& _value)
		{
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			_KeyType hash = __super::Hash(_value);
			_HashNode* node = &m_Data[hash];

			while (node && node != _NullNode) {
				if (node->_value == _value &&
					node->_control == Ctrl::kFull)
					return false;
				node = node->_next;
			}

			node = &m_Data[hash];
			if (node != _NullNode && node->_control != Ctrl::kEmpty) {
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;
				
				new(&m_Data[hash]) _HashNode(_value);
				m_Data[hash]._next = newNode;

				newNode = nullptr;
				delete newNode;
			}
			else {
				new(&m_Data[hash]) _HashNode(_value);
			}

			node = nullptr;
			delete node;

			__super::m_Size++;
			return true;

			//_KeyType hash = __super::Hash(_value);
			//_HashNode* node = &m_Data[hash];
			//_HashNode* prev = nullptr;
			//while (node) {
			//	if (node == _NullNode) {
			//		node = new _HashNode(_value);
			//		break;
			//	}
			//	if (node->_control != Ctrl::kFull) {
			//		node->_control = Ctrl::kFull;
			//		node->_value = _value;
			//		break;
			//	}
			//	if (node->_value == _value &&
			//		node->_control == Ctrl::kFull)
			//		return false;
			//	prev = node;
			//	node = node->_next;
			//}
			//if (prev) prev->_next = node;

			//node = nullptr;
			//delete node;

			//__super::m_Size++;
			//return true;
		}

		bool Insert(_Type&& _value)
		{
			if (__super::m_Size / __super::m_Capacity >= __super::m_LoadFactor) {
				ReAlloc(__super::m_Capacity * __super::m_Capacity - __super::m_Capacity);
			}

			_KeyType hash = __super::Hash(_value);
			_HashNode* node = &m_Data[hash];

			while (node && node != _NullNode) {
				if (node->_value == _value &&
					node->_control == Ctrl::kFull)
					return false;
				node = node->_next;
			}

			node = &m_Data[hash];
			if (node != _NullNode && node->_control != Ctrl::kEmpty) {
				auto newNode = new _HashNode(m_Data[hash]._value);
				newNode->_next = node->_next;
				node = nullptr;

				new(&m_Data[hash]) _HashNode(_value);
				m_Data[hash]._next = newNode;

				newNode = nullptr;
				delete newNode;
			}
			else {
				new(&m_Data[hash]) _HashNode(_value);
			}

			node = nullptr;
			delete node;

			__super::m_Size++;
			return true;
		}

		bool InsertRange(const IDataHandler<_Type>& _container)
		{
			if (_container.Data()) {
				size_t size = _container.Capacity();
				for (size_t i = 0; i < size; i++)
					Insert(_container.Data()[i]);
				return true;
			}
			return false;
		}

		bool InsertRange(std::initializer_list<_Type>&& _initList)
		{
			if (_initList.size() > 0) {
				for (auto& item : _initList)
					Insert(std::move(item));
				return true;
			}
			return false;
		}

		bool InsertRange(const std::vector<_Type>& _vector)
		{
			if (_vector.data()) {
				size_t size = _vector.size();
				for (size_t i = 0; i < size; i++)
					Insert(_vector.data()[i]);
				return true;
			}
			return false;
		}

		bool Find(const _Type& _value) const
		{
			_KeyType hash = __super::Hash(_value);
			_HashNode* node = &m_Data[hash];
			while (node && node != _NullNode) {
				if (node->_control == Ctrl::kFull) {
					if (node->_value == _value) return true;
				}
				node = node->_next;
			}
			return false;
		}

		bool Erase(const _Type& _value)
		{
			_KeyType hash = __super::Hash(_value);
			_HashNode* node = &m_Data[hash];
			while (node && node != _NullNode) {
				if (node->_value == _value) {
					node->_control = Ctrl::kInvalid;
					__super::m_Size--;
					return true;
				}
				node = node->_next;
			}
			return false;
		}

		bool EraseKey(const _KeyType& _key)
		{
			if (_key < __super::m_Capacity && _key >= 0) {
				_HashNode* node = &m_Data[_key];
				if (node == nullptr || 
					node == _NullNode || 
					node->_control != Ctrl::kFull) 
					return false;
				while (node && node != _NullNode) {
					node->_control = Ctrl::kInvalid;
					node = node->_next;
				}
				__super::m_Size--;
				return true;
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

		void ForEach(const Param<const _HashNode&>& _param)
		{
			for (size_t i = 0; i < __super::m_Size; i++) {
				_param(m_Data[i]);
			}
		}

		void ForEach(const Param<const _Type&>& _param)
		{
			for (size_t i = 0; i < __super::m_Size; i++) {
				_param(m_Data[i]._value);
			}
		}

		constexpr _HashNode* Data() const { return m_Data; }

		// Accessors
		constexpr inline size_t Size() const { return __super::m_Size; }
		constexpr inline size_t Capacity() const { return __super::m_Capacity; }

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
		void operator=(const Set<_Type>& _other)
		{
			m_Data = _other.m_Data;
			__super::m_Capacity = _other.m_Capacity;
			__super::m_Size = _other.m_Size;
			__super::m_LoadFactor = _other.m_LoadFactor;
			__super::m_HashFunction = _other.m_HashFunction;
		}

		void operator=(Set<_Type>&& _other) noexcept
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

		friend std::ostream& operator<<(std::ostream& _stream, const Set<_Type>& _current)
		{
#if 1
			for (size_t i = 0; i < _current.m_Capacity; i++) {
				auto iter = &_current.m_Data[i];
				if (iter->_control != Ctrl::kFull) continue;
				while (iter && iter != _current._NullNode) {
					if (iter->_control == Ctrl::kFull)
						_stream << iter->_value << "\n";
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
						_stream << *iter;
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