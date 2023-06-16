#pragma once
#include <ostream>

#include "Common/Sequence.hpp"
#include "Interfaces/IList.h"
#include "Utility/Range.h"

namespace mtk
{
	template <typename T>
	class List final : public Sequence<T>
	{
	public:
		using Base = Sequence<T>;

		// Constructors/Destructors
		List() : Base{}
		{
			Reallocate(c_DefaultCapacity);
		}

		List(const List& other) : Base{other}, m_Size(other.m_Size)
		{
		}

		List(List&& other) noexcept : Base{std::move(other)}, m_Size(other.m_Size)
		{
		}

		List(const Base& other) : Base{other}, m_Size(other.m_Size)
		{
		}

		List(Base&& other) noexcept
			: Base{std::move(other)}, m_Size(other.m_Size)
		{
		}

		List(std::initializer_list<T>&& initializerList)
			: Base{std::move(initializerList)}, m_Size(initializerList.size())
		{
		}

		explicit List(size_t capacity) : Base{}
		{
			Reallocate(capacity);
		}

		// Utility
		void Add(const T& value)
		{
			if (Base::m_Capacity >= m_Size)
				Reallocate(Base::m_Capacity * 2);

			Base::m_Data[m_Size++] = value;
		}

		void Add(T&& value)
		{
			if (Base::m_Capacity >= m_Size)
				Reallocate(Base::m_Capacity * 2);

			Base::m_Data[m_Size++] = std::move(value);
		}

		template <typename... Args>
		T& Emplace(Args&&... args) noexcept
		{
			if (Base::m_Capacity >= m_Size)
				Reallocate(Base::m_Capacity * 2);

			new(&Base::m_Data[m_Size]) T(std::forward<Args>(args)...);
			return Base::m_Data[m_Size++];
		}

		void AddRange(const Base& sequence)
		{
			if (sequence.IsEmpty()) return;

			const size_t size = sequence.Capacity();
			const T* data = sequence.Data();
			for (size_t i = 0; i < size; i++)
				Add(data[i]);
		}

		void AddRange(Base&& sequence)
		{
			if (sequence.IsEmpty()) return;

			const size_t size = sequence.Capacity();
			const T* data = sequence.Data();
			for (size_t i = 0; i < size; i++)
				Add(std::move(data[i]));
		}

		void AddRange(std::initializer_list<T>&& initializerList)
		{
			if (initializerList.size() == 0) return;

			for (auto& item : initializerList)
				Add(std::move(item));
		}

		void Insert(const size_t index, const T& value)
		{
			if (m_Size <= index || index < 0)
				throw ArgumentOutOfRangeException(NAMEOF(index), index);

			Base::m_Data[index] = value;
		}

		void Insert(const size_t index, T&& value)
		{
			if (m_Size <= index || index < 0)
				throw ArgumentOutOfRangeException(NAMEOF(index), index);

			Base::m_Data[index] = std::move(value);
		}

		void InsertRange(size_t startIndex, const Base& sequence)
		{
			if (sequence.IsEmpty()) return;

			if (m_Size <= startIndex || startIndex < 0)
				throw ArgumentOutOfRangeException(NAMEOF(startIndex), startIndex);

			for (auto& item : sequence)
			{
				Insert(startIndex++, item);
			}
		}

		void InsertRange(size_t startIndex, Sequence<T>&& sequence)
		{
			if (sequence.IsEmpty()) return;

			if (m_Size <= startIndex || startIndex < 0)
				throw ArgumentOutOfRangeException(NAMEOF(startIndex), startIndex);

			for (auto& item : sequence)
				Insert(startIndex++, std::move(item));
		}

		void InsertRange(size_t startIndex, std::initializer_list<T>&& initializerList)
		{
			if (initializerList.size() == 0) return;

			if (m_Size <= startIndex || startIndex < 0)
				throw ArgumentOutOfRangeException(NAMEOF(startIndex), startIndex);

			for (auto& item : initializerList)
				Insert(startIndex++, std::move(item));
		}

		bool Remove(const T& _value)
		{
			// Return if empty
			if (Sequence<T>::IsEmpty()) return false;

			// Get and return if invalid index
			const int32_t index = Sequence<T>::IndexOf(this->AsSequence(), _value);
			if (index == -1) return false;

			// Shift items down
			const size_t size = Sequence<T>::Capacity();
			for (size_t i = index + 1; i < size; ++i)
			{
				Sequence<T>::m_Data[i - 1] = std::move(Sequence<T>::m_Data[i]);
			}

			// Decrement size
			--Sequence<T>::m_Capacity;
			return true;
		}

