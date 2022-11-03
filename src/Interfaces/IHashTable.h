#pragma once

#include "mtkpch.h"
#include "Utility/HashNode.h"

namespace mtk {

	template<typename _Type, typename _KeyType>
	_KeyType Hash_Temp(const _Type& _obj)
	{
		return {};
	}

	template<>
	newhash_t Hash_Temp(const int& _obj)
	{
		newhash_t size = (newhash_t)(sizeof(int) + sizeof(newhash_t)) * (newhash_t)pow(_obj, 1.5);
		newhash_t factor = (newhash_t)((float)size - (float)_obj + ((float)size * 0.5f));
		return factor;
	}

	template<>
	newhash_t Hash_Temp(const uint32_t& _obj)
	{
		newhash_t size = (newhash_t)(sizeof(uint32_t) + sizeof(newhash_t)) * (newhash_t)pow(_obj, 1.5);
		newhash_t factor = (newhash_t)((float)size - (float)_obj + ((float)size * 0.5f));
		return factor;
	}

	template<>
	newhash_t Hash_Temp(const float& _obj)
	{
		newhash_t size = (newhash_t)(sizeof(float) + sizeof(newhash_t)) * (newhash_t)(pow(_obj, 1.4) * 2.1);
		newhash_t factor = (newhash_t)((float)size - (float)_obj + ((float)size * 0.5f));
		return factor;
	}

	template<>
	newhash_t Hash_Temp(const double& _obj)
	{
		newhash_t size = (newhash_t)(sizeof(double) + sizeof(newhash_t)) * (newhash_t)(pow(_obj, 1.36) * 2.4);
		newhash_t factor = (newhash_t)((float)size - (float)_obj + ((float)size * 0.5f));
		return factor;
	}

	template<>
	newhash_t Hash_Temp(const char& _obj)
	{
		return _obj;
	}

	template<>
	newhash_t Hash_Temp(const std::string& _obj)
	{
		newhash_t size = (newhash_t)(sizeof(std::string) + sizeof(newhash_t));
		newhash_t length = (newhash_t)strlen(_obj.c_str());
		newhash_t hashVal = length + _obj.back();
		for (size_t i = 0; i < length; i++) {
			hashVal += _obj[i];
			hashVal *= _obj[i];
		}
		return hashVal;
	}

	template<typename _Type, typename _KeyType = newhash_t>
	class IHashTable
	{
	public:
		//template<typename ... _Args>
		//virtual _Type& Emplace(_Args&&... _args) = 0;

		//virtual bool Find(const _Type& _obj) const = 0;
		//virtual bool Erase(const _Type& _obj) = 0;
		//virtual bool EraseKey(const _KeyType& _obj) = 0;
		//virtual void Clear() = 0;

		constexpr void SetHash(const Func<_KeyType, const _Type&>& _hashFunc) { m_HashFunction = _hashFunc; }
		constexpr void Reserve(size_t _capacity) { ReAlloc(_capacity); }

	protected:
		_KeyType Hash(const _Type& _obj) const
		{
			return m_HashFunction(_obj) % m_Capacity;
		}

		virtual void ReAlloc(size_t _capacity) = 0;
		virtual void CleanUp() = 0;

	protected:
		size_t m_Capacity = 0;
		size_t m_Size = 0;
		float m_LoadFactor = 6.0f / 10.0f;
		Func<_KeyType, const _Type&> m_HashFunction;
	};

	#define _CAPACITY __super::m_Capacity
	#define _LOADFACTOR __super::m_LoadFactor
	#define _HASHFUNC __super::m_HashFunction
}