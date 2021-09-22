#pragma once

#include "Interfaces/IStruct.h"

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
		}

		Array(std::initializer_list<T>&& _initList)
		{
			size_t i = 0;
			for (auto&& item : _initList) {
				if (i < S) {
					m_Data[i] = std::move(item);
					i++;
				}
				else break;
			}
		}

		// Utility
		bool Fill(const T& _value)
		{
			for (size_t i = 0; i < S; i++) {
				m_Data[i] = _value;
			}
			return true;
		}

		bool Fill(T&& _value)
		{
			for (size_t i = 0; i < S; i++) {
				m_Data[i] = std::move(_value);
			}
			return true;
		}

		bool Set(std::initializer_list<T>&& _initList)
		{
			size_t i = 0;
			for (const auto& _item : _initList) {
				m_Data[i] = std::move(_item);
				i++;
			}
			return true;
		}

		bool Set(const IContainer<T>& _container)
		{
			size_t size = _container.Capacity();
			for (size_t i = 0; i < size; i++) {
				m_Data[i] = _container.Data()[i];
			}
			return true;
		}

		bool Swap(Array<T, S>& _other)
		{
			return false;
		}

		bool Contains(const T& _value) const
		{
			for (size_t i = 0; i < S; i++) {
				if (m_Data[i] == _value)
					return true;
			}
			return false;
		}

		bool Contains(T&& _value) const
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
				new(&m_Data[S]) T();
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
		T& operator[](const size_t& _index)
		{
			if (_index >= S) {
				__debugbreak();
			}
			return m_Data[_index];
		}

		const T& operator[](const size_t& _index) const
		{
			if (_index >= S) {
				__debugbreak();
			}
			return m_Data[_index];
		}

		void operator=(std::initializer_list<T>&& _initList)
		{
			size_t i = 0;
			for (auto& item : _initList) {
				if (i < S) {
					m_Data[i] = std::move(item);
					i++;
				}
				else break;
			}
		}

		void operator=(const Array<T, S>& _array)
		{
			for (size_t i = 0; i < S; i++) {
				m_Data[i] = _array.m_Data[i];
			}
		}

		void operator=(Array<T, S>&& _array)
		{
			for (size_t i = 0; i < S; i++) {
				m_Data[i] = _array.m_Data[i];
			}
			_array.Clear();
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
		T Verify(std::initializer_list<T>&& _initList)
		{
			if (_initList.size() <= S) {
				return _initList;
			}

			T list[S];
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