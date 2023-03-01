#pragma once
#include <ostream>

#include "Collections/Sequence.h"
#include "Interfaces/IList.h"
#include "Utility/Range.h"

namespace mtk
{
	template<typename T>
	class List final : public Sequence<T>, public IList<T>
	{
	public:
		List()
			: Sequence<T> { }
		{
			Reallocate(c_DefaultCapacity);
		}

		List(const List<T>& _other)
			: Sequence<T> { _other }, IList<T>{ }, m_Capacity(_other.m_Capacity)
		{
		}

		List(List<T>&& _other) noexcept
			: Sequence<T> { std::move(_other) }, IList<T>{ }, m_Capacity(_other.m_Capacity)
		{
		}

		List(const Sequence<T>& _other)
			: Sequence<T> { _other }, IList<T>{ }, m_Capacity(_other.Size())
		{
		}

		List(Sequence<T>&& _other) noexcept
			: Sequence<T> { std::move(_other) }, IList<T>{ }, m_Capacity(_other.Size())
		{
		}

		List(const std::vector<T>& _vector)
			: Sequence<T> { _vector.data(), _vector.size(), _vector.capacity() }, IList<T>{ }, m_Capacity(_vector.capacity())
		{
		}

		List(std::vector<T>&& _vector)
			: Sequence<T> { (T*&&) _vector.data(), _vector.size() }, IList<T>{ }, m_Capacity(_vector.capacity())
		{
			_vector.clear();
		}

		List(std::initializer_list<T>&& _initList)
			: Sequence<T> { std::move(_initList) }, IList<T>{ }, m_Capacity(_initList.size())
		{
		}

		explicit List(size_t _capacity)
			: Sequence<T> { }, IList<T>{ }
		{
			Reallocate(_capacity);
		}

		// Utility
		void Add(const T& _value) override
		{
			if (Sequence<T>::m_Size >= m_Capacity) {
				Reallocate(m_Capacity + m_Capacity);
			}

			Sequence<T>::m_Data[Sequence<T>::m_Size++] = _value;
		}

		void Add(T&& _value) override
		{
			if (Sequence<T>::m_Size >= m_Capacity) {
				Reallocate(m_Capacity + m_Capacity);
			}

			Sequence<T>::m_Data[Sequence<T>::m_Size++] = std::move(_value);
		}

		template<typename ... _Args>
		T& Emplace(_Args&&... args)
		{
			if (Sequence<T>::m_Size >= m_Capacity) {
				Reallocate(m_Capacity + m_Capacity);
			}

			new(&Sequence<T>::m_Data[Sequence<T>::m_Size]) T(std::forward<_Args>(args)...);
			return Sequence<T>::m_Data[Sequence<T>::m_Size++];
		}

		void AddRange(const Sequence<T>& _sequence)
		{
			if (_sequence.IsEmpty()) return;

			const size_t size = _sequence.Size();
			const T* data = _sequence.Data();
			for (size_t i = 0; i < size; i++)
			{
				Add(data[i]);
			}
		}

		void AddRange(Sequence<T>&& _sequence)
		{
			if (_sequence.IsEmpty()) return;

			const size_t size = _sequence.Size();
			const T* data = _sequence.Data();
			for (size_t i = 0; i < size; i++)
			{
				Add(std::move(data[i]));
			}
		}

		void AddRange(std::initializer_list<T>&& _initList)
		{
			if (_initList.size() == 0) return;

			for (auto& item : _initList)
			{
				Add(std::move(item));
			}
		}

		void Insert(size_t _index, const T& _value) override
		{
			if (_index >= Sequence<T>::m_Size) {
				DEBUG_BREAK();
			}
			
			Sequence<T>::m_Data[_index] = _value;
		}
		
		void Insert(size_t _index, T&& _value) override
		{
			if (_index >= Sequence<T>::m_Size) {
				DEBUG_BREAK();
			}
			
			Sequence<T>::m_Data[_index] = std::move(_value);
		}

		void InsertRange(size_t _index, const Sequence<T>& _sequence)
		{
			if (_sequence.Size() == 0) return;

			for (auto& item : _sequence)
			{
				Insert(_index++, item);
			}
		}

		void InsertRange(size_t _index, Sequence<T>&& _sequence)
		{
			if (_sequence.Size() == 0) return;

			for (auto& item : _sequence)
			{
				Insert(_index++, std::move(item));
			}
		}

		void InsertRange(size_t _index, std::initializer_list<T>&& _initList)
		{
			if (_initList.size() == 0) return;

			for (auto& item : _initList)
			{
				Insert(_index++, std::move(item));
			}
		}
		
		bool Remove(const T& _value) override
		{
			// Return if empty
			if (Sequence<T>::IsEmpty()) return false;

			// Get and return if invalid index
			const int32_t index = Sequence<T>::IndexOf(this->AsSequence(), _value);
			if (index == -1) return false;

			// Shift items down
			const size_t size = Sequence<T>::Size();
			for (size_t i = index + 1; i < size; ++i)
			{
				Sequence<T>::m_Data[i - 1] = std::move(Sequence<T>::m_Data[i]);
			}

			// Decrement size
			--Sequence<T>::m_Size;
			return true;
		}

		uint32_t RemoveAll(const Predicate<T>& _predicate)
		{
			// TODO: Finish implementation
			uint32_t count = 0;
			const T* data = Sequence<T>::m_Data;
			const size_t size = Sequence<T>::m_Size;
			
			// Find indexes of elements matching predicate
			for (size_t i = 0; i < size; i++)
			{
				const T& elem = data[i];
				if (_predicate(elem))
				{
					const size_t index1 = i;
					const size_t index2 = i + 1;
					if (index2 < size)
					{
						const T copy = Sequence<T>::m_Data[index2];
						Sequence<T>::m_Data[index1] = copy;
					}
					
					count++;
				}
			}

			Sequence<T>::m_Size -= count;
			return count;
		}

