#pragma once

#include "mtkpch.h"
#include "Utility/HashNode.h"

namespace mtk
{
	template <typename _Type, typename _KeyType = newhash_t>
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
