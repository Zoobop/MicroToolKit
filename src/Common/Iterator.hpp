#pragma once

namespace mtk
{
	template <typename T>
	class Iterator
	{
	public:
		using ValueType = T;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;

		constexpr Iterator(PointerType ptr) noexcept
			: m_Ptr(ptr)
		{
		}

		Iterator& operator++()
		{
			++m_Ptr;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator iterator = *this;
			++*this;
			return *this;
		}

		Iterator& operator++() const
		{
			++m_Ptr;
			return *this;
		}

		Iterator operator++(int) const
		{
			Iterator iterator = *this;
			++*this;
			return *this;
		}

		Iterator& operator--()
		{
			--m_Ptr;
			return this;
		}

		Iterator operator--(int)
		{
			Iterator iterator = *this;
			--*this;
			return *this;
		}

		Iterator& operator--() const
		{
			--m_Ptr;
			return this;
		}

		Iterator operator--(int) const
		{
			Iterator iterator = *this;
			--*this;
			return *this;
		}

		ReferenceType operator[](size_t index)
		{
			return *(m_Ptr + index);
		}

		ReferenceType operator[](size_t index) const
		{
			return *(m_Ptr + index);
		}

		PointerType operator->()
		{
			return m_Ptr;
		}

		ReferenceType operator*()
		{
			return *m_Ptr;
		}

		bool operator==(const Iterator& other) const
		{
			return m_Ptr == other.m_Ptr;
		}

		bool operator!=(const Iterator& other) const
		{
			return !(*this == other);
		}

	private:
		PointerType m_Ptr = nullptr;
	};
}
