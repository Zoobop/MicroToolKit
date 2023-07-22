#pragma once
#include <ostream>

#include "Experimental/Enumerable.hpp"
#include "Core/Memory/Allocator.hpp"
#include "Collections/Base/Iterator.hpp"
#include "Collections/Base/Sequence.hpp"

namespace Micro
{
	template <typename T>
#ifdef EXPERIMENTAL
	class Collection : public Enumerable<T>
#else
	class Collection
#endif
	{
	public:
		constexpr Collection() noexcept = default;
		constexpr Collection(const Collection&) noexcept = default;
		constexpr Collection(Collection&&) noexcept = default;
#ifdef EXPERIMENTAL
		constexpr ~Collection() noexcept override = default;
#else
		constexpr virtual ~Collection() noexcept = default;
#endif
		

		NODISCARD constexpr virtual const T* Data() const noexcept = 0;
		NODISCARD constexpr virtual size_t Capacity() const noexcept = 0;

		constexpr Collection& operator=(const Collection&) noexcept = default;
		constexpr Collection& operator=(Collection&&) noexcept = default;
	};

	template <typename T, typename TAllocator = Allocator<T>>
	class ContiguousCollection : public Collection<T>
	{
	public:
		// Aliases
		using Iterator = ContiguousIterator<T>;
		using ConstIterator = const Iterator;
		using Memory = Memory<T>;
		using AllocatorProxy = AllocatorProxy<T, TAllocator>;
		using Sequence = Sequence<T>;

		// Constructors/Destructor
		constexpr ContiguousCollection() noexcept = default;

		ContiguousCollection(const ContiguousCollection& other)
		{
			if (other.m_Data == nullptr)
				return;

			Allocate(other.m_Capacity);

			auto result = Copy(m_Data, m_Capacity, other.m_Data, other.m_Capacity);
			if (result == Error)
				throw BadCopyException();
		}

		ContiguousCollection(ContiguousCollection&& other) noexcept
			: m_Data(other.m_Data), m_Size(other.m_Size), m_Capacity(other.m_Capacity)
		{
			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;
		}

		ContiguousCollection(const Sequence& other)
		{
			if (other.m_Data == nullptr)
				return;

			Allocate(other.m_Capacity);

			auto result = Copy(m_Data, m_Capacity, other.m_Data, other.m_Capacity);
			if (result == Error)
				throw BadCopyException();
		}

		ContiguousCollection(Sequence&& other) noexcept
			: m_Data(other.m_Data), m_Size(other.m_Capacity), m_Capacity(other.m_Capacity)
		{
			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;
		}

		ContiguousCollection(std::initializer_list<T>&& initializerList) noexcept
		{
			const size_t length = initializerList.size();
			if (length == 0)
				return;

			Allocate(length);

			for (auto& elem : initializerList)
				m_Data[m_Size++] = std::move(const_cast<T&>(elem));
		}

		explicit ContiguousCollection(std::convertible_to<T> auto... elements) noexcept
		{
			// Get number of elements (arg count)
			const size_t length = sizeof ...(elements);

			Allocate(length);

			// Move values in data block
			for (auto values = {elements...}; auto&& elem : values)
				new(&m_Data[m_Size++]) T(std::move(elem));
		}

		~ContiguousCollection() override
		{
			// Invalidate memory, then free
			AllocatorProxy::ClearMemory(m_Data, m_Size);
			AllocatorProxy::Dispose(m_Data, m_Capacity);
		}

		// Utility
		virtual void Clear()
		{
			// Invalidate data
			AllocatorProxy::ClearMemory(m_Data, m_Size);
			m_Size = 0;
		}

		NODISCARD virtual bool Contains(const T& value)
		{
			for (size_t i = 0; i < m_Size; i++)
				if (m_Data[i] == value)
					return true;

			return false;
		}

		// Accessors
		NODISCARD constexpr bool IsEmpty() const { return m_Size == 0; }
		NODISCARD constexpr const T* Data() const noexcept override { return m_Data; }
		NODISCARD constexpr size_t Size() const { return m_Size; }
		NODISCARD constexpr size_t Capacity() const noexcept override { return m_Capacity; }
		NODISCARD constexpr Sequence AsSequence() const noexcept { return {m_Data, m_Size}; }

#ifdef EXPERIMENTAL
		// Enumerators
		NODISCARD Enumerator<T> GetEnumerator() override
		{
			for (size_t i = 0; i < m_Size; i++)
			{
				auto& element = m_Data[i];
				co_yield element;
			}
		}

		NODISCARD Enumerator<T> GetEnumerator() const override
		{
			for (size_t i = 0; i < m_Size; i++)
			{
				const auto& element = m_Data[i];
				co_yield element;
			}
		}
#else		
		// Iterators
		NODISCARD constexpr Iterator begin() const noexcept { return Iterator(m_Data); }
		NODISCARD constexpr Iterator end() const noexcept { return Iterator(m_Data + m_Size); }
		NODISCARD constexpr ConstIterator cbegin() const noexcept { return Iterator(m_Data); }
		NODISCARD constexpr ConstIterator cend() const noexcept { return Iterator(m_Data + m_Size); }
#endif

		// Operator Overloads
		NODISCARD explicit constexpr operator Sequence() const noexcept { return AsSequence(); }

