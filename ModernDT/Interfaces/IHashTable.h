#pragma once

#include "Utility/HashNode.h"
#include "DataStruct/LinkedList.h"

namespace mdt {

	typedef uint64_t newhash_t;

	template<typename _Type, typename _KeyType = newhash_t>
	_KeyType Hash_Temp(const _Type& _obj, size_t _capacity)
	{
		return _KeyType;
	}

	template<>
	newhash_t Hash_Temp(const int& _obj, size_t _capacity)
	{
		newhash_t size = (newhash_t)(sizeof(int) + sizeof(newhash_t)) * (newhash_t)pow(_obj, 1.5);
		newhash_t factor = (newhash_t)round((float)size - (float)_obj + ((float)size * 0.5f));
		return factor % _capacity;
	}

	template<>
	newhash_t Hash_Temp(const uint32_t& _obj, size_t _capacity)
	{
		newhash_t size = (newhash_t)(sizeof(uint32_t) + sizeof(newhash_t)) * (newhash_t)pow(_obj, 1.5);
		newhash_t factor = (newhash_t)round((float)size - (float)_obj + ((float)size * 0.5f));
		return factor % _capacity;
	}

	template<>
	newhash_t Hash_Temp(const float& _obj, size_t _capacity)
	{
		newhash_t size = (newhash_t)(sizeof(float) + sizeof(newhash_t)) * (newhash_t)(pow(_obj, 1.4) * 2.1);
		newhash_t factor = (newhash_t)round((float)size - (float)_obj + ((float)size * 0.5f));
		return factor % _capacity;
	}


	template<>
	newhash_t Hash_Temp(const double& _obj, size_t _capacity)
	{
		newhash_t size = (newhash_t)(sizeof(double) + sizeof(newhash_t)) * (newhash_t)(pow(_obj, 1.36) * 2.4);
		newhash_t factor = (newhash_t)round((float)size - (float)_obj + ((float)size * 0.5f));
		return factor % _capacity;
	}


	template<>
	newhash_t Hash_Temp(const std::string& _obj, size_t _capacity)
	{
		newhash_t size = (newhash_t)(sizeof(std::string) + sizeof(newhash_t));
		newhash_t length = (newhash_t)strlen(_obj.c_str());
		newhash_t hashVal = length + _obj.back();
		for (size_t i = 0; i < length; i++) {
			hashVal += _obj[i];
			hashVal *= _obj[i] % _capacity;
		}
		return hashVal % _capacity;
	}

	template<typename _Type, typename _KeyType = newhash_t>
	class IHashTable
	{
	public:
		using _HashType = HashNode<_Type>;
		using hashptr_t = HashNode<_Type>*;

	public:
		IHashTable()
		{
			ReAlloc(10);
		}

		IHashTable(const size_t& _capacity)
			: m_Capacity(_capacity)
		{
			ReAlloc(_capacity);
		}

		IHashTable(const size_t& _capacity, const float& _loadFactor)
			: m_Capacity(_capacity), m_LoadFactor(_loadFactor)
		{
			ReAlloc(_capacity);
		}

		IHashTable(const IHashTable<_Type>& _other)
			: m_Data(_other.m_Data), m_Capacity(_other.m_Capacity), m_Size(_other.m_Size), m_LoadFactor(_other.m_LoadFactor), m_HashFunction(_other.m_HashFunction)
		{
		}

		IHashTable(IHashTable<_Type>&& _other)
			: m_Data(_other.m_Data), m_Capacity(_other.m_Capacity), m_Size(_other.m_Size), m_LoadFactor(_other.m_LoadFactor), m_HashFunction(_other.m_HashFunction)
		{
			_other.m_Data = nullptr;
			_other.m_Capacity = 0;
			_other.m_Size = 0;
			_other.m_LoadFactor = 0;
		}

		virtual bool Insert(const _Type& _obj) = 0;
		virtual bool Insert(_Type&& _obj) = 0;
		virtual bool InsertRange(const IContainer<_Type>& _initList) = 0;
		virtual bool InsertRange(std::initializer_list<_Type>&& _initList) = 0;
		virtual bool InsertRange(const std::vector<_Type>& _stdvec) = 0;
		//template<typename ... _Args>
		//virtual _Type& Emplace(_Args&&... _args) = 0;

		virtual bool Remove(const _Type& _obj) = 0;
		virtual bool Remove(_Type&& _obj) = 0;
		virtual bool Find(const _Type& _obj) const = 0;
		virtual bool Find(_Type&& _obj) const = 0;

		virtual void Clear() = 0;

		constexpr inline void SetHash(const Dynamic<_KeyType, const _Type&, size_t>& _hashFunc) { m_HashFunction = _hashFunc; }
		constexpr inline void Reserve(const size_t& _capacity) { ReAlloc(_capacity); }

	protected:
		_KeyType Hash(const _Type& _obj) const
		{
			return m_HashFunction(_obj, m_Capacity);
		}

		void ReAlloc(size_t _capacity)
		{
			_HashType* newBlock = (_HashType*)Alloc<_HashType>(_capacity);

			if (_capacity < m_Size)
				m_Size = _capacity;

			for (size_t i = 0; i < m_Size; i++) {
				newBlock[i] = m_Data[i];
			}

			for (size_t i = 0; i < m_Size; i++) {
				m_Data[i].~_HashType();
			}

			Delete(m_Data, m_Capacity);
			m_Data = newBlock;
			m_Capacity = _capacity;
		}

	protected:

#define HASH(obj)			__super::Hash(obj)
#define REALLOC(capacity)	__super::ReAlloc(capacity)

	protected:
		_HashType* m_Data = nullptr;

		size_t m_Capacity = 0;
		size_t m_Size = 0;
		float m_LoadFactor = 6.0f / 10.0f;

		Dynamic<_KeyType, const _Type&, size_t> m_HashFunction;

#define _DATA			__super::m_Data
#define _CAPACITY		__super::m_Capacity
#define _SIZE			__super::m_Size
#define _LOADFACTOR		__super::m_LoadFactor
	};
}