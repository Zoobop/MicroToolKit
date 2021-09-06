#pragma once

#include "Tuple.h"
#include "Container.h"

namespace mdt {

	template<typename TKey, typename TVal>
	class Map
	{
	public:
		using Iterator = ContainerIterator<Tuple<TKey, TVal>>;

	public:
		Map()
		{
			ReAlloc(2);
		}

		Map(size_t size)
			: m_Capacity(size)
		{
			ReAlloc(m_Capacity);
		}

		Map(const Map<TKey, TVal>& other)
		{

		}

		Map(Map<TKey, TVal>&& other)
		{
		}

		~Map()
		{
			delete[] m_Data;
		}

		// Override Methods
		bool Add(const TKey& _key, const TVal& _value)
		{
			if (!ContainsKey(_key)) {
				if (m_Size <= m_Capacity) {
					ReAlloc(m_Capacity + m_Capacity / 2);
				}

				m_Data[m_Size] = Tuple(_key, _value);
				m_Size++;

				return true;
			}
			return false;
		}

		bool RemoveKey(const TKey& _key)
		{
			if (ContainsKey(_key)) {
				size_t currentIndex = 0;
				for (currentIndex; currentIndex < m_Size; currentIndex++) {
					if (m_Data[currentIndex].First() == _key) {
						m_Data[currentIndex].~Tuple<TKey, TVal>();
						currentIndex++;
						break;
					}
				}

				for (currentIndex; currentIndex < m_Size; currentIndex++) {
					m_Data[currentIndex - 1] = m_Data[currentIndex];
				}

				m_Size--;
				return true;
			}
			return false;
		}

		bool ContainsKey(const TKey& _key) const
		{
			for (size_t i = 0; i < m_Size; i++) {
				if (m_Data[i].First() == _key)
					return true;
			}
			return false;
		}

		bool TryGetValue(const TKey& _key, TVal& _outValue) const
		{
			for (size_t i = 0; i < m_Size; i++) {
				if (m_Data[i].First() == _key)
					_outValue = m_Data[i].Second();
					return true;
			}
			return false;
		}

		virtual void Clear()
		{
			m_Data = nullptr;
			m_Size = 0;
		}

		// Accessors
		constexpr inline size_t Size() const { return m_Size; }
		constexpr inline size_t Capacity() const { return m_Capacity; }

		// Iterator
		constexpr virtual Iterator begin()
		{
			return Iterator(m_Data);
		}

		constexpr virtual Iterator end()
		{
			return Iterator(m_Data + m_Size);
		}

		// Operator Overloads
		const TKey& operator[](const TKey& _key) const
		{
			TVal out;
			for (size_t i = 0; i < m_Size; i++) {
				if (m_Data[i].First() == _key) {
					return m_Data[i].Second();
				}
			}
			__debugbreak();
			return out;
		}

		TVal& operator[](const TKey& _key)
		{
			TVal out;
			for (size_t i = 0; i < m_Size; i++) {
				if (m_Data[i].First() == _key) {
					return m_Data[i].Second();
				}
			}
			__debugbreak();
			return out;
		}

		void operator=(const Map<TKey, TVal>& _other)
		{
			m_Data = _other.m_Data;
			m_Size = _other.m_Size;
			m_Capacity = _other.m_Capacity;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Map<TKey, TVal>& _current)
		{
			_stream << "[ ";
			for (size_t i = 0; i < _current.m_Size; i++) {
				_stream << _current.m_Data[i];
				if (i != _current.m_Size - 1)
					_stream << ", ";
			}

			_stream << " ]" << std::endl;
			return _stream;
		}

	private:
		void ReAlloc(size_t _capacity)
		{
			Tuple<TKey, TVal>* newBlock = new Tuple<TKey, TVal>[_capacity];

			if (_capacity < m_Size)
				m_Size = _capacity;

			for (size_t i = 0; i < m_Size; i++) {
				newBlock[i] = m_Data[i];
			}

			delete[] m_Data;
			m_Data = newBlock;
			m_Capacity = _capacity;
		}

	private:
		Tuple<TKey, TVal>* m_Data = nullptr;

		size_t m_Size = 0;
		size_t m_Capacity = 0;
	};

}