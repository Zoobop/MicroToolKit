#pragma once
#include "mtkpch.h"
#include "Utility/MultiNode.h"

namespace mtk {

#if 1
	template<typename _Type>
	class Graph
	{
	public:
		using _Node = MultiNode<_Type>;

	public:
		Graph()
		{
			ReAlloc(5);
		}

		Graph(size_t _capacity)
			: m_Capacity(_capacity)
		{
			ReAlloc(m_Capacity);
		}

		Graph(const Graph& _other)
			: m_Data(_other.m_Data), m_Capacity(_other.m_Capacity), m_Size(_other.m_Size)
		{
		}

		Graph(Graph&& _other)
			: m_Capacity(_other.m_Capacity), m_Size(_other.m_Size)
		{
			m_Data = _other.m_Data;
		
			_other.m_Data = nullptr;
			_other.m_Capacity = 0;
			_other.m_Size = 0;
		}

		bool Insert(const _Type& _value)
		{
			return false;
		}

		bool Insert(const _Type& _from, const _Type& _to)
		{
			return false;
		}
		
	private:
		void ReAlloc(size_t _capacity)
		{
			_Type* newBlock = (_Type*)Alloc<_Type>(_capacity);

			if (_capacity < m_Size)
				m_Size = _capacity;

			for (size_t i = 0; i < m_Size; i++) {
				newBlock[i] = std::move(m_Data[i]);
			}

			for (size_t i = 0; i < m_Size; i++)
				m_Data[i].~_Type();

			Delete(m_Data, m_Capacity);
			m_Data = newBlock;
			m_Capacity = _capacity;
		}

	private:
		_Node* m_Data = nullptr;

		size_t m_Capacity = 0;
		size_t m_Size = 0;
	};
#endif

}