#pragma once

#include "Interfaces/IStruct.h"
#include "Interfaces/IHashTable.h"

namespace mdt {

	template<typename _Type>
	class Set : public IHashTable<_Type, newhash_t>
	{
	public:
		using _KeyType = newhash_t;
		using Iterator = ContainerIterator<Set<_Type>>;
		using _HashType = typename IHashTable<_Type, _KeyType>::_HashType;
		using hashptr_t = typename IHashTable<_Type, _KeyType>::hashptr_t;

	public:
		friend class IContainer<_Type>;
		friend class IHashTable<_Type, _KeyType>;

	public:
		Set() 
			: IHashTable<_Type, _KeyType>()
		{
			__super::SetHash(Hash_Temp<_Type, _KeyType>);
		}

		Set(const size_t& _capacity, const float& _loadFactor)
			: IHashTable<_Type, _KeyType>(_capacity, _loadFactor)
		{
			__super::SetHash(Hash_Temp<_Type, _KeyType>);
		}

		Set(const Dynamic<_KeyType, const _Type&, size_t>& _hashFunc)
			: IHashTable<_Type, _KeyType>()
		{
			__super::SetHash(_hashFunc);
		}

		Set(const size_t& _size) 
			: IHashTable<_Type, _KeyType>(_size)
		{
			__super::SetHash(Hash_Temp<_Type, _KeyType>);
		}

		Set(std::initializer_list<_Type>&& _initList)
		{
			__super::SetHash(Hash_Temp<_Type, _KeyType>);

			size_t size = 0;
			for (const auto& _item : _initList)
				size++;

			REALLOC(size);
			for (const auto& _item : _initList)
				Insert(std::move(_item));
		}

		Set(const Set<_Type>& _other)
		{
			_DATA = _other.m_Data;
			_CAPACITY = _other.m_Capacity;
			_SIZE = _other.m_Size;
			_LOADFACTOR = _other.m_LoadFactor;
			_HASHFUNC = _other.m_HashFunction;
		}

		Set(Set<_Type>&& _other) noexcept
		{
			_DATA = _other.m_Data;
			_CAPACITY = _other.m_Capacity;
			_SIZE = _other.m_Size;
			_LOADFACTOR = _other.m_LoadFactor;
			_HASHFUNC = _other.m_HashFunction;

			_other.m_Data = nullptr;
			_other.m_Capacity = 0;
			_other.m_Size = 0;
			_other.m_LoadFactor = 0;
			_other.m_HashFunction.Clear();
		}

		~Set()
		{
			Clear();
			Delete(_DATA, _CAPACITY);
		}

		// Override Methods
		virtual bool Insert(const _Type& _value) override
		{
			if (_SIZE / _CAPACITY >= _LOADFACTOR) {
				REALLOC(_CAPACITY * _CAPACITY - _CAPACITY);
			}

			_KeyType hash = __super::Hash(_value);
			_HashType* node = &_DATA[hash];
			_HashType* prev = nullptr;
			while (node) {
				if (node == _NULLNODE) {
					node = new _HashType(_value);
					break;
				}
				if (node->_control != Ctrl::kFull) {
					node->_control = Ctrl::kFull;
					node->_value = _value;
					break;
				}
				if (node->_value == _value &&
					node->_control == Ctrl::kFull)
					return false;
				prev = node;
				node = node->_next;
			}
			if (prev) prev->_next = node;

			node = nullptr;
			delete node;

			_SIZE++;
			return true;
		}

		virtual bool Insert(_Type&& _value) override
		{
			if (_SIZE / _CAPACITY >= _LOADFACTOR) {
				REALLOC(_CAPACITY * _CAPACITY - _CAPACITY);
			}

			_KeyType hash = __super::Hash(_value);
			_HashType* node = &_DATA[hash];
			_HashType* prev = nullptr;
			while (node) {
				if (node == _NULLNODE) {
					node = new _HashType(_value);
					break;
				}
				if (node->_control != Ctrl::kFull) {
					node->_control = Ctrl::kFull;
					node->_value = _value;
					break;
				}
				if (node->_value == _value &&
					node->_control == Ctrl::kFull)
					return false;
				prev = node;
				node = node->_next;
			}
			if (prev) prev->_next = node;

			node = nullptr;
			delete node;

			_SIZE++;
			return true;
		}

		virtual bool InsertRange(const IContainer<_Type>& _container) override
		{
			if (_container.Data()) {
				size_t size = _container.Capacity();
				for (size_t i = 0; i < size; i++)
					Insert(_container.Data()[i]);
				return true;
			}
			return false;
		}

		virtual bool InsertRange(std::initializer_list<_Type>&& _initList) override
		{
			if (_initList.size() > 0) {
				for (auto& item : _initList)
					Insert(std::move(item));
				return true;
			}
			return false;
		}