		ContiguousCollection& operator=(const ContiguousCollection& other)
		{
			// Validation
			if (this == &other)
				return *this;

			if (other.m_Capacity == 0)
				return *this;

			// Allocation
			if (m_Data.IsValidMemory())
				Reallocate(other.m_Capacity);
			else
				Allocate(other.m_Capacity);

			// Assignment
			auto result = Copy(m_Data, m_Capacity, other.m_Data, other.m_Capacity);
			if (result == Error)
				throw BadCopyException();

			return *this;
		}

		ContiguousCollection& operator=(ContiguousCollection&& other) noexcept
		{
			if (this == &other)
				return *this;

			m_Data = other.m_Data;
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;

			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const ContiguousCollection& current)
		{
			stream << "[";
			for (size_t i = 0; i < current.m_Size; i++)
			{
				stream << current.m_Data[i];
				if (i != current.m_Size - 1)
					stream << ", ";
			}

			stream << "]";
			return stream;
		}

	protected:
		void Allocate(const size_t capacity)
		{
			m_Capacity = AllocatorProxy::Allocate(m_Data, m_Capacity, capacity);
		}

		void Reallocate(const size_t capacity)
		{
			m_Capacity = AllocatorProxy::Reallocate(m_Data, m_Capacity, capacity);
		}

	protected:
		Memory m_Data = nullptr;
		size_t m_Size = 0;
		size_t m_Capacity = 0;
	};

	template <typename T, size_t TSize>
	class FixedCollection : public Collection<T>
	{
	public:
		// Aliases
		using Iterator = ContiguousIterator<T>;
		using ConstIterator = const Iterator;
		using Memory = Memory<T>;
		using Sequence = Sequence<T>;

		// Constructors/Destructor
		constexpr FixedCollection() noexcept = default;

		FixedCollection(const FixedCollection& other)
		{
			for (size_t i = 0; i < TSize; i++)
				new(&m_Data[i]) T(other.m_Data[i]);
		}

		FixedCollection(FixedCollection&& other) noexcept
		{
			for (size_t i = 0; i < TSize; i++)
				new(&m_Data[i]) T(std::move(other.m_Data[i]));
		}

		FixedCollection(std::initializer_list<T>&& initializerList) noexcept
		{
			if (initializerList.size() == 0)
				return;

			size_t index = 0;
			for (auto& elem : initializerList)
			{
				if (index == TSize)
					return;

				new(&m_Data[index++]) T(std::move(const_cast<T&>(elem)));
			}
		}

		explicit FixedCollection(std::convertible_to<T> auto... elements) noexcept
		{
			// Move values in data block
			size_t index = 0;
			for (auto values = {elements...}; auto&& elem : values)
			{
				if (index == TSize)
					return;

				new(&m_Data[index++]) T(std::move(elem));
			}
		}

		constexpr ~FixedCollection() noexcept override = default;

		// Accessors
		NODISCARD constexpr const T* Data() const noexcept override { return static_cast<const T*>(&m_Data); }
		NODISCARD constexpr size_t Capacity() const noexcept override { return TSize; }
		NODISCARD constexpr Sequence AsSequence() const noexcept { return {&m_Data[0], TSize}; }

#ifdef EXPERIMENTAL
		// Enumerators
		NODISCARD Enumerator<T> GetEnumerator() override
		{
			for (size_t i = 0; i < TSize; i++)
			{
				auto& element = m_Data[i];
				co_yield element;
			}
		}

		NODISCARD Enumerator<T> GetEnumerator() const override
		{
			for (size_t i = 0; i < TSize; i++)
			{
				const auto& element = m_Data[i];
				co_yield element;
			}
		}
#else		
		// Iterators
		NODISCARD constexpr Iterator begin() const noexcept { return Iterator(m_Data); }
		NODISCARD constexpr Iterator end() const noexcept { return Iterator(m_Data + TSize); }
		NODISCARD constexpr ConstIterator cbegin() const noexcept { return Iterator(m_Data); }
		NODISCARD constexpr ConstIterator cend() const noexcept { return Iterator(m_Data + TSize); }
#endif

		// Operator Overloads
		NODISCARD explicit constexpr operator Sequence() const noexcept { return AsSequence(); }

		FixedCollection& operator=(const FixedCollection& other)
		{
			// Validation
			if (this == &other)
				return *this;

			CopyFrom(other);

			return *this;
		}

		FixedCollection& operator=(FixedCollection&& other) noexcept
		{
			if (this == &other)
				return *this;

			MoveFrom(std::move(other));

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& stream, const FixedCollection& current)
		{
			stream << "[";
			for (size_t i = 0; i < TSize; i++)
			{
				stream << current.m_Data[i];
				if (i != TSize - 1)
					stream << ", ";
			}

			stream << "]";
			return stream;
		}

	protected:
		void CopyFrom(const FixedCollection& other) noexcept
		{
			for (size_t i = 0; i < TSize; i++)
				m_Data[i] = other.m_Data[i];
		}

		void MoveFrom(FixedCollection&& other) noexcept
		{
			for (size_t i = 0; i < TSize; i++)
				m_Data[i] = std::move(other.m_Data[i]);
		}

	protected:
		T m_Data[TSize]{};
	};
}
