#pragma once
#include "Interfaces/IStruct.h"
#include "Interfaces/IExtendable.h"

namespace mtk {

	template<typename _Type>
	class List : public IStruct<_Type>, public IExtendable<_Type>
	{
	public:
		using Iterator = IStruct<_Type>::Iterator;

		friend class IExtendable<_Type>;

	public:
		List() 
		{
			ReAlloc(2);
		}

		List(size_t _size)
			: m_Capacity(_size)
		{
			ReAlloc(m_Capacity);
		}

		List(std::initializer_list<_Type>&& _initList)
		{
			size_t size = 0;
			for (const auto& _item : _initList)
				size++;

			ReAlloc(size);
			for (const auto& _item : _initList)
				Add(std::move(_item));
		}

		List(const std::vector<_Type>& _vector)
			: m_Data(const_cast<_Type*>(_vector.data())), m_Capacity(_vector.capacity())
		{
			_SIZE = _vector.size();
		}

		List(std::vector<_Type>&& _vector)
			: m_Data(const_cast<_Type*>(_vector.data())), m_Capacity(_vector.capacity())
		{
			_SIZE = _vector.size();
			_vector.clear();
		}

		List(const List<_Type>& _other)
			: m_Data(_other.m_Data), m_Capacity(_other.m_Capacity)
		{
			_SIZE = _other.m_Size;
		}

		List(List<_Type>&& _other) noexcept
			: m_Capacity(_other.m_Capacity)
		{
			m_Data = _other.m_Data;
			_SIZE = _other.m_Size;

			_other.m_Data = nullptr;
		}

		~List()
		{
			//Clear();
			Delete(m_Data, m_Capacity);
		}

		// Override Methods
		virtual bool Add(const _Type& _value) override
		{
			if (_SIZE >= m_Capacity) {
				ReAlloc(m_Capacity + m_Capacity / 2);
			}

			m_Data[_SIZE] = _value;
			_SIZE++;

			return true;
		}

		virtual bool Add(_Type&& _value) override
		{
			if (_SIZE >= m_Capacity) {
				ReAlloc(m_Capacity + m_Capacity / 2);
			}

			m_Data[_SIZE] = std::move(_value);
			_SIZE++;

			return true;
		}

		template<typename ... _Args>
		_Type& Emplace(_Args&&... args)
		{
			if (_SIZE >= m_Capacity) {
				ReAlloc(m_Capacity + m_Capacity / 2);
			}

			new(&m_Data[_SIZE]) _Type(std::forward<_Args>(args)...);
			return m_Data[_SIZE++];
		}

		virtual bool AddRange(const IDataHandler<_Type>& _container) override
		{
			if (_container.Data()) {
				size_t size = _container.Capacity();
				for (size_t i = 0; i < size; i++)
					Add(_container.Data()[i]);
				return true;
			}
			return false;
		}

		virtual bool AddRange(std::initializer_list<_Type>&& _initList)
		{
			if (_initList.size() > 0) {
				for (auto& item : _initList)
					Add(std::move(item));
				return true;
			}
			return false;
		}

		virtual bool AddRange(const std::vector<_Type>& _vector)
		{
			if (_vector.data()) {
				size_t size = _vector.size();
				for (size_t i = 0; i < size; i++)
					Add(_vector.data()[i]);
				return true;
			}
			return false;
		}

		virtual bool Remove(const _Type& _value) override
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
				return true;

			}
			return false;
		}

		virtual bool Remove(_Type&& _value) override
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

		virtual bool Contains(const _Type& _value) const override
		{
			for (size_t i = 0; i < _SIZE; i++) {
				if (m_Data[i] == _value)
					return true;
			}
			return false;
		}

		virtual bool Contains(_Type&& _value) const override
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
				m_Data[i].~_Type();

			_SIZE = 0;
		}

		// IExtendable
		virtual void ForEach(const Action<const _Type&>& _param) override
		{
			for (size_t i = 0; i < _SIZE; i++) {
				_param(m_Data[i]);
			}
		}

		constexpr virtual const _Type* Data() const override { return m_Data; }

		constexpr virtual _Type* Data() override { return m_Data; }

		// Accessors
		constexpr virtual inline size_t Capacity() const override { return m_Capacity; }

		constexpr inline void Reserve(size_t _capacity) { ReAlloc(_capacity); }

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
		const _Type& operator[](size_t _index) const 
		{
			if (_index >= m_Capacity) {
				DEBUG_BREAK();
			}
			return m_Data[_index];
		}

		void operator=(const List<_Type>& _other)
		{
			m_Data = _other.m_Data;
			_SIZE = _other._SIZE;
			m_Capacity = _other.m_Capacity;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const List<_Type>& _current)
		{
			_stream << "[ ";
			for (size_t i = 0; i < _current.m_Size; i++) {
				_stream << _current.m_Data[i];
				if (i != _current.m_Size - 1)
					_stream << ", ";
			}

			_stream << " ]";
			return _stream;
		}

	private:
		void ReAlloc(size_t _capacity)
		{
			_Type* newBlock = (_Type*)Alloc<_Type>(_capacity);

			if (_capacity < _SIZE)
				_SIZE = _capacity;

			for (size_t i = 0; i < _SIZE; i++) {
				newBlock[i] = std::move(m_Data[i]);
			}

			for (size_t i = 0; i < _SIZE; i++)
				m_Data[i].~_Type();

			Delete(m_Data, m_Capacity);
			m_Data = newBlock;
			m_Capacity = _capacity;
		}

	private:
		_Type* m_Data = nullptr;
	
		size_t m_Capacity = 0;
	};

}