		virtual bool InsertRange(const std::vector<_Type>& _vector) override
		{
			if (_vector.data()) {
				size_t size = _vector.size();
				for (size_t i = 0; i < size; i++)
					Insert(_vector.data()[i]);
				return true;
			}
			return false;
		}

		virtual bool Find(const _Type& _value) const override
		{
			_KeyType hash = __super::Hash(_value);
			_HashType* node = &_DATA[hash];
			while (node && node != _NULLNODE) {
				if (node->_control != Ctrl::kEmpty) {
					if (node->_value == _value) return true;
				}
				node = node->_next;
			}
			return false;
		}

		virtual bool Find(_Type&& _value) const override
		{
			_KeyType hash = __super::Hash(_value);
			_HashType* node = &_DATA[hash];
			while (node && node != _NULLNODE) {
				if (node->_control != Ctrl::kEmpty) {
					if (node->_value == _value) return true;
				}
				node = node->_next;
			}
			return false;
		}

		virtual bool Erase(const _Type& _obj) override
		{
			_KeyType hash = __super::Hash(_obj);
			_HashType* node = &_DATA[hash];
			_HashType* prev = nullptr;
			while (node && node != _NULLNODE) {
				if (node->_value == _obj) {
					node->_control = Ctrl::kDeleted;
					if (prev) prev->_next = node;
					return true;
				}
				prev = node;
				node = node->_next;
			}
			return false;
		}

		virtual bool Erase(_Type&& _obj) override
		{
			_KeyType hash = __super::Hash(_obj);
			_HashType* node = &_DATA[hash];
			_HashType* prev = nullptr;
			while (node && node != _NULLNODE) {
				if (node->_value == _obj) {
					node->_control = Ctrl::kDeleted;
					if (prev) prev->_next = node->_next;
					return true;
				}
				prev = node;
				node = node->_next;
			}
			return false;
		}

		virtual bool EraseKey(const _KeyType& _key) override
		{
			if (_key < _CAPACITY && _key >= 0) {
				_HashType* node = &_DATA[_key];
				if (node == nullptr || 
					node == _NULLNODE || 
					node->_control != Ctrl::kFull) 
					return false;
				while (node && node != _NULLNODE) {
					node->_control = Ctrl::kDeleted;
					node = node->_next;
				}
				return true;
			}
			return false;
		}

		virtual bool EraseKey(_KeyType&& _key) override
		{
			if (_key < _CAPACITY && _key >= 0) {
				_HashType* node = &_DATA[_key];
				if (node == nullptr || node == _NULLNODE || node->_control != Ctrl::kFull) return false;
				while (node && node != _NULLNODE) {
					node->_control = Ctrl::kDeleted;
					node = node->_next;
				}
				return true;
			}
			return false;
		}

		virtual void Clear() override
		{
			for (size_t i = 0; i < _SIZE; i++) {
				_DATA[i].~_HashType();
			}

			_SIZE = 0;
		}

		void ForEach(const Param<const _HashType&>& _param)
		{
			for (size_t i = 0; i < _SIZE; i++) {
				_param(_DATA[i]);
			}
		}

		void ForEach(const Param<const _Type&>& _param)
		{
			for (size_t i = 0; i < _SIZE; i++) {
				_param(_DATA[i]._value);
			}
		}

		constexpr _HashType* Data() const { return _DATA; }

		// Accessors
		constexpr inline size_t Capacity() const { return _CAPACITY; }

		// Iterator
		constexpr Iterator begin()
		{
			return Iterator(_DATA);
		}

		constexpr Iterator end()
		{
			return Iterator(_DATA + _SIZE);
		}

		// Operator Overloads
		void operator=(const Set<_Type>& _other)
		{
			_DATA = _other.m_Data;
			_CAPACITY = _other.m_Capacity;
			_SIZE = _other.m_Size;
			_LOADFACTOR = _other.m_LoadFactor;
			_HASHFUNC = _other.m_HashFunction;
		}

		void operator=(Set<_Type>&& _other) noexcept
		{
			delete[] _DATA;

			_DATA = _other.m_Data;
			_CAPACITY = _other.m_Capacity;
			_SIZE = _other.m_Size;
			_LOADFACTOR = _other.m_LoadFactor;
			_HASHFUNC = _other.m_HashFunction;

			_other.m_Data = nullptr;
			_other.m_Capacity = 0;
			_other.m_Size = 0;
			_other.m_LoadFactor = 0;
			_other.m_HashFunction.Clear();
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Set<_Type>& _current)
		{
			for (size_t i = 0; i < _current.m_Capacity; i++) {
				auto iter = &_current.m_Data[i];
				if (iter->_control != Ctrl::kFull) continue;
				_stream << i << ": ";
				while (iter && iter != _NULLNODE) {
					if (iter->_control == Ctrl::kFull)
						_stream << *iter;
					iter = iter->_next;
				}
				_stream << "\n";
			}
			return _stream;
		}

	};
}