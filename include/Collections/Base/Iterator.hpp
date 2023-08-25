#pragma once
#include "Core/Core.hpp"
#include "Utility/Node.hpp"

namespace Micro
{
	template <typename T>
	class ContiguousIterator final
	{
	public:
		constexpr explicit ContiguousIterator(T* ptr) noexcept
			: m_Ptr(ptr)
		{
		}

		constexpr ContiguousIterator& operator++() noexcept
		{
			++m_Ptr;
			return *this;
		}

		constexpr ContiguousIterator operator++(int) noexcept
		{
			ContiguousIterator iterator = *this;
			++*this;
			return *this;
		}

		constexpr ContiguousIterator& operator++() const noexcept
		{
			++m_Ptr;
			return *this;
		}

		constexpr ContiguousIterator operator++(int) const noexcept
		{
			ContiguousIterator iterator = *this;
			++*this;
			return *this;
		}

		constexpr ContiguousIterator& operator--() noexcept
		{
			--m_Ptr;
			return this;
		}

		constexpr ContiguousIterator operator--(int) noexcept
		{
			ContiguousIterator iterator = *this;
			--*this;
			return *this;
		}

		constexpr ContiguousIterator& operator--() const noexcept
		{
			--m_Ptr;
			return this;
		}

		constexpr ContiguousIterator operator--(int) const noexcept
		{
			ContiguousIterator iterator = *this;
			--*this;
			return *this;
		}

		constexpr T& operator[](size_t index)
		{
			return *(m_Ptr + index);
		}

		constexpr const T& operator[](size_t index) const
		{
			return *(m_Ptr + index);
		}

		constexpr T* operator->() noexcept
		{
			return m_Ptr;
		}

		constexpr const T* operator->() const noexcept
		{
			return m_Ptr;
		}

		constexpr T& operator*() noexcept
		{
			return *m_Ptr;
		}

		constexpr const T& operator*() const noexcept
		{
			return *m_Ptr;
		}

		NODISCARD constexpr bool operator==(const ContiguousIterator& other) const noexcept
		{
			return m_Ptr == other.m_Ptr;
		}

		NODISCARD constexpr bool operator!=(const ContiguousIterator& other) const noexcept
		{
			return !(*this == other);
		}

	private:
		T* m_Ptr = nullptr;
	};

	template <typename T, IsNode TNode>
	class SegmentedIterator final
	{
	public:
		using Node = TNode;

		explicit constexpr SegmentedIterator(Node* ptr) noexcept
			: m_Node(ptr)
		{
		}

		constexpr SegmentedIterator& operator++() noexcept
		{
			m_Node = m_Node->Next;
			return *this;
		}

		constexpr SegmentedIterator operator++(int) noexcept
		{
			SegmentedIterator iterator = *this;
			++*this;
			return *this;
		}

		constexpr SegmentedIterator& operator++() const noexcept
		{
			m_Node = m_Node->Next;
			return *this;
		}

		constexpr SegmentedIterator operator++(int) const noexcept
		{
			SegmentedIterator iterator = *this;
			++*this;
			return *this;
		}

		constexpr SegmentedIterator& operator--() noexcept
		{
			m_Node = m_Node->Prev;
			return this;
		}

		constexpr SegmentedIterator operator--(int) noexcept
		{
			SegmentedIterator iterator = *this;
			--*this;
			return *this;
		}

		constexpr SegmentedIterator& operator--() const noexcept
		{
			m_Node = m_Node->Prev;
			return this;
		}

		constexpr SegmentedIterator operator--(int) const noexcept
		{
			SegmentedIterator iterator = *this;
			--*this;
			return *this;
		}

		constexpr T& operator[](const size_t index)
		{
			auto node = m_Node;
			for (size_t i = 1; i <= index; i++)
				node = node->Next;

			return node->Value;
		}

		constexpr const T& operator[](const size_t index) const
		{
			auto node = m_Node;
			for (size_t i = 1; i <= index; i++)
				node = node->Next;

			return node->Value;
		}

		constexpr T* operator->() noexcept
		{
			return &m_Node->Value;
		}

		constexpr T& operator*() noexcept
		{
			return m_Node->Value;
		}

		NODISCARD constexpr bool operator==(const SegmentedIterator& other) const noexcept
		{
			return m_Node == other.m_Node;
		}

		NODISCARD constexpr bool operator!=(const SegmentedIterator& other) const noexcept
		{
			return !(*this == other);
		}

	private:
		Node* m_Node = nullptr;
	};

	template <typename TMetaData>
	class HashIterator final
	{
	public:
		// Aliases
		using MetaData = TMetaData;
		using IteratorType = typename MetaData::IteratorType;

		explicit constexpr HashIterator(MetaData* ptr) noexcept
			: m_Node(ptr)
		{
		}

		constexpr HashIterator& operator++() noexcept
		{
			m_Node = m_Node->Next;
			return *this;
		}

		constexpr HashIterator operator++(int) noexcept
		{
			SegmentedIterator iterator = *this;
			++*this;
			return *this;
		}

		constexpr HashIterator& operator++() const noexcept
		{
			m_Node = m_Node->Next;
			return *this;
		}

		constexpr HashIterator operator++(int) const noexcept
		{
			SegmentedIterator iterator = *this;
			++*this;
			return *this;
		}

		constexpr IteratorType* operator->() noexcept
		{
			return &m_Node->BucketReference->GetIteratorValue();
		}

		constexpr IteratorType& operator*() noexcept
		{
			return m_Node->BucketReference->GetIteratorValue();
		}

		NODISCARD constexpr bool operator==(const HashIterator& other) const noexcept
		{
			return m_Node == other.m_Node;
		}

		NODISCARD constexpr bool operator!=(const HashIterator& other) const noexcept
		{
			return !(*this == other);
		}

	private:
		MetaData* m_Node = nullptr;
	};
}
