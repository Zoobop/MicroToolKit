#pragma once

#include "Tuple.h"
#include "Interfaces/IStruct.h"
#include "Interfaces/IHashTable.h"

namespace mdt {

	template<typename _Type, typename _KeyType = newhash_t>
	class Map : public IHashTable<_Type, _KeyType>
	{
	public:
		using Iterator = ContainerIterator<Map<_Type, _KeyType>>;
		using _HashType = typename IHashTable<_Type, _KeyType>::_HashType;
		using hashptr_t = typename IHashTable<_Type, _KeyType>::hashptr_t;

	public:
		friend class IContainer<_Type>;
		friend class IHashTable<_Type, _KeyType>;

	public:
		Map()
			: IHashTable<_Type, _KeyType>()
		{
			__super::SetHash(Hash_Temp<_Type, _KeyType>);
		}

		Map(const Dynamic<_KeyType, const _Type&, size_t>& _hashFunc)
			: IHashTable<_Type, _KeyType>()
		{
			__super::SetHash(_hashFunc);
		}

		Map(const size_t& _capacity, const float& _loadFactor)
			: IHashTable<_Type, _KeyType>(_capacity, _loadFactor)
		{
			__super::SetHash(Hash_Temp<_Type, _KeyType>);
		}

		Map(const size_t& _size)
			: IHashTable<_Type, _KeyType>(_size)
		{
			__super::SetHash(Hash_Temp<_Type, _KeyType>);
		}

		Map(const Map<_Type>& _other)
		{
			_DATA = _other.m_Data;
			_CAPACITY = _other.m_Capacity;
			_SIZE = _other.m_Size;
			_LOADFACTOR = _other.m_LoadFactor;
			_HASHFUNC = _other.m_HashFunction;
		}

		Map(Map<_Type>&& _other) noexcept
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

		~Map()
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
			HashNode<_Type>* node = &_DATA[hash];
			HashNode<_Type>* prev = nullptr;
			while (node) {
				if (node == (hashptr_t)0xcdcdcdcdcdcdcdcd) {
					node = new HashNode<_Type>(_value);
					break;
				}
				if (node->_control != Ctrl::kFull) {
					node->_control = Ctrl::kFull;
					node->_value = _value;
					break;
				}
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
			HashNode<_Type>* node = &_DATA[hash];
			HashNode<_Type>* prev = nullptr;
			while (node) {
				if (node == (hashptr_t)0xcdcdcdcdcdcdcdcd) {
					node = new HashNode<_Type>(_value);
					break;
				}
				if (node->_control != Ctrl::kFull) {
					node->_control = Ctrl::kFull;
					node->_value = _value;
					break;
				}
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
			if (_DATA[hash]._control != Ctrl::kEmpty) {
				if (_DATA[hash]._value == _value) return true;
			}
			return false;
		}

		virtual bool Find(_Type&& _value) const override
		{
			_KeyType hash = __super::Hash(_value);
			if (_DATA[hash]._control != Ctrl::kEmpty) {
				if (_DATA[hash]._value == _value) return true;
			}
			return false;
		}

		virtual bool Erase(const _Type& _obj) override
		{
			return false;
		}

		virtual bool Erase(_Type&& _obj) override
		{
			return false;
		}

		virtual bool EraseKey(const _KeyType& _obj) override
		{
			return false;
		}

		virtual bool EraseKey(_KeyType&& _obj) override
		{
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
		const _Type& operator[](size_t _index) const
		{
			if (_index >= _CAPACITY) {
				__debugbreak();
			}
			return _DATA[_index];
		}

		_Type& operator[](size_t _index)
		{
			if (_index >= _CAPACITY) {
				__debugbreak();
			}
			return _DATA[_index];
		}

		void operator=(const Map<_Type>& _other)
		{
			_DATA = _other.m_Data;
			_CAPACITY = _other.m_Capacity;
			_SIZE = _other.m_Size;
			_LOADFACTOR = _other.m_LoadFactor;
			_HASHFUNC = _other.m_HashFunction;
		}

		void operator=(Map<_Type>&& _other) noexcept
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

		friend std::ostream& operator<<(std::ostream& _stream, const Map<_Type>& _current)
		{
			for (size_t i = 0; i < _current.m_Capacity; i++) {
				auto iter = &_current.m_Data[i];
				if (iter->_control != Ctrl::kFull) continue;
				_stream << i << ": ";
				while (iter != (hashptr_t)0xcdcdcdcdcdcdcdcd) {
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