		uint32_t RemoveAll(const Predicate<T>& _predicate)
		{
			// TODO: Finish implementation
			uint32_t count = 0;
			const T* data = Sequence<T>::m_Data;
			const size_t size = Sequence<T>::m_Capacity;

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

			Sequence<T>::m_Capacity -= count;
			return count;
		}

		void RemoveAt(size_t _index) override
		{
			if (_index >= Sequence<T>::m_Capacity) return;

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
			for (size_t i = 0; i < Sequence<T>::m_Capacity; i++)
			{
				if (_predicate(Sequence<T>::m_Data[i])) return true;
			}

			return false;
		}

		NODISCARD T* Find(const Predicate<T>& _predicate) const
		{
			T item = nullptr;
			for (size_t i = 0; i < Sequence<T>::m_Capacity; i++)
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
			for (size_t i = 0; i < Sequence<T>::m_Capacity; i++)
			{
				const T& elem = Sequence<T>::m_Data[i];
				if (_predicate(elem))
				{
					index = static_cast<int32_t>(i);
					break;
				}
			}
			return index;
		}

		NODISCARD T* FindLast(const Predicate<T>& _predicate) const
		{
			T item = nullptr;
			for (size_t i = 0; i < Sequence<T>::m_Capacity; i++)
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
			for (size_t i = 0; i < Sequence<T>::m_Capacity; i++)
			{
				const T& elem = Sequence<T>::m_Data[i];
				if (_predicate(elem))
				{
					index = static_cast<int32_t>(i);
				}
			}
			return index;
		}

		NODISCARD List<T> FindAll(const Predicate<T>& _predicate) const
		{
			List<T> list(Sequence<T>::m_Capacity);
			for (size_t i = 0; i < Sequence<T>::m_Capacity; i++)
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
			for (size_t i = 0; i < Sequence<T>::m_Capacity; i++)
			{
				Sequence<T>::m_Data[i].~T();
			}

			Sequence<T>::m_Capacity = 0;
		}

		// Accessors
		NODISCARD constexpr size_t Capacity() const override { return m_Capacity; }
		NODISCARD constexpr void Reserve(size_t _capacity) override { Reallocate(_capacity); }

		// Operator Overloads
		const T& operator[](size_t _index) const
		{
			if (_index >= m_Capacity)
			{
				DEBUG_BREAK();
			}
			return Sequence<T>::m_Data[_index];
		}

		Sequence<T> operator[](Range _range) const
		{
			if (_range.end >= m_Capacity || _range.start >= _range.end)
			{
				DEBUG_BREAK();
			}

			return {Sequence<T>::m_Data + _range.start, _range.end - _range.start};
		}

		List<T>& operator=(const List<T>& _other)
		{
			const size_t size = _other.m_Capacity;
			if (size == 0) return *this;

			if (!Sequence<T>::IsEmpty())
			{
				Reallocate(_other.m_Capacity);
			}
			else
			{
				Allocate(_other.m_Capacity);
			}

			Sequence<T>::m_Capacity = size;
			memcpy_s(Sequence<T>::m_Data, m_Capacity, _other.Data(), m_Capacity);
			return *this;
		}

		List<T>& operator=(List<T>&& _other) noexcept
		{
			const size_t size = _other.m_Capacity;
			if (size == 0) return *this;

			if (!Sequence<T>::IsEmpty())
			{
				Reallocate(_other.m_Capacity);
			}
			else
			{
				Allocate(_other.m_Capacity);
			}

			Sequence<T>::m_Capacity = size;
			memmove_s(Sequence<T>::m_Data, m_Capacity, _other.Data(), m_Capacity);
			_other.Clear();
			return *this;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const List<T>& _current)
		{
			_stream << "[ ";
			for (size_t i = 0; i < _current.m_Capacity; i++)
			{
				_stream << _current.m_Data[i];
				if (i != _current.m_Capacity - 1)
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
			if (Sequence<T>::m_Capacity == _capacity) return;

			T* newBlock = new T[_capacity];

			if (_capacity < Sequence<T>::m_Capacity)
			{
				Sequence<T>::m_Capacity = _capacity;
			}

			for (size_t i = 0; i < Sequence<T>::m_Capacity; i++)
			{
				newBlock[i] = std::move(Sequence<T>::m_Data[i]);
			}

			for (size_t i = 0; i < Sequence<T>::m_Capacity; i++)
			{
				Sequence<T>::m_Data[i].~T();
			}

			delete[] Sequence<T>::m_Data;
			Sequence<T>::m_Data = newBlock;
			m_Capacity = _capacity;
		}

	private:
		size_t m_Size = 0;
		static constexpr size_t c_DefaultCapacity = 4;
	};
}
