#pragma once

#include "Container.h"

namespace mdt {

	template<typename T, size_t S>
	class Array : public IContainer<T>
	{
	public:
		using ValueType = T;
		using Iterator = ContainerIterator<Array<T, S>>;

	public:
		Array()
		{
			Clear();
		}

		Array(const std::initializer_list<T>& _initList)
			: m_Data(Verify(_initList))
		{
		}

		~Array()
		{
			for (size_t i = 0; i < S; i++)
				m_Data[i].~T();
		}

		// Utility
		bool Fill(const T& _value)
		{
			for (size_t i = 0; i < S; i++) {
				m_Data[i] = _value;
			}
			return true;
		}

		bool Contains(const T& _value) const
		{
			for (size_t i = 0; i < S; i++) {
				if (m_Data[i] == _value)
					return true;
			}
			return false;
		}

		void Clear()
		{
			for (size_t i = 0; i < S; i++) {
				m_Data[i].~T();
			}
		}

		// IContainer
		virtual void ForEach(const Param<const T&>& _param) override
		{
			for (size_t i = 0; i < S; i++) {
				_param(m_Data[i]);
			}
		}

		constexpr virtual T* Data() const override { return (T*)m_Data; }

		// Accessors
		constexpr inline size_t Capacity() const override { return S; }

		// Iterator
		constexpr Iterator begin()
		{
			return Iterator(m_Data);
		}
		constexpr Iterator end()
		{
			return Iterator(m_Data + S);
		}

		// Operator Overloads
		T& operator[](size_t _index)
		{
			if (_index >= S) {
				__debugbreak();
			}
			return m_Data[_index];
		}

		const T& operator[](size_t _index) const
		{
			if (_index >= S) {
				__debugbreak();
			}
			return m_Data[_index];
		}

		void operator=(const std::initializer_list<T>& _initList)
		{
			size_t i = 0;
			for (auto& item : _initList) {
				if (i < S) {
					m_Data[i] = item;
					i++;
				}
				else break;
			}
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Array<T, S>& _current)
		{
			_stream << "[ ";
			for (size_t i = 0; i < S; i++) {
				_stream << _current.m_Data[i];
				if (i != S - 1)
					_stream << ", ";
			}

			_stream << " ]" << std::endl;
			return _stream;
		}

	private:
		T* Verify(const std::initializer_list<T>& _initList)
		{
			if (_initList.size() <= S) {
				return _initList;
			}

			T* list = new T[S];
			size_t i = 0;
			for (auto& item : _initList) {
				if (i < S) {
					list[i] = item;
					i++;
				}
				else break;
			}

			return list;
		}

	private:
		T m_Data[S];
	};

}