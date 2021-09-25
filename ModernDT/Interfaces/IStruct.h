#pragma once

#include <ostream>
#include <iostream>

#include "IContainer.h"

namespace mdt {

	template<typename DataContainer>
	class ContainerIterator
	{
	public:
		using ValueType = typename DataContainer::ValueType;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;

	public:
		ContainerIterator(PointerType _ptr)
			: m_Ptr(_ptr) {}

		ContainerIterator& operator++()
		{
			m_Ptr++;
			return *this;
		}

		ContainerIterator operator++(int)
		{
			ContainerIterator iterator = *this;
			++(*this);
			return ContainerIterator;
		}

		ContainerIterator& operator--()
		{
			m_Ptr--;
			return *this;
		}

		ContainerIterator operator--(int)
		{
			ContainerIterator iterator = *this;
			--(*this);
			return ContainerIterator;
		}

		ReferenceType operator[](size_t index)
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

		bool operator==(const ContainerIterator& other) const
		{
			return m_Ptr == other.m_Ptr;
		}

		bool operator!=(const ContainerIterator& other) const
		{
			return !(*this == other);
		}

	private:
		PointerType m_Ptr = nullptr;
	};

	template<typename _Type>
	class DataContainer
	{
	public:
		using ValueType = _Type;
		using Iterator = ContainerIterator<DataContainer<_Type>>;

	public:

		// Utility
		virtual bool Add(const _Type& _value) = 0;
		virtual bool Add(_Type&& _value) = 0;
		virtual bool AddRange(const IContainer<_Type>& _container) = 0;
		virtual bool Remove(const _Type& _value) = 0;
		virtual bool Remove(_Type&& _value) = 0;
		virtual bool RemoveAt(size_t _index) = 0;
		virtual bool Contains(const _Type& _value) const = 0;
		virtual bool Contains(_Type&& _value) const = 0;
		virtual void Clear() = 0;

		// Accessors
		constexpr inline virtual size_t Size() const { return m_Size; }

		// Iterator
		constexpr virtual Iterator begin() = 0;
		constexpr virtual Iterator end() = 0;

	protected:
		size_t m_Size = 0;

#define _SIZE	__super::m_Size
	};
}