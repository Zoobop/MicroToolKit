#pragma once

#include "Container.h"

namespace mdt {

	template<typename T>
	class Set : public DataContainer<T>, public IContainer<T>
	{
	public:
		using Iterator = DataContainer<T>::Iterator;

	public:
		friend class IContainer<T>;

	public:
		Set()
		{
			ReAlloc(2);
		}

		Set(size_t _size)
			: m_Capacity(_size)
		{
			ReAlloc(m_Capacity);
		}

		Set(std::initializer_list<T>&& _initList)
		{
			size_t size = 0;
			for (const auto& _item : _initList)
				size++;

			ReAlloc(size);
			for (const auto& _item : _initList)
				Add(std::move((T&&)_item));
		}

		Set(const Set<T>& _other)
			: m_Data(_other.m_Data), m_Capacity(_other.m_Capacity)
		{
			_SIZE = _other.m_Size;
		}

		Set(Set<T>&& _other) noexcept
			: m_Capacity(_other.m_Capacity)
		{
			m_Data = _other.m_Data;
			_SIZE = _other.m_Size;

			_other.m_Data = nullptr;
		}

		~Set()
		{
			Clear();
			Delete(m_Data, m_Capacity * sizeof(T));
		}

		// Override Methods
		virtual bool Add(const T& _value) override
		{
			if (!Contains(_value)) {
				if (_SIZE >= m_Capacity) {
					ReAlloc(m_Capacity + m_Capacity / 2);
				}

				m_Data[_SIZE] = _value;
				_SIZE++;

				return true;
			}
			return false;
		}
		virtual bool Add(T&& _value) override
		{
			if (!Contains(std::move(_value))) {
				if (_SIZE >= m_Capacity) {
					ReAlloc(m_Capacity + m_Capacity / 2);
				}

				m_Data[_SIZE] = std::move(_value);
				_SIZE++;

				return true;
			}
			return false;
		}

		virtual bool AddRange(const IContainer<T>& _container) override
		{
			if (_container.Data()) {
				size_t size = _container.Capacity();
				for (size_t i = 0; i < size; i++)
					Add(_container.Data()[i]);
				return true;
			}
			return false;
		}

		virtual bool AddRange(std::initializer_list<T>&& _initList)
		{
			if (_initList.size() > 0) {
				for (auto& item : _initList)
					Add(std::move(item));
				return true;
			}
			return false;
		}

		virtual bool AddRange(const std::vector<T>& _vector)
		{
			if (_vector.data()) {
				size_t size = _vector.size();
				for (size_t i = 0; i < size; i++)
					Add(_vector.data()[i]);
				return true;
			}
			return false;
		}

		virtual bool Remove(const T& _value) override
		{
			if (_SIZE > 0) {
				size_t currentIndex = 0;
				while (currentIndex < _SIZE) {
					if (m_Data[currentIndex] == _value) {
						currentIndex++;
						while (currentIndex < _SIZE) {
							m_Data[currentIndex - 1] = std::move(m_Data[currentIndex]);
							currentIndex++;
						}
					}
					currentIndex++;
				}

				_SIZE--;
				m_Data[_SIZE].~T();
				return true;

			}
			return false;
		}

		virtual bool Remove(T&& _value) override
		{
			if (_SIZE > 0) {
				size_t currentIndex = 0;
				while (currentIndex < _SIZE) {
					if (m_Data[currentIndex] == _value) {
						currentIndex++;
						while (currentIndex < _SIZE) {
							m_Data[currentIndex - 1] = std::move(m_Data[currentIndex]);
							currentIndex++;
						}
					}
					currentIndex++;
				}

				_SIZE--;
				m_Data[_SIZE].~T();
				return true;

			}
			return false;
		}

		virtual bool RemoveAt(size_t _index) override
		{
			if (_index >= 0 && _index < _SIZE) {
				return Remove(m_Data[_index]);
			}
			return false;
		}

		virtual bool Contains(const T& _value) const override
		{
			for (size_t i = 0; i < _SIZE; i++) {
				if (m_Data[i] == _value)
					return true;
			}
			return false;
		}

		virtual bool Contains(T&& _value) const override
		{
			for (size_t i = 0; i < _SIZE; i++) {
				if (m_Data[i] == _value)
					return true;
			}
			return false;
		}

		virtual void Clear() override
		{
			for (size_t i = 0; i < _SIZE; i++)
				m_Data[i].~T();

			_SIZE = 0;
		}

		// IContainer
		virtual void ForEach(const Param<const T&>& _param) override
		{
			for (size_t i = 0; i < _SIZE; i++) {
				_param(m_Data[i]);
			}
		}

		constexpr virtual T* Data() const override { return m_Data; }

		// Accessors
		constexpr inline size_t Capacity() const override { return m_Capacity; }

		constexpr inline void SetCapacity(size_t _capacity) { m_Capacity = _capacity; }

		// Iterator
		constexpr Iterator begin() override
		{
			return Iterator(m_Data);
		}

		constexpr Iterator end() override
		{
			return Iterator(m_Data + _SIZE);
		}

		// Operator Overloads
		const T& operator[](size_t _index) const
		{
			if (_index >= m_Capacity) {
				__debugbreak();
			}
			return m_Data[_index];
		}

		T& operator[](size_t _index)
		{
			if (_index >= m_Capacity) {
				__debugbreak();
			}
			return m_Data[_index];
		}

		void operator=(const Set<T>& _other)
		{
			m_Data = _other.m_Data;
			_SIZE = _other._SIZE;
			m_Capacity = _other.m_Capacity;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const Set<T>& _current)
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
			T* newBlock = Alloc<T>(_capacity);

			if (_capacity < _SIZE)
				_SIZE = _capacity;

			for (size_t i = 0; i < _SIZE; i++) {
				newBlock[i] = std::move(m_Data[i]);
			}

			for (size_t i = 0; i < _SIZE; i++)
				m_Data[i].~T();

			Delete(m_Data, m_Capacity * sizeof(T));
			m_Data = newBlock;
			m_Capacity = _capacity;
		}

	private:
		T* m_Data = nullptr;

		size_t m_Capacity = 0;
	};


}