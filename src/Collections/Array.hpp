#pragma once

#include "Common/Sequence.hpp"

namespace mtk
{
	template <typename T, size_t TSize>
	class Array final : public Memory<T>
	{
	public:
		using Iterator = Iterator<T>;
		using ConstIterator = const Iterator;

		// Constructors/Destructors
		Array() = default;

		Array(const Array& other)
		{
			for (size_t i = 0; i < TSize; i++)
				m_Data[i] = other.m_Data[i];
		}

		Array(Array&& other) noexcept
		{
			for (size_t i = 0; i < TSize; i++)
				m_Data[i] = std::move(other.m_Data[i]);
		}

		Array(std::initializer_list<T>&& data) noexcept
		{
			Set(std::move(data));
		}

		~Array() override = default;

		// Utility
		void Initialize()
		{
			for (size_t i = 0; i < TSize; i++)
				new(&m_Data[i]) T();
		}

		void Fill(const T& value)
		{
			for (size_t i = 0; i < TSize; i++)
				m_Data[i] = value;
		}

		void Set(std::initializer_list<T>&& initList) noexcept
		{
			size_t index = 0;
			for (auto& elem : initList)
			{
				if (index == TSize)
					return;

				m_Data[index++] = std::move(const_cast<T&&>(elem));
			}
		}

		void Set(const Sequence<T>& container)
		{
			const size_t size = MIN(container.Capacity(), TSize);
			const T* data = container.Data();
			for (size_t i = 0; i < size; i++)
				m_Data[i] = data[i];
		}

		void Swap(Array& other) noexcept
		{
			for (size_t i = 0; i < TSize; i++)
			{
				auto temp = std::move(m_Data[i]);
				m_Data[i] = std::move(other.m_Data[i]);
				other.m_Data[i] = std::move(temp);
			}
		}

		NODISCARD bool Contains(const T& value) const
		{
			for (size_t i = 0; i < TSize; i++)
			{
				if (m_Data[i] == value)
					return true;
			}
			return false;
		}

		NODISCARD bool IsEmpty() const override
		{
			return false;
		}

		// Accessors
		NODISCARD constexpr const T* Data() const override { return reinterpret_cast<const T*>(&m_Data); }
		NODISCARD constexpr size_t Capacity() const override { return TSize; }

		// Iterator
		NODISCARD constexpr Iterator begin() { return Iterator(m_Data); }
		NODISCARD constexpr Iterator end() { return Iterator(m_Data + TSize); }
		NODISCARD constexpr ConstIterator begin() const { return {m_Data}; }
		NODISCARD constexpr ConstIterator end() const { return {m_Data + TSize}; }

		// Operator Overloads
		T& operator[](const size_t index)
		{
			if (index >= TSize || index < 0)
				throw IndexOutOfRangeException(index);

			return m_Data[index];
		}

		const T& operator[](const size_t index) const
		{
			if (index >= TSize || index < 0)
				throw IndexOutOfRangeException(index);

			return m_Data[index];
		}

		Array& operator=(std::initializer_list<T>&& initList) noexcept
		{
			Set(std::move(initList));
			return *this;
		}

		Array& operator=(const Array& array)
		{
			if (this == &array)
				return *this;

			for (size_t i = 0; i < TSize; i++)
				m_Data[i] = array.m_Data[i];

			return *this;
		}

		Array& operator=(Array&& array) noexcept
		{
			if (this == &array)
				return *this;

			for (size_t i = 0; i < TSize; i++)
				m_Data[i] = std::move(array.m_Data[i]);

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const Array& current)
		{
			stream << '[';
			for (size_t i = 0; i < TSize; i++)
			{
				stream << current.m_Data[i];
				if (i != TSize - 1)
					stream << ", ";
			}

			stream << ']';
			return stream;
		}

	private:
		T m_Data[TSize];
	};
}
