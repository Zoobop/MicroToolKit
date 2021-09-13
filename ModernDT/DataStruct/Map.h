#pragma once

#include "Tuple.h"
#include "Container.h"

namespace mdt {

	template<typename _TKey, typename _TVal>
	class Map
	{
	public:
		using _Type = Tuple<_TKey, _TVal>;
		using ValueType = _Type;
		using Iterator = ContainerIterator<Map<_TKey, _TVal>>;

	public:
		Map()
		{
			ReAlloc(2);
		}

		Map(size_t _size)
			: m_Capacity(_size)
		{
			ReAlloc(m_Capacity);
		}

		Map(const Map<_TKey, _TVal>& _other)
			: m_Capacity(_other.m_Capacity)
		{
			m_Data = _other.m_Data;
		}

		Map(Map<_TKey, _TVal>&& _other)
			: m_Capacity(_other.m_Capacity)
		{
			m_Data = _other.m_Data;
			_other.m_Data = nullptr;
		}

		~Map()
		{
			Clear();
			Delete(m_Data, m_Capacity);
		}

		// Utility
		bool Add(const _TKey& _key, const _TVal& _value)
		{
			if (!ContainsKey(_key)) {
				if (m_Size >= m_Capacity) {
					ReAlloc(m_Capacity + m_Capacity / 2);
				}

				new(&m_Data[m_Size]) _Type(_key, _value);
				m_Size++;

				return true;
			}
			return false;
		}

		bool Add(_TKey&& _key, _TVal&& _value)
		{
			if (!ContainsKey(_key)) {
				if (m_Size >= m_Capacity) {
					ReAlloc(m_Capacity + m_Capacity / 2);
				}

				new(&m_Data[m_Size]) _Type(_key, _value);
				m_Size++;

				return true;
			}
			return false;
		}


		bool Add(const _Type& _tuple)
		{
			if (m_Size >= m_Capacity) {
				ReAlloc(m_Capacity + m_Capacity / 2);
			}

			if (!ContainsKey(_tuple.First())) {
				m_Data[m_Size] = _tuple;
			}
			else {
				for (size_t i = 0; i < m_Size; i++) {
					if (m_Data[i].First() == _tuple.First()) {
						m_Data[i] = _tuple;
						break;
					}
				}
			}
			m_Size++;
			return true;
		}

		bool Add(_Type&& _tuple)
		{
			if (m_Size >= m_Capacity) {
				ReAlloc(m_Capacity + m_Capacity / 2);
			}

			if (!ContainsKey(_tuple.First())) {
				new(&m_Data[m_Size]) _Type(_tuple);
			}
			else {
				for (size_t i = 0; i < m_Size; i++) {
					if (m_Data[i].First() == _tuple.First()) {
						m_Data[i] = std::move(_tuple);
						break;
					}
				}
			}
			m_Size++;
			return true;
		}

		template<typename ... _Args>
		_Type& Emplace(_Args&&... args)
		{
			if (m_Size >= m_Capacity) {
				ReAlloc(m_Capacity + m_Capacity / 2);
			}

			new(&m_Data[m_Size]) _Type(std::forward<_Args>(args)...);
			return m_Data[m_Size++];
		}

		bool RemoveKey(const _TKey& _key)
		{
			if (ContainsKey(_key)) {
				size_t currentIndex = 0;
				while (currentIndex < m_Size) {
					if (m_Data[currentIndex].First() == _key) {
						currentIndex++;
						while (currentIndex < m_Size) {
							m_Data[currentIndex - 1] = std::move(m_Data[currentIndex]);
							currentIndex++;
						}
					}
					currentIndex++;
				}

				m_Size--;
				m_Data[m_Size].~_Type();
				return true;

			}
			return false;
		}

		bool RemoveKey(_TKey&& _key)
		{
			if (ContainsKey(_key)) {
				size_t currentIndex = 0;
				while (currentIndex < m_Size) {
					if (m_Data[currentIndex].First() == _key) {
						currentIndex++;
						while (currentIndex < m_Size) {
							m_Data[currentIndex - 1] = std::move(m_Data[currentIndex]);
							currentIndex++;
						}
					}
					currentIndex++;
				}

				m_Size--;
				m_Data[m_Size].~_Type();
				return true;

			}
			return false;
		}

		bool ContainsKey(const _TKey& _key) const
		{
			for (size_t i = 0; i < m_Size; i++) {
				if (m_Data[i].First() == _key)
					return true;
			}
			return false;
		}

		bool ContainsKey(_TKey&& _key) const
		{
			for (size_t i = 0; i < m_Size; i++) {
				if (m_Data[i].First() == _key)
					return true;
			}
			return false;
		}

		bool TryGetValue(const _TKey& _key, _TVal& _outValue) const
		{
			for (size_t i = 0; i < m_Size; i++) {
				if (m_Data[i].First() == _key)
					_outValue = m_Data[i].Second();
					return true;
			}
			return false;
		}

		bool TryGetValue(_TKey&& _key, _TVal& _outValue) const
		{
			for (size_t i = 0; i < m_Size; i++) {
				if (m_Data[i].First() == _key) {
					_outValue = std::move(m_Data[i].Second());
					return true;
				}
			}
			return false;
		}

		void Clear()
		{
			for (size_t i = 0; i < m_Size; i++)
				m_Data[i].~_Type();

			m_Size = 0;
		}

		// Accessors
		constexpr inline size_t Size() const { return m_Size; }
		constexpr inline size_t Capacity() const { return m_Capacity; }

		// Iterator
		constexpr Iterator begin()
		{
			return Iterator(m_Data);
		}

		constexpr Iterator end()
		{
			return Iterator(m_Data + m_Size);
		}

		// Operator Overloads
		const _TKey& operator[](const _TKey& _key) const
		{
			_TVal out;
			for (size_t i = 0; i < m_Size; i++) {
				if (m_Data[i].First() == _key) {
					return m_Data[i].Second();
				}
			}
			__debugbreak();
			return out;
		}

		_TVal& operator[](const _TKey& _key)
		{
			_TVal out;
			for (size_t i = 0; i < m_Size; i++) {
				if (m_Data[i].First() == _key) {
					return m_Data[i].Second();
				}
			}
			__debugbreak();
			return out;
		}

		void operator=(const Map<_TKey, _TVal>& _other)
		{
			m_Data = _other.m_Data;
			m_Size = _other.m_Size;
			m_Capacity = _other.m_Capacity;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Map<_TKey, _TVal>& _current)
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
			_Type* newBlock = Alloc<_Type>(_capacity);

			if (_capacity < m_Size)
				m_Size = _capacity;

			for (size_t i = 0; i < m_Size; i++) {
				newBlock[i] = std::move(m_Data[i]);
			}

			for (size_t i = 0; i < m_Size; i++)
				m_Data[i].~_Type();

			Delete(m_Data, m_Capacity * sizeof(_Type));
			m_Data = newBlock;
			m_Capacity = _capacity;
		}

	private:
		_Type* m_Data = nullptr;

		size_t m_Size = 0;
		size_t m_Capacity = 0;
	};

}