		void RemoveAt(size_t _index) override
		{
			if (_index >= Sequence<T>::m_Size) return;
			
			Remove(Sequence<T>::m_Data[_index]);
		}

		void RemoveRange(size_t _index, size_t _count)
		{
			// TODO: Finish implementation
		}

		NODISCARD bool Contains(const T& _value) const override
		{
			return Sequence<T>::Contains(this->AsSequence(), _value);
		}

		NODISCARD bool Exists(const Predicate<T>& _predicate) const
		{
			for (size_t i = 0 ; i < Sequence<T>::m_Size; i++)
			{
				if (_predicate(Sequence<T>::m_Data[i])) return true;
			}
			
			return false;
		}

		NODISCARD T* Find(const Predicate<T>& _predicate) const
		{
			T item = nullptr;
			for (size_t i = 0 ; i < Sequence<T>::m_Size; i++)
			{
				const T& elem = Sequence<T>::m_Data[i];
				if (_predicate(elem))
				{
					item = elem;
					break;
				}
			}
			return item;
		}

		NODISCARD int32_t FindIndex(const Predicate<T>& _predicate) const
		{
			int32_t index = -1;
			for (size_t i = 0 ; i < Sequence<T>::m_Size; i++)
			{
				const T& elem = Sequence<T>::m_Data[i];
				if (_predicate(elem))
				{
					index = (int32_t) i;
					break;
				}
			}
			return index;
		}

		NODISCARD T* FindLast(const Predicate<T>& _predicate) const
		{
			T item = nullptr;
			for (size_t i = 0 ; i < Sequence<T>::m_Size; i++)
			{
				const T& elem = Sequence<T>::m_Data[i];
				if (_predicate(elem))
				{
					item = elem;
				}
			}
			
			return item;
		}

		NODISCARD int32_t FindLastIndex(const Predicate<T>& _predicate) const
		{
			int32_t index = -1;
			for (size_t i = 0 ; i < Sequence<T>::m_Size; i++)
			{
				const T& elem = Sequence<T>::m_Data[i];
				if (_predicate(elem))
				{
					index = (int32_t) i;
				}
			}
			return index;
		}

		NODISCARD List<T> FindAll(const Predicate<T>& _predicate) const
		{
			List<T> list(Sequence<T>::m_Size);
			for (size_t i = 0 ; i < Sequence<T>::m_Size; i++)
			{
				const T& elem = Sequence<T>::m_Data[i];
				if (_predicate(elem))
				{
					list.Add(elem);
				}
			}
			return list;
		}

		void Clear() override
		{
			for (size_t i = 0; i < Sequence<T>::m_Size; i++)
			{
				Sequence<T>::m_Data[i].~T();
			}

			Sequence<T>::m_Size = 0;
		}

		// Accessors
		NODISCARD constexpr size_t Capacity() const override { return m_Capacity; }
		NODISCARD constexpr void Reserve(size_t _capacity) override { Reallocate(_capacity); }

		// Operator Overloads
		const T& operator[](size_t _index) const 
		{
			if (_index >= m_Capacity) {
				DEBUG_BREAK();
			}
			return Sequence<T>::m_Data[_index];
		}

		Sequence<T> operator[](Range _range) const 
		{
			if (_range.end >= m_Capacity || _range.start >= _range.end) {
				DEBUG_BREAK();
			}
			
			return { Sequence<T>::m_Data + _range.start, _range.end - _range.start };
		}

		List<T>& operator=(const List<T>& _other)
		{
			const size_t size = _other.m_Size;
			if (size == 0) return *this;
			
			if (!Sequence<T>::IsEmpty())
			{
				Reallocate(_other.m_Capacity);
			}
			else
			{
				Allocate(_other.m_Capacity);
			}

			Sequence<T>::m_Size = size;
			memcpy_s(Sequence<T>::m_Data, m_Capacity, _other.Data(), m_Capacity);
			return *this;
		}

		List<T>& operator=(List<T>&& _other) noexcept
		{
			const size_t size = _other.m_Size;
			if (size == 0) return *this;
			
			if (!Sequence<T>::IsEmpty())
			{
				Reallocate(_other.m_Capacity);
			}
			else
			{
				Allocate(_other.m_Capacity);
			}

			Sequence<T>::m_Size = size;
			memmove_s(Sequence<T>::m_Data, m_Capacity, _other.Data(), m_Capacity);
			_other.Clear();
			return *this;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const List<T>& _current)
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

	protected:
		void Allocate(size_t _capacity) override
		{
			if (m_Capacity == _capacity) return;
			
			Sequence<T>::m_Data = new T[_capacity];
			m_Capacity = _capacity;
		}
		
		void Reallocate(size_t _capacity) override
		{
			if (m_Capacity == _capacity) return;
			
			T* newBlock = new T[_capacity];

			if (_capacity < Sequence<T>::m_Size)
			{
				Sequence<T>::m_Size = _capacity;
			}

			for (size_t i = 0; i < Sequence<T>::m_Size; i++)
			{
				newBlock[i] = std::move(Sequence<T>::m_Data[i]);
			}

			for (size_t i = 0; i < Sequence<T>::m_Size; i++)
			{
				Sequence<T>::m_Data[i].~T();
			}

			delete[] Sequence<T>::m_Data;
			Sequence<T>::m_Data = newBlock;
			m_Capacity = _capacity;
		}

	private:
		size_t m_Capacity = 0;
		static constexpr size_t c_DefaultCapacity = 4;
	};

}
